#include <Core/Audio/WaveFileReader.hpp>

#include <Core/System/Logger.hpp>

#include <fstream>

namespace Core
{

	bool WaveFileReader::read(const std::string & filepath, WaveFile & file)
	{
		std::ifstream reader(filepath, std::istream::binary);
		if (!reader.is_open())
		{
			CORE_ERROR("Failed to open \"%s\"", filepath.c_str());
			return false;
		}
        
        /* read in the header informations */
        WaveFileHeader & header = file.header;
        if (!reader.read(reinterpret_cast<char *>(&header), sizeof WaveFileHeader))
        {
            CORE_ERROR("Failed to read the WaveFileHeader");
            return false;
        }
        
        /* validate the header */
        if (!validateWaveHeader(filepath, header))
        {
            return false;
        }

        /* create room for the each sample */
        file.data.resize(header.subchunk2Size);
        
        /* read the data in */
        if (!reader.read(reinterpret_cast<char *>(&file.data[0]), header.subchunk2Size))
        {
            CORE_ERROR("Failed to read in the data");
            return false;
        }

		return true;
	}

    bool WaveFileReader::validateWaveHeader(const std::string & filepath, WaveFileHeader & header)
    {
        if (std::strncmp(header.riff, "RIFF", sizeof header.riff) != 0)
        {
            CORE_ERROR("Failed to read \"RIFF\" in \"%s\"", filepath.c_str());
            return false;
        }

        if (std::strncmp(header.wave, "WAVE", sizeof header.wave) != 0)
        {
            CORE_ERROR("Failed to read \"WAVE\" in \"%s\"", filepath.c_str());
            return false;
        }

        if (std::strncmp(header.subchunk1ID, "fmt ", sizeof header.subchunk1ID) != 0)
        {
            CORE_ERROR("Failed to read \"fmt \" in \"%s\"", filepath.c_str());
            return false;
        }

        if (std::strncmp(header.subchunk2ID, "data", sizeof header.subchunk2ID) != 0)
        {
            CORE_ERROR("Failed to read \"data\" in \"%s\"", filepath.c_str());
            return false;
        }

        return true;
    }

}