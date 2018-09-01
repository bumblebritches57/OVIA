#include "../../../../Dependencies/FoundationIO/libFoundationIO/include/Macros.h"
#include "../../../include/Private/Image/BMPCommon.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    static uint64_t BMPGetRowSize(const uint16_t BitsPerPixel, const uint32_t ImageWidth) {
        return floor((BitsPerPixel * ImageWidth + 31) / 32) * 4; // floor((4 * 1024 + 31) / 32) * 4
    }
    
    static uint64_t BMPGetPixelArraySize(const uint64_t RowSize, const int32_t ImageHeight) { // 5568, 3712, there's 54 extra bytes in the PixelArray...
        return RowSize * ImageHeight;
    }
    
    void BMPParseMetadata(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            BitBuffer_Skip(BitB, 16);                           // Skip BM
            uint64_t DIBSize                                     = 0UL;
            uint64_t FileSize                                    = BitBuffer_ReadBits(LSByteFirst, LSBitFirst, BitB, 32);
            OVIA_SetFileSize(Ovia, FileSize);
            BitBuffer_Skip(BitB, 32);                           // 2 16 bit Reserved fields
            uint64_t Offset      = BitBuffer_ReadBits(LSByteFirst, LSBitFirst, BitB, 32);
            OVIA_SetSampleOffset(Ovia, Offset);
            if (Offset > 14) { // DIB Header
                DIBSize                                         = BitBuffer_ReadBits(LSByteFirst, LSBitFirst, BitB, 32); // 40
                if (DIBSize >= 40) {
                    uint64_t Width                              = BitBuffer_ReadBits(LSByteFirst, LSBitFirst, BitB, 32);
                    OVIA_SetWidth(Ovia, Width);
                    int64_t Height                              = BitBuffer_ReadBits(LSByteFirst, LSBitFirst, BitB, 32);
                    OVIA_SetHeight(Ovia, Height);
                } else if (DIBSize == 12) {
                    uint64_t Width                              = BitBuffer_ReadBits(LSByteFirst, LSBitFirst, BitB, 16);
                    OVIA_SetWidth(Ovia, Width);
                    int64_t Height                              = BitBuffer_ReadBits(LSByteFirst, LSBitFirst, BitB, 16);
                    OVIA_SetHeight(Ovia, Height);
                }
                BitBuffer_Skip(BitB, 16);                       // NumPlanes, always 1
                uint64_t BitDepth                                = BitBuffer_ReadBits(LSByteFirst, LSBitFirst, BitB, 16);
                OVIA_SetBitDepth(Ovia, BitDepth);
                BMPCompressionType                               = BitBuffer_ReadBits(LSByteFirst, LSBitFirst, BitB, 32);
                BitBuffer_Skip(BitB, 32); // NumBytesUsedBySamples
                BitBuffer_Skip(BitB, 32); // WidthPixelsPerMeter
                BitBuffer_Skip(BitB, 32); // HeightPixelsPerMeter
                BMPColorsIndexed                                 = BitBuffer_ReadBits(LSByteFirst, LSBitFirst, BitB, 32);
                BMPIndexedColorsUsed                             = BitBuffer_ReadBits(LSByteFirst, LSBitFirst, BitB, 32);
                if (DIBSize >= 56) {
                    BitBuffer_Skip(BitB, 32); // BMPColorSpaceType
                    if (DIBSize >= 108) {
                        BitBuffer_Skip(BitB, 32); // X Coordinate
                        BitBuffer_Skip(BitB, 32); // Y Coordinate
                        BitBuffer_Skip(BitB, 32); // Z Coordinate
                        BitBuffer_Skip(BitB, 32); // R Gamma
                        BitBuffer_Skip(BitB, 32); // G Gamma
                        BitBuffer_Skip(BitB, 32); // B Gamma
                        if (DIBSize >= 124) {
                            BitBuffer_Skip(BitB, 32); // ICC Intent
                            BitBuffer_Skip(BitB, 32); // ICC Payload
                            BitBuffer_Skip(BitB, 32); // ICC Payload Size
                            BitBuffer_Skip(BitB, 32); // More Reserved data.
                            // Ok so when the Height is positive, the image is upside down, the bottom of the image is at the top of the file.
                        }
                    }
                }
                BitBuffer_Skip(BitB, DIBSize - BitBuffer_GetPosition(BitB)); // Skip the remaining bits.
            } else if (DIBSize == 40 && BMPCompressionType == BMP_BitFields || BMPCompressionType == BMP_RGBABitFields) {
                BMPRedMask                                       = BitBuffer_ReadBits(LSByteFirst, LSBitFirst, BitB, OVIA_GetBitDepth(Ovia));
                BMPGreenMask                                     = BitBuffer_ReadBits(LSByteFirst, LSBitFirst, BitB, OVIA_GetBitDepth(Ovia));
                BMPBlueMask                                      = BitBuffer_ReadBits(LSByteFirst, LSBitFirst, BitB, OVIA_GetBitDepth(Ovia));
                if (BMPCompressionType == BMP_RGBABitFields) {
                    BMPAlphaMask                                 = BitBuffer_ReadBits(LSByteFirst, LSBitFirst, BitB, OVIA_GetBitDepth(Ovia));
                }
            } else {
                BitBuffer_Skip(BitB, Bits2Bytes((OVIA_GetSampleOffset(Ovia) - 14) - DIBSize, false));
            }
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    ImageContainer *BMPExtractImage(OVIA *Ovia, BitBuffer *BitB) {
        ImageContainer *Image = NULL;
        if (Ovia != NULL && BitB != NULL) {
            // We need to create a ImageContainer, based on the bit depth of the image.
            uint64_t BitDepth    = OVIA_GetBitDepth(Ovia);
            uint64_t NumChannels = OVIA_GetNumChannels(Ovia);
            uint64_t Width       = OVIA_GetWidth(Ovia);
            int64_t  Height      = OVIA_GetHeight(Ovia);
            if (BitDepth <= 8) {
                Image = ImageContainer_Init(ImageContainer_2DUInteger8, BitDepth, NumChannels, Width, Height);
            } else if (BitDepth > 8 && BitDepth <= 16) {
                Image = ImageContainer_Init(ImageContainer_2DUInteger16, BitDepth, NumChannels, Width, Height);
            }
            
            if (OVIA_GetHeight(Ovia) < 0) {
                // Read from the bottom to the top.
                if (BMPCompressionType == BMP_RGB) {
                    if (BitDepth <= 8) {
                        uint8_t ****Array  = (uint8_t****) ImageContainer_GetArray(Image);
                        for (uint64_t View = 0ULL; View < ImageContainer_GetNumViews(Image); View++) {
                            for (uint64_t W = 0ULL; W < Width; W++) {
                                for (uint64_t H = 0ULL; H < Height; H++) {
                                    for (uint64_t Channel = 0ULL; Channel < NumChannels; Channel++) {
                                        Array[View][W][H][Channel] = BitBuffer_ReadBits(LSByteFirst, LSBitFirst, BitB, Bits2Bytes(BitDepth, Yes));
                                    }
                                }
                            }
                        }
                    } else if (BitDepth > 8 && BitDepth <= 16) {
                        uint16_t ****Array = (uint16_t****) ImageContainer_GetArray(Image);
                        for (uint64_t View = 0ULL; View < ImageContainer_GetNumViews(Image); View++) {
                            for (uint64_t W = 0ULL; W < Width; W++) {
                                for (uint64_t H = 0ULL; H < Height; H++) {
                                    for (uint64_t Channel = 0ULL; Channel < NumChannels; Channel++) {
                                        Array[View][W][H][Channel] = BitBuffer_ReadBits(LSByteFirst, LSBitFirst, BitB, Bits2Bytes(BitDepth, Yes));
                                    }
                                }
                            }
                        }
                    }
                } else if (BMPCompressionType == BMP_RLE_8Bit || BMPCompressionType == BMP_RLE_4Bit) {
                    Log(Log_ERROR, __func__, U8("We don't support BMP's encoded with RLE"));
                } else if (BMPCompressionType == BMP_BitFields || BMPCompressionType == BMP_RGBABitFields) {
                    Log(Log_ERROR, __func__, U8("We don't support BMP's encoded with BitFields"));
                } else if (BMPCompressionType == BMP_JPEG) {
                    Log(Log_ERROR, __func__, U8("BMP Compression Type is JPEG, there's nothing we can do since JPEG is a lossy format"));
                } else if (BMPCompressionType == BMP_PNG) {
                    Log(Log_ERROR, __func__, U8("BMP Compression Type is PNG, extract it as is or OVIA it to uncompressed BMP"));
                } else if (BMPCompressionType == BMP_CMYK || BMPCompressionType == BMP_CMYK_RLE_8Bit || BMPCompressionType == BMP_CMYK_RLE_4Bit) {
                    Log(Log_ERROR, __func__, U8("CMYK Images are unsupported"));
                }
                if (Height < 0) { // The Image is upside down, so we need to flip it
                    ImageContainer_Flip(Image, Yes, No);
                }
            }
            if (BMPColorsIndexed > 0) {
                // The image is palettized, so we need to go ahead and map the pixel bits to the actual colors.
            }
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitB Pointer is NULL"));
        }
        return Image;
    }
    
#ifdef __cplusplus
}
#endif
