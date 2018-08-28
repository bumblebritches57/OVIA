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
     uint16_t Marker = ReadBits(MSByteFirst, LSBitFirst, BitB, 14, true);
     if (Marker == FLACFrameMagic) {
     // Get frame size by reading ahead until you find either the end of the stream, or another FLACFrameMagic
     // then skip back, and read it all.
     // OR could we somehow just read it until we got to the end of the frame, and
     size_t FrameSizeInBits = 0;
     while (ReadBits(MSByteFirst, LSBitFirst, BitB, 14, true) != FLACFrameMagic || (BitB->FilePosition + BitB->BitsUnavailable) < BitB->FileSize) {
     FrameSizeInBits += 14;
     }
     SkipBits(BitB, FrameSizeInBits);
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
        if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else {
            uint16_t Marker = PeekBits(MSByteFirst, LSBitFirst, BitB, 14);
            if (Marker == FLACFrameMagic) {
                SkipBits(BitB, 14);
                FLACReadFrame(BitB, Ovia);
            } else {
                FLACParseMetadata(BitB, Ovia);
            }
        }
    }
    
    void FLACReadFrame(OVIA *Ovia, BitBuffer *BitB) {
        if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else {
            SkipBits(BitB, 1); // 0
            Ovia->Data->Frame->BlockType            = ReadBits(MSByteFirst, LSBitFirst, BitB, 1); // 0 aka Fixed
            // Next byte, 0xEC
            uint8_t CodedSampleBlockSize           = ReadBits(MSByteFirst, LSBitFirst, BitB, 4); // 14
            if (CodedSampleBlockSize != 6 && CodedSampleBlockSize != 7) {
                Ovia->Data->Frame->BlockSize        = GetBlockSizeInSamples(CodedSampleBlockSize); // 16384
            }
            Ovia->Data->Frame->CodedSampleRate      = ReadBits(MSByteFirst, LSBitFirst, BitB, 4); // 12 aka 44100
            if ((Ovia->Data->Frame->CodedSampleRate >= 0) && (Ovia->Data->Frame->CodedSampleRate <= 11)) {
                FLACSampleRate(BitB, Ovia);
            }
            Ovia->Data->Frame->ChannelLayout        = ReadBits(MSByteFirst, LSBitFirst, BitB, 4) + 1; // Average_Diff
            Ovia->Data->Frame->CodedBitDepth        = ReadBits(MSByteFirst, LSBitFirst, BitB, 3); // 6 aka 24 bits per sample
            if (Ovia->Data->Frame->CodedBitDepth != 0) {
                FLACBitDepth(Ovia);
            }
            SkipBits(BitB, 1); // 0
            
            if (Ovia->Data->Frame->BlockType        == FixedBlockSize) { // Fixed
                // The FrameNumber is 0, which means it's just NULL.
                Ovia->Data->Frame->FrameNumber      = ReadBits(MSByteFirst, LSBitFirst, BitB, 31); // Encoded with/like? UTF-8
            } else if (Ovia->Data->Frame->BlockType == VariableBlockSize) {
                Ovia->Data->Frame->SampleNumber     = ReadBits(MSByteFirst, LSBitFirst, BitB, 36);
            }
            
            if (CodedSampleBlockSize == 6) {
                Ovia->Data->Frame->BlockSize        = ReadBits(MSByteFirst, LSBitFirst, BitB, 8); // SamplesInBlock
            } else if (CodedSampleBlockSize == 7) {
                Ovia->Data->Frame->BlockSize        = ReadBits(MSByteFirst, LSBitFirst, BitB, 16); // SamplesInBlock
            }
            
            
            if (Ovia->Data->Frame->CodedSampleRate == 12) {
                Ovia->Data->Frame->SampleRate       = ReadBits(MSByteFirst, LSBitFirst, BitB, 8) * 1000; // 0xC0 = 192000
            } else if (Ovia->Data->Frame->CodedSampleRate == 13) {
                Ovia->Data->Frame->SampleRate       = ReadBits(MSByteFirst, LSBitFirst, BitB, 16);
            } else if (Ovia->Data->Frame->CodedSampleRate == 14) {
                Ovia->Data->Frame->SampleRate       = ReadBits(MSByteFirst, LSBitFirst, BitB, 16) * 10;
            }
            
            Ovia->Data->Frame->FLACFrameCRC         = ReadBits(MSByteFirst, LSBitFirst, BitB, 8); // CRC, 0xCF
            
            for (uint8_t Channel = 0; Channel < Ovia->Meta->StreamInfo->Channels; Channel++) { // read SubFrame
                FLACReadSubFrame(BitB, Ovia, Channel);
            }
        }
    }
    
    void FLACReadSubFrame(OVIA *Ovia, BitBuffer *BitB, uint8_t Channel) { // 2 channels
        if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else {
            SkipBits(BitB, 1); // Reserved
            Ovia->Data->SubFrame->SubFrameType      = ReadBits(MSByteFirst, LSBitFirst, BitB, 6); // LPC
            if (Ovia->Data->SubFrame->SubFrameType > 0) {
                Ovia->Data->LPC->LPCOrder           = (Ovia->Data->SubFrame->SubFrameType & 0x1F) - 1; // 6
            }
            Ovia->Data->SubFrame->WastedBitsFlag    = ReadBits(MSByteFirst, LSBitFirst, BitB, 1); // 0
            if (Ovia->Data->SubFrame->WastedBitsFlag == true) {
                Ovia->Data->SubFrame->WastedBits    = ReadRICE(BitB, false, 0); // 11111 0 00000
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
        }
    }
    
    void FLACOVIASubFrameVerbatim(OVIA *Ovia, BitBuffer *BitB) {
        if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else {
            for (uint16_t Sample = 0; Sample < Ovia->Data->Frame->BlockSize; Sample++) {
                Ovia->OVIAdSamples[Sample] = ReadBits(MSByteFirst, LSBitFirst, BitB, Ovia->Data->Frame->BitDepth);
            }
        }
    }
    
    void FLACOVIASubFrameConstant(OVIA *Ovia, BitBuffer *BitB) {
        if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else {
            int64_t Constant = ReadBits(MSByteFirst, LSBitFirst, BitB, Ovia->Data->Frame->BitDepth);
            memset(Ovia->OVIAdSamples, Constant, Ovia->Data->Frame->BlockSize);
        }
    }
    
    void FLACOVIASubFrameFixed(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
        
        if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else {
            for (uint16_t WarmupSample = 0; WarmupSample < Ovia->Data->Frame->BitDepth * Ovia->Data->LPC->LPCOrder; WarmupSample++) {
                Ovia->OVIAdSamples[WarmupSample]  = ReadBits(MSByteFirst, LSBitFirst, BitB, Ovia->Data->Frame->BitDepth);
            }
            DecodeFLACResidual(BitB, Ovia);
        }
    }
    
    void FLACOVIASubFrameLPC(OVIA *Ovia, BitBuffer *BitB, uint8_t Channel) { // 4 0's
        if (Ovia != NULL && BitB != NULL) {
            
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
        
        if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else {
            for (uint16_t WarmupSample = 0; WarmupSample < Ovia->Data->Frame->BitDepth * Ovia->Data->LPC->LPCOrder; WarmupSample++) {
                Ovia->OVIAdSamples[WarmupSample]  = ReadBits(MSByteFirst, LSBitFirst, BitB, Ovia->Data->Frame->BitDepth); // 18 0's
            }
            Ovia->Data->LPC->LPCPrecision           = ReadBits(MSByteFirst, LSBitFirst, BitB, 4) + 1; // 1
            Ovia->Data->LPC->LPCShift               = ReadBits(MSByteFirst, LSBitFirst, BitB, 5); // 0
            Ovia->Data->LPC->NumLPCCoeffs           = Ovia->Data->LPC->LPCPrecision * Ovia->Data->LPC->LPCOrder;
            
            for (uint16_t LPCCoefficent = 0; LPCCoefficent < Ovia->Data->LPC->NumLPCCoeffs; LPCCoefficent++) {
                Ovia->Data->LPC->LPCCoeff[LPCCoefficent] = ReadBits(MSByteFirst, LSBitFirst, BitB, Ovia->Data->LPC->LPCPrecision) + 1;
            }
            DecodeFLACResidual(BitB, Ovia);
        }
    }
    
    void DecodeFLACResidual(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
        
        if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else {
            Ovia->Data->LPC->RicePartitionType      = ReadBits(MSByteFirst, LSBitFirst, BitB, 2);
            if (Ovia->Data->LPC->RicePartitionType == RICE1) {
                DecodeRice1Partition(BitB, Ovia);
            } else if (Ovia->Data->LPC->RicePartitionType == RICE2) {
                DecodeFLACRice2Partition(BitB, Ovia);
            }
        }
    }
    
    void DecodeRice1Partition(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
        
        if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else {
            Ovia->Data->LPC->PartitionOrder = ReadBits(MSByteFirst, LSBitFirst, BitB, 4); // byte 5, 3 bits read.
            for (uint8_t Partition = 0; Partition < Ovia->Data->LPC->PartitionOrder; Partition++) {
                Ovia->Data->Rice->RICEParameter[Partition] = ReadBits(MSByteFirst, LSBitFirst, BitB, 4) + 5;
                if (Ovia->Data->Rice->RICEParameter[Partition] == 20) {
                    // Escape code, meaning the partition is in unencoded binary form using n bits per sample; n follows as a 5-bit number.
                } else {
                    if (Ovia->Data->LPC->PartitionOrder == 0) {
                        Ovia->Data->Rice->RICEParameter[Partition] = ReadBits(MSByteFirst, LSBitFirst, BitB, Ovia->Data->Frame->BlockSize - Ovia->Data->LPC->LPCOrder);
                    } else if (Ovia->Data->LPC->PartitionOrder > 0) {
                        Ovia->Data->Rice->RICEParameter[Partition] = ReadBits(MSByteFirst, LSBitFirst, BitB, (Ovia->Data->Frame->BlockSize / pow(2, Ovia->Data->LPC->PartitionOrder)));
                    } else {
                        Ovia->Data->Rice->RICEParameter[Partition] = ReadBits(MSByteFirst, LSBitFirst, BitB, (Ovia->Data->Frame->BlockSize / pow(2, Ovia->Data->LPC->PartitionOrder)) - Ovia->Data->LPC->LPCOrder);
                    }
                }
            }
        }
    }
    
    void DecodeFLACRice2Partition(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
        
        if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else {
            for (uint8_t Partition = 0; Partition < Ovia->Data->LPC->PartitionOrder; Partition++) {
                Ovia->Data->Rice->RICEParameter[Partition] = ReadBits(MSByteFirst, LSBitFirst, BitB, 5) + 5;
                if (Ovia->Data->Rice->RICEParameter[Partition] == 36) {
                    // Escape code, meaning the partition is in unencoded binary form using n bits per sample; n follows as a 5-bit number.
                } else {
                    if (Ovia->Data->LPC->PartitionOrder == 0) {
                        Ovia->Data->Rice->RICEParameter[Partition] = ReadBits(MSByteFirst, LSBitFirst, BitB, Ovia->Data->Frame->BlockSize - Ovia->Data->LPC->LPCOrder);
                    } else if (Ovia->Data->LPC->PartitionOrder > 0) {
                        Ovia->Data->Rice->RICEParameter[Partition] = ReadBits(MSByteFirst, LSBitFirst, BitB, (Ovia->Data->Frame->BlockSize / pow(2, Ovia->Data->LPC->PartitionOrder)));
                    } else {
                        Ovia->Data->Rice->RICEParameter[Partition] = ReadBits(MSByteFirst, LSBitFirst, BitB, (Ovia->Data->Frame->BlockSize / pow(2, Ovia->Data->LPC->PartitionOrder)) - Ovia->Data->LPC->LPCOrder);
                    }
                }
            }
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
            
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
        
        if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else {
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
        }
    }
    
    void FLACSampleRate(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
        
        if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else {
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
            
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
        
        if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else {
            Ovia->LastMetadataBlock          = ReadBits(MSByteFirst, LSBitFirst, BitB, 1);  // true
            uint8_t  MetadataBlockType      = ReadBits(MSByteFirst, LSBitFirst, BitB, 7);  // 4
            Ovia->Meta->MetadataSize         = ReadBits(MSByteFirst, LSBitFirst, BitB, 24); // 385 Does NOT count the 2 fields above.
            
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
        }
    }
    
    void FLACParseStreamInfo(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            OVIA_FLAC_SetMinBlockSize(Ovia, ReadBits(MSByteFirst, LSBitFirst, BitB, 16));
            OVIA_FLAC_SetMaxBlockSize(Ovia, ReadBits(MSByteFirst, LSBitFirst, BitB, 16));
            OVIA_FLAC_SetMinFrameSize(Ovia, ReadBits(MSByteFirst, LSBitFirst, BitB, 24));
            OVIA_FLAC_SetMaxFrameSize(Ovia, ReadBits(MSByteFirst, LSBitFirst, BitB, 24));
            OVIA_SetSampleRate(Ovia,        ReadBits(MSByteFirst, LSBitFirst, BitB, 20));
            OVIA_SetNumChannels(Ovia,       ReadBits(MSByteFirst, LSBitFirst, BitB, 3) + 1);
            OVIA_SetBitDepth(Ovia,          ReadBits(MSByteFirst, LSBitFirst, BitB, 5) + 1);
            OVIA_SetNumSamples(Ovia,        ReadBits(MSByteFirst, LSBitFirst, BitB, 36));
            uint8_t *MD5 = calloc(16, sizeof(uint8_t));
            if (MD5 != NULL) {
                for (uint8_t MD5Byte = 0; MD5Byte < 16; MD5Byte++) {
                    MD5[MD5Byte] = ReadBits(MSByteFirst, LSBitFirst, BitB, 8);
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
            SkipBits(BitB, Bytes2Bits(Ovia->Meta->MetadataSize));
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
        
        if (BitB == NULL) {
            
        } else if (Ovia == NULL) {
            
        } else {
            
        }
    }
    
    void FLACSkipCustom(OVIA *Ovia, BitBuffer *BitB) { // 134,775
        if (Ovia != NULL && BitB != NULL) {
            
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
        
        if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else {
            SkipBits(BitB, Bytes2Bits(Ovia->Meta->MetadataSize + 1));
        }
    }
    
    void FLACParseSeekTable(OVIA *Ovia, BitBuffer *BitB) { // 18
        if (Ovia != NULL && BitB != NULL) {
            
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
        
        if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else {
            Ovia->Meta->Seek->NumSeekPoints = Ovia->Meta->MetadataSize / 18; // 21
            
            for (uint16_t SeekPoint = 0; SeekPoint < Ovia->Meta->Seek->NumSeekPoints; SeekPoint++) {
                Ovia->Meta->Seek->SampleInTargetFrame[SeekPoint]        = ReadBits(MSByteFirst, LSBitFirst, BitB, 64); // 0
                Ovia->Meta->Seek->OffsetFrom1stSample[SeekPoint]        = ReadBits(MSByteFirst, LSBitFirst, BitB, 64); // 0
                Ovia->Meta->Seek->TargetFrameSize[SeekPoint]            = ReadBits(MSByteFirst, LSBitFirst, BitB, 16); // 16384
            }
        }
    }
    
    void FLACParseVorbisComment(OVIA *Ovia, BitBuffer *BitB) { // LITTLE ENDIAN
        if (Ovia != NULL && BitB != NULL) {
            
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
        
        if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else {
            Ovia->Meta->Vorbis->VendorTagSize              = ReadBits(BitIOLSByteFirst, LSBitFirst, BitB, 32); // 13
            Ovia->Meta->Vorbis->VendorTag                  = calloc(1, Ovia->Meta->Vorbis->VendorTagSize * sizeof(char));
            for (uint32_t TagByte = 0; TagByte < Ovia->Meta->Vorbis->VendorTagSize; TagByte++) {
                Ovia->Meta->Vorbis->VendorTag[TagByte]     = ReadBits(BitIOLSByteFirst, LSBitFirst, BitB, 8); // Lavf58.17.101
            }
            Ovia->Meta->Vorbis->NumUserTags                = ReadBits(BitIOLSByteFirst, LSBitFirst, BitB, 32); // 14
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
                Ovia->Meta->Vorbis->UserTagSize[Comment]   = ReadBits(BitIOLSByteFirst, LSBitFirst, BitB, 32);
                Ovia->Meta->Vorbis->UserTagString[Comment] = calloc(1, Ovia->Meta->Vorbis->UserTagSize[Comment] * sizeof(char));
                
                char UserTagString[Ovia->Meta->Vorbis->UserTagSize[Comment]];
                for (uint32_t CommentByte = 0; CommentByte < Ovia->Meta->Vorbis->UserTagSize[Comment]; CommentByte++) {
                    UserTagString[CommentByte] = ReadBits(BitIOLSByteFirst, LSBitFirst, BitB, 8);
                }
                Ovia->Meta->Vorbis->UserTagString[Comment] = UserTagString;
            }
            
            for (uint32_t UserTag = 0; UserTag < Ovia->Meta->Vorbis->NumUserTags; UserTag++) {
                Log(Log_DEBUG, __func__, U8("User Tag %d of %d: %c\n", UserTag, Ovia->Meta->Vorbis->NumUserTags, Ovia->Meta->Vorbis->UserTagString[UserTag]);
            }
        }
    }
    
    void FLACParseCuesheet(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
        
        if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else {
            for (uint8_t CatalogChar = 0; CatalogChar < FLACMedizCatalogNumberSize; CatalogChar++) {
                Ovia->Meta->Cue->CatalogID[CatalogChar] = ReadBits(MSByteFirst, LSBitFirst, BitB, 8);
            }
            Ovia->Meta->Cue->LeadIn = ReadBits(MSByteFirst, LSBitFirst, BitB, 64);
            Ovia->Meta->Cue->IsCD   = ReadBits(MSByteFirst, LSBitFirst, BitB, 1);
            SkipBits(BitB, 2071); // Reserved
            Ovia->Meta->Cue->NumTracks = ReadBits(MSByteFirst, LSBitFirst, BitB, 8);
            
            for (uint8_t Track = 0; Track < Ovia->Meta->Cue->NumTracks; Track++) {
                Ovia->Meta->Cue->TrackOffset[Track]  = ReadBits(MSByteFirst, LSBitFirst, BitB, 64);
                Ovia->Meta->Cue->TrackNum[Track]     = ReadBits(MSByteFirst, LSBitFirst, BitB, 64);
                
                for (uint8_t ISRCByte = 0; ISRCByte < FLACISRCSize; ISRCByte++) {
                    Ovia->Meta->Cue->ISRC[Track][ISRCByte]  = ReadBits(MSByteFirst, LSBitFirst, BitB, 8);
                }
                Ovia->Meta->Cue->IsAudio[Track] = ReadBits(MSByteFirst, LSBitFirst, BitB, 1);
                Ovia->Meta->Cue->PreEmphasis[Track] = ReadBits(MSByteFirst, LSBitFirst, BitB, 1);
                SkipBits(BitB, 152); // Reserved, all 0
                Ovia->Meta->Cue->NumTrackIndexPoints[Track] = ReadBits(MSByteFirst, LSBitFirst, BitB, 8);
            }
            
            Ovia->Meta->Cue->IndexOffset    = ReadBits(MSByteFirst, LSBitFirst, BitB, 64);
            Ovia->Meta->Cue->IndexPointNum  = ReadBits(MSByteFirst, LSBitFirst, BitB, 8);
            SkipBits(BitB, 24); // Reserved
        }
    }
    
    void FLACParsePicture(OVIA *Ovia, BitBuffer *BitB) { // 865,236
        if (Ovia != NULL && BitB != NULL) {
            
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
        
        if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else {
            Ovia->Meta->Pic->PicType  = ReadBits(MSByteFirst, LSBitFirst, BitB, 32); // 3
            Ovia->Meta->Pic->MIMESize = ReadBits(MSByteFirst, LSBitFirst, BitB, 32); // 10
            for (uint32_t MIMEByte = 0; MIMEByte < Ovia->Meta->Pic->MIMESize; MIMEByte++) {
                Ovia->Meta->Pic->MIMEString[MIMEByte] = ReadBits(MSByteFirst, LSBitFirst, BitB, 8); // image/jpeg
            }
            Ovia->Meta->Pic->PicDescriptionSize = ReadBits(MSByteFirst, LSBitFirst, BitB, 32); // 0
            for (uint32_t Char = 0; Char < Ovia->Meta->Pic->PicDescriptionSize; Char++) {
                Ovia->Meta->Pic->PicDescriptionString[Char] = ReadBits(MSByteFirst, LSBitFirst, BitB, 8);
            }
            Ovia->Meta->Pic->Width       = ReadBits(MSByteFirst, LSBitFirst, BitB, 32); // 1000
            Ovia->Meta->Pic->Height      = ReadBits(MSByteFirst, LSBitFirst, BitB, 32); // 1000
            Ovia->Meta->Pic->BitDepth    = ReadBits(MSByteFirst, LSBitFirst, BitB, 32); // 24, PER PIXEL, NOT SUBPIXEL
            Ovia->Meta->Pic->ColorsUsed  = ReadBits(MSByteFirst, LSBitFirst, BitB, 32); // 0
            Ovia->Meta->Pic->PictureSize = ReadBits(MSByteFirst, LSBitFirst, BitB, 32); // 865,194
            // Pop in the address of the start of the data, and skip over the data instead of buffering it.
            
        }
    }
    
#ifdef __cplusplus
}
#endif
