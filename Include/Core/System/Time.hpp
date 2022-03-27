// 
// Time.hpp
// Core
// 
// Created by Felix Busch on 15.02.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#pragma once

#include <cstdint>

namespace Core
{
	////////////////////////////////////////////////////////////
	/// \brief Define time object for durations, timestamps etc.
	/// 
	////////////////////////////////////////////////////////////
	class Time
	{
	public:

		////////////////////////////////////////////////////////////
		/// \brief Evaluation factors to convert from/into different
		///		   time formats
		/// 
		////////////////////////////////////////////////////////////
		inline static constexpr int64_t NanosecondsFactor	= 1;
		inline static constexpr int64_t MicrosecondsFactor	= NanosecondsFactor		* 1000;
		inline static constexpr int64_t MillisecondsFactor	= MicrosecondsFactor	* 1000;
		inline static constexpr int64_t SecondsFactor		= MillisecondsFactor	* 1000;
		inline static constexpr int64_t MinutesFactor		= SecondsFactor			* 60;
		inline static constexpr int64_t HoursFactor			= MinutesFactor			* 60;
		inline static constexpr int64_t DaysFactor			= HoursFactor			* 24;

		////////////////////////////////////////////////////////////
		/// \brief Static members
		/// 
		////////////////////////////////////////////////////////////
		static const Time Zero;

		////////////////////////////////////////////////////////////
		/// \brief Default constructor
		/// 
		////////////////////////////////////////////////////////////
		constexpr Time():
			nanoseconds(0)
		{}

		////////////////////////////////////////////////////////////
		/// \brief Conversions into type
		/// 
		////////////////////////////////////////////////////////////
		template<typename T> [[nodiscard]] constexpr T ToNanoseconds() const { return (T)nanoseconds / (T)NanosecondsFactor; }
		template<typename T> [[nodiscard]] constexpr T ToMicroseconds() const { return (T)nanoseconds / (T)MicrosecondsFactor; }
		template<typename T> [[nodiscard]] constexpr T ToMilliseconds() const { return (T)nanoseconds / (T)MillisecondsFactor; }
		template<typename T> [[nodiscard]] constexpr T ToSeconds() const { return (T)nanoseconds / (T)SecondsFactor; }
		template<typename T> [[nodiscard]] constexpr T ToMinutes() const { return (T)nanoseconds / (T)MinutesFactor; }
		template<typename T> [[nodiscard]] constexpr T ToHours() const { return (T)nanoseconds / (T)HoursFactor; }
		template<typename T> [[nodiscard]] constexpr T ToDays() const { return (T)nanoseconds / (T)DaysFactor; }

		////////////////////////////////////////////////////////////
		/// \brief Conversions from type
		/// 
		////////////////////////////////////////////////////////////
		template<typename T> constexpr static Time FromNanoseconds(T amount) { return Time((int64_t)(amount * (T)NanosecondsFactor)); }
		template<typename T> constexpr static Time FromMicroseconds(T amount) { return Time((int64_t)(amount * (T)MicrosecondsFactor)); }
		template<typename T> constexpr static Time FromMilliseconds(T amount) { return Time((int64_t)(amount * (T)MillisecondsFactor)); }
		template<typename T> constexpr static Time FromSeconds(T amount) { return Time((int64_t)(amount * (T)SecondsFactor)); }
		template<typename T> constexpr static Time FromMinutes(T amount) { return Time((int64_t)(amount * (T)MinutesFactor)); }
		template<typename T> constexpr static Time FromHours(T amount) { return Time((int64_t)(amount * (T)HoursFactor)); }
		template<typename T> constexpr static Time FromDays(T amount) { return Time((int64_t)(amount * (T)DaysFactor)); }

		////////////////////////////////////////////////////////////
		/// \brief Global functions to create Time objects with
		/// 
		////////////////////////////////////////////////////////////
		template<typename T> constexpr friend Time Nanoseconds(T amount);
		template<typename T> constexpr friend Time Microseconds(T amount);
		template<typename T> constexpr friend Time Milliseconds(T amount);
		template<typename T> constexpr friend Time Seconds(T amount);
		template<typename T> constexpr friend Time Minutes(T amount);
		template<typename T> constexpr friend Time Hours(T amount);
		template<typename T> constexpr friend Time Days(T amount);

		////////////////////////////////////////////////////////////
		/// \brief Boolean operators
		/// 
		////////////////////////////////////////////////////////////
		constexpr friend bool operator == (const Time& lhs, const Time& rhs);
		constexpr friend bool operator != (const Time& lhs, const Time& rhs);
		constexpr friend bool operator <= (const Time& lhs, const Time& rhs);
		constexpr friend bool operator >= (const Time& lhs, const Time& rhs);
		constexpr friend bool operator < (const Time& lhs, const Time& rhs);
		constexpr friend bool operator > (const Time& lhs, const Time& rhs);

