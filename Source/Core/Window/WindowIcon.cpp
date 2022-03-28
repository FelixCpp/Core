// 
// WindowIcon.cpp
// Core
// 
// Created by Felix Busch on 28.03.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#include <Core/Window/WindowIcon.hpp>
#include <Core/System/Error.hpp>
#include <Core/System/Types.hpp>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

//#ifndef STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_IMPLEMENTATION
//#endif
////
#ifndef STBI_WINDOWS_UTF8
#define STBI_WINDOWS_UTF8
#endif

#include <stb/stb_image.h>
#include <vector>

namespace Core
{
	////////////////////////////////////////////////////////////
	/// \brief Define concrete implementation class for the
	///		   WindowIcon
	/// 
	////////////////////////////////////////////////////////////
	class WindowIcon::Impl
	{
	public:

		////////////////////////////////////////////////////////////
		/// \brief Default constructor
		/// 
		////////////////////////////////////////////////////////////
		Impl():
			handle(nullptr),
			isSystemHandle(false)
		{
		}

		////////////////////////////////////////////////////////////
		/// \brief Default destructor
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
			handle(CopyIcon(other.handle)),
			isSystemHandle(other.isSystemHandle)
		{}

		////////////////////////////////////////////////////////////
		/// \brief Copy-assignment
		/// 
		////////////////////////////////////////////////////////////
		Impl& operator=(const Impl& other)
		{
			Release();
			handle = CopyIcon(other.handle);
			isSystemHandle = other.isSystemHandle;
			return *this;
		}

		////////////////////////////////////////////////////////////
		/// \brief Load the icon from a file.
		///
		///	\param filepath The file location on disk.
		///
		///	\return True if the icon has been loaded successfully,
		///			false otherwise.
		/// 
		////////////////////////////////////////////////////////////
		bool LoadFromFile(const String& filepath)
		{
			// convert wstring to cstring
			std::string cFilepath;
			cFilepath.resize(filepath.length());
			stbi_convert_wchar_to_utf8(&cFilepath[0], cFilepath.length(), filepath.data());

			Release();

			// load the pixel data
			int width = 0, height = 0, colorChannels = 0;
			stbi_uc* pixels = stbi_load(cFilepath.c_str(), &width, &height, &colorChannels, STBI_rgb_alpha);

			// validate success
			if(pixels == nullptr)
			{
				Err() << "Failed to load the pixel data from \"" << cFilepath << "\"" << std::endl;
				return false;
			}

			// Windows want B-G-R-A pixels: swap red and blue channels
			std::vector<u8> iconPixels((usize)width * (usize)height * 4);
			for(usize i = 0; i < iconPixels.size() / 4; ++i)
			{
				iconPixels[i * 4 + 0] = pixels[i * 4 + 2]; // blue
				iconPixels[i * 4 + 1] = pixels[i * 4 + 1]; // green
				iconPixels[i * 4 + 2] = pixels[i * 4 + 0]; // red
				iconPixels[i * 4 + 3] = pixels[i * 4 + 3]; // alpha
			}

			// free the pixel memory
			stbi_image_free(pixels);

			// create the icon from the pixel array
			handle = CreateIcon(GetModuleHandle(nullptr), width, height, 1, 32, nullptr, iconPixels.data());
			if(handle == nullptr)
			{
				Err() << "Failed to create the icon" << std::endl;
				return false;
			}
			
			return isSystemHandle = true;
		}

		////////////////////////////////////////////////////////////
		/// \brief Loads the icon from pre-defined system assets.
		///
		///	\param system The icon identification.
		///
		///	\return True if the icon has been loaded successfully,
		///			false otherwise.
		/// 
		////////////////////////////////////////////////////////////
		bool LoadFromSystem(System system)
		{
			Release();

			switch (system)
			{
				default:
				case Application: handle = LoadIcon(nullptr, IDI_APPLICATION); break;
				case Asterisk: handle = LoadIcon(nullptr, IDI_ASTERISK); break;
				case Error:	handle = LoadIcon(nullptr, IDI_ERROR); break;
				case Exclamation: handle = LoadIcon(nullptr, IDI_EXCLAMATION); break;
				case Hand: handle = LoadIcon(nullptr, IDI_HAND); break;
				case Information: handle = LoadIcon(nullptr, IDI_INFORMATION); break;
				case Question: handle = LoadIcon(nullptr, IDI_QUESTION); break;
				case Shield: handle = LoadIcon(nullptr, IDI_SHIELD); break;
				case Warning: handle = LoadIcon(nullptr, IDI_WARNING); break;
				case WinLogo: handle = LoadIcon(nullptr, IDI_WINLOGO); break;
			}

			if (handle == nullptr)
			{
				Err() << "Failed to load icon asset from the system" << std::endl;
				return isSystemHandle = false;
			}

			return isSystemHandle = true;
		}

		////////////////////////////////////////////////////////////
		/// \brief Get the icon handle.
		/// 
		////////////////////////////////////////////////////////////
		IconHandle GetIconHandle() const
		{
			return handle;
		}

	private:

		////////////////////////////////////////////////////////////
		/// \brief Releases the handle if it's needed
		/// 
		////////////////////////////////////////////////////////////
		void Release()
		{
			if(!isSystemHandle && handle)
			{
				DestroyIcon(handle);
				handle = nullptr;
			}
		}

		////////////////////////////////////////////////////////////
		/// Member data
		/// 
		////////////////////////////////////////////////////////////
		IconHandle	handle;			///< The icon handle to assign to the window.
		bool		isSystemHandle;	///< Whether the icon is loaded from the system or not.

	};

	////////////////////////////////////////////////////////////
	WindowIcon::WindowIcon():
		impl(std::make_shared<Impl>())
	{
		LoadFromSystem(Application);
	}

	////////////////////////////////////////////////////////////
	bool WindowIcon::LoadFromFile(const String& filepath)
	{
		return impl->LoadFromFile(filepath);
	}

	////////////////////////////////////////////////////////////
	bool WindowIcon::LoadFromSystem(System system)
	{
		return impl->LoadFromSystem(system);
	}

	////////////////////////////////////////////////////////////
	IconHandle WindowIcon::GetIconHandle() const
	{
		return impl->GetIconHandle();
	}
}
