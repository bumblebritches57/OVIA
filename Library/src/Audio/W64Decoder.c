#include "../../include/libPCM.h"
#include "../../include/Private/libPCMTypes.h"
#include "../../include/Private/Audio/W64Common.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    void ParseW64Metadata(PCMFile *PCM, BitBuffer *BitB) {
        uint8_t  *ChunkID  = NULL;
        ChunkID            = ReadGUUID(BitIOBinaryGUID, BitB);
        uint64_t W64Size   = ReadBits(BitIOLSByte, BitIOLSBit, BitB, 64);
        
        uint64_t ChunkSize = ReadBits(BitIOLSByte, BitIOLSBit, BitB, 64);
        
        if (CompareUUIDs(ChunkID, W64_WAVE) == 0) {
            
        }
        if (CompareUUIDs(ChunkID, W64_FMT) == 0) {
            ParseW64FMTChunk(PCM, BitB);
        } else if (CompareUUIDs(ChunkID, W64_BEXT) == 0) {
            ParseW64BEXTChunk(PCM, BitB);
        }
    }
    
    /* Format decoding */
    void ParseW64FMTChunk(PCMFile *PCM, BitBuffer *BitB) {
        PCM->WAVW64FormatType = ReadBits(BitIOLSByte, BitIOLSBit, BitB, 16);
        PCM->NumChannels      = ReadBits(BitIOLSByte, BitIOLSBit, BitB, 16);
        PCM->SampleRate       = ReadBits(BitIOLSByte, BitIOLSBit, BitB, 32);
        SkipBits(BitB, 32); // ByteRate
        PCM->BlockAlignment   = ReadBits(BitIOLSByte, BitIOLSBit, BitB, 16);
        PCM->BitDepth         = ReadBits(BitIOLSByte, BitIOLSBit, BitB, 16);
    }
    
    void ParseW64BEXTChunk(PCMFile *PCM, BitBuffer *BitB) {
        
    }
    
    void ParseW64DataChunk(PCMFile *PCM, BitBuffer *BitB, uint32_t ChunkSize) { // return the number of samples read
        PCM->NumSamples = (((ChunkSize - 24 / PCM->BlockAlignment) / PCM->NumChannels) / PCM->BitDepth);
    }
    
    void ParseW64LEVL(PCMFile *PCM, BitBuffer *BitB) { // aka Peak Envelope Chunk
        
    }
    
#ifdef __cplusplus
}
#endif

