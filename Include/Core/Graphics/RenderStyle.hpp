// 
// RenderStyle.hpp
// Core
// 
// Created by Felix Busch on 27.03.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#pragma once

#include <Core/Graphics/SolidColorBrush.hpp>
#include <Core/Graphics/DrawMode.hpp>
#include <Core/Graphics/StrokeStyle.hpp>
#include <Core/Graphics/Transformation.hpp>

#include <stack>

////////////////////////////////////////////////////////////
/// Forward declaration
/// 
////////////////////////////////////////////////////////////
struct ID2D1Brush;

namespace Core
{
	////////////////////////////////////////////////////////////
	/// \brief Define rendering style class.
	///
	///	The render style contains a bunch of information
	///	that are used to render content on screen.
	/// 
	////////////////////////////////////////////////////////////
	struct RenderStyle
	{
		ID2D1Brush* ActiveFill		= nullptr;	///< The currently used filling brush
		ID2D1Brush* ActiveStroke	= nullptr;	///< The currently used outlining brush

		SolidColorBrush SolidFill	= {};	///< The solid fill brush
		SolidColorBrush SolidStroke = {};	///< The solid outlining brush

		DrawMode RectMode		= Corner;	///< The draw mode to use for rectangles
		DrawMode EllipseMode	= Center;	///< The draw mode to use for ellipsis
		//DrawMode ImageMode;		///< The draw mode to use for image modes

		float StrokeWeight = 1.0f;	///< The outline thickness

		StrokeStyle					StrokeStyle = {};	///< The stroke style to use when rendering
		std::stack<Transformation>	Transform	= {};	///< The transformation to apply to the render target
	};
}