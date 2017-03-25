#include "libPCM.h"

#pragma once

#ifndef LIBPCM_WAVCOMMON_H
#define LIBPCM_WAVCOMMON_H

#ifdef __cplusplus
extern "C" {
#endif
    
    void ReadINFO_IART(BitInput *BitI, PCMFile *PCM, uint32_t ChunkSize);
    void ReadINFO_ICRD(BitInput *BitI, PCMFile *PCM, uint32_t ChunkSize);
    void ReadINFO_IGNR(BitInput *BitI, PCMFile *PCM, uint32_t ChunkSize);
    void ReadINFO_INAM(BitInput *BitI, PCMFile *PCM, uint32_t ChunkSize);
    void ReadINFO_IPRD(BitInput *BitI, PCMFile *PCM, uint32_t ChunkSize);
    void ReadINFO_ISFT(BitInput *BitI, PCMFile *PCM, uint32_t ChunkSize);
    void ParseWavLISTChunk(BitInput *BitI, PCMFile *PCM, uint32_t ChunkSize);
    void ParseWavDATAChunk(BitInput *BitI, PCMFile *PCM, uint32_t ChunkSize);
    void ParseWavFMTChunk(BitInput *BitI, PCMFile *PCM, uint32_t ChunkSize);
    void WAVExtractSamples(BitInput *BitI, PCMFile *PCM, uint64_t NumSamples2Extract);
    
#ifdef __cplusplus
}
#endif

#endif /* LIBPCM_WAVCOMMON_H */
