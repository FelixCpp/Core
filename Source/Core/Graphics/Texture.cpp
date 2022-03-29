// 
// Texture.cpp
// Core
// 
// Created by Felix Busch on 28.03.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
//

#include <Core/Graphics/Texture.hpp>
#include <Core/System/Error.hpp>
#include <Core/Application/Factories.hpp>
#include <Core/Application/Application.hpp>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <d2d1.h>
#include <wincodec.h>
#include <wrl/client.h>

namespace Core
{
	////////////////////////////////////////////////////////////
	/// \brief Define concrete implementation for the texture
	///		   class.
	/// 
	////////////////////////////////////////////////////////////
	class Texture::Impl
	{
	public:

		////////////////////////////////////////////////////////////
		/// Member data
		/// 
		////////////////////////////////////////////////////////////
		Microsoft::WRL::ComPtr<ID2D1Bitmap> Bitmap;

	};

	////////////////////////////////////////////////////////////
	Texture::Texture():
		impl(std::make_shared<Impl>())
	{
	}

	////////////////////////////////////////////////////////////
	bool Texture::LoadFromFile(const std::filesystem::path& filepath)
	{
		using Microsoft::WRL::ComPtr;

		ComPtr<IWICBitmapDecoder> pDecoder = nullptr;
		ComPtr<IWICBitmapFrameDecode> pSource = nullptr;
		ComPtr<IWICFormatConverter> pConverter = nullptr;

		IWICImagingFactory* imagingFactory = Factories::ImagingFactory.Get();
		if(!imagingFactory)
		{
			Err() << "There is no imaging factory. Make sure to setup the imaging factory before loading images from a file." << std::endl;
			return false;
		}

		// 1. Create an IWICBitmapDecoder by using the IWICImagingFactory::CreateDecoderFromFilename method.
		HRESULT success = imagingFactory->CreateDecoderFromFilename(filepath.wstring().c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder);
		if(FAILED(success))
		{
			Err() << "Failed to create an image decoder from filename: \"" << filepath.string() << "\"" << std::endl;
			return false;
		}

		// 2. Retrieve a frame from the image and save the frame in an IWICBitmapFrameDecode object.
		success = pDecoder->GetFrame(0, &pSource);
		if(FAILED(success))
		{
			Err() << "Failed to save a frame from the IWICBitmapDecoder object in an IWICBitmapFrameDecode object." << std::endl;
			return false;
		}

		// 3. The bitmap must be converted to a format that Direct2D can use. Therefore, convert the pixel format of
		//    the image to 32bppPBGRA. (For a list of supported formats, see:
		//    https://docs.microsoft.com/de-de/windows/win32/direct2d/supported-pixel-formats-and-alpha-modes.
		//
		//    Call the IWICImagingFactory::CreateFormatConverter method to create an IWICFormatConverter object,
		//    then call the Initialize method of the IWICFormatConverter object to perform the conversion.
		success = imagingFactory->CreateFormatConverter(&pConverter);
		if(FAILED(success))
		{
			Err() << "Failed to convert the image format in a format that Direct2D can use." << std::endl;
			return false;
		}

		success = pConverter->Initialize(pSource.Get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeMedianCut);
		if(FAILED(success))
		{
			Err() << "Failed to convert the image format in a format that Direct2D can use." << std::endl;
			return false;
		}

		ID2D1RenderTarget& renderTarget = Application::Instance->Graphics.GetRenderTarget();

		// 4. Call the CreateBitmapFromWicBitmap method to create an ID2D1Bitmap object
		//    that can be drawn by a render target and used with other Direct2D objects.
		success = renderTarget.CreateBitmapFromWicBitmap(pConverter.Get(), nullptr, &impl->Bitmap);
		if(FAILED(success))
		{
			Err() << "Failed to create an ID2D1Bitmap object" << std::endl;
			return false;
		}

		// store the size to retrieve it later
		const auto [width, height] = impl->Bitmap->GetSize();
		size.X = width;
		size.Y = height;

		return true;
	}

	////////////////////////////////////////////////////////////
	Texture Texture::Get(i32 x, i32 y, i32 width, i32 height) const
	{
		// get the bitmap to copy from
		ID2D1Bitmap* sourceBitmap = impl->Bitmap.Get();
		if(!sourceBitmap)
		{
			return {};
		}

		// the bitmap to write into
		Microsoft::WRL::ComPtr<ID2D1Bitmap> destinationBitmap = nullptr;

		// get the pixel format from the source bitmap
		const D2D1_PIXEL_FORMAT pixelFormat = sourceBitmap->GetPixelFormat();

		// get the dots per inch from the source bitmap
		FLOAT dpiX = 0.0f, dpiY = 0.0f;
		sourceBitmap->GetDpi(&dpiX, &dpiY);
		
		ID2D1RenderTarget& renderTarget = Application::Instance->Graphics.GetRenderTarget();

		// create the destination bitmap
		HRESULT success = renderTarget.CreateBitmap(
			D2D1::SizeU((u32)width, (u32)height),
			D2D1::BitmapProperties(pixelFormat, dpiX, dpiY),
			&destinationBitmap
		);

		if(FAILED(success))
		{
			Err() << "Failed to create the destination bitmap to write to." << std::endl;
			return {};
		}

		// the point on the destination bitmap to start writing to
		const D2D1_POINT_2U destinationPoint = D2D1::Point2U(0, 0);

		// the rectangle to copy from the source bitmap
		const D2D1_RECT_U sourceRectangle = D2D1::RectU(x, y, x + width, y + height);

		// Do the actual copy
		success = destinationBitmap->CopyFromBitmap(
			&destinationPoint, sourceBitmap, &sourceRectangle
		);

		if(FAILED(success))
		{
			Err() << "Failed to copy the source bitmap into the destination." << std::endl;
			return {};
		}

		const D2D1_SIZE_F bitmapSize = destinationBitmap->GetSize();

		// create the output texture
		Texture output;
		output.size = Float2(bitmapSize.width, bitmapSize.height);
		output.impl->Bitmap = std::move(destinationBitmap);
		return output;
	}

	////////////////////////////////////////////////////////////
	ID2D1Bitmap* Texture::GetBitmap() const
	{
		return impl->Bitmap.Get();
	}

	////////////////////////////////////////////////////////////
	const Float2& Texture::GetSize() const
	{
		return size;
	}
}