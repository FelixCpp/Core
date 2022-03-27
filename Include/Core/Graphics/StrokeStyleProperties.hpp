﻿// 
// StrokeStyleProperties.hpp
// Core
// 
// Created by Felix Busch on 27.03.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#pragma once

namespace Core
{
	////////////////////////////////////////////////////////////
	/// \brief Enum which describes the drawing of the ends of
	///        a line.
	/// 
	////////////////////////////////////////////////////////////
	enum class CapStyle
    {
	    ////////////////////////////////////////////////////////////
	    /// Flat line cap.
	    /// 
	    ////////////////////////////////////////////////////////////
	    Flat,

	    ////////////////////////////////////////////////////////////
	    /// Square line cap.
	    /// 
	    ////////////////////////////////////////////////////////////
	    Square,

	    ////////////////////////////////////////////////////////////
	    /// Round line cap.
	    /// 
	    ////////////////////////////////////////////////////////////
	    Round,

	    ////////////////////////////////////////////////////////////
	    /// Triangular line cap.
	    /// 
	    ////////////////////////////////////////////////////////////
	    Triangle,
    };

	////////////////////////////////////////////////////////////
	/// \brief Describes the sequence of dashes and gaps in a
	///        stroke.
	/// 
	////////////////////////////////////////////////////////////
	enum class DashStyle
    {
        Solid,
        Dash,
        Dot,
        DashDot,
        DashDotDot,
        Custom,
    };

	////////////////////////////////////////////////////////////
	/// \brief Enum which describes the drawing of the corners
	///		   on the line.
	/// 
	////////////////////////////////////////////////////////////
	enum class LineJoin
    {
	    ////////////////////////////////////////////////////////////
	    /// Miter join.
	    /// 
	    ////////////////////////////////////////////////////////////
	    Miter,

	    ////////////////////////////////////////////////////////////
	    /// Bevel join.
	    /// 
	    ////////////////////////////////////////////////////////////
	    Bevel,

	    ////////////////////////////////////////////////////////////
	    /// Round join.
	    /// 
	    ////////////////////////////////////////////////////////////
	    Round,

	    ////////////////////////////////////////////////////////////
	    /// Miter/Bevel join.
	    /// 
	    ////////////////////////////////////////////////////////////
	    MiterOrBevel,
    };
}