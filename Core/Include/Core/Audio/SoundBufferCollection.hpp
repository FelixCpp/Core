#pragma once

#include <Core/System/Datatypes.hpp>

#include <vector>
#include <string>

namespace Core
{

	class SoundBufferCollection {
	public:
		
		/* singleton instance */
		static SoundBufferCollection & get();

		/* creates the requested number of buffers, inserts them into the list and returns the, */
		void createBuffers(i32_t count, u32_t * buffers);

		/* loads the sound effect and adds it to the buffers list, if it was loaded successfully */
		u32_t addSoundEffect(const std::string & filepath);

		/* removes a buffer from the buffers list */
		bool removeSoundEffect(u32_t buffer);

	private:

		/* default constructor */
		SoundBufferCollection();

		/* default destructor */
		~SoundBufferCollection();

	private:

		/* returns the OpenAL audioFormat */
		static i32_t getAudioFormat(u16_t channels);

	private:

		/* storage of all buffers. */
		std::vector<u32_t> buffers;

	};

}