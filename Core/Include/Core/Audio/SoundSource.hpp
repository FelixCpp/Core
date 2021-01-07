#pragma once

#include <Core/System/Datatypes.hpp>
#include <Core/Maths/Vector3.hpp>

namespace Core
{

	class SoundSource {
	public:

		enum class State {
			Invalid,
			Initial,
			Playing,
			Paused,
			Stopped
		};

		SoundSource();
		~SoundSource();

		void play(u32_t bufferID);
		void pause();
		void stop();

		void setPosition(float x, float y, float z);
		void setPosition(const FVector3 & position);
		const FVector3 & getPosition() const;

		void setVelocity(float x, float y, float z);
		void setVelocity(const FVector3 & velocity);
		const FVector3 & getVelocity() const;

		void setLooping(bool loop);
		bool isLooping() const;

		void setVolume(float volume);
		float getVolume() const;

		void setPitch(float pitch);
		float getPitch() const;

		State getState() const;
		
	private:

		u32_t sourceID;
		u32_t attachedBufferID;

		float pitch;
		float volume;

		FVector3 position;
		FVector3 velocity;

		bool loop;

	};

}