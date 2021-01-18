#include <Core/Rendering/ImageSource.hpp>
#include <Core/Rendering/GraphicsContext.hpp>
#include <Core/System/Logger.hpp>

#include <wrl/client.h>
#include <wincodec.h>

namespace Core
{

	struct ImageSource::Implementation {
		Microsoft::WRL::ComPtr<IWICBitmap> bitmap = nullptr;
	};

	ImageSource::ImageSource() :
		impl(std::make_shared<Implementation>()),
		colors(),
		width(0),
		height(0)
	{
	}

	bool ImageSource::LoadFromFile(const std::string & filepath, GraphicsContext * gctx)
	{
		// Get the imaging factory
		IWICImagingFactory * factory = gctx->imagingFactory.Get();
		if (!factory)
		{
			CORE_ERROR("There is no ImagingFactory");
			return false;
		}

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
		WICRect rcLock = { 0, 0, (INT)uiWidth, (INT)uiHeight };
		hr = factory->CreateBitmapFromSource(
			decoderFrame.Get(),     // Create a bitmap from the image frame
			WICBitmapCacheOnDemand, // Cache metadata when needed
			&this->impl->bitmap		// Pointer to the bitmap
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
		hr = this->impl->bitmap->Lock(&rcLock, WICBitmapLockRead, &lock);
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

		if (rgb) { colorChannels = 3; }
		else if (rgba) { colorChannels = 4; }
		else
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

		// copy the dimensions
		this->width = (i32_t)uiWidth;
		this->height = (i32_t)uiHeight;

		return true;
	}

} // namespace Core