#include "libPCM.h"

#pragma once

#ifdef __cplusplus
extern "C" {
#endif
    
    void ReadINFO_IART(BitInput *BitI, WAVHeader *WAV, uint32_t ChunkSize);
    void ReadINFO_ICRD(BitInput *BitI, WAVHeader *WAV, uint32_t ChunkSize);
    void ReadINFO_IGNR(BitInput *BitI, WAVHeader *WAV, uint32_t ChunkSize);
    void ReadINFO_INAM(BitInput *BitI, WAVHeader *WAV, uint32_t ChunkSize);
    void ReadINFO_IPRD(BitInput *BitI, WAVHeader *WAV, uint32_t ChunkSize);
    void ReadINFO_ISFT(BitInput *BitI, WAVHeader *WAV, uint32_t ChunkSize);
    void ParseWavLISTChunk(BitInput *BitI, WAVHeader *WAV, uint32_t ChunkSize);
    void ParseWavDATAChunk(BitInput *BitI, WAVHeader *WAV, uint32_t ChunkSize);
    void ParseWavFMTChunk(BitInput *BitI, WAVHeader *WAV, uint32_t ChunkSize);
    
    
#ifdef __cplusplus
}
#endif
