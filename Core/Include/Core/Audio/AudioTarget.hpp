#pragma once

#include <unordered_map>
#include <string>

#include <Core/System/Datatypes.hpp>

#include <Core/Audio/SoundBuffer.hpp>
#include <Core/Audio/SoundSource.hpp>

struct ALCdevice_struct;
struct ALCcontext_struct;

namespace Core
{

	class AudioTarget {
	public:

		/* the default constructor */
		AudioTarget();

		/* a virtual destructor */
		virtual ~AudioTarget();

		/* creates a buffer and returns the SoundSource from it */
		SoundSource loadSound(const std::string & filepath);

	private:

		/* initializes the audio */
		bool initAudio();

		/* destroys the audio */
		void destroyAudio();

		/* destroys every buffer and SoundSources */
		void destroyBuffers();

	private:

		/* an OpenAL device */
		ALCdevice_struct * device;

		/* an OpenAL context */
		ALCcontext_struct * context;

		/* a map from filepath to SoundBuffer. Kind of a cache */
		std::unordered_map<std::string, SoundBuffer> buffers;

	};

}