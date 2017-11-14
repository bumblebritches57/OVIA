#include "../libPCM.h"

#pragma once

#ifndef LIBPCM_libPCMTypes_H
#define LIBPCM_libPCMTypes_H

#ifdef __cplusplus
extern "C" {
#endif
    
    typedef struct AUDMetadata {
        uint8_t           NumTags;
        uint8_t           ArtistTagSize;
        uint8_t           ReleaseDataSize;
        uint8_t           GenreSize;
        uint8_t           AlbumSize;
        uint8_t           TitleSize;
        uint8_t           EncoderSize;
        uint32_t          NumANNOChunks;
        const char       *ArtistTag;
        const char       *ReleaseDateTag;
        const char       *GenreTag;
        const char       *AlbumTag;
        const char       *SongTitleTag;
        const char       *EncoderTag;
        const char      **AnnoChunks;
    } AUDMetadata;
    
    typedef struct AUDHeader {
        uint64_t          SampleRate;
        uint64_t          ChannelMask;
        uint16_t          WAVCompressionFormat;
        uint16_t          WAVAvgBytesPerSecond;
        uint32_t          AIFOffset;
        uint32_t          AIFBlockSize;
        uint8_t           BlockAlignment;
        uint8_t           FormatType;
        AUDMetadata      *Meta;
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
        PXMTypes          PXMType;
    } PICHeader;
    
    struct PCMFile {
        libPCMFileFormats InputFileType;
        libPCMFileFormats OutputFileType;
        uint64_t          FileSize;
        uint64_t          BitDepth;
        uint64_t          NumChannels;
        uint64_t          NumChannelAgnosticSamples;
        AUDHeader        *AUD;
        PICHeader        *PIC;
    };
    
#ifdef __cplusplus
}
#endif

#endif /* LIBPCM_libPCMTypes_H */
