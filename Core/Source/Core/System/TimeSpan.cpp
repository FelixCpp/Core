#include <Core/System/TimeSpan.hpp>

namespace Core
{

	const TimeSpan TimeSpan::Zero;

	TimeSpan::TimeSpan() :
		TimeSpan(0ll)
	{
	}

	float TimeSpan::ToSeconds() const
	{
		return this->microseconds / 1000.f / 1000.f;
	}

	i32_t TimeSpan::ToMilliseconds() const
	{
		return this->microseconds / 1000;
	}

	i64_t TimeSpan::ToMicroseconds() const
	{
		return this->microseconds;
	}

	TimeSpan TimeSpan::FromSeconds(float seconds)
	{
		return TimeSpan(seconds * 1000 * 1000);
	}

	TimeSpan TimeSpan::FromMilliseconds(i32_t milliseconds)
	{
		return TimeSpan(milliseconds * 1000);
	}

	TimeSpan TimeSpan::FromMicroseconds(i64_t microseconds)
	{
		return TimeSpan(microseconds);
	}

	bool TimeSpan::operator==(const TimeSpan & other) const
	{
		return this->microseconds == other.microseconds;
	}

	bool TimeSpan::operator!=(const TimeSpan & other) const
	{
		return this->microseconds != other.microseconds;
	}

	bool TimeSpan::operator>=(const TimeSpan & other) const
	{
		return this->microseconds >= other.microseconds;
	}

	bool TimeSpan::operator<=(const TimeSpan & other) const
	{
		return this->microseconds <= other.microseconds;
	}

	bool TimeSpan::operator>(const TimeSpan & other) const
	{
		return this->microseconds > other.microseconds;
	}

	bool TimeSpan::operator<(const TimeSpan & other) const
	{
		return this->microseconds < other.microseconds;
	}

	TimeSpan TimeSpan::operator+(const TimeSpan & other) const
	{
		return TimeSpan::FromMicroseconds(this->microseconds + other.microseconds);
	}

	TimeSpan TimeSpan::operator-(const TimeSpan & other) const
	{
		return TimeSpan::FromMicroseconds(this->microseconds - other.microseconds);
	}

	TimeSpan TimeSpan::operator/(const TimeSpan & other) const
	{
		return TimeSpan::FromMicroseconds(this->microseconds / other.microseconds);
	}

	TimeSpan TimeSpan::operator*(const TimeSpan & other) const
	{
		return TimeSpan::FromMicroseconds(this->microseconds * other.microseconds);
	}

	TimeSpan TimeSpan::operator%(const TimeSpan & other) const
	{
		return TimeSpan::FromMicroseconds(this->microseconds % other.microseconds);
	}

	TimeSpan & TimeSpan::operator+=(const TimeSpan & other)
	{
		return *this = *this + other;
	}

	TimeSpan & TimeSpan::operator-=(const TimeSpan & other)
	{
		return *this = *this - other;
	}

	TimeSpan & TimeSpan::operator/=(const TimeSpan & other)
	{
		return *this = *this / other;
	}

	TimeSpan & TimeSpan::operator*=(const TimeSpan & other)
	{
		return *this = *this * other;
	}

	TimeSpan & TimeSpan::operator%=(const TimeSpan & other)
	{
		return *this = *this % other;
	}

	TimeSpan::TimeSpan(i64_t microseconds) :
		microseconds(microseconds)
	{
	}

}