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

        WaveFileDataChunk & dataChunk = file.dataChunk;

        // data chunk header "data"
        if (!reader.read(dataChunk.data, 4))
        {
            std::cerr << "ERROR: could not read data chunk header" << std::endl;
            return false;
        }
        if (std::strncmp(dataChunk.data, "data", 4) != 0)
        {
            std::cerr << "ERROR: file is not a valid WAVE file (doesn't have 'data' tag)" << std::endl;
            return false;
        }

        // size of data
        char buffer[4];
        if (!reader.read(buffer, 4))
        {
            std::cerr << "ERROR: could not read data size" << std::endl;
            return false;
        }
        std::memcpy(&dataChunk.size, &buffer, sizeof buffer);

        /* cannot be at the end of file */
        if (reader.eof())
        {
            std::cerr << "ERROR: reached EOF on the file" << std::endl;
            return false;
        }
        if (reader.fail())
        {
            std::cerr << "ERROR: fail state set on the file" << std::endl;
            return false;
        }

        /* create room for the each sample */
        dataChunk.samples.resize(dataChunk.size);
         
        /* read the data in */
        if (!reader.read(&dataChunk.samples[0], dataChunk.samples.size()))
        {
            std::cerr << "Failed to read in the data" << std::endl;
            return false;
        }

		return true;
	}

}