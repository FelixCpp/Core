#include <Core/Rendering/StrokeStyle.hpp>
#include <Core/Rendering/GraphicsContext.hpp>

#include <Core/System/Logger.hpp>

namespace Core
{

	StrokeStyle::StrokeStyle(GraphicsContext *& gctx) :
		startCap(CapStyle::Round),
		endCap(CapStyle::Round),
		dashCap(CapStyle::Round),
		lineJoin(LineJoin::Round),
		dashStyle(DashStyle::Solid),
		dashOffset(0.f),
		miterLimit(10.f),
		dashes(),
		style(nullptr),
		updated(false),
		gctx(gctx)
	{
	}

	void StrokeStyle::SetStartCap(CapStyle style)
	{
		if (this->startCap != style)
		{
			this->startCap = style;
			this->updated = false;
		}
	}

	CapStyle StrokeStyle::GetStartCap() const
	{
		return this->startCap;
	}

	void StrokeStyle::SetEndCap(CapStyle style)
	{
		if (this->endCap != style)
		{
			this->endCap = style;
			this->updated = false;
		}
	}

	CapStyle StrokeStyle::GetEndCap() const
	{
		return this->endCap;
	}

	void StrokeStyle::SetDashCap(CapStyle style)
	{
		if (this->dashCap != style)
		{
			this->dashCap = style;
			this->updated = false;
		}
	}

	CapStyle StrokeStyle::GetDashCap() const
	{
		return this->dashCap;
	}

	void StrokeStyle::SetLineJoin(LineJoin lineJoin)
	{
		if (this->lineJoin != lineJoin)
		{
			this->lineJoin = lineJoin;
			this->updated = false;
		}
	}

	LineJoin StrokeStyle::GetLineJoin() const
	{
		return this->lineJoin;
	}

	void StrokeStyle::SetDashStyle(DashStyle style)
	{
		if (this->dashStyle != style)
		{
			this->dashStyle = style;
			this->updated = false;
		}
	}

	DashStyle StrokeStyle::GetDashStyle() const
	{
		return this->dashStyle;
	}

	void StrokeStyle::SetDashOffset(float offset)
	{
		if (this->dashOffset != offset)
		{
			this->dashOffset = offset;
			this->updated = false;
		}
	}

	float StrokeStyle::GetDashOffset() const
	{
		return this->dashOffset;
	}

	void StrokeStyle::SetMiterLimit(float miterLimit)
	{
		if (this->miterLimit != miterLimit)
		{
			this->miterLimit = miterLimit;
			this->updated = false;
		}
	}

	float StrokeStyle::GetMiterLimit() const
	{
		return this->miterLimit;
	}

	void StrokeStyle::SetDashes(const std::vector<float> & dashes)
	{
		if (this->dashes != dashes)
		{
			this->dashes = dashes;
			this->SetDashStyle(DashStyle::Custom);
			this->updated = false;
		}
	}

	const std::vector<float> & StrokeStyle::GetDashes() const
	{
		return this->dashes;
	}

	ID2D1StrokeStyle * StrokeStyle::GetStrokeStyle() const
	{
		if (!this->updated)
		{
			const D2D1_STROKE_STYLE_PROPERTIES properties = D2D1::StrokeStyleProperties(
				static_cast<D2D1_CAP_STYLE>(this->startCap),
				static_cast<D2D1_CAP_STYLE>(this->endCap),
				static_cast<D2D1_CAP_STYLE>(this->dashCap),
				static_cast<D2D1_LINE_JOIN>(this->lineJoin),
				this->miterLimit,
				static_cast<D2D1_DASH_STYLE>(this->dashStyle),
				this->dashOffset
			);

			ID2D1Factory * factory = this->gctx->mainFactory.Get();
			HRESULT hr = factory->CreateStrokeStyle(
				properties,
				this->dashes.data(),
				this->dashes.size(),
				&this->style
			);
			if (FAILED(hr))
			{
				CORE_ERROR("CreateStrokeStyle");
				return nullptr;
			}

			this->updated = true;
		}

		return this->style.Get();
	}

}