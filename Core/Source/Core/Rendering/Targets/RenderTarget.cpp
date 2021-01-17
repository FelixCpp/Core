#include <Core/Rendering/Targets/RenderTarget.hpp>
#include <Core/Rendering/GraphicsContext.hpp>
#include <Core/Rendering/RenderStateManager.hpp>

#include <Core/Window/Window.hpp>

namespace Core
{

	RenderTarget::RenderTarget(GraphicsContext *& gctx, RenderStateManager *& rsm) :
		ImageRenderTarget(gctx, rsm),
		ShapeRenderTarget(gctx, rsm),
		StrokeStyleTarget(rsm),
		RenderStateTarget(rsm),
		TransformationTarget(rsm),
		BrushTarget(rsm),
		TextRenderTarget(gctx, rsm),
		PrimitiveRenderTarget(gctx, rsm),
		gctx(gctx),
		rsm(rsm)
	{
	}

	void RenderTarget::Background(const Color & color)
	{
		this->gctx->renderTarget->Clear(D2D1::ColorF(
			(float)color.r / 255.f,
			(float)color.g / 255.f,
			(float)color.b / 255.f,
			1.f
		));
	}

}