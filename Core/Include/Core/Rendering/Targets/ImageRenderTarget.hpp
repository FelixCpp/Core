#pragma once

#include <Core/Rendering/Image.hpp>

#include <Core/Rendering/DrawMode.hpp>

namespace Core
{

	class RenderStateManager;
	class GraphicsContext;

	class ImageRenderTarget {
	public:

		/* assigns the member variable to the parameter */
		explicit ImageRenderTarget(GraphicsContext *& gctx, RenderStateManager *& rsm);

		/// <summary>
		/// virtual destructor
		/// </summary>
		virtual ~ImageRenderTarget() = default;

		/* creates an image with the passed dimensions and color */
		Image CreateImage(u32_t width, u32_t height, const Color & color = Color::White);

		/* loads an image from memory. */
		Image LoadImageFromMemory(u32_t width, u32_t height, const Color * colors);

		/* loads an image from a filepath */
		Image LoadImageFromFile(const std::string & filepath);

		/* loads an image from the rendertarget (screen) */
		Image LoadImageFromScreen(i32_t x, i32_t y, i32_t width, i32_t height);

		/// <summary>
		/// Changes the drawmode of
		/// images for the current RenderState
		/// </summary>
		/// <param name="mode">the new draw-mode</param>
		void ImageMode(DrawMode mode);

		/// <summary>
		/// Draws an image at x, y with the images width and height as size
		/// </summary>
		/// <param name="image">the image to draw</param>
		/// <param name="x">position x in pixel space</param>
		/// <param name="y">position y in pixel space</param>
		void Image(const Core::Image & image, float x, float y);

		/// <summary>
		/// Draws an image at x, y with the specified width and height as size
		/// </summary>
		/// <param name="image">the image to draw</param>
		/// <param name="x">position x in pixel space</param>
		/// <param name="y">position y in pixel space</param>
		void Image(const Core::Image & image, float x, float y, float width, float height);

	private:

		/* an instance of the GraphicsContext */
		GraphicsContext *& gctx;

		/* an instance of a RenderStateManager to get access to the current image-mode (DrawMode) */
		RenderStateManager *& rsm;

	};

}