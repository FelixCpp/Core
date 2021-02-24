#pragma once

/// <summary>
/// Core
/// </summary>
#include <Core/Maths/Animations/Animation.hpp>

namespace Core
{

	template<typename T>
	class EaseOutExponentialAnimation : public Animation<T> {
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
			return this->start + (this->end - this->start) * (-std::powf(2.f, -10.f * time / this->duration) + 1.f);
		}

	};

}