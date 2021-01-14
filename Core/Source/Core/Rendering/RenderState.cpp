#include <Core/Rendering/RenderState.hpp>
#include <Core/Rendering/GraphicsContext.hpp>

namespace Core
{

	RenderState::RenderState(GraphicsContext *& gctx) :
		shape(gctx),
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
		strokeWeight(1.f),
		defaultMatrix(D2D1::Matrix3x2F::Identity()),
		activeMatrix(&this->defaultMatrix),
		metrics(),
		gctx(gctx)
	{ }

	void RenderState::SetActiveMatrix(const D2D1::Matrix3x2F & matrix)
	{
		D2D1::Matrix3x2F & active = this->GetActiveMatrix();
		active = matrix;
		this->ActivateMatrix();
	}

	D2D1::Matrix3x2F & RenderState::GetActiveMatrix()
	{
		return *this->activeMatrix;
	}

	void RenderState::PushMatrix()
	{
		/* we want to advance the matrix by the last active one. */
		this->metrics.push(this->GetActiveMatrix());

		/* activate the new matrix */
		this->ActivateMatrix();
	}

	void RenderState::PopMatrix()
	{
		/* we only want to pop a state from the stack if he's not empty */
		if (!this->metrics.empty())
		{
			/* pop the element from the stack */
			this->metrics.pop();
		}

		/* activate the new matrix */
		this->ActivateMatrix();
	}

	void RenderState::Reset()
	{
		/* pop every element from the stack */
		while (!this->metrics.empty())
			this->metrics.pop();

		this->defaultMatrix = D2D1::Matrix3x2F::Identity(); // reset to its identity value
		this->ActivateMatrix();
	}

	void RenderState::ActivateMatrix()
	{
		this->activeMatrix = this->metrics.empty() ? &this->defaultMatrix : &this->metrics.top();

		if (ID2D1HwndRenderTarget * rt = this->gctx->renderTarget.Get())
		{
			// activate the matrix as the new transformation
			rt->SetTransform(this->GetActiveMatrix());
		}
	}

}