#include "../../include/Private/StreamIO/RIFFStream.h"
#include "../../../Dependencies/FoundationIO/Library/include/TextIO/LogIO.h"
#include "../../../Dependencies/FoundationIO/Library/include/MathIO.h"

#ifdef __cplusplus
extern "C" {
#endif

    /*
     This encoder works for WAVE, BroadcastWAVE, RF64

     BWF Spcific Chunks:
     BEXT
     UBXT

     BW64 Specific Chunks:
     BW64
     DS64
     JUNK
     AXML
     CHNA

     BW64 is used instead of RIFF
     DS64 replaces WAVE

     Also, W64 needs to be included here too
     */

    void WAV_Read_DS64(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            RIFFOptions *WAV       = Options;
            uint32_t ChunkSize    = BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 32);
            uint32_t SizeLow      = BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 32);
            uint32_t SizeHigh     = BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 32);
            uint32_t DataSizeLow  = BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 32);
            uint32_t DataSizeHigh = BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 32);

        } else if (Options == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }

    void WAV_Read_BEXT(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            RIFFOptions *WAV                = Options;
            uint64_t DescriptionStringSize = BitBuffer_GetUTF8StringSize(BitB);
            WAV->BEXT->Description         = BitBuffer_ReadUTF8(BitB, DescriptionStringSize);
            uint64_t OriginatorStringSize  = BitBuffer_GetUTF8StringSize(BitB);
            WAV->BEXT->Originator          = BitBuffer_ReadUTF8(BitB, OriginatorStringSize);
            uint64_t OriginatorRefSize     = BitBuffer_GetUTF8StringSize(BitB);
            WAV->BEXT->OriginatorRef       = BitBuffer_ReadUTF8(BitB, OriginatorRefSize);
            uint64_t OriginatorDateSize    = BitBuffer_GetUTF8StringSize(BitB);
            WAV->BEXT->OriginatorDate      = BitBuffer_ReadUTF8(BitB, OriginatorDateSize);
        } else if (Options == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }

    static void WAV_Read_INFO_TRCK(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            RIFFOptions *WAV                = Options;
            uint64_t StringSize            = BitBuffer_GetUTF8StringSize(BitB);
            WAV->Info->Artist              = BitBuffer_ReadUTF8(BitB, StringSize);
        } else if (Options == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }

    static void WAV_Read_INFO_IPRT(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            RIFFOptions *WAV                = Options;
            uint64_t StringSize            = BitBuffer_GetUTF8StringSize(BitB);
            WAV->Info->Artist              = BitBuffer_ReadUTF8(BitB, StringSize);
        } else if (Options == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }

    static void WAV_Read_INFO_IART(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            RIFFOptions *WAV                = Options;
            uint64_t StringSize            = BitBuffer_GetUTF8StringSize(BitB);
            WAV->Info->Artist              = BitBuffer_ReadUTF8(BitB, StringSize);
        } else if (Options == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }

    static void WAV_Read_INFO_ICRD(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            RIFFOptions *WAV                = Options;
            uint64_t StringSize            = BitBuffer_GetUTF8StringSize(BitB);
            WAV->Info->ReleaseDate         = BitBuffer_ReadUTF8(BitB, StringSize);
        } else if (Options == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }

    static void WAV_Read_INFO_IGNR(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            RIFFOptions *WAV                = Options;
            uint64_t StringSize            = BitBuffer_GetUTF8StringSize(BitB);
            WAV->Info->Genre               = BitBuffer_ReadUTF8(BitB, StringSize);
        } else if (Options == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }

    static void WAV_Read_INFO_INAM(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            RIFFOptions *WAV                = Options;
            uint64_t StringSize            = BitBuffer_GetUTF8StringSize(BitB);
            WAV->Info->Title               = BitBuffer_ReadUTF8(BitB, StringSize);
        } else if (Options == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }

    static void WAV_Read_INFO_IPRD(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            RIFFOptions *WAV                = Options;
            uint64_t StringSize            = BitBuffer_GetUTF8StringSize(BitB);
            WAV->Info->Album               = BitBuffer_ReadUTF8(BitB, StringSize);
        } else if (Options == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }

    static void WAV_Read_INFO_ISFT(void *Options, BitBuffer *BitB) { // Encoder
        if (Options != NULL && BitB != NULL) {
            RIFFOptions *WAV                = Options;
            uint64_t StringSize            = BitBuffer_GetUTF8StringSize(BitB);
            WAV->Info->CreationSoftware    = BitBuffer_ReadUTF8(BitB, StringSize);
        } else if (Options == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }

    static void WAV_Read_LIST(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            uint32_t SubChunkID   = BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 32);
            uint32_t SubChunkSize = BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 32);

            switch (SubChunkID) {
                case WAVChunkID_IART: // Artist
                    WAV_Read_INFO_IART(Options, BitB);
                    break;
                case WAVChunkID_ICRD: // Release date
                    WAV_Read_INFO_ICRD(Options, BitB);
                    break;
                case WAVChunkID_IGNR: // Genre
                    WAV_Read_INFO_IGNR(Options, BitB);
                    break;
                case WAVChunkID_INAM: // Title
                    WAV_Read_INFO_INAM(Options, BitB);
                    break;
                case WAVChunkID_IPRD: // Album
                    WAV_Read_INFO_IPRD(Options, BitB);
                    break;
                case WAVChunkID_ISFT: // Encoder
                    WAV_Read_INFO_ISFT(Options, BitB);
                    break;
                default:
                    Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("Unknown LIST Chunk: 0x%X"), SubChunkID);
                    break;
            }
        } else if (Options == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }

    static void WAV_Read_FMT(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            RIFFOptions *WAV        = Options;
            uint32_t ChunkSize     = BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 32);
            WAV->CompressionFormat = BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 16);
            WAV->NumChannels       = BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 16);
            WAV->SampleRate        = BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 32);
            WAV->ByteRate          = BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 32);
            WAV->BlockAlign        = BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 16);
            WAV->BitDepth          = BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 16);
            if (ChunkSize == 18) {
                uint16_t CBSize             = BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 16);
                BitBuffer_Seek(BitB, Bytes2Bits(CBSize - 16));
            } else if (ChunkSize == 40) {
                uint16_t CBSize             = BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 16);
                WAV->ValidBitsPerSample     = BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 16);

                WAV->SpeakerMask            = BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 32);
                uint8_t  *BinaryGUIDFormat  = BitBuffer_ReadGUUID(BitB, GUUIDType_BinaryGUID);
                BitBuffer_Seek(BitB, Bytes2Bits(CBSize - 22));
            }
        } else if (Options == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }

    void WAVParseMetadata(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            RIFFOptions *WAV    = Options;
            uint32_t ChunkID   = BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsFarthest, BitOrder_LSBitIsNearest, 32);
            uint32_t ChunkSize = BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 32);

            switch (ChunkID) {
                case WAVChunkID_LIST:
                    break;
                case WAVChunkID_FMT:
                    WAV_Read_FMT(Options, BitB);
                    break;
                case WAVChunkID_WAVE:
                    BitBuffer_Seek(BitB, 32);
                    break;
                default:
                    Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("Invalid ChunkID: 0x%X"), ChunkID);
                    break;
            }
        } else if (Options == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }

    void WAVExtractSamples(void *Options, BitBuffer *BitB, Audio2DContainer *Audio) {
        if (Options != NULL && BitB != NULL && Audio != NULL) {
            RIFFOptions *WAV            = Options;
            uint64_t NumSamples        = Audio2DContainer_GetNumSamples(Audio);
            uint8_t  SampleSizeRounded = (uint8_t) Bytes2Bits(Bits2Bytes(WAV->BitDepth, RoundingType_Up));
            if (WAV->BitDepth <= 8) {
                uint8_t **Samples = (uint8_t**) Audio2DContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < WAV->NumChannels; Channel++) {
                        Samples[Channel][Sample] = BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, SampleSizeRounded);
                    }
                }
            } else if (WAV->BitDepth <= 16) {
                uint16_t **Samples = (uint16_t**) Audio2DContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < WAV->NumChannels; Channel++) {
                        Samples[Channel][Sample] = BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, SampleSizeRounded);
                    }
                }
            } else if (WAV->BitDepth <= 32) {
                uint32_t **Samples = (uint32_t**) Audio2DContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < WAV->NumChannels; Channel++) {
                        Samples[Channel][Sample] = BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, SampleSizeRounded);
                    }
                }
            }
        } else if (Options == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("BitBuffer Pointer is NULL"));
        } else if (Audio == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("Audio2DContainer Pointer is NULL"));
        }
    }

    /* Format Decoding */
    static void W64_Read_FMT(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            RIFFOptions *W64                  = Options;
            W64->CompressionFormat           = BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 16);
            W64->NumChannels                 = BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 16);
            W64->SampleRate                  = BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 32);
            W64->BlockAlign                  = BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 32);
            W64->BlockAlignment              = BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 16);
            W64->BitDepth                    = BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 16);
            // Read the SpeakerMask
        } else if (Options == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }

    static void W64_Read_BEXT(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            RIFFOptions *W64                  = Options;
        } else if (Options == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }

    static void W64_Read_LEVL(void *Options, BitBuffer *BitB) { // aka Peak Envelope Chunk
        if (Options != NULL && BitB != NULL) {
            RIFFOptions *W64                  = Options;
        } else if (Options == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }

    void W64ParseMetadata(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            RIFFOptions *W64          = Options;
            uint8_t *ChunkUUIDString = BitBuffer_ReadGUUID(BitB, GUUIDType_BinaryGUID);
            uint64_t W64Size         = BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 64);
            if (GUUID_Compare(GUUIDType_BinaryGUID, ChunkUUIDString, W64_RIFF_GUIDString) == true) {

            } else if (GUUID_Compare(GUUIDType_BinaryGUID, ChunkUUIDString, W64_WAVE_GUIDString) == true) {

            } else if (GUUID_Compare(GUUIDType_BinaryGUID, ChunkUUIDString, W64_FMT_GUIDString) == true) {
                W64_Read_FMT(W64, BitB);
            } else if (GUUID_Compare(GUUIDType_BinaryGUID, ChunkUUIDString, W64_DATA_GUIDString) == true) {

            } else if (GUUID_Compare(GUUIDType_BinaryGUID, ChunkUUIDString, W64_LEVL_GUIDString) == true) {

            } else if (GUUID_Compare(GUUIDType_BinaryGUID, ChunkUUIDString, W64_BEXT_GUIDString) == true) {
                W64_Read_BEXT(W64, BitB);
            } else if (GUUID_Compare(GUUIDType_BinaryGUID, ChunkUUIDString, W64_FACT_GUIDString) == true) {

            } else if (GUUID_Compare(GUUIDType_BinaryGUID, ChunkUUIDString, W64_JUNK_GUIDString) == true) {

            } else if (GUUID_Compare(GUUIDType_BinaryGUID, ChunkUUIDString, W64_MRKR_GUIDString) == true) {

            } else if (GUUID_Compare(GUUIDType_BinaryGUID, ChunkUUIDString, W64_SUMM_GUIDString) == true) {

            } else if (GUUID_Compare(GUUIDType_BinaryGUID, ChunkUUIDString, W64_LIST_GUIDString) == true) {

            }
        } else if (Options == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }

    void W64ExtractSamples(void *Options, BitBuffer *BitB, Audio2DContainer *Audio, uint64_t NumSamples) {
        if (Options != NULL && BitB != NULL && Audio != NULL) {
            RIFFOptions *W64        = Options;
            uint64_t BitDepth      = W64->BitDepth;
            uint64_t NumChannels   = W64->NumChannels;
            uint8_t  BitDepthRound = Bytes2Bits(Bits2Bytes(BitDepth, RoundingType_Up));
            if (BitDepth <= 8) {
                uint8_t **Samples = (uint8_t**) Audio2DContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < NumChannels; Channel++) {
                        Samples[Channel][Sample] = BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsFarthest, BitOrder_LSBitIsNearest, BitDepthRound);
                    }
                }
            } else if (BitDepth <= 16) {
                uint16_t **Samples = (uint16_t**) Audio2DContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < NumChannels; Channel++) {
                        Samples[Channel][Sample] = BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsFarthest, BitOrder_LSBitIsNearest, BitDepthRound);
                    }
                }
            } else if (BitDepth <= 32) {
                uint32_t **Samples = (uint32_t**) Audio2DContainer_GetArray(Audio);
                for (uint64_t Sample = 0; Sample < NumSamples; Sample++) {
                    for (uint64_t Channel = 0; Channel < NumChannels; Channel++) {
                        Samples[Channel][Sample] = BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsFarthest, BitOrder_LSBitIsNearest, BitDepthRound);
                    }
                }
            }
        } else if (Options == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("BitBuffer Pointer is NULL"));
        } else if (Audio == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("Audio2DContainer is NULL"));
        }
    }

#ifdef __cplusplus
}
#endif

