#include <Core/Audio/SoundBuffer.hpp>
#include <Core/Audio/SoundFileLoader.hpp>

#include <al.h>

#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

namespace
{
	ALenum getAudioFormat(Core::u8_t channels, Core::u8_t bitsPerSample)
	{
		if (channels == 1)
		{
			if (bitsPerSample == 8) return AL_FORMAT_MONO8;
			if (bitsPerSample == 16) return AL_FORMAT_MONO16;
		} else if (channels == 2)
		{
			if (bitsPerSample == 8) return AL_FORMAT_STEREO8;
			if (bitsPerSample == 16) return AL_FORMAT_STEREO16;
		} else
		{
			return AL_INVALID;
		}
	}
}

namespace Core
{

	SoundBuffer::SoundBuffer() :
		bufferID(0)
	{
	}

	SoundBuffer::~SoundBuffer()
	{
		for (SoundSource & source : this->soundSources)
		{
			alDeleteSources(1, &source.getSourceID());
		}
		
		alDeleteBuffers(1, &this->bufferID);
	}

	bool SoundBuffer::loadFromFile(const std::string & filepath)
	{
		const fs::path extension = fs::path(filepath).extension();
		
		if (extension == ".wav")
		{
			u8_t channels = 0;
			u8_t bitsPerSample = 0;
			i32_t samplesPerSecond = 0;
			std::vector<char> soundData = {};

			const bool success = SoundFileLoader::loadWav(filepath, channels, bitsPerSample, samplesPerSecond, soundData);
			if (!success)
			{
				std::cerr << "Failed to load the .wav file" << std::endl;
				return false;
			}

			const ALenum audioFormat = getAudioFormat(channels, bitsPerSample);
			if (audioFormat == AL_INVALID)
			{
				std::cerr << "Invalid audioformat" << std::endl;
				return false;
			}

			alGenBuffers(1, &this->bufferID);
			alBufferData(this->bufferID, audioFormat, soundData.data(), soundData.size(), samplesPerSecond);
		} else
		{
			std::cerr << "The filetype is not supported" << std::endl;
			return false;
		}

		return true;
	}

	SoundSource SoundBuffer::createNewSoundSource()
	{
		const SoundSource newSoundSource = SoundSource::create(*this);
		this->soundSources.push_back(newSoundSource);
		return newSoundSource;
	}



	u32_t SoundBuffer::getBufferID() const
	{
		return this->bufferID;
	}

}