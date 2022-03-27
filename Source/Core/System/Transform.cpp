// 
// Transform.cpp
// Core
// 
// Created by Felix Busch on 17.02.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#include <Core/System/Transform.hpp>

namespace Core
{

	////////////////////////////////////////////////////////////
	/// Link static members
	/// 
	////////////////////////////////////////////////////////////
	constexpr Transform Transform::Identity;

	////////////////////////////////////////////////////////////
	Transform& Transform::Rotate(const Angle& angle)
	{
		return Multiply(Rotation(angle));
	}

	////////////////////////////////////////////////////////////
	Transform& Transform::Rotate(const Angle& angle, float centerX, float centerY)
	{
		return Multiply(Rotation(angle, centerX, centerY));
	}

	////////////////////////////////////////////////////////////
	Transform& Transform::Rotate(const Angle& angle, const Float2& center)
	{
		return Rotate(angle, center.X, center.Y);
	}
	
	////////////////////////////////////////////////////////////
	Transform Transform::Rotation(const Angle& angle)
	{
		const float radians = angle.ToRadians();
		const float cos = std::cos(radians);
		const float sin = std::sin(radians);

		return {
			cos, -sin, 0.0f,
			sin, cos, 0.0f,
			0.0f, 0.0f, 1.0f
		};
	}

	////////////////////////////////////////////////////////////
	Transform Transform::Rotation(const Angle& angle, float centerX, float centerY)
	{
		const float radians = angle.ToRadians();
		const float cos = std::cos(radians);
		const float sin = std::sin(radians);
		const float tx = -centerX * cos + centerY * sin + centerX;
		const float ty = -centerX * sin - centerY * cos + centerY;

		return {
			cos, -sin, tx,
			sin, cos, ty,
			0.0f, 0.0f, 1.0f
		};
	}

	////////////////////////////////////////////////////////////
	Transform Transform::Rotation(const Angle& angle, const Float2& center)
	{
		return Rotation(angle, center.X, center.Y);
	}
}
