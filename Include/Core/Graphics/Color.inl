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
	constexpr Color Color::Opacity(u8 opacity) const
	{
		return { R, G, B, opacity };
	}

	////////////////////////////////////////////////////////////
	/// Pre-defined color constants
	/// 
	////////////////////////////////////////////////////////////
	inline constexpr Color Color::White(255, 255, 255);
	inline constexpr Color Color::Black(0, 0, 0);
	inline constexpr Color Color::Clear(0, 0, 0, 0);
	inline constexpr Color Color::LightRed(255, 59, 48), Color::DarkRed(255, 69, 58);
	inline constexpr Color Color::LightOrange(255, 149, 0), Color::DarkOrange(255, 159, 10);
	inline constexpr Color Color::LightYellow(255, 204, 0), Color::DarkYellow(255, 214, 10);
	inline constexpr Color Color::LightGreen(52, 199, 89), Color::DarkGreen(48, 209, 88);
	inline constexpr Color Color::LightMint(0, 199, 190), Color::DarkMint(102, 212, 207);
	inline constexpr Color Color::LightTeal(48, 176, 199), Color::DarkTeal(64, 200, 224);
	inline constexpr Color Color::LightCyan(50, 173, 230), Color::DarkCyan(100, 210, 255);
	inline constexpr Color Color::LightBlue(0, 122, 255), Color::DarkBlue(10, 231, 255);
	inline constexpr Color Color::LightIndigo(88, 86, 214), Color::DarkIndigo(94, 92, 230);
	inline constexpr Color Color::LightPurple(175, 82, 222), Color::DarkPurple(191, 90, 242);
	inline constexpr Color Color::LightPink(255, 45, 85), Color::DarkPink(255, 55, 95);
	inline constexpr Color Color::LightBrown(162, 132, 94), Color::DarkBrown(172, 142, 104);

}