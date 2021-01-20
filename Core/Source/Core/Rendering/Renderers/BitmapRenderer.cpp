#include <Core/Rendering/Renderers/BitmapRenderer.hpp> // Core::BitmapRenderer
#include <Core/Rendering/FactoryManager.hpp>
#include <Core/System/Logger.hpp> // CORE_ERROR

#include <shlwapi.h>
#pragma comment(lib, "Shlwapi")

namespace Core
{

	BitmapRenderer::BitmapRenderer() :
		renderTarget(nullptr),
		windowHandle(nullptr),
        bitmap(nullptr)
	{
	}

	bool BitmapRenderer::Initialize(Windowhandle handle)
	{
		// Create a PixelFormat object
		const D2D1_PIXEL_FORMAT pixelFormat = D2D1::PixelFormat(
			DXGI_FORMAT_B8G8R8A8_UNORM,
			D2D1_ALPHA_MODE_PREMULTIPLIED
		);

		// Create the RenderTarget properties
		const D2D1_RENDER_TARGET_PROPERTIES properties = D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT, pixelFormat, 0.f, 0.f, D2D1_RENDER_TARGET_USAGE_NONE, D2D1_FEATURE_LEVEL_DEFAULT
		);

		// Create a bitmap RenderTarget
		RECT rect = {};
		if (!GetClientRect(handle, &rect))
		{
			CORE_ERROR("Failed to get the windows size");
			return false;
		}

		const UINT width = (UINT)(rect.right - rect.left);
		const UINT height = (UINT)(rect.bottom - rect.top);

		HRESULT hr = FactoryManager::wicFactory->CreateBitmap(width, height, GUID_WICPixelFormat32bppPBGRA, WICBitmapCacheOnLoad, &this->bitmap);
		
		// error handling
		if (FAILED(hr))
		{
			CORE_ERROR("Failed to create a IWICBitmap");
			return false;
		}

		hr = FactoryManager::d2dFactory->CreateWicBitmapRenderTarget(
			this->bitmap.Get(),
			properties,
			&this->renderTarget
		);

		if (FAILED(hr))
		{
			CORE_ERROR("Failed to create a WicBitmapRenderTarget");
			return false;
		}

		hr = FactoryManager::d2dFactory->CreateHwndRenderTarget(
			properties,
			D2D1::HwndRenderTargetProperties(handle, D2D1::SizeU(width, height), D2D1_PRESENT_OPTIONS_IMMEDIATELY),
			&this->hwndRenderTarget
		);

		return true;
	}

	void BitmapRenderer::Destroy()
	{
		this->renderTarget.Reset();
		this->bitmap.Reset();
	}

	void BitmapRenderer::BeginDraw()
	{
		this->renderTarget->BeginDraw();
	}

	void BitmapRenderer::EndDraw()
	{
		this->renderTarget->EndDraw();

		Microsoft::WRL::ComPtr<ID2D1Bitmap> d2dBitmap = nullptr;
		this->hwndRenderTarget->CreateBitmapFromWicBitmap(this->bitmap.Get(), &d2dBitmap);
		
		this->hwndRenderTarget->BeginDraw();
		this->hwndRenderTarget->DrawBitmap(d2dBitmap.Get());
		this->hwndRenderTarget->EndDraw();
	}

	void BitmapRenderer::ResizeViewport(u32_t width, u32_t height)
	{
		
	}

	void BitmapRenderer::SaveFrame(const std::string & filepath)
	{
		// -- Steps --
		
		// 5) Save to a file
		Microsoft::WRL::ComPtr<IStream> file;
		std::wstring wFilepath(filepath.begin(), filepath.end());
		HRESULT hr = SHCreateStreamOnFileEx(
			wFilepath.c_str(),
			STGM_CREATE | STGM_WRITE | STGM_SHARE_EXCLUSIVE,
			FILE_ATTRIBUTE_NORMAL,
			TRUE, // create
			nullptr, // template
			&file
		);

		Microsoft::WRL::ComPtr<IWICBitmapEncoder> encoder = nullptr;

		hr = FactoryManager::wicFactory->CreateEncoder(
			GUID_ContainerFormatPng,
			nullptr, // vendor
			encoder.GetAddressOf()
		);

		hr = encoder->Initialize(file.Get(), WICBitmapEncoderNoCache);

		Microsoft::WRL::ComPtr<IWICBitmapFrameEncode> frame = nullptr;
		Microsoft::WRL::ComPtr<IPropertyBag2> bag = nullptr;

		hr = encoder->CreateNewFrame(&frame, &bag);
		hr = frame->Initialize(bag.Get());

		UINT width = 0, height = 0;
		this->bitmap->GetSize(&width, &height);
		hr = frame->SetSize(width, height);

		GUID format;
		hr = this->bitmap->GetPixelFormat(&format);

		GUID negotiated = format;
		hr = frame->SetPixelFormat(&negotiated);
		hr = frame->WriteSource(this->bitmap.Get(), nullptr);
		hr = frame->Commit();
		hr = encoder->Commit();
    }

	ID2D1RenderTarget * BitmapRenderer::GetRenderTarget() const
	{
		return this->renderTarget.Get();
	}

} // namespace Core