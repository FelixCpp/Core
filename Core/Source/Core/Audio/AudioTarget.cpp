#include <Core/Audio/AudioTarget.hpp>

#include <Core/Audio/WaveFile.hpp>
#include <Core/Audio/WaveFileReader.hpp>

#include <Core/System/Logger.hpp>

#include <alc.h>
#include <al.h>

#include <iostream>
#include <optional>

#pragma comment(lib, "OpenAL32.lib")

namespace Core
{

	AudioTarget::AudioTarget() :
		device(nullptr),
		context(nullptr),
		buffers()
	{
		this->initAudio();
	}

	AudioTarget::~AudioTarget()
	{
		this->destroyAudio();
	}

	SoundSource AudioTarget::loadSound(const std::string & filepath)
	{
		/* find a key-value pair matching the filepath */
		const auto itr = this->buffers.find(filepath);
		if (itr != this->buffers.end())	/* there is already a sound-buffer under that key */
		{
			SoundBuffer & buffer = itr->second;
			return buffer.addSoundSource();
		} else /* there is no sound-buffer under that key */
		{
			/* create a WaveFile structure containing the data of the passed file */
			WaveFile file = {};
			std::memset(&file, 0, sizeof WaveFile);

			/* read the data */
			if (WaveFileReader::read(filepath, file))
			{
				/* the []-operator inserts a new item automatically */
				SoundBuffer & buffer = this->buffers[filepath];
				
				/* create a new buffer with the given data from that file */
				if (buffer.create(file))
				{
					return buffer.addSoundSource();
				}
			}
		}

		return SoundSource();
	}

	bool AudioTarget::initAudio()
	{
		/* open the audio-device */
		this->device = alcOpenDevice(nullptr);
		if (this->device == nullptr)
		{
			CORE_ERROR("Failed to open the audio-device");
			return false;
		}

		/* create an audio-context */
		this->context = alcCreateContext(this->device, nullptr);
		if (this->context == nullptr)
		{
			CORE_ERROR("Failed to create an AudioContext");
			return false;
		}

		/* make the context to the current one */
		ALCboolean success = alcMakeContextCurrent(this->context);
		if (success == ALC_FALSE)
		{
			CORE_ERROR("Failed to activate the new audio-context");
			return false;
		}

		return true;
	}

	void AudioTarget::destroyAudio()
	{
		this->destroyBuffers();

		/* release the audio-context. */
		ALCboolean success = alcMakeContextCurrent(nullptr);
		if (success == ALC_FALSE)
		{
			CORE_ERROR("Failed to release the audio-context");
		}
		
		/* destroy the audio-context */
		if (this->context)
		{
			alcDestroyContext(this->context);
		}

		/* close the audio-device */
		if (this->device)
		{
			alcCloseDevice(this->device);
		}
	}

	void AudioTarget::destroyBuffers()
	{
		for (auto & [filepath, buffer] : this->buffers)
		{
			buffer.destroy();
		}

		this->buffers.clear();
	}

}