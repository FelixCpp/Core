// 
// State.hpp
// Core
// 
// Created by Felix Busch on 28.03.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#pragma once

#include <Core/System/Time.hpp>

#include <optional>
#include <algorithm>
#include <memory>

namespace Core
{
	////////////////////////////////////////////////////////////
	/// \brief Define container object for timed animations
	///		   such as any easing-animation.
	/// 
	////////////////////////////////////////////////////////////
	class TimedAnimation
	{
	public:

		////////////////////////////////////////////////////////////
		/// Pre-defined constants
		/// 
		////////////////////////////////////////////////////////////
		inline static constexpr Time DefaultDuration = Seconds(1.0f);

	public:

		////////////////////////////////////////////////////////////
		/// \brief Default constructor.
		///
		///	\param duration The duration it takes to finish the
		///					animation
		/// 
		////////////////////////////////////////////////////////////
		constexpr TimedAnimation(const Time& duration = DefaultDuration) :
			duration(duration.ToSeconds<f32>()),
			elapsedTime(0.0f)
		{
		}

		void Reset()
		{
			elapsedTime = 0.0f;
		}

		bool IsDone() const
		{
			return elapsedTime >= duration;
		}

	protected:

		////////////////////////////////////////////////////////////
		/// Member data
		/// 
		////////////////////////////////////////////////////////////
		f32 duration;		///< The duration in seconds
		f32 elapsedTime;	///< The elapsed time since starting the animation in seconds

	};

	////////////////////////////////////////////////////////////
	/// \brief Define linear animation class
	/// 
	////////////////////////////////////////////////////////////
	class LinearAnimation : public TimedAnimation
	{
	public:

		using TimedAnimation::TimedAnimation;

		template<typename T>
		constexpr T Update(const T& initial, const T& destination, f32 deltaTime)
		{
			elapsedTime += deltaTime;
			const f32 x = std::clamp(elapsedTime / duration, 0.0f, 1.0f);
			return initial * (1.0f - x) + destination * x;
		}
		
	};

	////////////////////////////////////////////////////////////
	/// \brief Define null animation class. This is not really
	///		   an animation but a way to move to the destination
	///		   value immediately.
	/// 
	////////////////////////////////////////////////////////////
	class NullAnimation
	{
	public:

		////////////////////////////////////////////////////////////
		/// \brief Return the destination value immediately.
		/// 
		////////////////////////////////////////////////////////////
		template<typename T>
		constexpr T Update(const T& initial, const T& destination, f32 deltaTime)
		{
			return destination;
		}

		////////////////////////////////////////////////////////////
		/// \brief Nothing to do here.
		/// 
		////////////////////////////////////////////////////////////
		void Reset()
		{
		}

		////////////////////////////////////////////////////////////
		/// \brief We're always done since we don't animate.
		/// 
		////////////////////////////////////////////////////////////
		bool IsDone() const
		{
			return true;
		}

	};

	template<typename TAnimation>
	class RepeatForeverAnimation
	{
	public:

		constexpr RepeatForeverAnimation(TAnimation animation):
			animation(std::move(animation))
		{}

		template<typename T>
		constexpr T Update(const T& initial, const T& destination, f32 deltaTime)
		{
			if(animation.IsDone())
			{
				animation.Reset();
			}

			return animation.Update(initial, destination, deltaTime);
		}

		constexpr bool IsDone() const
		{
			return false;
		}

	private:
		
		TAnimation animation;

	};

	////////////////////////////////////////////////////////////
	/// \brief Define Animation wrapper class
	/// 
	////////////////////////////////////////////////////////////
	template<typename TAnimation>
	class Animator
	{
	public:

		////////////////////////////////////////////////////////////
		/// Type definitions
		/// 
		////////////////////////////////////////////////////////////
		using AnimationType = TAnimation;

		////////////////////////////////////////////////////////////
		/// \brief Define explicit constructor that creates an animator
		///		   associated to an animation.
		/// 
		////////////////////////////////////////////////////////////
		explicit constexpr Animator(AnimationType animation):
			Animation(std::move(animation))
		{
		}

