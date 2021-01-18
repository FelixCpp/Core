#pragma once

/// <summary>
/// C++ / STL
/// </summary>
#include <memory> // std::shared_ptr
#include <string> // std::string
#include <vector> // std::vector

/// <summary>
/// Core
/// </summary>
#include <Core/Rendering/Color.hpp>
#include <Core/Rendering/ImageInterpolationMode.hpp>
#include <Core/System/Datatypes.hpp>

struct ID2D1Bitmap;

namespace Core
{

	/// <summary>
	/// Predeclarations
	/// of classes the user
	/// shouldn't interact with
	/// </summary>
	class GraphicsContext;

	/// <summary>
	/// A class which creates images
	/// based around colors (Image + ImageSource)
	/// </summary>
	class Texture {
	public:

		/// <summary>
		/// The default constructor
		/// </summary>
		Texture();

		/// <summary>
		/// Loads the color values from
		/// the image at that filepath
		/// </summary>
		/// <param name="filepath">location of the image file</param>
		/// <returns>True if the file was loaded successfully</returns>
		bool LoadFromFile(const std::string & filepath, GraphicsContext * gctx);

		/// <summary>
		/// Creates an image from
		/// the color data
		/// </summary>
		/// <param name="width">width of the image in pixels</param>
		/// <param name="height">height of the image in pixels</param>
		/// <param name="colors">the data (Color[width * height])</param>
		/// <param name="gctx">A GraphicsContext to create the bitmap from</param>
		/// <returns>True if the image was created successfully</returns>
		bool LoadFromMemory(i32_t width, i32_t height, const Color * colors, GraphicsContext * gctx);

		/// <summary>
		/// Returns the raw
		/// Direct2D Bitmap
		/// </summary>
		ID2D1Bitmap * GetBitmap() const;

	public:

		/// <summary>
		/// container object which
		/// holds the color values
		/// </summary>
		std::vector<Color> colors;

		/// <summary>
		/// The width of the loaded image
		/// </summary>
		i32_t width;

		/// <summary>
		/// The height of the loaded image
		/// </summary>
		i32_t height;

		/// <summary>
		/// The interpolation mode of the image
		/// </summary>
		ImageInterpolationMode mode;

		/// <summary>
		/// The opacity (a value from 0 to 255)
		/// </summary>
		i32_t opacity;

	private:

		/// <summary>
		/// We're using the PImpl pattern
		/// here, so we don't need to
		/// include any dirty headers
		/// in this file
		/// </summary>
		struct Implementation;

		/// <summary>
		/// Pointer to the
		/// Implementation structure
		/// </summary>
		std::shared_ptr<Implementation> impl;

	}; // class Texture

} // namespace Core