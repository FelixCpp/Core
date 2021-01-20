#pragma once

/// <summary>
/// Core
/// </summary>
#include <Core/Rendering/Renderers/Renderer.hpp>

/// <summary>
/// Direct2D dependencies
/// </summary>
#include <d2d1.h>

/// <summary>
/// WIC (Windows Imaging Component)
/// </summary>
#include <wincodec.h>

#include <wrl/client.h>

namespace Core
{
	
	class BitmapRenderer : public Renderer {
	public:

		/// <summary>
		/// Default constructor
		/// </summary>
		BitmapRenderer();
		
		/// <summary>
		/// Initializes the RenderTarget
		/// </summary>
		/// <param name="handle">the Window handle</param>
		virtual bool Initialize(Windowhandle handle) override;

		/// <summary>
		/// Destroys the RendererTargets
		/// </summary>
		virtual void Destroy() override;

		/// <summary>
		/// Tells the RenderTarget to start
		/// listening on render commands
		/// </summary>
		virtual void BeginDraw() override;

		/// <summary>
		/// Stops listening on
		/// Render commands, gets
		/// the bitmap from the BitmapRenderTarget
		/// and draws it to the screen
		/// </summary>
		virtual void EndDraw() override;

		/// <summary>
		/// Resizes the rendering
		/// area
		/// </summary>
		/// <param name="width">width of the area</param>
		/// <param name="height">height of the area</param>
		virtual void ResizeViewport(u32_t width, u32_t height) override;

		/// <summary>
		/// Saves the current screen
		/// of the Renderer to a file
		/// </summary>
		/// <param name="filepath">where the image gets stored to</param>
		virtual void SaveFrame(const std::string & filepath) override;

		/// <summary>
		/// Returns the BitmapRenderTarget
		/// </summary>
		virtual ID2D1RenderTarget * GetRenderTarget() const override;

	public:

		/// <summary>
		/// The Bitmap RenderTarget.
		/// Use this for rendering
		/// </summary>
		Microsoft::WRL::ComPtr<ID2D1RenderTarget> renderTarget;

		Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> hwndRenderTarget;

		/// <summary>
		/// the actual Windowhandle.
		/// We need this to release
		/// the DC properly
		/// </summary>
		Windowhandle windowHandle;

		/// <summary>
		/// The bitmap which gets updated everytime
		/// when EndDraw() is called.
		/// 
		/// This bitmap contains the data from
		/// every render command
		/// </summary>
		Microsoft::WRL::ComPtr<IWICBitmap> bitmap;

	}; // class BitmapRenderer

} // namespace Core