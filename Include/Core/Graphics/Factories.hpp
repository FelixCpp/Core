// 
// Factories.hpp
// Core
// 
// Created by Felix Busch on 27.03.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
//

#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <wrl/client.h>
#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>

namespace Core
{
	////////////////////////////////////////////////////////////
	/// \brief Container for Direct2D factories.
	///
	///	This class is intended to be used as a static
	///	accessor through the entire application.
	/// 
	////////////////////////////////////////////////////////////
	class Factories
	{
	public:

		////////////////////////////////////////////////////////////
		/// Member data
		/// 
		////////////////////////////////////////////////////////////
		static Microsoft::WRL::ComPtr<ID2D1Factory>			D2DFactory;		///< The main factory for Direct2D
		static Microsoft::WRL::ComPtr<IWICImagingFactory>	ImagingFactory;	///< The factory to use for images
		static Microsoft::WRL::ComPtr<IDWriteFactory>		DWriteFactory;	///< The factory to use for text processing

	public:

		////////////////////////////////////////////////////////////
		/// \brief Call this method to instantiate the factories
		///		   and initialize the COM-interface.
		///
		///	\return True if all factories have been initializes
		///			successfully, false otherwise.
		/// 
		////////////////////////////////////////////////////////////
		static bool Setup();

		////////////////////////////////////////////////////////////
		/// \brief Cleans up the factory handles as well as the
		///		   COM-interface.
		/// 
		////////////////////////////////////////////////////////////
		static void Cleanup();

	};
}