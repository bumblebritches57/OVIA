#include "../libPCM.h"

#pragma once

#ifndef LIBPCM_libPCMTypes_H
#define LIBPCM_libPCMTypes_H

#ifdef __cplusplus
extern "C" {
#endif
    
    typedef struct BMPHeader {
        uint16_t Magic;
        uint32_t FileSize;
        uint16_t Reserved1;
        uint16_t Reserved2;
        uint32_t PixelOffset;
        /* This is technically the DIB header */
        uint32_t DIBType;
        int32_t  Width;
        int32_t  Height;
        uint16_t BiPlanes;
        uint16_t BitDepth;
        uint32_t CompressionType;
        uint32_t ImageSize;
        int32_t  WidthPixelsPerMeter;
        int32_t  HeightPixelsPerMeter;
        uint32_t NumColorsInIndexUsed;
        uint32_t NumImportantColorsInIndex;
        uint32_t RedMask;
        uint32_t BlueMask;
        uint32_t GreenMask;
        uint32_t AlphaMask;
        uint32_t ColorSpaceType; // bV4CSType
        uint32_t XYZCoordinates[3]; // bV4Endpoints
        uint32_t GammaRed;
        uint32_t GammaGreen;
        uint32_t GammaBlue;
        uint32_t ICCIntent; // bV5Intent
        uint32_t ICCProfilePayload; // bV5ProfileData
        uint32_t ICCProfilePayloadSize; // bV5ProfileSize
        uint32_t Reserved;
    } BMPHeader;
    
    typedef struct PXMHeader {
        uint64_t Width;
        uint64_t Height;
        uint64_t NumChannels;
        uint64_t MaxVal;
    } PXMHeader;
    /*
 	struct AudioMetadata {
        uint8_t      NumTags;
        const char  *ArtistTag;
        const char  *ReleaseDateTag;
        const char  *GenreTag;
        const char  *AlbumTag;
        const char  *SongTitleTag;
        const char  *EncoderTag;
    };
    
    typedef struct PCMMetadata PCMMetadata;
    
 	struct PCMFile {
        bool         MetaIsPresent;
        uint8_t      WAVW64FormatType;
        uint8_t      FileFormat;
        uint8_t      BlockAlignment;
        uint64_t     NumSamples;
        uint64_t     FileSize;
        
        uint64_t     BitDepth;
        uint64_t     SampleRate;
        uint64_t     NumChannels;
        uint64_t     ChannelMask;
        
        uint64_t   **Samples;
        PCMMetadata *Meta;
    };
    */
    typedef struct AUDMetadata {
        uint8_t      NumTags;
        const char  *ArtistTag;
        const char  *ReleaseDateTag;
        const char  *GenreTag;
        const char  *AlbumTag;
        const char  *SongTitleTag;
        const char  *EncoderTag;
    } AUDMetadata;
    
    typedef struct AUDHeader {
        uint8_t      FormatType;
        uint8_t      BlockAlignment;
        uint64_t     FileSize;
        uint64_t     NumSamples;
        
        uint64_t     BitDepth;
        uint64_t     SampleRate;
        uint64_t     NumChannels;
        uint64_t     ChannelMask;
        AUDMetadata *Meta;
    } AUDHeader;
    
    struct PCMFile {
        uint8_t    PCMFileType;
        BMPHeader *BMP;
        PXMHeader *PXM;
        AUDHeader *AUD;
    };
    
#ifdef __cplusplus
}
#endif

#endif /* LIBPCM_libPCMTypes_H */
