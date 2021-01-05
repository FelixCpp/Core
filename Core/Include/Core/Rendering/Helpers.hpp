#pragma once

#include <Core/System/Datatypes.hpp>
#include <Core/System/StaticObject.hpp>

namespace Core
{

	constexpr float radians(float degrees);
	constexpr float degrees(float radians);
	float random(float min, float max);
	float random(float max);

}