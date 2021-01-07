#include <Core/Audio/SoundDevice.hpp>
#include <Core/System/Logger.hpp>

#include <al.h>
#include <alc.h>

namespace Core
{

	SoundDevice & SoundDevice::get()
	{
		static SoundDevice device;
		return device;
	}

	const std::string & SoundDevice::name() const
	{
		return this->deviceName;
	}

	SoundDevice::SoundDevice() :
		device(nullptr),
		context(nullptr),
		deviceName()
	{
		this->initialize();
	}

	SoundDevice::~SoundDevice()
	{
		this->shutdown();
	}

	bool SoundDevice::initialize()
	{
		/* open the default AudioDevice */
		this->device = alcOpenDevice(nullptr);
		if (!this->device)
		{
			CORE_ERROR("Failed to open the default AudioDevice");
			this->shutdown();
			return false;
		}

		/* create a context */
		this->context = alcCreateContext(this->device, nullptr);
		if (!this->context)
		{
			CORE_ERROR("Failed to create an AudioDevice");
			this->shutdown();
			return false;
		}

		/* set the context */
		if (!alcMakeContextCurrent(this->context))
		{
			CORE_ERROR("Failed to make context current");
			this->shutdown();
			return false;
		}

		if (alcIsExtensionPresent(this->device, "ALC_ENUMERATE_ALL_EXT"))
		{
			this->deviceName = alcGetString(this->device, ALC_ALL_DEVICES_SPECIFIER);
		}

		if (this->deviceName.empty())
		{
			this->deviceName = alcGetString(this->device, ALC_DEVICE_SPECIFIER);
		}
	}

	void SoundDevice::shutdown()
	{
		alcMakeContextCurrent(nullptr);
		if(this->context) alcDestroyContext(this->context);
		if(this->device) alcCloseDevice(this->device);
		this->deviceName.clear();
	}

}