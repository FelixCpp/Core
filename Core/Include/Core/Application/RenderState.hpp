#pragma once

#include <Core/Application/DrawMode.hpp>
#include <Core/Application/StrokeStyle.hpp>
#include <Core/Application/TextRenderer.hpp>
#include <Core/Application/ShapeRenderer.hpp>
#include <Core/Application/SolidColorBrush.hpp>
#include <Core/Application/LinearGradientBrush.hpp>
#include <Core/Application/RadialGradientBrush.hpp>

#include <d2d1.h>
#include <stack>

namespace Core
{

	class GraphicsContext;

	struct RenderState {

		template<class TBrush>
		struct BrushPair {
			TBrush fill;
			TBrush stroke;
		};
		
		ShapeRenderer shapeRenderer;
		TextRenderer textRenderer;
		StrokeStyle strokeStyle;

		BrushPair<ID2D1Brush *> active;
		BrushPair<SolidColorBrush> solid;
		BrushPair<LinearGradientBrush> linear;
		BrushPair<RadialGradientBrush> radial;

		DrawMode rectMode;
		DrawMode ellipseMode;
		DrawMode imageMode;

		std::stack<D2D1::Matrix3x2F> metrics;

		float strokeWeight;

		explicit RenderState(GraphicsContext * gctx);

	};

}