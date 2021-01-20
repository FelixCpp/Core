#include <Core/Rendering/FactoryManager.hpp>
#include <Core/System/Logger.hpp>

namespace Core
{

	Microsoft::WRL::ComPtr<ID2D1Factory> FactoryManager::d2dFactory = nullptr;
	Microsoft::WRL::ComPtr<IDWriteFactory> FactoryManager::dwriteFactory = nullptr;
	Microsoft::WRL::ComPtr<IWICImagingFactory> FactoryManager::wicFactory = nullptr;
	bool FactoryManager::isInitialized = false;

	bool FactoryManager::Initialize()
	{
		if (isInitialized)
			return true;

		// Create Direct2D factory
		HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, d2dFactory.GetAddressOf());
		
		// error handling
		if (FAILED(hr))
		{
			CORE_ERROR("Failed to create a Direct2D Factory");
			return false;
		}

		hr = CoInitialize(nullptr);

		// error handling
		if (FAILED(hr))
		{
			CORE_ERROR("CoInitialize failed");
			return false;
		}

		// initializes WIC factory
		hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IWICImagingFactory,
			(LPVOID *)&wicFactory
		);
		if (FAILED(hr))
		{
			CORE_ERROR("failed to create an imaging factory");
			return false;
		}

		// initializes DirectWrite factory
		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &dwriteFactory);
		if (FAILED(hr))
		{
			CORE_ERROR("failed to create a writing factory");
			return false;
		}

		isInitialized = true;
		return true;
	}

	void FactoryManager::Destroy()
	{
		// The order of these operations is important!
		wicFactory.Reset();
		CoUninitialize();

		d2dFactory.Reset();
		dwriteFactory.Reset();

		isInitialized = false;
	}

	bool FactoryManager::IsInitialized()
	{
		return isInitialized;
	}

} // namespace Core