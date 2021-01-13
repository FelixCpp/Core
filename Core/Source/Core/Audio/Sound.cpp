#include <Core/Audio/Sound.hpp>

#include <al.h>

namespace Core
{

	void Sound::setLooping(bool loop)
	{
		alSourcei(this->sourceID, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
	}

	bool Sound::isLooping() const
	{
		ALint looping = AL_FALSE;
		alGetSourcei(this->sourceID, AL_LOOPING, &looping);
		return looping == AL_TRUE;
	}

}