#pragma once

#include <memory>

#include <Core/Rendering/ShapeAttributes.hpp>

struct ID2D1Geometry;

namespace Core
{

	class GraphicsContext;

	class Shape {
	public:

		explicit Shape(GraphicsContext *& gctx);

		void SetShapeBegin(ShapeBegin begin);
		ShapeBegin GetShapeBegin() const;

		void SetShapeEnd(ShapeEnd end);
		ShapeEnd GetShapeEnd() const;

		void SetFillMode(FillMode fillMode);
		FillMode GetFillMode() const;

		void SetPathSegment(PathSegment segment);
		PathSegment GetPathSegment() const;

		bool Begin();
		bool End();

		void Vertex(float x, float y);
		void QuadraticBezier(float x1, float y1, float x2, float y2);
		void Bezier(float x1, float y1, float x2, float y2, float x3, float y3);
		void Arc(float x, float y, float width, float height, float rotationInDegrees, SweepDirection direction, ArcSize arcSize);

		ID2D1Geometry * GetGeometry() const;

	private:

		/// <summary>
		/// PImpl pattern so we don't need to
		/// include dirty headers (d2d1.h)
		/// </summary>
		struct Implementation;
		std::shared_ptr<Implementation> impl;

		GraphicsContext *& gctx;

		ShapeBegin shapeBegin;
		ShapeEnd shapeEnd;

		FillMode fillMode;
		PathSegment pathSegment;
		

	};

}