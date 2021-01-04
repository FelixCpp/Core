#pragma once

#include <Core/System/Datatypes.hpp>

#include <Core/Maths/Vector3.hpp>

namespace Core
{

	class SoundSource {
	public:

		/* an enumeration containing every state of the source */
		enum class State {
			Invalid, /* this value should never be handled */
			Initial, /* the source is in its initial state (not playing yet) */
			Playing, /* the source is currently playing */
			Paused,  /* the source is currently paused */
			Stopped  /* the source was stopped from playing */
		};

	public:

		/* the default constructor */
		SoundSource();

		/* creates a SoundSource based on a bufferID */
		void create(u32_t bufferID);

		/* destroys the SoundSource */
		void destroy();

		/* plays the sound */
		void play();

		/* pauses the sound */
		void pause();

		/* stops the sound */
		void stop();
		
		/* if the parameter is true, the source will play again when its done */
		void setLooping(bool loop);

		/* returns true if the source should loop */
		bool isLooping() const;

		/* returns the current state of the source */
		State getState() const;

		/*
			changes the volume of the source.
			volume should be a value between 0.0 and 1.0
		*/
		void setVolume(float volume);

		/* returns the volume of the source */
		float getValue() const;

		/*
			changes the gain of the source.
			pitch should be a valid greater than 0.0
		*/
		void setPitch(float pitch);

		/* returns the pitch of the source */
		float getPitch() const;

		/* changes the position of the source */
		void setPosition(float x, float y, float z);
		
		/* changes the position of the source */
		void setPosition(const FVector3 & position);

		/* returns the position of the source */
		FVector3 getPosition() const;

		/* changes the velocity of the source */
		void setVelocity(float x, float y, float z);

		/* changes the velocity of the source */
		void setVelocity(const FVector3 & velocity);

		/* returns the velocity of the source */
		FVector3 getVelocity() const;
		
	private:

		/* An internal OpenAL SourceID */
		u32_t sourceID;

	};

	/* a handy typedef for the user */
	typedef SoundSource Sound;

}