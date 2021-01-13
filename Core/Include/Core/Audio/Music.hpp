#pragma once

#include <memory>

#include <Core/Audio/Playable.hpp>
#include <Core/System/Datatypes.hpp>

namespace Core
{

	class Music : public Playable {
	public:

		/* Amount of Samples per Buffer */
		inline static const int BUFFER_SIZE = 65536; // 32kb of data in each buffer

		/* Amount of buffers per Soundfile */
		inline static const int NUM_BUFFERS = 4;

	public:

		/* default constructor */
		Music();

		/* default destructor */
		~Music() = default;
		
		/* loads the data from a file */
		bool init(i32_t audioFormat, i32_t sampleRate, const std::vector<char> & soundData);

		/* overrides the setLooping function */
		void setLooping(bool value);

		/* returns true if the music is looping */
		bool isLooping() const;

		/* override the play method */
		virtual void play() override;

		/* continues loading the soundfile */
		void update();

	private:

		/* buffers so the music can loop */
		void updateBufferingLoop(u32_t buffer);

		/* buffers so the music plays once */
		void updateBuffering(u32_t buffer);

	private:

		/*
			We're using a PImpl (Pointered Implementation)
			pattern so we can include dirty files in the
			cpp so they doesn't spread out into the Program
			which uses this file
		*/
		//struct Impl;
		//std::shared_ptr<Impl> impl;

		i32_t audioFormat;
		i32_t sampleRate;
		std::vector<char> soundData;
		std::size_t cursor;

		bool bufferingFinished;
		bool looping;

	};

}