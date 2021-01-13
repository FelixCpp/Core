#pragma once

#include <string>

#include <Core/System/StaticObject.hpp>

#include <Core/Audio/WaveFile.hpp>

namespace Core
{

	struct WaveFileReader : StaticObject {
	
		static bool read(const std::string & filepath, WaveFile & file);

	};

}