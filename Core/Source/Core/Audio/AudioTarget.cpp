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
		u32_t bufferID = 0;

		/* does the sound exists in the cache? */
		if (this->cache.contains(filepath))
		{
			/* assign the value to the cached buffer */
			bufferID = this->cache.get(filepath);
		} else
		{
			/* load/create, register and assign the value to a new buffer */
			bufferID = SoundBufferCollection::get().addSoundEffect(filepath);

			if (bufferID == 0) // the loading wasn't successfull
				return SoundSource();

			/* store the value in cache */
			this->cache.set(filepath, bufferID);
		}

		/* create a new SoundSource from that */
		SoundSource source = SoundSource::create(bufferID);

		/* register the source so we can delete it later */
		this->soundSources.push_back(source.getSourceID());
		
		/* simply return the source */
		return source;
	}

	AudioTarget::AudioTarget(const std::string & deviceName) :
		soundSources()
	{
		SoundDevice::initialize(deviceName); // initialize the default SoundDevice
	}

}