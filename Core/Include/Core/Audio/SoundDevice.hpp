#pragma once

#include <string>

#include <Core/System/StaticObject.hpp>

struct ALCdevice_struct;
struct ALCcontext_struct;

namespace Core
{

	class SoundDevice : public StaticObject {
	public:

		/* opens the default AudioDevice and initializes the context */
		static bool initialize();

		/* shutdowns the AudioDevice */
		static void shutdown();

		/* returns the name of the opened device */
		static const std::string & name();

	private:

		/* An internal OpenAL device */
		static ALCdevice_struct * device;

		/* An internal OpenAL context */
		static ALCcontext_struct * context;

		/* Name of the device which was opened */
		static std::string deviceName;

	};

}