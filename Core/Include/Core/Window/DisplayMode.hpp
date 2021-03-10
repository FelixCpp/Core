#pragma once

/// <summary>
/// Core
/// </summary>
#include <Core/System/Datatypes.hpp>

/// <summary>
/// C++ / STL
/// </summary>
#include <vector>
#include <string>


namespace Core
{

	class DisplayMode {
	public:

		/// <summary>
		/// The default constructor
		/// </summary>
		DisplayMode();
		
		/// <summary>
		/// Copies the input into the member variables
		/// </summary>
		/// <param name="width">the width</param>
		/// <param name="height">the height</param>
		/// <param name="bitsPerPel">the bits per pixel</param>
		/// <param name="displayFrequency">the frquency in Herz</param>
		explicit DisplayMode(Int32 width, Int32 height, Int32 bitsPerPel, Int32 displayFrequency);

		/// <summary>
		/// Returns all valid display modes for fullscreen mode
		/// </summary>
		static std::vector<DisplayMode> GetFullscreenModes();
		
		/// <summary>
		/// Returns the DisplayMode for the primary Monitor
		/// </summary>
		/// <returns></returns>
		static DisplayMode GetDesktopMode();

		/// <summary>
		/// A string representation of the DisplayMode
		/// </summary>
		std::string ToString() const;

	public:

		/* the width of the DisplayMode in pixels */
		Int32 width;

		/* the height of the DisplayMode in pixels */
		Int32 height;

		/* the bit-depth of the DisplayMode in pixel */
		Int32 bitsPerPel;

		/* the refresh-reate of the DisplayMode in hertz */
		Int32 displayFrequency;

	public:

		/* checks for equality */
		bool operator==(const DisplayMode & other) const;

		/* checks for differences */
		bool operator!=(const DisplayMode & other) const;

	};

}