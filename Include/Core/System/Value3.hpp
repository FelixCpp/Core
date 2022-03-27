// 
// Value3.hpp
// Core
// 
// Created by Felix Busch on 24.02.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#pragma once

#include <cstdint>

#include <Core/System/Value2.hpp>

namespace Core
{
	////////////////////////////////////////////////////////////
	/// \brief Define 3D coordinate/point/vector
	/// 
	////////////////////////////////////////////////////////////
	template<typename T>
	struct Value3
	{
		////////////////////////////////////////////////////////////
		/// \brief Default constructor
		/// 
		////////////////////////////////////////////////////////////
		constexpr Value3():
			X(T{}), Y(T{}), Z(T{})
		{}

		////////////////////////////////////////////////////////////
		/// \brief Construct a 3D value based on x, y, z values
		///
		///	\param x The x coordinate
		///	\param y The y coordinate
		///	\param z The z coordinate
		/// 
		////////////////////////////////////////////////////////////
		constexpr Value3(T x, T y, T z):
			X(x), Y(y), Z(z)
		{}

		constexpr explicit Value3(const Value2<T>& xy, T z):
			X(xy.X), Y(xy.Y), Z(z)
		{
		}

		constexpr explicit Value3(T x, const Value2<T>& yz) :
			X(x), Y(yz.X), Z(yz.Y)
		{
		}

		////////////////////////////////////////////////////////////
		/// \brief Construct 3D coordinate based on a single value
		///
		///	\param v The value to assign to X, Y and Z
		/// 
		////////////////////////////////////////////////////////////
		constexpr explicit Value3(T v):
			X(v), Y(v), Z(v)
		{}

		////////////////////////////////////////////////////////////
		/// \brief Convertible copy constructor
		/// 
		////////////////////////////////////////////////////////////
		template<typename O> requires std::is_convertible_v<O, T>
		constexpr Value3(const Value3<O>& other):
			X((T)other.X), Y((T)other.Y), Z((T)other.Z)
		{}

		T Dot(const Value3& other) const { return X * other.X + Y * other.Y + Z * other.Z; }
		T Length() const { return std::sqrt(X * X + Y * Y + Z * Z); }
		T LengthSq() const { return X * X + Y * Y + Z * Z; }
		T Max() const { return std::max({ X, Y, Z }); }
		T Min() const { return std::min({ X, Y, Z }); }

		Value3 Cross(const Value3& other) const { return { Y * other.Z - other.Y * Z, Z * other.X - other.Z * X, X * other.Y - other.X * Y }; }
		Value3 Floored() const { return { std::floor(X), std::floor(Y), std::floor(Z) }; }
		Value3 Ceiled() const { return { std::ceil(X), std::ceil(Y), std::ceil(Z) }; }
		Value3 Max(const T& value) const { return { std::max(X, value), std::max(Y, value), std::max(Z, value) }; }
		Value3 Min(const T& value) const { return { std::min(X, value), std::min(Y, value), std::min(Z, value) }; }
		Value3 Max(const Value3& other) const { return { std::max(X, other.X), std::max(Y, other.Y), std::max(Z, other.Z) }; }
		Value3 Min(const Value3& other) const { return { std::min(X, other.X), std::min(Y, other.Y), std::min(Z, other.Z) }; }
		Value3 Abs() const { return { std::abs(X), std::abs(Y), std::abs(Z), }; }
		Value3 Neg() const { return { -std::abs(X), -std::abs(Y), -std::abs(Z) }; }
		Value3 Normalized() const { const T mag = 1 / Length(); return { X * mag, Y * mag, Z * mag }; }

		////////////////////////////////////////////////////////////
		/// Member data
		/// 
		////////////////////////////////////////////////////////////
		union
		{
			struct
			{
				union
				{
					struct { T X, Y; };
					Value2<T> XY;
				};

				T Z;
			};

			struct
			{
				T X, Y, Z;
			} XYZ;
		};
	};

	////////////////////////////////////////////////////////////
	/// Creation operators
	/// 
	////////////////////////////////////////////////////////////
	template<typename T> constexpr Value3<T> operator | (const Value2<T>& xy, T z) { return Value3(xy, z); }
	template<typename T> constexpr Value3<T> operator | (T x, const Value2<T>& yz) { return Value3(x, yz); }

	////////////////////////////////////////////////////////////
	/// Sign operators
	/// 
	////////////////////////////////////////////////////////////
	template<typename T> constexpr Value3<T> operator + (const Value3<T>& value) { return { +value.X, +value.Y, +value.Z }; }
	template<typename T> constexpr Value3<T> operator - (const Value3<T>& value) { return { -value.X, -value.Y, -value.Z }; }

