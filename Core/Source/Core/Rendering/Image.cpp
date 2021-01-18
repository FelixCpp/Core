#include <Core/Rendering/Image.hpp>
#include <Core/Rendering/GraphicsContext.hpp> // GraphicsContext
#include <Core/System/Logger.hpp> // CORE_ERROR

#include <wrl/client.h> // Microsoft::WRL::ComPtr
#include <d2d1.h> // Direct2D functionality
#include <wincodec.h> // WIC (Windows Imaging Component)

using Microsoft::WRL::ComPtr;

namespace Core
{

	/// <summary>
	/// Image implementation
	/// </summary>
	struct Image::Implementation {
		ComPtr<ID2D1Bitmap> bitmap = nullptr;
	};

	Image::Image() :
		width(0),
		height(0),
		mode(ImageInterpolationMode::NearestNeighbor),
		opacity(255),
		colors(),
		impl(std::make_shared<Implementation>())
	{
	}

	bool Image::Create(i32_t width, i32_t height, const Color colors[], GraphicsContext * gctx)
	{
		// handy typedef for the implementations attribute
		auto & bitmap = this->impl->bitmap;

		// Get the RenderTarget
		ID2D1HwndRenderTarget * renderTarget = gctx->renderTarget.Get();
		if (!renderTarget)
		{
			CORE_ERROR("There is no RenderTarget");
			return false;
		}

		// Get the pixel format
		const D2D1_PIXEL_FORMAT pixelFormat = renderTarget->GetPixelFormat();

		// Get the dpiX, dpiY values
		FLOAT dpiX = 0.f, dpiY = 0.f;
		renderTarget->GetDpi(&dpiX, &dpiY);

		// Calculate the pitch (width * sizeof element + byteoffset in class)
		const UINT pitch = width * sizeof Color + 0;

		// create the bitmap
		HRESULT hr = renderTarget->CreateBitmap(
			D2D1::SizeU(width, height),
			colors,
			pitch,
			D2D1::BitmapProperties(pixelFormat, dpiX, dpiY),
			&bitmap
		);

		// error handling
		if (FAILED(hr))
		{
			CORE_ERROR("Failed to create a new Bitmap with the given colors");
			return false;
		}

		this->width = width;
		this->height = height;
		this->colors.resize(width * height);
		this->colors.assign(colors, colors + width * height);

		return true;
	}

