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
		void Create(u32_t bufferID);

		/* plays the source and attaches a buffer to it */
		virtual void Play();

		/* paueses the source. If you call play() again it will continue */
		void Pause();

		/* stops the source from playing. If you call play() again it will restart from the beginning */
		void Stop();

		/* changes the position in a 3D space */
		void SetPosition(float x, float y, float z);

		/* changes the position in a 3D space */
		void SetPosition(const FVector3 & position);

		/* returns the current position */
		FVector3 GetPosition() const;

		/* changes the velocity */
		void SetVelocity(float x, float y, float z);

		/* changes the velocity */
		void SetVelocity(const FVector3 & velocity);

		/* returns the velocity */
		FVector3 GetVelocity() const;

		/* changes the direction */
		void SetDirection(float x, float y, float z);

		/* changes the direction */
		void SetDirection(const FVector3 & direction);

		/* returns the direction */
		FVector3 GetDirection() const;

		/* changes wether the source is played relative compared to the listener or not */
		void SetRelativeToListener(bool relative);

		/* returns true if the source is relative compared to the listener */
		bool IsRelativeToListener() const;

		/* changes the volume. The parameter should have a value between 0.0 and 1.0 */
		void SetVolume(float volume);

		/* returns the volume */
		float GetVolume() const;

		/* changes the pitch. The paramter should have a value greater than 0.0 */
		void SetPitch(float pitch);

		/* returns the pitch */
		float GetPitch() const;

		/* changes the rolloff factor */
		void SetRolloffFactor(float factor);

		/* returns the rolloff factor */
		float GetRolloffFactor() const;

		/* changes the reference distance */
		void SetReferenceDistance(float distance);

		/* returns the reference distance */
		float GetReferenceDistance() const;

		/* changes the max distance between the sound and the listener where the sound stops playing */
		void SetMaxDistance(float distance);

		/* returns the max distance */
		float GetMaxDistance() const;

		/* gets the state and returns it. */
		State GetState() const;

		/* a getter for the sourceID */
		u32_t GetSourceID() const;

	protected:

		/* internal OpenAL soure id */
		u32_t sourceID;

	};

}