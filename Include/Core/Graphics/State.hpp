// 
// State.hpp
// Core
// 
// Created by Felix Busch on 28.03.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#pragma once

#include <Core/System/Time.hpp>
#include <Core/System/Types.hpp>

#include <optional>
#include <algorithm>
#include <memory>

#define CORE_DECLARE_ANIMATION(className, functionName, easeFunctionName)\
	class className final : public Core::TimedAnimation {\
	public:\
		using TimedAnimation::TimedAnimation;\
		virtual f32 GetPercentage(f32 x) const {\
			return (easeFunctionName)(x);\
		}\
	};\
	constexpr auto functionName(const Core::Time& duration = TimedAnimation::DefaultDuration) {\
		return Core::Animator<className>(duration);\
	}
	

namespace Core
{
	////////////////////////////////////////////////////////////
	/// \brief Easing functions
	/// 
	////////////////////////////////////////////////////////////
	namespace Ease
	{
		f32 Linear(f32 x);
		f32 InSine(f32 x);
		f32 OutSine(f32 x);
		f32 InOutSine(f32 x);
		f32 InCubic(f32 x);
		f32 OutCubic(f32 x);
		f32 InOutCubic(f32 x);
		f32 InQuint(f32 x);
		f32 OutQuint(f32 x);
		f32 InOutQuint(f32 x);
		f32 InCirc(f32 x);
		f32 OutCirc(f32 x);
		f32 InOutCirc(f32 x);
		f32 InElastic(f32 x);
		f32 OutElastic(f32 x);
		f32 InOutElastic(f32 x);
		f32 InQuad(f32 x);
		f32 OutQuad(f32 x);
		f32 InOutQuad(f32 x);
		f32 InQuart(f32 x);
		f32 OutQuart(f32 x);
		f32 InOutQuart(f32 x);
		f32 InExpo(f32 x);
		f32 OutExpo(f32 x);
		f32 InOutExpo(f32 x);
		f32 InBack(f32 x);
		f32 OutBack(f32 x);
		f32 InOutBack(f32 x);
		f32 InBounce(f32 x);
		f32 OutBounce(f32 x);
		f32 InOutBounce(f32 x);
	}

	////////////////////////////////////////////////////////////
	/// \brief Define animation base class
	/// 
	////////////////////////////////////////////////////////////
	class AnimationBase
	{
	public:

		////////////////////////////////////////////////////////////
		/// \brief Default constructor.
		/// 
		////////////////////////////////////////////////////////////
		constexpr AnimationBase():
			forwards(true)
		{}

		////////////////////////////////////////////////////////////
		/// \brief Default destructor.
		/// 
		////////////////////////////////////////////////////////////
		virtual ~AnimationBase() = default;

		////////////////////////////////////////////////////////////
		/// \brief Reverses the direction.
		/// 
		////////////////////////////////////////////////////////////
		void OnReverse()
		{
			forwards = !forwards;
		}

		////////////////////////////////////////////////////////////
		/// \brief Declare method for any animations
		///		   that don't do anything in here.
		/// 
		////////////////////////////////////////////////////////////
		virtual void OnCycleDone()
		{
		}

	protected:

		////////////////////////////////////////////////////////////
		/// Member data
		/// 
		////////////////////////////////////////////////////////////
		bool forwards;	///< The direction indicated by a boolean

	};

	////////////////////////////////////////////////////////////
	/// \brief Define timed animation base class.
	///
	/// A TimedAnimation contains a duration to control
	///	how long the animation should take before finishing.
	///
	///	This class is intended to be used for easing animations
	///	of any kind. That includes EaseInSine as well as
	///	EaseInQuad.
	/// 
	////////////////////////////////////////////////////////////
	class TimedAnimation : public AnimationBase
	{
	public:

		////////////////////////////////////////////////////////////
		/// Pre-defined Constants
		/// 
		////////////////////////////////////////////////////////////
		inline static constexpr Time DefaultDuration = Seconds(1.0f);

	public:

		////////////////////////////////////////////////////////////
		/// \brief Construct a TimeAnimation object.
		///
		///	\param duration The duration this animation should take
		///	to finish.
		/// 
		////////////////////////////////////////////////////////////
		constexpr TimedAnimation(const Time& duration):
			duration(duration.ToSeconds<float>()),
			elapsedTime(0.0f)
		{
		}

		////////////////////////////////////////////////////////////
		/// \brief Default destructor
		/// 
		////////////////////////////////////////////////////////////
		virtual ~TimedAnimation() override = default;

		////////////////////////////////////////////////////////////
		/// \brief Compute the percentage and linearly interpolate
		///		   to the destination value.
		/// 
		////////////////////////////////////////////////////////////
		template<typename TValue>
		TValue Update(const TValue& initial, const TValue& destination, f32 deltaTime)
		{
			elapsedTime += deltaTime;
			const f32 progress = std::clamp(elapsedTime / duration, 0.0f, 1.0f);
			const f32 percentage = GetPercentage(forwards ? progress : (1.0f - progress));
			return initial * (1.0f - percentage) + destination * percentage;
		}

