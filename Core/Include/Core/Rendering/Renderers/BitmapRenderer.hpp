#pragma once

/// <summary>
/// Core
/// </summary>
#include <Core/Rendering/Renderers/Renderer.hpp>

/// <summary>
/// Direct2D dependencies
/// </summary>
#include <d2d1.h>
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
		/// Returns the BitmapRenderTarget
		/// </summary>
		virtual ID2D1RenderTarget * GetRenderTarget() const override;

	public:

		/// <summary>
		/// The DC RenderTarget
		/// </summary>
		Microsoft::WRL::ComPtr<ID2D1DCRenderTarget> dcRenderTarget;

		/// <summary>
		/// The Bitmap RenderTarget.
		/// Use this for rendering
		/// </summary>
		Microsoft::WRL::ComPtr<ID2D1BitmapRenderTarget> bitmapRenderTarget;

		/// <summary>
		/// A DC which is
		/// bound to the
		/// BitmapRenderTarget
		/// </summary>
		HDC hdc;

	}; // class BitmapRenderer

} // namespace Core