// 
// Dpi.cpp
// Core
// 
// Created by Felix Busch on 18.02.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#include <Core/Window/Dpi.hpp>
#include <Core/System/Error.hpp>

#define NOMINMAX
#define WIN32_MEAN_AND_LEAN
#include <Windows.h>

namespace Core
{
	////////////////////////////////////////////////////////////
	bool Dpi::ConfigureDpi()
	{
		// try user32.dll & shcore.dll
		if(!(TryUser32() || TryShCore()))
		{
			// try win32 api as fallback
			if(!SetProcessDPIAware())
			{
				Err() << "Failed to configure the Dpi scale for this system";
				return false;
			}
		}

		return true;
	}

	////////////////////////////////////////////////////////////
	bool Dpi::TryUser32()
	{
		// load user32 dll
		if(const HMODULE user32 = LoadLibraryW(L"user32"))
		{
			using SetProcessDPIAwareFuncType = BOOL(WINAPI *)(DPI_AWARENESS_CONTEXT);
			if(const auto setThreadDpiAwarenessContext = reinterpret_cast<SetProcessDPIAwareFuncType>(GetProcAddress(user32, "SetProcessDpiAwarenessContext")))
			{
				if(setThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE))
				{
					return true;
				}
			}
		}

		return false;
	}

	////////////////////////////////////////////////////////////
	bool Dpi::TryShCore()
	{
		if(const HMODULE shCore = LoadLibraryW(L"ShCore.dll"))
		{
			enum class ProcessDpiAwareness
			{
				// DPI unaware. This app does not scale for DPI changes and is always assumed to have a scale factor of 100% (96 DPI).
				// It will be automatically scaled by the system on any other DPI setting.
				Unaware = 0,

				// System DPI aware. This app does not scale for DPI changes. It will query for the DPI once and use that value for the lifetime of the app.
				// If the DPI changes, the app will not adjust to the new DPI value. It will be automatically scaled up or down by the system when the DPI
				// changes from the system value.
				System = 1,

				// Per monitor DPI aware. This app checks for the DPI when it is created and adjusts the scale factor whenever the DPI changes. These
				// applications are not automatically scaled by the system.
				PerMonitor = 2
			};

			using SetProcessDpiAwarenessFuncType = HRESULT(WINAPI *)(ProcessDpiAwareness);
			if (const auto setProcessDpiAwareness = reinterpret_cast<SetProcessDpiAwarenessFuncType>(GetProcAddress(shCore, "SetProcessDpiAwareness")))
			{
				// SetProcessDpiAwareness returns either S_OK, E_INVALIDARG or E_ACCESSDENIED
				// we don't really care about E_ACCESSDENIED since that's not an issue in our scenario.
				// S_OK				=> The DPI awareness for the app was set successfully.
				// E_INVALIDARG		=> The value passed in is not valid.
				// E_ACCESSDENIED	=> The DPI awareness is already set, either by calling this API previously or through the application (.exe) manifest.

				if (setProcessDpiAwareness(ProcessDpiAwareness::System) != E_INVALIDARG)
				{
					return true;
				}
			}
		}

		return false;
	}
}