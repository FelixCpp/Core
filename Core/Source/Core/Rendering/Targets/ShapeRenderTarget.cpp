#include <Core/Rendering/Targets/ShapeRenderTarget.hpp>
#include <Core/Rendering/Renderers/Renderer.hpp>
#include <Core/Rendering/RenderStateManager.hpp>
#include <Core/Rendering/Shape.hpp>

namespace Core
{

	ShapeRenderTarget::ShapeRenderTarget(LateRef<Renderer> renderer, LateRef<RenderStateManager> rsm) :
		renderer(renderer),
		rsm(rsm)
	{ }

	void ShapeRenderTarget::BeginShape(ShapeBegin begin)
	{
		Shape & shape = this->GetShape();
		shape.SetShapeBegin(begin);
		shape.Begin();
	}

	void ShapeRenderTarget::EndShape(ShapeEnd end)
	{
		Shape & shape = this->GetShape();
		shape.SetShapeEnd(end);
		shape.End();

		this->DrawShape();
	}

	void ShapeRenderTarget::ShapeFillMode(FillMode fillMode)
	{
		this->GetShape().SetFillMode(fillMode);
	}

	void ShapeRenderTarget::ShapePathSegment(PathSegment pathSegment)
	{
		this->GetShape().SetPathSegment(pathSegment);
	}

	void ShapeRenderTarget::Vertex(float x, float y)
	{
		this->GetShape().Vertex(x, y);
	}

	void ShapeRenderTarget::QuadraticBezier(float x1, float y1, float x2, float y2)
	{
		this->GetShape().QuadraticBezier(x1, y1, x2, y2);
	}

	void ShapeRenderTarget::Bezier(float x1, float y1, float x2, float y2, float x3, float y3)
	{
		this->GetShape().Bezier(x1, y1, x2, y2, x3, y3);
	}

	void ShapeRenderTarget::Arc(float x, float y, float width, float height, float rotationInDegrees, SweepDirection direction, ArcSize size)
	{
		this->GetShape().Arc(x, y, width, height, rotationInDegrees, direction, size);
	}

	Shape & ShapeRenderTarget::GetShape()
	{
		return this->rsm->GetActiveState().shape;
	}

	void ShapeRenderTarget::DrawShape()
	{
		// get the render target
		ID2D1RenderTarget * renderTarget = this->renderer->GetRenderTarget();
		if (!renderTarget) { return; }

		// get the geometry
		ID2D1Geometry * geometry = this->GetShape().GetGeometry();
		if (!geometry) { return; }

		RenderState & state = this->rsm->GetActiveState();

		// draw the filled version
		if (ID2D1Brush * brush = state.activeFill)
		{
			renderTarget->FillGeometry(geometry, brush);
		}

		// draw the outlined version
		if (ID2D1Brush * brush = state.activeStroke)
		{
			ID2D1StrokeStyle * strokeStyle = state.strokeStyle.GetStrokeStyle();
			renderTarget->DrawGeometry(geometry, brush, state.strokeWeight, strokeStyle);
		}
	}

}