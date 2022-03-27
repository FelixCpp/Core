// 
// StrokeStyle.hpp
// Core
// 
// Created by Felix Busch on 27.03.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#pragma once

#include <Core/Graphics/StrokeStyleProperties.hpp>
#include <Core/System/Types.hpp>

#include <memory>
#include <vector>

////////////////////////////////////////////////////////////
/// Forward declaration
/// 
////////////////////////////////////////////////////////////
struct ID2D1StrokeStyle;

namespace Core
{
	////////////////////////////////////////////////////////////
	/// \brief Define stroke style wrapper that defines
	///		   how lines are rendered.
	/// 
	////////////////////////////////////////////////////////////
	class StrokeStyle
	{
	public:

		////////////////////////////////////////////////////////////
		/// \brief Default constructor.
		/// 
		////////////////////////////////////////////////////////////
		StrokeStyle();

		void SetStartCap(CapStyle style);
		void SetEndCap(CapStyle style);
		void SetDashCap(CapStyle style);
		void SetLineJoin(LineJoin join);
		void SetMiterLimit(float limit);
		void SetDashStyle(DashStyle style);
		void SetDashOffset(float offset);
		void SetDashes(const float* dashes, usize count);

		CapStyle GetStartCap() const;
		CapStyle GetEndCap() const;
		CapStyle GetDashCap() const;
		LineJoin GetLineJoin() const;
		float GetMiterLimit() const;
		DashStyle GetDashStyle() const;
		float GetDashOffset() const;
		const std::vector<float>& GetDashes() const;

		////////////////////////////////////////////////////////////
		/// \brief Get the underlying stroke style from the
		///		   implementation.
		/// 
		////////////////////////////////////////////////////////////
		ID2D1StrokeStyle* GetStyleStroke() const;

	private:

		////////////////////////////////////////////////////////////
		/// \brief Use PImpl here to avoid including Direct2D here.
		/// 
		////////////////////////////////////////////////////////////
		class Impl;

		////////////////////////////////////////////////////////////
		/// Member data
		/// 
		////////////////////////////////////////////////////////////
		std::shared_ptr<Impl>	impl;		///< Pointer to implementation
		mutable bool			updated;	///< State whether the style needs to update or not

		////////////////////////////////////////////////////////////
		/// Stroke Properties
		/// 
		////////////////////////////////////////////////////////////
		CapStyle				startCap;
		CapStyle				endCap;
		CapStyle				dashCap;
		LineJoin				lineJoin;
		float					miterLimit;
		DashStyle				dashStyle;
		float					dashOffset;
		std::vector<float>		dashes;

	};
}