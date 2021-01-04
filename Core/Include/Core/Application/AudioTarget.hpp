#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include <Core/System/Datatypes.hpp>

#include <Core/Audio/SoundSource.hpp>
#include <Core/Audio/SoundBuffer.hpp>

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

		bool initialize();
		void shutdown();

	private:

		ALCdevice_struct * device;
		ALCcontext_struct * context;

		std::unordered_map<std::string, std::shared_ptr<SoundBuffer>> soundBuffers;

	};

}