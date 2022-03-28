// 
// MouseCursor.cpp
// Core
// 
// Created by Felix Busch on 27.03.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#include <Core/Window/MouseCursor.hpp>
#include <Core/System/Error.hpp>
#include <Core/System/Types.hpp>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

//#ifndef STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_IMPLEMENTATION
//#endif
#include <stb/stb_image.h>

namespace Core
{
	////////////////////////////////////////////////////////////
	/// \brief Define concrete implementation class for
	///		   the mouse cursor
	/// 
	////////////////////////////////////////////////////////////
	class MouseCursor::Impl
	{
	public:

		////////////////////////////////////////////////////////////
		/// \brief Initializes an arrow cursor
		/// 
		////////////////////////////////////////////////////////////
		Impl():
			handle(nullptr),
			isSystemHandle(false)
		{
		}

		////////////////////////////////////////////////////////////
		/// \brief Destroy the cursor
		/// 
		////////////////////////////////////////////////////////////
		~Impl()
		{
			Release();
		}

		////////////////////////////////////////////////////////////
		/// \brief Copy-constructor
		/// 
		////////////////////////////////////////////////////////////
		Impl(const Impl& other):
			handle(CopyCursor(other.handle)),
			isSystemHandle(other.isSystemHandle)
		{
		}

		////////////////////////////////////////////////////////////
		/// \brief Copy-assignment
		/// 
		////////////////////////////////////////////////////////////
		Impl& operator=(const Impl& other)
		{
			Release();
			handle = CopyCursor(other.handle);
			isSystemHandle = other.isSystemHandle;
			return *this;
		}

		////////////////////////////////////////////////////////////
		/// \brief Loads the cursor asset from disk
		/// 
		////////////////////////////////////////////////////////////
		bool LoadFromFile(const std::string& filepath, const UInt2& hotspot)
		{
			// Load the pixel data
			int width = 0, height = 0, colorChannels = 0;
			stbi_uc* pixels = stbi_load(filepath.c_str(), &width, &height, &colorChannels, STBI_rgb_alpha);
			if(pixels == nullptr)
			{
				Err() << "Failed to load cursor from \"" << filepath << "\"" << std::endl;
				return false;
			}

			// free the old memory
			Release();

			// create the bitmap that will hold our color data
			BITMAPV5HEADER bitmapHeader;
			ZeroMemory(&bitmapHeader, sizeof(BITMAPV5HEADER));

			bitmapHeader.bV5Size		= sizeof(BITMAPV5HEADER);
			bitmapHeader.bV5Width		= static_cast<LONG>(width);
			bitmapHeader.bV5Height		= -static_cast<LONG>(height); // Negative indicates origin is in upper-left corner
			bitmapHeader.bV5Planes		= 1;
			bitmapHeader.bV5BitCount	= 32;
			bitmapHeader.bV5Compression = BI_BITFIELDS;
			bitmapHeader.bV5RedMask		= 0x00ff0000;
			bitmapHeader.bV5GreenMask	= 0x0000ff00;
			bitmapHeader.bV5BlueMask	= 0x000000ff;
			bitmapHeader.bV5AlphaMask	= 0xff000000;

			u32* bitmapData = nullptr;
			const HDC monitorDC = GetDC(nullptr);
			const HBITMAP color = CreateDIBSection(monitorDC, reinterpret_cast<const BITMAPINFO*>(&bitmapHeader), DIB_RGB_COLORS, reinterpret_cast<void**>(&bitmapData), nullptr, 0);
			ReleaseDC(nullptr, monitorDC);

			if(!color)
			{
				stbi_image_free(pixels);
				Err() << "Failed to create cursor color bitmap" << std::endl;
				return false;
			}

			// Fill our bitmap with the cursor color data
			// We'll have to swap the red and blue channels here
			u32* bitmapOffset = bitmapData;
			stbi_uc* pixel = pixels;
			for (std::size_t remaining = width * height; remaining > 0; --remaining, pixel += 4)
			{
				*bitmapOffset++ = static_cast<u32>((pixel[3] << 24) | (pixel[0] << 16) | (pixel[1] << 8) | pixel[2]);
			}
			
			stbi_image_free(pixels);

			// Create a dummy mask bitmap (it won't be used)
			const HBITMAP mask = CreateBitmap(width, height, 1, 1, nullptr);

			if (!mask)
			{
				DeleteObject(color);
				Err() << "Failed to create cursor mask bitmap" << std::endl;
				return false;
			}

			// create the structure that describes our cursor
			ICONINFO cursorInfo = {};
			ZeroMemory(&cursorInfo, sizeof(ICONINFO));
			cursorInfo.fIcon = FALSE; // This is a cursor and not an icon
			cursorInfo.xHotspot = hotspot.X;
			cursorInfo.yHotspot = hotspot.Y;
			cursorInfo.hbmColor = color;
			cursorInfo.hbmMask = mask;

			// create the cursor
			handle = CreateIconIndirect(&cursorInfo);

			// the data has been copied into the cursor, so get rid of these
			DeleteObject(color);
			DeleteObject(mask);

			if(handle)
			{
				isSystemHandle = false;
				return true;
			}

			Err() << "Failed to create cursor from bitmaps" << std::endl;
			return false;
		}

