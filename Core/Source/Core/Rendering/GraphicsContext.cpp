#include <Core/Rendering/GraphicsContext.hpp>
#include <Core/Rendering/RenderState.hpp>

#include <Core/System/Logger.hpp>

namespace Core
{

	GraphicsContext::GraphicsContext(Windowhandle handle) :
		mainFactory(nullptr),
		imagingFactory(nullptr),
		writeFactory(nullptr),
		hwndRenderTarget(nullptr),
		states(),
		gdiToken(0ull),
		drawing(false)
	{
		HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, this->mainFactory.ReleaseAndGetAddressOf());
		if (FAILED(hr))
		{
			CORE_ERROR("D2D1CreateFactory");
			return;
		}

		hr = CoInitialize(nullptr);
		if (FAILED(hr))
		{
			CORE_ERROR("CoInitialize");
			return;
		}

		hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&this->imagingFactory)
		);
		if (FAILED(hr))
		{
			CORE_ERROR("CoCreateInstance");
			return;
		}

		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &this->writeFactory);
		if (FAILED(hr))
		{
			CORE_ERROR("DWriteCreateFactory");
			return;
		}

		RECT rect = {};
		if (!GetClientRect(handle, &rect))
		{
			CORE_ERROR("GetClientRect");
			return;
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
			CORE_ERROR("CreateHwndRenderTarget");
			return;
		}

		const Gdiplus::Status status = Gdiplus::GdiplusStartup(&this->gdiToken, &this->gdiInput, nullptr);
		if (status != Gdiplus::Status::Ok)
		{
			CORE_ERROR("GdiplusStartup");
			return;
		}
	}

	GraphicsContext::~GraphicsContext()
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
	
	RenderState & GraphicsContext::getActiveRenderState()
	{
		return this->states.top();
	}
	
}