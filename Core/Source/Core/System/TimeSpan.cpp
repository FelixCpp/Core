#include <Core/System/TimeSpan.hpp>
#include <type_traits>

namespace Core
{

	const TimeSpan TimeSpan::Zero;

	TimeSpan::TimeSpan() :
		nanoseconds(0ll)
	{
	}

	TimeSpan::TimeSpan(Int32 days, Int32 hours, Int32 minutes, Int32 seconds, Int32 milliseconds, Int32 microseconds, Int32 nanoseconds) :
		TimeSpan(
			FromDays(days) + FromHours(hours) + FromMinutes(minutes) + FromSeconds(seconds) + FromMilliseconds(milliseconds) + FromMicroseconds(microseconds) + FromNanoseconds(nanoseconds)
		)
	{
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

	TimeSpan TimeSpan::operator%(const TimeSpan & other) const
	{
		return TimeSpan(this->nanoseconds % other.nanoseconds);
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

	Int32 TimeSpan::Compare(const TimeSpan & other) const
	{
		if (this->nanoseconds > other.nanoseconds) return -1;
		if (this->nanoseconds < other.nanoseconds) return  1;
		
		return 0;
	}

	TimeSpan::TimeSpan(Int64 nanoseconds) :
		nanoseconds(nanoseconds)
	{
	}

}

_STD_BEGIN

	/// <summary>
	/// Overloaded hash for TimeSpan
	/// </summary>
	template<>
	struct hash<Core::TimeSpan> {
		typedef Core::TimeSpan argument_type;
		typedef size_t result_type;

		inline result_type operator()(const argument_type & value) const
		{
			const std::hash<Core::Int64> i64Hasher;
			return 17 * 31 + i64Hasher(value.ToNanoseconds());
		}
	};

_STD_END