#include "libPCM.h"

#pragma once

#ifndef LIBPCM_WAVCOMMON_H
#define LIBPCM_WAVCOMMON_H

#ifdef __cplusplus
extern "C" {
#endif
    
    void ReadINFO_IART(WAVHeader *Wav, BitBuffer *BitB, uint32_t ChunkSize);
    void ReadINFO_ICRD(WAVHeader *Wav, BitBuffer *BitB, uint32_t ChunkSize);
    void ReadINFO_IGNR(WAVHeader *Wav, BitBuffer *BitB, uint32_t ChunkSize);
    void ReadINFO_INAM(WAVHeader *Wav, BitBuffer *BitB, uint32_t ChunkSize);
    void ReadINFO_IPRD(WAVHeader *Wav, BitBuffer *BitB, uint32_t ChunkSize);
    void ReadINFO_ISFT(WAVHeader *Wav, BitBuffer *BitB, uint32_t ChunkSize);
    void ParseWavLISTChunk(WAVHeader *Wav, BitBuffer *BitB, uint32_t ChunkSize);
    void ParseWavDATAChunk(WAVHeader *Wav, BitBuffer *BitB, uint32_t ChunkSize);
    void ParseWavFMTChunk(WAVHeader *Wav, BitBuffer *BitB, uint32_t ChunkSize);
    void WAVExtractSamples(WAVHeader *Wav, BitBuffer *BitB, uint64_t NumSamples2Extract);
    
#ifdef __cplusplus
}
#endif

#endif /* LIBPCM_WAVCOMMON_H */
