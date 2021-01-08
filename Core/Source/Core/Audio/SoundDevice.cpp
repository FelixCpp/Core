#include <Core/Audio/SoundDevice.hpp>
#include <Core/System/Logger.hpp>

#include <al.h>
#include <alc.h>

#pragma comment(lib, "OpenAL32.lib")

namespace Core
{

	std::string SoundDevice::name()
	{
		ALCcontext * context = alcGetCurrentContext();
		ALCdevice * device = alcGetContextsDevice(context);

		if (alcIsExtensionPresent(device, "ALC_ENUMERATE_ALL_EXT"))
		{
			return alcGetString(device, ALC_ALL_DEVICES_SPECIFIER);
		}

		return alcGetString(device, ALC_DEVICE_SPECIFIER);
	}

	bool SoundDevice::initialize(const std::string & deviceName)
	{
		/* just to make clear that the deviceName can be empty */
		const char * deviceNameCStr = deviceName.empty() ? nullptr : deviceName.c_str();

		/* open the default AudioDevice */
		ALCdevice * device = alcOpenDevice(deviceNameCStr);
		if (!device)
		{
			CORE_ERROR("Failed to open the default AudioDevice");
			shutdown();
			return false;
		}

		/* create a context */
		ALCcontext * context = alcCreateContext(device, nullptr);
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
	}

	std::vector<std::string> SoundDevice::getAvailableDevices()
	{
		ALCcontext * context = alcGetCurrentContext();
		ALCdevice * device = alcGetContextsDevice(context);
		
		const ALCchar * devices = alcGetString(device, ALC_DEVICE_SPECIFIER);
		if (!devices)
		{
			return {};
		}

		std::vector<std::string> result;
		const char * ptr = devices;

		do
		{
			result.push_back(devices);
			ptr += result.back().size() + 1;
		} while (*(ptr + 1) != '\0');

		return result;
	}

	void SoundDevice::shutdown()
	{
		ALCcontext * context = alcGetCurrentContext();
		ALCdevice * device = alcGetContextsDevice(context);

		alcMakeContextCurrent(nullptr);
		if(context) alcDestroyContext(context);
		if(device) alcCloseDevice(device);
	}

}