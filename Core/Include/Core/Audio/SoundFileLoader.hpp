#pragma once

#include <string>
#include <vector>

#include <Core/System/Datatypes.hpp>
#include <Core/System/StaticObject.hpp>

namespace Core
{

	struct SoundFileLoader : StaticObject {

		static bool loadWav(const std::string & filepath, u8_t & channels, u8_t & bitsPerSample, i32_t & samplesPerSecond, std::vector<char> & soundData);

	};

}