	bool Image::LoadFromFile(const std::string & filepath, GraphicsContext * gctx)
	{
		// Get the ImagingFactory
		IWICImagingFactory * factory = gctx->imagingFactory.Get();

		// error handling
		if (!factory)
		{
			CORE_ERROR("There is no ImagingFactory");
			return false;
		}

		// Create a Decoder from the filepath
		const std::wstring wFilepath(filepath.begin(), filepath.end());
		ComPtr<IWICBitmapDecoder> pDecoder = nullptr;
		HRESULT hr = factory->CreateDecoderFromFilename(
			wFilepath.c_str(),
			nullptr,
			GENERIC_READ,
			WICDecodeMetadataCacheOnLoad,
			&pDecoder
		);

		if (FAILED(hr))
		{
			CORE_ERROR("Failed to create a Decoder from \"%s\"", filepath.c_str());
			return false;
		}

		// Create the initial frame.
		ComPtr<IWICBitmapFrameDecode> pSource = nullptr;
		hr = pDecoder->GetFrame(0, &pSource);

		// error handling
		if (FAILED(hr))
		{
			CORE_ERROR("Failed to get the initial frame");
			return false;
		}

		// Convert the image format to 32bppPBGRA
		// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
		ComPtr<IWICFormatConverter> pConverter = nullptr;
		hr = factory->CreateFormatConverter(&pConverter);
		
		// error handling
		if (FAILED(hr))
		{
			CORE_ERROR("Failed to create a FormatConverter");
			return false;
		}

		// Initialize the FormatConverter
		hr = pConverter->Initialize(
			pSource.Get(),
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			nullptr,
			0.0,
			WICBitmapPaletteTypeMedianCut
		);

		// error handling
		if (FAILED(hr))
		{
			CORE_ERROR("Failed to initialize the FormatConverter");
			return false;
		}
		
		// Get the RenderTarget
		ID2D1HwndRenderTarget * renderTarget = gctx->renderTarget.Get();
		if (!renderTarget)
		{
			CORE_ERROR("There is no RenderTarget");
			return false;
		}

		// Create a Direct2D bitmap from the WIC bitmap.
		hr = renderTarget->CreateBitmapFromWicBitmap(
			pConverter.Get(),
			nullptr,
			&this->impl->bitmap
		);

		// error handling
		if(FAILED(hr))
		{
			CORE_ERROR("Failed to create a ID2D1Bitmap from IWICBitmap");
			return false;
		}

		// Get the size of the image
		UINT uiWidth = 0, uiHeight = 0;
		hr = pConverter->GetSize(&uiWidth, &uiHeight);
		
		// error handling
		if (FAILED(hr))
		{
			CORE_ERROR("Failed to get the size from the FormatConverter");
			return false;
		}

		// Copy the values
		this->width = (i32_t)uiWidth;
		this->height = (i32_t)uiHeight;

		// Get the color values
		
		// Create the bitmap from the image frame
		ComPtr<IWICBitmap> wicBitmap = nullptr;
		hr = factory->CreateBitmapFromSource(
			pSource.Get(),			// Create a bitmap from the image frame
			WICBitmapCacheOnDemand,	// Cache metadata when needed
			&wicBitmap				// Pointer to the bitmap
		);

		// error handling
		if (FAILED(hr))
		{
			CORE_ERROR("Failed to create a Bitmap from the initial frame");
			return false;
		}

		// Obtain a bitmap lock for exclusive write.
		// The lock is for a 10x10 rectangle starting at the top left of the
		// bitmap.
		const WICRect lockBoundary = { 0, 0, (INT)uiWidth, (INT)uiHeight };
		ComPtr<IWICBitmapLock> bitmapLock = nullptr;
		hr = wicBitmap->Lock(&lockBoundary, WICBitmapLockRead, &bitmapLock);
		
		// error handling
		if (FAILED(hr))
		{
			CORE_ERROR("Failed locking bitmap for reading the pixels");
			return false;
		}

		UINT cbBufferSize = 0;
		BYTE * data = NULL;

		// Retrieve a pointer to the pixel data.
		hr = bitmapLock->GetDataPointer(&cbBufferSize, &data);
		
		if (FAILED(hr))
		{
			CORE_ERROR("Failed to get raw data from the bitmap");
			return false;
		}

		// Pixel manipulation using the image data pointer pv.
		// ...
		const bool hasAlpha = cbBufferSize == uiWidth * uiHeight * 4;
		const i32_t colorChannels = hasAlpha ? 4 : 3;

		// make space for the colors
		this->colors.resize(uiWidth * uiHeight);
		for (UINT y = 0u; y < uiHeight; y++)
		{
			for (UINT x = 0u; x < uiWidth; x++)
			{
				const UINT dataIndex  = (y * uiWidth + x) * colorChannels;
				const UINT colorIndex = (y * uiWidth + x);

				this->colors[colorIndex].r = data[dataIndex + 0];
				this->colors[colorIndex].g = data[dataIndex + 1];
				this->colors[colorIndex].b = data[dataIndex + 2];
				this->colors[colorIndex].a = hasAlpha ? data[dataIndex + 3] : 255;
			}
		}

		return true;
	}

	bool Image::LoadFromMemory(i32_t width, i32_t height, const Color colors[], GraphicsContext * gctx)
	{
		auto & bitmap = this->impl->bitmap;
	
		if (bitmap)
		{
			// Copy the pixels into the existing bitmap
			// we can't return this method because we want to copy the values
			// into their member variables futher down
			if (!this->CopyFromRawData(width, height, colors))
			{
				return false;
			}

			// Copy the values
			this->width = width;
			this->height = height;

			this->colors.resize(width * height);
			this->colors.assign(colors, colors + width * height);
		} else
		{
			// Create a brand new bitmap
			if (!this->Create(width, height, colors, gctx))
			{
				return false;
			}

			// note: we don't need to copy the data here because
			// Create(...) already does that for us
		}

		return true;
	}

	void Image::LoadColors()
	{
		// nothing to do
	}

	bool Image::UpdateColors()
	{
		// TODO: Copy the colors into the bitmap
		return this->CopyFromRawData(this->width, this->height, this->colors.data());
	}

	ID2D1Bitmap * Image::GetBitmap() const
	{
		return this->impl->bitmap.Get();
	}

	bool Image::CopyFromRawData(i32_t width, i32_t height, const Color colors[])
	{
		auto & bitmap = this->impl->bitmap;

		// copy the colors from memory into the bitmap
		const D2D1_RECT_U dstRect = { 0u, 0u, (UINT)width, (UINT)height };
		HRESULT hr = bitmap->CopyFromMemory(&dstRect, colors, width * sizeof Color + 0);

		// error handling
		if (FAILED(hr))
		{
			CORE_ERROR("Failed to copy the pixels from memory into existing Bitmap");
			return false;
		}

		return true;
	}

} // namespace Core