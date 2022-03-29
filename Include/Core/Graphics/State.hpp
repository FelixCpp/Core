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

	class AnimationBase
	{
	public:

		constexpr AnimationBase():
			forwards(true)
		{}

		void OnReverse()
		{
			forwards = !forwards;
		}

		void OnCycleDone()
		{
		}

	protected:

		bool forwards;

	};

	class LinearAnimation : public AnimationBase
	{
	public:

		constexpr LinearAnimation(const Time& duration):
			duration(duration.ToSeconds<f32>()),
			elapsedTime(0.0f)
		{}

		template<typename TValue>
		TValue Update(const TValue& initial, const TValue& destination, f32 deltaTime)
		{
			elapsedTime += deltaTime;
			const f32 progress = std::clamp(elapsedTime / duration, 0.0f, 1.0f);
			const f32 percentage = forwards ? progress : (1.0f - progress);
			return initial * (1.0f - percentage) + destination * percentage;
		}

		bool IsDone() const
		{
			return elapsedTime >= duration;
		}

		void OnRepeat()
		{
			elapsedTime = 0.0f;
		}
		
	private:
		
		f32 duration;
		f32 elapsedTime;

	};

	template<typename TAnimation, typename TPredicate>
	class RepeatAnimation
	{
	public:
	
		constexpr RepeatAnimation(TAnimation animation, TPredicate predicate):
			animation(std::move(animation)), predicate(std::move(predicate)), repetitions(1)
		{
		}
	
		template<typename TValue>
		TValue Update(const TValue& initial, const TValue& destination, f32 deltaTime)
		{
			return animation.Update(initial, destination, deltaTime);
		}

		void OnCycleDone()
		{
			OnRepeat();
			animation.OnCycleDone();
		}

		void OnRepeat()
		{
			if (predicate(repetitions))
			{
				animation.OnRepeat();
				++repetitions;
			}
		}

		void OnReverse()
		{
			animation.OnReverse();
		}

		bool IsDone() const
		{
			return animation.IsDone();
		}

	private:
	
		TAnimation	animation;
		TPredicate	predicate;
		u32			repetitions;
	
	};

	template<typename TAnimation, typename TPredicate>
	class ReverseAnimation
	{
	public:

		constexpr ReverseAnimation(TAnimation animation, TPredicate predicate) :
			animation(std::move(animation)), predicate(std::move(predicate)), reversions(1)
		{
		}

		template<typename TValue>
		TValue Update(const TValue& initial, const TValue& destination, f32 deltaTime)
		{
			return animation.Update(initial, destination, deltaTime);
		}

		void OnCycleDone()
		{
			OnReverse();
			animation.OnCycleDone();
		}

		void OnRepeat()
		{
			animation.OnRepeat();
		}

		void OnReverse()
		{
			if (predicate(reversions))
			{
				animation.OnReverse();
				++reversions;
			}
		}

		bool IsDone() const
		{
			return animation.IsDone();
		}

	private:

		TAnimation	animation;
		TPredicate	predicate;
		u32			reversions;

	};

	template<typename TAnimation>
	class DelayAnimation
	{
	public:

		constexpr DelayAnimation(TAnimation animation, const Time& delay):
			animation(std::move(animation)), delay(delay.ToSeconds<float>()), elapsedTime(0.0f)
		{
		}

		template<typename TValue>
		TValue Update(const TValue& initial, const TValue& destination, f32 deltaTime)
		{
			elapsedTime += deltaTime;

			if (elapsedTime < delay)
				return initial;

			return animation.Update(initial, destination, deltaTime);
		}

		void OnCycleDone()
		{
			animation.OnCycleDone();
		}

		void OnRepeat()
		{
			animation.OnRepeat();
		}

		void OnReverse()
		{
			animation.OnReverse();
		}

		bool IsDone() const
		{
			return animation.IsDone();
		}

	private:

		TAnimation	animation;
		f32			delay;
		f32			elapsedTime;
		
	};

	template<typename TAnimation>
	class SpeedAnimation
	{
	public:

		constexpr SpeedAnimation(TAnimation animation, f32 speed) :
			animation(std::move(animation)), speed(speed)
		{
		}

		template<typename TValue>
		TValue Update(const TValue& initial, const TValue& destination, f32 deltaTime)
		{
			return animation.Update(initial, destination, deltaTime * speed);
		}

		void OnCycleDone()
		{
			animation.OnCycleDone();
		}

		void OnRepeat()
		{
			animation.OnRepeat();
		}

		void OnReverse()
		{
			animation.OnReverse();
		}

		bool IsDone() const
		{
			return animation.IsDone();
		}

	private:

		TAnimation	animation;
		f32			speed;

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

		template<typename TPredicate>
		constexpr auto Repeat(TPredicate predicate) const
		{
			return Animator<RepeatAnimation<TAnimation, TPredicate>>({ Animation, std::move(predicate) });
		}

		constexpr auto RepeatForever() const
		{
			static constexpr auto TruePredicate = [](u32 _) { return true; };
			return Animator<RepeatAnimation<TAnimation, decltype(TruePredicate)>>({Animation, TruePredicate });
		}

		template<typename TPredicate>
		constexpr auto Reverse(TPredicate predicate) const
		{
			return Animator<ReverseAnimation<TAnimation, TPredicate>>({ Animation, std::move(predicate) });
		}

		constexpr auto ReverseForever() const
		{
			static constexpr auto TruePredicate = [](u32 _) { return true; };
			return Animator<ReverseAnimation<TAnimation, decltype(TruePredicate)>>({ Animation, TruePredicate });
		}

		constexpr auto Delay(const Time& delay) const
		{
			return Animator<DelayAnimation<TAnimation>>({ Animation, delay });
		}

		constexpr auto Speed(f32 speed) const
		{
			return Animator<SpeedAnimation<TAnimation>>({ Animation, speed });
		}

	public:

		////////////////////////////////////////////////////////////
		/// Member data
		/// 
		////////////////////////////////////////////////////////////
		AnimationType Animation;

	};

	constexpr auto Linear(const Time& duration)
	{
		return Animator<LinearAnimation>(duration);
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
			virtual void OnCycleDone() = 0;
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

			virtual void OnCycleDone() override
			{
				animation.OnCycleDone();
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
			animating(false),
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
			animating = true;
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
			if(animation && animating)
			{
				if(animation->IsDone())
				{
					animation->OnCycleDone();
				}

				if(animation->IsDone())
				{
					animating = false;
					currentValue = destinationValue;
				}
				else
				{
					currentValue = animation->Update(initialValue, destinationValue, deltaTime);
				}
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
		bool  animating;

		std::unique_ptr<AnimationWrapper> animation;

	};

}