		constexpr Animator<RepeatForeverAnimation<TAnimation>> RepeatForever() const
		{
			return Animator<RepeatForeverAnimation<TAnimation>>(Animation);
		}

	public:

		////////////////////////////////////////////////////////////
		/// Member data
		/// 
		////////////////////////////////////////////////////////////
		AnimationType Animation;

	};

	////////////////////////////////////////////////////////////
	/// \brief Create linear-animation animator
	/// 
	////////////////////////////////////////////////////////////
	constexpr Animator<LinearAnimation> Linear(const Time& duration = TimedAnimation::DefaultDuration)
	{
		return Animator<LinearAnimation>(duration);
	}

	////////////////////////////////////////////////////////////
	/// \brief Create linear-animation animator
	/// 
	////////////////////////////////////////////////////////////
	constexpr Animator<NullAnimation> Null()
	{
		return Animator<NullAnimation>({});
	}

	////////////////////////////////////////////////////////////
	/// \brief Define animatable value wrapper.
	/// 
	////////////////////////////////////////////////////////////
	template<typename TValue>
	class Animatable
	{
	public:

		////////////////////////////////////////////////////////////
		/// \brief Define animation wrapper that defined the update
		///		   method as abstract member.
		/// 
		////////////////////////////////////////////////////////////
		class AnimationWrapper
		{
		public:

			virtual ~AnimationWrapper() = default;
			virtual TValue Update(const TValue& initial, const TValue& destination, f32 deltaTime) = 0;
			virtual bool IsDone() const = 0;

		};
		
		template<typename TAnimation>
		class AnimationContainer final : public AnimationWrapper
		{
		public:

			constexpr AnimationContainer(TAnimation animation):
				animation(std::move(animation))
			{}

			virtual TValue Update(const TValue& initial, const TValue& destination, f32 deltaTime) override
			{
				return animation.Update(initial, destination, deltaTime);
			}
			
			virtual bool IsDone() const override
			{
				return animation.IsDone();
			}

		private:

			////////////////////////////////////////////////////////////
			/// Member data
			/// 
			////////////////////////////////////////////////////////////
			TAnimation animation;

		};

		////////////////////////////////////////////////////////////
		/// Type definitions.
		/// 
		////////////////////////////////////////////////////////////
		using Value			= TValue;

		////////////////////////////////////////////////////////////
		/// \brief Default constructor.
		/// 
		////////////////////////////////////////////////////////////
		Animatable():
			initialValue(Value{}),
			destinationValue(Value{}),
			currentValue(Value{}),
			animation(nullptr)
		{
		}

		////////////////////////////////////////////////////////////
		/// \brief Start an animation to the retrieved value from
		///		   \a function.
		/// 
		////////////////////////////////////////////////////////////
		template<typename TAnimation, typename TFunction, typename TResult = std::invoke_result_t<TFunction>>
		void WithAnimation(Animator<TAnimation> animator, const TFunction& function)
		{
			animation.reset(new AnimationContainer<TAnimation>(animator.Animation));
			destinationValue = function();
			initialValue = currentValue;
		}

		////////////////////////////////////////////////////////////
		/// \brief Get the current value.
		/// 
		////////////////////////////////////////////////////////////
		operator const Value&() const
		{
			return currentValue;
		}

		////////////////////////////////////////////////////////////
		/// \brief Update the animation.
		/// 
		////////////////////////////////////////////////////////////
		void Update(f32 deltaTime)
		{
			// update the current value
			if (animation && !animation->IsDone())
			{
				currentValue = animation->Update(initialValue, destinationValue, deltaTime);
			}
		}

	private:

		////////////////////////////////////////////////////////////
		/// Member data
		/// 
		////////////////////////////////////////////////////////////
		Value initialValue;
		Value destinationValue;
		Value currentValue;

		std::unique_ptr<AnimationWrapper> animation;

	};

}