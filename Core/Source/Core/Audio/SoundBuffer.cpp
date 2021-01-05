#include <Core/Audio/SoundBuffer.hpp>

#include <Core/System/Logger.hpp>

#include <iostream>

#include <al.h>

namespace Core
{

	SoundBuffer::SoundBuffer() :
		bufferID(0),
		sources()
	{
	}

	/* creates an OpenAL buffer */
	bool SoundBuffer::create(const WaveFile & file)
	{
		const WaveFileHeader & header = file.header;
		
		/* convert to file-content to an OpenAL format */
		ALenum format = 0;
		if (header.channels == 1)
		{
			if (header.bitsPerSample == 8) format = AL_FORMAT_MONO8;
			else if (header.bitsPerSample == 16) format = AL_FORMAT_MONO16;
		} else if (header.channels == 2)
		{
			if (header.bitsPerSample == 8) format = AL_FORMAT_STEREO8;
			else if (header.bitsPerSample == 16) format = AL_FORMAT_STEREO16;
		} else
		{
			CORE_ERROR("Unsupported audio-format");
			return false;
		}

		/* generate a new buffer */
		alGenBuffers(1, &this->bufferID);

		/* put in the data from the file */
		alBufferData(this->bufferID, format, file.data.data(), file.data.size(), header.samplesPerSec);

		return true;
	}

	/* destroys an OpenAL buffer */
	void SoundBuffer::destroy()
	{
		/* delete every sound-source */
		for (SoundSource & source : this->sources)
		{
			source.destroy();
		}

		/* delete the whole buffer */
		alDeleteBuffers(1, &this->bufferID);
	}

	const SoundSource & SoundBuffer::addSoundSource()
	{
		/* create a new source */
		SoundSource source;
		source.create(this->bufferID);
		
		/* push it into the vector */
		this->sources.push_back(source);

		/* we're returning a reference so we need to return the recently added source of the vector */
		return this->sources.back();
	}

}