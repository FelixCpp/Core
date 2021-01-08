#pragma once

#include <memory>

#include <Core/Audio/Playable.hpp>

#include <Core/System/Datatypes.hpp>

namespace Core
{

	class Music : public Playable {
	public:

		/* Amount of Samples per Buffer */
		inline static const int BUFFER_SAMPLES = 8192;

		/* Amount of buffers per Soundfile */
		inline static const int NUM_BUFFERS = 4;

	public:

		/* default constructor */
		Music();

		/* default destructor */
		~Music() = default;
		
		/* loads the data from a file */
		bool loadFromFile(const std::string & filepath);

		/* override the play method */
		virtual void play() override;

		/* continues loading the soundfile */
		void update();

	private:

		/*
			We're using a PImpl (Pointered Implementation)
			pattern so we can include dirty files in the
			cpp so they doesn't spread out into the Program
			which uses this file
		*/
		struct Impl;
		std::shared_ptr<Impl> impl;

		u32_t buffers[NUM_BUFFERS];
		i32_t audioFormat;

	};

}