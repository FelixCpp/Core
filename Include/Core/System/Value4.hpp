// 
// Value4.hpp
// Core
// 
// Created by Felix Busch on 24.02.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#pragma once

#include <Core/System/Value2.hpp>
#include <Core/System/Value3.hpp>

#include <cstdint>

namespace Core
{

	template<typename T>
	class Value4
	{
	public:

		////////////////////////////////////////////////////////////
		/// \brief Default constructor
		/// 
		////////////////////////////////////////////////////////////
		constexpr Value4():
			X(T{}), Y(T{}), Z(T{}), W(T{})
		{
		}

		constexpr Value4(T x, T y, T z, T w) :
			X(x), Y(y), Z(z), W(w)
		{
		}

		constexpr explicit Value4(const Value2<T>& xy, T z, T w):
			Value4(xy.X, xy.Y, z, w)
		{}

		constexpr explicit Value4(T x, const Value2<T>& yz, T w):
			Value4(x, yz.X, yz.Y, w)
		{}

		constexpr explicit Value4(T x, T y, const Value2<T>& zw):
			Value4(x, y, zw.X, zw.Y)
		{}

		constexpr explicit Value4(const Value2<T>& xy, const Value2<T>& zw):
			Value4(xy.X, xy.Y, zw.X, zw.Y)
		{}

		constexpr explicit Value4(const Value3<T>& xyz, T w):
			Value4(xyz.X, xyz.Y, xyz.Z, w)
		{}

		constexpr explicit Value4(T x, const Value3<T>& yzw):
			Value4(x, yzw.X, yzw.Y, yzw.Z)
		{}

		template<typename O> requires std::is_convertible_v<O, T>
		constexpr explicit Value4(const Value4<O>& other):
			X((T)other.X), Y((T)other.Y), Z((T)other.Z), W((T)other.W)
		{}

		T Dot(const Value4& other) const { return X * other.X + Y * other.Y + Z * other.Z + W * other.W; }
		T LengthSq() const { return Dot(*this); }
		T Length() const { return std::sqrt(LengthSq()); }
		Value4 Floored() const { return { std::floor(X), std::floor(Y), std::floor(Z), std::floor(W) }; }
		Value4 Ceiled() const { return { std::ceil(X), std::ceil(Y), std::ceil(Z), std::ceil(W) }; }
		Value4 Max(const Value4& other) const { return { std::max(X, other.X), std::max(Y, other.Y), std::max(Z, other.Z), std::max(W, other.W) }; }
		Value4 Min(const Value4& other) const { return { std::min(X, other.X), std::min(Y, other.Y), std::min(Z, other.Z), std::min(W, other.W) }; }
		Value4 Abs() const { return { std::abs(X), std::abs(Y), std::abs(Z), std::abs(W) }; }
		Value4 Neg() const { return { -std::abs(X), -std::abs(Y), -std::abs(Z), -std::abs(W) }; }

		Value4 Normalized() const
		{
			const T length = Length();
			if(length != (T)0.0)
			{
				return *this / length;
			}

			return *this;
		}

		////////////////////////////////////////////////////////////
		/// Member data
		/// 
		////////////////////////////////////////////////////////////
		union
		{
			struct // X-Y-Z-W
			{
				union
				{
					struct // X-Y-Z
					{
						union // XY
						{
							struct { T X, Y; };
							Value2<T> XY;
						};

						T Z;
					};

					Value3<T> XYZ;
				};

				T W;
			};

			struct // XYZW
			{
				T X, Y, Z, W;
			} XYZW;
		};
	};

	////////////////////////////////////////////////////////////
	/// Boolean operators
	/// 
	////////////////////////////////////////////////////////////
	template<typename T> constexpr bool operator == (const Value4<T>& lhs, const Value4<T>& rhs) { return lhs.X == rhs.R && lhs.Y == rhs.Y && lhs.Z == rhs.Z && lhs.W == rhs.W; }
	template<typename T> constexpr bool operator != (const Value4<T>& lhs, const Value4<T>& rhs) { return lhs.X != rhs.R || lhs.Y != rhs.Y || lhs.Z != rhs.Z || lhs.W != rhs.W; }

