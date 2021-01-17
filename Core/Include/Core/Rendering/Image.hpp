#pragma once

#include <Core/System/Datatypes.hpp>

#include <Core/Rendering/Color.hpp>
#include <Core/Rendering/ImageInterpolationMode.hpp>

#include <string>
#include <memory>

struct ID2D1Bitmap;

namespace Core
{

	class GraphicsContext;

	class Image {
	public:

		/* default constructor */
		Image();

		/* creates an image with the given dimensions */
		bool Create(u32_t width, u32_t height, const Color & color, GraphicsContext * gctx);

		/* loads the image from the pixel data provided by the 'colors' parameter */
		bool LoadFromMemory(u32_t width, u32_t height, const Color * colors, GraphicsContext * gctx);

		/* loads the image from a file */
		bool LoadFromFile(const std::string & filepath, GraphicsContext * gctx);

		/* loads an image from the screen */
		bool LoadFromScreen(i32_t x, i32_t y, i32_t width, i32_t height, GraphicsContext * gctx);

		/* returns the raw Direct2D Bitmap */
		ID2D1Bitmap * GetBitmap() const;

	public:

		/* the width of the Image */
		i32_t width;

		/* the height of the Image */
		i32_t height;

		/* the interpolation mode */
		ImageInterpolationMode mode;

		/* the opacity of the image. This value should have a value between 0.0 and 1.0 */
		i32_t opacity;

	private:

		/* PImpl pattern */
		struct Implementation;
		std::shared_ptr<Implementation> impl;

	};

}