#include <Core/Rendering/Renderers/WindowRenderer.hpp> // Core::WindowRenderer
#include <Core/Rendering/FactoryManager.hpp> // Core::FactoryManager
#include <Core/System/Logger.hpp> // CORE_ERROR

#include <vector>
#include <algorithm>

namespace Core
{

	WindowRenderer::WindowRenderer() :
		renderTarget(nullptr),
		drawing(false)
	{
	}

	bool WindowRenderer::Initialize(Windowhandle handle)
	{
		// get the size of the window
		RECT wndRect = {};
		if (!GetClientRect(handle, &wndRect))
		{
			CORE_ERROR("Failed to get the clients rect");
			return false;
		}

		// create a D2D1_SIZE_U object
		const D2D1_SIZE_U pixelSize = D2D1::SizeU(
			wndRect.right - wndRect.left,
			wndRect.bottom - wndRect.top
		);

		// Create a PixelFormat object
		const D2D1_PIXEL_FORMAT pixelFormat = D2D1::PixelFormat(
			DXGI_FORMAT_B8G8R8A8_UNORM,
			D2D1_ALPHA_MODE_PREMULTIPLIED
		);

		// Create the RenderTarget properties
		const D2D1_RENDER_TARGET_PROPERTIES properties = D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT, pixelFormat, 0.f, 0.f, D2D1_RENDER_TARGET_USAGE_NONE, D2D1_FEATURE_LEVEL_DEFAULT
		);

		// Create a HwndRenderTarget
		const HRESULT hr = FactoryManager::d2dFactory->CreateHwndRenderTarget(
			properties,
			D2D1::HwndRenderTargetProperties(handle, pixelSize, D2D1_PRESENT_OPTIONS_IMMEDIATELY),
			&this->renderTarget
		);

		// error handling
		if (FAILED(hr))
		{
			CORE_ERROR("Failed to create a HwndRenderTarget");
			return false;
		}

		return true;
	}

	void WindowRenderer::Destroy()
	{
		this->renderTarget.Reset();
		this->drawing = false;
	}

	void WindowRenderer::BeginDraw()
	{
		this->renderTarget->BeginDraw();
		this->drawing = true;
	}

	void WindowRenderer::EndDraw()
	{
		this->renderTarget->EndDraw();
		this->drawing = false;
	}

	void WindowRenderer::ResizeViewport(UInt32 width, UInt32 height)
	{
		if (this->drawing)
		{
			this->renderTarget->EndDraw();
			this->renderTarget->Resize(D2D1::SizeU(width, height));
			this->renderTarget->BeginDraw();
		} else
		{
			this->renderTarget->Resize(D2D1::SizeU(width, height));
		}
	}

	ID2D1RenderTarget * WindowRenderer::GetRenderTarget() const
	{
		return this->renderTarget.Get();
	}

	Windowhandle WindowRenderer::GetWindowhandle() const
	{
		return this->renderTarget->GetHwnd();
	}

	bool WindowRenderer::IsInitialized() const
	{
		return this->renderTarget != nullptr;
	}

	bool WindowRenderer::IsDrawing() const
	{
		return this->drawing;
	}

} // namespace Core