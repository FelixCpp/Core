#pragma once

#include <vector>

#include <Core/System/Datatypes.hpp>

namespace Core
{

	struct WaveFileHeader {
        /* RIFF Chunk Descriptor */
        char  riff[4];        // RIFF Header Magic header
        u32_t chunkSize;      // RIFF Chunk Size
        char  wave[4];        // WAVE Header

        /* "fmt" sub-chunk */
        char  fmt[4];         // FMT header
        u32_t subchunk1Size;  // Size of the fmt chunk
        u16_t audioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
        u16_t channels;       // Number of channels 1=Mono 2=Sterio
        u32_t samplesPerSec;  // Sampling Frequency in Hz
        u32_t bytesPerSec;    // bytes per second
        u16_t blockAlign;     // 2=16-bit mono, 4=16-bit stereo
        u16_t bitsPerSample;  // Number of bits per sample
	};

    struct WaveFileDataChunk {
        /* "data" sub-chunk */
        char  data[4];              // "data"  string
        u32_t size;                 // Sampled data length
        std::vector<char> samples;  // the actual data
    };

    struct WaveFile {
        WaveFileHeader header;
        WaveFileDataChunk dataChunk;
    };

}