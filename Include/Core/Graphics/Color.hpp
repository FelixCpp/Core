// 
// Color.hpp
// Core
// 
// Created by Felix Busch on 27.03.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#pragma once

#include <Core/System/Types.hpp>

namespace Core
{
	////////////////////////////////////////////////////////////
	/// \brief Define color class that contains R, G, B and A
	///		   components in 8-bit unsigned integer format.
	/// 
	////////////////////////////////////////////////////////////
	class Color
	{
	public:

		////////////////////////////////////////////////////////////
		/// Pre-defined color constants
		/// 
		////////////////////////////////////////////////////////////
		static const Color White;
		static const Color Black;
		static const Color Clear;
		static const Color LightRed, DarkRed;
		static const Color LightOrange, DarkOrange;
		static const Color LightYellow, DarkYellow;
		static const Color LightGreen, DarkGreen;
		static const Color LightMint, DarkMint;
		static const Color LightTeal, DarkTeal;
		static const Color LightCyan, DarkCyan;
		static const Color LightBlue, DarkBlue;
		static const Color LightIndigo, DarkIndigo;
		static const Color LightPurple, DarkPurple;
		static const Color LightPink, DarkPink;
		static const Color LightBrown, DarkBrown;

		////////////////////////////////////////////////////////////
		/// \brief Constructs a white color.
		/// 
		////////////////////////////////////////////////////////////
		constexpr Color();

		////////////////////////////////////////////////////////////
		/// \brief Constructs an R-G-B-A color.
		///
		///	\param red		The red component
		///	\param green	The green component
		///	\param blue		The blue component
		///	\param alpha	The opacity/transparency component
		/// 
		////////////////////////////////////////////////////////////
		constexpr Color(u8 red, u8 green, u8 blue, u8 alpha = 255);

		////////////////////////////////////////////////////////////
		/// \brief Get the same exact color except with different
		///		   opacity.
		///
		///	\param opacity The new transparency where 0 is fully
		///				   transparent and 255 is not transparent at all.
		/// 
		////////////////////////////////////////////////////////////
		constexpr Color Opacity(u8 opacity) const;

		////////////////////////////////////////////////////////////
		/// Member data
		/// 
		////////////////////////////////////////////////////////////
		u8 R; ///< The red component
		u8 G; ///< The green component
		u8 B; ///< The blue component
		u8 A; ///< The transparent component (alpha)

	};
	
}

#include <Core/Graphics/Color.inl>