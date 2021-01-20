#include <Core/Rendering/GraphicsContext.hpp>
#include <Core/Rendering/RenderState.hpp>

#include <Core/Rendering/Renderers/RendererFactory.hpp>

#include <Core/System/Logger.hpp>

namespace Core
{

	GraphicsContext::GraphicsContext() :
		mainFactory(nullptr),
		imagingFactory(nullptr),
		writeFactory(nullptr),
		renderer(nullptr),
		renderTarget(nullptr),
		drawing(false)
	{
	}

	bool GraphicsContext::Initialize(Windowhandle handle, RendererType type)
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
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IWICImagingFactory,
			(LPVOID *)&this->imagingFactory
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

		// Initialize the Renderer
		this->renderer = RendererFactory::Create(type);
		if (!this->renderer)
		{
			CORE_ERROR("Invalid RendererType");
			return false;
		}

		// Initialize the RenderTarget
		if (!this->renderer->Initialize(this->mainFactory.Get(), handle))
		{
			return false;
		}

		this->renderTarget = this->renderer->GetRenderTarget();

		return true;
	}

	void GraphicsContext::Destroy()
	{
		// We need to destroy the ImagingFactory first!
		this->imagingFactory.Reset();
		CoUninitialize();
		
		this->writeFactory.Reset();
		this->mainFactory.Reset();

		delete this->renderer;
		this->renderer = nullptr;

		this->renderTarget = nullptr;
	}

	void GraphicsContext::BeginDraw()
	{
		this->renderer->BeginDraw();
	}

	void GraphicsContext::EndDraw()
	{
		this->renderer->EndDraw();
	}

	void GraphicsContext::ResizeViewport(u32_t width, u32_t height)
	{
		this->renderer->ResizeViewport(width, height);
		/*if (this->drawing)
		{
			if (ID2D1RenderTarget * rt = this->renderTarget.Get())
			{
				rt->EndDraw();
				rt->Resize(D2D1::SizeU(width, height));
				rt->BeginDraw();
			}
		} else
		{
			if(ID2D1HwndRenderTarget * rt = this->renderTarget.Get())
				rt->Resize(D2D1::SizeU(width, height));
		}*/
	}
	
}