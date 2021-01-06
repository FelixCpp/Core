#pragma once

#include <Core/Rendering/Image.hpp>

namespace Core
{

	class RenderStateManager;
	class GraphicsContext;

	class ImageTarget {
	public:

		/* assigns the member variable to the parameter */
		explicit ImageTarget(GraphicsContext *& gctx, RenderStateManager *& rsm);

		/* creates an image with the passed dimensions and color */
		Image createImage(u32_t width, u32_t height, const Color & color = Color::White);

		/* loads an image from memory. */
		Image loadImageFromMemory(u32_t width, u32_t height, const Color * colors);

		/* loads an image from a filepath */
		Image loadImageFromFile(const std::string & filepath);

		/* Draws an image at x, y with the images width and height as size */
		void image(const Image & image, float x, float y);

		/* Draws an image at x, y, with the specified width and height as size */
		void image(const Image & image, float x, float y, float width, float height);

	private:

		/* an instance of the GraphicsContext */
		GraphicsContext *& gctx;

		/* an instance of a RenderStateManager to get access to the current image-mode (DrawMode) */
		RenderStateManager *& rsm;

	};

}