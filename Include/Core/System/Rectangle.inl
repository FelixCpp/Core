// 
// Rectangle.inl
// Core
// 
// Created by Felix Busch on 06.03.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#pragma once

namespace Core
{

	template <typename T>
	constexpr Rectangle<T>::Rectangle():
		Left(T{}), Top(T{}), Width(T{}), Height(T{})
	{
	}

	template <typename T>
	constexpr Rectangle<T>::Rectangle(T left, T top, T width, T height):
		Left(left), Top(top), Width(width), Height(height)
	{
	}

	template <typename T>
	template <typename TOther>
	constexpr Rectangle<T>::Rectangle(const Rectangle<TOther>& other):
		Left((T)other.Left), Top((T)other.Top), Width((T)other.Width), Height((T)other.Height)
	{
	}

	template <typename T>
	constexpr Value2<T> Rectangle<T>::GetCenter() const
	{
		return { Left + Width / 2, Top + Height / 2 };
	}

	template <typename T>
	constexpr bool Rectangle<T>::Contains(const T& x, const T& y) const
	{
		return x >= Left && x < Left + Width && y >= Top && y < Top + Height;
	}

	template <typename T>
	constexpr bool Rectangle<T>::Contains(const Value2<T>& point) const
	{
		return Contains(point.X, point.Y);
	}


	template<typename T>
	bool operator == (const Rectangle<T>& lhs, const Rectangle<T>& rhs)
	{
		return lhs.Left == rhs.Left && lhs.Top == rhs.Top && lhs.Width == rhs.Width && lhs.Bottom == rhs.Bottom;
	}

	template<typename T>
	bool operator != (const Rectangle<T>& lhs, const Rectangle<T>& rhs)
	{
		return !(lhs == rhs);
	}


}