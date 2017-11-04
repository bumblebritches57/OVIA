#include "../../include/libPCM.h"
#include "../../include/Private/libPCMTypes.h"
#include "../../include/Private/Audio/W64Common.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    // To encode W64 i'll need to read the values from the struct and write it to the file
    
    uint64_t CalculateW64ByteRate(const uint64_t NumChannels, const uint64_t SampleRate, const uint8_t BitDepth) {
        return NumChannels * SampleRate * (BitDepth / 8);
    }
    
    uint64_t CalculateW64BlockAlign(const uint64_t NumChannels, const uint8_t BitDepth) {
        return NumChannels * (BitDepth / 8);
    }
    
    void W64WriteFMTChunk(PCMFile *PCM, BitBuffer *BitB) {
        uint64_t ByteRate   = CalculateW64ByteRate(PCM->AUD->NumChannels, PCM->AUD->SampleRate, PCM->AUD->BitDepth);
        uint64_t BlockAlign = CalculateW64BlockAlign(PCM->AUD->NumChannels, PCM->AUD->BitDepth);
        WriteBits(BitIOLSByte, BitIOLSBit, BitB, 16, 0);
        WriteBits(BitIOLSByte, BitIOLSBit, BitB, 16, PCM->AUD->NumChannels);
        WriteBits(BitIOLSByte, BitIOLSBit, BitB, 32, PCM->AUD->SampleRate);
        WriteBits(BitIOLSByte, BitIOLSBit, BitB, 32, ByteRate);
        WriteBits(BitIOLSByte, BitIOLSBit, BitB, 32, BlockAlign);
        WriteBits(BitIOLSByte, BitIOLSBit, BitB, 16, PCM->AUD->BitDepth);
    }
    
    void W64WriteHeader(PCMFile *PCM, BitBuffer *BitB) {
        WriteGUUID(BitIOUUIDString, BitB, W64_RIFF_GUIDString);
        // Write the size of the file including all header fields
        uint64_t W64Size = (PCM->AUD->NumSamples * PCM->AUD->NumChannels * PCM->AUD->BitDepth);
        WriteBits(BitIOLSByte, BitIOLSBit, BitB, 64, W64Size);
        WriteGUUID(BitIOUUIDString, BitB, W64_WAVE_GUIDString);
        WriteGUUID(BitIOUUIDString, BitB, W64_FMT_GUIDString);
        uint64_t FMTSize = 40;
        WriteBits(BitIOLSByte, BitIOLSBit, BitB, 64, FMTSize);
        W64WriteFMTChunk(PCM, BitB);
        WriteGUUID(BitIOUUIDString, BitB, W64_DATA_GUIDString);
        WriteBits(BitIOLSByte, BitIOLSBit, BitB, 64, PCM->AUD->NumSamples);
    }
    
    void W64InsertSamples(PCMFile *PCM, BitBuffer *OutputSamples, uint32_t NumSamples2Write, uint32_t **Samples2Write) {
        if (PCM == NULL) {
            BitIOLog(LOG_ERROR, libPCMLibraryName, __func__, "PCM Pointer is NULL");
        } else if (OutputSamples == NULL) {
            BitIOLog(LOG_ERROR, libPCMLibraryName, __func__, "BitBuffer Pointer is NULL");
        } else {
            uint64_t ChannelCount = PCM->AUD->NumChannels;
            uint64_t BitDepth     = PCM->AUD->BitDepth;
            for (uint32_t Sample = 0; Sample < NumSamples2Write; Sample++) {
                for (uint16_t Channel = 0; Channel < ChannelCount; Channel++) {
                    WriteBits(BitIOLSByte, BitIOLSBit, OutputSamples, BitDepth, Samples2Write[Channel][Sample]);
                }
            }
        }
    }
    
#ifdef __cplusplus
}
#endif
