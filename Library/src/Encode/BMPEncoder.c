#include "../../include/Private/BMPCommon.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    void BMPWriteHeader(void *Options, BitBuffer *BitB) {
        if (Options != NULL && BitB != NULL) {
            BMPOptions *BMP        = Options;
            uint32_t ImageSize     = (uint32_t) Bits2Bytes(BMP->Width * AbsoluteI(BMP->Height) * BMP->BitDepth, RoundingType_Up);
            uint32_t DIBHeaderSize = 40;
            uint64_t FileSize      = DIBHeaderSize + 2 + ImageSize;
            uint16_t NumPlanes     = 1; // Constant
            
            BitBuffer_WriteBits(BitB, MSByteFirst, LSBitFirst, 16, BMP_BM);
            BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, 32, FileSize);
            BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, 16, 0); // Reserved1
            BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, 16, 0); // Reserved2
            BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, 32, 2 + 40);
            BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, 32, DIBHeaderSize);
            /* Write DIB Header */
            BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, 32, BMP->Width);
            BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, 32, BMP->Height);
            
            BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, 16, NumPlanes);
            BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, 16, BMP->BitDepth);
            BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, 32, BMP->CompressionType);
            BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, 32, ImageSize);
            BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, 32, BMP->WidthPixelsPerMeter);
            BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, 32, BMP->HeightPixelsPerMeter);
            BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, 32, BMP->ColorsIndexed);
            BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, 32, BMP->IndexedColorsUsed);
        } else if (Options == NULL) {
            Log(Log_DEBUG, __func__, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_DEBUG, __func__, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    void BMPInsertImage(void *Options, void *Container, BitBuffer *BitB) {
        if (Options != NULL && Container != NULL && BitB != NULL) {
            BMPOptions *BMP       = Options;
            ImageContainer *Image = Container;
            uint64_t Width        = ImageContainer_GetWidth(Image);
            uint64_t Height       = ImageContainer_GetHeight(Image);
            ImageChannelMap *Map  = ImageContainer_GetChannelMap(Image);
            uint64_t NumChannels  = ImageChannelMap_GetNumChannels(Map);
            uint64_t BitDepth     = Bits2Bytes(ImageContainer_GetBitDepth(Image), RoundingType_Up);
            Image_Types Type      = ImageContainer_GetType(Image);
            
            if (Type == ImageType_Integer8) {
                uint8_t ****Array = (uint8_t****) ImageContainer_GetArray(Image);
                for (uint64_t W = 0; W < Width; W++) {
                    for (uint64_t H = 0; H < Height; H++) {
                        for (uint16_t C = 0; C < NumChannels; C++) {
                            BitBuffer_WriteBits(BitB, MSByteFirst, MSBitFirst, BitDepth, Array[0][W][H][C]);
                        }
                    }
                }
            } else if (Type == ImageType_Integer16) {
                uint16_t ****Array = (uint16_t****) ImageContainer_GetArray(Image);
                for (uint64_t W = 0; W < Width; W++) {
                    for (uint64_t H = 0; H < Height; H++) {
                        for (uint16_t C = 0; C < NumChannels; C++) {
                            BitBuffer_WriteBits(BitB, MSByteFirst, MSBitFirst, BitDepth, Array[0][W][H][C]);
                        }
                    }
                }
            }
        } else if (Options == NULL) {
            Log(Log_DEBUG, __func__, UTF8String("Options Pointer is NULL"));
        } else if (Container == NULL) {
            Log(Log_DEBUG, __func__, UTF8String("ImageContainer Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_DEBUG, __func__, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
    static void RegisterEncoder_BMP(OVIA *Ovia) {
        Ovia->NumEncoders                                    += 1;
        uint64_t EncoderIndex                                 = Ovia->NumEncoders;
        Ovia->Encoders                                        = realloc(Ovia->Encoders, sizeof(OVIAEncoder) * Ovia->NumEncoders);
        
        Ovia->Encoders[EncoderIndex].EncoderID                = CodecID_BMP;
        Ovia->Encoders[EncoderIndex].MediaType                = MediaType_Audio2D;
        Ovia->Encoders[EncoderIndex].Function_Initialize[0]   = BMPOptions_Init;
        Ovia->Encoders[EncoderIndex].Function_WriteHeader[0]  = BMPWriteHeader;
        Ovia->Encoders[EncoderIndex].Function_Encode[0]       = BMPInsertImage;
        Ovia->Encoders[EncoderIndex].Function_WriteFooter[0]  = NULL;
        Ovia->Encoders[EncoderIndex].Function_Deinitialize[0] = BMPOptions_Deinit;
    }
    
    static OVIACodecRegistry Register_BMPEncoder = {
        .Function_RegisterEncoder[CodecID_BMP - 1]   = RegisterEncoder_BMP,
    };
    
#ifdef __cplusplus
}
#endif