		////////////////////////////////////////////////////////////
		/// \brief Tell whether the animation has finished or is
		///		   still running.
		///
		/// A TimedAnimation is done, if the elapsed time
		///	is greater or equal than the requested duration.
		///
		///	\return True if the animation has finished animating,
		///			false otherwise.
		/// 
		////////////////////////////////////////////////////////////
		bool IsDone() const
		{
			return elapsedTime >= duration;
		}

		////////////////////////////////////////////////////////////
		/// \brief Resets the animation's state.
		///
		///	Reset the elapsed time back to zero.
		/// 
		////////////////////////////////////////////////////////////
		void OnRepeat()
		{
			elapsedTime = 0.0f;
		}

	protected:

		////////////////////////////////////////////////////////////
		/// \brief Abstract method that retrieves the percentage
		///		   amount that tells the animation how far to go.
		///
		///	\return The percentage amount where 0.0 is 0% and 1.0 is
		///			100%.
		/// 
		////////////////////////////////////////////////////////////
		virtual f32 GetPercentage(f32 x) const = 0;

	private:

		////////////////////////////////////////////////////////////
		/// Member data
		/// 
		////////////////////////////////////////////////////////////
		f32 duration;		///< The duration this animation takes to finish
		f32 elapsedTime;	///< The elapsed time since starting the animation

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

	////////////////////////////////////////////////////////////
	/// \brief Define easing animations
	/// 
	////////////////////////////////////////////////////////////
	CORE_DECLARE_ANIMATION(LinearAnimation, Linear, Ease::Linear);
	CORE_DECLARE_ANIMATION(EaseInSineAnimation, EaseInSine, Ease::InSine);
	CORE_DECLARE_ANIMATION(EaseOutSineAnimation, EaseOutSine, Ease::OutSine);
	CORE_DECLARE_ANIMATION(EaseInOutSineAnimation, EaseInOutSine, Ease::InOutSine);
	CORE_DECLARE_ANIMATION(EaseInCubicAnimation, EaseInCubic, Ease::InCubic);
	CORE_DECLARE_ANIMATION(EaseOutCubicAnimation, EaseOutCubic, Ease::OutCubic);
	CORE_DECLARE_ANIMATION(EaseInOutCubicAnimation, EaseInOutCubic, Ease::InOutCubic);
	CORE_DECLARE_ANIMATION(EaseInQuintAnimation, EaseInQuint, Ease::InQuint);
	CORE_DECLARE_ANIMATION(EaseOutQuintAnimation, EaseOutQuint, Ease::OutQuint);
	CORE_DECLARE_ANIMATION(EaseInOutQuintAnimation, EaseInOutQuint, Ease::InOutQuint);
	CORE_DECLARE_ANIMATION(EaseInCircAnimation, EaseInCirc, Ease::InCirc);
	CORE_DECLARE_ANIMATION(EaseOutCircAnimation, EaseOutCirc, Ease::OutCirc);
	CORE_DECLARE_ANIMATION(EaseInOutCircAnimation, EaseInOutCirc, Ease::InOutCirc);
	CORE_DECLARE_ANIMATION(EaseInElasticAnimation, EaseInElastic, Ease::InElastic);
	CORE_DECLARE_ANIMATION(EaseOutElasticAnimation, EaseOutElastic, Ease::OutElastic);
	CORE_DECLARE_ANIMATION(EaseInOutElasticAnimation, EaseInOutElastic, Ease::InOutElastic);
	CORE_DECLARE_ANIMATION(EaseInQuadAnimation, EaseInQuad, Ease::InQuad);
	CORE_DECLARE_ANIMATION(EaseOutQuadAnimation, EaseOutQuad, Ease::OutQuad);
	CORE_DECLARE_ANIMATION(EaseInOutQuadAnimation, EaseInOutQuad, Ease::InOutQuad);
	CORE_DECLARE_ANIMATION(EaseInQuartAnimation, EaseInQuart, Ease::InQuart);
	CORE_DECLARE_ANIMATION(EaseOutQuartAnimation, EaseOutQuart, Ease::OutQuart);
	CORE_DECLARE_ANIMATION(EaseInOutQuartAnimation, EaseInOutQuart, Ease::InOutQuart);
	CORE_DECLARE_ANIMATION(EaseInExpoAnimation, EaseInExpo, Ease::InExpo);
	CORE_DECLARE_ANIMATION(EaseOutExpoAnimation, EaseOutExpo, Ease::OutExpo);
	CORE_DECLARE_ANIMATION(EaseInOutExpoAnimation, EaseInOutExpo, Ease::InOutExpo);
	CORE_DECLARE_ANIMATION(EaseInBackAnimation, EaseInBack, Ease::InBack);
	CORE_DECLARE_ANIMATION(EaseOutBackAnimation, EaseOutBack, Ease::OutBack);
	CORE_DECLARE_ANIMATION(EaseInOutBackAnimation, EaseInOutBack, Ease::InOutBack);
	CORE_DECLARE_ANIMATION(EaseInBounceAnimation, EaseInBounce, Ease::InBounce);
	CORE_DECLARE_ANIMATION(EaseOutBounceAnimation, EaseOutBounce, Ease::OutBounce);
	CORE_DECLARE_ANIMATION(EaseInOutBounceAnimation, EaseInOutBounce, Ease::InOutBounce);

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