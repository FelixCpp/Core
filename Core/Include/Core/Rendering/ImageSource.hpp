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
#include <Core/Rendering/Color.hpp>  // Core::Color
#include <Core/System/Datatypes.hpp> // Core::i32_t

namespace Core
{

	/// <summary>
	/// Predeclarations
	/// of classes the user
	/// shouldn't interact with
	/// </summary>
	class GraphicsContext;

	/// <summary>
	/// A class to get raw colors
	/// from an image
	/// </summary>
	class ImageSource {
	public:

		/// <summary>
		/// Default constructor
		/// </summary>
		ImageSource();

		/// <summary>
		/// Loads the color values from
		/// the image at that filepath
		/// </summary>
		/// <param name="filepath">location of the image file</param>
		/// <returns>True if the file was loaded successfully</returns>
		bool LoadFromFile(const std::string & filepath, GraphicsContext * gctx);

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

	}; // class ImageSource

} // namespace Core