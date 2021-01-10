#pragma once

#include <Core/Maths/Vector3.hpp>
#include <Core/System/StaticObject.hpp>

namespace Core
{

	struct Listener : public StaticObject {
	
		/* changes the position in a 3D space */
		static void setPosition(float x, float y, float z);

		/* changes the position in a 3D space */
		static void setPosition(const FVector3 & position);

		/* returns the current position */
		static FVector3 getPosition();

		/* changes the velocity */
		void setVelocity(float x, float y, float z);

		/* changes the velocity */
		void setVelocity(const FVector3 & velocity);

		/* returns the velocity */
		static FVector3 getVelocity();

		/* changes the orientation */
		static void setOrientation(float px, float py, float pz, float upx, float upy, float upz);

		/* changes the orientation */
		static void setOrientation(const FVector3 & position, const FVector3 & up);

		/* returns the orientation (an array with the length of 6 elements: float[6]) */
		static float * getOrientation();

		/* changes the volume. The parameter should have a value between 0.0 and 1.0 */
		static void setVolume(float volume);

		/* returns the volume */
		static float getVolume();

	};

}