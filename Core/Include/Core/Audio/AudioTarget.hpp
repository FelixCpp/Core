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

		AudioTarget();

	private:

		SoundBufferCache cache;
		std::vector<u32_t> soundSources;

	};

}