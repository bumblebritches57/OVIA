#include "../../include/Private/AIFCommon.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    /*
     Like WAV, Chunks are padded if their size is not even, and the ChunkSize field contains the real size without the padding
     the ChunkSize field does NOT include the ChunkID or ChunkSize fields.
     */
    
    static void AIFReadCOMMChunk(AIFOptions *AIF, BitBuffer *BitB) {
        if (AIF != NULL && BitB != NULL) {
            AIF->NumChannels                   = BitBuffer_ReadBits(BitB, MSByteFirst, LSBitFirst, 16);
            AIF->NumSamples                    = BitBuffer_ReadBits(BitB, MSByteFirst, LSBitFirst, 32); // A SampleFrame is simply a single sample from all channels.
            AIF->BitDepth                      = BitBuffer_ReadBits(BitB, MSByteFirst, LSBitFirst, 16);
            AIF->SampleRate_Exponent           = 16446 - BitBuffer_ReadBits(BitB, MSByteFirst, LSBitFirst, 16);
            AIF->SampleRate_Mantissa           = BitBuffer_ReadBits(BitB, MSByteFirst, LSBitFirst, 64);
            if (AIF->SampleRate_Exponent >= 0) {
                AIF->SampleRate                = AIF->SampleRate_Mantissa << AIF->SampleRate_Exponent;
            } else {
                uint64_t NegatedExponent       = ~AIF->SampleRate_Exponent;
                AIF->SampleRate                = AIF->SampleRate_Mantissa + ((1 << (NegatedExponent - 1)) >> NegatedExponent);
            }
        } else if (AIF == NULL) {
            Log(Log_DEBUG, __func__, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_DEBUG, __func__, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    static void AIFReadNameChunk(AIFOptions *AIF, BitBuffer *BitB) {
        if (AIF != NULL && BitB != NULL) {
            uint32_t TitleSize                 = BitBuffer_ReadBits(BitB, MSByteFirst, LSBitFirst, 32);
            UTF8    *Title                     = BitBuffer_ReadUTF8(BitB, TitleSize);
            AIF->NumTags                      += 1;
            AIF->Tags[AIF->NumTags - 1]        = Title;
            AIF->TagTypes[AIF->NumTags - 1]    = TitleTag;
        } else if (AIF == NULL) {
            Log(Log_DEBUG, __func__, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_DEBUG, __func__, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    static void AIFReadAuthorChunk(AIFOptions *AIF, BitBuffer *BitB) {
        if (AIF != NULL && BitB != NULL) {
            uint32_t AuthorSize                = BitBuffer_ReadBits(BitB, MSByteFirst, LSBitFirst, 32);
            UTF8    *Author                    = BitBuffer_ReadUTF8(BitB, AuthorSize);
            AIF->NumTags                      += 1;
            AIF->Tags[AIF->NumTags - 1]        = Author;
            AIF->TagTypes[AIF->NumTags - 1]    = AuthorTag;
        } else if (AIF == NULL) {
            Log(Log_DEBUG, __func__, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_DEBUG, __func__, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    static void AIFReadAnnotationChunk(AIFOptions *AIF, BitBuffer *BitB) {
        if (AIF != NULL && BitB != NULL) {
            uint32_t AnnotationSize            = BitBuffer_ReadBits(BitB, MSByteFirst, LSBitFirst, 32);
            UTF8    *Annotation                = BitBuffer_ReadUTF8(BitB, AnnotationSize);
            AIF->NumTags                      += 1;
            AIF->Tags[AIF->NumTags - 1]        = Annotation;
            AIF->TagTypes[AIF->NumTags - 1]    = AnnotationTag;
        } else if (AIF == NULL) {
            Log(Log_DEBUG, __func__, UTF8String("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_DEBUG, __func__, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    void AIFReadMetadata(AIFOptions *AIF, BitBuffer *BitB) {
        if (AIF != NULL && BitB != NULL) {
            AIF->FileSize                      = BitBuffer_ReadBits(BitB, MSByteFirst, LSBitFirst, 32);
            AIFChunkIDs AIFFChunkID            = BitBuffer_ReadBits(BitB, MSByteFirst, LSBitFirst, 32);
            if (AIFFChunkID == AIF_AIFF || AIFFChunkID == AIF_AIFC) {
                AIFSubChunkIDs AIFFSubChunkID  = BitBuffer_ReadBits(BitB, MSByteFirst, LSBitFirst, 32);
                uint32_t AIFFSubChunkSize      = BitBuffer_ReadBits(BitB, MSByteFirst, LSBitFirst, 32);
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
                        AIF->SampleOffset = BitBuffer_ReadBits(BitB, MSByteFirst, LSBitFirst, 32);
                        AIF->BlockSize    = BitBuffer_ReadBits(BitB, MSByteFirst, LSBitFirst, 32);
                        break;
                }
            } else {
                Log(Log_DEBUG, __func__, UTF8String("Invalid ChunkID 0x%X"), AIFFChunkID);
            }
        } else if (AIF == NULL) {
            Log(Log_DEBUG, __func__, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_DEBUG, __func__, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    void AIFExtractSamples(AIFOptions *AIF, BitBuffer *BitB, Audio2DContainer *Audio) { // I should change this so that the user manages their own buffer
        if (AIF != NULL && BitB != NULL && Audio) {
            uint64_t BitDepth     = Bits2Bytes(Audio2DContainer_GetBitDepth(Audio), RoundingType_Up);
            uint64_t NumChannels  = Audio2DContainer_GetNumChannels(Audio);
            uint64_t NumSamples   = Audio2DContainer_GetNumSamples(Audio);
            if (BitDepth <= 8) {
                uint8_t **Samples = (uint8_t**) Audio2DContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < NumChannels; Channel++) {
                        Samples[Channel][Sample] = BitBuffer_ReadBits(BitB, MSByteFirst, LSBitFirst, BitDepth);
                    }
                }
            } else if (BitDepth > 8 && BitDepth <= 16) {
                uint16_t **Samples = (uint16_t**) Audio2DContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < NumChannels; Channel++) {
                        Samples[Channel][Sample] = BitBuffer_ReadBits(BitB, MSByteFirst, LSBitFirst, BitDepth);
                    }
                }
            } else if (BitDepth > 16 && BitDepth <= 32) {
                uint32_t **Samples = (uint32_t**) Audio2DContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < NumChannels; Channel++) {
                        Samples[Channel][Sample] = BitBuffer_ReadBits(BitB, MSByteFirst, LSBitFirst, BitDepth);
                    }
                }
            }
        } else if (AIF == NULL) {
            Log(Log_DEBUG, __func__, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_DEBUG, __func__, UTF8String("BitBuffer Pointer is NULL"));
        } else if (Audio == NULL) {
            Log(Log_DEBUG, __func__, UTF8String("Audio2DContainer Pointer is NULL"));
        }
    }
    
    static void RegisterDecoder_AIF(OVIA *Ovia) {
        Ovia->NumDecoders                                    += 1;
        uint64_t DecoderIndex                                 = Ovia->NumDecoders;
        Ovia->Decoders                                        = realloc(Ovia->Decoders, sizeof(OVIADecoder) * Ovia->NumDecoders);
        
        Ovia->Decoders[DecoderIndex].DecoderID                = CodecID_AIF;
        Ovia->Decoders[DecoderIndex].MediaType                = MediaType_Audio2D;
        Ovia->Decoders[DecoderIndex].NumMagicIDs              = 1;
        Ovia->Decoders[DecoderIndex].MagicIDOffset[0]         = 0;
        Ovia->Decoders[DecoderIndex].MagicIDSize[0]           = 4;
        Ovia->Decoders[DecoderIndex].MagicID[0]               = (uint8_t[4]) {0x46, 0x4F, 0x52, 0x4D};
        Ovia->Decoders[DecoderIndex].Function_Initialize[0]   = AIFOptions_Init;
        Ovia->Decoders[DecoderIndex].Function_Read[0]         = AIFReadMetadata;
        Ovia->Decoders[DecoderIndex].Function_Decode[0]       = AIFExtractSamples;
        Ovia->Decoders[DecoderIndex].Function_Deinitialize[0] = AIFOptions_Deinit;
    }
    
    static OVIACodecRegistry Register_AIFDecoder = {
        .Function_RegisterDecoder[CodecID_AIF - 1]   = RegisterDecoder_AIF,
    };
    
#ifdef __cplusplus
}
#endif
