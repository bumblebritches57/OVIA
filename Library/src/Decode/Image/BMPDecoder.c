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
    
    static uint8_t BMPDecompressRLE8(uint8_t Count, uint8_t Pixel) {
        /*
         End of Scanline Marker: 0
         End of File Marker: 1
         RLE8 Delta: 2; Contains a vertical and horizontal coordinate for the value
         There are 2 modes, absolute and RLE
         The first byte is the run length, the second byte is the value; special values as defined above have a run length of 0.
         */
        return 0;
    }
    
    static uint8_t BMPDecompressRLE4(uint8_t Count, uint8_t Pixels) {
        /*
         End of Scanline Marker: 0
         End of File Marker: 1
         RLE8 Delta: 2; Contains a vertical and horizontal coordinate for the value
         There are 2 modes, absolute and RLE
         The first byte is the run length, the second byte is the value; special values as defined above have a run length of 0.
         */
        return 0;
    }
    
    static void BMPParsePalette(OVIA *Ovia, BitBuffer *BitB) {
        
    }
    
    void BMPParseMetadata(OVIA *Ovia, BitBuffer *BitB) {
        BitBuffer_Skip(BitB, 16);                           // Skip BM
        uint32_t DIBSize                                    = 0UL;
        Ovia->FileSize                                       = ReadBits(LSByteFirst, LSBitFirst, BitB, 32); // 1049654
        BitBuffer_Skip(BitB, 32);                           // 2 16 bit Reserved fields
        Ovia->Pic->BMPPixelOffset                            = ReadBits(LSByteFirst, LSBitFirst, BitB, 32); // 1078
        if (Ovia->Pic->BMPPixelOffset > 14) { // DIB Header
            DIBSize                                         = ReadBits(LSByteFirst, LSBitFirst, BitB, 32); // 40
            if (DIBSize >= 40) {
                Ovia->Pic->Width                             = ReadBits(LSByteFirst, LSBitFirst, BitB, 32); // 1024
                int32_t Height                              = ReadBits(LSByteFirst, LSBitFirst, BitB, 32); // 1024
                if (Height < 0) {
                    Ovia->Pic->ImageIsTopDown                = Yes;
                } else {
                    Ovia->Pic->ImageIsTopDown                = No;
                }
                Ovia->Pic->Height                            = Absolute(Height);
            } else if (DIBSize == 12) {
                Ovia->Pic->Width                             = ReadBits(LSByteFirst, LSBitFirst, BitB, 16);
                int16_t Height                              = ReadBits(LSByteFirst, LSBitFirst, BitB, 16);
                if (Height < 0) {
                    Ovia->Pic->ImageIsTopDown                = Yes;
                } else {
                    Ovia->Pic->ImageIsTopDown                = No;
                }
                Ovia->Pic->Height                            = Absolute(Height);
            }
            BitBuffer_Skip(BitB, 16);                       // NumPlanes, always 1
            Ovia->BitDepth                                   = ReadBits(LSByteFirst, LSBitFirst, BitB, 16); // 8
            Ovia->Pic->BMPCompressionType                    = ReadBits(LSByteFirst, LSBitFirst, BitB, 32); // 0
            Ovia->Pic->NumBytesUsedBySamples                 = ReadBits(LSByteFirst, LSBitFirst, BitB, 32); // 16
            Ovia->Pic->BMPWidthPixelsPerMeter                = ReadBits(LSByteFirst, LSBitFirst, BitB, 32); // 3780
            Ovia->Pic->BMPHeightPixelsPerMeter               = ReadBits(LSByteFirst, LSBitFirst, BitB, 32); // 3780
            Ovia->Pic->BMPColorsIndexed                      = ReadBits(LSByteFirst, LSBitFirst, BitB, 32); // 0
            Ovia->Pic->BMPIndexColorsUsed                    = ReadBits(LSByteFirst, LSBitFirst, BitB, 32); // 0
            if (DIBSize >= 56) {
                Ovia->Pic->BMPColorSpaceType                 = ReadBits(LSByteFirst, LSBitFirst, BitB, 32); // 0000FF00 aka 16,711,680
                if (DIBSize >= 108) {
                    Ovia->Pic->BMPXYZCoordinates[0]          = ReadBits(LSByteFirst, LSBitFirst, BitB, 32); // 65280
                    Ovia->Pic->BMPXYZCoordinates[1]          = ReadBits(LSByteFirst, LSBitFirst, BitB, 32); // 256
                    Ovia->Pic->BMPXYZCoordinates[2]          = ReadBits(LSByteFirst, LSBitFirst, BitB, 32); // 4,278,190,080
                    Ovia->Pic->BMPRedGamma                   = ReadBits(LSByteFirst, LSBitFirst, BitB, 32); // 1,934,772,034
                    Ovia->Pic->BMPGreenGamma                 = ReadBits(LSByteFirst, LSBitFirst, BitB, 32); // 687,194,752
                    Ovia->Pic->BMPBlueGamma                  = ReadBits(LSByteFirst, LSBitFirst, BitB, 32); // 354,334,816
                    if (DIBSize >= 124) {
                        Ovia->Pic->BMPICCIntent              = ReadBits(LSByteFirst, LSBitFirst, BitB, 32); // 32,212,256
                        Ovia->Pic->BMPICCPayload             = ReadBits(LSByteFirst, LSBitFirst, BitB, 32); // 322,122,560
                        Ovia->Pic->BMPICCPayloadSize         = ReadBits(LSByteFirst, LSBitFirst, BitB, 32); // 644,245,120
                        BitBuffer_Skip(BitB, 32); // More Reserved data.
                        // Ok so when the Height is positive, the image is upside down, the bottom of the image is at the top of the file.
                    }
                }
            }
            BitBuffer_Skip(BitB, DIBSize - BitBuffer_GetPosition(BitB)); // Skip the remaining bits.
        } else if (DIBSize == 40 && (Ovia->Pic->BMPCompressionType == BMP_BitFields || Ovia->Pic->BMPCompressionType == BMP_RGBABitFields)) {
            Ovia->Pic->BMPRedMask                            = ReadBits(LSByteFirst, MSBitFirst, BitB, Ovia->BitDepth);
            Ovia->Pic->BMPGreenMask                          = ReadBits(LSByteFirst, LSBitFirst, BitB, Ovia->BitDepth);
            Ovia->Pic->BMPBlueMask                           = ReadBits(MSByteFirst, LSBitFirst, BitB, Ovia->BitDepth);
            if (Ovia->Pic->BMPCompressionType == BMP_RGBABitFields) {
                Ovia->Pic->BMPAlphaMask                      = ReadBits(MSByteFirst, LSBitFirst, BitB, Ovia->BitDepth);
            }
        } else {
            BitBuffer_Skip(BitB, Bits2Bytes((Ovia->Pic->BMPPixelOffset - 14) - DIBSize, false));
        }
    }
    
    uint16_t ***BMPExtractImage(OVIA *Ovia, BitBuffer *BitB) { // We need to convert the pixels to the Runtime Byte and Bit order.
        uint16_t ***ExtractedImage                  = calloc(Ovia->Pic->Width * Ovia->Pic->Height * Ovia->NumChannels, sizeof(uint16_t**));
        if (PCM != NULL && BitB != NULL && ExtractedImage != NULL) {
            if (Ovia->Pic->ImageIsTopDown == Yes) {
                // Read from the bottom to the top.
                for (uint32_t HeightLine = 0; HeightLine < Ovia->Pic->Height; HeightLine++) {
                    for (uint32_t WidthLine = 0; WidthLine < Ovia->Pic->Width; WidthLine++) {
                        for (uint16_t Channel = 0; Channel < Ovia->NumChannels; Channel++) { // Ok, this works when the bit depth is 8 bits per pixel, but what about 1 bit images, or palettized ones?
                            if (Ovia->Pic->BMPCompressionType == BMP_RGB) {
                                ExtractedImage[HeightLine][WidthLine][Channel] = ReadBits(LSByteFirst, LSBitFirst, BitB, Ovia->BitDepth);
                            } else if (Ovia->Pic->BMPCompressionType == BMP_RLE_8Bit) {
                                uint8_t Count       = ReadBits(LSByteFirst, LSBitFirst, BitB, 8);
                                uint8_t PixelIndex  = ReadBits(LSByteFirst, LSBitFirst, BitB, 8);
                            } else if (Ovia->Pic->BMPCompressionType == BMP_RLE_4Bit) {
                                uint8_t Count       = ReadBits(LSByteFirst, LSBitFirst, BitB, 8);
                                uint8_t NibblePixel = ReadBits(LSByteFirst, LSBitFirst, BitB, 8);
                            } else if (Ovia->Pic->BMPCompressionType == BMP_BitFields) {
                                if (Channel == 0) {
                                    ExtractedImage[HeightLine][WidthLine][0] = ReadBits(LSByteFirst, LSBitFirst, BitB, Ovia->Pic->BMPRedMask);
                                } else if (Channel == 1) {
                                    ExtractedImage[HeightLine][WidthLine][1] = ReadBits(LSByteFirst, LSBitFirst, BitB, Ovia->Pic->BMPGreenMask);
                                } else if (Channel == 2) {
                                    ExtractedImage[HeightLine][WidthLine][2] = ReadBits(LSByteFirst, LSBitFirst, BitB, Ovia->Pic->BMPBlueMask);
                                }
                            } else if (Ovia->Pic->BMPCompressionType == BMP_RGBABitFields) {
                                if (Channel == 0) {
                                    ExtractedImage[HeightLine][WidthLine][0] = ReadBits(LSByteFirst, LSBitFirst, BitB, Ovia->Pic->BMPRedMask);
                                } else if (Channel == 1) {
                                    ExtractedImage[HeightLine][WidthLine][1] = ReadBits(LSByteFirst, LSBitFirst, BitB, Ovia->Pic->BMPGreenMask);
                                } else if (Channel == 2) {
                                    ExtractedImage[HeightLine][WidthLine][2] = ReadBits(LSByteFirst, LSBitFirst, BitB, Ovia->Pic->BMPBlueMask);
                                } else if (Channel == 3) {
                                    ExtractedImage[HeightLine][WidthLine][3] = ReadBits(LSByteFirst, LSBitFirst, BitB, Ovia->Pic->BMPAlphaMask);
                                }
                            } else if (Ovia->Pic->BMPCompressionType == BMP_JPEG) {
                                Log(Log_ERROR, __func__, U8("BMP Compression Type is JPEG, there's nothing we can do since JPEG is a lossy format"));
                            } else if (Ovia->Pic->BMPCompressionType == BMP_PNG) {
                                Log(Log_ERROR, __func__, U8("BMP Compression Type is PNG, decode it to uncompressed BMP"));
                            } else if (Ovia->Pic->BMPCompressionType == BMP_CMYK || Ovia->Pic->BMPCompressionType == BMP_CMYK_RLE_8Bit || Ovia->Pic->BMPCompressionType == BMP_CMYK_RLE_4Bit) {
                                Log(Log_ERROR, __func__, U8("CMYK Images are unsupported"));
                            }
                        }
                    }
                }
            } else {
                // Read from the bottom to the top.
                for (uint32_t HeightLine = Absolute(Ovia->Pic->Height); HeightLine > 0; HeightLine--) {
                    for (uint32_t WidthLine = 0; WidthLine < Ovia->Pic->Width; WidthLine++) {
                        for (uint16_t Channel = 0; Channel < Ovia->NumChannels; Channel++) { // Ok, this works when the bit depth is 8 bits per pixel, but what about 1 bit images, or palettized ones?
                            if (Ovia->Pic->BMPCompressionType == BMP_RGB) {
                                ExtractedImage[HeightLine][WidthLine][Channel] = ReadBits(LSByteFirst, LSBitFirst, BitB, Ovia->BitDepth);
                            } else if (Ovia->Pic->BMPCompressionType == BMP_RLE_8Bit) {
                                uint8_t Count       = ReadBits(LSByteFirst, LSBitFirst, BitB, 8);
                                uint8_t PixelIndex  = ReadBits(LSByteFirst, LSBitFirst, BitB, 8);
                                // Look up PixelIndex in the Palette, set Count pixels to that value
                                Ovia->Pic->BMPPalette[PixelIndex]; // TODO: Finish the palette lookup
                            } else if (Ovia->Pic->BMPCompressionType == BMP_RLE_4Bit) {
                                uint8_t Count       = ReadBits(LSByteFirst, LSBitFirst, BitB, 8);
                                uint8_t NibblePixel = ReadBits(LSByteFirst, LSBitFirst, BitB, 8);
                            } else if (Ovia->Pic->BMPCompressionType == BMP_BitFields) {
                                if (Channel == 0) {
                                    ExtractedImage[HeightLine][WidthLine][0] = ReadBits(LSByteFirst, LSBitFirst, BitB, Ovia->Pic->BMPRedMask);
                                } else if (Channel == 1) {
                                    ExtractedImage[HeightLine][WidthLine][1] = ReadBits(LSByteFirst, LSBitFirst, BitB, Ovia->Pic->BMPGreenMask);
                                } else if (Channel == 2) {
                                    ExtractedImage[HeightLine][WidthLine][2] = ReadBits(LSByteFirst, LSBitFirst, BitB, Ovia->Pic->BMPBlueMask);
                                }
                            } else if (Ovia->Pic->BMPCompressionType == BMP_RGBABitFields) {
                                if (Channel == 0) {
                                    ExtractedImage[HeightLine][WidthLine][0] = ReadBits(LSByteFirst, LSBitFirst, BitB, Ovia->Pic->BMPRedMask);
                                } else if (Channel == 1) {
                                    ExtractedImage[HeightLine][WidthLine][1] = ReadBits(LSByteFirst, LSBitFirst, BitB, Ovia->Pic->BMPGreenMask);
                                } else if (Channel == 2) {
                                    ExtractedImage[HeightLine][WidthLine][2] = ReadBits(LSByteFirst, LSBitFirst, BitB, Ovia->Pic->BMPBlueMask);
                                } else if (Channel == 3) {
                                    ExtractedImage[HeightLine][WidthLine][3] = ReadBits(LSByteFirst, LSBitFirst, BitB, Ovia->Pic->BMPAlphaMask);
                                }
                            } else if (Ovia->Pic->BMPCompressionType == BMP_JPEG) {
                                Log(Log_ERROR, __func__, U8("BMP Compression Type is JPEG, there's nothing we can do since JPEG is a lossy format"));
                            } else if (Ovia->Pic->BMPCompressionType == BMP_PNG) {
                                Log(Log_ERROR, __func__, U8("BMP Compression Type is PNG, decode it to uncompressed BMP"));
                            } else if (Ovia->Pic->BMPCompressionType == BMP_CMYK || Ovia->Pic->BMPCompressionType == BMP_CMYK_RLE_8Bit || Ovia->Pic->BMPCompressionType == BMP_CMYK_RLE_4Bit) {
                                Log(Log_ERROR, __func__, U8("CMYK Images are unsupported"));
                            }
                        }
                    }
                }
            }
            if (Ovia->Pic->BMPColorsIndexed > 0) {
                // The image is palettized, so we need to go ahead and map the pixel bits to the actual colors.
            }
        } else if (PCM == NULL) {
            Log(Log_ERROR, __func__, U8("PCM Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitB Pointer is NULL"));
        } else if (ExtractedImage == NULL) {
            Log(Log_ERROR, __func__, U8("Couldn't allocate enough memory to decode the image"));
        }
        
        return ExtractedImage;
    }
    
    Container *BMPExtractImage2(OVIA *Ovia, BitBuffer *BitB) {
        Container *Container = NULL;
        uint8_t   *Pixels8   = NULL;
        uint16_t  *Pixels16  = NULL;
        
        if (PCM != NULL && BitB != NULL) {
            if (Ovia->BitDepth <= 8) {
                Pixels8        = calloc(Ovia->Pic->Width * Ovia->Pic->Height * Ovia->NumChannels, sizeof(uint8_t));
                Container      = Container_Init(Ovia->NumChannels, Ovia->Pic->Width * Ovia->Pic->Height, UInteger8Array);
                Container_Attach(Container, Pixels8);
            } else if (Ovia->BitDepth <= 16) {
                Pixels16       = calloc(Ovia->Pic->Width * Ovia->Pic->Height * Ovia->NumChannels, sizeof(uint16_t));
                Container      = Container_Init(Ovia->NumChannels, Ovia->Pic->Width * Ovia->Pic->Height, UInteger16Array);
                Container_Attach(Container, Pixels16);
            }
            
            if (Ovia->Pic->ImageIsTopDown == Yes) {
                // Read from the bottom to the top.
                for (uint32_t HeightLine = 0; HeightLine < Ovia->Pic->Height; HeightLine++) {
                    for (uint32_t WidthLine = 0; WidthLine < Ovia->Pic->Width; WidthLine++) {
                        for (uint16_t Channel = 0; Channel < Ovia->NumChannels; Channel++) { // Ok, this works when the bit depth is 8 bits per pixel, but what about 1 bit images, or palettized ones?
                            if (Ovia->BitDepth <= 8) {
                                Pixels8[HeightLine * WidthLine * Channel]  = ReadBits(LSByteFirst, LSBitFirst, BitB, Ovia->BitDepth);
                            } else if (Ovia->BitDepth <= 16) {
                                Pixels16[HeightLine * WidthLine * Channel] = ReadBits(LSByteFirst, LSBitFirst, BitB, Ovia->BitDepth);
                            }
                        }
                    }
                }
            } else {
                // Read from the bottom to the top.
                for (uint32_t HeightLine = Absolute(Ovia->Pic->Height); HeightLine > 0; HeightLine--) {
                    for (uint32_t WidthLine = 0; WidthLine < Ovia->Pic->Width; WidthLine++) {
                        for (uint16_t Channel = 0; Channel < Ovia->NumChannels; Channel++) { // Ok, this works when the bit depth is 8 bits per pixel, but what about 1 bit images, or palettized ones?
                            if (Ovia->BitDepth <= 8) {
                                Pixels8[HeightLine * WidthLine * Channel]  = ReadBits(LSByteFirst, LSBitFirst, BitB, Ovia->BitDepth);
                            } else if (Ovia->BitDepth <= 16) {
                                Pixels16[HeightLine * WidthLine * Channel] = ReadBits(LSByteFirst, LSBitFirst, BitB, Ovia->BitDepth);
                            }
                            if (Ovia->Pic->BMPColorsIndexed == Yes) {
                                // Look up Pixels8[HeightLine * WidthLine * Channel] in the index.
                                Ovia->Pic->BMPColorsIndexed;
                            }
                        }
                    }
                }
            }
            if (Ovia->Pic->BMPColorsIndexed > 0) {
                // The image is palettized, so we need to go ahead and map the pixel bits to the actual colors.
                // Basically just look up the value
            }
        }
        return Container;
    }
    
#ifdef __cplusplus
}
#endif
