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
	struct Vector2 {

		union {
			T x;
			T width;
		};

		union {
			T y;
			T height;
		};

		inline Vector2() :
			x(T()), y(T())
		{ }

		inline Vector2(T x, T y) :
			x(x), y(y)
		{ }

		template<typename TOther>
		inline Vector2(const Vector2<TOther> & other) :
			x(static_cast<T>(other.x)),
			y(static_cast<T>(other.y))
		{ }

		T Distance(const Vector2 & point) const
		{
			return (*this - point).Length();
		}

		T Length() const
		{
			return std::sqrt(this->LengthSq());
		}

		T LengthSq() const
		{
			return (this->x * this->x) + (this->y * this->y);
		}

		T Cross(const Vector2 & other) const
		{
			return this->x * other.y - this->y * other.x;
		}

		float Heading() const
		{
			return std::atan2(this->y, this->x);
		}

		Vector2 & Normalize()
		{
			const T length = this->Length();
			if (length != (T)0.0 && length != (T)1.0)
			{
				this->x /= length;
				this->y /= length;
			}

			return *this;
		}

		Vector2 Normalized() const
		{
			return Vector2(*this).Normalize();
		}

		Vector2 & SetLength(const T & length)
		{
			this->Normalize();
			return *this *= length;
		}

		static Vector2 FromAngle(const T & radians)
		{
			return Vector2(std::cos(radians), std::sin(radians));
		}

		static Vector2 Random2D()
		{
			return Vector2::FromAngle(Random::Get(FMath::TwoPi));
		}

		inline Vector2 operator+(const Vector2 & other) const { return Vector2(this->x + other.x, this->y + other.y); }
		inline Vector2 operator-(const Vector2 & other) const { return Vector2(this->x - other.x, this->y - other.y); }
		inline Vector2 operator/(const Vector2 & other) const { return Vector2(this->x / other.x, this->y / other.y); }
		inline Vector2 operator*(const Vector2 & other) const { return Vector2(this->x * other.x, this->y * other.y); }

		inline Vector2 operator+(T value) const { return Vector2(this->x + value, this->y + value); }
		inline Vector2 operator-(T value) const { return Vector2(this->x - value, this->y - value); }
		inline Vector2 operator/(T value) const { return Vector2(this->x / value, this->y / value); }
		inline Vector2 operator*(T value) const { return Vector2(this->x * value, this->y * value); }

		inline Vector2 & operator+=(const Vector2 & other) { return *this = *this + other; }
		inline Vector2 & operator-=(const Vector2 & other) { return *this = *this - other; }
		inline Vector2 & operator/=(const Vector2 & other) { return *this = *this / other; }
		inline Vector2 & operator*=(const Vector2 & other) { return *this = *this * other; }

		inline Vector2 & operator+=(T value) { return *this = *this + value; }
		inline Vector2 & operator-=(T value) { return *this = *this - value; }
		inline Vector2 & operator/=(T value) { return *this = *this / value; }
		inline Vector2 & operator*=(T value) { return *this = *this * value; }

		inline bool operator==(const Vector2 & other) const { return this->x == other.x && this->y == other.y; }
		inline bool operator!=(const Vector2 & other) const { return !(*this == other); }
		
		inline bool operator==(T value) const { return this->x == value && this->y == value; }
		inline bool operator!=(T value) const { return !(*this == value); }
		
		inline std::string ToString() const
		{
			return '[' + std::to_string(this->x) + ", " + std::to_string(this->y) + ']';
		}

	};

	typedef Vector2<float> FVector2;
	typedef Vector2<double> DVector2;
	typedef Vector2<Int32> IVector2;
	typedef Vector2<UInt32> UVector2;

}