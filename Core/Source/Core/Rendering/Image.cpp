#include <Core/Rendering/Image.hpp>
#include <Core/Rendering/Renderers/Renderer.hpp> // Core::Renderer
#include <Core/Rendering/FactoryManager.hpp> // Core::FactoryManager

#include <Core/System/Logger.hpp> // CORE_ERROR
#include <Core/Maths/Math.hpp> // Core::FMath

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

	bool Image::Create(i32_t width, i32_t height, const Color colors[], Renderer * renderer)
	{
		// handy typedef for the implementations attribute
		auto & bitmap = this->impl->bitmap;

		// Get the RenderTarget
		ID2D1RenderTarget * renderTarget = renderer->GetRenderTarget();
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

		// Copy the values
		this->width = width;
		this->height = height;
		this->colors.assign(colors, colors + width * height);

		return true;
	}

	bool Image::LoadFromFile(const std::string & filepath, Renderer * renderer)
	{
		// Get the ImagingFactory
		IWICImagingFactory * factory = FactoryManager::wicFactory.Get();

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
		ID2D1RenderTarget * renderTarget = renderer->GetRenderTarget();
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

	bool Image::LoadFromMemory(i32_t width, i32_t height, const Color colors[], Renderer * renderer)
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
			this->colors.assign(colors, colors + width * height);
		} else
		{
			// Create a brand new bitmap
			if (!this->Create(width, height, colors, renderer))
			{
				return false;
			}

			// note: we don't need to copy the data here because
			// Create(...) already does that for us
		}

		return true;
	}

	bool Image::LoadFromImage(const Image & other, i32_t x, i32_t y, i32_t width, i32_t height, Renderer * renderer)
	{
		auto & bitmap = this->impl->bitmap;
		ID2D1Bitmap * source = other.GetBitmap();
		if (!source)
		{
			this->width = 0;
			this->height = 0;
			this->colors.clear();
			bitmap.Reset();
			return true;
		}

		// clamp the values so the full width and full height can always be copied
		x = (i32_t)FMath::Constrain((float)x, 0.f, (float)(other.width - width));
		y = (i32_t)FMath::Constrain((float)y, 0.f, (float)(other.height - height));

		if (!bitmap)
		{
			// we need to create a brand new bitmap to copy the data into later
			// Get the RenderTarget
			ID2D1RenderTarget * renderTarget = renderer->GetRenderTarget();
			if (!renderTarget)
			{
				CORE_ERROR("There is no RenderTarget");
				return false;
			}

			// Get the pixel format
			const D2D1_PIXEL_FORMAT pixelFormat = source->GetPixelFormat();

			// Get the dpiX, dpiY values
			FLOAT dpiX = 0.f, dpiY = 0.f;
			source->GetDpi(&dpiX, &dpiY);

			// Calculate the pitch (width * sizeof element + byteoffset in class)
			const UINT pitch = width * sizeof Color + 0;

			// create the bitmap
			HRESULT hr = renderTarget->CreateBitmap(
				D2D1::SizeU(width, height),
				D2D1::BitmapProperties(pixelFormat, dpiX, dpiY),
				&bitmap
			);

			// error handling
			if (FAILED(hr))
			{
				CORE_ERROR("Failed to create a new Bitmap with the given colors");
				return false;
			}
		}

		// copy the section
		const D2D1_POINT_2U destPoint = { 0u, 0u };
		const D2D1_RECT_U srcRect = { (UINT)x, (UINT)y, x + (UINT)width, y + (UINT)height };
		
		// Copy the bitmap
		HRESULT hr = bitmap->CopyFromBitmap(&destPoint, source, &srcRect);

		// error handling
		if (FAILED(hr))
		{
			CORE_ERROR("Failed to copy the bitmap from the given image");
			return false;
		}

		// copy the dimensions
		this->width = width;
		this->height = height;
		
		// copy the pixels
		this->colors.resize(this->width * this->height);

		const auto xd = x;
		const auto yd = y;
		for (u32_t y = 0; y < this->height; y++)
		{
			for (u32_t x = 0; x < this->width; x++)
			{
				this->colors[y * this->width + x] = other.colors[yd * width + xd];
			}
		}

		return true;
	}

	bool Image::LoadFromScreen(i32_t x, i32_t y, i32_t width, i32_t height, Renderer * renderer)
	{
		auto & bitmap = this->impl->bitmap;

		// get the RenderTarget
		ID2D1RenderTarget * renderTarget = renderer->GetRenderTarget();
		if (!renderTarget)
		{
			CORE_ERROR("There is no RenderTarget");
			return false;
		}

		const D2D1_SIZE_F rtSize = renderTarget->GetSize();
		// clamp the values so the full width and full height can always be copied
		x = (i32_t)FMath::Constrain((float)x, 0.f, (float)(rtSize.width - width));
		y = (i32_t)FMath::Constrain((float)y, 0.f, (float)(rtSize.height - height));

		// Get the pixel format
		const D2D1_PIXEL_FORMAT pixelFormat = renderTarget->GetPixelFormat();

		// Get the dpiX, dpiY values
		FLOAT dpiX = 0.f, dpiY = 0.f;
		renderTarget->GetDpi(&dpiX, &dpiY);

		// create the bitmap properties
		const D2D1_BITMAP_PROPERTIES bitmapProperties = D2D1::BitmapProperties(
			pixelFormat,
			dpiX, dpiY
		);

		const D2D1_SIZE_U size = D2D1::SizeU(width, height);

		HRESULT hr = renderTarget->CreateBitmap(size, bitmapProperties, &bitmap);
		if (FAILED(hr))
		{
			CORE_ERROR("Failed to create a Bitmap");
			return false;
		}

		const D2D1_POINT_2U destPoint = D2D1::Point2U(0, 0);
		const D2D1_RECT_U srcRect = D2D1::RectU(x, y, x + width, y + height);

		// Copy data from the RenderTarget
		hr = bitmap->CopyFromRenderTarget(&destPoint, renderTarget, &srcRect);
		if (FAILED(hr))
		{
			CORE_ERROR("Failed to copy data from the RenderTarget");
			return false;
		}

		// Copy the pixels from screen

		this->width = width;
		this->height = height;
		this->colors.clear();

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