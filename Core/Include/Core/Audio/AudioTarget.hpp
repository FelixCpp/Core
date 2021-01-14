#pragma once

#include <string>

#include <Core/Audio/Music.hpp>
#include <Core/Audio/Sound.hpp>
#include <Core/Audio/Cache.hpp>
#include <Core/Audio/WaveFile.hpp>

namespace Core
{

	class AudioTarget {
	public:

		/* virtual destructor */
		virtual ~AudioTarget();

		/* loads a sound effect, creates a buffer (if needed) and source and returns it */
		Sound LoadSound(const std::string & filepath);

		/*
			loads music. The difference between sound and music is the preloading (buffering / streaming) 
			Music doesn't get loaded in one step. It loads chunks so the music can play immediately even
			if the file is really large.
		*/
		Music LoadMusic(const std::string & filepath);

	protected:

		/* opens the default AudioDevice and initializes the context. If the deviceName is empty, it will open the default SoundDevice. */
		explicit AudioTarget(const std::string & deviceName);

		/* returns an OpenAL Audio format based on the passed channel count */
		static i32_t GetAudioFormat(u16_t channels);

	private:

		Cache<std::string, u32_t> soundCache;
		Cache<std::string, WaveFile> musicCache;

	};

}