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
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("Options Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("BitBuffer Pointer is NULL"));
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
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("Options Pointer is NULL"));
        } else if (Container == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("ImageContainer Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    
#define NumBMPExtensions 2
    
    static const UTF32 *BMPExtensions[NumBMPExtensions] = {
        [0] = UTF32String("bmp"),
        [1] = UTF32String("dib"),
    };
    
    static const OVIAEncoder BMPEncoder = {
        .EncoderID             = CodecID_BMP,
        .MediaType             = MediaType_Image,
        .NumExtensions         = NumBMPExtensions,
        .Extensions            = BMPExtensions,
        .Function_Initialize   = BMPOptions_Init,
        .Function_WriteHeader  = BMPWriteHeader,
        .Function_Encode       = BMPInsertImage,
        .Function_Deinitialize = BMPOptions_Deinit,
    };
    
#ifdef __cplusplus
}
#endif
