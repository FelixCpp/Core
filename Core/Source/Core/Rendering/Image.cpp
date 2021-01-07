 #include <Core/Rendering/Image.hpp>
#include <Core/Rendering/GraphicsContext.hpp>

#include <Core/System/Logger.hpp>

#include <wrl/client.h>

namespace Core
{

	struct Image::Implementation {
		
		Microsoft::WRL::ComPtr<ID2D1Bitmap> bitmap = nullptr;

	};

	Image::Image() :
		width(0),
		height(0),
		mode(ImageInterpolationMode::NearestNeighbor),
		opacity(1.f),
		impl(std::make_shared<Implementation>())
	{ }

	bool Image::create(u32_t width, u32_t height, const Color & color, GraphicsContext * gctx)
	{
		ID2D1HwndRenderTarget * rt = gctx->hwndRenderTarget.Get();
		if (!rt) return false;

		const D2D1_PIXEL_FORMAT pixelFormat = rt->GetPixelFormat();
		FLOAT dpiX = 0.f, dpiY = 0.f;
		rt->GetDpi(&dpiX, &dpiY);

		u32_t * srcData = new u32_t[width * height];
		std::fill_n(srcData, width * height, color.argb());
		
		const HRESULT hr = rt->CreateBitmap(
			D2D1::SizeU(width, height),
			srcData,
			width * sizeof u32_t + 0,
			D2D1::BitmapProperties(pixelFormat, dpiX, dpiY),
			&this->impl->bitmap
		);

		delete[] srcData;
		srcData = nullptr;

		if (FAILED(hr))
		{
			CORE_ERROR("Failed to create a Bitmap");
			return false;
		}
		
		this->width = (i32_t)width;
		this->height = (i32_t)height;

		return true;
	}

	bool Image::loadFromMemory(u32_t width, u32_t height, const Color * colors, GraphicsContext * gctx)
	{
		ID2D1HwndRenderTarget * rt = gctx->hwndRenderTarget.Get();
		if (!rt) return false;

		if (this->impl->bitmap)
		{
			/* copy the data into the existing image */
			const D2D1_RECT_U dstRect = D2D1::RectU(0, 0, width, height);
			const HRESULT hr = this->impl->bitmap->CopyFromMemory(
				&dstRect, colors, width * sizeof Color + 0
			);

			if (FAILED(hr))
			{
				CORE_ERROR("Failed to copy the pixels into the image");
				return false;
			}
		} else
		{
			/* create an image */
			const D2D1_PIXEL_FORMAT pixelFormat = rt->GetPixelFormat();
			FLOAT dpiX = 0.f, dpiY = 0.f;
			rt->GetDpi(&dpiX, &dpiY);

			const HRESULT hr = rt->CreateBitmap(
				D2D1::SizeU(width, height),
				colors,
				width * sizeof Color + 0,
				D2D1::BitmapProperties(pixelFormat, dpiX, dpiY),
				&this->impl->bitmap
			);

			if (FAILED(hr))
			{
				CORE_ERROR("Failed to create a bitmap with the specified colors");
				return false;
			}
		}

		this->width = (i32_t)width;
		this->height = (i32_t)height;

		return true;
	}

	bool Image::loadFromFile(const std::string & filepath, GraphicsContext * gctx)
	{
		IWICImagingFactory * factory = gctx->imagingFactory.Get();
		if (!factory) return false;

		Microsoft::WRL::ComPtr<IWICBitmapDecoder> pDecoder = nullptr;
		Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> pSource = nullptr;
		Microsoft::WRL::ComPtr<IWICStream> pStream = nullptr;
		Microsoft::WRL::ComPtr<IWICFormatConverter> pConverter = nullptr;
		Microsoft::WRL::ComPtr<IWICBitmapScaler> pScaler = nullptr;

		const std::wstring uri(filepath.begin(), filepath.end());
		HRESULT hr = factory->CreateDecoderFromFilename(
			uri.c_str(),
			nullptr,
			GENERIC_READ,
			WICDecodeMetadataCacheOnLoad,
			&pDecoder
		);

		if (FAILED(hr))
		{
			CORE_ERROR("Failed to create an ImageDecoder from filename \"%s\"", filepath.c_str());
			return false;
		}

		/* Create the initial frame. */
		hr = pDecoder->GetFrame(0, &pSource);
		if (FAILED(hr))
		{
			CORE_ERROR("Failed to get the initial frame of the ImageDecoder");
			return false;
		}
		
		/*
			Convert the image format to 32bppPBGRA
			(DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
		*/
		hr = factory->CreateFormatConverter(&pConverter);
		if (FAILED(hr))
		{
			CORE_ERROR("Failed to create a FormatConverter");
			return false;
		}

		hr = pConverter->Initialize(
			pSource.Get(),
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			nullptr,
			0.0,
			WICBitmapPaletteTypeMedianCut
		);
		if (FAILED(hr))
		{
			CORE_ERROR("Failed to initialize the FormatConverter");
			return false;
		}

		ID2D1HwndRenderTarget * rt = gctx->hwndRenderTarget.Get();
		if (!rt) return false;

		/* Create a Direct2D bitmap from the WIC bitmap. */
		hr = rt->CreateBitmapFromWicBitmap(
			pConverter.Get(),
			nullptr,
			&this->impl->bitmap
		);
		if (FAILED(hr))
		{
			CORE_ERROR("Failed to create a Bitmap from WicBitmap");
			return false;
		}

		/* copy the size */
		const D2D1_SIZE_F size = this->impl->bitmap->GetSize();
		this->width = (i32_t)size.width;
		this->height = (i32_t)size.height;

		return true;
	}

	ID2D1Bitmap * Image::getBitmap() const
	{
		return this->impl->bitmap.Get();
	}


}