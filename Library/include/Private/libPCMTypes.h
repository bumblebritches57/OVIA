#include "../libPCM.h"

#pragma once

#ifndef LIBPCM_libPCMTypes_H
#define LIBPCM_libPCMTypes_H

#ifdef __cplusplus
extern "C" {
#endif
    
    typedef struct AUDMetadata {
        const char      **AnnoChunks;
        const char       *ArtistTag;
        const char       *ReleaseDateTag;
        const char       *GenreTag;
        const char       *AlbumTag;
        const char       *SongTitleTag;
        const char       *EncoderTag;
        uint32_t          NumANNOChunks;
        uint8_t           NumTags;
        uint8_t           ArtistTagSize;
        uint8_t           ReleaseDataSize;
        uint8_t           GenreSize;
        uint8_t           AlbumSize;
        uint8_t           TitleSize;
        uint8_t           EncoderSize;
    } AUDMetadata;
    
    typedef struct AUDHeader {
        AUDMetadata      *Meta;
        uint64_t          SampleRate;
        uint64_t          ChannelMask;
        uint32_t          AIFOffset;
        uint32_t          AIFBlockSize;
        uint16_t          WAVCompressionFormat;
        uint16_t          WAVAvgBytesPerSecond;
        uint8_t           BlockAlignment;
        uint8_t           FormatType;
    } AUDHeader;
    
    typedef struct PICHeader {
        uint64_t          NumBytesUsedBySamples;
        uint64_t          Width;
        uint64_t          Height;
        uint64_t          ChannelType;
        uint64_t          BMPPixelOffset;
        uint32_t          BMPColorsIndexed;
        uint32_t          BMPIndexColorsUsed;
        uint32_t          BMPColorSpaceType;
        uint32_t          BMPXYZCoordinates[3];
        uint32_t          BMPRedMask;
        uint32_t          BMPBlueMask;
        uint32_t          BMPGreenMask;
        uint32_t          BMPAlphaMask;
        uint32_t          BMPRedGamma;
        uint32_t          BMPGreenGamma;
        uint32_t          BMPBlueGamma;
        uint32_t          BMPICCIntent;
        uint32_t          BMPICCPayload;
        uint32_t          BMPICCPayloadSize;
        int32_t           BMPWidthPixelsPerMeter;
        int32_t           BMPHeightPixelsPerMeter;
        uint16_t          BMPCompressionType;
        uint8_t           PXMTupleType;
        uint8_t          *BMPPalette;
        bool              ImageIsTopDown;
        PXMTypes          PXMType;
    } PICHeader;
    
    struct PCMFile {
        AUDHeader        *Aud;
        PICHeader        *Pic;
        uint64_t          FileSize;
        uint64_t          NumChannelAgnosticSamples;
        uint8_t           BitDepth;
        uint8_t           NumChannels;
        libPCMFileFormats InputFileType;
        libPCMFileFormats OutputFileType;
    };
    
#ifdef __cplusplus
}
#endif

#endif /* LIBPCM_libPCMTypes_H */
