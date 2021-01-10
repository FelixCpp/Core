#include <Core/Audio/SoundSpace.hpp>

#include <al.h>

namespace Core
{

	void SoundSpace::setDistanceModel(DistanceModel model)
	{
		alDistanceModel(static_cast<ALenum>(model));
	}

	DistanceModel SoundSpace::getDistanceModel()
	{
		ALint distanceModel = alGetInteger(AL_DISTANCE_MODEL);
		return static_cast<DistanceModel>(distanceModel);
	}

	void SoundSpace::setDopplerFactor(float value)
	{
		alDopplerFactor(value);
	}

	float SoundSpace::getDopplerFactor()
	{
		return alGetFloat(AL_DOPPLER_FACTOR);
	}

	void SoundSpace::setDopplerVelocity(float value)
	{
		alDopplerVelocity(value);
	}

	float SoundSpace::getDopplerVelocity()
	{
		return alGetFloat(AL_DOPPLER_VELOCITY);
	}

	void SoundSpace::setSpeedOfSound(float value)
	{
		alSpeedOfSound(value);
	}

	float SoundSpace::getSpeedOfSound()
	{
		return alGetFloat(AL_SPEED_OF_SOUND);
	}

}