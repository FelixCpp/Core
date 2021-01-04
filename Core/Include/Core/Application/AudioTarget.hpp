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

		AudioTarget();
		virtual ~AudioTarget();

		SoundSource loadSound(const std::string & filepath);

	private:

		bool initAudio();
		void destroyAudio();
		void destroyBuffers();

	private:

		ALCdevice_struct * device;
		ALCcontext_struct * context;

		std::unordered_map<std::string, SoundBuffer> buffers;

	};

}