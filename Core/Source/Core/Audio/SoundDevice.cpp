#include <Core/Audio/SoundDevice.hpp>
#include <Core/System/Logger.hpp>

#include <al.h>
#include <alc.h>

#pragma comment(lib, "OpenAL32.lib")

namespace Core
{

	ALCdevice * SoundDevice::device = nullptr;
	ALCcontext * SoundDevice::context = nullptr;
	std::string SoundDevice::deviceName = "";

	const std::string & SoundDevice::name()
	{
		return deviceName;
	}

	bool SoundDevice::initialize()
	{
		/* open the default AudioDevice */
		device = alcOpenDevice(nullptr);
		if (!device)
		{
			CORE_ERROR("Failed to open the default AudioDevice");
			shutdown();
			return false;
		}

		/* create a context */
		context = alcCreateContext(device, nullptr);
		if (!context)
		{
			CORE_ERROR("Failed to create an AudioDevice");
			shutdown();
			return false;
		}

		/* set the context */
		if (!alcMakeContextCurrent(context))
		{
			CORE_ERROR("Failed to make context current");
			shutdown();
			return false;
		}

		if (alcIsExtensionPresent(device, "ALC_ENUMERATE_ALL_EXT"))
		{
			deviceName = alcGetString(device, ALC_ALL_DEVICES_SPECIFIER);
		}

		if (deviceName.empty())
		{
			deviceName = alcGetString(device, ALC_DEVICE_SPECIFIER);
		}
	}

	void SoundDevice::shutdown()
	{
		alcMakeContextCurrent(nullptr);
		if(context) alcDestroyContext(context);
		if(device) alcCloseDevice(device);
		deviceName.clear();
	}

}