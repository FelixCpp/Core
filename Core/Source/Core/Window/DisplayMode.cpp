#include <Core/Window/DisplayMode.hpp>

#include <sstream>

/// <summary>
/// Windows API
/// </summary>
#include <Windows.h>

namespace Core
{

	DisplayMode::DisplayMode() :
		DisplayMode(0, 0, 0, 0)
	{
	}

	DisplayMode::DisplayMode(Int32 width, Int32 height, Int32 bitsPerPel, Int32 displayFrequency) :
		width(width),
		height(height),
		bitsPerPel(bitsPerPel),
		displayFrequency(displayFrequency)
	{
	}

	std::vector<DisplayMode> DisplayMode::GetFullscreenModes()
	{
		/* create a DEVMODEA instance to read the DevModes in */
		DEVMODEA devMode = {};
		ZeroMemory(&devMode, sizeof DEVMODEA);
		devMode.dmSize = sizeof DEVMODEA;
		devMode.dmDriverExtra = 0;

		/* the list which gets returned at the end */
		std::vector<DisplayMode> modes;

		/* iterate through every display-setting */
		for (Int32 count = 0; EnumDisplaySettingsA(nullptr, count, &devMode); count++)
		{
			/* create a DisplayMode */
			const auto mode = DisplayMode(devMode.dmPelsWidth, devMode.dmPelsHeight, devMode.dmBitsPerPel, devMode.dmDisplayFrequency);
			const auto itr = std::find(modes.begin(), modes.end(), mode);
			
			/* if the element is not in the list */
			if (itr == modes.end())
				/* push it in */
				modes.push_back(mode);
		}

		return modes;
	}

	DisplayMode DisplayMode::GetDesktopMode()
	{
		/* create a DEVMODEA instance to read the Monitor DisplaySettings in */
		DEVMODEA devMode = {};
		ZeroMemory(&devMode, sizeof DEVMODEA);
		devMode.dmSize = sizeof DEVMODEA;
		devMode.dmDriverExtra = 0;

		/* get the monitors DisplaySettings */
		const BOOL success = EnumDisplaySettingsA(nullptr, ENUM_CURRENT_SETTINGS, &devMode);
		if (success)
		{
			/* pass the values over and return the DisplayMode */
			return DisplayMode(devMode.dmPelsWidth, devMode.dmPelsHeight, devMode.dmBitsPerPel, devMode.dmDisplayFrequency);
		} else
		{
			return DisplayMode();
		}
	}

	std::string DisplayMode::ToString() const
	{
		std::stringstream builder;
		builder << "width: " << this->width << '\n';
		builder << "height: " << this->height << '\n';
		builder << "bitsPerPel: " << this->bitsPerPel << '\n';
		builder << "frequency: " << this->displayFrequency << " Herz.";
		return builder.str();
	}

	bool DisplayMode::operator==(const DisplayMode & other) const
	{
		return this->width == other.width && this->height == other.height && this->bitsPerPel == other.bitsPerPel && this->displayFrequency == other.displayFrequency;
	}

	bool DisplayMode::operator!=(const DisplayMode & other) const
	{
		return !(*this == other);
	}

}