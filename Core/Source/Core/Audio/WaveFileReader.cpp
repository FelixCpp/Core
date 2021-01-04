#include <Core/Audio/WaveFileReader.hpp>

#include <fstream>
#include <iostream>

namespace Core
{

	bool WaveFileReader::read(const std::string & filepath, WaveFile & file)
	{
		std::ifstream reader(filepath, std::istream::binary);
		if (!reader.is_open())
		{
			std::cerr << "Failed to open \"" << filepath << "\"" << std::endl;
			return false;
		}

        /* read in the header informations */
        WaveFileHeader & header = file.header;
        if (!reader.read(reinterpret_cast<char *>(&header), sizeof WaveFileHeader))
        {
            std::cerr << "Failed to read the WaveFileHeader" << std::endl;
            return false;
        }

        /* create room for the each sample */
        file.data.resize(header.subchunk2Size);
        
        /* read the data in */
        if (!reader.read(&file.data[0], file.data.size()))
        {
            std::cerr << "Failed to read in the data" << std::endl;
            return false;
        }

		return true;
	}

}