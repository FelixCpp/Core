#pragma once

#include <Core/Rendering/ShapeAttributes.hpp>

#include <d2d1.h>
#include <wrl/client.h>

namespace Core
{

	class GraphicsContext;

	class ShapeRenderer {
	public:

		explicit ShapeRenderer(GraphicsContext *& gctx);

		void setFillMode(FillMode fillMode);
		FillMode getFillMode() const;

		void setPathSegment(PathSegment pathSegment);
		PathSegment getPathSegment() const;

		void beginShape();
		void endShape(ShapeEndType end);

		void vertex(float x, float y);
		void quadraticBezier(float x1, float y1, float x2, float y2);
		void bezier(float x1, float y1, float x2, float y2, float x3, float y3);
		void arc(float x, float y, float width, float height, float rotationInDegrees, SweepDirection sweepDirection, ArcSize size);

		ID2D1Geometry * getGeometry() const;

	private:

		Microsoft::WRL::ComPtr<ID2D1PathGeometry> path;
		Microsoft::WRL::ComPtr<ID2D1GeometrySink> sink;

		FillMode fillMode;
		PathSegment pathSegment;

		bool shapeBegan;

		GraphicsContext *& gctx;

	};

}