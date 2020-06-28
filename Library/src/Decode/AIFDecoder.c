#include "../../include/Private/AIFCommon.h"

#if (PlatformIO_Language == PlatformIO_LanguageIsCXX)
extern "C" {
#endif
    
    /*
     Like WAV, Chunks are padded if their size is not even, and the ChunkSize field contains the real size without the padding
     the ChunkSize field does NOT include the ChunkID or ChunkSize fields.
     */
    
    static void AIFReadCOMMChunk(AIFOptions *AIF, BitBuffer *BitB) {
        if (AIF != NULL && BitB != NULL) {
            AIF->NumChannels                   = (uint16_t) BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsFarthest, BitIO_BitOrder_LSBit, 16);
            AIF->NumSamples                    = (uint32_t) BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsFarthest, BitIO_BitOrder_LSBit, 32); // A SampleFrame is simply a single sample from all channels.
            AIF->BitDepth                      = (uint16_t) BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsFarthest, BitIO_BitOrder_LSBit, 16);
            AIF->SampleRate_Exponent           = (uint16_t) 16446 - BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsFarthest, BitIO_BitOrder_LSBit, 16);
            AIF->SampleRate_Mantissa           = BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsFarthest, BitIO_BitOrder_LSBit, 64);
            if (AIF->SampleRate_Exponent >= 0) {
                AIF->SampleRate                = AIF->SampleRate_Mantissa << AIF->SampleRate_Exponent;
            } else {
                uint64_t NegatedExponent       = ~AIF->SampleRate_Exponent;
                AIF->SampleRate                = AIF->SampleRate_Mantissa + ((1 << (NegatedExponent - 1)) >> NegatedExponent);
            }
        } else if (AIF == NULL) {
            Log(Severity_DEBUG, UnicodeIOTypes_FunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Severity_DEBUG, UnicodeIOTypes_FunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    static void AIFReadNameChunk(AIFOptions *AIF, BitBuffer *BitB) {
        if (AIF != NULL && BitB != NULL) {
            uint32_t TitleSize                 = (uint32_t) BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsFarthest, BitIO_BitOrder_LSBit, 32);
            UTF8    *Title                     = BitBuffer_ReadUTF8(BitB, TitleSize);
            AIF->NumTags                      += 1;
            AIF->Tags[AIF->NumTags - 1]        = *Title;
            AIF->TagTypes[AIF->NumTags - 1]    = TitleTag;
        } else if (AIF == NULL) {
            Log(Severity_DEBUG, UnicodeIOTypes_FunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Severity_DEBUG, UnicodeIOTypes_FunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    static void AIFReadAuthorChunk(AIFOptions *AIF, BitBuffer *BitB) {
        if (AIF != NULL && BitB != NULL) {
            uint32_t AuthorSize                = (uint32_t) BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsFarthest, BitIO_BitOrder_LSBit, 32);
            UTF8    *Author                    = BitBuffer_ReadUTF8(BitB, AuthorSize);
            AIF->NumTags                      += 1;
            AIF->Tags[AIF->NumTags - 1]        = *Author;
            AIF->TagTypes[AIF->NumTags - 1]    = AuthorTag;
        } else if (AIF == NULL) {
            Log(Severity_DEBUG, UnicodeIOTypes_FunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Severity_DEBUG, UnicodeIOTypes_FunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    static void AIFReadAnnotationChunk(AIFOptions *AIF, BitBuffer *BitB) {
        if (AIF != NULL && BitB != NULL) {
            uint32_t AnnotationSize            = (uint32_t) BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsFarthest, BitIO_BitOrder_LSBit, 32);
            UTF8    *Annotation                = BitBuffer_ReadUTF8(BitB, AnnotationSize);
            AIF->NumTags                      += 1;
            AIF->Tags[AIF->NumTags - 1]        = *Annotation;
            AIF->TagTypes[AIF->NumTags - 1]    = AnnotationTag;
        } else if (AIF == NULL) {
            Log(Severity_DEBUG, UnicodeIOTypes_FunctionName, UTF8String("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Severity_DEBUG, UnicodeIOTypes_FunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    void AIFReadMetadata(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            AIFOptions *AIF                    = Options;
            AIF->FileSize                      = (uint32_t) BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsFarthest, BitIO_BitOrder_LSBit, 32);
            AIFChunkIDs AIFFChunkID            = (uint32_t) BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsFarthest, BitIO_BitOrder_LSBit, 32);
            if (AIFFChunkID == AIF_AIFF || AIFFChunkID == AIF_AIFC) {
                AIFSubChunkIDs AIFFSubChunkID  = (uint32_t) BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsFarthest, BitIO_BitOrder_LSBit, 32);
                uint32_t AIFFSubChunkSize      = (uint32_t) BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsFarthest, BitIO_BitOrder_LSBit, 32);
                uint32_t SampleOffset          = 0;
                uint32_t BlockSize             = 0;
                switch (AIFFSubChunkID) {
                    case AIF_AAPL:
                        BitBuffer_Seek(BitB, Bytes2Bits(AIFFSubChunkSize));
                        AIFSkipPadding(BitB, AIFFSubChunkSize);
                        break;
                    case AIF_AESD:
                        BitBuffer_Seek(BitB, Bytes2Bits(AIFFSubChunkSize));
                        AIFSkipPadding(BitB, AIFFSubChunkSize);
                        break;
                    case AIF_ANNO:
                        AIFReadAnnotationChunk(AIF, BitB);
                        AIFSkipPadding(BitB, AIFFSubChunkSize);
                        break;
                    case AIF_AUTH:
                        AIFReadAuthorChunk(AIF, BitB);
                        AIFSkipPadding(BitB, AIFFSubChunkSize);
                        break;
                    case AIF_COMM:
                        AIFReadCOMMChunk(AIF, BitB);
                        AIFSkipPadding(BitB, AIFFSubChunkSize);
                        break;
                    case AIF_COMT:
                        BitBuffer_Seek(BitB, Bytes2Bits(AIFFSubChunkSize));
                        AIFSkipPadding(BitB, AIFFSubChunkSize);
                        break;
                    case AIF_ID3:
                        BitBuffer_Seek(BitB, Bytes2Bits(AIFFSubChunkSize));
                        AIFSkipPadding(BitB, AIFFSubChunkSize);
                        break;
                    case AIF_INST:
                        BitBuffer_Seek(BitB, Bytes2Bits(AIFFSubChunkSize));
                        AIFSkipPadding(BitB, AIFFSubChunkSize);
                        break;
                    case AIF_MARK:
                        BitBuffer_Seek(BitB, Bytes2Bits(AIFFSubChunkSize));
                        AIFSkipPadding(BitB, AIFFSubChunkSize);
                        break;
                    case AIF_MIDI:
                        BitBuffer_Seek(BitB, Bytes2Bits(AIFFSubChunkSize));
                        AIFSkipPadding(BitB, AIFFSubChunkSize);
                        break;
                    case AIF_NAME:
                        AIFReadNameChunk(AIF, BitB);
                        AIFSkipPadding(BitB, AIFFSubChunkSize);
                        break;
                    case AIF_SSND:
                        AIF->SampleOffset = (uint32_t) BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsFarthest, BitIO_BitOrder_LSBit, 32);
                        AIF->BlockSize    = (uint32_t) BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsFarthest, BitIO_BitOrder_LSBit, 32);
                        break;
                }
            } else {
                Log(Severity_DEBUG, UnicodeIOTypes_FunctionName, UTF8String("Invalid ChunkID 0x%X"), AIFFChunkID);
            }
        } else if (Options == NULL) {
            Log(Severity_DEBUG, UnicodeIOTypes_FunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Severity_DEBUG, UnicodeIOTypes_FunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    void *AIFExtractSamples(void *Options, BitBuffer *BitB) { // I should change this so that the user manages their own buffer
        Audio2DContainer *Audio = NULL;
        if (Options != NULL && BitB != NULL) {
            AIFOptions *AIF       = Options;
            uint64_t BitDepth     = Bits2Bytes(AIF->BitDepth, RoundingType_Up);
            uint64_t NumChannels  = AIF->NumChannels;
            uint64_t NumSamples   = AIF->NumSamples;
            
            if (BitDepth <= 8) {
                uint8_t **Samples = (uint8_t**) Audio2DContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < NumChannels; Channel++) {
                        Samples[Channel][Sample] = (uint8_t) BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsFarthest, BitIO_BitOrder_LSBit, BitDepth);
                    }
                }
            } else if (BitDepth > 8 && BitDepth <= 16) {
                uint16_t **Samples = (uint16_t**) Audio2DContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < NumChannels; Channel++) {
                        Samples[Channel][Sample] = (uint16_t) BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsFarthest, BitIO_BitOrder_LSBit, BitDepth);
                    }
                }
            } else if (BitDepth > 16 && BitDepth <= 32) {
                uint32_t **Samples = (uint32_t**) Audio2DContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < NumChannels; Channel++) {
                        Samples[Channel][Sample] = (uint32_t) BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsFarthest, BitIO_BitOrder_LSBit, BitDepth);
                    }
                }
            }
        } else if (Options == NULL) {
            Log(Severity_DEBUG, UnicodeIOTypes_FunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Severity_DEBUG, UnicodeIOTypes_FunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
        return Audio;
    }
    
    static const OVIADecoder AIFDecoder = {
        .Function_Initialize   = AIFOptions_Init,
        .Function_Decode       = AIFExtractSamples,
        .Function_Read         = AIFReadMetadata,
        .Function_Deinitialize = AIFOptions_Deinit,
        .MagicIDs              = &AIFMagicIDs,
        .Extensions            = &AIFExtensions,
        .MediaType             = MediaType_Audio2D,
        .DecoderID             = CodecID_PCMAudio,
    };
    
#if (PlatformIO_Language == PlatformIO_LanguageIsCXX)
}
#endif
