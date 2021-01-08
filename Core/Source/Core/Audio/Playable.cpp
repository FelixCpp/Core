#include <Core/Audio/Playable.hpp>
#include <Core/Audio/OpenALIDProvider.hpp>

#include <al.h>

namespace Core
{


	Playable::Playable() :
		sourceID(0),
		pitch(1.f),
		volume(1.f),
		position(0.f, 0.f, 0.f),
		velocity(0.f, 0.f, 0.f),
		loop(false)
	{
	}

	void Playable::create(u32_t bufferID)
	{
		u32_t & sourceID = this->sourceID;
		OpenALSourceIDProvider::generate(1, &sourceID);

		alSourcef(sourceID, AL_PITCH, this->pitch);
		alSourcef(sourceID, AL_GAIN, this->volume);
		alSource3f(sourceID, AL_POSITION, this->position.x, this->position.y, this->position.z);
		alSource3f(sourceID, AL_VELOCITY, this->velocity.x, this->velocity.y, this->velocity.z);
		alSourcei(sourceID, AL_LOOPING, this->loop);
		alSourcei(sourceID, AL_BUFFER, bufferID);
	}

	void Playable::play()
	{
		alSourcePlay(this->sourceID);
	}

	void Playable::pause()
	{
		alSourcePause(this->sourceID);
	}

	void Playable::stop()
	{
		alSourceStop(this->sourceID);
	}

	void Playable::setPosition(float x, float y, float z)
	{
		this->position.x = x;
		this->position.y = y;
		this->position.z = z;

		alSource3f(this->sourceID, AL_POSITION, x, y, z);
	}

	void Playable::setPosition(const FVector3 & position)
	{
		this->setPosition(position.x, position.y, position.z);
	}

	const FVector3 & Playable::getPosition() const
	{
		return this->position;
	}

	void Playable::setVelocity(float x, float y, float z)
	{
		this->velocity.x = x;
		this->velocity.y = y;
		this->velocity.z = z;

		alSource3f(this->sourceID, AL_VELOCITY, x, y, z);
	}

	void Playable::setVelocity(const FVector3 & velocity)
	{
		this->setVelocity(velocity.x, velocity.y, velocity.z);
	}

	const FVector3 & Playable::getVelocity() const
	{
		return this->velocity;
	}

	void Playable::setLooping(bool loop)
	{
		this->loop = loop;
		alSourcei(this->sourceID, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
	}

	bool Playable::isLooping() const
	{
		return this->loop;
	}

	void Playable::setVolume(float volume)
	{
		this->volume = volume < 0.f ? 0.f : volume;
		alSourcef(this->sourceID, AL_GAIN, volume);
	}

	float Playable::getVolume() const
	{
		return this->volume;
	}

	void Playable::setPitch(float pitch)
	{
		this->pitch = pitch < 0.f ? 0.f : pitch;
		alSourcef(this->sourceID, AL_PITCH, pitch);
	}

	float Playable::getPitch() const
	{
		return this->pitch;
	}

	Playable::State Playable::getState() const
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

	u32_t Playable::getSourceID() const
	{
		return this->sourceID;
	}

}