#include "../../../include/Private/Image/BMPCommon.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    void BMPWriteHeader(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            uint32_t ImageSize     = Bits2Bytes(OVIA_GetWidth(Ovia) * OVIA_GetHeight(Ovia) * OVIA_GetBitDepth(Ovia), false);
            uint32_t DIBHeaderSize = 40;
            uint64_t FileSize      = DIBHeaderSize + 2 + ImageSize;
            uint16_t NumPlanes     = 1; // Constant
            
            BitBuffer_WriteBits(MSByteFirst, LSBitFirst, BitB, 16, BMP_BM);
            BitBuffer_WriteBits(LSByteFirst, LSBitFirst, BitB, 32, FileSize);
            BitBuffer_WriteBits(LSByteFirst, LSBitFirst, BitB, 16, 0); // Reserved1
            BitBuffer_WriteBits(LSByteFirst, LSBitFirst, BitB, 16, 0); // Reserved2
            BitBuffer_WriteBits(LSByteFirst, LSBitFirst, BitB, 32, 2 + 40);
            BitBuffer_WriteBits(LSByteFirst, LSBitFirst, BitB, 32, DIBHeaderSize);
            /* Write DIB Header */
            BitBuffer_WriteBits(LSByteFirst, LSBitFirst, BitB, 32, OVIA_GetWidth(Ovia));
            BitBuffer_WriteBits(LSByteFirst, LSBitFirst, BitB, 32, OVIA_GetHeight(Ovia));
            
            BitBuffer_WriteBits(LSByteFirst, LSBitFirst, BitB, 16, NumPlanes);
            BitBuffer_WriteBits(LSByteFirst, LSBitFirst, BitB, 16, OVIA_GetBitDepth(Ovia));
            BitBuffer_WriteBits(LSByteFirst, LSBitFirst, BitB, 32, OVIA_BMP_GetCompressionType(Ovia));
            BitBuffer_WriteBits(LSByteFirst, LSBitFirst, BitB, 32, ImageSize);
            BitBuffer_WriteBits(LSByteFirst, LSBitFirst, BitB, 32, OVIA_BMP_GetWidthInMeters(Ovia));
            BitBuffer_WriteBits(LSByteFirst, LSBitFirst, BitB, 32, OVIA_BMP_GetHeightInMeters(Ovia));
            BitBuffer_WriteBits(LSByteFirst, LSBitFirst, BitB, 32, OVIA_BMP_GetColorsIndexed(Ovia));
            BitBuffer_WriteBits(LSByteFirst, LSBitFirst, BitB, 32, OVIA_BMP_GetIndexColorsUsed(Ovia));
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
            uint64_t BitDepth    = Bits2Bytes(ImageContainer_GetBitDepth(Image), true);
            Image_Types Type     = ImageContainer_GetType(Image);
            
            if (Type == ImageType_Integer8) {
                uint8_t *Array = (uint8_t*) ImageContainer_GetArray(Image);
                for (uint64_t W = 0; W < Width; W++) {
                    for (uint64_t H = 0; H < Height; H++) {
                        for (uint16_t C = 0; C < NumChannels; C++) {
                            BitBuffer_WriteBits(MSByteFirst, MSBitFirst, BitB, BitDepth, Array[W * H * C]);
                        }
                    }
                }
            } else if (Type == AudioType_Integer16) {
                uint16_t *Array = (uint16_t*) ImageContainer_GetArray(Image);
                for (uint64_t W = 0; W < Width; W++) {
                    for (uint64_t H = 0; H < Height; H++) {
                        for (uint16_t C = 0; C < NumChannels; C++) {
                            BitBuffer_WriteBits(MSByteFirst, MSBitFirst, BitB, BitDepth, Array[W * H * C]);
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
    
    OVIACodecs OVIACodecList = {
        .MagicID[CodecID_BMP        - CodecType_Encode] = (uint8_t[]) {0x42, 0x4D},
        .MagicIDSize[CodecID_BMP    - CodecType_Encode] = 2,
        .MagicIDOffset[CodecID_BMP  - CodecType_Encode] = 0,
        .MediaTypes[CodecID_BMP     - CodecType_Encode] = MediaType_Image,
        .CodecTypes[CodecID_BMP     - CodecType_Encode] = CodecType_Encode,
        .ParseFunction[CodecID_BMP  - CodecType_Encode] = BMPParseMetadata,
        .EncodeImage[CodecID_BMP    - CodecType_Encode] = BMPInsertImage,
    };
    
#ifdef __cplusplus
}
#endif
