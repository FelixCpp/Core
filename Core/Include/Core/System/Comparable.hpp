#pragma once

#include <type_traits>

namespace Core
{

	template<class Base>
	class Comparable {
	public:

		/// <summary>
		/// Default constructor
		/// </summary>
		Comparable() = default;

		/// <summary>
		/// Default virtual destructor
		/// </summary>
		virtual ~Comparable() = default;

		/// <summary>
		/// Compares two objects against each other.
		/// </summary>
		/// <param name="other">the object to compare against</param>
		/// <returns>
		///		-1: If the current object is less than the parameter
		///		 0: If the current object equals the parameter
		///		 1: If the current object is greater than the parameter
		/// </returns>
		virtual Int32 Compare(const Base & other) const = 0;

		/// <summary>
		/// Comparing operators
		/// </summary>

		bool operator == (const Base & other) const { return this->Compare(other) == 0; }
		bool operator != (const Base & other) const { return this->Compare(other) != 0; }
		bool operator <= (const Base & other) const { return this->Compare(other) <= 0; }
		bool operator >= (const Base & other) const { return this->Compare(other) >= 0; }
		bool operator < (const Base & other) const  { return this->Compare(other) < 0; }
		bool operator > (const Base & other) const  { return this->Compare(other) > 0; }

	};

}