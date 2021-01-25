#pragma once

/// <summary>
/// Core
/// </summary>
#include <Core/System/Datatypes.hpp>

/// <summary>
/// C++ / STL
/// </summary>
#include <string>

/// <summary>
/// Ignore this
/// </summary>
struct ID2D1RenderTarget;

namespace Core
{

	class Renderer {
	public:

		/// <summary>
		/// Default constructor
		/// </summary>
		Renderer() = default;
		
		/// <summary>
		/// Default destructor
		/// </summary>
		virtual ~Renderer() = default;

		/// <summary>
		/// Initializes the Renderer
		/// </summary>
		/// <param name="handle">the Window handle</param>
		virtual bool Initialize(Windowhandle handle) = 0;

		/// <summary>
		/// Destroys the Renderer
		/// </summary>
		virtual void Destroy() = 0;

		/// <summary>
		/// Tells the RenderTarget to start
		/// listening on render commands
		/// </summary>
		virtual void BeginDraw() = 0;

		/// <summary>
		/// Stops listening on
		/// Render commands
		/// </summary>
		virtual void EndDraw() = 0;

		/// <summary>
		/// Resizes the rendering
		/// area
		/// </summary>
		/// <param name="width">width of the area</param>
		/// <param name="height">height of the area</param>
		virtual void ResizeViewport(u32_t width, u32_t height) = 0;

		/// <summary>
		/// Saves the current screen
		/// of the Renderer to a file
		/// </summary>
		/// <param name="filepath">where the image gets stored to</param>
		virtual void SaveFrame(const std::string & filepath) = 0;

		/// <summary>
		/// Returns the RenderTarget
		/// as base class
		/// </summary>
		virtual ID2D1RenderTarget * GetRenderTarget() const = 0;

		/// <summary>
		/// Checks wether the Renderer
		/// is initialized or in its
		/// initial state
		/// </summary>
		virtual bool IsInitialized() const = 0;

	}; // class RendererBase

} // namespace Core