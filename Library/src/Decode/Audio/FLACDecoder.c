#include "../../../../Dependencies/FoundationIO/libFoundationIO/include/Macros.h"
#include "../../../include/Private/Audio/FLACCommon.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    /*!
     @abstract Reads and OVIAs MIME Base64
     */
    void ReadBase64(BitBuffer *BitB, uint8_t *Buffer, uint64_t BufferSize, uint64_t LineLength) {
        
    }
    
    
    /* Start User facing functions */
    
    /*!
     @abstract          Copies frames from the stream pointed to by BitB, to OutputFrameBuffer (which needs to be freed by you)
     @param     StartFrame IS NOT zero indexed.
     */
    /*
     uint8_t *CopyFLACFrame(OVIA *Ovia, BitBuffer *BitB) { // for apps that don't care about metadata
     
     // scan stream for FrameMagic, once found, start counting until you hit StartFrame
     
     // See if there's a seektable, if so use that to get as close as possible, otherwise scan byte by byte.
     // Which means we need metadata flags.
     //
     for (size_t StreamByte = 0; StreamByte < BitB->FileSize; StreamByte++) {
     uint16_t Marker = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 14, true);
     if (Marker == FLACFrameMagic) {
     // Get frame size by reading ahead until you find either the end of the stream, or another FLACFrameMagic
     // then skip back, and read it all.
     // OR could we somehow just read it until we got to the end of the frame, and
     size_t FrameSizeInBits = 0;
     while (BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 14, true) != FLACFrameMagic || (BitB->FilePosition + BitB->BitsUnavailable) < BitB->FileSize) {
     FrameSizeInBits += 14;
     }
     BitBuffer_Seek(BitB, FrameSizeInBits);
     // Create buffer that's FrameSizeInBits, and then start copying each byte into the buffer
     uint8_t FrameBuffer[1];
     realloc(FrameBuffer, Bits2Bytes(FrameSizeInBits, true));
     for (size_t FrameByte = 0; FrameByte < Bits2Bytes(FrameSizeInBits, true); FrameByte++) {
     FrameByte[FrameByte] = BitB->Buffer[FrameByte];
     }
     }
     }
     
     return NULL;
     }
     */
    
    /* End User Facing Functions */
    
    void OVIA_FLAC_Stream_Read(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            uint16_t Marker = BitBuffer_PeekBits(MSByteFirst, LSBitFirst, BitB, 14);
            if (Marker == FLACFrameMagic) {
                BitBuffer_Seek(BitB, 14);
                OVIA_FLAC_Frame_Read(Ovia, BitB);
            } else {
                FLACParseMetadata(Ovia, BitB);
            }
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void OVIA_FLAC_Frame_Read(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            BitBuffer_Seek(BitB, 1);                                                                          // 0
            OVIA_FLAC_Frame_SetBlockType(Ovia, BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 1));         // 0
            OVIA_FLAC_Frame_SetCodedBlockSize(Ovia, BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 4));    // 12
            OVIA_FLAC_Frame_SetCodedSampleRate(Ovia, BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 4));   // 9
            OVIA_FLAC_Frame_SetChannelLayout(Ovia, BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 4) + 1); // 2
            OVIA_FLAC_Frame_SetCodedBitDepth(Ovia, BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 3));     // 4
            BitBuffer_Seek(BitB, 1);                                                                          // 0
            
            if (OVIA_FLAC_Frame_GetBlockType(Ovia) == FixedBlockSize) {
                uint8_t  Bytes2Read                 = BitBuffer_ReadUnary(MSByteFirst, LSBitFirst, BitB, CountUnary, 0) + 1;
                uint64_t FrameNumber                = BitBuffer_ReadUTF8(BitB, Bytes2Read); // 0
                OVIA_FLAC_Frame_SetFrameNumber(Ovia, FrameNumber);
            } else if (OVIA_FLAC_Frame_GetBlockType(Ovia) == VariableBlockSize) {
                uint8_t  Bytes2Read                 = BitBuffer_ReadUnary(MSByteFirst, LSBitFirst, BitB, CountUnary, 0) + 1;
                uint64_t SampleNumber               = BitBuffer_ReadUTF8(BitB, Bytes2Read);
                OVIA_FLAC_Frame_SetSampleNumber(Ovia, SampleNumber);
            }
            
            if (OVIA_FLAC_Frame_GetCodedBlockSize(Ovia) == 6) {
                OVIA_FLAC_Frame_SetBlockSize(Ovia, BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 8));
            } else if (OVIA_FLAC_Frame_GetCodedBlockSize(Ovia) == 7) {
                OVIA_FLAC_Frame_SetBlockSize(Ovia, BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 16));
            } else {
                // What do we do?
            }
            
            if (OVIA_FLAC_Frame_GetCodedSampleRate(Ovia) == 12) {
                OVIA_FLAC_Frame_SetSampleRate(Ovia, BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 8) * 1000);
            } else if (OVIA_FLAC_Frame_GetCodedSampleRate(Ovia) == 13) {
                OVIA_FLAC_Frame_SetSampleRate(Ovia, BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 16));
            } else if (OVIA_FLAC_Frame_GetCodedSampleRate(Ovia) == 14) {
                OVIA_FLAC_Frame_SetSampleRate(Ovia, BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 16) * 10);
            } else {
                // What do we do?
            }
            
            for (uint8_t Channel = 0; Channel < OVIA_GetNumChannels(Ovia); Channel++) { // 2 channels
                OVIA_FLAC_SubFrame_Read(Ovia, BitB, Channel);
            }
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void OVIA_FLAC_SubFrame_Read(OVIA *Ovia, BitBuffer *BitB, uint8_t Channel) { // 2 channels
        if (Ovia != NULL && BitB != NULL) {
            BitBuffer_Seek(BitB, 1); // Reserved
            uint8_t SubFrameType = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 6);
            OVIA_FLAC_SubFrame_SetType(Ovia, SubFrameType);
            
            bool WastedBitsFlag = BitBuffer_ReadBits(MSByteFirst, MSBitFirst, BitB, 1);
            if (WastedBitsFlag == Yes) {
                uint8_t WastedBits = BitBuffer_ReadUnary(MSByteFirst, MSBitFirst, BitB, TruncatedCountUnary, 0);
                OVIA_FLAC_SubFrame_SetWastedBits(Ovia, WastedBitsFlag, WastedBits);
            }
            
            if (SubFrameType == Subframe_Verbatim) { // PCM
                OVIA_FLAC_SubFrame_Verbatim(Ovia, BitB);
            } else if (SubFrameType == Subframe_Constant) {
                OVIA_FLAC_SubFrame_Constant(Ovia, BitB);
            } else if (SubFrameType >= Subframe_Fixed && SubFrameType <= Subframe_LPC) { // Fixed
                OVIA_FLAC_SubFrame_Fixed(Ovia, BitB);
            } else if (SubFrameType >= Subframe_LPC) { // LPC
                OVIA_FLAC_SubFrame_LPC(Ovia, BitB, Channel);
            } else {
                Log(Log_ERROR, __func__, U8("Invalid Subframe type: %d"), SubFrameType);
            }
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void OVIA_FLAC_SubFrame_Verbatim(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            // First Raw  Sample L channel = 0x5E83
            // First Raw  Sample R channel = 0x7139
            // First FLAC Sample L channel = 0x5ED7, aka 87 more?
            uint64_t        NumSamples  = OVIA_FLAC_Frame_GetCodedBlockSize(Ovia);
            uint8_t         BitDepth    = Bits2Bytes(OVIA_GetBitDepth(Ovia), Yes);
            uint8_t         NumChannels = OVIA_GetNumChannels(Ovia);
            AudioContainer *Audio       = OVIA_GetAudioContainerPointer(Ovia);
            Audio_Types     Type        = AudioContainer_GetType(Audio);
            if (Type == (AudioType_Unsigned | AudioType_Integer8)) {
                uint8_t **Array         = AudioContainer_GetArray(Audio);
                for (uint8_t Channel = 0; Channel < NumChannels; Channel++) {
                    for (uint16_t Sample = 0; Sample < NumSamples; Sample++) {
                        Array[Channel][Sample] = BitBuffer_ReadBits(MSByteFirst, MSBitFirst, BitB, BitDepth);
                    }
                }
            } else if (Type == (AudioType_Signed | AudioType_Integer8)) {
                int8_t  **Array         = AudioContainer_GetArray(Audio);
                for (uint8_t Channel = 0; Channel < NumChannels; Channel++) {
                    for (uint16_t Sample = 0; Sample < NumSamples; Sample++) {
                        Array[Channel][Sample] = BitBuffer_ReadBits(MSByteFirst, MSBitFirst, BitB, BitDepth);
                    }
                }
            } else if (Type == (AudioType_Unsigned | AudioType_Integer16)) {
                uint16_t **Array        = AudioContainer_GetArray(Audio);
                for (uint8_t Channel = 0; Channel < NumChannels; Channel++) {
                    for (uint16_t Sample = 0; Sample < NumSamples; Sample++) {
                        Array[Channel][Sample] = BitBuffer_ReadBits(MSByteFirst, MSBitFirst, BitB, BitDepth);
                    }
                }
            } else if (Type == (AudioType_Signed | AudioType_Integer16)) {
                int16_t  **Array        = AudioContainer_GetArray(Audio);
                for (uint8_t Channel = 0; Channel < NumChannels; Channel++) {
                    for (uint16_t Sample = 0; Sample < NumSamples; Sample++) {
                        Array[Channel][Sample] = BitBuffer_ReadBits(MSByteFirst, MSBitFirst, BitB, BitDepth);
                    }
                }
            } else if (Type == (AudioType_Unsigned | AudioType_Integer32)) {
                uint32_t **Array        = AudioContainer_GetArray(Audio);
                for (uint8_t Channel = 0; Channel < NumChannels; Channel++) {
                    for (uint16_t Sample = 0; Sample < NumSamples; Sample++) {
                        Array[Channel][Sample] = BitBuffer_ReadBits(MSByteFirst, MSBitFirst, BitB, BitDepth);
                    }
                }
            } else if (Type == (AudioType_Signed | AudioType_Integer32)) {
                int32_t  **Array        = AudioContainer_GetArray(Audio);
                for (uint8_t Channel = 0; Channel < NumChannels; Channel++) {
                    for (uint16_t Sample = 0; Sample < NumSamples; Sample++) {
                        Array[Channel][Sample] = BitBuffer_ReadBits(MSByteFirst, MSBitFirst, BitB, BitDepth);
                    }
                }
            }
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void OVIA_FLAC_SubFrame_Constant(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            uint64_t        NumSamples  = OVIA_FLAC_Frame_GetCodedBlockSize(Ovia);
            uint8_t         BitDepth    = Bits2Bytes(OVIA_GetBitDepth(Ovia), Yes);
            uint8_t         NumChannels = OVIA_GetNumChannels(Ovia);
            AudioContainer *Audio       = OVIA_GetAudioContainerPointer(Ovia);
            Audio_Types     Type        = AudioContainer_GetType(Audio);
            int64_t         Constant    = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, Ovia, BitB_GetBitDepth(Ovia));
            
            if (Type == (AudioType_Unsigned | AudioType_Integer8)) {
                uint8_t **Array         = AudioContainer_GetArray(Audio);
                for (uint8_t Channel = 0; Channel < NumChannels; Channel++) {
                    for (uint16_t Sample = 0; Sample < NumSamples; Sample++) {
                        Array[Channel][Sample] = Constant;
                    }
                }
            } else if (Type == (AudioType_Signed | AudioType_Integer8)) {
                int8_t  **Array         = AudioContainer_GetArray(Audio);
                for (uint8_t Channel = 0; Channel < NumChannels; Channel++) {
                    for (uint16_t Sample = 0; Sample < NumSamples; Sample++) {
                        Array[Channel][Sample] = Constant;
                    }
                }
            } else if (Type == (AudioType_Unsigned | AudioType_Integer16)) {
                uint16_t **Array        = AudioContainer_GetArray(Audio);
                for (uint8_t Channel = 0; Channel < NumChannels; Channel++) {
                    for (uint16_t Sample = 0; Sample < NumSamples; Sample++) {
                        Array[Channel][Sample] = Constant;
                    }
                }
            } else if (Type == (AudioType_Signed | AudioType_Integer16)) {
                int16_t  **Array        = AudioContainer_GetArray(Audio);
                for (uint8_t Channel = 0; Channel < NumChannels; Channel++) {
                    for (uint16_t Sample = 0; Sample < NumSamples; Sample++) {
                        Array[Channel][Sample] = Constant;
                    }
                }
            } else if (Type == (AudioType_Unsigned | AudioType_Integer32)) {
                uint32_t **Array        = AudioContainer_GetArray(Audio);
                for (uint8_t Channel = 0; Channel < NumChannels; Channel++) {
                    for (uint16_t Sample = 0; Sample < NumSamples; Sample++) {
                        Array[Channel][Sample] = Constant;
                    }
                }
            } else if (Type == (AudioType_Signed | AudioType_Integer32)) {
                int32_t  **Array        = AudioContainer_GetArray(Audio);
                for (uint8_t Channel = 0; Channel < NumChannels; Channel++) {
                    for (uint16_t Sample = 0; Sample < NumSamples; Sample++) {
                        Array[Channel][Sample] = Constant;
                    }
                }
            }
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void OVIA_FLAC_SubFrame_Fixed(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            uint64_t        NumSamples  = OVIA_FLAC_Frame_GetCodedBlockSize(Ovia);
            uint8_t         BitDepth    = Bits2Bytes(OVIA_GetBitDepth(Ovia), Yes);
            uint8_t         NumChannels = OVIA_GetNumChannels(Ovia);
            AudioContainer *Audio       = OVIA_GetAudioContainerPointer(Ovia);
            Audio_Types     Type        = AudioContainer_GetType(Audio);
            
            if (Type == (AudioType_Unsigned | AudioType_Integer8)) {
                uint8_t **Array         = AudioContainer_GetArray(Audio);
                for (uint8_t Channel = 0; Channel < NumChannels; Channel++) {
                    for (uint16_t Sample = 0; Sample < NumSamples; Sample++) {
                        Array[Channel][Sample] = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, Ovia, BitB_GetBitDepth(Ovia));
                    }
                }
            } else if (Type == (AudioType_Signed | AudioType_Integer8)) {
                int8_t  **Array         = AudioContainer_GetArray(Audio);
                for (uint8_t Channel = 0; Channel < NumChannels; Channel++) {
                    for (uint16_t Sample = 0; Sample < NumSamples; Sample++) {
                        Array[Channel][Sample] = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, Ovia, BitB_GetBitDepth(Ovia));
                    }
                }
            } else if (Type == (AudioType_Unsigned | AudioType_Integer16)) {
                uint16_t **Array        = AudioContainer_GetArray(Audio);
                for (uint8_t Channel = 0; Channel < NumChannels; Channel++) {
                    for (uint16_t Sample = 0; Sample < NumSamples; Sample++) {
                        Array[Channel][Sample] = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, Ovia, BitB_GetBitDepth(Ovia));
                    }
                }
            } else if (Type == (AudioType_Signed | AudioType_Integer16)) {
                int16_t  **Array        = AudioContainer_GetArray(Audio);
                for (uint8_t Channel = 0; Channel < NumChannels; Channel++) {
                    for (uint16_t Sample = 0; Sample < NumSamples; Sample++) {
                        Array[Channel][Sample] = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, Ovia, BitB_GetBitDepth(Ovia));
                    }
                }
            } else if (Type == (AudioType_Unsigned | AudioType_Integer32)) {
                uint32_t **Array        = AudioContainer_GetArray(Audio);
                for (uint8_t Channel = 0; Channel < NumChannels; Channel++) {
                    for (uint16_t Sample = 0; Sample < NumSamples; Sample++) {
                        Array[Channel][Sample] = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, Ovia, BitB_GetBitDepth(Ovia));
                    }
                }
            } else if (Type == (AudioType_Signed | AudioType_Integer32)) {
                int32_t  **Array        = AudioContainer_GetArray(Audio);
                for (uint8_t Channel = 0; Channel < NumChannels; Channel++) {
                    for (uint16_t Sample = 0; Sample < NumSamples; Sample++) {
                        Array[Channel][Sample] = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, Ovia, BitB_GetBitDepth(Ovia));
                    }
                }
            }
            OVIA_FLAC_Decode_Residual(Ovia, BitB);
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void OVIA_FLAC_SubFrame_LPC(OVIA *Ovia, BitBuffer *BitB, uint8_t Channel) { // 4 0's
        if (Ovia != NULL && BitB != NULL) {
            uint64_t        NumSamples  = OVIA_FLAC_Frame_GetCodedBlockSize(Ovia);
            uint8_t         BitDepth    = Bits2Bytes(OVIA_GetBitDepth(Ovia), Yes);
            uint8_t         NumChannels = OVIA_GetNumChannels(Ovia);
            AudioContainer *Audio       = OVIA_GetAudioContainerPointer(Ovia);
            Audio_Types     Type        = AudioContainer_GetType(Audio);
            
            if (Type == (AudioType_Unsigned | AudioType_Integer8)) {
                uint8_t **Array         = AudioContainer_GetArray(Audio);
                for (uint8_t Channel = 0; Channel < NumChannels; Channel++) {
                    for (uint16_t Sample = 0; Sample < NumSamples; Sample++) {
                        Array[Channel][Sample] = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, Ovia, BitB_GetBitDepth(Ovia));
                    }
                }
            } else if (Type == (AudioType_Signed | AudioType_Integer8)) {
                int8_t  **Array         = AudioContainer_GetArray(Audio);
                for (uint8_t Channel = 0; Channel < NumChannels; Channel++) {
                    for (uint16_t Sample = 0; Sample < NumSamples; Sample++) {
                        Array[Channel][Sample] = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, Ovia, BitB_GetBitDepth(Ovia));
                    }
                }
            } else if (Type == (AudioType_Unsigned | AudioType_Integer16)) {
                uint16_t **Array        = AudioContainer_GetArray(Audio);
                for (uint8_t Channel = 0; Channel < NumChannels; Channel++) {
                    for (uint16_t Sample = 0; Sample < NumSamples; Sample++) {
                        Array[Channel][Sample] = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, Ovia, BitB_GetBitDepth(Ovia));
                    }
                }
            } else if (Type == (AudioType_Signed | AudioType_Integer16)) {
                int16_t  **Array        = AudioContainer_GetArray(Audio);
                for (uint8_t Channel = 0; Channel < NumChannels; Channel++) {
                    for (uint16_t Sample = 0; Sample < NumSamples; Sample++) {
                        Array[Channel][Sample] = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, Ovia, BitB_GetBitDepth(Ovia));
                    }
                }
            } else if (Type == (AudioType_Unsigned | AudioType_Integer32)) {
                uint32_t **Array        = AudioContainer_GetArray(Audio);
                for (uint8_t Channel = 0; Channel < NumChannels; Channel++) {
                    for (uint16_t Sample = 0; Sample < NumSamples; Sample++) {
                        Array[Channel][Sample] = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, Ovia, BitB_GetBitDepth(Ovia));
                    }
                }
            } else if (Type == (AudioType_Signed | AudioType_Integer32)) {
                int32_t  **Array        = AudioContainer_GetArray(Audio);
                for (uint8_t Channel = 0; Channel < NumChannels; Channel++) {
                    for (uint16_t Sample = 0; Sample < OVIA_GetBitDepth(Ovia) * OVIA_FLAC_LPC_GetLPCOrder(Ovia); Sample++) {
                        Array[Channel][Sample] = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, Ovia, BitB_GetBitDepth(Ovia));
                    }
                }
            }
            
            OVIA_FLAC_LPC_SetLPCPrecision(Ovia, BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 4) + 1);
            OVIA_FLAC_LPC_SetLPCShift(Ovia, BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 5));
            OVIA_FLAC_LPC_SetNumLPCCoeffs(Ovia, OVIA_FLAC_LPC_GetLPCPrecision(Ovia) * OVIA_FLAC_LPC_GetLPCOrder(Ovia));
            
            for (uint16_t LPCCoefficent = 0; LPCCoefficent < OVIA_FLAC_LPC_GetNumLPCCoeffs(Ovia); LPCCoefficent++) {
                OVIA_FLAC_LPC_SetLPCCoeff(Ovia, LPCCoefficent, BitBuffer_ReadBits(MSByteFirst, MSBitFirst, Ovia, BitB_FLAC_LPC_GetLPCPrecision(Ovia) + 1));
            }
            OVIA_FLAC_Decode_Residual(Ovia, BitB);
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void OVIA_FLAC_Decode_Residual(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            uint8_t RICEPartitionType = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 2);
            
            OVIA_FLAC_LPC_SetRICEPartitionType(Ovia, RICEPartitionType);
            if (RICEPartitionType == RICE1) {
                OVIA_FLAC_Decode_RICE1(Ovia, BitB);
            } else if (RICEPartitionType == RICE2) {
                OVIA_FLAC_Decode_RICE2(Ovia, BitB);
            }
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void OVIA_FLAC_Decode_RICE1(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            uint8_t PartitionOrder = BitBuffer_ReadBits(MSByteFirst, MSBitFirst, BitB, 4);
            OVIA_FLAC_LPC_SetRICEPartitionOrder(Ovia, PartitionOrder);
            
            for (uint8_t Partition = 0; Partition < PartitionOrder; Partition++) {
                uint8_t Parameter = 0;
                Parameter         = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 4) + 5;
                OVIA_FLAC_LPC_SetRICEParameter(Ovia, Partition, Parameter);
                if (Parameter == 20) {
                    // Escape code, meaning the partition is in unencoded binary form using n bits per sample; n follows as a 5-bit number.
                } else {
                    if (OVIA_FLAC_LPC_GetRICEPartitionOrder(Ovia) == 0) {
                        Parameter = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, Ovia, BitB_GetBlockSize(Ovia) - OVIA_FLAC_LPC_GetLPCOrder(Ovia));
                        OVIA_FLAC_LPC_SetRICEParameter(Ovia, Partition, Parameter);
                    } else if (OVIA_FLAC_LPC_GetRICEPartitionOrder(Ovia) > 0) {
                        Parameter = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, (OVIA_GetBlockSize(Ovia) / pow(2, OVIA_FLAC_LPC_GetRICEPartitionOrder(Ovia))));
                        OVIA_FLAC_LPC_SetRICEParameter(Ovia, Partition, Parameter);
                    } else {
                        Parameter = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, (OVIA_GetBlockSize(Ovia) / pow(2, OVIA_FLAC_LPC_GetRICEPartitionOrder(Ovia) - OVIA_FLAC_LPC_GetLPCOrder(Ovia))));
                        OVIA_FLAC_LPC_SetRICEParameter(Ovia, Partition, Parameter);
                    }
                }
            }
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void OVIA_FLAC_Decode_RICE2(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            for (uint8_t Partition = 0; Partition < OVIA_FLAC_LPC_GetRICEPartitionOrder(Ovia); Partition++) {
                uint8_t Parameter = 0;
                Parameter         = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 5) + 5;
                
                OVIA_FLAC_LPC_SetRICEParameter(Ovia, Partition, Parameter);
                uint8_t RiceParameter = OVIA_FLAC_LPC_GetRICEParameter(Ovia, Partition);
                if (RiceParameter == 36) {
                    // Escape code, meaning the partition is in unencoded binary form using n bits per sample; n follows as a 5-bit number.
                } else {
                    if (OVIA_FLAC_LPC_GetRICEPartitionOrder(Ovia) == 0) {
                        Parameter = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, Ovia, BitB_GetBlockSize(Ovia) - OVIA_FLAC_LPC_GetLPCOrder(Ovia));
                        OVIA_FLAC_LPC_SetRICEParameter(Ovia, Partition, Parameter);
                    } else if (OVIA_FLAC_LPC_GetRICEPartitionOrder(Ovia) > 0) {
                        Parameter = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, Ovia, BitB_GetBlockSize(Ovia) / pow(2, OVIA_FLAC_LPC_GetRICEPartitionOrder(Ovia)));
                        OVIA_FLAC_LPC_SetRICEParameter(Ovia, Partition, Parameter);
                    } else {
                        Parameter = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, (OVIA_GetBlockSize(Ovia) / pow(2, OVIA_FLAC_LPC_GetRICEPartitionOrder(Ovia) - OVIA_FLAC_LPC_GetLPCOrder(Ovia))));
                        OVIA_FLAC_LPC_SetRICEParameter(Ovia, Partition, Parameter);
                    }
                }
            }
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    uint8_t GetBlockSizeInSamples(uint8_t BlockSize) {
        uint16_t SamplesInBlock = 0;
        if (BlockSize == 1) {
            SamplesInBlock = 192;
        } else if (BlockSize >= 2 && BlockSize <= 5) {
            SamplesInBlock = (576 * pow(2, BlockSize - 2)); // 576/1152/2304/4608, pow(2, (BlockSize - 2))
        } else if (BlockSize == 6) {
            // get 8 bit block from the end of the header
        } else if (BlockSize == 7) {
            // get 16 bit block from the end of the header
        } else if (BlockSize >= 8 && BlockSize <= 15) {
            SamplesInBlock = (256 * pow(2, (BlockSize - 8))); // 256/512/1024/2048/4096/8192/16384/32768
                                                              // 256 * pow(2, 4)
            // 256 * pow(2, 6) 256 * 64 = 16384
        } else {
            // Reserved
        }
        return SamplesInBlock;
    }
    
    void FLACOVIALPC(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
        
        // Basically you use the warmup samples in Ovia->OVIAdSamples, along with the info in Ovia->LPC to deocde the file by using summation.
        // LPC is lossy, which is why you use filters to reduce the size of the residual.
        
        // I need 2 loops, one for the warmup samples, and one for the LPC encoded samples.
        
        
        // Original algorithm: X^
    }
    
    void FLACParseMetadata(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            bool     LastMetadataBlock       = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 1);  // true
            uint8_t  MetadataBlockType       = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 7);  // 1
            uint32_t MetadataSize            = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 24); // 4123 Does NOT count the 2 fields above.
            
            switch (MetadataBlockType) {
                case StreamInfo:
                    OVIA_FLAC_StreamInfo_Parse(Ovia, BitB);
                    break;
                case Padding:
                    OVIA_FLAC_SkipPadding(Ovia, BitB, MetadataSize);
                    break;
                case Custom:
                    OVIA_FLAC_SkipCustom(Ovia, BitB, MetadataSize);
                    break;
                case SeekTable:
                    OVIA_FLAC_SeekTable_Read(Ovia, BitB, MetadataSize);
                    break;
                case VorbisComment:
                    OVIA_FLAC_Vorbis_Parse(Ovia, BitB);
                    break;
                case Cuesheet:
                    OVIA_FLAC_CUE_Parse(Ovia, BitB);
                    break;
                case Picture:
                    FLACParsePicture(Ovia, BitB);
                    break;
                default:
                    Log(Log_ERROR, __func__, U8("Invalid Metadata Type: %d\n"), MetadataBlockType);
                    break;
            }
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void OVIA_FLAC_StreamInfo_Parse(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            OVIA_FLAC_SetMinBlockSize(Ovia, BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 16));    // 4096
            OVIA_FLAC_SetMaxBlockSize(Ovia, BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 16));    // 4096
            OVIA_FLAC_SetMinFrameSize(Ovia, BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 24));    // 839
            OVIA_FLAC_SetMaxFrameSize(Ovia, BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 24));    // 13109
            OVIA_SetSampleRate(Ovia,        BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 20));    // 44100
            OVIA_SetNumChannels(Ovia,       BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 3) + 1); // 2
            OVIA_SetBitDepth(Ovia,          BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 5) + 1); // 16
            OVIA_SetNumSamples(Ovia,        BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 36));    // 0x00083B3D4 = 8,631,252
            uint8_t *MD5 = calloc(16, sizeof(uint8_t));
            if (MD5 != NULL) {
                for (uint8_t MD5Byte = 0; MD5Byte < 16; MD5Byte++) { // 0x828C0962092D4FDEAA23DFF9BA13E0C0
                    MD5[MD5Byte] = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 8);
                }
                OVIA_FLAC_SetMD5(Ovia, MD5);
            } else {
                BitBuffer_Seek(BitB, 128);
            }
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void OVIA_FLAC_SkipPadding(OVIA *Ovia, BitBuffer *BitB, uint32_t ChunkSize) { // 8192
        if (Ovia != NULL && BitB != NULL) {
            BitBuffer_Seek(BitB, Bytes2Bits(ChunkSize));
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void OVIA_FLAC_SkipCustom(OVIA *Ovia, BitBuffer *BitB, uint32_t ChunkSize) { // 134,775
        if (Ovia != NULL && BitB != NULL) {
            BitBuffer_Seek(BitB, Bytes2Bits(ChunkSize + 1));
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void OVIA_FLAC_SeekTable_Read(OVIA *Ovia, BitBuffer *BitB, uint32_t ChunkSize) { // 3528
        if (Ovia != NULL && BitB != NULL) {
            uint64_t NumSeekPoints = ChunkSize / 18; // 196
            OVIA_FLAC_SetNumSeekPoints(Ovia, NumSeekPoints);
            
            for (uint16_t SeekPoint = 0; SeekPoint < NumSeekPoints; SeekPoint++) {
                uint64_t Sample    = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 64);
                uint64_t Offset    = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 64);
                uint16_t FrameSize = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 16);
                OVIA_FLAC_Seek_SetSeekPoint(Ovia, SeekPoint, Sample, Offset, FrameSize);
            }
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void OVIA_FLAC_Vorbis_Parse(OVIA *Ovia, BitBuffer *BitB) { // LITTLE ENDIAN, size = 438
        if (Ovia != NULL && BitB != NULL) {
            uint32_t VendorTagSize = BitBuffer_ReadBits(LSByteFirst, LSBitFirst, BitB, 32);
            UTF8    *VendorTag     = BitBuffer_ReadUTF8(BitB, VendorTagSize);
            OVIA_SetTag(Ovia, CreatingSoftware, VendorTag);
            
            uint32_t NumUserTags   = BitBuffer_ReadBits(LSByteFirst, LSBitFirst, BitB, 32);
            for (uint32_t Comment = 0; Comment < NumUserTags; Comment++) {
                uint32_t  TagSize                          = BitBuffer_ReadBits(LSByteFirst, LSBitFirst, BitB, 32);
                UTF8     *Tag                              = BitBuffer_ReadUTF8(BitB, TagSize);
                OVIA_SetTag(Ovia, UnknownTag, Tag);
            }
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void OVIA_FLAC_CUE_Parse(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            char *CatalogID = calloc(FLACMediaCatalogNumberSize, sizeof(char));
            for (uint8_t CatalogChar = 0; CatalogChar < FLACMediaCatalogNumberSize; CatalogChar++) {
                CatalogID[CatalogChar] = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 8);
            }
            OVIA_FLAC_CUE_SetCatalogID(Ovia, CatalogID);
            OVIA_FLAC_CUE_SetLeadIn(Ovia, BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 64));
            OVIA_FLAC_CUE_SetIsCD(Ovia, BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 1));
            BitBuffer_Seek(BitB, 2071); // Reserved
            
            uint8_t NumTracks = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 8);
            OVIA_FLAC_CUE_SetNumTracks(Ovia, NumTracks);
            
            for (uint8_t Track = 0; Track < NumTracks; Track++) {
                uint64_t  Offset      = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 64);
                uint8_t   ISRCSize    = BitBuffer_GetUTF8StringSize(BitB);
                UTF8     *ISRCString  = BitBuffer_ReadUTF8(BitB, ISRCSize);
                bool      IsAudio     = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 1);
                bool      PreEmphasis = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 1);
                
                OVIA_FLAC_CUE_SetTrack(Ovia, Offset, IsAudio, PreEmphasis, ISRCString);
                
                BitBuffer_Seek(BitB, 152); // Reserved, all 0
                BitBuffer_Seek(BitB, 8); // NumIndexPoints
            }
            /*
            Ovia->Meta->Cue->IndexOffset    = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 64);
            Ovia->Meta->Cue->IndexPointNum  = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 8);
             */
            BitBuffer_Seek(BitB, 24); // Reserved
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void FLACParsePicture(OVIA *Ovia, BitBuffer *BitB) { // 81012
        if (Ovia != NULL && BitB != NULL) {
            uint32_t PicType          = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 32);
            uint32_t MIMESize         = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 32);
            UTF8    *MIMEType         = BitBuffer_ReadUTF8(BitB, MIMESize);
            
            uint32_t PicDescriptionSize = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 32);
            UTF8    *PicDescription     = BitBuffer_ReadUTF8(BitB, PicDescriptionSize);
            
            uint32_t Width              = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 32);
            uint32_t Height             = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 32);
            uint32_t BitDepth           = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 32);
            uint32_t ColorsUsed         = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 32);
            uint32_t PicSize            = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 32);
            // Pop in the address of the start of the data, and skip over the data instead of buffering it.
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
#ifdef __cplusplus
}
#endif
