#pragma once

#include <Core/System/StaticObject.hpp>

namespace Core
{

	/**
	* Distance models
	*
	* used in conjunction with DistanceModel
	*
	* implicit: NONE, which disances distance attenuation.
	*/
	enum class DistanceModel {
		InverseDistance			= 0xD001,
		InverseDistanceClamped	= 0xD002,
		LinearDistance			= 0xD003,
		LinearDistanceClamped	= 0xD004,
		ExponentDistance		= 0xD005,
		ExponentDistanceClamped = 0xD006
	};

	class SoundSpace : StaticObject {
	public:

		/*
		* Distance models
		*
		* used in conjunction with DistanceModel
		*
		* implicit: NONE, which disances distance attenuation.
		*/

		/* changes the distance model */
		static void setDistanceModel(DistanceModel model);

		/* returns the current distance model */
		static DistanceModel getDistanceModel();

		/*
		* -- Doppler factor --
		* Doppler scale.  Default 1.0
		*/

		/* changes the doppler factor */
		static void setDopplerFactor(float value);

		/* returns the doppler factor */
		static float getDopplerFactor();

		/*
		* -- Doppler velocity --
		* Tweaks speed of propagation.
		*/

		/* changes the dopplers velocity */
		static void setDopplerVelocity(float value);

		/* returns the dopplers velocity */
		static float getDopplerVelocity();

		/* 
		* -- Speed of sound --
		* Speed of Sound in units per second
		*/

		/* changes the speed of sound */
		static void setSpeedOfSound(float value);

		/* returns the speed of sound */
		static float getSpeedOfSound();

	private:
				
	};

}