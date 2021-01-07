#pragma once

#include <Core/System/Datatypes.hpp>
#include <Core/Maths/Vector3.hpp>

namespace Core
{

	class SoundSource {
	public:

		/* the different states of a SoundSource */
		enum class State {
			Invalid, // this state should never be active
			Initial, // initial state
			Playing, // the source is currently playing
			Paused,	 // the source is paused and ready to continue
			Stopped  // the source was stopped and ready to start from the beginning
		};

		/* default constructor */
		SoundSource();

		/* creates an OpenAL source attached to a buffer */
		static SoundSource create(u32_t bufferID);

		/* plays the source and attaches a buffer to it */
		void play();

		/* paueses the source. If you call play() again it will continue */
		void pause();

		/* stops the source from playing. If you call play() again it will restart from the beginning */
		void stop();

		/* changes the position in a 3D space */
		void setPosition(float x, float y, float z);
		
		/* changes the position in a 3D space */
		void setPosition(const FVector3 & position);

		/* returns the current position */
		const FVector3 & getPosition() const;

		/* changes the velocity */
		void setVelocity(float x, float y, float z);
		
		/* changes the velocity */
		void setVelocity(const FVector3 & velocity);
		
		/* returns the velocity */
		const FVector3 & getVelocity() const;

		/* loops the source after finishing playing */
		void setLooping(bool loop);

		/* if this returns true it means that the source is playing again after finishing */
		bool isLooping() const;

		/* changes the volume. The parameter should have a value between 0.0 and 1.0 */
		void setVolume(float volume);

		/* returns the volume */
		float getVolume() const;

		/* changes the pitch. The paramter should have a value greater than 0.0 */
		void setPitch(float pitch);
		
		/* returns the pitch */
		float getPitch() const;

		/* gets the state and returns it. */
		State getState() const;
		
		/* a getter for the sourceID */
		u32_t getSourceID() const;

	private:

		/* source identifier */
		u32_t sourceID;

		/* pitch */
		float pitch;

		/* volume */
		float volume;

		/* position in a 3D space */
		FVector3 position;

		/* velocity */
		FVector3 velocity;

		/* looping state */
		bool loop;

	};

}