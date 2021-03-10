#include <Core/Audio/Playable.hpp>
#include <Core/Audio/OpenALIDProvider.hpp>

#include <al.h>

namespace Core
{

	Playable::Playable() :
		sourceID(0)
	{
	}

	void Playable::Create(UInt32 bufferID)
	{
		UInt32 & sourceID = this->sourceID;
		OpenALSourceIDProvider::Generate(1, &sourceID);

		alSourcef(sourceID, AL_PITCH, 1.f);
		alSourcef(sourceID, AL_GAIN, 1.f);
		alSource3f(sourceID, AL_POSITION, 0.f, 0.f, 0.f);
		alSource3f(sourceID, AL_VELOCITY, 0.f, 0.f, 0.f);
		alSourcei(sourceID, AL_LOOPING, AL_FALSE);
		alSourcei(sourceID, AL_BUFFER, bufferID);
	}

	void Playable::Play()
	{
		alSourcePlay(this->sourceID);
	}

	void Playable::Pause()
	{
		alSourcePause(this->sourceID);
	}

	void Playable::Stop()
	{
		alSourceStop(this->sourceID);
	}

	void Playable::SetPosition(float x, float y, float z)
	{
		alSource3f(this->sourceID, AL_POSITION, x, y, z);
	}

	void Playable::SetPosition(const FVector3 & position)
	{
		this->SetPosition(position.x, position.y, position.z);
	}

	FVector3 Playable::GetPosition() const
	{
		ALfloat x = 0.f, y = 0.f, z = 0.f;
		alGetSource3f(this->sourceID, AL_POSITION, &x, &y, &z);
		return FVector3(x, y, z);
	}

	void Playable::SetVelocity(float x, float y, float z)
	{
		alSource3f(this->sourceID, AL_VELOCITY, x, y, z);
	}

	void Playable::SetVelocity(const FVector3 & velocity)
	{
		this->SetVelocity(velocity.x, velocity.y, velocity.z);
	}

	FVector3 Playable::GetVelocity() const
	{
		ALfloat x = 0.f, y = 0.f, z = 0.f;
		alGetSource3f(this->sourceID, AL_VELOCITY, &x, &y, &z);
		return FVector3(x, y, z);
	}

	void Playable::SetDirection(float x, float y, float z)
	{
		alSource3f(this->sourceID, AL_DIRECTION, x, y, z);
	}

	void Playable::SetDirection(const FVector3 & direction)
	{
		this->SetDirection(direction.x, direction.y, direction.z);
	}

	FVector3 Playable::GetDirection() const
	{
		ALfloat x = 0.f, y = 0.f, z = 0.f;
		alGetSource3f(this->sourceID, AL_DIRECTION, &x, &y, &z);
		return FVector3(x, y, z);
	}

	void Playable::SetRelativeToListener(bool relative)
	{
		alSourcei(this->sourceID, AL_SOURCE_RELATIVE, relative ? AL_TRUE : AL_FALSE);
	}

	bool Playable::IsRelativeToListener() const
	{
		ALint isRelative = AL_FALSE;
		alGetSourcei(this->sourceID, AL_SOURCE_RELATIVE, &isRelative);
		return isRelative == AL_TRUE;
	}

	void Playable::SetVolume(float volume)
	{
		alSourcef(this->sourceID, AL_GAIN, volume < 0.f ? 0.f : volume);
	}

	float Playable::GetVolume() const
	{
		ALfloat volume = 0.f;
		alGetSourcef(this->sourceID, AL_GAIN, &volume);
		return volume;
	}

	void Playable::SetPitch(float pitch)
	{
		alSourcef(this->sourceID, AL_PITCH, pitch < 0.f ? 0.f : pitch);
	}

	float Playable::GetPitch() const
	{
		ALfloat pitch = 0.f;
		alGetSourcef(this->sourceID, AL_PITCH, &pitch);
		return pitch;
	}

	void Playable::SetRolloffFactor(float factor)
	{
		alSourcef(this->sourceID, AL_ROLLOFF_FACTOR, factor);
	}

	float Playable::GetRolloffFactor() const
	{
		ALfloat factor = 0.f;
		alGetSourcef(this->sourceID, AL_ROLLOFF_FACTOR, &factor);
		return factor;
	}

	void Playable::SetReferenceDistance(float distance)
	{
		alSourcef(this->sourceID, AL_REFERENCE_DISTANCE, distance);
	}

	float Playable::GetReferenceDistance() const
	{
		ALfloat referenceDistance = 0.f;
		alGetSourcef(this->sourceID, AL_REFERENCE_DISTANCE, &referenceDistance);
		return referenceDistance;
	}

	void Playable::SetMaxDistance(float distance)
	{
		alSourcef(this->sourceID, AL_MAX_DISTANCE, distance);
	}

	float Playable::GetMaxDistance() const
	{
		ALfloat maxDistance = 0.f;
		alGetSourcef(this->sourceID, AL_MAX_DISTANCE, &maxDistance);
		return maxDistance;
	}

	Playable::State Playable::GetState() const
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

	UInt32 Playable::GetSourceID() const
	{
		return this->sourceID;
	}

}