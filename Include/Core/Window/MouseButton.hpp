// 
// MouseButton.hpp
// Core
// 
// Created by Felix Busch on 16.02.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#pragma once

namespace Core
{
	////////////////////////////////////////////////////////////
	/// \brief Enumeration of mouse buttons
	/// 
	////////////////////////////////////////////////////////////
	enum class MouseButton
	{
		Left,		//!< The left mouse button
		Right,		//!< The right mouse button
		Middle,		//!< The middle mouse button (between left and right)
		XButton1,	//!< The first extra button
		XButton2,	//!< The second extra button
		Count		//!< The number of mouse buttons
	};
}