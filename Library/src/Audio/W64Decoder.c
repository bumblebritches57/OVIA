#include "../../include/libPCM.h"
#include "../../include/Private/libPCMTypes.h"
#include "../../include/Private/Audio/W64Common.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    void ParseW64Metadata(PCMFile *PCM, BitBuffer *BitB) {
        uint8_t  *ChunkID = calloc(1, BitIOBinaryGUIDSize);
        
        ChunkID           = ReadUUID(BitB);
        uint64_t RiffSize = ReadBits(BitB, 64, true);
        
        
        
        
        
        uint64_t ChunkSize = ReadBits(BitB, 64, true);
        
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
        PCM->WAVW64FormatType = ReadBits(BitB, 16, true);
        PCM->NumChannels      = ReadBits(BitB, 16, true);
        PCM->SampleRate       = ReadBits(BitB, 32, true);
        SkipBits(BitB, 32); // ByteRate
        PCM->BlockAlignment   = ReadBits(BitB, 16, true);
        PCM->BitDepth         = ReadBits(BitB, 16, true);
    }
    
    void ParseW64BEXTChunk(PCMFile *PCM, BitBuffer *BitB) {
        
    }
    
    void ParseW64DataChunk(PCMFile *PCM, BitBuffer *BitB, uint32_t ChunkSize) { // return the number of samples read
        PCM->NumSamples = (((ChunkSize - 24 / PCM->BlockAlign) / PCM->Channels) / PCM->BitDepth);
    }
    
    void ParseW64LEVL(PCMFile *PCM, BitBuffer *BitB) { // aka Peak Envelope Chunk
        
    }
    
#ifdef __cplusplus
}
#endif

