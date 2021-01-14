#include <Core/Rendering/Shape.hpp>
#include <Core/Rendering/GraphicsContext.hpp>

#include <Core/System/Logger.hpp>

#include <d2d1.h>
#include <wrl/client.h>

namespace Core
{

	struct Shape::Implementation {
		Microsoft::WRL::ComPtr<ID2D1PathGeometry> geometry = nullptr;
		Microsoft::WRL::ComPtr<ID2D1GeometrySink> sink = nullptr;
		bool isOpen = false;
	};

	Shape::Shape(GraphicsContext *& gctx) :
		impl(std::make_shared<Implementation>()),
		gctx(gctx),
		shapeBegin(ShapeBegin::Filled),
		shapeEnd(ShapeEnd::Open),
		fillMode(FillMode::Alternate),
		pathSegment(PathSegment::None)
	{
	}

	void Shape::SetShapeBegin(ShapeBegin begin)
	{
		this->shapeBegin = begin;
	}

	ShapeBegin Shape::GetShapeBegin() const
	{
		return this->shapeBegin;
	}

	void Shape::SetShapeEnd(ShapeEnd end)
	{
		this->shapeEnd = end;
	}

	ShapeEnd Shape::GetShapeEnd() const
	{
		return this->shapeEnd;
	}

	void Shape::SetFillMode(FillMode fillMode)
	{
		this->fillMode = fillMode;
	}

	FillMode Shape::GetFillMode() const
	{
		return this->fillMode;
	}

	void Shape::SetPathSegment(PathSegment segment)
	{
		this->pathSegment = segment;
	}

	PathSegment Shape::GetPathSegment() const
	{
		return this->pathSegment;
	}

	bool Shape::Begin()
	{
		if (!this->gctx)
		{
			CORE_ERROR("There is no GraphicsContext");
			return false;
		}

		/* get main factory */
		ID2D1Factory * factory = this->gctx->mainFactory.Get();
		if (!factory)
		{
			CORE_ERROR("There is no factory created");
			return false;
		}

		auto & geometry = this->impl->geometry;
		auto & sink = this->impl->sink;

		HRESULT hr = factory->CreatePathGeometry(&geometry);
		if (FAILED(hr))
		{
			CORE_ERROR("Failed to create a PathGeometry");
			return false;
		}

		hr = geometry->Open(&sink);
		if (FAILED(hr))
		{
			CORE_ERROR("Failed to open the GeometrySink");
			return false;
		}

		sink->SetFillMode(static_cast<D2D1_FILL_MODE>(this->fillMode));
		sink->SetSegmentFlags(static_cast<D2D1_PATH_SEGMENT>(this->pathSegment));
		
		return true;
	}

	bool Shape::End()
	{
		auto & geometry = this->impl->geometry;
		auto & sink = this->impl->sink;
		auto & isOpen = this->impl->isOpen;

		if (isOpen)
		{
			sink->EndFigure(static_cast<D2D1_FIGURE_END>(this->shapeEnd));
			isOpen = false;
		}

		HRESULT hr = sink->Close();
		if (FAILED(hr))
		{
			CORE_ERROR("Failed to close the GeometrySink");
			return false;
		}

		return true;
	}

	void Shape::Vertex(float x, float y)
	{
		auto & sink = this->impl->sink;
		auto & isOpen = this->impl->isOpen;

		const D2D1_POINT_2F point = D2D1::Point2F(x, y);

		if (!isOpen)
		{
			sink->BeginFigure(point, static_cast<D2D1_FIGURE_BEGIN>(this->shapeBegin));
			isOpen = true;
		} else
		{
			sink->AddLine(point);
		}
	}

	void Shape::QuadraticBezier(float x1, float y1, float x2, float y2)
	{
		auto & sink = this->impl->sink;
		auto & isOpen = this->impl->isOpen;

		if (!isOpen)
		{
			sink->BeginFigure(D2D1::Point2F(x1, y1), static_cast<D2D1_FIGURE_BEGIN>(this->shapeBegin));
			isOpen = false;
		}

		sink->AddQuadraticBezier(D2D1::QuadraticBezierSegment(
			D2D1::Point2F(x1, y1),
			D2D1::Point2F(x2, y2)
		));
	}

	void Shape::Bezier(float x1, float y1, float x2, float y2, float x3, float y3)
	{
		auto & sink = this->impl->sink;
		auto & isOpen = this->impl->isOpen;

		if (!isOpen)
		{
			sink->BeginFigure(D2D1::Point2F(x1, y1), static_cast<D2D1_FIGURE_BEGIN>(this->shapeBegin));
			isOpen = false;
		}
	
		sink->AddBezier(D2D1::BezierSegment(
			D2D1::Point2F(x1, y1),
			D2D1::Point2F(x2, y2),
			D2D1::Point2F(x3, y3)
		));
	}

	void Shape::Arc(float x, float y, float width, float height, float rotationInDegrees, SweepDirection direction, ArcSize arcSize)
	{
		auto & sink = this->impl->sink;
		auto & isOpen = this->impl->isOpen;

		if (!isOpen)
		{
			sink->BeginFigure(D2D1::Point2F(x, y), static_cast<D2D1_FIGURE_BEGIN>(this->shapeBegin));
			isOpen = false;
		}

		sink->AddArc(D2D1::ArcSegment(
			D2D1::Point2F(x, y),
			D2D1::SizeF(width, height),
			rotationInDegrees,
			static_cast<D2D1_SWEEP_DIRECTION>(direction),
			static_cast<D2D1_ARC_SIZE>(arcSize)
		));
	}

	ID2D1Geometry * Shape::GetGeometry() const
	{
		return this->impl->geometry.Get();
	}

}