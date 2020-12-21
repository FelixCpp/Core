#pragma once

#include <Core/System/Datatypes.hpp>
#include <Core/Maths/Vector2.hpp>

#include <Core/Application/RenderState.hpp>

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
#include <stack>

namespace Core
{

	class GraphicsContext {
	public:

		explicit GraphicsContext(Windowhandle handle);
		~GraphicsContext();

		void beginDraw();
		void endDraw();

		void resizeViewport(u32_t width, u32_t height);

		RenderState & getActiveRenderState();
		
	public:

		Microsoft::WRL::ComPtr<ID2D1Factory> mainFactory;
		Microsoft::WRL::ComPtr<IWICImagingFactory> imagingFactory;
		Microsoft::WRL::ComPtr<IDWriteFactory> writeFactory;
		Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> hwndRenderTarget;
		
		std::stack<RenderState> states;

		Gdiplus::GdiplusStartupInput gdiInput;
		ULONG_PTR gdiToken;

		bool drawing;

	};

}