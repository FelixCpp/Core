#pragma once

/// <summary>
/// C++ / STL
/// </summary>
#include <memory>
#include <vector>
#include <string>

/// <summary>
/// Core
/// </summary>
#include <Core/Rendering/Color.hpp>
#include <Core/Rendering/ImageInterpolationMode.hpp>
#include <Core/System/Datatypes.hpp>

/// <summary>
/// Ignore this
/// </summary>
struct ID2D1Bitmap;

namespace Core
{

	/// <summary>
	/// Predeclarations
	/// of classes the user
	/// shouldn't interact with
	/// </summary>
	class Renderer;

	/// <summary>
	/// A class which represents
	/// renderable images
	/// </summary>
	class Image {
	public:

		/// <summary>
		/// Default constructor
		/// </summary>
		Image();

		/// <summary>
		/// Basically does the name thing as
		/// LoadFromMemory but guarantees to
		/// create a brand new bitmap
		/// </summary>
		/// <param name="width">width of the image</param>
		/// <param name="height">height of the image</param>
		/// <param name="colors">the data (Color[width * height]</param>
		/// <param name="renderer">a Renderer to get access to the RenderTarget</param>
		/// <returns>True if the bitmap was created successfully</returns>
		bool Create(i32_t width, i32_t height, const Color colors[], Renderer * renderer);

		/// <summary>
		/// Loads the color values from
		/// the image at that filepath
		/// </summary>
		/// <param name="filepath">location of the image file</param>
		/// <param name="renderer">a Renderer to get access to the RenderTarget</param>
		/// <returns>True if the file was loaded successfully</returns>
		bool LoadFromFile(const std::string & filepath, Renderer * renderer);

		/// <summary>
		/// Creates an image based on the
		/// passed colors
		/// </summary>
		/// <param name="width">width of the image</param>
		/// <param name="height">height of the image</param>
		/// <param name="colors">Color[width * height]</param>
		/// <param name="renderer">a Renderer to get access to the RenderTarget</param>
		/// <returns>True if everything the image was created successfully</returns>
		bool LoadFromMemory(i32_t width, i32_t height, const Color colors[], Renderer * renderer);

		/// <summary>
		/// Copies an area from the other image
		/// and puts them into the bitmap
		/// structure.
		/// 
		/// If the boundary is invalid, the boundary
		/// shrinks to fit.
		/// 
		/// <remarks>
		/// shrinking the boundary (area to copy)
		/// won't not return false
		/// </remarks>
		/// </summary>
		/// <param name="other">the image to copy from</param>
		/// <param name="x">start x in pixels</param>
		/// <param name="y">start y in pixels</param>
		/// <param name="width">width to copy in pixels</param>
		/// <param name="height">height to copy in pixels</param>
		/// <param name="renderer">a Renderer to get access to the RenderTarget</param>
		/// <returns>True if the image was copied successfully</returns>
		bool LoadFromImage(const Image & other, i32_t x, i32_t y, i32_t width, i32_t height, Renderer * renderer);

		/// <summary>
		/// Copies the pixels from the RenderTarget
		/// contained by the Renderer into the
		/// bitmap
		/// </summary>
		/// <param name="x">start x in pixel space</param>
		/// <param name="y">start y in pixel space</param>
		/// <param name="width">width to copy in pixels</param>
		/// <param name="height">height to copy in pixels</param>
		/// <param name="renderer">the Renderer containing the RenderTarget to copy the data from</param>
		/// <returns>True if everything goes as expected</returns>
		bool LoadFromScreen(i32_t x, i32_t y, i32_t width, i32_t height, Renderer * renderer);

		/// <summary>
		/// Updates the image to
		/// match the colors
		/// </summary>
		bool UpdateColors();

		/// <summary>
		/// Returns the raw
		/// Direct2D-Bitmap
		/// </summary>
		/// <returns></returns>
		ID2D1Bitmap * GetBitmap() const;

	private:

		/// <summary>
		/// Copies the raw data
		/// into the bitmap.
		/// 
		/// <remarks>
		/// This method does not copy
		/// any data into their
		/// member variables!
		/// </remarks>
		/// </summary>
		bool CopyFromRawData(i32_t width, i32_t height, const Color colors[]);

	public:

		/// <summary>
		/// width of the image
		/// </summary>
		i32_t width;

		/// <summary>
		/// height of the image
		/// </summary>
		i32_t height;

		/// <summary>
		/// interpolation mode
		/// </summary>
		ImageInterpolationMode mode;

		/// <summary>
		/// the opacity (a value between 0 and 255)
		/// </summary>
		i32_t opacity;

		/// <summary>
		/// each pixel represented
		/// as a Color
		/// </summary>
		std::vector<Color> colors;

	private:

		/// <summary>
		/// We're using the PImpl pattern
		/// here, so we don't need to
		/// include any dirty headers
		/// in this file
		/// </summary>
		struct Implementation;

		/// <summary>
		/// Implementation instance to get
		/// acess to its members
		/// </summary>
		std::shared_ptr<Implementation> impl;

	};

}