		////////////////////////////////////////////////////////////
		/// \brief Mathematical operators that does not change \a lhs
		/// 
		////////////////////////////////////////////////////////////
		constexpr friend Time operator + (const Time& lhs, const Time& rhs);
		constexpr friend Time operator - (const Time& lhs, const Time& rhs);
		constexpr friend Time operator / (const Time& lhs, const Time& rhs);
		constexpr friend Time operator * (const Time& lhs, const Time& rhs);
		constexpr friend Time operator % (const Time& lhs, const Time& rhs);

		////////////////////////////////////////////////////////////
		/// \brief Mathematical operators that does change \a lhs
		/// 
		////////////////////////////////////////////////////////////
		constexpr friend Time& operator += (Time& lhs, const Time& rhs);
		constexpr friend Time& operator -= (Time& lhs, const Time& rhs);
		constexpr friend Time& operator /= (Time& lhs, const Time& rhs);
		constexpr friend Time& operator *= (Time& lhs, const Time& rhs);
		constexpr friend Time& operator %= (Time& lhs, const Time& rhs);

	private:

		////////////////////////////////////////////////////////////
		/// \brief Create time object from nanoseconds
		/// 
		////////////////////////////////////////////////////////////
		constexpr explicit Time(int64_t nanoseconds):
			nanoseconds(nanoseconds)
		{}

		////////////////////////////////////////////////////////////
		/// Member data
		/// 
		////////////////////////////////////////////////////////////
		int64_t nanoseconds;

	};

	////////////////////////////////////////////////////////////
	/// Linking static members
	/// 
	////////////////////////////////////////////////////////////
	inline constexpr Time Time::Zero;

	////////////////////////////////////////////////////////////
	/// \brief Global functions to create Time objects with
	/// 
	////////////////////////////////////////////////////////////
	template<typename T> constexpr Time Nanoseconds(T amount) { return Time::FromNanoseconds(amount); }
	template<typename T> constexpr Time Microseconds(T amount) { return Time::FromMicroseconds(amount); }
	template<typename T> constexpr Time Milliseconds(T amount) { return Time::FromMilliseconds(amount); }
	template<typename T> constexpr Time Seconds(T amount) { return Time::FromSeconds(amount); }
	template<typename T> constexpr Time Minutes(T amount) { return Time::FromMinutes(amount);  }
	template<typename T> constexpr Time Hours(T amount) { return Time::FromHours(amount); }
	template<typename T> constexpr Time Days(T amount) { return Time::FromDays(amount); }

	////////////////////////////////////////////////////////////
	/// \brief Boolean operators
	/// 
	////////////////////////////////////////////////////////////
	constexpr bool operator == (const Time& lhs, const Time& rhs) { return lhs.nanoseconds == rhs.nanoseconds; }
	constexpr bool operator != (const Time& lhs, const Time& rhs) { return lhs.nanoseconds != rhs.nanoseconds; }
	constexpr bool operator <= (const Time& lhs, const Time& rhs) { return lhs.nanoseconds <= rhs.nanoseconds; }
	constexpr bool operator >= (const Time& lhs, const Time& rhs) { return lhs.nanoseconds >= rhs.nanoseconds; }
	constexpr bool operator < (const Time& lhs, const Time& rhs) { return lhs.nanoseconds < rhs.nanoseconds; }
	constexpr bool operator > (const Time& lhs, const Time& rhs) { return lhs.nanoseconds > rhs.nanoseconds; }

	////////////////////////////////////////////////////////////
	/// \brief Mathematical operators that does not change \a lhs
	/// 
	////////////////////////////////////////////////////////////
	constexpr Time operator + (const Time& lhs, const Time& rhs) { return Time(lhs.nanoseconds + rhs.nanoseconds); }
	constexpr Time operator - (const Time& lhs, const Time& rhs) { return Time(lhs.nanoseconds - rhs.nanoseconds); }
	constexpr Time operator / (const Time& lhs, const Time& rhs) { return Time(lhs.nanoseconds / rhs.nanoseconds); }
	constexpr Time operator * (const Time& lhs, const Time& rhs) { return Time(lhs.nanoseconds * rhs.nanoseconds); }
	constexpr Time operator % (const Time& lhs, const Time& rhs) { return Time(lhs.nanoseconds % rhs.nanoseconds); }

	////////////////////////////////////////////////////////////
	/// \brief Mathematical operators that does change \a lhs
	/// 
	////////////////////////////////////////////////////////////
	constexpr Time& operator += (Time& lhs, const Time& rhs) { return lhs = lhs + rhs; }
	constexpr Time& operator -= (Time& lhs, const Time& rhs) { return lhs = lhs - rhs; }
	constexpr Time& operator /= (Time& lhs, const Time& rhs) { return lhs = lhs / rhs; }
	constexpr Time& operator *= (Time& lhs, const Time& rhs) { return lhs = lhs * rhs; }
	constexpr Time& operator %= (Time& lhs, const Time& rhs) { return lhs = lhs % rhs; }

}