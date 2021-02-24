#pragma once

#include <Core/Maths/Math.hpp>

namespace Core
{

	/// <summary>
	/// This class is the base class for any animation class
	/// </summary>
	template<typename T>
	class Animation {
	public:

		explicit Animation(T start = 0, T end = 0, T duration = 0) :
			start(start), end(end), duration(duration)
		{ }

		/// <summary>
		/// Evaluates the value based on the formular overriden
		/// by the subclasses
		/// </summary>
		/// <param name="time">the time to compute the value based on</param>
		T Evaluate(T time) const
		{
			const T clampedTime = MathImpl<T>::Constrain(time, this->start, this->end);
			return this->ComputeValue(clamedTime);
		}

	protected:

		/// <summary>
		/// A method to compute the value based on the start,
		/// end, duration and time complexity
		/// </summary>
		/// <param name="time">the time to compute the value based on</param>
		/// <returns>The calculation result</returns>
		virtual T ComputeValue(T time) const = 0;

	public:

		/// <summary>
		/// The starting value of the animation
		/// </summary>
		T start;
		
		/// <summary>
		/// The end value of the animation
		/// </summary>
		T end;
		
		/// <summary>
		/// The duration. This value is most likely a positive value
		/// </summary>
		T duration;

	};

}