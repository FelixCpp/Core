#pragma once

#include <vector>
#include <Core/System/Datatypes.hpp>

#include <sndfile.h>

namespace Core
{

	/*
		This structure contains all data
		from a file to fill an OpenAL Buffer
	*/
	struct OpenALBufferProperties {
		SF_INFO header;			 // header of the Soundfile
		std::vector<short> data; // audio data from the Soundfile
	};

}