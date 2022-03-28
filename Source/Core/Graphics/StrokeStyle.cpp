// 
// StrokeStyle.cpp
// Core
// 
// Created by Felix Busch on 27.03.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#include <Core/Graphics/StrokeStyle.hpp>
#include <Core/Application/Factories.hpp>
#include <Core/System/Error.hpp>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <d2d1.h>
#include <wrl/client.h>

namespace Core
{
	////////////////////////////////////////////////////////////
	/// \brief Define implementation class for the stroke style.
	/// 
	////////////////////////////////////////////////////////////
	class StrokeStyle::Impl
	{
	public:
		
		////////////////////////////////////////////////////////////
		/// Member data
		/// 
		////////////////////////////////////////////////////////////
		Microsoft::WRL::ComPtr<ID2D1StrokeStyle> Style;	///< The wrapped style attribute

	};

	////////////////////////////////////////////////////////////
	StrokeStyle::StrokeStyle():
		impl(std::make_shared<Impl>()),
		updated(false),
		startCap(CapStyle::Round),
		endCap(CapStyle::Round),
		dashCap(CapStyle::Round),
		lineJoin(LineJoin::Miter),
		miterLimit(0.0f),
		dashStyle(DashStyle::Solid),
		dashOffset(0.0f)
	{
	}

	////////////////////////////////////////////////////////////
	void StrokeStyle::SetStartCap(CapStyle style)
	{ if (startCap != style) { startCap = style; updated = false; } }

	////////////////////////////////////////////////////////////
	void StrokeStyle::SetEndCap(CapStyle style)
	{ if (endCap != style) { endCap = style; updated = false; } }

	////////////////////////////////////////////////////////////
	void StrokeStyle::SetDashCap(CapStyle style)
	{ if (dashCap != style) { dashCap = style; updated = false; } }

	////////////////////////////////////////////////////////////
	void StrokeStyle::SetLineJoin(LineJoin join)
	{ if (lineJoin != join) { lineJoin = join; updated = false; } }

	////////////////////////////////////////////////////////////
	void StrokeStyle::SetMiterLimit(float limit)
	{ if (miterLimit != limit) { miterLimit = limit; updated = false; } }

	////////////////////////////////////////////////////////////
	void StrokeStyle::SetDashStyle(DashStyle style)
	{ if (dashStyle != style) { dashStyle = style; updated = false; } }

	////////////////////////////////////////////////////////////
	void StrokeStyle::SetDashOffset(float offset)
	{ if (dashOffset != offset) { dashOffset = offset; updated = false; } }

	////////////////////////////////////////////////////////////
	void StrokeStyle::SetDashes(const float* dashes, usize count)
	{ this->dashes.assign(dashes, dashes + count); dashStyle = DashStyle::Custom; updated = false; }

	////////////////////////////////////////////////////////////
	CapStyle StrokeStyle::GetStartCap() const
	{ return startCap; }

	////////////////////////////////////////////////////////////
	CapStyle StrokeStyle::GetEndCap() const
	{ return endCap; }

	////////////////////////////////////////////////////////////
	CapStyle StrokeStyle::GetDashCap() const
	{ return dashCap; }

	////////////////////////////////////////////////////////////
	LineJoin StrokeStyle::GetLineJoin() const
	{ return lineJoin; }

	////////////////////////////////////////////////////////////
	float StrokeStyle::GetMiterLimit() const
	{ return miterLimit; }

	////////////////////////////////////////////////////////////
	DashStyle StrokeStyle::GetDashStyle() const
	{ return dashStyle; }

	////////////////////////////////////////////////////////////
	float StrokeStyle::GetDashOffset() const
	{ return dashOffset; }

	////////////////////////////////////////////////////////////
	const std::vector<float>& StrokeStyle::GetDashes() const
	{ return dashes; }

	////////////////////////////////////////////////////////////
	ID2D1StrokeStyle* StrokeStyle::GetStyleStroke() const
	{
		if(!updated)
		{
			if(ID2D1Factory* d2dFactory = Factories::D2DFactory.Get())
			{
				const D2D1_STROKE_STYLE_PROPERTIES properties
				{
					.startCap = (D2D1_CAP_STYLE)startCap,
					.endCap = (D2D1_CAP_STYLE)endCap,
					.dashCap = (D2D1_CAP_STYLE)dashCap,
					.lineJoin = (D2D1_LINE_JOIN)lineJoin,
					.miterLimit = miterLimit,
					.dashStyle = (D2D1_DASH_STYLE)dashStyle,
					.dashOffset = dashOffset
				};

				const HRESULT success = d2dFactory->CreateStrokeStyle(properties, dashes.data(), (UINT32)dashes.size(), &impl->Style);

				if(FAILED(success))
				{
					Err() << "Failed to update the stroke style." << std::endl;
					return nullptr;
				}

				updated = true;
			}
		}

		return impl->Style.Get();
	}
}