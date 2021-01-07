#include <Core/Audio/SoundSource.hpp>

#include <al.h>

namespace Core
{

	SoundSource::SoundSource() :
		sourceID(0),
		pitch(1.f),
		volume(1.f),
		position(0.f, 0.f, 0.f),
		velocity(0.f, 0.f, 0.f),
		loop(false)
	{
	}

	SoundSource SoundSource::create(u32_t bufferID)
	{
		SoundSource result;
		u32_t & sourceID = result.sourceID;
		
		alGenSources(1, &sourceID);
		alSourcef(sourceID, AL_PITCH, result.pitch);
		alSourcef(sourceID, AL_GAIN, result.volume);
		alSource3f(sourceID, AL_POSITION, result.position.x, result.position.y, result.position.z);
		alSource3f(sourceID, AL_VELOCITY, result.velocity.x, result.velocity.y, result.velocity.z);
		alSourcei(sourceID, AL_LOOPING, result.loop);
		alSourcei(sourceID, AL_BUFFER, bufferID);

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

	u32_t SoundSource::getSourceID() const
	{
		return this->sourceID;
	}

}