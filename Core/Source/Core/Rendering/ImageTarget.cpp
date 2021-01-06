#include <Core/Rendering/ImageTarget.hpp>

#include <Core/Rendering/GraphicsContext.hpp>
#include <Core/Rendering/RenderStateManager.hpp>

namespace Core
{

	ImageTarget::ImageTarget(GraphicsContext *& gctx, RenderStateManager *& rsm) :
		gctx(gctx),
		rsm(rsm)
	{
	}

	Image ImageTarget::createImage(u32_t width, u32_t height, const Color & color)
	{
		Image image;
		image.create(width, height, color, this->gctx);
		return image;
	}

	Image ImageTarget::loadImageFromMemory(u32_t width, u32_t height, const Color * colors)
	{
		Image image;
		image.loadFromMemory(width, height, colors, this->gctx);
		return image;
	}

	Image ImageTarget::loadImageFromFile(const std::string & filepath)
	{
		Image image;
		image.loadFromFile(filepath, this->gctx);
		return image;
	}

	void ImageTarget::image(const Image & image, float x, float y)
	{
		this->image(image, x, y, (float)image.width, (float)image.height);
	}

	void ImageTarget::image(const Image & image, float x, float y, float width, float height)
	{
		ID2D1HwndRenderTarget * rt = this->gctx->hwndRenderTarget.Get();
		if (!rt) return;

		ID2D1Bitmap * bitmap = image.getBitmap();
		if (!bitmap) return;

		D2D1_RECT_F destinationRectangle = D2D1::RectF();
		switch (this->rsm->getActiveState().imageMode)
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