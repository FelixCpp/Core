#include <Core/Audio/Listener.hpp>

#include <al.h>

namespace Core
{
	
	void Listener::setPosition(float x, float y, float z)
	{
		alListener3f(AL_POSITION, x, y, z);
	}

	void Listener::setPosition(const FVector3 & position)
	{
		setPosition(position.x, position.y, position.z);
	}

	FVector3 Listener::getPosition()
	{
		ALfloat x, y, z;
		alGetListener3f(AL_POSITION, &x, &y, &z);
		return FVector3(x, y, z);
	}

	void Listener::setVelocity(float x, float y, float z)
	{
		alListener3f(AL_VELOCITY, x, y, z);
	}

	void Listener::setVelocity(const FVector3 & velocity)
	{
		this->setVelocity(velocity.x, velocity.y, velocity.z);
	}

	void Listener::setOrientation(float px, float py, float pz, float ux, float uy, float uz)
	{
		const float orientation[6] = { px, py, pz, ux, uy, uz };
		alListenerfv(AL_ORIENTATION, orientation);
	}

	void Listener::setOrientation(const FVector3 & position, const FVector3 & up)
	{
		setOrientation(position.x, position.y, position.z, up.x, up.y, up.z);
	}

	float * Listener::getOrientation()
	{
		float values[6] = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };
		alGetListenerfv(AL_ORIENTATION, values);
		return values;
	}

	FVector3 Listener::getVelocity()
	{
		ALfloat x, y, z;
		alGetListener3f(AL_VELOCITY, &x, &y, &z);
		return FVector3(x, y, z);
	}

	void Listener::setVolume(float volume)
	{
		volume = volume < 0.f ? 0.f : volume;
		alListenerf(AL_GAIN, volume);
	}

	float Listener::getVolume()
	{
		ALfloat volume = 0.f;
		alGetListenerf(AL_GAIN, &volume);
		return volume;
	}

}