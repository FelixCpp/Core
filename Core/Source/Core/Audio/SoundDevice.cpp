#include <Core/Audio/SoundDevice.hpp>
#include <Core/System/Logger.hpp>

#include <al.h>
#include <alc.h>

#pragma comment(lib, "OpenAL32.lib")

namespace Core
{

	ALCdevice_struct * SoundDevice::device = nullptr;
	ALCcontext_struct * SoundDevice::context = nullptr;

	std::string SoundDevice::Name()
	{
		if (alcIsExtensionPresent(device, "ALC_ENUMERATE_ALL_EXT"))
		{
			return alcGetString(device, ALC_ALL_DEVICES_SPECIFIER);
		}

		return alcGetString(device, ALC_DEVICE_SPECIFIER);
	}

	bool SoundDevice::Initialize(const std::string & deviceName)
	{
		/* just to make clear that the deviceName can be empty */
		const char * deviceNameCStr = deviceName.empty() ? nullptr : deviceName.c_str();

		/* open the default AudioDevice */
		device = alcOpenDevice(deviceNameCStr);
		if (!device)
		{
			CORE_ERROR("Failed to open the default AudioDevice");
			Shutdown();
			return false;
		}

		/* create a context */
		context = alcCreateContext(device, nullptr);
		if (!context)
		{
			CORE_ERROR("Failed to create an AudioDevice");
			Shutdown();
			return false;
		}

		/* set the context */
		if (!alcMakeContextCurrent(context))
		{
			CORE_ERROR("Failed to make context current");
			Shutdown();
			return false;
		}

		return true;
	}

	std::vector<std::string> SoundDevice::GetAvailableDevices()
	{
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

	void SoundDevice::Shutdown()
	{
		alcMakeContextCurrent(nullptr);
		if(context) alcDestroyContext(context);
		if(device) alcCloseDevice(device);
	}

}