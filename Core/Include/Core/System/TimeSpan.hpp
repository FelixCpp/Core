#pragma once

/// <summary>
/// Core
/// </summary>
#include <Core/System/Datatypes.hpp>

/// <summary>
/// C++ / STL
/// </summary>
#include <chrono>

namespace Core
{
	
	class TimeSpan {
	public:

		/// <summary>
		/// The default constructor.
		/// It will create a TimeSpan equal to
		/// 'Zero'
		/// </summary>
		TimeSpan();

		/// <summary>
		/// Creates a timespan by using the
		/// parameter as value
		/// </summary>
		/// <param name="nanoseconds">the amount of the span</param>
		explicit TimeSpan(i64_t nanoseconds);

		/// <summary>
		/// Creates a TimeSpan using the
		/// chrono API
		/// </summary>
		template<class _Rep, class _Period>
		explicit TimeSpan(const std::chrono::duration<_Rep, _Period> & duration) :
			nanoseconds(std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count())
		{
		}

		/// <summary>
		/// Timespan offset operations
		/// </summary>
		
		/// <summary>
		/// Returns the TimeSpan plus one nanosecond
		/// </summary>
		TimeSpan operator+() const;
		
		/// <summary>
		/// Returns the TimeSpan minus one nanosecond
		/// </summary>
		TimeSpan operator-() const;

		/// <summary>
		/// Adds two TimeSpans together
		/// </summary>
		/// <param name="other">the TimeSpan to add</param>
		TimeSpan operator+(const TimeSpan & other) const;

		/// <summary>
		/// Subtracts two TimeSpans from each other
		/// </summary>
		/// <param name="other">the TimeSpan to subtract</param>
		TimeSpan operator-(const TimeSpan & other) const;

		/// <summary>
		/// Divides two TimeSpans
		/// </summary>
		/// <param name="other">the TimeSpan to divide from</param>
		TimeSpan operator/(const TimeSpan & other) const;

		/// <summary>
		/// Multiplies two TimeSpans together
		/// </summary>
		/// <param name="other">the TimeSpan to multiply</param>
		TimeSpan operator*(const TimeSpan & other) const;

		/// <summary>
		/// Adds two TimeSpans together
		/// </summary>
		/// <param name="other">the TimeSpan to add</param>
		TimeSpan & operator+=(const TimeSpan & other);

		/// <summary>
		/// Subtracts two TimeSpans from each other
		/// </summary>
		/// <param name="other">the TimeSpan to subtract</param>
		TimeSpan & operator-=(const TimeSpan & other);

		/// <summary>
		/// Divides two TimeSpans
		/// </summary>
		/// <param name="other">the TimeSpan to divide from</param>
		TimeSpan & operator/=(const TimeSpan & other);

		/// <summary>
		/// Multiplies two TimeSpans together
		/// </summary>
		/// <param name="other">the TimeSpan to multiply</param>
		TimeSpan & operator*=(const TimeSpan & other);

		/// <summary>
		/// Comparism operators
		/// </summary>
		
		/// <summary>
		/// Equal operator
		/// </summary>
		bool operator==(const TimeSpan & other) const;
		
		/// <summary>
		/// Different operator
		/// </summary>
		bool operator!=(const TimeSpan & other) const;

		/// <summary>
		/// Less or equal operator
		/// </summary>
		bool operator<=(const TimeSpan & other) const;

		/// <summary>
		/// Greater or equal operator
		/// </summary>
		bool operator>=(const TimeSpan & other) const;

		/// <summary>
		/// Less operator
		/// </summary>
		bool operator<(const TimeSpan & other) const;

		/// <summary>
		/// Greater operator
		/// </summary>
		bool operator>(const TimeSpan & other) const;

		/// <summary>
		/// Returns the TimeSpan as an duration object from
		/// the chrono API
		/// </summary>
		std::chrono::system_clock::duration Chrono() const;

		/// <summary>
		/// Returns the TimeSpan as days
		/// </summary>
		i64_t ToDays() const;

		/// <summary>
		/// Returns the TimeSpan as hours
		/// </summary>
		i64_t ToHours() const;

		/// <summary>
		/// Returns the TimeSpan as minutes
		/// </summary>
		i64_t ToMinutes() const;
		
		/// <summary>
		/// Returns the TimeSpan as seconds
		/// </summary>
		i64_t ToSeconds() const;

		/// <summary>
		/// Returns the TimeSpan as milliseconds
		/// </summary>
		i64_t ToMilliseconds() const;

		/// <summary>
		/// Returns the TimeSpan as microseconds
		/// </summary>
		i64_t ToMicroseconds() const;

		/// <summary>
		/// Returns the TimeSpan as nanoseconds
		/// </summary>
		i64_t ToNanoseconds() const;

		/// <summary>
		/// Same as GetNanoseconds()
		/// </summary>
		i64_t Total() const;

		/// <summary>
		/// Some handy ways to create a TimeSpan
		/// </summary>
		
		/// <summary>
		/// Creates a timespan using days as
		/// orientation
		/// </summary>
		/// <param name="days">amount of days</param>
		static TimeSpan FromDays(i64_t days);

		/// <summary>
		/// Creates a timespan using hours as
		/// orientation
		/// </summary>
		/// <param name="hours">amount of hours</param>
		static TimeSpan FromHours(i64_t hours);
		
		/// <summary>
		/// Creates a timespan using minutes as
		/// orientation
		/// </summary>
		/// <param name="minutes">amount of minutes</param>
		static TimeSpan FromMinutes(i64_t minutes);

		/// <summary>
		/// Creates a timespan using seconds as
		/// orientation
		/// </summary>
		/// <param name="seconds">amount of seconds</param>
		static TimeSpan FromSeconds(i64_t seconds);

		/// <summary>
		/// Creates a timespan using milliseconds as
		/// orientation
		/// </summary>
		/// <param name="milliseconds">amount of milliseconds</param>
		static TimeSpan FromMilliseconds(i64_t milliseconds);

		/// <summary>
		/// Creates a timespan using microseconds as
		/// orientation
		/// </summary>
		/// <param name="microseconds">amount of microseconds</param>
		static TimeSpan FromMicroseconds(i64_t microseconds);

		/// <summary>
		/// Creates a timespan using nanoseconds as
		/// orientation
		/// </summary>
		/// <param name="nanoseconds">amount of nanoseconds</param>
		static TimeSpan FromNanoseconds(i64_t nanoseconds);

		/// <summary>
		/// Creates a TimeSpan using the duration class
		/// from the chrono API
		/// </summary>
		template<class _Rep, typename _Period>
		static TimeSpan FromChrono(const std::chrono::duration<_Rep, _Period> & duration)
		{
			return TimeSpan(duration);
		}

		/// <summary>
		/// The default TimeSpan
		/// </summary>
		static const TimeSpan Zero;

	private:

		/// <summary>
		/// Internal 64-bit integer to store
		/// the nanoseconds
		/// </summary>
		i64_t nanoseconds;

	};

}

namespace std
{

	/// <summary>
	/// Overloaded hash for TimeSpan
	/// </summary>
	struct hash<Core::TimeSpan> {
		typedef Core::TimeSpan argument_type;
		typedef size_t result_type;
		
		result_type operator()(const argument_type & value) const;
	};

}