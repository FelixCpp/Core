#include <Core/Audio/SoundBufferCollection.hpp>
#include <Core/Audio/SoundFileReader.hpp>
#include <Core/Audio/OpenALBufferProperties.hpp>

#include <Core/System/Logger.hpp>

#include <sndfile.h>
#include <al.h>

namespace Core
{

	SoundBufferCollection & SoundBufferCollection::get()
	{
		static SoundBufferCollection collection;
		return collection;
	}

	void SoundBufferCollection::createBuffers(i32_t count, u32_t * buffers)
	{
		/* generate them */
		alGenBuffers(count, buffers);
		
		/* insert each buffer to delete them later */
		for (i32_t i = 0; i < count; i++)
			this->buffers.push_back(buffers[i]);
	}

	u32_t SoundBufferCollection::addSoundEffect(const std::string & filepath)
	{
		OpenALBufferProperties properties = {};
		std::memset(&properties, 0, sizeof OpenALBufferProperties);

		if (!SoundFileReader::read(filepath, properties))
		{
			/* we don't need to print anything out. Its all done in WaveFileReader::read(...) */
			return 0;
		}

		SF_INFO & info = properties.header;
		std::vector<short> & data = properties.data;

		/* get the OpenAL audio format */
		const ALenum audioFormat = getAudioFormat(info.channels);
		if (audioFormat == -1)
		{
			CORE_ERROR("Invalid Audio Format (%d)", audioFormat);
			return 0;
		}

		/* create a buffer */
		u32_t buffer = 0;
		alGenBuffers(1, &buffer);

		/* fill the buffer with data */
		alBufferData(buffer, audioFormat, data.data(), data.size(), info.samplerate);

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

	i32_t SoundBufferCollection::getAudioFormat(u16_t channels)
	{
		switch (channels)
		{
			case 1:  return AL_FORMAT_MONO16;                    break;
			case 2:  return AL_FORMAT_STEREO16;                  break;
			case 4:  return alGetEnumValue("AL_FORMAT_QUAD16");  break;
			case 6:  return alGetEnumValue("AL_FORMAT_51CHN16"); break;
			case 7:  return alGetEnumValue("AL_FORMAT_61CHN16"); break;
			case 8:  return alGetEnumValue("AL_FORMAT_71CHN16"); break;
			default: return -1;                                  break;
		}
	}

}