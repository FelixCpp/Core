﻿// 
// SolidColorBrush.hpp
// Core
// 
// Created by Felix Busch on 27.03.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#pragma once

#include <Core/Graphics/Color.hpp>

#include <memory>

////////////////////////////////////////////////////////////
/// Forward declaration
/// 
////////////////////////////////////////////////////////////
struct ID2D1Brush;

namespace Core
{
	////////////////////////////////////////////////////////////
	/// \brief Define solid color brush wrapper.
	/// 
	////////////////////////////////////////////////////////////
	class SolidColorBrush
	{
	public:

		////////////////////////////////////////////////////////////
		/// \brief Default constructor
		/// 
		////////////////////////////////////////////////////////////
		SolidColorBrush();

		////////////////////////////////////////////////////////////
		/// \brief Set the color of the brush.
		///
		///	\param color The new color.
		/// 
		////////////////////////////////////////////////////////////
		void SetColor(const Color& color);

		////////////////////////////////////////////////////////////
		/// \brief Get the current color
		/// 
		////////////////////////////////////////////////////////////
		const Color& GetColor() const;

		////////////////////////////////////////////////////////////
		/// \brief Get the underlying brush
		/// 
		////////////////////////////////////////////////////////////
		ID2D1Brush* GetBrush() const;

	private:

		////////////////////////////////////////////////////////////
		/// \brief Define extern implementation class to prevent
		///		   including Direct2D header files.
		/// 
		////////////////////////////////////////////////////////////
		class Impl;

		////////////////////////////////////////////////////////////
		/// Member data
		/// 
		////////////////////////////////////////////////////////////
		std::shared_ptr<Impl>	impl;	///< pointer to implementation
		Color					color;	///< The current color

	};

}