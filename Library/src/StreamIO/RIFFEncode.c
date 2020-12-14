#include "../../include/Private/StreamIO/RIFFStream.h"
#include "../../../Dependencies/FoundationIO/Library/include/TextIO/LogIO.h"
#include "../../../Dependencies/FoundationIO/Library/include/MathIO.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    void WAV_Write_Header(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            /*
             We need to make all WV files Broadcast Wave compatible, by writing a JUNK chunk and renaming it ds64 if the number of samples grows too large for standard WAVE.
             
             So, that means that we also need to use the WriteFooter function pointer to create a function that checks the size, and if the number of samples times the number of channels times the bitdepth is greater than the 4GB barried, we renae the JUNK chunk to ds64.
             */
        } else if (Options == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    static void WAV_Write_FMT(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            RIFFOptions *WAV      = Options;
            uint64_t NumChannels = WAV->NumChannels;
            uint64_t SampleRate  = WAV->SampleRate;
            uint64_t BitDepth    = WAV->BitDepth;
            uint8_t CBSize = 46;
            
            BitBuffer_WriteBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 32, 40); // ChunkSize
            BitBuffer_WriteBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 16, 0xFFFE); // WaveFormatExtensible
            BitBuffer_WriteBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 16, NumChannels);
            BitBuffer_WriteBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 32, SampleRate);
            BitBuffer_WriteBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 32, SampleRate * NumChannels * BitDepth);
            BitBuffer_WriteBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 32, WAV->BlockAlign);
            BitBuffer_WriteBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 16, BitDepth);
            BitBuffer_WriteBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 16, CBSize);
            BitBuffer_WriteBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 16, BitDepth); // ValidBitsPerSample
            BitBuffer_WriteBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 32, WAV->ChannelMask);
            BitBuffer_WriteGUUID(BitB, GUUIDType_BinaryGUID, WAVNULLBinaryGUID);
        } else if (Options == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    static void WAV_Write_LIST(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            RIFFOptions *WAV      = Options;
            // Write the tags
        } else if (Options == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    void WAVAppendSamples(Audio2DContainer *Audio, BitBuffer *BitB) {
        if (Audio != NULL && BitB != NULL) {
            uint64_t NumChannels    = Audio2DContainer_GetNumChannels(Audio);
            uint64_t BitDepth       = Audio2DContainer_GetBitDepth(Audio);
            uint64_t NumSamples     = Audio2DContainer_GetNumSamples(Audio);
            MediaIO_AudioTypes Type = Audio2DContainer_GetType(Audio);
            
            if (Type == (AudioType_Unsigned | AudioType_Integer8)) {
                uint8_t **Samples  = (uint8_t**) Audio2DContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < NumChannels; Channel++) {
                        BitBuffer_WriteBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, BitDepth, Samples[Channel][Sample]);
                    }
                }
            } else if (Type == (AudioType_Signed | AudioType_Integer8)) {
                int8_t **Samples   = (int8_t**) Audio2DContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < NumChannels; Channel++) {
                        BitBuffer_WriteBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, BitDepth, Samples[Channel][Sample]);
                    }
                }
            } else if (Type == (AudioType_Unsigned | AudioType_Integer16)) {
                uint16_t **Samples = (uint16_t**) Audio2DContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < NumChannels; Channel++) {
                        BitBuffer_WriteBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, BitDepth, Samples[Channel][Sample]);
                    }
                }
            } else if (Type == (AudioType_Signed | AudioType_Integer16)) {
                int16_t **Samples  = (int16_t**) Audio2DContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < NumChannels; Channel++) {
                        BitBuffer_WriteBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, BitDepth, Samples[Channel][Sample]);
                    }
                }
            } else if (Type == (AudioType_Unsigned | AudioType_Integer32)) {
                uint32_t **Samples = (uint32_t**) Audio2DContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < NumChannels; Channel++) {
                        BitBuffer_WriteBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, BitDepth, Samples[Channel][Sample]);
                    }
                }
            } else if (Type == (AudioType_Signed | AudioType_Integer32)) {
                int32_t **Samples  = (int32_t**) Audio2DContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < NumChannels; Channel++) {
                        BitBuffer_WriteBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, BitDepth, Samples[Channel][Sample]);
                    }
                }
            }
        } else if (Audio == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("Audio2DContainer Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    void WAV_Write_Footer(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            RIFFOptions *WAV          = Options;
            uint64_t    WAVSampleSize = Bits2Bytes(WAV->NumSamplesWritten * WAV->NumChannels * WAV->BitDepth, RoundingType_Up); // Don't forget padding
            if (WAVSampleSize > 0xFFFFFFFF) {
                // Rename the JUNK chunk to ds64,
                // So save the current bit position
                // What do we do if the BitBuffer was written to the file?
                // Well, we could store the current offset, reload the buffer with the first kilobyte or whatever the header may take up, then change it, rewrite that section of the file, then reload where we were...
                // that probably wouldn't work, and if it did, it wouldn't work for true streaming applications.
                
                // A better idea is to just set the WAVE FMT header to 0xFFFFFFFF and set the ds64 block by default.
            }
            // Now check the size of the samples.
        } else if (Options == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    // To encode W64 i'll need to read the values from the struct and write it to the file
    
    static void W64_Write_FMT(Audio2DContainer *Audio, BitBuffer *BitB) {
        if (Audio != NULL && BitB != NULL) {
            uint64_t NumChannels = Audio2DContainer_GetNumSamples(Audio);
            uint64_t SampleRate  = Audio2DContainer_GetSampleRate(Audio);
            uint8_t  BitDepth    = Bits2Bytes(Audio2DContainer_GetBitDepth(Audio), RoundingType_Up);
            uint64_t ByteRate    = RIFF_CalculateByteRate(NumChannels, BitDepth, SampleRate);
            uint64_t BlockAlign  = RIFF_CalculateBlockAlignment(NumChannels, BitDepth);
            BitBuffer_WriteBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 16, 0);
            BitBuffer_WriteBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 16, NumChannels);
            BitBuffer_WriteBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 32, SampleRate);
            BitBuffer_WriteBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 32, ByteRate);
            BitBuffer_WriteBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 32, BlockAlign);
            BitBuffer_WriteBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 16, BitDepth);
        } else if (Audio == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("Audio2DContainer Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    void W64_Write_Header(Audio2DContainer *Audio, BitBuffer *BitB) {
        if (Audio != NULL && BitB != NULL) {
            uint64_t NumChannels = Audio2DContainer_GetNumChannels(Audio);
            uint64_t NumSamples  = Audio2DContainer_GetNumSamples(Audio);
            uint8_t  BitDepth    = Bits2Bytes(Audio2DContainer_GetBitDepth(Audio), RoundingType_Up);
            uint64_t W64Size     = (NumSamples * NumChannels * BitDepth);
            uint64_t FMTSize     = 40;
            
            BitBuffer_WriteGUUID(BitB, GUUIDType_GUIDString, W64_RIFF_GUIDString);
            BitBuffer_WriteBits(BitB,  ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 64, W64Size);
            BitBuffer_WriteGUUID(BitB, GUUIDType_GUIDString, W64_WAVE_GUIDString);
            BitBuffer_WriteGUUID(BitB, GUUIDType_GUIDString, W64_FMT_GUIDString);
            BitBuffer_WriteBits(BitB,  ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 64, FMTSize);
            W64_Write_FMT(Audio, BitB);
            BitBuffer_WriteGUUID(BitB, GUUIDType_GUIDString, W64_DATA_GUIDString);
            BitBuffer_WriteBits(BitB,  ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 64, NumSamples);
        } else if (Audio == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("Audio2DContainer Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    void W64AppendSamples(Audio2DContainer *Audio, BitBuffer *BitB) {
        if (Audio != NULL && BitB != NULL) {
            uint64_t NumChannels    = Audio2DContainer_GetNumChannels(Audio);
            uint64_t BitDepth       = Bits2Bytes(Audio2DContainer_GetBitDepth(Audio), RoundingType_Up);
            uint64_t NumSamples     = Audio2DContainer_GetNumSamples(Audio);
            MediaIO_AudioTypes Type = Audio2DContainer_GetType(Audio);
            if (Type == (AudioType_Signed | AudioType_Integer8)) {
                int8_t **Samples  = (int8_t**)    Audio2DContainer_GetArray(Audio);
                for (uint32_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint16_t Channel = 0; Channel < NumChannels; Channel++) {
                        BitBuffer_WriteBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, BitDepth, Samples[Channel][Sample]);
                    }
                }
            } else if (Type == (AudioType_Unsigned | AudioType_Integer8)) {
                uint8_t **Samples = (uint8_t**)   Audio2DContainer_GetArray(Audio);
                for (uint32_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint16_t Channel = 0; Channel < NumChannels; Channel++) {
                        BitBuffer_WriteBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, BitDepth, Samples[Channel][Sample]);
                    }
                }
            } else if (Type == (AudioType_Signed | AudioType_Integer16)) {
                int16_t **Samples = (int16_t**)   Audio2DContainer_GetArray(Audio);
                for (uint32_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint16_t Channel = 0; Channel < NumChannels; Channel++) {
                        BitBuffer_WriteBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, BitDepth, Samples[Channel][Sample]);
                    }
                }
            } else if (Type == (AudioType_Unsigned | AudioType_Integer16)) {
                uint16_t **Samples = (uint16_t**) Audio2DContainer_GetArray(Audio);
                for (uint32_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint16_t Channel = 0; Channel < NumChannels; Channel++) {
                        BitBuffer_WriteBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, BitDepth, Samples[Channel][Sample]);
                    }
                }
            } else if (Type == (AudioType_Signed | AudioType_Integer32)) {
                int32_t  **Samples = (int32_t**)  Audio2DContainer_GetArray(Audio);
                for (uint32_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint16_t Channel = 0; Channel < NumChannels; Channel++) {
                        BitBuffer_WriteBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, BitDepth, Samples[Channel][Sample]);
                    }
                }
            } else if (Type == (AudioType_Unsigned & AudioType_Integer32)) {
                uint32_t **Samples = (uint32_t**) Audio2DContainer_GetArray(Audio);
                for (uint32_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint16_t Channel = 0; Channel < NumChannels; Channel++) {
                        BitBuffer_WriteBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, BitDepth, Samples[Channel][Sample]);
                    }
                }
            }
        } else if (Audio == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("Audio2DContainer Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
#ifdef __cplusplus
}
#endif

