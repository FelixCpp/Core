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

	void StrokeStyle::setStartCap(CapStyle style)
	{
		if (this->startCap != style)
		{
			this->startCap = style;
			this->updated = false;
		}
	}

	CapStyle StrokeStyle::getStartCap() const
	{
		return this->startCap;
	}

	void StrokeStyle::setEndCap(CapStyle style)
	{
		if (this->endCap != style)
		{
			this->endCap = style;
			this->updated = false;
		}
	}

	CapStyle StrokeStyle::getEndCap() const
	{
		return this->endCap;
	}

	void StrokeStyle::setDashCap(CapStyle style)
	{
		if (this->dashCap != style)
		{
			this->dashCap = style;
			this->updated = false;
		}
	}

	CapStyle StrokeStyle::getDashCap() const
	{
		return this->dashCap;
	}

	void StrokeStyle::setLineJoin(LineJoin lineJoin)
	{
		if (this->lineJoin != lineJoin)
		{
			this->lineJoin = lineJoin;
			this->updated = false;
		}
	}

	LineJoin StrokeStyle::getLineJoin() const
	{
		return this->lineJoin;
	}

	void StrokeStyle::setDashStyle(DashStyle style)
	{
		if (this->dashStyle != style)
		{
			this->dashStyle = style;
			this->updated = false;
		}
	}

	DashStyle StrokeStyle::getDashStyle() const
	{
		return this->dashStyle;
	}

	void StrokeStyle::setDashOffset(float offset)
	{
		if (this->dashOffset != offset)
		{
			this->dashOffset = offset;
			this->updated = false;
		}
	}

	float StrokeStyle::getDashOffset() const
	{
		return this->dashOffset;
	}

	void StrokeStyle::setMiterLimit(float miterLimit)
	{
		if (this->miterLimit != miterLimit)
		{
			this->miterLimit = miterLimit;
			this->updated = false;
		}
	}

	float StrokeStyle::getMiterLimit() const
	{
		return this->miterLimit;
	}

	void StrokeStyle::setDashes(const std::vector<float> & dashes)
	{
		if (this->dashes != dashes)
		{
			this->dashes = dashes;
			this->setDashStyle(DashStyle::Custom);
			this->updated = false;
		}
	}

	const std::vector<float> & StrokeStyle::getDashes() const
	{
		return this->dashes;
	}

	ID2D1StrokeStyle * StrokeStyle::getStrokeStyle() const
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