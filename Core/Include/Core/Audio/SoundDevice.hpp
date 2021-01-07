#pragma once

#include <string>

struct ALCdevice_struct;
struct ALCcontext_struct;

namespace Core
{

	class SoundDevice {
	public:

		/* Singleton instance */
		static SoundDevice & get();

		/* returns the name of the opened device */
		const std::string & name() const;

	private:

		/* default constructor. Calls initialize() */
		SoundDevice();

		/* default destructor. Calls shutdown() */
		~SoundDevice();

		/* opens the default AudioDevice and initializes the context */
		bool initialize();

		/* shutdowns the AudioDevice */
		void shutdown();

	private:

		/* An internal OpenAL device */
		ALCdevice_struct * device;

		/* An internal OpenAL context */
		ALCcontext_struct * context;

		/* Name of the device which was opened */
		std::string deviceName;

	};

}