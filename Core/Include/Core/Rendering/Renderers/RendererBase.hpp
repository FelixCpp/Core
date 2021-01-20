#pragma once

/// <summary>
/// Core
/// </summary>
#include <Core/System/Datatypes.hpp>

struct ID2D1RenderTarget;
struct ID2D1Factory;

namespace Core
{

	class RendererBase {
	public:

		/// <summary>
		/// Default constructor
		/// </summary>
		RendererBase() = default;
		
		/// <summary>
		/// Default destructor
		/// </summary>
		virtual ~RendererBase() = default;

		/// <summary>
		/// Initializes the RenderTarget
		/// </summary>
		/// <param name="factory">the graphics factory</param>
		/// <param name="handle">the Window handle</param>
		virtual bool Initialize(ID2D1Factory * factory, Windowhandle handle) = 0;

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
		/// Returns the RenderTarget
		/// as base class
		/// </summary>
		virtual ID2D1RenderTarget * GetRenderTarget() const = 0;

	}; // class RendererBase

} // namespace Core