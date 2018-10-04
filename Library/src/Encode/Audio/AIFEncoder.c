#include "../../../../Dependencies/FoundationIO/libFoundationIO/include/Macros.h"
#include "../../../include/Private/Audio/AIFCommon.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    typedef union Integer2Double {
        double   Double;
        uint64_t Integer;
    } Integer2Double;
    
    static double ConvertInteger2Double(uint64_t Number) {
        Integer2Double Cast;
        Cast.Integer = Number;
        return Cast.Double;
    }
    
    static void AIFWriteCOMM(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            BitBuffer_WriteBits(MSByteFirst, LSBitFirst, BitB, 32, AIF_COMM);
            uint16_t COMMSize = 18;
            BitBuffer_WriteBits(MSByteFirst, LSBitFirst, BitB, 32, COMMSize);
            BitBuffer_WriteBits(MSByteFirst, LSBitFirst, BitB, 16, OVIA_GetNumChannels(Ovia)); // 2
            BitBuffer_WriteBits(MSByteFirst, LSBitFirst, BitB, 32, OVIA_GetNumSamples(Ovia)); // 7979748
            BitBuffer_WriteBits(MSByteFirst, LSBitFirst, BitB, 16, OVIA_GetBitDepth(Ovia)); // 16
            uint64_t SampleRate = ConvertInteger2Double(OVIA_GetSampleRate(Ovia));
            // 44100  = 0x400E 0xAC44000000000000
            // 48000  = 0x400E 0xBB80000000000000
            // 88200  = 0x400F 0xAC44000000000000
            // 96000  = 0x400F 0xBB80000000000000
            // 192000 = 0x4010 0xBB80000000000000
            
            // Treat them as 2 seperate numbers.
            // The first is Unknown.
            // The second is the sample rate
            
            // So, 0x400X = Unsigned, Exponent = {,,}
            
            
            
            // e = (((int32_t)ext.exponent[0]&0x7F)<<8) | ext.exponent[1];
            // Exponent = (0x4X & 0x7F << 8) | 0x01
            // So, remove the sign bit from the exponent.
            // Shift the Exponent then bitwise add the second half of the exponent...
            
            // Literally how does that differ from reading the sign bit, then reading the Exponent...
            // If the Exponent is 0x7FFF AND the Mantissa is greater than 0, return 0.
            // e -= 16383 + 63;
            // Subtract 0x403E from the Exponent? that makes no sense...
            
            // Maybe it's supposed to be Exponent = (Exponent - 0x3FFF) + 63?
            // 0x4010 - (0x4010 - 0x3FFF) = 17
            // 17 + 63 = 80...
            
            // If (Exponent & 0x8000) >> 15 == 1, the Mantissa is negative.
            
            // if Exponent & 0x7FFF > 0, Mantissa << Exponent.
            // If Exponent & 0x7FFF < 0, Mantissa >> -Exponent.
            
            // Well, let's try it.
            // Sign = 0, positive.
            // Exponent = 0x4010 - 0x3FFF = 17, 17 + 63 = 80.
            // Mantissa is positive.
            // Mantissa << 80? that seems excessive as fuck but ok
            // 0xAC44
            
            
            
            
            
            
            // LibAV's Algorithm for extrcting the sample rate:
            
            // Read the first 16 bits as the Exponent. (Includes sign bit)
            // Read the next  64 bits as the Mantissa.
            
            // Call ldexp with the Mantissa first, followed by the exponent - 16383 - 63 aka 0x3FFF - 0x3F = 0x3FC0
            // ldexp multiplies the Mantissa by 2 to the power of Exponent.
            // aka SampleRate = Mantissa * 2^Exponent.
            // SampleRate = 0xBB80000000000000 * 0x4010 - 0x3FC0 = 80
            
            // Subtract 16383 from the exponent.
            // 17, 48000.
            // So, subtract 11 from the result of Exponent - 16383 to get the times to multiply it by...
            // 17 - 11 = 6, which is wrong, it needs to be 13.
            
            // 2 ^ 12 = 4096, 3904 more.
            // FA = 250
            
            
            // Ok, easy way to do this I think.
            
            // Exponent - 16383.
            // Subtract 13 from that.
            // 13 - 12 = -1, or
            // That is the number of times you multiply the Mantissa by.
            // 0xFA = 250
            // 2 * 250 = 500
            
            
            BitBuffer_WriteBits(MSByteFirst, LSBitFirst, BitB, 16, (SampleRate >> 52) + 15360);
            BitBuffer_WriteBits(MSByteFirst, LSBitFirst, BitB, 64, 0x8000000000000000LLU | SampleRate << 11); // SampleRate Mantissa
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    static void AIFWriteTitle(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            uint64_t TitleTagIndex = OVIA_GetTagsIndex(Ovia, TitleTag);
            uint32_t TitleTagSize  = 0ULL;
            if (TitleTagIndex != 0xFFFFFFFFFFFFFFFF) {
                UTF8 *TitleTag = OVIA_GetTag(Ovia, TitleTagIndex);
                uint32_t TagByte = 0ULL;
                do {
                    BitBuffer_WriteBits(MSByteFirst, LSBitFirst, BitB, 8, TitleTag[TagByte]);
                    TagByte += 1;
                } while (TitleTag[TagByte] != NULLTerminator);
                TitleTagSize = TagByte;
            }
            AIFSkipPadding(BitB, TitleTagSize);
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    static void AIFWriteArtist(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            uint64_t ArtistTagIndex = OVIA_GetTagsIndex(Ovia, ArtistTag);
            uint32_t ArtistTagSize  = 0ULL;
            if (ArtistTagIndex != 0xFFFFFFFFFFFFFFFF) {
                UTF8 *ArtistTag = OVIA_GetTag(Ovia, ArtistTagIndex);
                uint32_t TagByte = 0ULL;
                do {
                    BitBuffer_WriteBits(MSByteFirst, LSBitFirst, BitB, 8, ArtistTag[TagByte]);
                    TagByte += 1;
                } while (ArtistTag[TagByte] != NULLTerminator);
                ArtistTagSize = TagByte;
            }
            AIFSkipPadding(BitB, ArtistTagSize);
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    static void AIFWriteSSND(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            BitBuffer_WriteBits(MSByteFirst, LSBitFirst, BitB, 32, AIF_SSND);
            uint64_t NumSamples  = OVIA_GetNumSamples(Ovia);
            uint64_t NumChannels = OVIA_GetNumChannels(Ovia);
            uint64_t BitDepth    = OVIA_GetBitDepth(Ovia);
            uint64_t Offset      = OVIA_GetSampleOffset(Ovia);
            uint64_t BlockSize   = OVIA_GetBlockSize(Ovia);
            
            uint32_t ChunkSize   = 8 + ((NumSamples * NumChannels) * Bits2Bytes(BitDepth, true));
            BitBuffer_WriteBits(MSByteFirst, LSBitFirst, BitB, 32, ChunkSize);
            BitBuffer_WriteBits(MSByteFirst, LSBitFirst, BitB, 32, Offset);
            BitBuffer_WriteBits(MSByteFirst, LSBitFirst, BitB, 32, BlockSize);
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void AIFWriteHeader(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            BitBuffer_WriteBits(MSByteFirst, LSBitFirst, BitB, 32, AIF_SSND);
            uint64_t NumSamples  = OVIA_GetNumSamples(Ovia);
            uint64_t NumChannels = OVIA_GetNumChannels(Ovia);
            uint64_t BitDepth    = OVIA_GetBitDepth(Ovia);
            
            BitBuffer_WriteBits(MSByteFirst, LSBitFirst, BitB, 32, AIF_FORM);
            BitBuffer_WriteBits(MSByteFirst, LSBitFirst, BitB, 32, ((NumSamples * NumChannels) * Bits2Bytes(BitDepth, true))); // FIXME: AIF Size calculation is wrong
            BitBuffer_WriteBits(MSByteFirst, LSBitFirst, BitB, 32, AIF_AIFF);
            AIFWriteCOMM(Ovia, BitB);
            AIFWriteTitle(Ovia, BitB);
            AIFWriteArtist(Ovia, BitB);
            AIFWriteSSND(Ovia, BitB);
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void AIFAppendSamples(OVIA *Ovia, AudioContainer *Audio, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL && Audio != NULL) {
            uint64_t NumSamples  = OVIA_GetNumSamples(Ovia);
            uint64_t NumChannels = OVIA_GetNumChannels(Ovia);
            uint64_t BitDepth    = OVIA_GetBitDepth(Ovia);
            uint64_t Offset      = OVIA_GetSampleOffset(Ovia);
            uint64_t BlockSize   = OVIA_GetBlockSize(Ovia);
            
            Audio_Types AudioType = AudioContainer_GetType(Audio);
            if (AudioType == AudioType_SInteger8) {
                int8_t **Samples  = (int8_t**) AudioContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < NumChannels; Channel++) {
                        BitBuffer_WriteBits(MSByteFirst, LSBitFirst, BitB, BitDepth, Samples[Channel][Sample]);
                        BitBuffer_Skip(BitB, 8 - (BitDepth % 8));
                    }
                }
            } else if (AudioType == AudioType_UInteger8) {
                uint8_t **Samples  = (uint8_t**) AudioContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < NumChannels; Channel++) {
                        BitBuffer_WriteBits(MSByteFirst, LSBitFirst, BitB, BitDepth, Samples[Channel][Sample]);
                        BitBuffer_Skip(BitB, 8 - (BitDepth % 8));
                    }
                }
            } else if (AudioType == AudioType_SInteger16) {
                int16_t **Samples  = (int16_t**) AudioContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < NumChannels; Channel++) {
                        BitBuffer_WriteBits(MSByteFirst, LSBitFirst, BitB, BitDepth, Samples[Channel][Sample]);
                        BitBuffer_Skip(BitB, 8 - (BitDepth % 8));
                    }
                }
            } else if (AudioType == AudioType_UInteger16) {
                uint16_t **Samples  = (uint16_t**) AudioContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < NumChannels; Channel++) {
                        BitBuffer_WriteBits(MSByteFirst, LSBitFirst, BitB, BitDepth, Samples[Channel][Sample]);
                        BitBuffer_Skip(BitB, 8 - (BitDepth % 8));
                    }
                }
            } else if (AudioType == AudioType_SInteger32) {
                int32_t **Samples  = (int32_t**) AudioContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < NumChannels; Channel++) {
                        BitBuffer_WriteBits(MSByteFirst, LSBitFirst, BitB, BitDepth, Samples[Channel][Sample]);
                        BitBuffer_Skip(BitB, 8 - (BitDepth % 8));
                    }
                }
            } else if (AudioType == AudioType_UInteger32) {
                uint32_t **Samples  = (uint32_t**) AudioContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < NumChannels; Channel++) {
                        BitBuffer_WriteBits(MSByteFirst, LSBitFirst, BitB, BitDepth, Samples[Channel][Sample]);
                        BitBuffer_Skip(BitB, 8 - (BitDepth % 8));
                    }
                }
            }
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        } else if (Audio == NULL) {
            Log(Log_ERROR, __func__, U8("AudioContainer Pointer is NULL"));
        }
    }
    
#ifdef __cplusplus
}
#endif
