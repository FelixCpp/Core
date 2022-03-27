// 
// Factories.cpp
// Core
// 
// Created by Felix Busch on 27.03.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
//

#include <Core/Graphics/Factories.hpp>
#include <Core/System/Error.hpp>

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")
#pragma comment(lib, "windowscodecs")

namespace Core
{
	////////////////////////////////////////////////////////////
	/// Linking static members
	/// 
	////////////////////////////////////////////////////////////
	Microsoft::WRL::ComPtr<ID2D1Factory>		Factories::D2DFactory;
	Microsoft::WRL::ComPtr<IWICImagingFactory>	Factories::ImagingFactory;
	Microsoft::WRL::ComPtr<IDWriteFactory>		Factories::DWriteFactory;

	////////////////////////////////////////////////////////////
	bool Factories::Setup()
	{
		// initialize COM-interface
		HRESULT success = CoInitialize(nullptr);

		if(FAILED(success))
		{
			Err() << "Failed to initialize the COM-interface." << std::endl;
			return false;
		}

		// initialize Direct2D factory
		const D2D1_FACTORY_OPTIONS options =
		{
#ifdef _DEBUG
			.debugLevel = D2D1_DEBUG_LEVEL_ERROR
#else
			.debugLevel = D2D1_DEBUG_LEVEL_NONE
#endif
		};

		success = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, __uuidof(ID2D1Factory), &options, &D2DFactory);
		if(FAILED(success))
		{
			Err() << "Failed to create an ID2D1Factory instance." << std::endl;
			return false;
		}

		// initialize imaging factory
		success = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&ImagingFactory));
		if (FAILED(success))
		{
			Err() << "Failed to create an Windows-Imaging-Component Factory instance." << std::endl;
			return false;
		}

		// initialize write factory
		success = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &DWriteFactory);
		if (FAILED(success))
		{
			Err() << "Failed to create an IDWriteFactory instance." << std::endl;
			return false;
		}

		return true;
	}

	////////////////////////////////////////////////////////////
	void Factories::Cleanup()
	{
		// The order matters!
		ImagingFactory.Reset();
		CoUninitialize();

		DWriteFactory.Reset();
		D2DFactory.Reset();
	}
}