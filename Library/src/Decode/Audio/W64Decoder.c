#include "../../../include/Private/Audio/W64Common.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    /* Format Oviaoding */
    static void W64ParseFMTChunk(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
        
        Ovia->Aud->FormatType       = ReadBits(LSByteFirst, LSBitFirst, BitB, 16);
        OVIA_GetNumChannels(Ovia)           = ReadBits(LSByteFirst, LSBitFirst, BitB, 16);
        Ovia->Aud->SampleRate       = ReadBits(LSByteFirst, LSBitFirst, BitB, 32);
        BitBuffer_Skip(BitB, 32); // ByteRate
        Ovia->Aud->BlockAlignment   = ReadBits(LSByteFirst, LSBitFirst, BitB, 16);
        Ovia->BitDepth              = ReadBits(LSByteFirst, LSBitFirst, BitB, 16);
    }
    
    static void W64ParseBEXTChunk(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    static void W64ParseDATAChunk(OVIA *Ovia, BitBuffer *BitB, uint32_t ChunkSize) { // return the number of samples read
        if (Ovia != NULL && BitB != NULL) {
            
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
        
        Ovia->NumChannelAgnosticSamples = (((ChunkSize - 24 / Ovia->Aud->BlockAlignment) / OVIA_GetNumChannels(Ovia)) / Ovia->BitDepth);
    }
    
    static void W64ParseLEVLChunk(OVIA *Ovia, BitBuffer *BitB) { // aka Peak Envelope Chunk
        if (Ovia != NULL && BitB != NULL) {
            
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void W64ParseMetadata(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
        
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
    
    AudioContainer *W64ExtractSamples(OVIA *Ovia, BitBuffer *BitB) {
        AudioContainer *Audio = NULL;
        if (Ovia != NULL && BitB != NULL) {
            uint64_t BitDepth     = OVIA_GetBitDepth(Ovia);
            uint64_t NumChannels  = OVIA_GetNumChannels(Ovia);
            uint64_t SampleRate   = OVIA_GetSampleRate(Ovia);
            uint64_t NumSamples   = OVIA_GetNumSamples(Ovia);
            if (BitDepth <= 8) {
                Audio = AudioContainer_Init(AudioContainer_UInteger8, BitDepth, NumChannels, SampleRate, NumSamples);
                uint8_t **Samples = (uint8_t**) AudioContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < NumChannels; Channel++) {
                        Samples[Channel][Sample] = ReadBits(MSByteFirst, LSBitFirst, BitB, Bits2Bytes(BitDepth, Yes));
                    }
                }
            } else if (BitDepth > 8 && BitDepth <= 16) {
                Audio = AudioContainer_Init(AudioContainer_UInteger16, BitDepth, NumChannels, SampleRate, NumSamples);
                uint16_t **Samples = (uint16_t**) AudioContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < NumChannels; Channel++) {
                        Samples[Channel][Sample] = ReadBits(MSByteFirst, LSBitFirst, BitB, Bits2Bytes(BitDepth, Yes));
                    }
                }
            } else if (BitDepth > 16 && BitDepth <= 32) {
                Audio = AudioContainer_Init(AudioContainer_UInteger32, BitDepth, NumChannels, SampleRate, NumSamples);
                uint32_t **Samples = (uint32_t**) AudioContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < NumChannels; Channel++) {
                        Samples[Channel][Sample] = ReadBits(MSByteFirst, LSBitFirst, BitB, Bits2Bytes(BitDepth, Yes));
                    }
                }
            }
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
        return Audio;
    }
    
#ifdef __cplusplus
}
#endif

