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
        PCM->AUD->FormatType       = ReadBits(BitIOLSByte, BitIOLSBit, BitB, 16);
        PCM->AUD->NumChannels      = ReadBits(BitIOLSByte, BitIOLSBit, BitB, 16);
        PCM->AUD->SampleRate       = ReadBits(BitIOLSByte, BitIOLSBit, BitB, 32);
        BitBufferSkip(BitB, 32); // ByteRate
        PCM->AUD->BlockAlignment   = ReadBits(BitIOLSByte, BitIOLSBit, BitB, 16);
        PCM->AUD->BitDepth         = ReadBits(BitIOLSByte, BitIOLSBit, BitB, 16);
    }
    
    void ParseW64BEXTChunk(PCMFile *PCM, BitBuffer *BitB) {
        
    }
    
    void ParseW64DataChunk(PCMFile *PCM, BitBuffer *BitB, uint32_t ChunkSize) { // return the number of samples read
        PCM->AUD->NumSamples = (((ChunkSize - 24 / PCM->AUD->BlockAlignment) / PCM->AUD->NumChannels) / PCM->AUD->BitDepth);
    }
    
    void ParseW64LEVL(PCMFile *PCM, BitBuffer *BitB) { // aka Peak Envelope Chunk
        
    }
    
#ifdef __cplusplus
}
#endif

