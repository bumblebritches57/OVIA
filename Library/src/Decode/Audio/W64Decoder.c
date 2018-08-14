#include "../../../include/Private/Audio/W64Common.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    /* Format decoding */
    static void W64ParseFMTChunk(OVIA *Ovia, BitBuffer *BitB) {
        Ovia->Aud->FormatType       = ReadBits(LSByteFirst, LSBitFirst, BitB, 16);
        Ovia->NumChannels           = ReadBits(LSByteFirst, LSBitFirst, BitB, 16);
        Ovia->Aud->SampleRate       = ReadBits(LSByteFirst, LSBitFirst, BitB, 32);
        BitBuffer_Skip(BitB, 32); // ByteRate
        Ovia->Aud->BlockAlignment   = ReadBits(LSByteFirst, LSBitFirst, BitB, 16);
        Ovia->BitDepth              = ReadBits(LSByteFirst, LSBitFirst, BitB, 16);
    }
    
    static void W64ParseBEXTChunk(OVIA *Ovia, BitBuffer *BitB) {
        
    }
    
    static void W64ParseDATAChunk(OVIA *Ovia, BitBuffer *BitB, uint32_t ChunkSize) { // return the number of samples read
        Ovia->NumChannelAgnosticSamples = (((ChunkSize - 24 / Ovia->Aud->BlockAlignment) / Ovia->NumChannels) / Ovia->BitDepth);
    }
    
    static void W64ParseLEVLChunk(OVIA *Ovia, BitBuffer *BitB) { // aka Peak Envelope Chunk
        
    }
    
    void W64ParseMetadata(OVIA *Ovia, BitBuffer *BitB) {
        uint8_t *ChunkUUIDString = ReadGUUID(GUIDString, BitB);
        uint64_t W64Size         = ReadBits(LSByteFirst, LSBitFirst, BitB, 64);
        if (CompareGUUIDs(GUIDString, ChunkUUIDString, W64_RIFF_GUIDString) == Yes) {
            
        } else if (CompareGUUIDs(GUIDString, ChunkUUIDString, W64_WAVE_GUIDString) == Yes) {
            
        } else if (CompareGUUIDs(GUIDString, ChunkUUIDString, W64_FMT_GUIDString) == Yes) {
            W64ParseFMTChunk(Ovia, BitB);
        } else if (CompareGUUIDs(GUIDString, ChunkUUIDString, W64_DATA_GUIDString) == Yes) {
            
        } else if (CompareGUUIDs(GUIDString, ChunkUUIDString, W64_LEVL_GUIDString) == Yes) {
            
        } else if (CompareGUUIDs(GUIDString, ChunkUUIDString, W64_BEXT_GUIDString) == Yes) {
            W64ParseBEXTChunk(Ovia, BitB);
        } else if (CompareGUUIDs(GUIDString, ChunkUUIDString, W64_FACT_GUIDString) == Yes) {
            
        } else if (CompareGUUIDs(GUIDString, ChunkUUIDString, W64_JUNK_GUIDString) == Yes) {
            
        } else if (CompareGUUIDs(GUIDString, ChunkUUIDString, W64_MRKR_GUIDString) == Yes) {
            
        } else if (CompareGUUIDs(GUIDString, ChunkUUIDString, W64_SUMM_GUIDString) == Yes) {
            
        } else if (CompareGUUIDs(GUIDString, ChunkUUIDString, W64_LIST_GUIDString) == Yes) {
            
        }
    }
    
    void W64ExtractSamples(OVIA *Ovia, BitBuffer *BitB, uint64_t NumSamples2Extract, uint32_t **ExtractedSamples) {
        if (PCM != NULL && BitB != NULL && ExtractedSamples != NULL) {
            for (uint64_t Sample = 0; Sample < NumSamples2Extract; Sample++) {
                for (uint64_t Channel = 0; Channel < Ovia->NumChannels; Channel++) {
                    ExtractedSamples[Channel][Sample] = ReadBits(LSByteFirst, LSBitFirst, BitB, (uint64_t) Bits2Bytes(Ovia->BitDepth, Yes));
                }
            }
        } else if (PCM == NULL) {
            Log(Log_ERROR, __func__, U8("PCM Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        } else if (ExtractedSamples == NULL) {
            Log(Log_ERROR, __func__, U8("ExtractedSamples Pointer is NULL"));
        }
    }
    
#ifdef __cplusplus
}
#endif

