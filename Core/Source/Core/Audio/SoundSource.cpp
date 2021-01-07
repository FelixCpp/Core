#include <Core/Audio/SoundSource.hpp>

#include <al.h>

namespace Core
{

	SoundSource::SoundSource() :
		sourceID(0),
		attachedBufferID(0),
		pitch(1.f),
		volume(1.f),
		position(0.f, 0.f, 0.f),
		velocity(0.f, 0.f, 0.f),
		loop(false)
	{
		alGenSources(1, &this->sourceID);
		alSourcef(this->sourceID, AL_PITCH, this->pitch);
		alSourcef(this->sourceID, AL_GAIN, this->volume);
		alSource3f(this->sourceID, AL_POSITION, this->position.x, this->position.y, this->position.z);
		alSource3f(this->sourceID, AL_VELOCITY, this->velocity.x, this->velocity.y, this->velocity.z);
		alSourcei(this->sourceID, AL_LOOPING, this->loop);
		alSourcei(this->sourceID, AL_BUFFER, this->attachedBufferID);
	}

	SoundSource::~SoundSource()
	{
		alDeleteSources(1, &this->sourceID);
	}

	void SoundSource::play(u32_t bufferID)
	{
		if (this->attachedBufferID != bufferID)
		{
			this->attachedBufferID = bufferID;
			alSourcei(this->sourceID, AL_BUFFER, this->attachedBufferID);
		}

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

	void SoundSource::setPosition(float x, float y, float z)
	{
		this->position.x = x;
		this->position.y = y;
		this->position.z = z;

		alSource3f(this->sourceID, AL_POSITION, x, y, z);
	}

	void SoundSource::setPosition(const FVector3 & position)
	{
		this->setPosition(position.x, position.y, position.z);
	}

	const FVector3 & SoundSource::getPosition() const
	{
		return this->position;
	}

	void SoundSource::setVelocity(float x, float y, float z)
	{
		this->velocity.x = x;
		this->velocity.y = y;
		this->velocity.z = z;

		alSource3f(this->sourceID, AL_VELOCITY, x, y, z);
	}

	void SoundSource::setVelocity(const FVector3 & velocity)
	{
		this->setVelocity(velocity.x, velocity.y, velocity.z);
	}

	const FVector3 & SoundSource::getVelocity() const
	{
		return this->velocity;
	}

	void SoundSource::setLooping(bool loop)
	{
		this->loop = loop;
		alSourcei(this->sourceID, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
	}

	bool SoundSource::isLooping() const
	{
		return this->loop;
	}

	void SoundSource::setVolume(float volume)
	{
		this->volume = volume < 0.f ? 0.f : volume;
		alSourcef(this->sourceID, AL_GAIN, volume);
	}

	float SoundSource::getVolume() const
	{
		return this->volume;
	}

	void SoundSource::setPitch(float pitch)
	{
		this->pitch = pitch < 0.f ? 0.f : pitch;
		alSourcef(this->sourceID, AL_PITCH, pitch);
	}

	float SoundSource::getPitch() const
	{
		return this->pitch;
	}

	SoundSource::State SoundSource::getState() const
	{
		ALint state = 0;
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

}