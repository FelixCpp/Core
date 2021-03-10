#pragma once

#include <vector>

#include <Core/System/Datatypes.hpp>

namespace Core
{

	struct WaveFile {
		/*
		* The "RIFF" chunk descriptor
		* 
		* The Format of convern here is
		* "WAVE", which requires two
		* sub-chunks: "fmt " and "data"
		*/
		char ChunkID[4]; /* Contains the letters "RIFF" in ASCII form (0x52494646 big-endian form) */
		UInt32 ChunkSize; /* This is the size of the rest of the chunk following this number. */
		char Format[4]; /* Contains the letters "WAVE" (0x57415645 big-endian form) */

		/*
		* The "fmt " sub-chunk
		* 
		* descripbes the format of
		* the sound information in
		* the data sub-chunk
		*/
		char Subchunk1ID[4]; /* Contains the letters "fmt " (0x666d7420 big-endian form) */
		UInt32 Subchunk1Size; /* 16 for PCM. This is the size of the rest of the Subchunk which follows this number */
		UInt16 AudioFormat; /* PCM = 1 (i.e. Linear quantization) Values other than 1 indicate some form of compression */
		UInt16 NumChannels; /* Mono = 1, Stereo = 2 etc. */
		UInt32 SampleRate; /* 8000, 44100 etc. */
		UInt32 ByteRate; /* == SampleRate * NumChannels * BitsPerSample / 8 */
		UInt16 BlockAlign; /* == NumChannels * BitsPerSample / 8. The number of bytes for one sample including all channels. */
		UInt16 BitsPerSample; /* 8 bits = 8, 16 bits = 16, etc. */
		
		/*
		* The "data" sub-chunk
		* 
		* Indicates the size of the
		* sound information
		*/
		char Subchunk2ID[4]; /* Contains the letters "data" (0x64617461 big-endian form) */
		UInt32 Subchunk2Size; /* == NumSamples * NumChannels * BitsPerSample / 8. This is the number of bytes in the data. You can also think of this as the size of the read of the subchunk following this number */
		std::vector<char> Data; /* The actual sound data */
	};

}