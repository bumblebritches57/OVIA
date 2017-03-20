#include "libPCM.h"

#ifndef LIBPCM_WAVCOMMON_H
#define LIBPCM_WAVCOMMON_H

#pragma once

#ifdef __cplusplus
extern "C" {
#endif
    
    typedef struct WAVHeader {
        uint32_t RIFF;
        uint32_t RIFFSize; // Everything except RIFF or RIFF size aka -8
        uint32_t WAVE;
        uint32_t FMT;
        uint32_t FMTSize; // Size of the format chunk
        uint16_t FMTType;
        uint16_t CompressionAlgorithm;
        uint16_t Channels;
        // START WAVE_FORMAT_EXTENSIBLE
        uint16_t ValidBitsPerSample;
        uint32_t SpeakerMask; // is actually 18 bits long
                              // END WAVE_FORMAT_EXTENSIBLE
        uint32_t SampleRate;
        uint32_t ByteRate;
        uint16_t BlockSize; // Padding
        uint16_t BitDepth; // BitDepth
        uint16_t ExtensionSize;
        uint32_t DATAChunk;
        uint32_t AudioDataSize;
        uint64_t SampleCount;
        uint8_t  HeaderSize;
        PCMMetadata *Meta;
    };
    
    enum WAVChunkIDs { // odd sized chunks have a trailing 0 byte, and strings are null padded
        WAV_RIFF = 0x52494646,
        WAV_WAVE = 0x57415645,
        WAV_FMT  = 0x666D7420,
        WAV_LIST = 0x4C495354,
        WAV_INFO = 0x494E464F,
        WAV_IART = 0x49415254, // Artist
        WAV_ICRD = 0x49435244, // Release data
        WAV_IGNR = 0x49474E52, // Genre
        WAV_INAM = 0x494E414D, // Song name aka title
        WAV_IPRD = 0x49505244, // Album name
        WAV_IPRT = 0x49505254, // Track number
        WAV_ISFT = 0x49534654, // Software that created it?
        WAV_CODE = 0x434f4445, // Encoder
        WAV_ICMT = 0x49434d54, // Comment
        WAV_ICOP = 0x49434f50, // Copyright
        WAV_YEAR = 0x59454152, // Year
        WAV_TRCK = 0x5452434b, // Track
        WAV_DATA = 0x64617461,
    };
    
    void ReadINFO_IART(BitInput *BitI, PCMFile *PCM, uint32_t ChunkSize);
    void ReadINFO_ICRD(BitInput *BitI, PCMFile *PCM, uint32_t ChunkSize);
    void ReadINFO_IGNR(BitInput *BitI, PCMFile *PCM, uint32_t ChunkSize);
    void ReadINFO_INAM(BitInput *BitI, PCMFile *PCM, uint32_t ChunkSize);
    void ReadINFO_IPRD(BitInput *BitI, PCMFile *PCM, uint32_t ChunkSize);
    void ReadINFO_ISFT(BitInput *BitI, PCMFile *PCM, uint32_t ChunkSize);
    void ParseWavLISTChunk(BitInput *BitI, PCMFile *PCM, uint32_t ChunkSize);
    void ParseWavDATAChunk(BitInput *BitI, PCMFile *PCM, uint32_t ChunkSize);
    void ParseWavFMTChunk(BitInput *BitI, PCMFile *PCM, uint32_t ChunkSize);
    void WAVExtractSamples(BitInput *BitI, PCMFile *PCM, uint32_t NumSamples2Extract);
    
#ifdef __cplusplus
}
#endif

#endif /* LIBPCM_WAVCOMMON_H */
