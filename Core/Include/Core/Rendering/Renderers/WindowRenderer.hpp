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

	class WindowRenderer : public Renderer {
	public:

		/// <summary>
		/// Default constructor
		/// </summary>
		WindowRenderer();

		/// <summary>
		/// Initializes the RenderTarget
		/// </summary>
		/// <param name="handle">the Window handle</param>
		virtual bool Initialize(Windowhandle handle) override;

		/// <summary>
		/// Destroys the RenderTarget
		/// </summary>
		virtual void Destroy() override;

		/// <summary>
		/// Tells the RenderTarget to start
		/// listening on render commands
		/// </summary>
		virtual void BeginDraw() override;

		/// <summary>
		/// Stops listening on
		/// Render commands and
		/// draws the content to
		/// the screen
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
		/// Returns the RenderTarget
		/// </summary>
		virtual ID2D1RenderTarget * GetRenderTarget() const override;

		/// <summary>
		/// Checks wether the RenderTarget
		/// is initialized or not
		/// </summary>
		virtual bool IsInitialized() const override;

	public:

		/// <summary>
		/// The actual RenderTarget
		/// </summary>
		Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> renderTarget;

		/// <summary>
		/// A boolean to keep
		/// track of wether
		/// the RenderTarget is
		/// currently drawing or not.
		/// 
		/// We need this to restore the
		/// drawing state after force
		/// the Resize method to execute
		/// </summary>
		bool drawing;

	}; // class BitmapRenderer

} // namespace Core