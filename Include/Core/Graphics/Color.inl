// 
// Color.inl
// Core
// 
// Created by Felix Busch on 27.03.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#pragma once

namespace Core
{
	////////////////////////////////////////////////////////////
	constexpr Color::Color():
		R(255), G(255), B(255), A(255)
	{
	}

	////////////////////////////////////////////////////////////
	constexpr Color::Color(u8 red, u8 green, u8 blue, u8 alpha):
		R(red), G(green), B(blue), A(alpha)
	{}

	////////////////////////////////////////////////////////////
	/// Pre-defined color constants
	/// 
	////////////////////////////////////////////////////////////
	inline constexpr Color Color::White(255, 255, 255);
	inline constexpr Color Color::Black(0, 0, 0);
	inline constexpr Color Color::Clear(0, 0, 0, 0);

}