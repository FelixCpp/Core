#include <Core/Audio/SoundSource.hpp>

#include <al.h>

namespace Core
{

	void SoundSource::create(u32_t bufferID)
	{
		alGenSources(1, &this->sourceID);
		alSourcef(this->sourceID, AL_GAIN, 1.f);
		alSourcef(this->sourceID, AL_PITCH, 1.f);
		alSource3f(this->sourceID, AL_POSITION, 0.f, 0.f, 0.f);
		alSource3f(this->sourceID, AL_VELOCITY, 0.f, 0.f, 0.f);
		alSourcei(this->sourceID, AL_LOOPING, AL_FALSE);
		alSourcei(this->sourceID, AL_BUFFER, bufferID);
	}

	void SoundSource::destroy()
	{
		alDeleteSources(1, &this->sourceID);
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

	void SoundSource::setLooping(bool loop)
	{
		alSourcei(this->sourceID, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
	}

	bool SoundSource::isLooping() const
	{
		ALint looping = AL_FALSE;
		alGetSourcei(this->sourceID, AL_LOOPING, &looping);
		return looping;
	}

	SoundSource::State SoundSource::getState() const
	{
		ALsizei state = 0;
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

	void SoundSource::setVolume(float volume)
	{
		if (volume < 0.f) volume = 0.f;
		if (volume > 1.f) volume = 1.f;
		alSourcef(this->sourceID, AL_GAIN, volume);
	}

	float SoundSource::getValue() const
	{
		float volume = 0.f;
		alGetSourcef(this->sourceID, AL_GAIN, &volume);
		return volume;
	}

	void SoundSource::setPitch(float pitch)
	{
		if (pitch < 0.f) pitch = 0.f;
		alSourcef(this->sourceID, AL_PITCH, pitch);
	}

	float SoundSource::getPitch() const
	{
		float pitch = 0.f;
		alGetSourcef(this->sourceID, AL_PITCH, &pitch);
		return pitch;
	}

	void SoundSource::setPosition(float x, float y, float z)
	{
		alSource3f(this->sourceID, AL_POSITION, x, y, z);
	}

	void SoundSource::setPosition(const FVector3 & position)
	{
		this->setPosition(position.x, position.y, position.z);
	}

	FVector3 SoundSource::getPosition() const
	{
		float x = 0.f, y = 0.f, z = 0.f;
		alGetSource3f(this->sourceID, AL_POSITION, &x, &y, &z);
		return FVector3(x, y, z);
	}

	void SoundSource::setVelocity(float x, float y, float z)
	{
		alSource3f(this->sourceID, AL_VELOCITY, x, y, z);
	}

	void SoundSource::setVelocity(const FVector3 & velocity)
	{
		this->setVelocity(velocity.x, velocity.y, velocity.z);
	}

	FVector3 SoundSource::getVelocity() const
	{
		float x = 0.f, y = 0.f, z = 0.f;
		alGetSource3f(this->sourceID, AL_VELOCITY, &x, &y, &z);
		return FVector3(x, y, z);
	}

	SoundSource::SoundSource() :
		sourceID(0)
	{
	}

}