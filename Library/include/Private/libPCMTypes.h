#include "../libPCM.h"

#pragma once

#ifndef LIBPCM_libPCMTypes_H
#define LIBPCM_libPCMTypes_H

#ifdef __cplusplus
extern "C" {
#endif
    
    typedef struct PCMMetadata {
        uint8_t      NumTags;
        const char  *ArtistTag;
        const char  *ReleaseDateTag;
        const char  *GenreTag;
        const char  *AlbumTag;
        const char  *SongTitleTag;
        const char  *EncoderTag;
    } PCMMetadata;
    
    typedef struct WAVHeader {
        uint32_t     RIFF;
        uint32_t     RIFFSize; // Everything except RIFF or RIFF size aka -8
        uint32_t     WAVE;
        uint32_t     FMT;
        uint32_t     FMTSize; // Size of the format chunk
        uint16_t     FMTType;
        uint16_t     CompressionAlgorithm;
        uint16_t     Channels;
        // START WAVE_FORMAT_EXTENSIBLE
        uint16_t     ValidBitsPerSample;
        uint32_t     SpeakerMask; // is actually 18 bits long
                              // END WAVE_FORMAT_EXTENSIBLE
        uint32_t     SampleRate;
        uint32_t     ByteRate;
        uint16_t     BlockSize; // Padding
        uint16_t     BitDepth; // BitDepth
        uint16_t     ExtensionSize;
        uint32_t     DATAChunk;
        uint32_t     AudioDataSize;
        uint64_t     SampleCount;
        uint8_t      HeaderSize;
    } WAVHeader;
    
    typedef struct PCMFile {
        bool         MetadataHasBeenParsed;
        bool         FilledRequest; // Were there enough samples left to fill the request?
        uint8_t      FileType;
        uint64_t     NumSamples;    // Channel agnostic
        uint64_t     SampleRate;
        uint64_t     BitDepth;
        uint64_t     NumChannels;
        uint32_t     ChannelMask;
        WAVHeader   *WAV;
        PCMMetadata *Meta;
        uint64_t   **SamplesOrLines;
    } PCMFile;
    
    typedef struct AIFHeader {
        uint16_t     Channels;
        uint32_t     NumFrames; // More accurately described as number of channel agnostic samples.
        uint16_t     BitDepth; // Samples are padded to the next complete byte
        
        /* SNSD */
        uint32_t     Offset;
        uint32_t     BlockSize;
        
        PCMMetadata *Meta;
    } AIFHeader;
    
    typedef struct W64Header {
        uint16_t     FormatType;
        uint16_t     Channels;
        uint32_t     SampleRate;
        uint32_t     ByteRate;
        uint16_t     BlockAlign;
        uint16_t     BitDepth;
        uint64_t     DataSize;
        uint64_t     NumSamples; // Channel independent
    } W64Header;
    
    enum AIFChunkIDs {
        AIF_FORM = 0x464F524D,
        AIF_AIFF = 0x41494646,
        AIF_AIFC = 0x41494643,
        AIF_NAME = 0x4E414D45,
        AIF_COMM = 0x434F4D4D,
        AIF_SSND = 0x53534E44,
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
    
    enum W64ChunkIDs {
        W64_RIFF = 0x72696666,
        W64_WAVE = 0x77617665,
        W64_FMT  = 0x666D7420,
        W64_DATA = 0x64617461,
        W64_LEVL = 0x6C76656C,
        W64_JUNK = 0x6B6E756A,
        W64_BEXT = 0x74786562,
        W64_FACT = 0x74636166,
        W64_LIST = 0x7473696C,
        W64_MRKR = 0xABF76256,
        W64_SUMM = 0x925F94BC,
    };
    
    enum BMPMagic {
        BMP_BM   = 0x424D,
        
    };
    
    enum PXMMagic {
        PXM_P1   = 0x5031,
        PXM_P2   = 0x5032,
        PXM_P3   = 0x5033,
        PXM_P4   = 0x5034,
        PXM_P5   = 0x5035,
        PXM_P6   = 0x5036,
        PXM_P7   = 0x5037,
    };
    
#ifdef __cplusplus
}
#endif

#endif /* LIBPCM_libPCMTypes_H */
