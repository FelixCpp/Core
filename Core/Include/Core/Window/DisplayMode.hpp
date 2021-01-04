#pragma once

#include <vector>

#include <Core/System/Datatypes.hpp>

namespace Core
{

	class DisplayMode {
	public:

		/* the default constructor */
		DisplayMode();
		
		/* copies the parameter into the member-variables of this class */
		explicit DisplayMode(i32_t width, i32_t height, i32_t bitsPerPel, i32_t displayFrequency);

		/* returns a list of valid DisplayModes which are valid for fullscreen-mode */
		static std::vector<DisplayMode> getFullscreenModes();

		/* returns the DisplayMode of the primary monitor */
		static DisplayMode getDesktopMode();

	public:

		/* the width of the DisplayMode in pixels */
		i32_t width;

		/* the height of the DisplayMode in pixels */
		i32_t height;

		/* the bit-depth of the DisplayMode in pixel */
		i32_t bitsPerPel;

		/* the refresh-reate of the DisplayMode in hertz */
		i32_t displayFrequency;

	public:

		/* checks for equality */
		bool operator==(const DisplayMode & other) const;

		/* checks for differences */
		bool operator!=(const DisplayMode & other) const;

	};

}