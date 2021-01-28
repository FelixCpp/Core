#include <Core/Rendering/Targets/RenderTarget.hpp>
#include <Core/Rendering/Renderers/Renderer.hpp>

#include <d2d1.h>

namespace Core
{

	RenderTarget::RenderTarget(LateRef<Renderer> renderer, LateRef<RenderStateManager> rsm) :
		ImageRenderTarget(renderer, rsm),
		ShapeRenderTarget(renderer, rsm),
		StrokeStyleTarget(rsm),
		RenderStateTarget(rsm),
		TransformationTarget(rsm),
		BrushTarget(rsm),
		TextRenderTarget(renderer, rsm),
		PrimitiveRenderTarget(renderer, rsm),
		renderer(renderer)
	{
	}

	void RenderTarget::Background(const Color & color)
	{
		this->renderer->GetRenderTarget()->Clear(D2D1::ColorF(
			(float)color.r / 255.f,
			(float)color.g / 255.f,
			(float)color.b / 255.f,
			1.f
		));
	}

}