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

		/// <summary>
		/// Default constructor
		/// </summary>
		GraphicsContext();

		/// <summary>
		/// default destructor
		/// </summary>
		~GraphicsContext() = default;

		/// <summary>
		/// initializes the factories + rendertarget
		/// </summary>
		/// <param name="handle">A handle used to create the RenderTarget</param>
		/// <returns>True if everythings works as expected</returns>
		bool Initialize(Windowhandle handle);

		/// <summary>
		/// Destroys the GraphicsContext
		/// </summary>
		void Destroy();

		/// <summary>
		/// Tells the RenderTarget to prepare
		/// listening on Render commands
		/// </summary>
		void BeginDraw();

		/// <summary>
		/// Tells the RenderTarget stop
		/// listening on Render commands
		/// </summary>
		void EndDraw();

		/// <summary>
		/// Resizes the viewport.
		/// The RenderTarget can't be resized
		/// between BeginDraw() and EndDraw()
		/// but handles that using the drawing
		/// attribute.
		/// </summary>
		/// <param name="width">the new width</param>
		/// <param name="height">the new height</param>
		void ResizeViewport(u32_t width, u32_t height);

	public:

		/// <summary>
		/// Direct2D factory
		/// used to create Direct2D content
		/// </summary>
		Microsoft::WRL::ComPtr<ID2D1Factory> mainFactory;

		/// <summary>
		/// Imaging factory.
		/// Used to load images from file
		/// </summary>
		Microsoft::WRL::ComPtr<IWICImagingFactory> imagingFactory;

		/// <summary>
		/// DirectWrite factory.
		/// Used to create text-objects to display
		/// </summary>
		Microsoft::WRL::ComPtr<IDWriteFactory> writeFactory;

		/// <summary>
		/// Direct2D RenderTarget
		/// Used to draw anything
		/// on screen
		/// </summary>
		Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> renderTarget;
		
		/// <summary>
		/// Keeps track of the drawing-state.
		/// We need this to restore the state
		/// after Resizing the viewport
		/// </summary>
		bool drawing;

	};

}