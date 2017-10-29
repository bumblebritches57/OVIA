#include "../libPCM.h"

#pragma once

#ifndef LIBPCM_libPCMTypes_H
#define LIBPCM_libPCMTypes_H

#ifdef __cplusplus
extern "C" {
#endif
    
    typedef struct BMPHeader {
        uint32_t FileSize;
        uint16_t Reserved1;
        uint16_t Reserved2;
        uint32_t PixelOffset;
        /* This isn't actually present but we need it */
        uint16_t NumChannels;
        /* This is technically the DIB header */
        uint32_t DIBSize;                   // biSize
        int32_t  Width;                     // biWidth
        int32_t  Height;                    // biHeight
        uint16_t Planes;                    // biPlanes
        uint16_t BitDepth;                  // biBitCount
        uint32_t CompressionType;           // biCompression
        uint32_t ImageSize;                 // biSizeImage
        int32_t  WidthPixelsPerMeter;       // biXPelsPerMeter
        int32_t  HeightPixelsPerMeter;      // biYPelsPerMeter
        uint32_t NumColorsInIndexUsed;      // biClrUsed
        uint32_t NumImportantColorsInIndex; // biClrImportant
        uint32_t RedMask;
        uint32_t BlueMask;
        uint32_t GreenMask;
        uint32_t AlphaMask;
        uint32_t ColorSpaceType;            // bV4CSType
        uint32_t XYZCoordinates[3];         // bV4Endpoints
        uint32_t GammaRed;
        uint32_t GammaGreen;
        uint32_t GammaBlue;
        uint32_t ICCIntent;                 // bV5Intent
        uint32_t ICCProfilePayload;         // bV5ProfileData
        uint32_t ICCProfilePayloadSize;     // bV5ProfileSize
        uint32_t Reserved;
        uint32_t BMPBitMasks;
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