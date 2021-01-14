#include <Core/Audio/AudioTarget.hpp>
#include <Core/Audio/OpenALIDProvider.hpp>
#include <Core/Audio/SoundDevice.hpp>

#include <Core/Audio/WaveFileReader.hpp>

#include <Core/System/Logger.hpp>

#include <al.h>

namespace Core
{
	
	AudioTarget::~AudioTarget()
	{
		OpenALBufferIDProvider::Clear();
		OpenALSourceIDProvider::Clear();
		SoundDevice::Shutdown(); // destroy the sounddevice
	}

	Sound AudioTarget::LoadSound(const std::string & filepath)
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
		if (this->soundCache.Contains(filepath))
		{
			bufferID = this->soundCache.Get(filepath);
		} else
		{
			/* step 2: load sound */
			WaveFile info = {};
			std::memset(&info, 0, sizeof WaveFile);

			if (!WaveFileReader::Read(filepath, info))
			{
				return Sound();
			}

			/* step 3: create buffer */
			OpenALBufferIDProvider::Generate(1, &bufferID);

			const ALenum format = GetAudioFormat(info.NumChannels);
			if (format == AL_NONE)
			{
				CORE_ERROR("Invalid audio format");
				return Sound();
			}

			/* fill in the buffer */
			alBufferData(bufferID, format, &info.Data[0], info.Subchunk2Size, info.SampleRate);

			this->soundCache.Set(filepath, bufferID);
		}

		/* step 4: create sound */
		Sound sound;
		sound.Create(bufferID);
		
		/* step 5: return source */
		return sound;
	}

	Music AudioTarget::LoadMusic(const std::string & filepath)
	{
		WaveFile file = {};

		if (this->musicCache.Contains(filepath))
		{
			file = this->musicCache.Get(filepath);
		} else
		{
			if (!WaveFileReader::Read(filepath, file))
			{
				return Music();
			}
		}

		const ALenum format = GetAudioFormat(file.NumChannels);
		if (format == AL_NONE)
		{
			CORE_ERROR("Invalid audio format");
			return Music();
		}
		
		Music music;
		if (music.Init(format, file.SampleRate, file.Data))
		{
			this->musicCache.Set(filepath, file);
			return music;
		}

		return Music();
	}

	AudioTarget::AudioTarget(const std::string & deviceName) :
		soundCache()
	{
		SoundDevice::Initialize(deviceName); // initialize the default SoundDevice
	}

	i32_t AudioTarget::GetAudioFormat(u16_t channels)
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