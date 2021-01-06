#include <Core/Rendering/GraphicsContext.hpp>
#include <Core/Rendering/RenderState.hpp>

#include <Core/System/Logger.hpp>

namespace Core
{

	GraphicsContext::GraphicsContext() :
		mainFactory(nullptr),
		imagingFactory(nullptr),
		writeFactory(nullptr),
		hwndRenderTarget(nullptr),
		gdiToken(0ull),
		drawing(false)
	{
	}

	bool GraphicsContext::initialize(Windowhandle handle)
	{
		HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, this->mainFactory.ReleaseAndGetAddressOf());
		if (FAILED(hr))
		{
			CORE_ERROR("D2D1CreateFactory");
			return false;
		}

		hr = CoInitialize(nullptr);
		if (FAILED(hr))
		{
			CORE_ERROR("CoInitialize failed");
			return false;
		}

		hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&this->imagingFactory)
		);
		if (FAILED(hr))
		{
			CORE_ERROR("failed to create an imaging factory");
			return false;
		}

		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &this->writeFactory);
		if (FAILED(hr))
		{
			CORE_ERROR("failed to create a writing factory");
			return false;
		}

		RECT rect = {};
		if (!GetClientRect(handle, &rect))
		{
			CORE_ERROR("failed to get the window size");
			return false;
		}

		const UINT width = rect.right - rect.left;
		const UINT height = rect.bottom - rect.top;

		hr = mainFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(handle, D2D1::SizeU(width, height), D2D1_PRESENT_OPTIONS_IMMEDIATELY),
			&this->hwndRenderTarget
		);
		if (FAILED(hr))
		{
			CORE_ERROR("failed to create a RenderTarget");
			return false;
		}

		const Gdiplus::Status status = Gdiplus::GdiplusStartup(&this->gdiToken, &this->gdiInput, nullptr);
		if (status != Gdiplus::Status::Ok)
		{
			CORE_ERROR("failed to initialize Gdiplus");
			return false;
		}

		return true;
	}

	void GraphicsContext::destroy()
	{
		Gdiplus::GdiplusShutdown(this->gdiToken);

		CoUninitialize();
		this->imagingFactory.Reset();
		this->hwndRenderTarget.Reset();
		this->mainFactory.Reset();
	}

	void GraphicsContext::beginDraw()
	{
		if (!this->drawing)
		{
			this->hwndRenderTarget->BeginDraw();
			this->drawing = true;
		}
	}

	void GraphicsContext::endDraw()
	{
		if (this->drawing)
		{
			this->hwndRenderTarget->EndDraw();
			this->drawing = false;
		}
	}

	void GraphicsContext::resizeViewport(u32_t width, u32_t height)
	{
		if (this->drawing)
		{
			if (ID2D1HwndRenderTarget * rt = this->hwndRenderTarget.Get())
			{
				rt->EndDraw();
				rt->Resize(D2D1::SizeU(width, height));
				rt->BeginDraw();
			}
		} else
		{
			if(ID2D1HwndRenderTarget * rt = this->hwndRenderTarget.Get())
				rt->Resize(D2D1::SizeU(width, height));
		}
	}
	
}