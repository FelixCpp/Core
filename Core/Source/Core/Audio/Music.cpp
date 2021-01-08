#include <Core/Audio/Music.hpp>
#include <Core/Audio/OpenALIDProvider.hpp>

#include <Core/System/Logger.hpp>

#include <vector>
#include <sndfile.h>
#include <al.h>

namespace Core
{

	struct Music::Impl {

		SNDFILE * file = nullptr;
		SF_INFO info = {};
		std::vector<short> data = {};

	};

	Music::Music() :
		impl(std::make_shared<Impl>()),
		buffers(),
		audioFormat(AL_NONE)
	{
	}

	bool Music::loadFromFile(const std::string & filepath)
	{
		OpenALSourceIDProvider::generate(1, &this->sourceID);
		OpenALBufferIDProvider::generate(NUM_BUFFERS, this->buffers);

		auto & file = this->impl->file;
		auto & info = this->impl->info;
		auto & data = this->impl->data;

		file = sf_open(filepath.c_str(), SFM_READ, &info);
		if (!file)
		{
			CORE_ERROR("Failed to open music file \"%s\"", filepath.c_str());
			return false;
		}

		/* get the sound format, and figure out the OpenAL format */
		this->audioFormat = AL_NONE;
		switch (info.channels)
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
			CORE_ERROR("Unsupported channel count from file \"%s\"", filepath.c_str());
			sf_close(file);
			return false;
		}

		const std::size_t frameSize = (size_t)(BUFFER_SAMPLES * info.channels) * sizeof(short);
		data.resize(frameSize);

		return true;
	}

	void Music::play()
	{
		/* cler any al errors */
		alGetError();

		/* rewind the source position and clear the buffer queue */
		alSourceRewind(this->sourceID);
		alSourcei(this->sourceID, AL_BUFFER, 0);
		
		auto & file = this->impl->file;
		auto & info = this->impl->info;
		auto & data = this->impl->data;
		
		/* fill the buffer queue */
		int index = 0;
		for (; index < NUM_BUFFERS; index++)
		{
			/* get some data to give it to the buffer */
			sf_count_t length = sf_readf_short(file, &data[0], BUFFER_SAMPLES);
			if (length < 1) break;

			length *= info.channels * sizeof(short);
			alBufferData(this->buffers[index], this->audioFormat, data.data(), length, info.samplerate);
		}

		if (alGetError() != AL_NO_ERROR)
		{
			CORE_ERROR("Failed buffering for playback");
			return;
		}

		/* now queue and start playback */
		alSourceQueueBuffers(this->sourceID, index, this->buffers);
		alSourcePlay(this->sourceID);

		if (alGetError() != AL_NO_ERROR)
		{
			CORE_ERROR("Failed starting playback");
			return;
		}
	}

	void Music::update()
	{
		/* clear error */
		alGetError();

		ALint state = AL_NONE;
		ALint processed = AL_NONE;

		/*get relevant source info */
		alGetSourcei(this->sourceID, AL_SOURCE_STATE, &state);
		alGetSourcei(this->sourceID, AL_BUFFERS_PROCESSED, &processed);
		if (alGetError() != AL_NO_ERROR)
		{
			CORE_ERROR("Error checking music source state");
			return;
		}

		auto & file = this->impl->file;
		auto & info = this->impl->info;
		auto & data = this->impl->data;

		/* unqueue and handle each procesed buffer */
		while (processed > 0)
		{
			ALuint bufferID = 0;
			alSourceUnqueueBuffers(this->sourceID, 1, &bufferID);
			
			processed--;

			/*
				read the next chunk of data, refill the buffer, and queue it
				back on the source
			*/
			sf_count_t length = sf_readf_short(file, &data[0], BUFFER_SAMPLES);
			if (length > 0)
			{
				length *= info.channels * sizeof(short);
				alBufferData(bufferID, this->audioFormat, data.data(), length, info.samplerate);
				alSourceQueueBuffers(this->sourceID, 1, &bufferID);
			}

			if (alGetError() != AL_NO_ERROR)
			{
				CORE_ERROR("Error buffering music data");
				return;
			}
		}

		/* make sure the source hasn't underrun */
		if (state != AL_PLAYING && state != AL_PAUSED)
		{
			ALint queued = 0;
			
			/* if no buffers are queued, playback is finished */
			alGetSourcei(this->sourceID, AL_BUFFERS_QUEUED, &queued);
			if (queued == 0) return;

			alSourcePlay(this->sourceID);
			if (alGetError() != AL_NO_ERROR)
			{
				CORE_ERROR("Error restarting music playback");
				return;
			}
		}
	}

}