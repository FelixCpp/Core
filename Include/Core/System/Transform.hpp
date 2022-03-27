// 
// Transform.hpp
// Core
// 
// Created by Felix Busch on 17.02.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#pragma once

#include <Core/System/Value2.hpp>
#include <Core/System/Rectangle.hpp>
#include <Core/System/Angle.hpp>

#include <algorithm>

namespace Core
{
	////////////////////////////////////////////////////////////
	/// \brief Define 4x4 matrix wrapper
	/// 
	////////////////////////////////////////////////////////////
	class Transform
	{
	public:
		////////////////////////////////////////////////////////////
		/// \brief Identity matrix
		/// 
		////////////////////////////////////////////////////////////
		static const Transform Identity;

		////////////////////////////////////////////////////////////
		/// \brief Default constructor. Initializes an identity
		///		   matrix
		/// 
		////////////////////////////////////////////////////////////
		constexpr Transform():
			matrix{
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			}
		{
		}

		////////////////////////////////////////////////////////////
		/// \brief Constructs a 3x3 matrix
		/// 
		////////////////////////////////////////////////////////////
		constexpr Transform(
			float m00, float m01, float m02,
			float m10, float m11, float m12,
			float m20, float m21, float m22
		)
		{
			matrix[0] = m00;  matrix[4] = m01;  matrix[8]  = 0.0f; matrix[12] = m02;
			matrix[1] = m10;  matrix[5] = m11;  matrix[9]  = 0.0f; matrix[13] = m12;
			matrix[2] = 0.0f; matrix[6] = 0.0f; matrix[10] = 1.0f; matrix[14] = 0.0f;
			matrix[3] = m20;  matrix[7] = m21;  matrix[11] = 0.0f; matrix[15] = m22;
		}

		////////////////////////////////////////////////////////////
		/// \brief Create transform from 4x4 float array
		/// 
		////////////////////////////////////////////////////////////
		constexpr Transform(const float(&data)[16])
		{
			std::ranges::copy_n(data, 16, matrix);
		}

		////////////////////////////////////////////////////////////
		/// \brief Constructs a 4x4 matrix
		/// 
		////////////////////////////////////////////////////////////
		constexpr Transform(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33
		)
		{
			matrix[0] = m00; matrix[4] = m01; matrix[8]  = m02; matrix[12] = m03;
			matrix[1] = m10; matrix[5] = m11; matrix[9]  = m12; matrix[13] = m13;
			matrix[2] = m20; matrix[6] = m21; matrix[10] = m22; matrix[14] = m23;
			matrix[3] = m30; matrix[7] = m31; matrix[11] = m32; matrix[15] = m33;
		}

		////////////////////////////////////////////////////////////
		/// \brief Translate the matrix to the given coordinate
		/// 
		////////////////////////////////////////////////////////////
		constexpr Transform& Translate(float x, float y)
		{
			return Multiply(Translation(x, y));
		}

		constexpr Transform& Translate(const Float2 & position)
		{
			return Translate(position.X, position.Y);
		}

		////////////////////////////////////////////////////////////
		/// \brief Scales the matrix by the given factor
		/// 
		////////////////////////////////////////////////////////////
		constexpr Transform& Scale(float factorX, float factorY)
		{
			return Multiply(Scaling(factorX, factorY));
		}

		constexpr Transform& Scale(const Float2& factor)
		{
			return Scale(factor.X, factor.Y);
		}

		////////////////////////////////////////////////////////////
		/// \brief Scales the matrix by the given factor
		/// 
		////////////////////////////////////////////////////////////
		Transform& Rotate(const Angle& angle);
		Transform& Rotate(const Angle& angle, float centerX, float centerY);
		Transform& Rotate(const Angle& angle, const Float2& center);
		
		////////////////////////////////////////////////////////////
		/// \brief Combine metrics
		/// 
		////////////////////////////////////////////////////////////
		constexpr Transform& Multiply(const Transform& transform)
		{
			const float* a = matrix;
			const float* b = transform.matrix;

			// store result in output
			float output[4 * 4]{};
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					float sum = 0;
					for (int k = 0; k < 4; k++)
						sum += a[i * 4 + k] * b[k * 4 + j];
					output[i * 4 + j] = sum;
				}
			}

