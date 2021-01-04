#pragma once

#include <Core/Rendering/DrawMode.hpp>
#include <Core/Rendering/StrokeStyle.hpp>
#include <Core/Rendering/TextRenderer.hpp>
#include <Core/Rendering/ShapeRenderer.hpp>
#include <Core/Rendering/SolidColorBrush.hpp>
#include <Core/Rendering/LinearGradientBrush.hpp>
#include <Core/Rendering/RadialGradientBrush.hpp>

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