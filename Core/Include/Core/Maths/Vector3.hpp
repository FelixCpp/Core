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

		T distance(const Vector3 & point) const
		{
			return (*this - point).length();
		}

		T length() const
		{
			return std::sqrt(this->lengthSq());
		}

		T lengthSq() const
		{
			return (this->x * this->x) + (this->y * this->y) + (this->z * this->z);
		}

		Vector3 cross(const Vector3 & other) const
		{
			const float crossX = this->y * other.z - other.y * this->z;
			const float crossY = this->z * other.x - other.z * this->x;
			const float crossZ = this->x * other.y - other.x * this->y;

			return Vector3(crossX, crossY, crossZ);
		}

		float heading2D() const
		{
			return std::atan2(this->y, this->x);
		}

		Vector3 & normalize()
		{
			const T length = this->length();
			if (length != (T)0.0 && length != (T)1.0)
			{
				this->x /= length;
				this->y /= length;
				this->z /= length;
			}

			return *this;
		}

		Vector3 normalized() const
		{
			return Vector3(*this).normalize();
		}

		Vector3 & setLength(const T & length)
		{
			this->normalize();
			return *this *= length;
		}

		static Vector3 fromAngle(const T & radians)
		{
			return Vector3(std::cos(radians), std::sin(radians), 0.f);
		}

		static Vector3 random2D()
		{
			return Vector3::fromAngle(Random::get(FMath::TwoPi));
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

		inline std::string toString() const
		{
			return '[' + std::to_string(this->x) + ", " + std::to_string(this->y) + ","  + std::to_string(this->z) + ']';
		}

	};

	typedef Vector3<float> FVector3;
	typedef Vector3<double> DVector3;
	typedef Vector3<i32_t> IVector3;
	typedef Vector3<u32_t> UVector3;

}