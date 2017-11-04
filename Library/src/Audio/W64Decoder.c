#include "../../include/libPCM.h"
#include "../../include/Private/libPCMTypes.h"
#include "../../include/Private/Audio/W64Common.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    /* Format decoding */
    void W64ParseFMTChunk(PCMFile *PCM, BitBuffer *BitB) {
        PCM->AUD->FormatType       = ReadBits(BitIOLSByte, BitIOLSBit, BitB, 16);
        PCM->AUD->NumChannels      = ReadBits(BitIOLSByte, BitIOLSBit, BitB, 16);
        PCM->AUD->SampleRate       = ReadBits(BitIOLSByte, BitIOLSBit, BitB, 32);
        BitBufferSkip(BitB, 32); // ByteRate
        PCM->AUD->BlockAlignment   = ReadBits(BitIOLSByte, BitIOLSBit, BitB, 16);
        PCM->AUD->BitDepth         = ReadBits(BitIOLSByte, BitIOLSBit, BitB, 16);
    }
    
    void W64ParseBEXTChunk(PCMFile *PCM, BitBuffer *BitB) {
        
    }
    
    void W64ParseDATAChunk(PCMFile *PCM, BitBuffer *BitB, uint32_t ChunkSize) { // return the number of samples read
        PCM->AUD->NumSamples = (((ChunkSize - 24 / PCM->AUD->BlockAlignment) / PCM->AUD->NumChannels) / PCM->AUD->BitDepth);
    }
    
    void W64ParseLEVLChunk(PCMFile *PCM, BitBuffer *BitB) { // aka Peak Envelope Chunk
        
    }
    
    void W64ParseMetadata(PCMFile *PCM, BitBuffer *BitB) {
        uint8_t *ChunkUUIDString = ReadGUUID(BitIOUUIDString, BitB);
        uint64_t W64Size         = ReadBits(BitIOLSByte, BitIOLSBit, BitB, 64);
        if (CompareGUUIDs(ChunkUUIDString, W64_RIFF_GUIDString, BitIOGUUIDString) == Yes) {
            
        } else if (CompareGUUIDs(ChunkUUIDString, W64_WAVE_GUIDString, BitIOGUUIDString) == Yes) {
            
        } else if (CompareGUUIDs(ChunkUUIDString, W64_FMT_GUIDString, BitIOGUUIDString) == Yes) {
            W64ParseFMTChunk(PCM, BitB);
        } else if (CompareGUUIDs(ChunkUUIDString, W64_DATA_GUIDString, BitIOGUUIDString) == Yes) {
            
        } else if (CompareGUUIDs(ChunkUUIDString, W64_LEVL_GUIDString, BitIOGUUIDString) == Yes) {
            
        } else if (CompareGUUIDs(ChunkUUIDString, W64_BEXT_GUIDString, BitIOGUUIDString) == Yes) {
            W64ParseBEXTChunk(PCM, BitB);
        } else if (CompareGUUIDs(ChunkUUIDString, W64_FACT_GUIDString, BitIOGUUIDString) == Yes) {
            
        } else if (CompareGUUIDs(ChunkUUIDString, W64_JUNK_GUIDString, BitIOGUUIDString) == Yes) {
            
        } else if (CompareGUUIDs(ChunkUUIDString, W64_MRKR_GUIDString, BitIOGUUIDString) == Yes) {
            
        } else if (CompareGUUIDs(ChunkUUIDString, W64_SUMM_GUIDString, BitIOGUUIDString) == Yes) {
            
        } else if (CompareGUUIDs(ChunkUUIDString, W64_LIST_GUIDString, BitIOGUUIDString) == Yes) {
            
        }
    }
    
    uint32_t **W64ExtractSamples(PCMFile *PCM, BitBuffer *BitB, uint64_t NumSamples2Extract) {
        return NULL;
    }
    
#ifdef __cplusplus
}
#endif

