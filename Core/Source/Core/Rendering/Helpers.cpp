#include <Core/Rendering/Helpers.hpp>

#include <Core/Maths/Random.hpp>
#include <Core/Maths/Math.hpp>

namespace Core
{

	constexpr float radians(float degrees)
	{
		return degrees * 180.f / FMath::Pi;
	}

	constexpr float degrees(float radians)
	{
		return radians * FMath::Pi / 180.f;
	}

	float random(float min, float max)
	{
		return Random::get(min, max);
	}

	float random(float max)
	{
		return random(0.f, max);
	}

}