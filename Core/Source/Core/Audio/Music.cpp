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
		sampleRate(0),
		soundData(),
		cursor(0),
		bufferingFinished(true),
		looping(false)
	{
	}

	bool Music::Init(Int32 audioFormat, Int32 sampleRate, const std::vector<char> & soundData)
	{
		/* create the source */
		this->Create(0);

		/* copy the values */
		this->soundData = soundData;
		this->audioFormat = audioFormat;
		this->sampleRate = sampleRate;

		return true;
	}

	void Music::SetLooping(bool value)
	{
		this->looping = value;
	}

	bool Music::IsLooping() const
	{
		return this->looping;
	}

	void Music::Play()
	{
		/*
		* check if the music is even playable
		* if not, just return
		*/
		if (this->soundData.empty() || !this->sourceID)
		{
			return;
		}

		/*
		* changes the state of the source to AL_INITIAL
		* and sets it back to the beginning
		*/
		alSourceRewind(this->sourceID);
		alSourcei(this->sourceID, AL_BUFFER, 0);
		alSourceStop(this->sourceID);

		/* we want to start from the beginning */
		this->cursor = BUFFER_SIZE * NUM_BUFFERS;
		this->bufferingFinished = false;

		/* generate the buffers */
		ALuint buffers[NUM_BUFFERS];
		alGenBuffers(NUM_BUFFERS, buffers);

		/* fill the buffers with data */
		for (Int32 i = 0; i < NUM_BUFFERS; i++)
		{
			const size_t index = i * BUFFER_SIZE; // calculate the index where we want to take the soundData from inside the file
			alBufferData(buffers[i], this->audioFormat, &this->soundData[index], BUFFER_SIZE, this->sampleRate);
		}

		/* queue the buffers up */
		alSourceQueueBuffers(this->sourceID, NUM_BUFFERS, buffers);

		/* delete the buffers */
		alDeleteBuffers(NUM_BUFFERS, buffers);
		
		/* start playing the music */
		alSourcePlay(this->sourceID);
	}

	void Music::Update()
	{
		/* get the number of buffers which needs to be processed */
		ALint buffersProcessed = 0;
		alGetSourcei(this->sourceID, AL_BUFFERS_PROCESSED, &buffersProcessed);

		/* we don't need to do anything if there are no buffers to process */
		if (buffersProcessed <= 0)
			return;

		while (buffersProcessed--)
		{
			/*
			*	get the next buffer to process
			*	and remove them from the queue
			*/
			ALuint buffer = 0;
			alSourceUnqueueBuffers(this->sourceID, 1, &buffer);

			if (this->looping)
			{
				this->UpdateBufferingLoop(buffer);
			} else
			{
				this->UpdateBuffering(buffer);
			}
		}
	}

	void Music::UpdateBufferingLoop(UInt32 buffer)
	{
		/*
		* create the data.
		* It holds the soundData for the
		* current buffer
		*/
		const ALsizei dataSize = BUFFER_SIZE;
		char * data = new char[dataSize];
		std::memset(data, 0, dataSize);

		/*
		* a variable which defines how much data should be copied from the soundData
		* into the data buffer
		*/
		std::size_t dataSizeToCopy = BUFFER_SIZE;

		/* if we don't have that much data anymore, clamp the value */
		if (this->cursor + BUFFER_SIZE > this->soundData.size())
		{
			dataSizeToCopy = this->soundData.size() - this->cursor;
		}

		/* copy the data */
		std::memcpy(data, &this->soundData[this->cursor], dataSizeToCopy);

		/*
		* update the cursor so its at the end of the next chunk
		* or at the end of the file
		*/
		this->cursor += dataSizeToCopy;

		/*
		* copy the rest of the soundData into
		* the data chunk (Take the beginning
		* of the soundfile and append it
		* to the last bit of data) this
		* ends with kind of a loop
		*/
		if (dataSizeToCopy < BUFFER_SIZE)
		{
			this->cursor = 0;
			std::memcpy(&data[dataSizeToCopy], &this->soundData[this->cursor], BUFFER_SIZE - dataSizeToCopy);
			this->cursor = BUFFER_SIZE - dataSizeToCopy;
		}

		/*
		* we only want to queue up any more buffers if we're not finished buffering
		*/
		alBufferData(buffer, this->audioFormat, data, BUFFER_SIZE, this->sampleRate);
		alSourceQueueBuffers(this->sourceID, 1, &buffer);

		/* delete the data */
		delete[] data;
	}

	void Music::UpdateBuffering(UInt32 buffer)
	{
		/*
		* a variable which defines how much data should be copied from the soundData
		* into the data buffer
		*/
		std::size_t dataSizeToCopy = BUFFER_SIZE;

		/* if we don't have that much data anymore, clamp the value */
		if (this->cursor + BUFFER_SIZE > this->soundData.size())
		{
			dataSizeToCopy = this->soundData.size() - this->cursor;
		}

		/*
		* create the data.
		* It holds the soundData for the
		* current buffer
		*/
		char * data = new char[dataSizeToCopy];
		std::memset(data, 0, dataSizeToCopy);

		/* copy the data */
		std::memcpy(data, &this->soundData[this->cursor], dataSizeToCopy);

		/*
		* we only want to queue up any more buffers if we're not finished buffering
		*/
		if (!this->bufferingFinished)
		{
			alBufferData(buffer, this->audioFormat, data, dataSizeToCopy, this->sampleRate);
			alSourceQueueBuffers(this->sourceID, 1, &buffer);
		}

		/*
		* update the cursor so its at the end of the next chunk
		* or at the end of the file
		*/
		this->cursor += dataSizeToCopy;

		/*
		* if the cursor is at the end of the file, we don't need to queue up
		* any more buffers. We are finished buffering/loading buffers
		*/
		if (this->cursor == this->soundData.size())
		{
			this->bufferingFinished = true;
			this->cursor = 0;
		}

		/* delete the data */
		delete[] data;
	}

}