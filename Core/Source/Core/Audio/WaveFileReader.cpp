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

        /* create room for the each sample */
        file.data.resize(header.subchunk2Size);
        
        /* read the data in */
        if (!reader.read(&file.data[0], file.data.size()))
        {
            CORE_ERROR("Failed to read in the data");
            return false;
        }

		return true;
	}

}