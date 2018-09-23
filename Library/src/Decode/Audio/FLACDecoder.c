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
    
    void FLACReadStream(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            uint16_t Marker = BitBuffer_PeekBits(MSByteFirst, LSBitFirst, BitB, 14);
            if (Marker == FLACFrameMagic) {
                BitBuffer_Seek(BitB, 14);
                FLACReadFrame(BitB, Ovia);
            } else {
                FLACParseMetadata(BitB, Ovia);
            }
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void FLACReadFrame(OVIA *Ovia, BitBuffer *BitB) {
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
            
            OVIA_FLAC_Frame_SetCRC(Ovia, BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 8)); // 0xC2?????
            
            for (uint8_t Channel = 0; Channel < OVIA_GetNumChannels(Ovia); Channel++) { // 2 channels
                FLACReadSubFrame(BitB, Ovia, Channel);
            }
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void FLACReadSubFrame(OVIA *Ovia, BitBuffer *BitB, uint8_t Channel) { // 2 channels
        if (Ovia != NULL && BitB != NULL) {
            BitBuffer_Seek(BitB, 1); // Reserved
            Ovia->Data->SubFrame->SubFrameType      = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 6); // LPC, 1?
            if (Ovia->Data->SubFrame->SubFrameType > 0) {
                Ovia->Data->LPC->LPCOrder           = (Ovia->Data->SubFrame->SubFrameType & 0x1F) - 1; // 0
            }
            Ovia->Data->SubFrame->WastedBitsFlag    = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 1); // 0
            if (Ovia->Data->SubFrame->WastedBitsFlag == true) {
                uint8_t WastedBits = BitBuffer_ReadUnary(MSByteFirst, LSBitFirst, BitB, TruncatedCountUnary, 0); // 1?
                Ovia->Data->SubFrame->WastedBits    = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, WastedBits); // 0?
                
            }
            
            if (Ovia->Data->SubFrame->SubFrameType == Subframe_Verbatim) { // PCM
                FLACOVIASubFrameVerbatim(BitB, Ovia);
            } else if (Ovia->Data->SubFrame->SubFrameType == Subframe_Constant) {
                FLACOVIASubFrameConstant(BitB, Ovia);
            } else if (Ovia->Data->SubFrame->SubFrameType >= Subframe_Fixed && Ovia->Data->SubFrame->SubFrameType <= Subframe_LPC) { // Fixed
                FLACOVIASubFrameFixed(BitB, Ovia);
            } else if (Ovia->Data->SubFrame->SubFrameType >= Subframe_LPC) { // LPC
                FLACOVIASubFrameLPC(BitB, Ovia, Channel);
            } else {
                Log(Log_ERROR, __func__, U8("Invalid Subframe type: %d"), Ovia->Data->SubFrame->SubFrameType);
            }
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void FLACOVIASubFrameVerbatim(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            // First Raw  Sample L channel = 0x5E83
            // First Raw  Sample R channel = 0x7139
            // First FLAC Sample L channel = 0x5ED7, aka 87 more?
            for (uint16_t Sample = 0; Sample < Ovia->Data->Frame->BlockSize; Sample++) {
                Ovia->OVIAdSamples[Sample] = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, Ovia->Data->Frame->BitDepth);
            }
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void FLACOVIASubFrameConstant(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            int64_t Constant = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, Ovia->Data->Frame->BitDepth);
            memset(Ovia->OVIAdSamples, Constant, Ovia->Data->Frame->BlockSize);
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void FLACOVIASubFrameFixed(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            for (uint16_t WarmupSample = 0; WarmupSample < Ovia->Data->Frame->BitDepth * Ovia->Data->LPC->LPCOrder; WarmupSample++) {
                Ovia->OVIAdSamples[WarmupSample]  = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, Ovia->Data->Frame->BitDepth);
            }
            DecodeFLACResidual(BitB, Ovia);
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void FLACOVIASubFrameLPC(OVIA *Ovia, BitBuffer *BitB, uint8_t Channel) { // 4 0's
        if (Ovia != NULL && BitB != NULL) {
            for (uint16_t WarmupSample = 0; WarmupSample < Ovia->Data->Frame->BitDepth * Ovia->Data->LPC->LPCOrder; WarmupSample++) {
                Ovia->OVIAdSamples[WarmupSample]  = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, Ovia->Data->Frame->BitDepth); // 18 0's
            }
            Ovia->Data->LPC->LPCPrecision           = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 4) + 1; // 1
            Ovia->Data->LPC->LPCShift               = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 5); // 0
            Ovia->Data->LPC->NumLPCCoeffs           = Ovia->Data->LPC->LPCPrecision * Ovia->Data->LPC->LPCOrder;
            
            for (uint16_t LPCCoefficent = 0; LPCCoefficent < Ovia->Data->LPC->NumLPCCoeffs; LPCCoefficent++) {
                Ovia->Data->LPC->LPCCoeff[LPCCoefficent] = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, Ovia->Data->LPC->LPCPrecision) + 1;
            }
            DecodeFLACResidual(BitB, Ovia);
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void DecodeFLACResidual(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            Ovia->Data->LPC->RicePartitionType      = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 2);
            if (Ovia->Data->LPC->RicePartitionType == RICE1) {
                DecodeRice1Partition(BitB, Ovia);
            } else if (Ovia->Data->LPC->RicePartitionType == RICE2) {
                DecodeFLACRice2Partition(BitB, Ovia);
            }
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void DecodeRice1Partition(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            Ovia->Data->LPC->PartitionOrder = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 4); // byte 5, 3 bits read.
            for (uint8_t Partition = 0; Partition < Ovia->Data->LPC->PartitionOrder; Partition++) {
                Ovia->Data->Rice->RICEParameter[Partition] = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 4) + 5;
                if (Ovia->Data->Rice->RICEParameter[Partition] == 20) {
                    // Escape code, meaning the partition is in unencoded binary form using n bits per sample; n follows as a 5-bit number.
                } else {
                    if (Ovia->Data->LPC->PartitionOrder == 0) {
                        Ovia->Data->Rice->RICEParameter[Partition] = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, Ovia->Data->Frame->BlockSize - Ovia->Data->LPC->LPCOrder);
                    } else if (Ovia->Data->LPC->PartitionOrder > 0) {
                        Ovia->Data->Rice->RICEParameter[Partition] = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, (Ovia->Data->Frame->BlockSize / pow(2, Ovia->Data->LPC->PartitionOrder)));
                    } else {
                        Ovia->Data->Rice->RICEParameter[Partition] = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, (Ovia->Data->Frame->BlockSize / pow(2, Ovia->Data->LPC->PartitionOrder)) - Ovia->Data->LPC->LPCOrder);
                    }
                }
            }
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void DecodeFLACRice2Partition(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            for (uint8_t Partition = 0; Partition < Ovia->Data->LPC->PartitionOrder; Partition++) {
                Ovia->Data->Rice->RICEParameter[Partition] = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 5) + 5;
                if (Ovia->Data->Rice->RICEParameter[Partition] == 36) {
                    // Escape code, meaning the partition is in unencoded binary form using n bits per sample; n follows as a 5-bit number.
                } else {
                    if (Ovia->Data->LPC->PartitionOrder == 0) {
                        Ovia->Data->Rice->RICEParameter[Partition] = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, Ovia->Data->Frame->BlockSize - Ovia->Data->LPC->LPCOrder);
                    } else if (Ovia->Data->LPC->PartitionOrder > 0) {
                        Ovia->Data->Rice->RICEParameter[Partition] = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, (Ovia->Data->Frame->BlockSize / pow(2, Ovia->Data->LPC->PartitionOrder)));
                    } else {
                        Ovia->Data->Rice->RICEParameter[Partition] = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, (Ovia->Data->Frame->BlockSize / pow(2, Ovia->Data->LPC->PartitionOrder)) - Ovia->Data->LPC->LPCOrder);
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
    
    void FLACBitDepth(OVIA *Ovia) {
        if (Ovia != NULL && BitB != NULL) {
            switch (Ovia->Meta->StreamInfo->CodedBitDepth) {
                case 0:
                    Ovia->Data->Frame->BitDepth = Ovia->Meta->StreamInfo->BitDepth;
                    break;
                case 1:
                    Ovia->Data->Frame->BitDepth = 8;
                    break;
                case 2:
                    Ovia->Data->Frame->BitDepth = 12;
                    break;
                case 4:
                    Ovia->Data->Frame->BitDepth = 16;
                    break;
                case 5:
                    Ovia->Data->Frame->BitDepth = 20;
                    break;
                case 6:
                    Ovia->Data->Frame->BitDepth = 24;
                    break;
                default:
                    Ovia->Data->Frame->BitDepth = 0;
                    break;
            }
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void FLACSampleRate(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            switch (Ovia->Meta->StreamInfo->CodedSampleRate) {
                case 0:
                    Ovia->Data->Frame->SampleRate = Ovia->Meta->StreamInfo->SampleRate;
                    break;
                case 1:
                    Ovia->Data->Frame->SampleRate = 88200;
                    break;
                case 2:
                    Ovia->Data->Frame->SampleRate = 176400;
                    break;
                case 3:
                    Ovia->Data->Frame->SampleRate = 192000;
                    break;
                case 4:
                    Ovia->Data->Frame->SampleRate = 8000;
                    break;
                case 5:
                    Ovia->Data->Frame->SampleRate = 16000;
                    break;
                case 6:
                    Ovia->Data->Frame->SampleRate = 22050;
                    break;
                case 7:
                    Ovia->Data->Frame->SampleRate = 24000;
                    break;
                case 8:
                    Ovia->Data->Frame->SampleRate = 32000;
                    break;
                case 9:
                    Ovia->Data->Frame->SampleRate = 44100;
                    break;
                case 10:
                    Ovia->Data->Frame->SampleRate = 48000;
                    break;
                case 11:
                    Ovia->Data->Frame->SampleRate = 96000;
                    break;
                default:
                    break;
            }
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
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
            Ovia->LastMetadataBlock          = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 1);  // true
            uint8_t  MetadataBlockType       = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 7);  // 1
            Ovia->Meta->MetadataSize         = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 24); // 4123 Does NOT count the 2 fields above.
            
            switch (MetadataBlockType) {
                case StreamInfo:
                    FLACParseStreamInfo(BitB, Ovia);
                    break;
                case Padding:
                    FLACSkipPadding(BitB, Ovia);
                    break;
                case Custom:
                    FLACSkipCustom(BitB, Ovia);
                    break;
                case SeekTable:
                    FLACParseSeekTable(BitB, Ovia);
                    break;
                case VorbisComment:
                    FLACParseVorbisComment(BitB, Ovia);
                    break;
                case Cuesheet:
                    FLACParseCuesheet(BitB, Ovia);
                    break;
                case Picture:
                    FLACParsePicture(BitB, Ovia);
                    break;
                default:
                    Log(Log_DEBUG, __func__, U8("Invalid Metadata Type: %d\n", MetadataBlockType);
                        break;
            }
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void FLACParseStreamInfo(OVIA *Ovia, BitBuffer *BitB) {
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
                BitBuffer_Skip(BitB, 128);
            }
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void FLACSkipPadding(OVIA *Ovia, BitBuffer *BitB) { // 8192
        if (Ovia != NULL && BitB != NULL) {
            BitBuffer_Seek(BitB, Bytes2Bits(Ovia->Meta->MetadataSize));
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void FLACSkipCustom(OVIA *Ovia, BitBuffer *BitB) { // 134,775
        if (Ovia != NULL && BitB != NULL) {
            BitBuffer_Seek(BitB, Bytes2Bits(Ovia->Meta->MetadataSize + 1));
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void FLACParseSeekTable(OVIA *Ovia, BitBuffer *BitB, uint64_t ChunkSize) { // 3528
        if (Ovia != NULL && BitB != NULL) {
            uint64_t NumSeekPoints = ChunkSize / 18; // 196
            OVIA_FLAC_SetNumSeekPoints(Ovia, NumSeekPoints);
            
            for (uint16_t SeekPoint = 0; SeekPoint < NumSeekPoints; SeekPoint++) {
                Ovia->Meta->Seek->SampleInTargetFrame[SeekPoint]        = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 64); // 0
                Ovia->Meta->Seek->OffsetFrom1stSample[SeekPoint]        = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 64); // 0
                Ovia->Meta->Seek->TargetFrameSize[SeekPoint]            = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 16); // 4096
                
                // 0
                // 0
                // 4096
                
                // 40960
                // 50161
                // 4096
                
                //
            }
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void FLACParseVorbisComment(OVIA *Ovia, BitBuffer *BitB) { // LITTLE ENDIAN, size = 438
        if (Ovia != NULL && BitB != NULL) {
            Ovia->Meta->Vorbis->VendorTagSize              = BitBuffer_ReadBits(BitIOLSByteFirst, LSBitFirst, BitB, 32); // 13
            Ovia->Meta->Vorbis->VendorTag                  = calloc(1, Ovia->Meta->Vorbis->VendorTagSize * sizeof(char));
            for (uint32_t TagByte = 0; TagByte < Ovia->Meta->Vorbis->VendorTagSize; TagByte++) {
                Ovia->Meta->Vorbis->VendorTag[TagByte]     = BitBuffer_ReadBits(BitIOLSByteFirst, LSBitFirst, BitB, 8); // Lavf58.17.101
            }
            Ovia->Meta->Vorbis->NumUserTags                = BitBuffer_ReadBits(BitIOLSByteFirst, LSBitFirst, BitB, 32); // 14
            Ovia->Meta->Vorbis->UserTagSize                = calloc(Ovia->Meta->Vorbis->NumUserTags, sizeof(uint8_t)); // 28
            Ovia->Meta->Vorbis->UserTagString              = calloc(Ovia->Meta->Vorbis->NumUserTags, sizeof(char));
            // 28,  ALBUM=What A Wonderful World
            // 27?, ALBUMARTIST=Louis Armstrong
            // 22,  ARTIST=Louis Armstrong
            // 12,  DISCNUMBER=1
            // 10,  genre=Jazz
            // 15,  itunesgapless=0
            // 98,  iTunNORM=000001F8 00000142 00001CE3 000014F5 000092A4 000092B4 000041CC 000051C5 00012C9F 000145EF
            // 7,   BPM=283
            // 13,  BPM=00000 BPM
            // 28,  TITLE=What A Wonderful World
            // 12,  totaldiscs=1
            // 14,  totaltracks=11
            // 9,   DATE=1988
            // 13,  TRACKNUMBER=1
            for (uint32_t Comment = 0; Comment < Ovia->Meta->Vorbis->NumUserTags; Comment++) {
                Ovia->Meta->Vorbis->UserTagSize[Comment]   = BitBuffer_ReadBits(BitIOLSByteFirst, LSBitFirst, BitB, 32);
                Ovia->Meta->Vorbis->UserTagString[Comment] = calloc(1, Ovia->Meta->Vorbis->UserTagSize[Comment] * sizeof(char));
                
                char UserTagString[Ovia->Meta->Vorbis->UserTagSize[Comment]];
                for (uint32_t CommentByte = 0; CommentByte < Ovia->Meta->Vorbis->UserTagSize[Comment]; CommentByte++) {
                    UserTagString[CommentByte] = BitBuffer_ReadBits(BitIOLSByteFirst, LSBitFirst, BitB, 8);
                }
                Ovia->Meta->Vorbis->UserTagString[Comment] = UserTagString;
            }
            
            for (uint32_t UserTag = 0; UserTag < Ovia->Meta->Vorbis->NumUserTags; UserTag++) {
                Log(Log_DEBUG, __func__, U8("User Tag %d of %d: %c\n", UserTag, Ovia->Meta->Vorbis->NumUserTags, Ovia->Meta->Vorbis->UserTagString[UserTag]);
                    }
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void FLACParseCuesheet(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            char *CatalogID = calloc(FLACMediaCatalogNumberSize, sizeof(char));
            for (uint8_t CatalogChar = 0; CatalogChar < FLACMediaCatalogNumberSize; CatalogChar++) {
                CatalogID[CatalogChar] = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 8);
            }
            OVIA_FLAC_CUE_SetCatalogID(Ovia, CatalogID);
            Ovia->Meta->Cue->LeadIn = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 64);
            Ovia->Meta->Cue->IsCD   = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 1);
            BitBuffer_Seek(BitB, 2071); // Reserved
            Ovia->Meta->Cue->NumTracks = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 8);
            
            for (uint8_t Track = 0; Track < Ovia->Meta->Cue->NumTracks; Track++) {
                Ovia->Meta->Cue->TrackOffset[Track]  = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 64);
                Ovia->Meta->Cue->TrackNum[Track]     = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 64);
                
                for (uint8_t ISRCByte = 0; ISRCByte < FLACISRCSize; ISRCByte++) {
                    Ovia->Meta->Cue->ISRC[Track][ISRCByte]  = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 8);
                }
                Ovia->Meta->Cue->IsAudio[Track] = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 1);
                Ovia->Meta->Cue->PreEmphasis[Track] = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 1);
                BitBuffer_Seek(BitB, 152); // Reserved, all 0
                Ovia->Meta->Cue->NumTrackIndexPoints[Track] = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 8);
            }
            
            Ovia->Meta->Cue->IndexOffset    = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 64);
            Ovia->Meta->Cue->IndexPointNum  = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 8);
            BitBuffer_Seek(BitB, 24); // Reserved
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void FLACParsePicture(OVIA *Ovia, BitBuffer *BitB) { // 81012
        if (Ovia != NULL && BitB != NULL) {
            Ovia->Meta->Pic->PicType  = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 32); // 3
            Ovia->Meta->Pic->MIMESize = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 32); // 10
            for (uint32_t MIMEByte = 0; MIMEByte < Ovia->Meta->Pic->MIMESize; MIMEByte++) {
                Ovia->Meta->Pic->MIMEString[MIMEByte] = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 8); // image/jpeg
            }
            Ovia->Meta->Pic->PicDescriptionSize = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 32); // 0
            for (uint32_t Char = 0; Char < Ovia->Meta->Pic->PicDescriptionSize; Char++) {
                Ovia->Meta->Pic->PicDescriptionString[Char] = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 8);
            }
            Ovia->Meta->Pic->Width       = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 32); // 800
            Ovia->Meta->Pic->Height      = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 32); // 800
            Ovia->Meta->Pic->BitDepth    = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 32); // 24, PER PIXEL, NOT SUBPIXEL
            Ovia->Meta->Pic->ColorsUsed  = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 32); // 0
            Ovia->Meta->Pic->PictureSize = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 32); // 80970
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
