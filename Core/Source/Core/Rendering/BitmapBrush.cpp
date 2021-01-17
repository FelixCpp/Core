#include <Core/Rendering/BitmapBrush.hpp>
#include <Core/Rendering/GraphicsContext.hpp>

#include <wrl/client.h>
#include <d2d1.h>

namespace Core
{

	struct BitmapBrush::Implementation {
		Microsoft::WRL::ComPtr<ID2D1BitmapBrush> brush;

		void SetBitmap(ID2D1Bitmap * bitmap)
		{
			this->brush->SetBitmap(bitmap);
		}

		void CreateBrush(ID2D1HwndRenderTarget * renderTarget, ID2D1Bitmap * bitmap)
		{
			renderTarget->CreateBitmapBrush(bitmap, &this->brush);
		}

		void SetInterpolationMode(ImageInterpolationMode mode)
		{
			this->brush->SetInterpolationMode(static_cast<D2D1_BITMAP_INTERPOLATION_MODE>(mode));
		}

		void SetExtendModeX(ExtendMode mode)
		{
			this->brush->SetExtendModeX(static_cast<D2D1_EXTEND_MODE>(mode));
		}

		void SetExtendModeY(ExtendMode mode)
		{
			this->brush->SetExtendModeY(static_cast<D2D1_EXTEND_MODE>(mode));
		}

		void SetOpacity(i32_t opacity)
		{
			this->brush->SetOpacity((FLOAT)opacity / 255.f);
		}

	};

	BitmapBrush::BitmapBrush(GraphicsContext *& gctx) :
		impl(std::make_shared<Implementation>()),
		gctx(gctx),
		image(),
		interpolationMode(ImageInterpolationMode::NearestNeighbor),
		modeX(ExtendMode::Clamp),
		modeY(ExtendMode::Clamp)
	{
	}

	void BitmapBrush::SetImage(const Image & image)
	{
		this->image = image;

		ID2D1Bitmap * bitmap = image.GetBitmap();

		if (this->impl->brush)
		{
			this->impl->SetBitmap(bitmap);
			this->impl->SetOpacity(image.opacity);
		} else
		{
			if (ID2D1HwndRenderTarget * renderTarget = this->gctx->renderTarget.Get())
			{
				this->impl->CreateBrush(renderTarget, bitmap);
				this->impl->SetExtendModeX(this->modeX);
				this->impl->SetExtendModeY(this->modeY);
				this->impl->SetInterpolationMode(this->interpolationMode);
				this->impl->SetOpacity(image.opacity);
			}
		}
	}

	const Image & BitmapBrush::GetImage() const
	{
		return this->image;
	}

	void BitmapBrush::SetInterpolationMode(ImageInterpolationMode mode)
	{
		if (this->interpolationMode != mode)
		{
			this->interpolationMode = mode;

			if (this->impl->brush)
			{
				this->impl->SetInterpolationMode(mode);
			}
		}
	}

	ImageInterpolationMode BitmapBrush::GetInterpolationMode() const
	{
		return this->interpolationMode;
	}

	void BitmapBrush::SetExtendModeX(ExtendMode modeX)
	{
		if (this->modeX != modeX)
		{
			this->modeX = modeX;

			if (this->impl->brush)
			{
				this->impl->SetExtendModeX(modeX);
			}
		}
	}

	ExtendMode BitmapBrush::GetExtendModeX() const
	{
		return this->modeX;
	}

	void BitmapBrush::SetExtendModeY(ExtendMode modeY)
	{
		if (this->modeY != modeY)
		{
			this->modeY = modeY;

			if (this->impl->brush)
			{
				this->impl->SetExtendModeY(modeY);
			}
		}
	}

	ExtendMode BitmapBrush::GetExtendModeY() const
	{
		return this->modeY;
	}

	ID2D1Brush * BitmapBrush::GetBrush() const
	{
		return this->impl->brush.Get();
	}

}