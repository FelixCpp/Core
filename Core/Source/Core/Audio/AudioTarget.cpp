#include <Core/Audio/AudioTarget.hpp>
#include <Core/Audio/OpenALIDProvider.hpp>
#include <Core/Audio/SoundDevice.hpp>

#include <Core/Audio/WaveFile.hpp>
#include <Core/Audio/WaveFileReader.hpp>

#include <Core/System/Logger.hpp>

#include <al.h>
#include <inttypes.h>

namespace Core
{
	
	AudioTarget::~AudioTarget()
	{
		OpenALBufferIDProvider::clear();
		OpenALSourceIDProvider::clear();
		SoundDevice::shutdown(); // destroy the sounddevice
	}

	Sound AudioTarget::loadSound(const std::string & filepath)
	{	
		/*
			step 1: check cache
				step 2: load sound
				step 3: create buffer
			
			step 4: create sound
			step 5: return source
		*/

		u32_t bufferID = AL_NONE;

		/* step 1: check cache */
		if (this->soundCache.contains(filepath))
		{
			bufferID = this->soundCache.get(filepath);
		} else
		{
			/* step 2: load sound */
			WaveFile file = {};
			std::memset(&file, 0, sizeof WaveFile);

			if (!WaveFileReader::read(filepath, file))
			{
				return Sound();
			}

			auto & header = file.header;
			auto & data = file.data;

			/* step 3: create buffer */
			OpenALBufferIDProvider::generate(1, &bufferID);

			const ALenum format = getAudioFormat(header.channels);
			if (format == AL_NONE)
			{
				CORE_ERROR("Invalid audio format");
				return Sound();
			}

			/* fill in the buffer */
			alBufferData(bufferID, format, data.data(), data.size(), header.samplesPerSec);

			this->soundCache.set(filepath, bufferID);
		}

		/* step 4: create sound */
		Sound sound;
		sound.create(bufferID);
		
		/* step 5: return source */
		return sound;
	}

	Music AudioTarget::loadMusic(const std::string & filepath)
	{
		WaveFile file = {};

		if (this->musicCache.contains(filepath))
		{
			file = this->musicCache.get(filepath);
		} else
		{
			if (!WaveFileReader::read(filepath, file))
			{
				return Music();
			}
		}

		const ALenum format = getAudioFormat(file.header.channels);
		if (format == AL_NONE)
		{
			CORE_ERROR("Invalid audio format");
			return Music();
		}
		
		Music music;
		if (music.init(format, file.header.samplesPerSec, file.data))
		{
			this->musicCache.set(filepath, file);
			return music;
		}

		return Music();
	}

	AudioTarget::AudioTarget(const std::string & deviceName) :
		soundCache()
	{
		SoundDevice::initialize(deviceName); // initialize the default SoundDevice
	}

	i32_t AudioTarget::getAudioFormat(u16_t channels)
	{
		switch (channels)
		{
			case 1:  return AL_FORMAT_MONO16;
			case 2:  return AL_FORMAT_STEREO16;
			case 4:  return alGetEnumValue("AL_FORMAT_QUAD16");
			case 6:  return alGetEnumValue("AL_FORMAT_51CHN16");
			case 7:  return alGetEnumValue("AL_FORMAT_61CHN16");
			case 8:  return alGetEnumValue("AL_FORMAT_71CHN16");

			default:
			{
				CORE_ERROR("Unsupported channel count (%d)", (i32_t)channels);
				return AL_NONE;
			} break;
		}
	}

}