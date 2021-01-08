#pragma once

#include <string>
#include <vector>

#include <Core/System/StaticObject.hpp>

namespace Core
{

	class SoundDevice : public StaticObject {
	public:

		/* returns all available sound devices (headset, speaker etc.) */
		static std::vector<std::string> getAvailableDevices();

		/* opens the default AudioDevice and initializes the context. If the deviceName is empty, it will open the default SoundDevice. */
		static bool initialize(const std::string & deviceName);

		/* shutdowns the AudioDevice */
		static void shutdown();

		/* returns the name of the opened device */
		static std::string name();

	};

}