			// copy values
			std::ranges::copy_n(output, 16, matrix);
			return *this;
		}

		////////////////////////////////////////////////////////////
		/// \brief Compute the inverse matrix
		/// 
		////////////////////////////////////////////////////////////
		[[nodiscard]] constexpr Transform Inverse() const
		{
			const float* m = matrix;
			float inv[16]{};

			inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
			inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
			inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
			inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];

			const float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
			if (det == 0.0f)
				return *this;

			inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
			inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
			inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
			inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
			inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
			inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
			inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
			inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
			inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
			inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
			inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
			inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];
			
			const float determinant = 1.0f / det;
			
			float invOut[16]{};
			std::ranges::transform(inv, invOut, [&](float value)
			{
				return value * determinant;
			});

			return { invOut };
		}

		////////////////////////////////////////////////////////////
		/// \brief Transformations
		/// 
		////////////////////////////////////////////////////////////
		constexpr static Transform Translation(float x, float y)
		{
			return {
				1.0f, 0.0f, x,
				0.0f, 1.0f, y,
				0.0f, 0.0f, 1.0f
			};
		}

		constexpr static Transform Translation(const Float2& position)
		{
			return Translation(position.X, position.Y);
		}

		////////////////////////////////////////////////////////////
		/// \brief Scalings
		/// 
		////////////////////////////////////////////////////////////
		constexpr static Transform Scaling(float factorX, float factorY)
		{
			return {
				factorX, 0.0f, 0.0f,
				0.0f, factorY, 0.0f,
				0.0f, 0.0f, 1.0f
			};
		}

		constexpr static Transform Scaling(const Float2& factor)
		{
			return Scaling(factor.X, factor.Y);
		}

		////////////////////////////////////////////////////////////
		/// \brief Rotations
		/// 
		////////////////////////////////////////////////////////////
		static Transform Rotation(const Angle& angle);
		static Transform Rotation(const Angle& angle, float centerX, float centerY);
		static Transform Rotation(const Angle& angle, const Float2& center);

		////////////////////////////////////////////////////////////
		/// \brief Point Transformations
		/// 
		////////////////////////////////////////////////////////////
		[[nodiscard]] constexpr Float2 TransformPoint(float x, float y) const
		{
			return
			{
				matrix[0] * x + matrix[4] * y + matrix[12],
				matrix[1] * x + matrix[5] * y + matrix[13]
			};
		}

		[[nodiscard]] constexpr Float2 TransformPoint(const Float2& point) const
		{
			return TransformPoint(point.X, point.Y);
		}

		////////////////////////////////////////////////////////////
		/// \brief Transform the four corner points of the given rectangle
		/// 
		////////////////////////////////////////////////////////////
		[[nodiscard]] constexpr FloatRect TransformRect(const FloatRect& rectangle) const
		{
			// Transform the four corners of the rectangle
			const Float2 points[]
			{
				TransformPoint(rectangle.Left, rectangle.Top),
				TransformPoint(rectangle.Left, rectangle.Top + rectangle.Height),
				TransformPoint(rectangle.Left + rectangle.Width, rectangle.Top),
				TransformPoint(rectangle.Left + rectangle.Width, rectangle.Top + rectangle.Height),
			};

			// Compute the bounding rectangle of the transformed points
			float left = points[0].X;
			float top = points[0].Y;
			float right = points[0].X;
			float bottom = points[0].Y;
			for (int i = 1; i < 4; ++i)
			{
				const Float2& point = points[i];
				left   = std::min(left, point.X);
				top    = std::min(top, point.Y);
				right  = std::max(right, point.X);
				bottom = std::max(bottom, point.Y);
			}
			
			return { left, top, (right - left), (bottom - top) };
		}

		////////////////////////////////////////////////////////////
		/// \brief Creates orthographic projection transformation
		///
		///	\param viewport	The viewport that contains the left, top, right and bottom edges
		///	\param zNear	near coordinate
		///	\param zFar		far coordinate
		///
		///	\return Orthographic projection transformation
		/// 
		////////////////////////////////////////////////////////////
		static Transform Orthographic(const FloatRect& viewport, float zNear, float zFar)
		{
			const float left = viewport.Left;
			const float top = viewport.Top;
			const float right = left + viewport.Width;
			const float bottom = top + viewport.Height;
			
			const float m00 =  2.0f / (right - left);
			const float m11 =  2.0f / (top - bottom);
			const float m22 = -2.0f / (zFar - zNear);

			const float m03 = -((right + left) / (right - left));
			const float m13 = -((top + bottom) / (top - bottom));
			const float m23 = -((zFar + zNear) / (zFar - zNear));

			return
			{
				m00, 0.0f, 0.0f, m03,
				0.0f, m11, 0.0f, m13,
				0.0f, 0.0f, m22, m23,
				0.0f, 0.0f, 0.0f, 1.0f
			};
		}

		////////////////////////////////////////////////////////////
		/// \brief Get the data of the matrix
		/// 
		////////////////////////////////////////////////////////////
		[[nodiscard]] constexpr const float(&GetData() const)[16]
		{
			return matrix;
		}

		////////////////////////////////////////////////////////////
		/// Boolean operators
		/// 
		////////////////////////////////////////////////////////////
		constexpr friend bool operator == (const Transform& lhs, const Transform& rhs);
		constexpr friend bool operator != (const Transform& lhs, const Transform& rhs);

	private:

		////////////////////////////////////////////////////////////
		/// Member data
		/// 
		////////////////////////////////////////////////////////////
		float matrix[4 * 4]{};

	};

	////////////////////////////////////////////////////////////
	/// Mathematical operators
	/// 
	////////////////////////////////////////////////////////////
	constexpr Transform operator * (const Transform& lhs, const Transform& rhs)
	{
		return Transform(lhs).Multiply(rhs);
	}

	constexpr Transform& operator *= (Transform& lhs, const Transform& rhs)
	{
		return lhs = lhs * rhs;
	}

	////////////////////////////////////////////////////////////
	/// Boolean operators
	/// 
	////////////////////////////////////////////////////////////
	constexpr bool operator == (const Transform& lhs, const Transform& rhs)
	{
		return std::ranges::equal(lhs.matrix, rhs.matrix);
	}

	constexpr bool operator != (const Transform& lhs, const Transform& rhs)
	{
		return !(lhs == rhs);
	}

}