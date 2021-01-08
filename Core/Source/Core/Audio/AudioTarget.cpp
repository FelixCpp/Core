#include <Core/Audio/AudioTarget.hpp>
#include <Core/Audio/OpenALIDProvider.hpp>
#include <Core/Audio/SoundDevice.hpp>

#include <Core/Audio/WaveFile.hpp>
#include <Core/Audio/WaveFileReader.hpp>

#include <Core/System/Logger.hpp>

#include <al.h>

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

			ALenum format = AL_NONE;
			switch (header.channels)
			{
				case 1:  format = AL_FORMAT_MONO16;                    break;
				case 2:  format = AL_FORMAT_STEREO16;                  break;
				case 4:  format = alGetEnumValue("AL_FORMAT_QUAD16");  break;
				case 6:  format = alGetEnumValue("AL_FORMAT_51CHN16"); break;
				case 7:  format = alGetEnumValue("AL_FORMAT_61CHN16"); break;
				case 8:  format = alGetEnumValue("AL_FORMAT_71CHN16"); break;
				default: format = AL_NONE;                             break;
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
		Music music;
		music.loadFromFile(filepath);
		return music;
	}

	AudioTarget::AudioTarget(const std::string & deviceName) :
		soundCache(),
		musicCache()
	{
		SoundDevice::initialize(deviceName); // initialize the default SoundDevice
	}

}