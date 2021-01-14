#pragma once

#include <Core/System/Datatypes.hpp>

namespace Core
{
	
	class Duration {
	public:

		Duration();

		float ToSeconds() const;
		i32_t ToMilliseconds() const;
		i64_t ToMicroseconds() const;

		static Duration FromSeconds(float seconds);
		static Duration FromMilliseconds(i32_t milliseconds);
		static Duration FromMicroseconds(i64_t microseconds);

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