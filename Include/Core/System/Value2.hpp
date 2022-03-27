// 
// Value2.hpp
// Core
// 
// Created by Felix Busch on 16.02.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#pragma once

#include <type_traits>
#include <cmath>

namespace Core
{
	////////////////////////////////////////////////////////////
	/// \brief 2 dimensional value used as vector, coordinate etc.
	/// 
	////////////////////////////////////////////////////////////
	template<typename T>
	struct Value2
	{
		////////////////////////////////////////////////////////////
		/// \brief Default constructor
		/// 
		////////////////////////////////////////////////////////////
		constexpr Value2():
			X(T{}), Y(T{})
		{}

		////////////////////////////////////////////////////////////
		/// \brief Construct a 2D value with x and y coordinate
		///
		///	\param x X coordinate
		///	\param y Y coordinate
		/// 
		////////////////////////////////////////////////////////////
		constexpr Value2(T x, T y):
			X(x), Y(y)
		{}

		////////////////////////////////////////////////////////////
		/// \brief Copy constructor from another type
		/// 
		////////////////////////////////////////////////////////////
		template<typename O> requires std::is_convertible_v<O, T>
		constexpr Value2(const Value2<O>& other):
			X((T)other.X), Y((T)other.Y)
		{
		}

		T Dot(const Value2& other) const { return X * other.X + Y * other.Y; }
		T Cross(const Value2& other) const { return X * other.Y - Y * other.X; }
		T Length() const { return std::sqrt(X * X + Y * Y); }
		T LengthSq() const { return X * X + Y * Y; }
		T Max() const { return std::max(X, Y); }
		T Min() const { return std::min(X, Y); }

		Value2 Floored() const { return { std::floor(X), std::floor(Y) }; }
		Value2 Ceiled() const { return { std::ceil(X), std::ceil(Y) }; }
		Value2 Perpendicular() const { return { -Y, X }; }
		Value2 Cartesian() const { return { std::cos(Y) * X, std::sin(Y) * X }; }
		Value2 Max(const T& value) const { return { std::max(X, value), std::max(Y, value) }; }
		Value2 Min(const T& value) const { return { std::min(X, value), std::min(Y, value) }; }
		Value2 Max(const Value2& other) const { return { std::max(X, other.X), std::max(Y, other.Y) }; }
		Value2 Min(const Value2& other) const { return { std::min(X, other.X), std::min(Y, other.Y) }; }
		Value2 Abs() const { return { std::abs(X), std::abs(Y) }; }
		Value2 Neg() const { return { -std::abs(X), -std::abs(Y) }; }
		Value2 Normalized() const { const T mag = 1 / Length(); return { X * mag, Y * mag }; }

		////////////////////////////////////////////////////////////
		/// Member data
		/// 
		////////////////////////////////////////////////////////////
		union
		{
			struct
			{
				T X;
				T Y;
			};

			struct
			{
				T X;
				T Y;
			} XY;
		};

	};

	////////////////////////////////////////////////////////////
	/// Boolean operators
	/// 
	////////////////////////////////////////////////////////////
	template<typename T> constexpr bool operator == (const Value2<T>& lhs, const Value2<T>& rhs) { return lhs.X == rhs.X && lhs.Y == rhs.Y; }
	template<typename T> constexpr bool operator != (const Value2<T>& lhs, const Value2<T>& rhs) { return lhs.X != rhs.X || lhs.Y != rhs.Y; }

	////////////////////////////////////////////////////////////
	/// Mathematical operators
	/// 
	////////////////////////////////////////////////////////////
	template<typename T> constexpr Value2<T> operator + (const Value2<T>& lhs, const Value2<T>& rhs) { return { lhs.X + rhs.X, lhs.Y + rhs.Y }; }
	template<typename T> constexpr Value2<T> operator - (const Value2<T>& lhs, const Value2<T>& rhs) { return { lhs.X - rhs.X, lhs.Y - rhs.Y }; }
	template<typename T> constexpr Value2<T> operator / (const Value2<T>& lhs, const Value2<T>& rhs) { return { lhs.X / rhs.X, lhs.Y / rhs.Y }; }
	template<typename T> constexpr Value2<T> operator * (const Value2<T>& lhs, const Value2<T>& rhs) { return { lhs.X * rhs.X, lhs.Y * rhs.Y }; }
	template<typename T> constexpr Value2<T>& operator += (Value2<T>& lhs, const Value2<T>& rhs) { return lhs = lhs + rhs; }
	template<typename T> constexpr Value2<T>& operator -= (Value2<T>& lhs, const Value2<T>& rhs) { return lhs = lhs - rhs; }
	template<typename T> constexpr Value2<T>& operator /= (Value2<T>& lhs, const Value2<T>& rhs) { return lhs = lhs / rhs; }
	template<typename T> constexpr Value2<T>& operator *= (Value2<T>& lhs, const Value2<T>& rhs) { return lhs = lhs * rhs; }
	template<typename T> constexpr Value2<T> operator + (const Value2<T>& lhs, const T& value) { return { lhs.X + value, lhs.Y + value }; }
	template<typename T> constexpr Value2<T> operator - (const Value2<T>& lhs, const T& value) { return { lhs.X - value, lhs.Y - value }; }
	template<typename T> constexpr Value2<T> operator / (const Value2<T>& lhs, const T& value) { return { lhs.X / value, lhs.Y / value }; }
	template<typename T> constexpr Value2<T> operator * (const Value2<T>& lhs, const T& value) { return { lhs.X * value, lhs.Y * value }; }
	template<typename T> constexpr Value2<T>& operator += (Value2<T>& lhs, const T& value) { return lhs = lhs + value; }
	template<typename T> constexpr Value2<T>& operator -= (Value2<T>& lhs, const T& value) { return lhs = lhs - value; }
	template<typename T> constexpr Value2<T>& operator /= (Value2<T>& lhs, const T& value) { return lhs = lhs / value; }
	template<typename T> constexpr Value2<T>& operator *= (Value2<T>& lhs, const T& value) { return lhs = lhs * value; }
	template<typename T> constexpr Value2<T> operator + (const Value2<T>& value) { return { +value.X, +value.Y }; }
	template<typename T> constexpr Value2<T> operator - (const Value2<T>& value) { return { -value.X, -value.Y }; }

	////////////////////////////////////////////////////////////
	/// Type definitions
	/// 
	////////////////////////////////////////////////////////////
	using Int2   = Value2<int32_t>;
	using UInt2  = Value2<uint32_t>;
	using Float2 = Value2<float>;

	using int2   = Value2<int32_t>;
	using uint2  = Value2<uint32_t>;
	using float2 = Value2<float>;

}