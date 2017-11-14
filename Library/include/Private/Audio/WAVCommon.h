#include "../../libPCM.h"

#pragma once

#ifndef LIBPCM_WAVCommon_H
#define LIBPCM_WAVCommon_H

#ifdef __cplusplus
extern "C" {
#endif
    
    enum WAVFormatCode {
        PCM = 1,
    };
    
    enum WAVChunkIDs { // odd sized chunks have a trailing 0 byte, and strings are null padded, this is for all RIFF based formats.
        WAV_RIFF              = 0x52494646,
        WAV_WAVE              = 0x57415645,
        WAV_FMT               = 0x666D7420,
        WAV_LIST              = 0x4C495354,
        WAV_INFO              = 0x494E464F,
        WAV_IART              = 0x49415254, // Artist
        WAV_ICRD              = 0x49435244, // Release data
        WAV_IGNR              = 0x49474E52, // Genre
        WAV_INAM              = 0x494E414D, // Song name aka title
        WAV_IPRD              = 0x49505244, // Album name
        WAV_IPRT              = 0x49505254, // Track number
        WAV_ISFT              = 0x49534654, // Software that created it?
        WAV_CODE              = 0x434f4445, // Encoder
        WAV_ICMT              = 0x49434d54, // Comment
        WAV_ICOP              = 0x49434f50, // Copyright
        WAV_YEAR              = 0x59454152, // Year
        WAV_TRCK              = 0x5452434b, // Track
        WAV_DATA              = 0x64617461,
    };
    
    void       WAVParseMetadata(PCMFile *PCM, BitBuffer *BitB);
    
    uint32_t **WAVExtractSamples(PCMFile *PCM, BitBuffer *BitB, uint64_t NumSamples2Extract);
    
    void       WAVInsertSamples(PCMFile *PCM, BitBuffer *OutputSamples, uint32_t NumSamples2Write, uint32_t **Samples2Write);
    
#ifdef __cplusplus
}
#endif

#endif /* LIBPCM_WAVCommon_H */
