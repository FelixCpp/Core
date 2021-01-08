#include <Core/Audio/Music.hpp>
#include <Core/Audio/WaveFile.hpp>
#include <Core/Audio/WaveFileReader.hpp>
#include <Core/Audio/OpenALIDProvider.hpp>

#include <Core/System/Logger.hpp>

#include <vector>
#include <al.h>

namespace Core
{
	
	Music::Music() :
		audioFormat(AL_NONE),
		cursor(0)
	{

	}

	bool Music::loadFromFile(const std::string & filepath)
	{
		WaveFile file;
		if (!WaveFileReader::read(filepath, file))
		{
			return false;
		}

		int channels = file.header.channels;
		int bitsPerSample = file.header.bitsPerSample;
		sampleRate = file.header.samplesPerSec;
		soundData = file.data;

		ALuint buffers[NUM_BUFFERS];
		alGenBuffers(NUM_BUFFERS, &buffers[0]);

		switch (channels)
		{
			case 1:  this->audioFormat = AL_FORMAT_MONO16;                    break;
			case 2:  this->audioFormat = AL_FORMAT_STEREO16;                  break;
			case 4:  this->audioFormat = alGetEnumValue("AL_FORMAT_QUAD16");  break;
			case 6:  this->audioFormat = alGetEnumValue("AL_FORMAT_51CHN16"); break;
			case 7:  this->audioFormat = alGetEnumValue("AL_FORMAT_61CHN16"); break;
			case 8:  this->audioFormat = alGetEnumValue("AL_FORMAT_71CHN16"); break;
			default: this->audioFormat = AL_NONE;                             break;
		}

		if (this->audioFormat == AL_NONE)
		{
			CORE_ERROR("Invalid audio format");
			return false;
		}

		for (std::size_t i = 0; i < NUM_BUFFERS; ++i)
		{
			alBufferData(buffers[i], this->audioFormat, &soundData[i * BUFFER_SIZE], BUFFER_SIZE, sampleRate);
		}

		//this->create(0);

		//OpenALSourceIDProvider::generate(1, &this->sourceID);
		
		ALuint & source = this->sourceID;
		alGenSources(1, &source);
		alSourcef(source, AL_PITCH, 1);
		alSourcef(source, AL_GAIN, 1.0f);
		alSource3f(source, AL_POSITION, 0, 0, 0);
		alSource3f(source, AL_VELOCITY, 0, 0, 0);
		alSourcei(source, AL_LOOPING, AL_FALSE);

		alSourceQueueBuffers(source, NUM_BUFFERS, &buffers[0]);
		
		this->cursor = BUFFER_SIZE * NUM_BUFFERS;

		return true;
	}

	void Music::play()
	{
		alSourcePlay(this->sourceID);
	}

	void Music::update()
	{
		if (this->getState() != State::Playing) return;

		ALint buffersProcessed = 0;
		
		alGetSourcei(this->sourceID, AL_BUFFERS_PROCESSED, &buffersProcessed);

		if (buffersProcessed <= 0)
			return;

		while (buffersProcessed--)
		{
			ALuint buffer;
			alSourceUnqueueBuffers(this->sourceID, 1, &buffer);

			ALsizei dataSize = BUFFER_SIZE;

			char * data = new char[dataSize];
			std::memset(data, 0, dataSize);

			std::size_t dataSizeToCopy = BUFFER_SIZE;
			if (cursor + BUFFER_SIZE > soundData.size())
				dataSizeToCopy = soundData.size() - cursor;

			std::memcpy(&data[0], &soundData[cursor], dataSizeToCopy);
			cursor += dataSizeToCopy;

			if (dataSizeToCopy < BUFFER_SIZE)
			{
				cursor = 0;
				std::memcpy(&data[dataSizeToCopy], &soundData[cursor], BUFFER_SIZE - dataSizeToCopy);
				cursor = BUFFER_SIZE - dataSizeToCopy;
			}

			alBufferData(buffer, this->audioFormat, data, BUFFER_SIZE, sampleRate);
			alSourceQueueBuffers(this->sourceID, 1, &buffer);

			delete[] data;
		}
	}

}