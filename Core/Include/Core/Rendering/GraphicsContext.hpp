#pragma once

#include <Core/System/Datatypes.hpp>
#include <Core/Maths/Vector2.hpp>

#include <d2d1.h>
#pragma comment(lib, "d2d1")

#include <dwrite.h>
#pragma comment(lib, "dwrite")

#include <gdiplus.h>
#pragma comment(lib, "gdi32")
#pragma comment(lib, "gdiplus")

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
		bool initialize(Windowhandle handle);

		/* destroys the Factories & RenderTarget(s) */
		void destroy();

		/* tells the RenderTarget to start listening on rendercommands */
		void beginDraw();

		/* tells the RenderTarget to stop listening on rendercommands */
		void endDraw();

		/* resizes the viewport of the RenderTarget */
		void resizeViewport(u32_t width, u32_t height);

	public:

		/* Direct2D Factory (used to create Direct2D content) */
		Microsoft::WRL::ComPtr<ID2D1Factory> mainFactory;

		/* Imaging Factory (used to load images from files) */
		Microsoft::WRL::ComPtr<IWICImagingFactory> imagingFactory;

		/* DirectWrite Factory (used to create text-objects to display) */
		Microsoft::WRL::ComPtr<IDWriteFactory> writeFactory;

		/* Direct2D WindowRenderTarget (used to draw anything on screen) */
		Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> hwndRenderTarget;
		
		/* Currently unused */
		Gdiplus::GdiplusStartupInput gdiInput;
		ULONG_PTR gdiToken;

		/* we need to keep track of the drawing-state when we're resizing */
		bool drawing;

	};

}