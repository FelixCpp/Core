#pragma once

#include <string>

#include <Core/Audio/Music.hpp>
#include <Core/Audio/Sound.hpp>
#include <Core/Audio/SoundBufferCache.hpp>

namespace Core
{

	class AudioTarget {
	public:

		virtual ~AudioTarget();

		/* loads a sound effect, creates a buffer (if needed) and source and returns it */
		Sound loadSound(const std::string & filepath);

		/*
			loads music. The difference between sound and music is the preloading (buffering / streaming) 
			Music doesn't get loaded in one step. It loads chunks so the music can play immediately even
			if the file is really large.
		*/
		Music loadMusic(const std::string & filepath);

	protected:

		/* opens the default AudioDevice and initializes the context. If the deviceName is empty, it will open the default SoundDevice. */
		explicit AudioTarget(const std::string & deviceName);

	private:

		SoundBufferCache soundCache;
	
	};

}