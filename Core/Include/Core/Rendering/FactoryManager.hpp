#pragma once

/// <summary>
/// Core
/// </summary>
#include <Core/System/StaticObject.hpp>

/// <summary>
/// Direct2D content
/// </summary>
#include <d2d1.h>
#pragma lib "d2d1"

/// <summary>
/// DirectWrite content
/// </summary>
#include <dwrite.h>
#pragma lib "dwrite"

/// <summary>
/// WIC (Windows Imaging Component)
/// </summary>
#include <wincodec.h>
#pragma lib "windowscodecs"

/// <summary>
/// Contains smart pointer
/// for Direct2D content
/// </summary>
#include <wrl/client.h>

namespace Core
{

	/// <summary>
	/// This class contains
	/// all needed factories
	/// and manage (initializes,
	/// destroys) them properly
	/// </summary>
	class FactoryManager : public StaticObject {
	public:

		/// <summary>
		/// Initializes the factories
		/// </summary>
		/// <returns>True if everything was initialized properly</returns>
		static bool Initialize();

		/// <summary>
		/// Destroys the factories
		/// </summary>
		static void Destroy();
		
		/// <summary>
		/// Returns true if the Initialize method was already
		/// called and returnes true
		/// </summary>
		static bool IsInitialized();

	public:

		/// <summary>
		/// Direct2D factory
		/// </summary>
		static Microsoft::WRL::ComPtr<ID2D1Factory> d2dFactory;

		/// <summary>
		/// DirectWrite factory
		/// </summary>
		static Microsoft::WRL::ComPtr<IDWriteFactory> dwriteFactory;

		/// <summary>
		/// WIC factory
		/// </summary>
		static Microsoft::WRL::ComPtr<IWICImagingFactory> wicFactory;

	private:

		/// <summary>
		/// A boolean to indicate
		/// wether the factories are
		/// initialized or not
		/// </summary>
		static bool isInitialized;

	}; // class FactoryManager

} // namespace Core