#pragma once

#include <Core/Maths/Vector3.hpp>
#include <Core/System/StaticObject.hpp>

namespace Core
{

	struct Listener : public StaticObject {
	
		/* changes the position in a 3D space */
		static void SetPosition(float x, float y, float z);

		/* changes the position in a 3D space */
		static void SetPosition(const FVector3 & position);

		/* returns the current position */
		static FVector3 GetPosition();

		/* changes the velocity */
		static void SetVelocity(float x, float y, float z);

		/* changes the velocity */
		static void SetVelocity(const FVector3 & velocity);

		/* returns the velocity */
		static FVector3 GetVelocity();

		/* changes the orientation */
		static void SetOrientation(float px, float py, float pz, float upx, float upy, float upz);

		/* changes the orientation */
		static void SetOrientation(const FVector3 & position, const FVector3 & up);

		/* returns the orientation (an array with the length of 6 elements: float[6]) */
		static void GetOrientation(float (&orientation)[6]);

		/* changes the volume. The parameter should have a value between 0.0 and 1.0 */
		static void SetVolume(float volume);

		/* returns the volume */
		static float GetVolume();

	};

}