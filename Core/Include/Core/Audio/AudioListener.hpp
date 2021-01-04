#pragma once

#include <Core/System/StaticObject.hpp>
#include <Core/Maths/Vector3.hpp>

namespace Core
{

	struct AudioListener : StaticObject {
	
		/* changes the volume of the listener */
		static void setVolume(float volume);

		/* returns the current volume of the listener */
		static float getVolume();

		/* changes the position of the listener */
		static void setPosition(float x, float y, float z);

		/* changes the position of the listener */
		static void setPosition(const FVector3 & position);

		/* returns the current position of the listener */
		static FVector3 getPosition();

		/* changes the velocity of the listener */
		static void setVelocity(float x, float y, float z);
		
		/* changes the velocity of the listener */
		static void setVelocity(const FVector3 & velocity);
		
		/* returns the current velocity of the listener */
		static FVector3 getVelocity();
		
	};

}