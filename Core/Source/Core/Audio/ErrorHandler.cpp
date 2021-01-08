#include <Core/Audio/ErrorHandler.hpp>

#include <Core/System/Logger.hpp>
#include <al.h>

namespace Core
{

	bool check_al_errors(const std::string & filename, std::uint_fast32_t line)
	{
		ALenum error = alGetError();
		if (error != AL_NO_ERROR)
		{
			CORE_ERROR("Error in \"%s\" on line %d", filename.c_str(), line);

			switch (error)
			{
				case AL_INVALID_NAME: CORE_ERROR("AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function"); break;
				case AL_INVALID_ENUM: CORE_ERROR("AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function"); break;
				case AL_INVALID_VALUE: CORE_ERROR("AL_INVALID_VALUE: an invalid value was passed to an OpenAL function"); break;
				case AL_INVALID_OPERATION: CORE_ERROR("AL_INVALID_OPERATION: the requested operation is not valid"); break;
				case AL_OUT_OF_MEMORY: CORE_ERROR("AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory"); break;
				default: CORE_ERROR("Unknown OpenAL Error"); break;
			}

			return false;
		}

		return true;
	}

}