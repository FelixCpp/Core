#include <Core/Application/AudioTarget.hpp>

#include <alc.h>
#include <iostream>

#pragma comment(lib, "OpenAL32")

namespace Core
{

	AudioTarget::AudioTarget() :
		device(nullptr),
		context(nullptr),
		soundBuffers()
	{
		this->initialize();
	}
	
	AudioTarget::~AudioTarget()
	{
		this->shutdown();
	}
	
	SoundSource AudioTarget::loadSound(const std::string & filepath)
	{
		auto buffer = std::make_shared<SoundBuffer>();
		if (buffer->loadFromFile(filepath))
		{
			this->soundBuffers[filepath] = buffer;
			return this->soundBuffers[filepath]->createNewSoundSource();
		}

		return SoundSource();
	}

	bool AudioTarget::initialize()
	{
		this->device = alcOpenDevice(nullptr);
		if (this->device == nullptr)
		{
			std::cerr << "Failed to open the audiodevice" << std::endl;
			this->shutdown();
			return false;
		}

		this->context = alcCreateContext(this->device, nullptr);
		if (this->context == nullptr)
		{
			std::cerr << "Failed to create an audiocontext" << std::endl;
			this->shutdown();
			return false;
		}

		const ALCboolean success = alcMakeContextCurrent(this->context);
		if (success == ALC_FALSE)
		{
			std::cerr << "Failed to activate the new context" << std::endl;
			this->shutdown();
			return false;
		}

		return true;
	}

	void AudioTarget::shutdown()
	{
		this->soundBuffers.clear();

		const ALCboolean success = alcMakeContextCurrent(nullptr);
		if (success == ALC_FALSE)
		{
			std::cerr << "Failed to release the context" << std::endl;
		}

		if (this->context)
		{
			alcDestroyContext(this->context);
		}

		if (this->device != nullptr)
		{
			const ALCboolean success = alcCloseDevice(this->device);
			if (success == ALC_FALSE)
			{
				std::cerr << "Failed to close the audiodevice" << std::endl;
			}
		}
	}

}