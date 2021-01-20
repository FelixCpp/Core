#include <Core/Rendering/RenderState.hpp>
#include <Core/Rendering/Renderers/Renderer.hpp>

namespace Core
{

	RenderState::RenderState(Renderer *& renderer) :
		shape(),
		textRenderer(),
		strokeStyle(),
		activeFill(nullptr),
		activeStroke(nullptr),
		solidFill(SolidColorBrush(renderer)),
		solidStroke(SolidColorBrush(renderer)),
		linearFill(LinearGradientBrush(renderer)),
		linearStroke(LinearGradientBrush(renderer)),
		radialFill(RadialGradientBrush(renderer)),
		radialStroke(RadialGradientBrush(renderer)),
		bitmapFill(BitmapBrush(renderer)),
		bitmapStroke(BitmapBrush(renderer)),
		rectMode(DrawMode::Corner),
		ellipseMode(DrawMode::Center),
		imageMode(DrawMode::Corner),
		strokeWeight(1.f),
		defaultMatrix(D2D1::Matrix3x2F::Identity()),
		activeMatrix(&this->defaultMatrix),
		metrics(),
		renderer(renderer)
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

		if (ID2D1RenderTarget * rt = this->renderer->GetRenderTarget())
		{
			// activate the matrix as the new transformation
			rt->SetTransform(this->GetActiveMatrix());
		}
	}

}