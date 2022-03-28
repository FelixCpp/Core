// 
// Transformation.cpp
// Core
// 
// Created by Felix Busch on 28.03.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#include <Core/Graphics/Transformation.hpp>

namespace Core
{
	Transformation::Transformation():
		position(0.0f, 0.0f),
		scale(1.0f, 1.0f),
		rotation(Angle::Zero),
		needsUpdate(true),
		transform({ 1.0f, 0.0f,
					0.0f, 1.0f,
					0.0f, 0.0f })
	{
	}

	void Transformation::SetPosition(float x, float y)
	{
		position.X = x;
		position.Y = y;
		needsUpdate = true;
	}

	void Transformation::SetPosition(const Float2& position)
	{
		SetPosition(position.X, position.Y);
	}

	const Float2& Transformation::GetPosition() const
	{
		return position;
	}

	void Transformation::Move(float x, float y)
	{
		position.X += x;
		position.Y += y;
		needsUpdate = true;
	}

	void Transformation::Move(const Float2& movement)
	{
		Move(movement.X, movement.Y);
	}

	void Transformation::SetScale(float x, float y)
	{
		scale.X = x;
		scale.Y = y;
		needsUpdate = true;
	}

	void Transformation::SetScale(const Float2& scale)
	{
		SetScale(scale.X, scale.Y);
	}

	const Float2& Transformation::GetScale() const
	{
		return scale;
	}

	void Transformation::Scale(float factorX, float factorY)
	{
		scale.X *= factorX;
		scale.Y *= factorY;
		needsUpdate = true;
	}

	void Transformation::Scale(const Float2& factor)
	{
		Scale(factor.X, factor.Y);
	}

	void Transformation::SetRotation(const Angle& rotation)
	{
		this->rotation = rotation % Degrees(360.0f);
		needsUpdate = true;
	}

	const Angle& Transformation::GetRotation() const
	{
		return rotation;
	}

	void Transformation::Rotate(const Angle& angle)
	{
		SetRotation(rotation + angle);
	}

	const Matrix3x2& Transformation::GetTransform() const
	{
		if(needsUpdate)
		{
			const float rotation = this->rotation.ToRadians();
			const float cosine = std::cos(rotation);
			const float sine = std::sin(rotation);
			const float sxc = scale.X * cosine;
			const float syc = scale.Y * cosine;
			const float sxs = scale.X * sine;
			const float sys = scale.Y * sine;
			const float tx = position.X;
			const float ty = position.Y;

			transform = { .Data = {
				sxc, -sys,
				sxs, syc,
				tx, ty
			} };

			needsUpdate = false;
		}

		return transform;
	}
}
