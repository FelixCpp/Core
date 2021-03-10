#pragma once

#include <cmath>
#include <string>
#include <type_traits>

#include <Core/Maths/Random.hpp>
#include <Core/Maths/Math.hpp>

#include <Core/System/Datatypes.hpp>

namespace Core
{

	template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
	struct Vector3 {

		union {
			T x;
			T width;
		};

		union {
			T y;
			T height;
		};

		union {
			T z;
			T depth;
		};

		inline Vector3() :
			x(T()), y(T()), z(T())
		{
		}

		inline Vector3(T x, T y, T z) :
			x(x), y(y), z(z)
		{
		}

		template<typename TOther>
		inline Vector3(const Vector3<TOther> & other) :
			x(static_cast<T>(other.x)),
			y(static_cast<T>(other.y)),
			z(static_cast<T>(other.z))
		{
		}

		T Distance(const Vector3 & point) const
		{
			return (*this - point).Length();
		}

		T Length() const
		{
			return std::sqrt(this->LengthSq());
		}

		T LengthSq() const
		{
			return (this->x * this->x) + (this->y * this->y) + (this->z * this->z);
		}

		Vector3 Cross(const Vector3 & other) const
		{
			const T crossX = this->y * other.z - other.y * this->z;
			const T crossY = this->z * other.x - other.z * this->x;
			const T crossZ = this->x * other.y - other.x * this->y;

			return Vector3(crossX, crossY, crossZ);
		}

		float Heading2D() const
		{
			return std::atan2(this->y, this->x);
		}

		Vector3 & Normalize()
		{
			const T length = this->Length();
			if (length != (T)0.0 && length != (T)1.0)
			{
				this->x /= length;
				this->y /= length;
				this->z /= length;
			}

			return *this;
		}

		Vector3 Normalized() const
		{
			return Vector3(*this).Normalize();
		}

		Vector3 & SetLength(const T & length)
		{
			this->Normalize();
			return *this *= length;
		}

		static Vector3 FromAngle(const T & radians)
		{
			return Vector3(std::cos(radians), std::sin(radians), 0.f);
		}

		static Vector3 Random2D()
		{
			return Vector3::FromAngle(Random::Get(FMath::TwoPi));
		}

		inline Vector3 operator+(const Vector3 & other) const { return Vector3(this->x + other.x, this->y + other.y, this->z + other.z); }
		inline Vector3 operator-(const Vector3 & other) const { return Vector3(this->x - other.x, this->y - other.y, this->z - other.z); }
		inline Vector3 operator/(const Vector3 & other) const { return Vector3(this->x / other.x, this->y / other.y, this->z / other.z); }
		inline Vector3 operator*(const Vector3 & other) const { return Vector3(this->x * other.x, this->y * other.y, this->z * other.z); }

		inline Vector3 operator+(T value) const { return Vector3(this->x + value, this->y + value, this->z + value); }
		inline Vector3 operator-(T value) const { return Vector3(this->x - value, this->y - value, this->z - value); }
		inline Vector3 operator/(T value) const { return Vector3(this->x / value, this->y / value, this->z / value); }
		inline Vector3 operator*(T value) const { return Vector3(this->x * value, this->y * value, this->z * value); }

		inline Vector3 & operator+=(const Vector3 & other) { return *this = *this + other; }
		inline Vector3 & operator-=(const Vector3 & other) { return *this = *this - other; }
		inline Vector3 & operator/=(const Vector3 & other) { return *this = *this / other; }
		inline Vector3 & operator*=(const Vector3 & other) { return *this = *this * other; }

		inline Vector3 & operator+=(T value) { return *this = *this + value; }
		inline Vector3 & operator-=(T value) { return *this = *this - value; }
		inline Vector3 & operator/=(T value) { return *this = *this / value; }
		inline Vector3 & operator*=(T value) { return *this = *this * value; }

		inline bool operator==(const Vector3 & other) const { return this->x == other.x && this->y == other.y && this->z == other.z; }
		inline bool operator!=(const Vector3 & other) const { return !(*this == other); }

		inline bool operator==(T value) const { return this->x == value && this->y == value && this->z == value; }
		inline bool operator!=(T value) const { return !(*this == value); }

		inline std::string ToString() const
		{
			return '[' + std::to_string(this->x) + ", " + std::to_string(this->y) + ","  + std::to_string(this->z) + ']';
		}

	};

	typedef Vector3<float> FVector3;
	typedef Vector3<double> DVector3;
	typedef Vector3<Int32> IVector3;
	typedef Vector3<UInt32> UVector3;

}