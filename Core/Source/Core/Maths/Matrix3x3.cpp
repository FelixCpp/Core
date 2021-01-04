#include <Core/Maths/Matrix3x3.hpp>

#include <sstream>

namespace Core
{

	const Matrix3x3 Matrix3x3::Identity;

	Matrix3x3::Matrix3x3() :
		matrix()
	{
		this->matrix[0][0] = 1.f; this->matrix[1][0] = 0.f; this->matrix[2][0] = 0.f;
		this->matrix[0][1] = 0.f; this->matrix[1][1] = 1.f; this->matrix[2][1] = 0.f;
		this->matrix[0][2] = 0.f; this->matrix[1][2] = 0.f; this->matrix[2][2] = 1.f;
	}

	Matrix3x3::Matrix3x3(float m00, float m10, float m20,
						 float m01, float m11, float m21,
						 float m02, float m12, float m22) :
		matrix()
	{
		this->matrix[0][0] = m00; this->matrix[1][0] = m10; this->matrix[2][0] = m20;
		this->matrix[0][1] = m01; this->matrix[1][1] = m11; this->matrix[2][1] = m21;
		this->matrix[0][2] = m02; this->matrix[1][2] = m12; this->matrix[2][2] = m22;
	}

	Matrix3x3 & Matrix3x3::translate(const FVector2 & position)
	{
		const Matrix3x3 translation = Matrix3x3(
			1.f, 0.f, position.x,
			0.f, 1.f, position.y,
			0.f, 0.f, 1.f
		);

		return this->multiply(translation);
	}

	Matrix3x3 & Matrix3x3::rotate(float radians)
	{
		const float cos = std::cosf(radians);
		const float sin = std::sinf(radians);

		const Matrix3x3 rotation = Matrix3x3(
			 cos, sin, 0.f,
			-sin, cos, 0.f,
			 0.f, 0.f, 1.f
		);

		return this->multiply(rotation);
	}

	Matrix3x3 & Matrix3x3::scale(const FVector2 & factor)
	{
		const Matrix3x3 scaled = Matrix3x3(
			factor.x, 0.f, 0.f,
			0.f, factor.y, 0.f,
			0.f, 0.f, 1.f
		);

		return this->multiply(scaled);
	}

	Matrix3x3 & Matrix3x3::shear(const FVector2 & size)
	{
		const Matrix3x3 sheared = Matrix3x3(
			1.f, size.width, 0.f,
			size.height, 1.f, 0.f,
			0.f, 0.f, 1.f
		);

		return this->multiply(sheared);
	}

	Matrix3x3 & Matrix3x3::multiply(const Matrix3x3 & other)
	{
		MatrixType results;

		const MatrixType & a = this->matrix;
		const MatrixType & b = other.matrix;

		for (int col = 0; col < 3; col++)
		{
			for (int row = 0; row < 3; row++)
			{
				results[col][row] =
					a[0][row] * b[col][0] +
					a[1][row] * b[col][1] +
					a[2][row] * b[col][2];
			}
		}

		this->matrix = results;
		return *this;
	}

	Matrix3x3 & Matrix3x3::invert()
	{
		const auto & o = this->matrix;
		const float det =
			o[0][0] * (o[1][1] * o[2][2] - o[1][2] * o[2][1]) -
			o[1][0] * (o[0][1] * o[2][2] - o[2][1] * o[0][2]) +
			o[2][0] * (o[0][1] * o[1][2] - o[1][1] * o[0][2]);

		if (det != 0.f)
		{
			const float idet = 1.f / det;

			MatrixType results;
			results[0][0] = (o[1][1] * o[2][2] - o[1][2] * o[2][1]) * idet;
			results[1][0] = (o[2][0] * o[1][2] - o[1][0] * o[2][2]) * idet;
			results[2][0] = (o[1][0] * o[2][1] - o[2][0] * o[1][1]) * idet;
			results[0][1] = (o[2][1] * o[0][2] - o[0][1] * o[2][2]) * idet;
			results[1][1] = (o[0][0] * o[2][2] - o[2][0] * o[0][2]) * idet;
			results[2][1] = (o[0][1] * o[2][0] - o[0][0] * o[2][1]) * idet;
			results[0][2] = (o[0][1] * o[1][2] - o[0][2] * o[1][1]) * idet;
			results[1][2] = (o[0][2] * o[1][0] - o[0][0] * o[1][2]) * idet;
			results[2][2] = (o[0][0] * o[1][1] - o[0][1] * o[1][0]) * idet;
			
			this->matrix = results;
		}

		return *this;
	}

	FVector2 Matrix3x3::transformPoint(const FVector2 & point) const
	{
		FVector2 transformedPoint;
		transformedPoint.x = point.x * this->matrix[0][0] + point.y * this->matrix[1][0] + this->matrix[2][0];
		transformedPoint.y = point.x * this->matrix[0][1] + point.y * this->matrix[1][1] + this->matrix[2][1];
		return transformedPoint;
	}

	std::string Matrix3x3::toString() const
	{
		std::stringstream builder;

		for (int row = 0; row < 3; row++)
		{
			for (int col = 0; col < 3; col++)
			{
				builder << this->matrix[col][row];

				if (!(col == 2 && row == 2))
				{
					builder << ", ";
				}
			}

			builder << "\n";
		}

		return builder.str();
	}

	const Matrix3x3::MatrixType & Matrix3x3::getMatrix() const
	{
		return this->matrix;
	}

	bool Matrix3x3::operator==(const Matrix3x3 & other) const
	{
		return this->matrix == other.matrix;
	}

	bool Matrix3x3::operator!=(const Matrix3x3 & other) const
	{
		return this->matrix != other.matrix;
	}

	Matrix3x3 Matrix3x3::operator*(const Matrix3x3 & other) const
	{
		return Matrix3x3(*this).multiply(other);
	}

	Matrix3x3 & Matrix3x3::operator*=(const Matrix3x3 & other)
	{
		return this->multiply(other);
	}

}