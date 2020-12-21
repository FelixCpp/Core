#include <Core/Application/RenderState.hpp>

namespace Core
{

	RenderState::RenderState(GraphicsContext * gctx) :
		shapeRenderer(gctx),
		textRenderer(gctx),
		strokeStyle(gctx),
		active(),
		solid({ SolidColorBrush(gctx), SolidColorBrush(gctx) }),
		linear({ LinearGradientBrush(gctx), LinearGradientBrush(gctx) }),
		radial({ RadialGradientBrush(gctx), RadialGradientBrush(gctx) }),
		rectMode(DrawMode::Corner),
		ellipseMode(DrawMode::Center),
		imageMode(DrawMode::Corner),
		metrics(),
		strokeWeight(1.f)
	{ }

}