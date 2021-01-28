#include <Core/Rendering/Targets/PrimitiveRenderTarget.hpp>
#include <Core/Rendering/RenderStateManager.hpp>
#include <Core/Rendering/Renderers/Renderer.hpp>
#include <Core/Rendering/RenderState.hpp>

namespace Core
{

	PrimitiveRenderTarget::PrimitiveRenderTarget(LateRef<Renderer> renderer, LateRef<RenderStateManager> rsm) :
		renderer(renderer),
		rsm(rsm)
	{ }

	void PrimitiveRenderTarget::RectMode(DrawMode mode)
	{
		this->GetState().rectMode = mode;
	}

	void PrimitiveRenderTarget::Rect(float x1, float y1, float x2, float y2, float cornerRadiusX, float cornerRadiusY)
	{
		ID2D1RenderTarget * rt = this->renderer->GetRenderTarget();
		RenderState & state = this->GetState();

		D2D1_RECT_F rect = D2D1::RectF();

		switch (state.rectMode)
		{
			case DrawMode::Corner:  rect = D2D1::RectF(x1, y1, x1 + x2, y1 + y2); break;
			case DrawMode::Corners: rect = D2D1::RectF(x1, y1, x2, y2); break;
			case DrawMode::Center:  rect = D2D1::RectF(x1 - x2 / 2.f, y1 - y2 / 2.f, x1 + x2 / 2.f, y1 + y2 / 2.f); break;
			default: break;
		}

		const D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(rect, cornerRadiusX, cornerRadiusY);

		if (ID2D1Brush * fill = state.activeFill)
		{
			rt->FillRoundedRectangle(roundedRect, fill);
		}

		if (ID2D1Brush * stroke = state.activeStroke)
		{
			rt->DrawRoundedRectangle(roundedRect, stroke, state.strokeWeight, state.strokeStyle.GetStrokeStyle());
		}
	}

	void PrimitiveRenderTarget::Quad(float x, float y, float size, float cornerRadiusX, float cornerRadiusY)
	{
		this->Rect(x, y, size, size, cornerRadiusX, cornerRadiusY);
	}

	void PrimitiveRenderTarget::EllipseMode(DrawMode mode)
	{
		this->GetState().ellipseMode = mode;
	}

	void PrimitiveRenderTarget::Ellipse(float x, float y, float rx, float ry)
	{
		ID2D1RenderTarget * rt = this->renderer->GetRenderTarget();
		RenderState & state = this->GetState();

		D2D1_ELLIPSE ellipse;

		switch (state.ellipseMode)
		{
			case DrawMode::Corner:  ellipse = D2D1::Ellipse(D2D1::Point2F(x + rx, y + ry), rx, ry); break;
			case DrawMode::Corners:
			{
				float w = (rx - x) / 2.f;
				float h = (ry - y) / 2.f;
				float xpos = x + w;
				float ypos = y + h;
				ellipse = D2D1::Ellipse(D2D1::Point2F(xpos, ypos), w, h);
			} break;
			case DrawMode::Center:  ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), rx, ry); break;
			default: break;
		}

		if (ID2D1Brush * fill = state.activeFill)
		{
			rt->FillEllipse(ellipse, fill);
		}

		if (ID2D1Brush * stroke = state.activeStroke)
		{
			rt->DrawEllipse(ellipse, stroke, state.strokeWeight, state.strokeStyle.GetStrokeStyle());
		}
	}

	void PrimitiveRenderTarget::Circle(float x, float y, float radius)
	{
		this->Ellipse(x, y, radius, radius);
	}

	void PrimitiveRenderTarget::Point(float x, float y)
	{
		ID2D1RenderTarget * rt = this->renderer->GetRenderTarget();
		RenderState & state = this->GetState();
		const D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), state.strokeWeight, state.strokeWeight);

		if (ID2D1Brush * stroke = state.activeStroke)
		{
			rt->FillEllipse(ellipse, stroke);
		}
	}

	void PrimitiveRenderTarget::Line(float x1, float y1, float x2, float y2)
	{
		ID2D1RenderTarget * rt = this->renderer->GetRenderTarget();
		RenderState & state = this->GetState();

		if (ID2D1Brush * stroke = state.activeStroke)
		{
			rt->DrawLine(D2D1::Point2F(x1, y1), D2D1::Point2F(x2, y2), stroke, state.strokeWeight, state.strokeStyle.GetStrokeStyle());
		}
	}

	RenderState & PrimitiveRenderTarget::GetState()
	{
		return this->rsm->GetActiveState();
	}

}