#include <Core/Audio/SoundBufferCollection.hpp>
#include <Core/Audio/WaveFile.hpp>
#include <Core/Audio/WaveFileReader.hpp>

#include <Core/System/Logger.hpp>

#include <al.h>

namespace Core
{

	SoundBufferCollection & SoundBufferCollection::get()
	{
		static SoundBufferCollection collection;
		return collection;
	}

	u32_t SoundBufferCollection::addSoundEffect(const std::string & filepath)
	{
		/* create a WaveFile instance */
		WaveFile file = {};
		std::memset(&file, 0, sizeof WaveFile);

		/* read the data from the filepath into the file object */
		if (!WaveFileReader::read(filepath, file))
		{
			CORE_ERROR("Failed to read data from \"%s\"", filepath.c_str());
			return 0;
		}

		/* get the OpenAL audio format */
		const WaveFileHeader & header = file.header;
		const ALenum audioFormat = getAudioFormat(header.channels, header.bitsPerSample);
		
		/* create a buffer */
		u32_t buffer = 0;
		alGenBuffers(1, &buffer);

		/* fill the buffer with data */
		alBufferData(buffer, audioFormat, file.data.data(), file.data.size(), header.samplesPerSec);

		/* store it in a list so we can delete them later */
		this->buffers.push_back(buffer);

		/* return the buffer */
		return buffer;
	}

	bool SoundBufferCollection::removeSoundEffect(u32_t buffer)
	{
		/* loop through every buffer */
		for (size_t i = 0; i < this->buffers.size(); i++)
		{
			const u32_t & current = this->buffers[i];
			
			/* check for ID equality */
			if (current == buffer)
			{
				/* delete the buffer */
				alDeleteBuffers(1, &current);

				/* remove it from the list */
				this->buffers.erase(this->buffers.begin() + i);
				
				/* we've found the buffer, so return true */
				return true;
			}
		}

		/* couldn't find the buffer */
		return false;
	}

	SoundBufferCollection::SoundBufferCollection() :
		buffers()
	{
	}

	SoundBufferCollection::~SoundBufferCollection()
	{
		/* delete all buffers from the list */
		alDeleteBuffers(this->buffers.size(), this->buffers.data());

		/* clear the list */
		this->buffers.clear();
	}

	i32_t SoundBufferCollection::getAudioFormat(u16_t channels, u16_t bitsPerSamples)
	{
		if (channels == 1)
		{
			if (bitsPerSamples == 8) return AL_FORMAT_MONO8;
			if (bitsPerSamples == 16) return AL_FORMAT_MONO16;
		} else if (channels == 2)
		{
			if (bitsPerSamples == 8) return AL_FORMAT_STEREO8;
			if (bitsPerSamples == 16) return AL_FORMAT_STEREO16;
		} else
		{
			return -1;
		}
	}

}