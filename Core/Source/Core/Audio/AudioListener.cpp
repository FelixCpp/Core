#include <Core/Audio/AudioListener.hpp>

#include <al.h>

namespace Core
{

	void AudioListener::setVolume(float volume)
	{
		if(volume < 0.f) volume = 0.f,
		alListenerf(AL_GAIN, volume);
	}

	float AudioListener::getVolume()
	{
		float volume = 0.f;
		alGetListenerf(AL_GAIN, &volume);
		return volume;
	}

	void AudioListener::setPosition(float x, float y, float z)
	{
		alListener3f(AL_POSITION, x, y, z);
	}

	void AudioListener::setPosition(const FVector3 & position)
	{
		setPosition(position.x, position.y, position.z);
	}

	FVector3 AudioListener::getPosition()
	{
		float x = 0.f, y = 0.f, z = 0.f;
		alGetListener3f(AL_POSITION, &x, &y, &z);
		return FVector3(x, y, z);
	}

	void AudioListener::setVelocity(float x, float y, float z)
	{
		alListener3f(AL_VELOCITY, x, y, z);
	}

	void AudioListener::setVelocity(const FVector3 & velocity)
	{
		setVelocity(velocity.x, velocity.y, velocity.z);
	}

	FVector3 AudioListener::getVelocity()
	{
		float x = 0.f, y = 0.f, z = 0.f;
		alGetListener3f(AL_VELOCITY, &x, &y, &z);
		return FVector3(x, y, z);
	}

}