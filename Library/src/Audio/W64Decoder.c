#include "../../../Dependencies/BitIO/libBitIO/include/BitIO.h"

#include "../../include/libPCM.h"
#include "../../include/Private/libPCMTypes.h"
#include "../../include/Private/Audio/W64Common.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    /* Format decoding */
    static void W64ParseFMTChunk(PCMFile *PCM, BitBuffer *BitB) {
        PCM->AUD->FormatType       = ReadBits(BitIOLSByte, BitIOLSBit, BitB, 16);
        PCM->NumChannels      = ReadBits(BitIOLSByte, BitIOLSBit, BitB, 16);
        PCM->AUD->SampleRate       = ReadBits(BitIOLSByte, BitIOLSBit, BitB, 32);
        BitBuffer_Skip(BitB, 32); // ByteRate
        PCM->AUD->BlockAlignment   = ReadBits(BitIOLSByte, BitIOLSBit, BitB, 16);
        PCM->BitDepth         = ReadBits(BitIOLSByte, BitIOLSBit, BitB, 16);
    }
    
    static void W64ParseBEXTChunk(PCMFile *PCM, BitBuffer *BitB) {
        
    }
    
    static void W64ParseDATAChunk(PCMFile *PCM, BitBuffer *BitB, uint32_t ChunkSize) { // return the number of samples read
        PCM->NumChannelAgnosticSamples = (((ChunkSize - 24 / PCM->AUD->BlockAlignment) / PCM->NumChannels) / PCM->BitDepth);
    }
    
    static void W64ParseLEVLChunk(PCMFile *PCM, BitBuffer *BitB) { // aka Peak Envelope Chunk
        
    }
    
    void W64ParseMetadata(PCMFile *PCM, BitBuffer *BitB) {
        uint8_t *ChunkUUIDString = ReadGUUID(BitIOGUUIDString, BitIOLSByte, BitB);
        uint64_t W64Size         = ReadBits(BitIOLSByte, BitIOLSBit, BitB, 64);
        if (GUUID_Compare(BitIOGUUIDString, ChunkUUIDString, W64_RIFF_GUIDString) == Yes) {
            
        } else if (GUUID_Compare(BitIOGUUIDString, ChunkUUIDString, W64_WAVE_GUIDString) == Yes) {
            
        } else if (GUUID_Compare(BitIOGUUIDString, ChunkUUIDString, W64_FMT_GUIDString) == Yes) {
            W64ParseFMTChunk(PCM, BitB);
        } else if (GUUID_Compare(BitIOGUUIDString, ChunkUUIDString, W64_DATA_GUIDString) == Yes) {
            
        } else if (GUUID_Compare(BitIOGUUIDString, ChunkUUIDString, W64_LEVL_GUIDString) == Yes) {
            
        } else if (GUUID_Compare(BitIOGUUIDString, ChunkUUIDString, W64_BEXT_GUIDString) == Yes) {
            W64ParseBEXTChunk(PCM, BitB);
        } else if (GUUID_Compare(BitIOGUUIDString, ChunkUUIDString, W64_FACT_GUIDString) == Yes) {
            
        } else if (GUUID_Compare(BitIOGUUIDString, ChunkUUIDString, W64_JUNK_GUIDString) == Yes) {
            
        } else if (GUUID_Compare(BitIOGUUIDString, ChunkUUIDString, W64_MRKR_GUIDString) == Yes) {
            
        } else if (GUUID_Compare(BitIOGUUIDString, ChunkUUIDString, W64_SUMM_GUIDString) == Yes) {
            
        } else if (GUUID_Compare(BitIOGUUIDString, ChunkUUIDString, W64_LIST_GUIDString) == Yes) {
            
        }
    }
    
    void W64ExtractSamples(PCMFile *PCM, BitBuffer *BitB, uint64_t NumSamples2Extract, uint32_t **ExtractedSamples) {
        for (uint64_t Sample = 0; Sample < NumSamples2Extract; Sample++) {
            for (uint64_t Channel = 0; Channel < PCM->NumChannels; Channel++) {
                ExtractedSamples[Channel][Sample] = ReadBits(BitIOLSByte, BitIOLSBit, BitB, (uint64_t) Bits2Bytes(PCM->BitDepth, Yes));
            }
        }
    }
    
#ifdef __cplusplus
}
#endif

