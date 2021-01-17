#include <Core/Rendering/Targets/ShapeTarget.hpp>
#include <Core/Rendering/GraphicsContext.hpp>
#include <Core/Rendering/RenderStateManager.hpp>
#include <Core/Rendering/Shape.hpp>

namespace Core
{

	ShapeTarget::ShapeTarget(GraphicsContext *& gctx, RenderStateManager *& rsm) :
		gctx(gctx),
		rsm(rsm)
	{ }

	void ShapeTarget::BeginShape(ShapeBegin begin)
	{
		Shape & shape = this->GetShape();
		shape.SetShapeBegin(begin);
		shape.Begin();
	}

	void ShapeTarget::EndShape(ShapeEnd end)
	{
		Shape & shape = this->GetShape();
		shape.SetShapeEnd(end);
		shape.End();

		this->DrawShape();
	}

	void ShapeTarget::ShapeFillMode(FillMode fillMode)
	{
		this->GetShape().SetFillMode(fillMode);
	}

	void ShapeTarget::ShapePathSegment(PathSegment pathSegment)
	{
		this->GetShape().SetPathSegment(pathSegment);
	}

	void ShapeTarget::Vertex(float x, float y)
	{
		this->GetShape().Vertex(x, y);
	}

	void ShapeTarget::QuadraticBezier(float x1, float y1, float x2, float y2)
	{
		this->GetShape().QuadraticBezier(x1, y1, x2, y2);
	}

	void ShapeTarget::Bezier(float x1, float y1, float x2, float y2, float x3, float y3)
	{
		this->GetShape().Bezier(x1, y1, x2, y2, x3, y3);
	}

	void ShapeTarget::Arc(float x, float y, float width, float height, float rotationInDegrees, SweepDirection direction, ArcSize size)
	{
		this->GetShape().Arc(x, y, width, height, rotationInDegrees, direction, size);
	}

	Shape & ShapeTarget::GetShape()
	{
		return this->rsm->GetActiveState().shape;
	}

	void ShapeTarget::DrawShape()
	{
		// get the render target
		ID2D1HwndRenderTarget * renderTarget = this->gctx->renderTarget.Get();
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