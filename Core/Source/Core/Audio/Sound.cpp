#include <Core/Audio/Sound.hpp>

#include <al.h>

namespace Core
{

	void Sound::SetLooping(bool loop)
	{
		alSourcei(this->sourceID, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
	}

	bool Sound::IsLooping() const
	{
		ALint looping = AL_FALSE;
		alGetSourcei(this->sourceID, AL_LOOPING, &looping);
		return looping == AL_TRUE;
	}

}