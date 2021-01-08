#pragma once

#include <string>
#include <Core/Audio/SoundSource.hpp>
#include <Core/Audio/SoundBufferCache.hpp>

namespace Core
{

	class AudioTarget {
	public:

		virtual ~AudioTarget();

		/* loads a sound, creates a buffer (if needed) and source and returns it */
		SoundSource loadSound(const std::string & filepath);

	protected:

		/* opens the default AudioDevice and initializes the context. If the deviceName is empty, it will open the default SoundDevice. */
		explicit AudioTarget(const std::string & deviceName);

	private:

		SoundBufferCache cache;
		std::vector<u32_t> soundSources;

	};

}