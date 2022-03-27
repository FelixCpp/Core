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