#pragma once

#include <string>
#include <vector>

#include <Core/System/StaticObject.hpp>

struct ALCdevice_struct;
struct ALCcontext_struct;

namespace Core
{

	class SoundDevice : public StaticObject {
	public:

		/* returns all available sound devices (headset, speaker etc.) */
		static std::vector<std::string> GetAvailableDevices();

		/* opens the default AudioDevice and initializes the context. If the deviceName is empty, it will open the default SoundDevice. */
		static bool Initialize(const std::string & deviceName);

		/* shutdowns the AudioDevice */
		static void Shutdown();

		/* returns the name of the opened device */
		static std::string Name();

	private:

		static ALCdevice_struct * device;
		static ALCcontext_struct * context;

	};

}