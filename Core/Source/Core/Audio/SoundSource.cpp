#include <Core/Audio/SoundSource.hpp>
#include <Core/Audio/SoundBuffer.hpp>

#include <al.h>

namespace Core
{

	SoundSource::SoundSource() :
		sourceID(AL_NONE)
	{
	}

	SoundSource SoundSource::create(const SoundBuffer & attachedTo)
	{
		ALuint sourceID = AL_NONE;
		alGenSources(1, &sourceID);
		alSourcef(sourceID, AL_PITCH, 1.f);
		alSourcef(sourceID, AL_GAIN, 1.f);
		alSource3f(sourceID, AL_POSITION, 0.f, 0.f, 0.f);
		alSource3f(sourceID, AL_VELOCITY, 0.f, 0.f, 0.f);
		alSourcei(sourceID, AL_LOOPING, AL_FALSE);
		alSourcei(sourceID, AL_BUFFER, attachedTo.getBufferID());
		
		SoundSource result;
		result.sourceID = sourceID;
		return result;
	}

	void SoundSource::play()
	{
		alSourcePlay(this->sourceID);
	}

	void SoundSource::pause()
	{
		alSourcePause(this->sourceID);
	}

	void SoundSource::stop()
	{
		alSourceStop(this->sourceID);
	}

	void SoundSource::setPitch(float pitch)
	{
		alSourcef(this->sourceID, AL_PITCH, pitch);
	}

	float SoundSource::getPitch() const
	{
		ALfloat pitch = 0.f;
		alGetSourcef(this->sourceID, AL_PITCH, &pitch);
		return pitch;
	}

	void SoundSource::setLooping(bool loop)
	{
		alSourcei(this->sourceID, AL_LOOPING, (ALint)loop);
	}

	bool SoundSource::isLooping() const
	{
		ALint looping = AL_FALSE;
		alGetSourcei(this->sourceID, AL_LOOPING, &looping);
		return (bool)looping;
	}

	void SoundSource::setVolume(i32_t volume)
	{
		alSourcef(this->sourceID, AL_GAIN, (ALfloat)volume / 100.f);
	}

	i32_t SoundSource::getVolume() const
	{
		ALfloat volume = 0.f;
		alGetSourcef(this->sourceID, AL_GAIN, &volume);
		return (i32_t)(volume * 100.f);
	}

	SoundSource::State SoundSource::getState() const
	{
		ALint state = AL_INVALID;
		alGetSourcei(this->sourceID, AL_SOURCE_STATE, &state);

		switch (state)
		{
			case AL_INITIAL: return State::Initial;
			case AL_PLAYING: return State::Playing;
			case AL_PAUSED: return State::Paused;
			case AL_STOPPED: return State::Stopped;
			default: return State::Invalid;
		}
	}

	const u32_t & SoundSource::getSourceID() const
	{
		return this->sourceID;
	}

}