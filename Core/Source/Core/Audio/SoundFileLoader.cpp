#include <Core/Audio/SoundFileLoader.hpp>

#include <fstream>
#include <iostream>

namespace
{

    Core::i32_t convertToInt(char * buffer, std::size_t len)
    {
        Core::i32_t a = 0;
        std::memcpy(&a, buffer, len);
        return a;
    }

    bool loadWavFileHeader(std::ifstream & reader, Core::u8_t & channels, Core::u8_t & bitsPerSample, Core::i32_t & samplesPerSecond, std::vector<char> & soundData)
    {
        char chunk[4];
        if (!reader.is_open())
            return false;

        // the RIFF
        if (!reader.read(chunk, 4))
        {
            std::cerr << "ERROR: could not read RIFF" << std::endl;
            return false;
        }
        if (std::strncmp(chunk, "RIFF", 4) != 0)
        {
            std::cerr << "ERROR: file is not a valid WAVE file (header doesn't begin with RIFF)" << std::endl;
            return false;
        }

        // the size of the file
        if (!reader.read(chunk, 4))
        {
            std::cerr << "ERROR: could not read size of file" << std::endl;
            return false;
        }

        // the WAVE
        if (!reader.read(chunk, 4))
        {
            std::cerr << "ERROR: could not read WAVE" << std::endl;
            return false;
        }
        if (std::strncmp(chunk, "WAVE", 4) != 0)
        {
            std::cerr << "ERROR: file is not a valid WAVE file (header doesn't contain WAVE)" << std::endl;
            return false;
        }

        // "fmt/0"
        if (!reader.read(chunk, 4))
        {
            std::cerr << "ERROR: could not read fmt/0" << std::endl;
            return false;
        }

        // this is always 16, the size of the fmt data chunk
        if (!reader.read(chunk, 4))
        {
            std::cerr << "ERROR: could not read the 16" << std::endl;
            return false;
        }

        // PCM should be 1?
        if (!reader.read(chunk, 2))
        {
            std::cerr << "ERROR: could not read PCM" << std::endl;
            return false;
        }

        // the number of channels
        if (!reader.read(chunk, 2))
        {
            std::cerr << "ERROR: could not read number of channels" << std::endl;
            return false;
        }

        channels = convertToInt(chunk, 2);

        // sample rate
        if (!reader.read(chunk, 4))
        {
            std::cerr << "ERROR: could not read sample rate" << std::endl;
            return false;
        }

        samplesPerSecond = convertToInt(chunk, 4);

        // (sampleRate * bitsPerSample * channels) / 8
        if (!reader.read(chunk, 4))
        {
            std::cerr << "ERROR: could not read (sampleRate * bitsPerSample * channels) / 8" << std::endl;
            return false;
        }

        // ?? dafaq
        if (!reader.read(chunk, 2))
        {
            std::cerr << "ERROR: could not read dafaq" << std::endl;
            return false;
        }

        // bitsPerSample
        if (!reader.read(chunk, 2))
        {
            std::cerr << "ERROR: could not read bits per sample" << std::endl;
            return false;
        }

        bitsPerSample = convertToInt(chunk, 2);

        // data chunk header "data"
        if (!reader.read(chunk, 4))
        {
            std::cerr << "ERROR: could not read data chunk header" << std::endl;
            return false;
        }
        if (std::strncmp(chunk, "data", 4) != 0)
        {
            std::cerr << "ERROR: file is not a valid WAVE file (doesn't have 'data' tag)" << std::endl;
            return false;
        }

        // size of data
        if (!reader.read(chunk, 4))
        {
            std::cerr << "ERROR: could not read data size" << std::endl;
            return false;
        }
        
        const Core::i32_t soundDataSize = convertToInt(chunk, 4);

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

        // read the data chunk
        soundData.resize(soundDataSize);
        reader.read(&soundData[0], soundData.size());

        return true;
    }

}

namespace Core
{

	bool SoundFileLoader::loadWav(const std::string & filepath, u8_t & channels, u8_t & bitsPerSample, i32_t & samplesPerSecond, std::vector<char> & soundData)
	{
        std::ifstream fileReader(filepath, std::istream::binary);
        if (!fileReader.is_open())
        {
            std::cerr << "Failed to open \"" << filepath << "\"" << std::endl;
            return false;
        }

        const bool success = loadWavFileHeader(fileReader, channels, bitsPerSample, samplesPerSecond, soundData);
        if (!success)
        {
            std::cerr << "Failed to load the wav-header" << std::endl;
            return false;
        }

		return true;
	}

}