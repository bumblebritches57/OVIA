#include "../../include/libPCM.h"
#include "../../include/Private/libPCMTypes.h"
#include "../../include/Private/Audio/W64Common.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    void ParseW64Metadata(W64Header *W64, BitBuffer *BitB) {
        uint8_t  ChunkID[16];
        
        uint64_t RiffSize  = ReadBits(BitB, 64, true);
        for (uint8_t GUIDByte = 0; GUIDByte < BitIOBinaryGUIDSize; GUIDByte++) {
            ChunkID[GUIDByte] = ReadBits(BitB, 8, true);
        }
        uint64_t ChunkSize = ReadBits(BitB, 64, true);
        switch (ChunkID) {
            case W64_FMT:
                ParseW64FMTChunk(BitB, W64);
                break;
            case W64_BEXT:
                ParseW64BEXTChunk(BitB, W64);
                break;
                
            default:
                break;
        }
    }
    
    /* Format decoding */
    void ParseW64FMTChunk(W64Header *W64, BitBuffer *BitB) {
        W64->FormatType  = ReadBits(BitB, 16, true);
        W64->Channels    = ReadBits(BitB, 16, true);
        W64->SampleRate  = ReadBits(BitB, 32, true);
        W64->ByteRate    = ReadBits(BitB, 32, true);
        W64->BlockAlign  = ReadBits(BitB, 16, true);
        W64->BitDepth    = ReadBits(BitB, 16, true);
    }
    
    void ParseW64BEXTChunk(W64Header *W64, BitBuffer *BitB) {
        
    }
    
    void ParseW64DataChunk(W64Header *W64, BitBuffer *BitB, uint32_t ChunkSize) { // return the number of samples read
        W64->NumSamples = (((ChunkSize - 24 / W64->BlockAlign) / W64->Channels) / W64->BitDepth);
    }
    
    void ParseW64LEVL(W64Header *W64, BitBuffer *BitB) { // aka Peak Envelope Chunk
        
    }
    
#ifdef __cplusplus
}
#endif

