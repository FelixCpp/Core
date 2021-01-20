#pragma once

#include <Core/Rendering/StrokeStyleAttributes.hpp>

#include <d2d1.h>
#include <wrl/client.h>

#include <vector>

namespace Core
{

	class StrokeStyle {
	public:

		StrokeStyle();

        void SetStartCap(CapStyle style);
        CapStyle GetStartCap() const;

        void SetEndCap(CapStyle style);
        CapStyle GetEndCap() const;

        void SetDashCap(CapStyle style);
        CapStyle GetDashCap() const;

        void SetLineJoin(LineJoin lineJoin);
        LineJoin GetLineJoin() const;

        void SetDashStyle(DashStyle style);
        DashStyle GetDashStyle() const;

        void SetDashOffset(float offset);
        float GetDashOffset() const;

        void SetMiterLimit(float miterLimit);
        float GetMiterLimit() const;

        void SetDashes(const std::vector<float> & dashes);
        const std::vector<float> & GetDashes() const;

		ID2D1StrokeStyle * GetStrokeStyle() const;

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

	};

}