#include "../../libPCM.h"

#pragma once

#ifndef LIBPCM_WAVCommon_H
#define LIBPCM_WAVCommon_H

#ifdef __cplusplus
extern "C" {
#endif
    
    void       WAVParseMetadata(PCMFile *PCM, BitBuffer *BitB);
    
    uint32_t **WAVExtractSamples(PCMFile *PCM, BitBuffer *BitB, uint64_t NumSamples2Extract);
    
#ifdef __cplusplus
}
#endif

#endif /* LIBPCM_WAVCommon_H */
