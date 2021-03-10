#pragma once

/// <summary>
/// Core
/// </summary>
#include <Core/System/Datatypes.hpp>
#include <Core/System/Comparable.hpp>

namespace Core
{


#define CPP_DECLARE_TIME_METHOD(fnName, pmName, factor)			\
static TimeSpan fnName(Int64 pmName)							\
{																\
	const Int64 nanoseconds = (Int64)(pmName * factor);			\
	return TimeSpan(nanoseconds);								\
}																\
																\
static TimeSpan fnName(Int32 pmName)							\
{																\
	return fnName((Int64)pmName);								\
}																\
																\
static TimeSpan fnName(double pmName)							\
{																\
	const Int64 nanoseconds = (Int64)(pmName * (double)factor);	\
	return TimeSpan(nanoseconds);								\
}																\

	class TimeSpan : public Comparable<TimeSpan> {
	public:

		inline static constexpr Int64 FactorNanoseconds		= 1ll;
		inline static constexpr Int64 FactorMicroseconds	= FactorNanoseconds		* 1'000ll;
		inline static constexpr Int64 FactorMilliseconds	= FactorMicroseconds	* 1'000ll;
		inline static constexpr Int64 FactorSeconds			= FactorMilliseconds	* 1'000ll;
		inline static constexpr Int64 FactorMinutes			= FactorSeconds			* 60ll;
		inline static constexpr Int64 FactorHours			= FactorMinutes			* 60ll;
		inline static constexpr Int64 FactorDays			= FactorHours			* 24ll;

	public:

		/// <summary>
		/// Default constructor
		/// 
		/// Equals:
		///		<code>
		///			TimeSpan ts = TimeSpan::Zero;
		///		</code>
		/// </summary>
		TimeSpan();

		/// <summary>
		/// Creates a TimeSpan using the given values
		/// </summary>
		/// <param name="days">amount of days</param>
		/// <param name="hours">amount of hours</param>
		/// <param name="minutes">amount of minutes</param>
		/// <param name="seconds">amount of seconds</param>
		/// <param name="milliseconds">amount of milliseconds</param>
		/// <param name="microseconds">amount of microseconds</param>
		/// <param name="nanoseconds">amount of nanoseconds</param>
		explicit TimeSpan(
			Int32 days,
			Int32 hours,
			Int32 minutes,
			Int32 seconds,
			Int32 milliseconds,
			Int32 microseconds,
			Int32 nanoseconds
		);

	public:

		/// <summary>
		/// Convertions using
		/// templated Datatypes
		/// </summary>

		template<typename T = Int64> T ToDays()			const { return (T)this->nanoseconds / (T)FactorDays; }
		template<typename T = Int64> T ToHours()		const { return (T)this->nanoseconds / (T)FactorHours; }
		template<typename T = Int64> T ToMinutes()		const { return (T)this->nanoseconds / (T)FactorMinutes; }
		template<typename T = Int64> T ToSeconds()		const { return (T)this->nanoseconds / (T)FactorSeconds; }
		template<typename T = Int64> T ToMilliseconds() const { return (T)this->nanoseconds / (T)FactorMilliseconds; }
		template<typename T = Int64> T ToMicroseconds() const { return (T)this->nanoseconds / (T)FactorMicroseconds; }
		template<typename T = Int64> T ToNanoseconds()	const { return (T)this->nanoseconds / (T)FactorNanoseconds; }

		/// <summary>
		/// Mathimatical operators
		/// </summary>

		TimeSpan operator + (const TimeSpan & other) const;
		TimeSpan operator - (const TimeSpan & other) const;
		TimeSpan operator / (const TimeSpan & other) const;
		TimeSpan operator * (const TimeSpan & other) const;
		TimeSpan operator % (const TimeSpan & other) const;

		TimeSpan & operator += (const TimeSpan & other);
		TimeSpan & operator -= (const TimeSpan & other);
		TimeSpan & operator /= (const TimeSpan & other);
		TimeSpan & operator *= (const TimeSpan & other);
		TimeSpan & operator %= (const TimeSpan & other);

		/// <summary>
		/// This method must be implemented by the Subclass!
		/// 
		/// If the instance this Method is called on is less/smaller
		/// than the 'other' parameter, the method should return -1,
		/// if the instance is greater this method should return 1,
		/// 0 otherwise (equality)
		/// </summary>
		/// <param name="other">the instance to compare against</param>
		virtual Int32 Compare(const TimeSpan & other) const override;

		/// <summary>
		/// Creation methods
		/// </summary>

		CPP_DECLARE_TIME_METHOD(FromDays, days, FactorDays)
		CPP_DECLARE_TIME_METHOD(FromHours, hours, FactorHours)
		CPP_DECLARE_TIME_METHOD(FromMinutes, minutes, FactorMinutes)
		CPP_DECLARE_TIME_METHOD(FromSeconds, seconds, FactorSeconds)
		CPP_DECLARE_TIME_METHOD(FromMilliseconds, milliseconds, FactorMilliseconds)
		CPP_DECLARE_TIME_METHOD(FromMicroseconds, microseconds, FactorMicroseconds)
		CPP_DECLARE_TIME_METHOD(FromNanoseconds, nanos, FactorNanoseconds)

		/// <summary>
		/// Just a TimeSpan with no time set
		/// </summary>
		static const TimeSpan Zero;

	private:

		/// <summary>
		/// Creates a TimeSpan with the given nanoseconds
		/// </summary>
		/// <param name="nanoseconds">amount of nanoseconds to convert from by calling To...()</param>
		explicit TimeSpan(Int64 nanoseconds);

	private:

		/// <summary>
		/// Internally stored nanoseconds
		/// </summary>
		Int64 nanoseconds;

	};

#if defined(CPP_DECLARE_TIME_METHOD)
#undef CPP_DECLARE_TIME_METHOD
#endif // defined(CPP_DECLARE_TIME_METHOD)

}