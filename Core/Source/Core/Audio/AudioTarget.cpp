#include <Core/Audio/AudioTarget.hpp>
#include <Core/Audio/SoundDevice.hpp>
#include <Core/Audio/SoundBufferCollection.hpp>

#include <al.h>

namespace Core
{
	
	AudioTarget::~AudioTarget()
	{
		alDeleteSources(this->soundSources.size(), this->soundSources.data());
		SoundDevice::shutdown(); // destroy the sounddevice
	}

	SoundSource AudioTarget::loadSound(const std::string & filepath)
	{		
		const u32_t bufferID = SoundBufferCollection::get().addSoundEffect(filepath);
		
		if (bufferID != 0) // means: loaded successfully
		{
			SoundSource source = SoundSource::create(bufferID);
			this->soundSources.push_back(source.getSourceID());
			return source;
		}

		return SoundSource();
	}

	AudioTarget::AudioTarget() :
		soundSources()
	{
		SoundDevice::initialize(); // initialize the default SoundDevice
	}

}