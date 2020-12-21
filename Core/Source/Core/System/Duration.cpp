#include <Core/System/Duration.hpp>

namespace Core
{

	const Duration Duration::Zero;

	Duration::Duration() :
		Duration(0ll)
	{
	}

	float Duration::toSeconds() const
	{
		return this->microseconds / 1000.f / 1000.f;
	}

	i32_t Duration::toMilliseconds() const
	{
		return this->microseconds / 1000;
	}

	i64_t Duration::toMicroseconds() const
	{
		return this->microseconds;
	}

	Duration Duration::fromSeconds(float seconds)
	{
		return Duration(seconds * 1000 * 1000);
	}

	Duration Duration::fromMilliseconds(i32_t milliseconds)
	{
		return Duration(milliseconds * 1000);
	}

	Duration Duration::fromMicroseconds(i64_t microseconds)
	{
		return Duration(microseconds);
	}

	bool Duration::operator==(const Duration & other) const
	{
		return this->microseconds == other.microseconds;
	}

	bool Duration::operator!=(const Duration & other) const
	{
		return this->microseconds != other.microseconds;
	}

	bool Duration::operator>=(const Duration & other) const
	{
		return this->microseconds >= other.microseconds;
	}

	bool Duration::operator<=(const Duration & other) const
	{
		return this->microseconds <= other.microseconds;
	}

	bool Duration::operator>(const Duration & other) const
	{
		return this->microseconds > other.microseconds;
	}

	bool Duration::operator<(const Duration & other) const
	{
		return this->microseconds < other.microseconds;
	}

	Duration Duration::operator+(const Duration & other) const
	{
		return Duration::fromMicroseconds(this->microseconds + other.microseconds);
	}

	Duration Duration::operator-(const Duration & other) const
	{
		return Duration::fromMicroseconds(this->microseconds - other.microseconds);
	}

	Duration Duration::operator/(const Duration & other) const
	{
		return Duration::fromMicroseconds(this->microseconds / other.microseconds);
	}

	Duration Duration::operator*(const Duration & other) const
	{
		return Duration::fromMicroseconds(this->microseconds * other.microseconds);
	}

	Duration Duration::operator%(const Duration & other) const
	{
		return Duration::fromMicroseconds(this->microseconds % other.microseconds);
	}

	Duration & Duration::operator+=(const Duration & other)
	{
		return *this = *this + other;
	}

	Duration & Duration::operator-=(const Duration & other)
	{
		return *this = *this - other;
	}

	Duration & Duration::operator/=(const Duration & other)
	{
		return *this = *this / other;
	}

	Duration & Duration::operator*=(const Duration & other)
	{
		return *this = *this * other;
	}

	Duration & Duration::operator%=(const Duration & other)
	{
		return *this = *this % other;
	}

	Duration::Duration(i64_t microseconds) :
		microseconds(microseconds)
	{
	}

}