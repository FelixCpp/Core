#include <Core/Audio/Music.hpp>
#include <Core/Audio/WaveFile.hpp>
#include <Core/Audio/WaveFileReader.hpp>
#include <Core/Audio/OpenALIDProvider.hpp>

#include <Core/System/Logger.hpp>

#include <vector>
#include <al.h>

namespace Core
{
	
	Music::Music()
	{

	}

	bool Music::init(i32_t audioFormat, i32_t sampleRate, const std::vector<char> & soundData)
	{
		/* create the source */
		this->create(0);

		/* create the buffers */
		ALuint buffers[NUM_BUFFERS];
		alGenBuffers(NUM_BUFFERS, buffers);

		/* fill in the buffers with data */
		for (i32_t i = 0; i < NUM_BUFFERS; i++)
		{
			alBufferData(buffers[i], audioFormat, &soundData[i * BUFFER_SIZE], BUFFER_SIZE, sampleRate);
		}
	
		/* error handling */
		if (alGetError() != AL_NO_ERROR)
		{
			CORE_ERROR("Failed to fill the buffers with data");
			return false;
		}

		/* queue them up */
		alSourceQueueBuffers(this->sourceID, NUM_BUFFERS, buffers);
		
		/* error handling */
		if (alGetError() != AL_NO_ERROR)
		{
			CORE_ERROR("Failedd to queue the buffer up");
			return false;
		}

		/* delete the buffers */
		alDeleteBuffers(NUM_BUFFERS, buffers);

		/* set the cursor */
		this->cursor = BUFFER_SIZE * NUM_BUFFERS;
		CORE_INFO("cursor: %d", (int)cursor);
		//this->cursor = 0;
		this->soundData = soundData;
		this->audioFormat = audioFormat;
		this->sampleRate = sampleRate;

		return true;
	}

	void Music::play()
	{
		if (this->getState() != State::Playing)
		{
			alSourcePlay(this->sourceID);
		}
	}

	void Music::update()
	{
		/* get the number of buffers which was processed */
		ALint buffersProcessed = 0;
		alGetSourcei(this->sourceID, AL_BUFFERS_PROCESSED, &buffersProcessed);
		
		//CORE_INFO("buffersProcessed: %d", buffersProcessed);

		/* we don't need to do anything if there are no buffers to process */
		if (buffersProcessed <= 0)
			return;

		static int i = 0;
		while (buffersProcessed--)
		{
			/*
				get the next buffer to process
				and remove them from the queue
			*/
			ALuint buffer = 0;
			alSourceUnqueueBuffers(this->sourceID, 1, &buffer);
			
			const ALsizei dataSize = BUFFER_SIZE;
			char * data = new char[dataSize];
			std::memset(data, 0, dataSize);

			std::size_t dataSizeToCopy = BUFFER_SIZE;
			if (this->cursor + BUFFER_SIZE > this->soundData.size())
			{
				dataSizeToCopy = this->soundData.size() - this->cursor;
			}

			static bool fill = true;
			std::memcpy(data, &this->soundData[this->cursor], dataSizeToCopy);
			this->cursor += dataSizeToCopy;

			if (this->cursor == this->soundData.size())
				fill = false;

			if (dataSizeToCopy < BUFFER_SIZE)
			{
				this->cursor = 0;
				std::memcpy(&data[dataSizeToCopy], &this->soundData[this->cursor], BUFFER_SIZE - dataSizeToCopy);
				this->cursor = BUFFER_SIZE - dataSizeToCopy;
			}

			if (fill)
			{
				alBufferData(buffer, this->audioFormat, data, BUFFER_SIZE, this->sampleRate);
				alSourceQueueBuffers(this->sourceID, 1, &buffer);
			}

			delete[] data;
		}
	}

}