		////////////////////////////////////////////////////////////
		/// \brief Loads the cursor asset from system
		/// 
		////////////////////////////////////////////////////////////
		bool LoadFromSystem(System system)
		{
			Release();

			switch (system)
			{
				default:
				case Arrow:			handle = LoadCursor(nullptr, IDC_ARROW); break;
				case AppStarting:	handle = LoadCursor(nullptr, IDC_APPSTARTING); break;
				case Cross:			handle = LoadCursor(nullptr, IDC_CROSS); break;
				case Hand:			handle = LoadCursor(nullptr, IDC_HAND); break;
				case Help:			handle = LoadCursor(nullptr, IDC_HELP); break;
				case IBeam:			handle = LoadCursor(nullptr, IDC_IBEAM); break;
				case Icon:			handle = LoadCursor(nullptr, IDC_ICON); break;
				case No:			handle = LoadCursor(nullptr, IDC_NO); break;
				case Size:			handle = LoadCursor(nullptr, IDC_SIZE); break;
				case SizeAll:		handle = LoadCursor(nullptr, IDC_SIZEALL); break;
				case SizeNESW:		handle = LoadCursor(nullptr, IDC_SIZENESW); break;
				case SizeNS:		handle = LoadCursor(nullptr, IDC_SIZENS); break;
				case SizeNWSE:		handle = LoadCursor(nullptr, IDC_SIZENWSE); break;
				case SizeWE:		handle = LoadCursor(nullptr, IDC_SIZEWE); break;
				case UpArrow:		handle = LoadCursor(nullptr, IDC_UPARROW); break;
				case Wait:			handle = LoadCursor(nullptr, IDC_WAIT); break;
			}

			if(handle == nullptr)
			{
				Err() << "Failed to load cursor asset from the system" << std::endl;
				return isSystemHandle = false;
			}

			return isSystemHandle = true;
		}

		////////////////////////////////////////////////////////////
		/// \brief Get the underlying cursor handle
		/// 
		////////////////////////////////////////////////////////////
		CursorHandle GetCursorHandle() const
		{
			return handle;
		}

	private:

		////////////////////////////////////////////////////////////
		/// \brief Releases the cursor handle if it's needed
		/// 
		////////////////////////////////////////////////////////////
		void Release()
		{
			if (!isSystemHandle && handle)
			{
				DestroyCursor(handle);
				handle = nullptr;
			}
		}

		////////////////////////////////////////////////////////////
		/// Member data
		/// 
		////////////////////////////////////////////////////////////
		CursorHandle	handle;				///< The actual handle value
		bool			isSystemHandle;		///< Keep track of whether the cursor is loaded from disk or not

	};

	////////////////////////////////////////////////////////////
	MouseCursor::MouseCursor():
		impl(std::make_unique<Impl>())
	{
		LoadFromSystem(Arrow);
	}
	
	////////////////////////////////////////////////////////////
	bool MouseCursor::LoadFromFile(const std::string& filepath, const UInt2& hotspot)
	{
		return impl->LoadFromFile(filepath, hotspot);
	}

	////////////////////////////////////////////////////////////
	bool MouseCursor::LoadFromSystem(System system)
	{
		return impl->LoadFromSystem(system);
	}

	////////////////////////////////////////////////////////////
	CursorHandle MouseCursor::GetCursorHandle() const
	{
		return impl->GetCursorHandle();
	}
}
