#pragma once

#include <Core/System/Datatypes.hpp>

namespace Core
{
	
	class Duration {
	public:

		Duration();

		float toSeconds() const;
		i32_t toMilliseconds() const;
		i64_t toMicroseconds() const;

		static Duration fromSeconds(float seconds);
		static Duration fromMilliseconds(i32_t milliseconds);
		static Duration fromMicroseconds(i64_t microseconds);

		static const Duration Zero;

		bool operator==(const Duration & other) const;
		bool operator!=(const Duration & other) const;
		bool operator>=(const Duration & other) const;
		bool operator<=(const Duration & other) const;
		bool operator>(const Duration & other) const;
		bool operator<(const Duration & other) const;

		Duration operator+(const Duration & other) const;
		Duration operator-(const Duration & other) const;
		Duration operator/(const Duration & other) const;
		Duration operator*(const Duration & other) const;
		Duration operator%(const Duration & other) const;

		Duration & operator+=(const Duration & other);
		Duration & operator-=(const Duration & other);
		Duration & operator/=(const Duration & other);
		Duration & operator*=(const Duration & other);
		Duration & operator%=(const Duration & other);

	private:

		explicit Duration(i64_t microseconds);
	
	private:

		i64_t microseconds;

	};

}