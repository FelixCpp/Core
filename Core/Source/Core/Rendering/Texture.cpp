#include <Core/Rendering/Texture.hpp>
#include <Core/Rendering/GraphicsContext.hpp>

#include <Core/System/Logger.hpp>

#include <wrl/client.h>
#include <d2d1.h>

namespace Core
{

	struct Texture::Implementation {
		Microsoft::WRL::ComPtr<ID2D1Bitmap> d2dBitmap = nullptr;
	};

	Texture::Texture() :
		colors(),
		width(0),
		height(0),
		mode(ImageInterpolationMode::NearestNeighbor),
		opacity(255),
		impl(std::make_shared<Implementation>())
	{
	}

	bool Texture::LoadFromFile(const std::string & filepath, GraphicsContext * gctx)
	{
		// Get the imaging factory
		IWICImagingFactory * factory = gctx->imagingFactory.Get();
		if (!factory)
		{
			CORE_ERROR("There is no ImagingFactory");
			return false;
		}

		ID2D1HwndRenderTarget * renderTarget = gctx->renderTarget.Get();
		if (!renderTarget)
		{
			CORE_ERROR("There is no RenderTarget");
			return false;
		}

		auto & d2dBitmap = this->impl->d2dBitmap;
		//auto & wicBitmap = this->impl->wicBitmap;

		// Create Decoder
		Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder = nullptr;
		Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> decoderFrame = nullptr;
		const std::wstring wFilepath(filepath.begin(), filepath.end());
		HRESULT hr = factory->CreateDecoderFromFilename(
			wFilepath.c_str(),				 // Image to be decoded
			nullptr,						 // Do not prefer a particular vendor
			GENERIC_READ,					 // Desired read access to the file
			WICDecodeMetadataCacheOnDemand,	 // Cache metadata when needed
			&decoder						 // Pointer to the decoder
		);

		// error handling
		if (FAILED(hr))
		{
			CORE_ERROR("Failed to create an ImageDecoder from filename \"%s\"", filepath.c_str());
			return false;
		}

		// Get the frame
		hr = decoder->GetFrame(0, &decoderFrame);

		// error handling
		if (FAILED(hr))
		{
			CORE_ERROR("Failed to get the initial frame of the ImageDecoder");
			return false;
		}

		// Get the size of the frame
		UINT uiWidth = 0, uiHeight = 0;
		hr = decoderFrame->GetSize(&uiWidth, &uiHeight);

		// error handling
		if (FAILED(hr))
		{
			CORE_ERROR("Failed to get the size of the initial frame");
			return false;
		}

		// Create the bitmap from the image frame.
		Microsoft::WRL::ComPtr<IWICBitmap> wicBitmap = nullptr;
		WICRect rcLock = { 0, 0, (INT)uiWidth, (INT)uiHeight };
		hr = factory->CreateBitmapFromSource(
			decoderFrame.Get(),     // Create a bitmap from the image frame
			WICBitmapCacheOnDemand, // Cache metadata when needed
			&wicBitmap				// Pointer to the bitmap
		);

		// error handling
		if (FAILED(hr))
		{
			CORE_ERROR("Failed to create a BitmapSource using the initial frame");
			return false;
		}

		// Obtain a bitmap lock for exclusive write.
		// The lock is for a uiWidth * uiHeight rectangle starting at the top left of the
		// bitmap.
		Microsoft::WRL::ComPtr<IWICBitmapLock> lock = nullptr;
		hr = wicBitmap->Lock(&rcLock, WICBitmapLockRead, &lock);
		if (FAILED(hr))
		{
			CORE_ERROR("Failed to lock the bitmap for writing");
			return false;
		}

		UINT cbBufferSize = 0;
		BYTE * data = nullptr;

		// Retrieve a pointer to the pixel data.
		hr = lock->GetDataPointer(&cbBufferSize, &data);

		// error handling
		if (FAILED(hr))
		{
			CORE_ERROR("Failed to get DataPointer (colors) from the bitmap");
			return false;
		}

		// Pixel manipulation using the image data pointer pv.
		// ...

		// get the number of channels
		const bool rgb = cbBufferSize == uiWidth * uiHeight * 3;
		const bool rgba = cbBufferSize == uiWidth * uiHeight * 4;
		i32_t colorChannels = 0;

		if (rgb) { colorChannels = 3; } else if (rgba) { colorChannels = 4; } else
		{
			CORE_INFO("Invalid number of color channels (supported: rgb / rgba)");
			return false;
		}

		// make space for the colors
		this->colors.resize(uiWidth * uiHeight);

		for (UINT y = 0; y < uiHeight; y++) // through every row
		{
			for (UINT x = 0; x < uiWidth; x++) // through every column
			{
				const UINT dataIndex = (y * uiWidth + x) * colorChannels;
				const size_t colorIndex = y * uiWidth + x;

				this->colors[colorIndex].r = data[dataIndex + 0];
				this->colors[colorIndex].g = data[dataIndex + 1];
				this->colors[colorIndex].b = data[dataIndex + 2];
				this->colors[colorIndex].a = rgba ? data[dataIndex + 3] : 255;
			}
		}

		// Convert the image format to 32bppPBGRA
		// DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED)
		Microsoft::WRL::ComPtr<IWICFormatConverter> formatConverter = nullptr;
		hr = factory->CreateFormatConverter(&formatConverter);
		if (FAILED(hr))
		{
			CORE_ERROR("Failed to create a FormatConverter");
			return false;
		}

		formatConverter->Initialize(
			decoderFrame.Get(),
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

		hr = renderTarget->CreateBitmapFromWicBitmap(
			formatConverter.Get(),
			nullptr,
			&d2dBitmap
		);
		if (FAILED(hr))
		{
			CORE_ERROR("Failed to create a D2D1Bitmap from WICBitmap");
			return false;
		}

		// copy the dimensions
		this->width = (i32_t)uiWidth;
		this->height = (i32_t)uiHeight;

		return true;
	}

	bool Texture::LoadFromMemory(i32_t width, i32_t height, const Color * colors, GraphicsContext * gctx)
	{
		// Get the RenderTarget
		ID2D1HwndRenderTarget * renderTarget = gctx->renderTarget.Get();
		if (!renderTarget)
		{
			CORE_ERROR("There is no RenderTarget");
			return false;
		}

		// Copy the colors
		this->colors.resize(width * height);
		this->colors.assign(colors, colors + width * height);

		auto & d2dBitmap = this->impl->d2dBitmap;

		if (d2dBitmap)
		{
			// TODO: copy the data into the existing image
			const D2D1_RECT_U dstRect = D2D1::RectU(0, 0, width, height);
			const HRESULT hr = d2dBitmap->CopyFromMemory(
				&dstRect, colors, width * sizeof Color + 0
			);

			if (FAILED(hr))
			{
				CORE_ERROR("Failed to copy the pixels into the image");
				return false;
			}
		} else
		{
			// TODO: Create a brand new image
			const D2D1_PIXEL_FORMAT pixelFormat = renderTarget->GetPixelFormat();
			FLOAT dpiX = 0.f, dpiY = 0.f;
			renderTarget->GetDpi(&dpiX, &dpiY);

			const HRESULT hr = renderTarget->CreateBitmap(
				D2D1::SizeU(width, height),
				colors,
				width * sizeof Color + 0,
				D2D1::BitmapProperties(pixelFormat, dpiX, dpiY),
				&d2dBitmap
			);

			if (FAILED(hr))
			{
				CORE_ERROR("Failed to create a bitmap with the specified colors");
				return false;
			}
		}

		// copy the dimensions
		this->width = (i32_t)width;
		this->height = (i32_t)height;

		return false;
	}

	ID2D1Bitmap * Texture::GetBitmap() const
	{
		return this->impl->d2dBitmap.Get();
	}

} // namespace Core