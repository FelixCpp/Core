#pragma once

#include <Core/System/Datatypes.hpp>

namespace Core
{

	class SoundBuffer;

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

		static SoundSource create(const SoundBuffer & attachedTo);
		
		void play();
		void pause();
		void stop();

		void setPitch(float pitch);
		float getPitch() const;
		
		void setLooping(bool loop);
		bool isLooping() const;

		void setVolume(i32_t volume);
		i32_t getVolume() const;

		State getState() const;

		const u32_t & getSourceID() const;

	private:

		u32_t sourceID;

	};

	typedef SoundSource Sound;

}