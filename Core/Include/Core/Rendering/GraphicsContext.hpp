#pragma once

#include <Core/System/Datatypes.hpp>
#include <Core/Maths/Vector2.hpp>

#include <d2d1.h>
#pragma comment(lib, "d2d1")

#include <dwrite.h>
#pragma comment(lib, "dwrite")

#include <wincodec.h>
#pragma comment(lib, "windowscodecs")

#include <wrl/client.h>

#include <memory>

namespace Core
{

	class GraphicsContext {
	public:

		/* default constructor */
		GraphicsContext();

		/* destructor */
		~GraphicsContext() = default;

		/* initializes the Factories & RenderTarget(s) */
		bool Initialize(Windowhandle handle);

		/* destroys the Factories & RenderTarget(s) */
		void Destroy();

		/* tells the RenderTarget to start listening on rendercommands */
		void BeginDraw();

		/* tells the RenderTarget to stop listening on rendercommands */
		void EndDraw();

		/* resizes the viewport of the RenderTarget */
		void ResizeViewport(u32_t width, u32_t height);

	public:

		/* Direct2D Factory (used to create Direct2D content) */
		Microsoft::WRL::ComPtr<ID2D1Factory> mainFactory;

		/* Imaging Factory (used to load images from files) */
		Microsoft::WRL::ComPtr<IWICImagingFactory> imagingFactory;

		/* DirectWrite Factory (used to create text-objects to display) */
		Microsoft::WRL::ComPtr<IDWriteFactory> writeFactory;

		/* Direct2D WindowRenderTarget (used to draw anything on screen) */
		Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> hwndRenderTarget;
		
		/* we need to keep track of the drawing-state when we're resizing */
		bool drawing;

	};

}