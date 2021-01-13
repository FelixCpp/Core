#pragma once

#include <Core/System/Datatypes.hpp>
#include <Core/Maths/Vector3.hpp>

#include <Core/Audio/Playable.hpp>

namespace Core
{

	struct Sound : Playable {

		/* loops the source after finishing playing */
		void setLooping(bool loop);

		/* if this returns true it means that the source is playing again after finishing */
		bool isLooping() const;

	};

}