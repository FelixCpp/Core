#pragma once

#include <array>
#include <string>

#include <Core/Maths/Vector2.hpp>
#include <Core/System/Datatypes.hpp>

namespace Core
{

	class Matrix3x3 {
	private:

		typedef std::array<std::array<float, 3>, 3> MatrixType;

	public:

		static const Matrix3x3 Identity;

		Matrix3x3();
		explicit Matrix3x3(float m00, float m10, float m20,
						   float m01, float m11, float m21,
						   float m02, float m12, float m22);

		Matrix3x3 & translate(const FVector2 & position);
		Matrix3x3 & rotate(float radians);
		Matrix3x3 & scale(const FVector2 & factor);
		Matrix3x3 & shear(const FVector2 & size);
		Matrix3x3 & multiply(const Matrix3x3 & other);
		Matrix3x3 & invert();

		FVector2 transformPoint(const FVector2 & point) const;

		std::string toString() const;

		const MatrixType & getMatrix() const;

		bool operator==(const Matrix3x3 & other) const;
		bool operator!=(const Matrix3x3 & other) const;
		Matrix3x3 operator*(const Matrix3x3 & other) const;
		Matrix3x3 & operator*=(const Matrix3x3 & other);

	private:

		MatrixType matrix;

	};

}