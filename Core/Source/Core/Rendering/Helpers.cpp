#include <Core/Rendering/Helpers.hpp>

#include <Core/Maths/Random.hpp>

namespace Core
{

	float random(float min, float max)
	{
		return Random::Get(min, max);
	}

	float random(float max)
	{
		return random(0.f, max);
	}

}