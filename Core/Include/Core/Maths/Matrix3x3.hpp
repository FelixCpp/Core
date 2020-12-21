#pragma once

#include <type_traits>
#include <sstream>
#include <array>
#include <cmath>

#include <Core/System/Datatypes.hpp>

namespace Core
{

	template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
	class Matrix3x3 {
	public:

		static const Matrix3x3 Identity;

		inline Matrix3x3() :
			matrix({ 1, 0, 0,
					 0, 1, 0,
					 0, 0, 1 })
		{ }

		inline Matrix3x3(T m11, T m12, T m13,
						 T m21, T m22, T m23,
						 T m31, T m32, T m33) :
			matrix({ m11, m12, m13,
				     m21, m22, m23,
				     m31, m32, m33 })
		{ }

		inline Matrix3x3 & translate(T x, T y, T z)
		{
			return this->combine(Matrix3x3(
				1, 0, x,
				0, 1, y,
				0, 0, z
			));
		}

		inline Matrix3x3 & rotateZ(float radians)
		{
			const auto cos = std::cos(radians);
			const auto sin = std::sin(radians);

			return this->combine(Matrix3x3(
				cos, -sin, 0,
				sin,  cos, 0,
				0,    0,   1
			));
		}

		inline Matrix3x3 & rotateX(float radians)
		{
			const auto cos = std::cos(radians);
			const auto sin = std::sin(radians);

			return this->combine(Matrix3x3(
				1, 0,    0,
				0, cos, -sin,
				0, sin,  cos
			));
		}


		inline Matrix3x3 & rotateY(float radians)
		{
			const auto cos = std::cos(radians);
			const auto sin = std::sin(radians);

			return this->combine(Matrix3x3(
				 cos, 0, sin,
				 0,   1, 0,
				-sin, 0, cos
			));
		}

		inline Matrix3x3 & scale(T factorX, T factorY, T factorZ)
		{
			return this->combine(Matrix3x3(
				factorX, 0, 0,
				0, factorY, 0,
				0, 0, factorZ
			));
		}

		inline Matrix3x3 & combine(const Matrix3x3 & other)
		{
			const T * a = this->matrix.data();
			const T * b = other.matrix.data();

			return *this = Matrix3x3(
				a[0] * b[0] + a[1] * b[3] + a[2] * b[6],
				a[0] * b[1] + a[1] * b[4] + a[2] * b[7],
				a[0] * b[2] + a[1] * b[5] + a[2] * b[8],

				a[3] * b[0] + a[4] * b[3] + a[5] * b[6],
				a[3] * b[1] + a[4] * b[4] + a[5] * b[7],
				a[3] * b[2] + a[4] * b[5] + a[5] * b[8],

				a[6] * b[0] + a[7] * b[3] + a[8] * b[6],
				a[6] * b[1] + a[7] * b[4] + a[8] * b[7],
				a[6] * b[2] + a[7] * b[5] + a[8] * b[8]
			);
		}

		inline std::string toString() const
		{
			std::ostringstream builder;
			
			for (int y = 0; y < 3; y++)
			{
				for (int x = 0; x < 3; x++)
				{
					builder << this->matrix[y * 3 + x];

					if (!(x == 2 && y == 2))
						builder << ", ";
				}

				builder << '\n';
			}

			return builder.str();
		}

	private:

		std::array<T, 3 * 3> matrix;

	};

	template<typename T, typename _ = std::enable_if_t<std::is_arithmetic_v<T>>>
	const Matrix3x3<T, _> Matrix3x3<T, _>::Identity;

	typedef Matrix3x3<float> FMatrix3x3;
	typedef Matrix3x3<double> DMatrix3x3;
	typedef Matrix3x3<i32> IMatrix3x3;
	typedef Matrix3x3<u32> UMatrix3x3;

}