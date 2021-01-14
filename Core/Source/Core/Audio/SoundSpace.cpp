#include <Core/Audio/SoundSpace.hpp>

#include <al.h>

namespace Core
{

	void SoundSpace::SetDistanceModel(DistanceModel model)
	{
		alDistanceModel(static_cast<ALenum>(model));
	}

	DistanceModel SoundSpace::GetDistanceModel()
	{
		ALint distanceModel = alGetInteger(AL_DISTANCE_MODEL);
		return static_cast<DistanceModel>(distanceModel);
	}

	void SoundSpace::SetDopplerFactor(float value)
	{
		alDopplerFactor(value);
	}

	float SoundSpace::GetDopplerFactor()
	{
		return alGetFloat(AL_DOPPLER_FACTOR);
	}

	void SoundSpace::SetDopplerVelocity(float value)
	{
		alDopplerVelocity(value);
	}

	float SoundSpace::GetDopplerVelocity()
	{
		return alGetFloat(AL_DOPPLER_VELOCITY);
	}

	void SoundSpace::SetSpeedOfSound(float value)
	{
		alSpeedOfSound(value);
	}

	float SoundSpace::GetSpeedOfSound()
	{
		return alGetFloat(AL_SPEED_OF_SOUND);
	}

}