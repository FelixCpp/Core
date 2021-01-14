#include <Core/Audio/Listener.hpp>

#include <al.h>

namespace Core
{
	
	void Listener::SetPosition(float x, float y, float z)
	{
		alListener3f(AL_POSITION, x, y, z);
	}

	void Listener::SetPosition(const FVector3 & position)
	{
		SetPosition(position.x, position.y, position.z);
	}

	FVector3 Listener::GetPosition()
	{
		ALfloat x, y, z;
		alGetListener3f(AL_POSITION, &x, &y, &z);
		return FVector3(x, y, z);
	}

	void Listener::SetVelocity(float x, float y, float z)
	{
		alListener3f(AL_VELOCITY, x, y, z);
	}

	void Listener::SetVelocity(const FVector3 & velocity)
	{
		SetVelocity(velocity.x, velocity.y, velocity.z);
	}

	void Listener::SetOrientation(float px, float py, float pz, float ux, float uy, float uz)
	{
		const float orientation[6] = { px, py, pz, ux, uy, uz };
		alListenerfv(AL_ORIENTATION, orientation);
	}

	void Listener::SetOrientation(const FVector3 & position, const FVector3 & up)
	{
		SetOrientation(position.x, position.y, position.z, up.x, up.y, up.z);
	}

	float * Listener::GetOrientation()
	{
		float values[6] = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };
		alGetListenerfv(AL_ORIENTATION, values);
		return values;
	}

	FVector3 Listener::GetVelocity()
	{
		ALfloat x, y, z;
		alGetListener3f(AL_VELOCITY, &x, &y, &z);
		return FVector3(x, y, z);
	}

	void Listener::SetVolume(float volume)
	{
		volume = volume < 0.f ? 0.f : volume;
		alListenerf(AL_GAIN, volume);
	}

	float Listener::GetVolume()
	{
		ALfloat volume = 0.f;
		alGetListenerf(AL_GAIN, &volume);
		return volume;
	}

}