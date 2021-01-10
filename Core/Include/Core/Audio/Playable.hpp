#pragma once

#include <Core/Maths/Vector3.hpp>
#include <Core/System/Datatypes.hpp>

namespace Core
{

	class Playable {
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
		Playable();

		/* default destructor. NOTE: It doesn't destroy the OpenAL resource */
		virtual ~Playable() = default;

		/* creates an OpenAL source attached to a buffer */
		void create(u32_t bufferID);

		/* plays the source and attaches a buffer to it */
		virtual void play();

		/* paueses the source. If you call play() again it will continue */
		void pause();

		/* stops the source from playing. If you call play() again it will restart from the beginning */
		void stop();

		/* changes the position in a 3D space */
		void setPosition(float x, float y, float z);

		/* changes the position in a 3D space */
		void setPosition(const FVector3 & position);

		/* returns the current position */
		FVector3 getPosition() const;

		/* changes the velocity */
		void setVelocity(float x, float y, float z);

		/* changes the velocity */
		void setVelocity(const FVector3 & velocity);

		/* returns the velocity */
		FVector3 getVelocity() const;

		/* changes the direction */
		void setDirection(float x, float y, float z);

		/* changes the direction */
		void setDirection(const FVector3 & direction);

		/* returns the direction */
		FVector3 getDirection() const;

		/* changes wether the source is played relative compared to the listener or not */
		void setRelativeToListener(bool relative);

		/* returns true if the source is relative compared to the listener */
		bool isRelativeToListener() const;

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

		/* changes the rolloff factor */
		void setRolloffFactor(float factor);

		/* returns the rolloff factor */
		float getRolloffFactor() const;

		/* changes the reference distance */
		void setReferenceDistance(float distance);

		/* returns the reference distance */
		float getReferenceDistance() const;

		/* changes the max distance between the sound and the listener where the sound stops playing */
		void setMaxDistance(float distance);

		/* returns the max distance */
		float getMaxDistance() const;

		/* gets the state and returns it. */
		State getState() const;

		/* a getter for the sourceID */
		u32_t getSourceID() const;

	protected:

		/* internal OpenAL soure id */
		u32_t sourceID;

	};

}