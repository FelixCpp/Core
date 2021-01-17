#include <Core/Rendering/Targets/ImageTarget.hpp>

#include <Core/Rendering/GraphicsContext.hpp>
#include <Core/Rendering/RenderStateManager.hpp>

namespace Core
{

	ImageTarget::ImageTarget(GraphicsContext *& gctx, RenderStateManager *& rsm) :
		gctx(gctx),
		rsm(rsm)
	{
	}

	Image ImageTarget::CreateImage(u32_t width, u32_t height, const Color & color)
	{
		Core::Image image;
		image.Create(width, height, color, this->gctx);
		return image;
	}

	Image ImageTarget::LoadImageFromMemory(u32_t width, u32_t height, const Color * colors)
	{
		Core::Image image;
		image.LoadFromMemory(width, height, colors, this->gctx);
		return image;
	}

	Image ImageTarget::LoadImageFromFile(const std::string & filepath)
	{
		Core::Image image;
		image.LoadFromFile(filepath, this->gctx);
		return image;
	}

	Image ImageTarget::LoadImageFromScreen(i32_t x, i32_t y, i32_t width, i32_t height)
	{
		Core::Image image;
		image.LoadFromScreen(x, y, width, height, this->gctx);
		return image;
	}

	void ImageTarget::ImageMode(DrawMode mode)
	{
		this->rsm->GetActiveState().imageMode = mode;
	}

	void ImageTarget::Image(const Core::Image & image, float x, float y)
	{
		this->Image(image, x, y, (float)image.width, (float)image.height);
	}

	void ImageTarget::Image(const Core::Image & image, float x, float y, float width, float height)
	{
		ID2D1HwndRenderTarget * rt = this->gctx->renderTarget.Get();
		if (!rt) return;

		ID2D1Bitmap * bitmap = image.GetBitmap();
		if (!bitmap) return;

		D2D1_RECT_F destinationRectangle = D2D1::RectF();
		switch (this->rsm->GetActiveState().imageMode)
		{
			case DrawMode::Corner: destinationRectangle = D2D1::RectF(x, y, x + width, y + height); break;
			case DrawMode::Corners: destinationRectangle = D2D1::RectF(x, y, width, height); break;
			case DrawMode::Center: destinationRectangle = D2D1::RectF(x - width / 2.f, y - height / 2.f, x + width / 2.f, y + height / 2.f); break;
			default: break;
		}

		rt->DrawBitmap(
			bitmap,
			destinationRectangle,
			image.opacity,
			static_cast<D2D1_BITMAP_INTERPOLATION_MODE>(image.mode),
			D2D1::RectF(0.f, 0.f, (float)image.width, (float)image.height)
		);
	}

}