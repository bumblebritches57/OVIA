#include "libPCM.h"

#pragma once

#ifndef LIBPCM_WAVCOMMON_H
#define LIBPCM_WAVCOMMON_H

#ifdef __cplusplus
extern "C" {
#endif
    
    void ReadINFO_IART(BitBuffer *BitB, PCMFile *PCM, uint32_t ChunkSize);
    void ReadINFO_ICRD(BitBuffer *BitB, PCMFile *PCM, uint32_t ChunkSize);
    void ReadINFO_IGNR(BitBuffer *BitB, PCMFile *PCM, uint32_t ChunkSize);
    void ReadINFO_INAM(BitBuffer *BitB, PCMFile *PCM, uint32_t ChunkSize);
    void ReadINFO_IPRD(BitBuffer *BitB, PCMFile *PCM, uint32_t ChunkSize);
    void ReadINFO_ISFT(BitBuffer *BitB, PCMFile *PCM, uint32_t ChunkSize);
    void ParseWavLISTChunk(BitBuffer *BitB, PCMFile *PCM, uint32_t ChunkSize);
    void ParseWavDATAChunk(BitBuffer *BitB, PCMFile *PCM, uint32_t ChunkSize);
    void ParseWavFMTChunk(BitBuffer *BitB, PCMFile *PCM, uint32_t ChunkSize);
    void WAVExtractSamples(BitBuffer *BitB, PCMFile *PCM, uint64_t NumSamples2Extract);
    
#ifdef __cplusplus
}
#endif

#endif /* LIBPCM_WAVCOMMON_H */
