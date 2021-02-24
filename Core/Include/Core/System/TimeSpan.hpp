#pragma once

#include <Core/System/Datatypes.hpp>

namespace Core
{
	
	class TimeSpan {
	public:

		TimeSpan();

		float ToSeconds() const;
		i32_t ToMilliseconds() const;
		i64_t ToMicroseconds() const;

		static TimeSpan FromSeconds(float seconds);
		static TimeSpan FromMilliseconds(i32_t milliseconds);
		static TimeSpan FromMicroseconds(i64_t microseconds);

		static const TimeSpan Zero;

		bool operator==(const TimeSpan & other) const;
		bool operator!=(const TimeSpan & other) const;
		bool operator>=(const TimeSpan & other) const;
		bool operator<=(const TimeSpan & other) const;
		bool operator>(const TimeSpan & other) const;
		bool operator<(const TimeSpan & other) const;

		TimeSpan operator+(const TimeSpan & other) const;
		TimeSpan operator-(const TimeSpan & other) const;
		TimeSpan operator/(const TimeSpan & other) const;
		TimeSpan operator*(const TimeSpan & other) const;
		TimeSpan operator%(const TimeSpan & other) const;

		TimeSpan & operator+=(const TimeSpan & other);
		TimeSpan & operator-=(const TimeSpan & other);
		TimeSpan & operator/=(const TimeSpan & other);
		TimeSpan & operator*=(const TimeSpan & other);
		TimeSpan & operator%=(const TimeSpan & other);

	private:

		explicit TimeSpan(i64_t microseconds);
	
	private:

		i64_t microseconds;

	};

}