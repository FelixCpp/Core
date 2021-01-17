#include <Core/Rendering/StrokeStyleTarget.hpp>
#include <Core/Rendering/RenderStateManager.hpp>
#include <Core/Rendering/StrokeStyle.hpp>

namespace Core
{

	StrokeStyleTarget::StrokeStyleTarget(RenderStateManager *& rsm) :
		rsm(rsm)
	{
	}

	void StrokeStyleTarget::StrokeStartCap(CapStyle style)
	{
		this->GetStyle().SetStartCap(style);
	}

	void StrokeStyleTarget::StrokeEndCap(CapStyle style)
	{
		this->GetStyle().SetEndCap(style);
	}

	void StrokeStyleTarget::StrokeDashCap(CapStyle style)
	{
		this->GetStyle().SetDashCap(style);
	}

	void StrokeStyleTarget::StrokeLineJoin(LineJoin lineJoin)
	{
		this->GetStyle().SetLineJoin(lineJoin);
	}

	void StrokeStyleTarget::StrokeDashStyle(DashStyle style)
	{
		this->GetStyle().SetDashStyle(style);
	}

	void StrokeStyleTarget::StrokeDashes(const std::vector<float> & dashes)
	{
		this->GetStyle().SetDashes(dashes);
		this->GetStyle().SetDashStyle(DashStyle::Custom);
	}

	void StrokeStyleTarget::StrokeDashOffset(float offset)
	{
		this->GetStyle().SetDashOffset(offset);
	}

	void StrokeStyleTarget::StrokeMiterLimit(float limit)
	{
		this->GetStyle().SetMiterLimit(limit);
	}

	StrokeStyle & StrokeStyleTarget::GetStyle()
	{
		return this->rsm->GetActiveState().strokeStyle;
	}

}