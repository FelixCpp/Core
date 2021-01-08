#pragma once

#include <string>

#include <Core/System/StaticObject.hpp>

#include <Core/Audio/WaveFile.hpp>

namespace Core
{

	class WaveFileReader : StaticObject {
	public:

		static bool read(const std::string & filepath, WaveFile & file);
	
	private:

		static bool validateWaveHeader(const std::string & filepath, WaveFileHeader & header);

	};

}