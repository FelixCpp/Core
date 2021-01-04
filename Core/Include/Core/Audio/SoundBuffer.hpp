#pragma once

#include <string>
#include <unordered_map>

#include <Core/System/Datatypes.hpp>
#include <Core/Audio/SoundSource.hpp>

namespace Core
{

	class SoundBuffer {
	public:

		SoundBuffer();
		~SoundBuffer();

		bool loadFromFile(const std::string & filepath);
		
		SoundSource createNewSoundSource();

		u32_t getBufferID() const;

	private:

		u32_t bufferID;

		std::vector<SoundSource> soundSources;

	};

}