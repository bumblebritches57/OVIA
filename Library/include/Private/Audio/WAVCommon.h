#include "../../libPCM.h"

#pragma once

#ifndef LIBPCM_WAVCommon_H
#define LIBPCM_WAVCommon_H

#ifdef __cplusplus
extern "C" {
#endif
    
    void ReadINFO_IART(PCMFile *PCM, BitBuffer *BitB, uint32_t ChunkSize);
    void ReadINFO_ICRD(PCMFile *PCM, BitBuffer *BitB, uint32_t ChunkSize);
    void ReadINFO_IGNR(PCMFile *PCM, BitBuffer *BitB, uint32_t ChunkSize);
    void ReadINFO_INAM(PCMFile *PCM, BitBuffer *BitB, uint32_t ChunkSize);
    void ReadINFO_IPRD(PCMFile *PCM, BitBuffer *BitB, uint32_t ChunkSize);
    void ReadINFO_ISFT(PCMFile *PCM, BitBuffer *BitB, uint32_t ChunkSize);
    void ParseWavLISTChunk(PCMFile *PCM, BitBuffer *BitB, uint32_t ChunkSize);
    void ParseWavDATAChunk(PCMFile *PCM, BitBuffer *BitB, uint32_t ChunkSize);
    void ParseWavFMTChunk(PCMFile *PCM, BitBuffer *BitB, uint32_t ChunkSize);
    void WAVExtractSamples(PCMFile *PCM, BitBuffer *BitB, uint64_t NumSamples2Extract);
    
#ifdef __cplusplus
}
#endif

#endif /* LIBPCM_WAVCommon_H */
