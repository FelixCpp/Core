#include <Core/System/TimeSpan.hpp>

namespace Core
{

	const TimeSpan TimeSpan::Zero;

	TimeSpan::TimeSpan() :
		nanoseconds(0)
	{
	}

	TimeSpan::TimeSpan(i64_t nanoseconds) :
		nanoseconds(nanoseconds)
	{
	}

	TimeSpan TimeSpan::operator+() const
	{
		return TimeSpan(this->nanoseconds + 1);
	}

	TimeSpan TimeSpan::operator-() const
	{
		return TimeSpan(this->nanoseconds - 1);
	}

	TimeSpan TimeSpan::operator+(const TimeSpan & other) const
	{
		return TimeSpan(this->nanoseconds + other.nanoseconds);
	}

	TimeSpan TimeSpan::operator-(const TimeSpan & other) const
	{
		return TimeSpan(this->nanoseconds - other.nanoseconds);
	}

	TimeSpan TimeSpan::operator/(const TimeSpan & other) const
	{
		return TimeSpan(this->nanoseconds / other.nanoseconds);
	}

	TimeSpan TimeSpan::operator*(const TimeSpan & other) const
	{
		return TimeSpan(this->nanoseconds * other.nanoseconds);
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

	bool TimeSpan::operator==(const TimeSpan & other) const
	{
		return this->nanoseconds == other.nanoseconds;
	}

	bool TimeSpan::operator!=(const TimeSpan & other) const
	{
		return this->nanoseconds != other.nanoseconds;
	}

	bool TimeSpan::operator<=(const TimeSpan & other) const
	{
		return this->nanoseconds <= other.nanoseconds;
	}

	bool TimeSpan::operator>=(const TimeSpan & other) const
	{
		return this->nanoseconds >= other.nanoseconds;
	}

	bool TimeSpan::operator<(const TimeSpan & other) const
	{
		return this->nanoseconds < other.nanoseconds;
	}

	bool TimeSpan::operator>(const TimeSpan & other) const
	{
		return this->nanoseconds > other.nanoseconds;
	}

	std::chrono::system_clock::duration TimeSpan::Chrono() const
	{
		return std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::nanoseconds(this->nanoseconds));
	}

	i64_t TimeSpan::ToDays() const
	{
		return this->nanoseconds / 1000'000'000 / 60 / 60 / 24;
	}

	i64_t TimeSpan::ToHours() const
	{
		return this->nanoseconds / 1000'000'000 / 60 / 60;
	}

	i64_t TimeSpan::ToMinutes() const
	{
		return this->nanoseconds / 1000'000'000 / 60;
	}

	i64_t TimeSpan::ToSeconds() const
	{
		return this->nanoseconds / 1000'000'000;
	}

	i64_t TimeSpan::ToMilliseconds() const
	{
		return this->nanoseconds / 1000'000;
	}

	i64_t TimeSpan::ToMicroseconds() const
	{
		return this->nanoseconds / 1000;
	}

	i64_t TimeSpan::ToNanoseconds() const
	{
		return this->nanoseconds;
	}

	i64_t TimeSpan::Total() const
	{
		return this->ToNanoseconds();
	}

	TimeSpan TimeSpan::FromDays(i64_t days)
	{
		return TimeSpan(days * 1000'000'000 * 60 * 60 * 24);
	}

	TimeSpan TimeSpan::FromHours(i64_t hours)
	{
		return TimeSpan(hours * 1000'000'000 * 60 * 60);
	}

	TimeSpan TimeSpan::FromMinutes(i64_t minutes)
	{
		return TimeSpan(minutes * 1000'000'000 * 60);
	}

	TimeSpan TimeSpan::FromSeconds(i64_t seconds)
	{
		return TimeSpan(seconds * 1000'000'000);
	}

	TimeSpan TimeSpan::FromMilliseconds(i64_t milliseconds)
	{
		return TimeSpan(milliseconds * 1000'000);
	}

	TimeSpan TimeSpan::FromMicroseconds(i64_t microseconds)
	{
		return TimeSpan(microseconds * 1000);
	}

	TimeSpan TimeSpan::FromNanoseconds(i64_t nanoseconds)
	{
		return TimeSpan(nanoseconds);
	}

}

namespace std
{

	inline hash<Core::TimeSpan>::result_type hash<Core::TimeSpan>::operator()(const argument_type & value) const
	{
		const std::hash<Core::i64_t> i64Hasher;
		return 17 * 31 + i64Hasher(value.Total());
	}

}