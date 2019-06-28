#include "../../include/Private/BMPCommon.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    void BMPWriteHeader(BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            uint32_t ImageSize     = Bits2Bytes(OVIA_GetWidth(Ovia) * OVIA_GetHeight(Ovia) * OVIA_GetBitDepth(Ovia), RoundingType_Down);
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
            BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, 32, OVIA_GetWidth(Ovia));
            BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, 32, OVIA_GetHeight(Ovia));
            
            BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, 16, NumPlanes);
            BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, 16, OVIA_GetBitDepth(Ovia));
            BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, 32, OVIA_BMP_GetCompressionType(Ovia));
            BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, 32, ImageSize);
            BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, 32, OVIA_BMP_GetWidthInMeters(Ovia));
            BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, 32, OVIA_BMP_GetHeightInMeters(Ovia));
            BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, 32, OVIA_BMP_GetColorsIndexed(Ovia));
            BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, 32, OVIA_BMP_GetIndexColorsUsed(Ovia));
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void BMPInsertImage(ImageContainer *Image, BitBuffer *BitB) {
        if (Image != NULL && BitB != NULL) {
            uint64_t Width       = ImageContainer_GetWidth(Image);
            uint64_t Height      = ImageContainer_GetHeight(Image);
            uint64_t NumChannels = ImageContainer_GetNumChannels(Image);
            uint64_t BitDepth    = Bits2Bytes(ImageContainer_GetBitDepth(Image), RoundingType_Up);
            Image_Types Type     = ImageContainer_GetType(Image);
            
            if (Type == ImageType_Integer8) {
                uint8_t *Array = (uint8_t*) ImageContainer_GetArray(Image);
                for (uint64_t W = 0; W < Width; W++) {
                    for (uint64_t H = 0; H < Height; H++) {
                        for (uint16_t C = 0; C < NumChannels; C++) {
                            BitBuffer_WriteBits(BitB, MSByteFirst, MSBitFirst, BitDepth, Array[W * H * C]);
                        }
                    }
                }
            } else if (Type == AudioType_Integer16) {
                uint16_t *Array = (uint16_t*) ImageContainer_GetArray(Image);
                for (uint64_t W = 0; W < Width; W++) {
                    for (uint64_t H = 0; H < Height; H++) {
                        for (uint16_t C = 0; C < NumChannels; C++) {
                            BitBuffer_WriteBits(BitB, MSByteFirst, MSBitFirst, BitDepth, Array[W * H * C]);
                        }
                    }
                }
            }
        } else if (Image == NULL) {
            Log(Log_ERROR, __func__, U8("ImageContainer Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    OVIAEncoder BMPEncoder = {
        .EncoderID             = CodecID_BMP,
        .MediaType             = MediaType_Image,
        .Function_Initialize   = BMPOptions_Init,
        .Function_WriteHeader  = BMPWriteHeader,
        .Function_Encode       = BMPInsertImage,
        .Function_WriteFooter  = NULL,
        .Function_Deinitialize = BMPOptions_Deinit,
    };
    
#ifdef __cplusplus
}
#endif
