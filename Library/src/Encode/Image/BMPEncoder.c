#include "../../../include/Private/Image/BMPCommon.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    void SetBMPParameters(OVIA *Ovia, uint32_t Height, uint32_t Width, uint8_t NumChannels, uint16_t BitDepth) {
        Ovia->Pic->Height = Height;
        Ovia->Pic->Width  = Width;
        Ovia->BitDepth    = BitDepth;
        Ovia->NumChannels = NumChannels;
    }
    
    void BMPWriteHeader(OVIA *Ovia, BitBuffer *BitB) {
        WriteBits(MSByteFirst, LSBitFirst, BitB, 16, BMP_BM);
        Ovia->FileSize = 2 + 40 + Bits2Bytes(Ovia->NumChannels * Ovia->Width * Ovia->Height, true); // Plus the various headers and shit.
        WriteBits(LSByteFirst, LSBitFirst, BitB, 32, Ovia->FileSize);
        WriteBits(LSByteFirst, LSBitFirst, BitB, 16, 0); // Reserved1
        WriteBits(LSByteFirst, LSBitFirst, BitB, 16, 0); // Reserved2
        WriteBits(LSByteFirst, LSBitFirst, BitB, 32, Ovia->Pic->BMPPixelOffset);
        uint32_t DIBHeaderSize = 40;
        WriteBits(LSByteFirst, LSBitFirst, BitB, 32, DIBHeaderSize);
        /* Write DIB Header */
        WriteBits(LSByteFirst, LSBitFirst, BitB, 32, Ovia->Pic->Width);
        WriteBits(LSByteFirst, LSBitFirst, BitB, 32, Ovia->Pic->Height);
        uint16_t NumPlanes = 1; // Constant
        WriteBits(LSByteFirst, LSBitFirst, BitB, 16, NumPlanes);
        WriteBits(LSByteFirst, LSBitFirst, BitB, 16, Ovia->BitDepth);
        WriteBits(LSByteFirst, LSBitFirst, BitB, 32, Ovia->Pic->BMPCompressionType);
        uint32_t ImageSize = Bits2Bytes(NumPixels * Ovia->BitDepth, No);
        WriteBits(LSByteFirst, LSBitFirst, BitB, 32, ImageSize);
        WriteBits(LSByteFirst, LSBitFirst, BitB, 32, Ovia->Pic->BMPWidthPixelsPerMeter);
        WriteBits(LSByteFirst, LSBitFirst, BitB, 32, Ovia->Pic->BMPHeightPixelsPerMeter);
        WriteBits(LSByteFirst, LSBitFirst, BitB, 32, Ovia->Pic->BMPColorsIndexed);
        WriteBits(LSByteFirst, LSBitFirst, BitB, 32, Ovia->Pic->BMPIndexColorsUsed);
    }
    
    void BMPInsertImage(OVIA *Ovia, BitBuffer *CreatedImage, uint16_t ***Image2Insert) {
        if (PCM != NULL && CreatedImage != NULL && Image2Insert != NULL) {
            for (uint64_t Width = 0; Width < Ovia->Pic->Width; Width++) {
                for (uint64_t Height = 0; Height < Ovia->Pic->Height; Height++) {
                    for (uint16_t Channel = 0; Channel < Ovia->NumChannels; Channel++) {
                        WriteBits(MSByteFirst, MSBitFirst, CreatedImage, Ovia->BitDepth, Image2Insert[Width][Height][Channel]);
                    }
                }
            }
        } else if (PCM == NULL) {
            Log(Log_ERROR, __func__, U8("PCM Pointer is NULL"));
        } else if (CreatedImage == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        } else if (Image2Insert == NULL) {
            Log(Log_ERROR, __func__, U8("Pixels2Write Pointer is NULL"));
        }
    }
    
#ifdef __cplusplus
}
#endif
