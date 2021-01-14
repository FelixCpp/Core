#include <Core/Audio/WaveFileReader.hpp>
#include <Core/System/Logger.hpp>

#include <fstream>

namespace
{
	template<typename T>
	bool _decode(std::ifstream & reader, T & value)
	{
		return (bool)reader.read(reinterpret_cast<char *>(&value), sizeof value);
	}

	bool decode(std::ifstream & reader, char (&id)[4])
	{
		return (bool)reader.read(id, sizeof id);
	}

	bool decode(std::ifstream & reader, uint16_t & value)
	{
		return _decode(reader, value);
	}

	bool decode(std::ifstream & reader, uint32_t & value)
	{
		return _decode(reader, value);
	}

}

namespace Core
{

    bool WaveFileReader::Read(const std::string & filepath, WaveFile & file)
	{
		using namespace std::string_literals;

		/* open the file */
		std::ifstream reader(filepath, std::ios::binary);
		if (!reader.is_open())
		{
			CORE_ERROR("Failed to open \"%s\"", filepath.c_str());
			return false;
		}

		/* read header */
		if (!decode(reader, file.ChunkID))
		{
			CORE_ERROR("Failed to read \"RIFF\" from \"%s\"", filepath.c_str());
			return false;
		}

		if (!decode(reader, file.ChunkSize))
		{
			CORE_ERROR("Failed to read size of the file \"%s\"", filepath.c_str());
			return false;
		}

		if (!decode(reader, file.Format))
		{
			CORE_ERROR("Failed to read \"WAVE\" from \"%s\"", filepath.c_str());
			return false;
		}

		/* read "fmt " chunk */
		if (!decode(reader, file.Subchunk1ID))
		{
			CORE_ERROR("Failed to read \"fmt \" from \"%s\"", filepath.c_str());
			return false;
		}

		if (!decode(reader, file.Subchunk1Size))
		{
			CORE_ERROR("Failed to read size of \"fmt \" chunk from \"%s\"", filepath.c_str());
			return false;
		}

		if (!decode(reader, file.AudioFormat))
		{
			CORE_ERROR("Failed to read the audio format from \"%s\"", filepath.c_str());
			return false;
		}

		if (!decode(reader, file.NumChannels))
		{
			CORE_ERROR("Failed to read the number of channels from \"%s\"", filepath.c_str());
			return false;
		}

		if (!decode(reader, file.SampleRate))
		{
			CORE_ERROR("Failed to read the samplerate from \"%s\"", filepath.c_str());
			return false;
		}

		if (!decode(reader, file.ByteRate))
		{
			CORE_ERROR("Failed to read the byterate from \"%s\"", filepath.c_str());
			return false;
		}

		if (!decode(reader, file.BlockAlign))
		{
			CORE_ERROR("Failed to read the blockalign from \"%s\"", filepath.c_str());
			return false;
		}

		if (!decode(reader, file.BitsPerSample))
		{
			CORE_ERROR("Failed to read the bits per sample from \"%s\"", filepath.c_str());
			return false;
		}

		/* read data chunk */
		if (!decode(reader, file.Subchunk2ID))
		{
			CORE_ERROR("Failed to read \"data\" from \"%s\"", filepath.c_str());
			return false;
		}

		if (!decode(reader, file.Subchunk2Size))
		{
			CORE_ERROR("Failed to read size of \"data\" chunk from \"%s\"", filepath.c_str());
			return false;
		}

		file.Data.resize(file.Subchunk2Size);
		if (!file.Data.empty())
		{
			if (!reader.read(&file.Data[0], file.Subchunk2Size))
			{
				CORE_ERROR("Failed to read the data from \"%s\"", filepath.c_str());
				return false;
			}
		}

		/* close the file */
		reader.close();

		return true;
	}

}