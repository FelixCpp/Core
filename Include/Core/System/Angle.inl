// 
// Angle.inl
// Core
// 
// Created by Felix Busch on 06.03.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#pragma once

#include <cmath>
#include <numbers>

namespace Core
{

	////////////////////////////////////////////////////////////
	/// Linking static members
	/// 
	////////////////////////////////////////////////////////////
	inline constexpr Angle Angle::Zero;
	inline constexpr float Angle::DegToRad = std::numbers::pi_v<float> / 180.0f;
	inline constexpr float Angle::RadToDeg = 180.0f / std::numbers::pi_v<float>;

	////////////////////////////////////////////////////////////
	constexpr Angle::Angle():
		degrees(0.0f)
	{}

	////////////////////////////////////////////////////////////
	constexpr float Angle::ToRadians() const
	{
		return degrees * DegToRad;
	}

	////////////////////////////////////////////////////////////
	constexpr float Angle::ToDegrees() const
	{
		return degrees;
	}

	////////////////////////////////////////////////////////////
	constexpr Angle::Angle(float degrees):
		degrees(degrees)
	{
	}

	////////////////////////////////////////////////////////////
	constexpr Angle Radians(float angle)
	{
		return Angle(angle * Angle::RadToDeg);
	}

	////////////////////////////////////////////////////////////
	constexpr Angle Degrees(float angle)
	{
		return Angle(angle);
	}

	////////////////////////////////////////////////////////////
	constexpr bool operator==(const Angle& lhs, const Angle& rhs)
	{
		return lhs.ToDegrees() == rhs.ToDegrees();
	}

	////////////////////////////////////////////////////////////
	constexpr bool operator!=(const Angle& lhs, const Angle& rhs)
	{
		return lhs.ToDegrees() == rhs.ToDegrees();
	}

	////////////////////////////////////////////////////////////
	constexpr bool operator<=(const Angle& lhs, const Angle& rhs)
	{
		return lhs.ToDegrees() <= rhs.ToDegrees();
	}

	////////////////////////////////////////////////////////////
	constexpr bool operator>=(const Angle& lhs, const Angle& rhs)
	{
		return lhs.ToDegrees() >= rhs.ToDegrees();
	}

	////////////////////////////////////////////////////////////
	constexpr bool operator<(const Angle& lhs, const Angle& rhs)
	{
		return lhs.ToDegrees() < rhs.ToDegrees();
	}

	////////////////////////////////////////////////////////////
	constexpr bool operator>(const Angle& lhs, const Angle& rhs)
	{
		return lhs.ToDegrees() > rhs.ToDegrees();
	}

	////////////////////////////////////////////////////////////
	constexpr Angle operator+(const Angle& lhs, const Angle& rhs)
	{
		return Degrees(lhs.ToDegrees() + rhs.ToDegrees());
	}

	////////////////////////////////////////////////////////////
	constexpr Angle operator-(const Angle& lhs, const Angle& rhs)
	{
		return Degrees(lhs.ToDegrees() - rhs.ToDegrees());
	}

	////////////////////////////////////////////////////////////
	constexpr Angle operator/(const Angle& lhs, const Angle& rhs)
	{
		return Degrees(lhs.ToDegrees() / rhs.ToDegrees());
	}

	////////////////////////////////////////////////////////////
	constexpr Angle operator*(const Angle& lhs, const Angle& rhs)
	{
		return Degrees(lhs.ToDegrees() * rhs.ToDegrees());
	}

	////////////////////////////////////////////////////////////
	constexpr Angle operator%(const Angle& lhs, const Angle& rhs)
	{
		return Degrees(std::fmodf(lhs.ToDegrees(), rhs.ToDegrees()));
	}

	////////////////////////////////////////////////////////////
	constexpr Angle& operator+=(Angle& lhs, const Angle& rhs)
	{
		return lhs = lhs + rhs;
	}

	////////////////////////////////////////////////////////////
	constexpr Angle& operator-=(Angle& lhs, const Angle& rhs)
	{
		return lhs = lhs - rhs;
	}

	////////////////////////////////////////////////////////////
	constexpr Angle& operator/=(Angle& lhs, const Angle& rhs)
	{
		return lhs = lhs / rhs;
	}

	////////////////////////////////////////////////////////////
	constexpr Angle& operator*=(Angle& lhs, const Angle& rhs)
	{
		return lhs = lhs * rhs;
	}

	////////////////////////////////////////////////////////////
	constexpr Angle& operator%=(Angle& lhs, const Angle& rhs)
	{
		return lhs = lhs % rhs;
	}

	////////////////////////////////////////////////////////////
	constexpr Angle operator+(const Angle& angle)
	{
		return Degrees(+angle.ToDegrees());
	}

	////////////////////////////////////////////////////////////
	constexpr Angle operator-(const Angle& angle)
	{
		return Degrees(-angle.ToDegrees());
	}

}