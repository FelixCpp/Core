#pragma once

#include <string>
#include <vector>

#include <Core/Audio/OpenALBufferProperties.hpp>

#include <Core/System/StaticObject.hpp>

namespace Core
{

	struct SoundFileReader : StaticObject {
		
		static bool read(const std::string & filepath, OpenALBufferProperties & properties);

	};

}