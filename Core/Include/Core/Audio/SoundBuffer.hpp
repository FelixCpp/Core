#pragma once

#include <vector>

#include <Core/System/Datatypes.hpp>

#include <Core/Audio/SoundSource.hpp>
#include <Core/Audio/WaveFile.hpp>

namespace Core
{

	class SoundBuffer {
	public:

		SoundBuffer();

		/* creates an OpenAL buffer */
		bool create(const WaveFile & file);

		/* destroys an OpenAL buffer */
		void destroy();

		/* creates a new SoundSource object, pushes it into the vector and returns it */
		const SoundSource & addSoundSource();

	public:

		/*
			the openal-generated id for the buffer
		*/
		u32_t bufferID;

		/*
			a list of sound-sources generated based on that bufferID
		*/
		std::vector<SoundSource> sources;

	};

}