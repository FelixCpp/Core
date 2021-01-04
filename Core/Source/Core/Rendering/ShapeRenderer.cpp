#include <Core/Rendering/ShapeRenderer.hpp>
#include <Core/Rendering/GraphicsContext.hpp>

namespace Core
{

	ShapeRenderer::ShapeRenderer(GraphicsContext * gctx) :
		path(nullptr),
		sink(nullptr),
		fillMode(FillMode::Alternate),
		pathSegment(PathSegment::None),
		shapeBegan(false),
		gctx(gctx)
	{
	}

	void ShapeRenderer::setFillMode(FillMode fillMode)
	{
		if (this->fillMode != fillMode)
		{
			this->fillMode = fillMode;

			if (this->sink)
			{
				this->sink->SetFillMode(static_cast<D2D1_FILL_MODE>(fillMode));
			}
		}
	}

	FillMode ShapeRenderer::getFillMode() const
	{
		return this->fillMode;
	}

	void ShapeRenderer::setPathSegment(PathSegment pathSegment)
	{
		if (this->pathSegment != pathSegment)
		{
			this->pathSegment = pathSegment;

			if (this->sink)
			{
				this->sink->SetSegmentFlags(static_cast<D2D1_PATH_SEGMENT>(pathSegment));
			}
		}
	}

	PathSegment ShapeRenderer::getPathSegment() const
	{
		return this->pathSegment;
	}

	void ShapeRenderer::beginShape()
	{
		ID2D1Factory * factory = this->gctx->mainFactory.Get();
		factory->CreatePathGeometry(&this->path);
		this->path->Open(&this->sink);

		this->sink->SetFillMode(static_cast<D2D1_FILL_MODE>(this->fillMode));
		this->sink->SetSegmentFlags(static_cast<D2D1_PATH_SEGMENT>(this->pathSegment));
	}

	void ShapeRenderer::endShape(ShapeEndType end)
	{
		this->sink->EndFigure(static_cast<D2D1_FIGURE_END>(end));
		this->sink->Close();

		this->shapeBegan = false;
	}

	void ShapeRenderer::vertex(float x, float y)
	{
		if (!this->shapeBegan)
		{
			this->sink->BeginFigure(D2D1::Point2F(x, y), D2D1_FIGURE_BEGIN_FILLED);
			this->shapeBegan = true;
		} else
		{
			this->sink->AddLine(D2D1::Point2F(x, y));
		}
	}

	void ShapeRenderer::quadraticBezier(float x1, float y1, float x2, float y2)
	{
		if (!this->shapeBegan)
		{
			this->sink->BeginFigure(D2D1::Point2F(x1, y1), D2D1_FIGURE_BEGIN_FILLED);
			this->shapeBegan = true;
		}

		this->sink->AddQuadraticBezier(D2D1::QuadraticBezierSegment(D2D1::Point2F(x1, y1), D2D1::Point2F(x2, y2)));
	}

	void ShapeRenderer::bezier(float x1, float y1, float x2, float y2, float x3, float y3)
	{
		if (!this->shapeBegan)
		{
			this->sink->BeginFigure(D2D1::Point2F(x1, y1), D2D1_FIGURE_BEGIN_FILLED);
			this->shapeBegan = true;
		}

		this->sink->AddBezier(D2D1::BezierSegment(D2D1::Point2F(x1, y1), D2D1::Point2F(x2, y2), D2D1::Point2F(x3, y3)));
	}

	void ShapeRenderer::arc(float x, float y, float width, float height, float rotationInDegrees, SweepDirection sweepDirection, ArcSize size)
	{
		if (!this->shapeBegan)
		{
			this->sink->BeginFigure(D2D1::Point2F(x, y), D2D1_FIGURE_BEGIN_FILLED);
			this->shapeBegan = true;
		}

		this->sink->AddArc(D2D1::ArcSegment(
			D2D1::Point2F(x, y), D2D1::SizeF(width, height), rotationInDegrees, static_cast<D2D1_SWEEP_DIRECTION>(sweepDirection), static_cast<D2D1_ARC_SIZE>(size)
		));
	}

	ID2D1Geometry * ShapeRenderer::getGeometry() const
	{
		return this->path.Get();
	}

}