	////////////////////////////////////////////////////////////
	/// Mathematical operators v4 => v4
	/// 
	////////////////////////////////////////////////////////////
	template<typename T> constexpr Value4<T> operator + (const Value4<T>& lhs, const Value4<T>& rhs) { return { lhs.X + rhs.X, lhs.Y + rhs.Y, lhs.Z + rhs.Z, lhs.W + rhs.W }; }
	template<typename T> constexpr Value4<T> operator - (const Value4<T>& lhs, const Value4<T>& rhs) { return { lhs.X - rhs.X, lhs.Y - rhs.Y, lhs.Z - rhs.Z, lhs.W - rhs.W }; }
	template<typename T> constexpr Value4<T> operator / (const Value4<T>& lhs, const Value4<T>& rhs) { return { lhs.X / rhs.X, lhs.Y / rhs.Y, lhs.Z / rhs.Z, lhs.W / rhs.W }; }
	template<typename T> constexpr Value4<T> operator * (const Value4<T>& lhs, const Value4<T>& rhs) { return { lhs.X * rhs.X, lhs.Y * rhs.Y, lhs.Z * rhs.Z, lhs.W * rhs.W }; }

	////////////////////////////////////////////////////////////
	/// Mathematical operators v4& => v4
	/// 
	////////////////////////////////////////////////////////////
	template<typename T> constexpr Value4<T>& operator += (Value4<T>& lhs, const Value4<T>& rhs) { return lhs = lhs + rhs; }
	template<typename T> constexpr Value4<T>& operator -= (Value4<T>& lhs, const Value4<T>& rhs) { return lhs = lhs - rhs; }
	template<typename T> constexpr Value4<T>& operator /= (Value4<T>& lhs, const Value4<T>& rhs) { return lhs = lhs / rhs; }
	template<typename T> constexpr Value4<T>& operator *= (Value4<T>& lhs, const Value4<T>& rhs) { return lhs = lhs * rhs; }

	////////////////////////////////////////////////////////////
	/// Mathematical operators v4 => value
	/// 
	////////////////////////////////////////////////////////////
	template<typename T> constexpr Value4<T> operator + (const Value4<T>& lhs, const T& value) { return { lhs.X + value, lhs.Y + value, lhs.Z + value, lhs.W + value }; }
	template<typename T> constexpr Value4<T> operator - (const Value4<T>& lhs, const T& value) { return { lhs.X - value, lhs.Y - value, lhs.Z - value, lhs.W - value }; }
	template<typename T> constexpr Value4<T> operator / (const Value4<T>& lhs, const T& value) { return { lhs.X / value, lhs.Y / value, lhs.Z / value, lhs.W / value }; }
	template<typename T> constexpr Value4<T> operator * (const Value4<T>& lhs, const T& value) { return { lhs.X * value, lhs.Y * value, lhs.Z * value, lhs.W * value }; }

	////////////////////////////////////////////////////////////
	/// Mathematical operators v4& => value
	/// 
	////////////////////////////////////////////////////////////
	template<typename T> constexpr Value4<T>& operator += (Value4<T>& lhs, const T& value) { return lhs = lhs + value; }
	template<typename T> constexpr Value4<T>& operator -= (Value4<T>& lhs, const T& value) { return lhs = lhs - value; }
	template<typename T> constexpr Value4<T>& operator /= (Value4<T>& lhs, const T& value) { return lhs = lhs / value; }
	template<typename T> constexpr Value4<T>& operator *= (Value4<T>& lhs, const T& value) { return lhs = lhs * value; }

	////////////////////////////////////////////////////////////
	/// Sign operators
	/// 
	////////////////////////////////////////////////////////////
	template<typename T> constexpr Value4<T> operator + (const Value4<T>& lhs) { return { +lhs.X, +lhs.Y, +lhs.Z, +lhs.W }; }
	template<typename T> constexpr Value4<T> operator - (const Value4<T>& lhs) { return { -lhs.X, -lhs.Y, -lhs.Z, -lhs.W }; }

	template<typename T> constexpr Value4<T> operator | (const Value2<T>& xy, const Value2<T>& zw) { return Value4<T>(xy, zw); }
	template<typename T> constexpr Value4<T> operator | (const Value3<T>& xyz, const T& w) { return Value4<T>(xyz, w); }
	template<typename T> constexpr Value4<T> operator | (const T& x, const Value3<T>& yzw) { return Value4<T>(x, yzw); }
	
	////////////////////////////////////////////////////////////
	/// Type definitions
	/// 
	////////////////////////////////////////////////////////////
	using Int4 = Value4<int32_t>;
	using UInt4 = Value4<uint32_t>;
	using Float4 = Value4<float>;
	using int4 = Value4<int32_t>;
	using uint4 = Value4<uint32_t>;
	using float4 = Value4<float>;
}