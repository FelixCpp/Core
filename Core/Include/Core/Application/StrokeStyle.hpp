#pragma once

#include <Core/Application/StrokeStyleAttributes.hpp>

#include <d2d1.h>
#include <wrl/client.h>

#include <vector>

namespace Core
{

	class GraphicsContext;

	class StrokeStyle {
	public:

		explicit StrokeStyle(GraphicsContext * gctx);

        void setStartCap(CapStyle style);
        CapStyle getStartCap() const;

        void setEndCap(CapStyle style);
        CapStyle getEndCap() const;

        void setDashCap(CapStyle style);
        CapStyle getDashCap() const;

        void setLineJoin(LineJoin lineJoin);
        LineJoin getLineJoin() const;

        void setDashStyle(DashStyle style);
        DashStyle getDashStyle() const;

        void setDashOffset(float offset);
        float getDashOffset() const;

        void setMiterLimit(float miterLimit);
        float getMiterLimit() const;

        void setDashes(const std::vector<float> & dashes);
        const std::vector<float> & getDashes() const;

		ID2D1StrokeStyle * getStrokeStyle() const;

	private:

		CapStyle startCap;
		CapStyle endCap;
		CapStyle dashCap;
		LineJoin lineJoin;
		DashStyle dashStyle;
		float dashOffset;
		float miterLimit;
		std::vector<float> dashes;

		mutable Microsoft::WRL::ComPtr<ID2D1StrokeStyle> style;
		mutable bool updated;

		GraphicsContext * gctx;

	};

}