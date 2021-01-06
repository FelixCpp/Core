#include <Core/Rendering/RenderState.hpp>
#include <Core/Rendering/GraphicsContext.hpp>

namespace Core
{

	RenderState::RenderState(GraphicsContext *& gctx) :
		shapeRenderer(gctx),
		textRenderer(gctx),
		strokeStyle(gctx),
		activeFill(nullptr),
		activeStroke(nullptr),
		solidFill(SolidColorBrush(gctx)),
		solidStroke(SolidColorBrush(gctx)),
		linearFill(LinearGradientBrush(gctx)),
		linearStroke(LinearGradientBrush(gctx)),
		radialFill(RadialGradientBrush(gctx)),
		radialStroke(RadialGradientBrush(gctx)),
		rectMode(DrawMode::Corner),
		ellipseMode(DrawMode::Center),
		imageMode(DrawMode::Corner),
		metrics(),
		strokeWeight(1.f),
		gctx(gctx)
	{ }

	void RenderState::setActiveMatrix(const D2D1::Matrix3x2F & matrix)
	{
		D2D1::Matrix3x2F & active = this->getActiveMatrix();
		active = matrix;
		this->activateMatrix();
	}

	D2D1::Matrix3x2F & RenderState::getActiveMatrix()
	{
		return this->metrics.empty() ? this->defaultMatrix : this->metrics.top();
	}

	void RenderState::pushMatrix()
	{
		/* we want to advance the matrix by the last active one. */
		this->metrics.push(this->getActiveMatrix());

		/* activate the new matrix */
		this->activateMatrix();
	}

	void RenderState::popMatrix()
	{
		/* we only want to pop a state from the stack if he's not empty */
		if (!this->metrics.empty())
		{
			/* pop the element from the stack */
			this->metrics.pop();
		}

		/* activate the new matrix */
		this->activateMatrix();
	}

	void RenderState::activateMatrix()
	{
		if (ID2D1HwndRenderTarget * rt = this->gctx->hwndRenderTarget.Get())
		{
			// activate the matrix as the new transformation
			rt->SetTransform(this->getActiveMatrix());
		}
	}

}