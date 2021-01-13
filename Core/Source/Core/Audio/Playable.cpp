#include <Core/Audio/Playable.hpp>
#include <Core/Audio/OpenALIDProvider.hpp>

#include <al.h>

namespace Core
{

	Playable::Playable() :
		sourceID(0)
	{
	}

	void Playable::create(u32_t bufferID)
	{
		u32_t & sourceID = this->sourceID;
		OpenALSourceIDProvider::generate(1, &sourceID);

		alSourcef(sourceID, AL_PITCH, 1.f);
		alSourcef(sourceID, AL_GAIN, 1.f);
		alSource3f(sourceID, AL_POSITION, 0.f, 0.f, 0.f);
		alSource3f(sourceID, AL_VELOCITY, 0.f, 0.f, 0.f);
		alSourcei(sourceID, AL_LOOPING, AL_FALSE);
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
		alSource3f(this->sourceID, AL_POSITION, x, y, z);
	}

	void Playable::setPosition(const FVector3 & position)
	{
		this->setPosition(position.x, position.y, position.z);
	}

	FVector3 Playable::getPosition() const
	{
		ALfloat x = 0.f, y = 0.f, z = 0.f;
		alGetSource3f(this->sourceID, AL_POSITION, &x, &y, &z);
		return FVector3(x, y, z);
	}

	void Playable::setVelocity(float x, float y, float z)
	{
		alSource3f(this->sourceID, AL_VELOCITY, x, y, z);
	}

	void Playable::setVelocity(const FVector3 & velocity)
	{
		this->setVelocity(velocity.x, velocity.y, velocity.z);
	}

	FVector3 Playable::getVelocity() const
	{
		ALfloat x = 0.f, y = 0.f, z = 0.f;
		alGetSource3f(this->sourceID, AL_VELOCITY, &x, &y, &z);
		return FVector3(x, y, z);
	}

	void Playable::setDirection(float x, float y, float z)
	{
		alSource3f(this->sourceID, AL_DIRECTION, x, y, z);
	}

	void Playable::setDirection(const FVector3 & direction)
	{
		this->setDirection(direction.x, direction.y, direction.z);
	}

	FVector3 Playable::getDirection() const
	{
		ALfloat x = 0.f, y = 0.f, z = 0.f;
		alGetSource3f(this->sourceID, AL_DIRECTION, &x, &y, &z);
		return FVector3(x, y, z);
	}

	void Playable::setRelativeToListener(bool relative)
	{
		alSourcei(this->sourceID, AL_SOURCE_RELATIVE, relative ? AL_TRUE : AL_FALSE);
	}

	bool Playable::isRelativeToListener() const
	{
		ALint isRelative = AL_FALSE;
		alGetSourcei(this->sourceID, AL_SOURCE_RELATIVE, &isRelative);
		return isRelative == AL_TRUE;
	}

	void Playable::setVolume(float volume)
	{
		alSourcef(this->sourceID, AL_GAIN, volume < 0.f ? 0.f : volume);
	}

	float Playable::getVolume() const
	{
		ALfloat volume = 0.f;
		alGetSourcef(this->sourceID, AL_GAIN, &volume);
		return volume;
	}

	void Playable::setPitch(float pitch)
	{
		alSourcef(this->sourceID, AL_PITCH, pitch < 0.f ? 0.f : pitch);
	}

	float Playable::getPitch() const
	{
		ALfloat pitch = 0.f;
		alGetSourcef(this->sourceID, AL_PITCH, &pitch);
		return pitch;
	}

	void Playable::setRolloffFactor(float factor)
	{
		alSourcef(this->sourceID, AL_ROLLOFF_FACTOR, factor);
	}

	float Playable::getRolloffFactor() const
	{
		ALfloat factor = 0.f;
		alGetSourcef(this->sourceID, AL_ROLLOFF_FACTOR, &factor);
		return factor;
	}

	void Playable::setReferenceDistance(float distance)
	{
		alSourcef(this->sourceID, AL_REFERENCE_DISTANCE, distance);
	}

	float Playable::getReferenceDistance() const
	{
		ALfloat referenceDistance = 0.f;
		alGetSourcef(this->sourceID, AL_REFERENCE_DISTANCE, &referenceDistance);
		return referenceDistance;
	}

	void Playable::setMaxDistance(float distance)
	{
		alSourcef(this->sourceID, AL_MAX_DISTANCE, distance);
	}

	float Playable::getMaxDistance() const
	{
		ALfloat maxDistance = 0.f;
		alGetSourcef(this->sourceID, AL_MAX_DISTANCE, &maxDistance);
		return maxDistance;
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