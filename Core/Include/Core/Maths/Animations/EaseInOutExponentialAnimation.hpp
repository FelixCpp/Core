#pragma once

/// <summary>
/// Core
/// </summary>
#include <Core/Maths/Animations/Animation.hpp>

namespace Core
{

	template<typename T>
	class EaseInOutExponentialAnimation : public Animation<T> {
	public:

		/// <summary>
		/// Inherit the constructor
		/// </summary>
		using Animation<T>::Animation;

	protected:

		/// <summary>
		/// A method to compute the value based on the start,
		/// end, duration and time complexity
		/// </summary>
		/// <param name="time">the time to compute the value based on</param>
		/// <returns>The calculation result</returns>
		virtual T ComputeValue(T time) const
		{
			//compute the current time relative to the midpoint
			time = time / (this->duration / 2);

			//if we haven't reached the midpoint, we want to do the ease-in portion
			if (time < 1)
			{
				return this->start + (this->end - this->start) / 2.f * std::powf(2.f, 10.f * (time - 1.f));
			}

			//otherwise, do the ease-out portion
			return this->start + (this->end - this->start) / 2.f * (-std::powf(2.f, -10.f * --time) + 2.f);
		}

	};

}