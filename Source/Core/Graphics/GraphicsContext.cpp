// 
// GraphicsContext.cpp
// Core
// 
// Created by Felix Busch on 27.03.2022.
// Copyright © 2022 Felix Busch. All rights reserved.
// 

#include <Core/Graphics/GraphicsContext.hpp>
#include <Core/Graphics/Factories.hpp>
#include <Core/Window/Window.hpp>
#include <Core/System/Error.hpp>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <wrl/client.h>
#include <d2d1.h>

namespace Core
{
	////////////////////////////////////////////////////////////
	/// \brief Define concrete implementation class
	///
	///	This class wraps the render target so it does not need
	///	to be declared in the header file.
	/// 
	////////////////////////////////////////////////////////////
	class GraphicsContext::Impl
	{
	public:
		
		Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> RenderTarget;	///< The actual render target used to render content on screen.

	};

	////////////////////////////////////////////////////////////
	GraphicsContext::GraphicsContext():
		impl(std::make_shared<Impl>()),
		sizeCache({ 0, 0, false })
	{
	}

	////////////////////////////////////////////////////////////
	bool GraphicsContext::Create(const Window& window)
	{
		const WindowHandle windowHandle = window.GetWindowHandle();
		if(windowHandle == nullptr)
		{
			Err() << "There is no window to render to. Make sure to create the window before creating a graphics context." << std::endl;
			return false;
		}

		ID2D1Factory* d2dFactory = Factories::D2DFactory.Get();
		if(d2dFactory == nullptr)
		{
			Err() << "There is no ID2D1Factory instance. Make sure to setup the factories before creating a graphics context." << std::endl;
			return false;
		}

		// Get the dpi
		const HDC windowDC = GetDC(windowHandle);
		const int dpiX = GetDeviceCaps(windowDC, LOGPIXELSX);
		const int dpiY = GetDeviceCaps(windowDC, LOGPIXELSY);
		ReleaseDC(windowHandle, windowDC);

		// get the window size
		const UInt2 windowSize = window.GetSize();

		const D2D1_RENDER_TARGET_PROPERTIES rtProperties = D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_HARDWARE,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
			(FLOAT)dpiX, (FLOAT)dpiY
		);

		const D2D1_HWND_RENDER_TARGET_PROPERTIES rtHwndProperties = D2D1::HwndRenderTargetProperties(
			windowHandle, D2D1::SizeU(windowSize.X, windowSize.Y), D2D1_PRESENT_OPTIONS_IMMEDIATELY
		);

		// create the render target
		const HRESULT success = d2dFactory->CreateHwndRenderTarget(rtProperties, rtHwndProperties, &impl->RenderTarget);
		if(FAILED(success))
		{
			Err() << "Failed to create an ID2D1HwndRenderTarget instance." << std::endl;
			return false;
		}

		return true;
	}

	////////////////////////////////////////////////////////////
	void GraphicsContext::Destroy()
	{
		impl->RenderTarget.Reset();
	}

	////////////////////////////////////////////////////////////
	bool GraphicsContext::BeginDraw()
	{
		ID2D1HwndRenderTarget* renderTarget = impl->RenderTarget.Get();

		if (sizeCache.NeedsResize)
		{
			const HRESULT success = renderTarget->Resize(D2D1::SizeU(sizeCache.ProjectionWidth, sizeCache.ProjectionHeight));

			if(FAILED(success))
			{
				Err() << "Failed to resize the render target" << std::endl;
				return false;
			}

			sizeCache.NeedsResize = false;
		}

		renderTarget->BeginDraw();
		return true;
	}

	////////////////////////////////////////////////////////////
	bool GraphicsContext::EndDraw()
	{
		const HRESULT success = impl->RenderTarget->EndDraw();

		if(FAILED(success) || success == D2DERR_RECREATE_TARGET)
		{
			Err() << "Failed to stop rendering." << std::endl;
			return false;
		}

		return true;
	}

	////////////////////////////////////////////////////////////
	void GraphicsContext::OnEvent(const WindowEvent& event)
	{
		// check for resize events
		if(event.Type == WindowEvent::Resized)
		{
			sizeCache.ProjectionWidth	= event.Size.Width;
			sizeCache.ProjectionHeight	= event.Size.Height;
			sizeCache.NeedsResize		= true;
		}
	}

	////////////////////////////////////////////////////////////
	ID2D1RenderTarget& GraphicsContext::GetRenderTarget()
	{
		return *impl->RenderTarget.Get();
	}
}