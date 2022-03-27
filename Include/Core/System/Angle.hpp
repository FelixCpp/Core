// 
// Angle.hpp
// Core
// 
// Created by Felix Busch on 17.02.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#pragma once

namespace Core
{
	////////////////////////////////////////////////////////////
	/// \brief Define angle wrapper for degrees and radians
	/// 
	////////////////////////////////////////////////////////////
	class Angle
	{
	public:

		////////////////////////////////////////////////////////////
		/// Constants
		/// 
		////////////////////////////////////////////////////////////
		static const float DegToRad;
		static const float RadToDeg;

		////////////////////////////////////////////////////////////
		/// Static members
		/// 
		////////////////////////////////////////////////////////////
		static const Angle Zero;

		////////////////////////////////////////////////////////////
		/// \brief Default constructor
		///
		///	Sets the angle value to zero.
		/// 
		////////////////////////////////////////////////////////////
		constexpr Angle();

		////////////////////////////////////////////////////////////
		/// \brief Return the angle value in degrees
		///
		///	\return Angle in degrees
		///
		///	\see ToRadians
		/// 
		////////////////////////////////////////////////////////////
		constexpr float ToDegrees() const;

		////////////////////////////////////////////////////////////
		/// \brief Return the angle value in radians
		///
		///	\return Angle in radians
		///
		///	\see ToDegrees
		/// 
		////////////////////////////////////////////////////////////
		constexpr float ToRadians() const;

	private:

		////////////////////////////////////////////////////////////
		/// \brief Construct an angle from value in degrees
		/// 
		////////////////////////////////////////////////////////////
		constexpr explicit Angle(float degrees);

		////////////////////////////////////////////////////////////
		/// Member data
		/// 
		////////////////////////////////////////////////////////////
		float degrees;

	private:

		////////////////////////////////////////////////////////////
		/// Friendships
		/// 
		////////////////////////////////////////////////////////////
		friend constexpr Angle Radians(float angle);
		friend constexpr Angle Degrees(float angle);

	};
	
	////////////////////////////////////////////////////////////
	/// Construct from angle
	/// 
	////////////////////////////////////////////////////////////
	constexpr Angle Radians(float angle);
	constexpr Angle Degrees(float angle);

	////////////////////////////////////////////////////////////
	/// Boolean operators
	/// 
	////////////////////////////////////////////////////////////
	constexpr bool operator==(const Angle& lhs, const Angle& rhs);
	constexpr bool operator!=(const Angle& lhs, const Angle& rhs);
	constexpr bool operator<=(const Angle& lhs, const Angle& rhs);
	constexpr bool operator>=(const Angle& lhs, const Angle& rhs);
	constexpr bool operator<(const Angle& lhs, const Angle& rhs);
	constexpr bool operator>(const Angle& lhs, const Angle& rhs);

	////////////////////////////////////////////////////////////
	/// Mathematical operators
	/// 
	////////////////////////////////////////////////////////////
	constexpr Angle operator+(const Angle& lhs, const Angle& rhs);
	constexpr Angle operator-(const Angle& lhs, const Angle& rhs);
	constexpr Angle operator/(const Angle& lhs, const Angle& rhs);
	constexpr Angle operator*(const Angle& lhs, const Angle& rhs);
	constexpr Angle operator%(const Angle& lhs, const Angle& rhs);

	constexpr Angle& operator+=(Angle& lhs, const Angle& rhs);
	constexpr Angle& operator-=(Angle& lhs, const Angle& rhs);
	constexpr Angle& operator/=(Angle& lhs, const Angle& rhs);
	constexpr Angle& operator*=(Angle& lhs, const Angle& rhs);
	constexpr Angle& operator%=(Angle& lhs, const Angle& rhs);

	constexpr Angle operator+(const Angle& angle);
	constexpr Angle operator-(const Angle& angle);

}

#include <Core/System/Angle.inl>