	////////////////////////////////////////////////////////////
	/// Mathematical operators on v3 => v3
	/// 
	////////////////////////////////////////////////////////////
	template<typename T> constexpr Value3<T> operator + (const Value3<T>& lhs, const Value3<T>& rhs) { return { lhs.X + rhs.X, lhs.Y + rhs.Y, lhs.Z + rhs.Z }; }
	template<typename T> constexpr Value3<T> operator - (const Value3<T>& lhs, const Value3<T>& rhs) { return { lhs.X - rhs.X, lhs.Y - rhs.Y, lhs.Z - rhs.Z }; }
	template<typename T> constexpr Value3<T> operator / (const Value3<T>& lhs, const Value3<T>& rhs) { return { lhs.X / rhs.X, lhs.Y / rhs.Y, lhs.Z / rhs.Z }; }
	template<typename T> constexpr Value3<T> operator * (const Value3<T>& lhs, const Value3<T>& rhs) { return { lhs.X * rhs.X, lhs.Y * rhs.Y, lhs.Z * rhs.Z }; }

	////////////////////////////////////////////////////////////
	/// Mathematical operators on v3 => value
	/// 
	///////////////////////////////////////////////////////////
	template<typename T> constexpr Value3<T> operator + (const Value3<T>& lhs, const T& value) { return { lhs.X + value, lhs.Y + value, lhs.Z + value }; }
	template<typename T> constexpr Value3<T> operator - (const Value3<T>& lhs, const T& value) { return { lhs.X - value, lhs.Y - value, lhs.Z - value }; }
	template<typename T> constexpr Value3<T> operator / (const Value3<T>& lhs, const T& value) { return { lhs.X / value, lhs.Y / value, lhs.Z / value }; }
	template<typename T> constexpr Value3<T> operator * (const Value3<T>& lhs, const T& value) { return { lhs.X * value, lhs.Y * value, lhs.Z * value }; }

	////////////////////////////////////////////////////////////
	/// Mathematical operators on v3 => v3
	/// 
	////////////////////////////////////////////////////////////
	template<typename T> constexpr Value3<T>& operator += (Value3<T>& lhs, const Value3<T>& rhs) { return lhs = lhs + rhs; }
	template<typename T> constexpr Value3<T>& operator -= (Value3<T>& lhs, const Value3<T>& rhs) { return lhs = lhs - rhs; }
	template<typename T> constexpr Value3<T>& operator /= (Value3<T>& lhs, const Value3<T>& rhs) { return lhs = lhs / rhs; }
	template<typename T> constexpr Value3<T>& operator *= (Value3<T>& lhs, const Value3<T>& rhs) { return lhs = lhs * rhs; }

	////////////////////////////////////////////////////////////
	/// Mathematical operators on v3 => value
	/// 
	////////////////////////////////////////////////////////////
	template<typename T> constexpr Value3<T>& operator += (Value3<T>& lhs, const T& value) { return lhs = lhs + value; }
	template<typename T> constexpr Value3<T>& operator -= (Value3<T>& lhs, const T& value) { return lhs = lhs - value; }
	template<typename T> constexpr Value3<T>& operator /= (Value3<T>& lhs, const T& value) { return lhs = lhs / value; }
	template<typename T> constexpr Value3<T>& operator *= (Value3<T>& lhs, const T& value) { return lhs = lhs * value; }

	////////////////////////////////////////////////////////////
	/// Mathematical operators on v3 => v3
	/// 
	////////////////////////////////////////////////////////////
	template<typename T> constexpr bool operator == (const Value3<T>& lhs, const Value3<T>& rhs) { return lhs.X == rhs.X && lhs.Y == rhs.Y && lhs.Z == rhs.Z; }
	template<typename T> constexpr bool operator != (const Value3<T>& lhs, const Value3<T>& rhs) { return lhs.X != rhs.X || lhs.Y != rhs.Y || lhs.Z != rhs.Z; }

	////////////////////////////////////////////////////////////
	/// Type definitions
	/// 
	////////////////////////////////////////////////////////////
	using Int3 = Value3<int32_t>;
	using UInt3 = Value3<uint32_t>;
	using Float3 = Value3<float>;
	using int3 = Value3<int32_t>;
	using uint3 = Value3<uint32_t>;
	using float3 = Value3<float>;

}