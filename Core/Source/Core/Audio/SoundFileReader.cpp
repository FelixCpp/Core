#include <Core/Audio/SoundFileReader.hpp>

#include <Core/System/Logger.hpp>

#include <al.h>
#include <inttypes.h>
#include <sndfile.h>

#pragma comment(lib, "libsndfile-1")

namespace Core
{

	bool SoundFileReader::read(const std::string & filepath, OpenALBufferProperties & properties)
	{
		SF_INFO & info = properties.header;
		std::vector<short> & data = properties.data;

		/* open the file */
		SNDFILE * file = sf_open(filepath.c_str(), SFM_READ, &info);
		if (!file)
		{
			CORE_ERROR("Failed to open \"%s\"", filepath.c_str());
			return false;
		}

		/* validate sample count */
		if (info.frames < 1 || info.frames >(sf_count_t)(std::numeric_limits<int>::max() / sizeof(short)) / info.channels)
		{
			CORE_ERROR("Bad sample count in \"%s\" (%" PRId64 ")", filepath.c_str(), info.frames);
			sf_close(file);
			return false;
		}

		/* decode the whole audio for to a buffer */
		const size_t size = (size_t)(info.frames * info.channels) * sizeof(short);
		data.resize(size);

		/* read the data */
		const sf_count_t numFrames = sf_readf_short(file, &data[0], info.frames);
		if (numFrames < 1)
		{
			data.clear();
			sf_close(file);
			CORE_ERROR("Failed to read samples in \"%s\" (%" PRId64 ")", filepath.c_str(), numFrames);
			return false;
		}

		/* close the file */
		sf_close(file);

		return true;
	}

}