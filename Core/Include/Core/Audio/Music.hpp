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
		bool Init(Int32 audioFormat, Int32 sampleRate, const std::vector<char> & soundData);

		/* overrides the setLooping function */
		void SetLooping(bool value);

		/* returns true if the music is looping */
		bool IsLooping() const;

		/* override the play method */
		virtual void Play() override;

		/* continues loading the soundfile */
		void Update();

	private:

		/* buffers so the music can loop */
		void UpdateBufferingLoop(UInt32 buffer);

		/* buffers so the music plays once */
		void UpdateBuffering(UInt32 buffer);

	private:

		/*
			We're using a PImpl (Pointered Implementation)
			pattern so we can include dirty files in the
			cpp so they doesn't spread out into the Program
			which uses this file
		*/
		//struct Impl;
		//std::shared_ptr<Impl> impl;

		Int32 audioFormat;
		Int32 sampleRate;
		std::vector<char> soundData;
		std::size_t cursor;

		bool bufferingFinished;
		bool looping;

	};

}