#include "../libPCM.h"

#pragma once

#ifndef LIBPCM_libPCMTypes_H
#define LIBPCM_libPCMTypes_H

#ifdef __cplusplus
extern "C" {
#endif
    
    struct PCMMetadata {
        uint8_t      NumTags;
        const char  *ArtistTag;
        const char  *ReleaseDateTag;
        const char  *GenreTag;
        const char  *AlbumTag;
        const char  *SongTitleTag;
        const char  *EncoderTag;
    };
    
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
        
        uint64_t    *Samples;
        PCMMetadata *Meta;
    };
    
#ifdef __cplusplus
}
#endif

#endif /* LIBPCM_libPCMTypes_H */
