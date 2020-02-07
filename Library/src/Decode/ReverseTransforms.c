#include "../../include/Private/OVIACommon.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    void ReverseTransform_RCT(ImageContainer *Image) {
        if (Image != NULL) {
            ImageChannelMap *Map  = ImageContainer_GetChannelMap(Image);
            uint8_t  NumChannels  = ImageChannelMap_GetNumChannels(Map);
            if (NumChannels < 2 || NumChannels > 4) {
                Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("NumChannels %u must be 3 or 4"), NumChannels);
            } else {
                //Image_ChannelMask RGBMask                         = ImageMask_Red | ImageMask_Green | ImageMask_Blue;
                //Image_ChannelMask RGBAMask                        = ImageMask_Red | ImageMask_Green | ImageMask_Blue | ImageMask_Alpha;
                
                //if (Mask == RGBMask || Mask == RGBAMask) {
                ImageChannelMap *Map                          = ImageContainer_GetChannelMap(Image);
                uint8_t  Chroma1Index                         = ImageChannelMap_GetChannelsIndex(Map, ImageMask_Chroma1);
                uint8_t  LumaIndex                            = ImageChannelMap_GetChannelsIndex(Map, ImageMask_Luma);
                uint8_t  Chroma2Index                         = ImageChannelMap_GetChannelsIndex(Map, ImageMask_Chroma2);
                
                uint8_t  NumViews                             = ImageChannelMap_GetNumViews(Map);
                uint8_t  BitDepth                             = ImageContainer_GetBitDepth(Image);
                uint64_t Width                                = ImageContainer_GetWidth(Image);
                uint64_t Height                               = ImageContainer_GetHeight(Image);
                
                if (BitDepth <= 8) {
                    uint8_t ****Array                         = (uint8_t****) ImageContainer_GetArray(Image);
                    
                    for (uint8_t View = 0; View < NumViews; View++) {
                        for (uint64_t W = 0; W < Width; W++) {
                            for (uint64_t H = 0; H < Height; H++) {
                                uint8_t Luma                    = Array[View][W][H][LumaIndex];    // 69
                                uint8_t Cr                      = Array[View][W][H][Chroma1Index]; // 157
                                uint8_t Cb                      = Array[View][W][H][Chroma2Index]; // 68
                                
                                // RGB = 00BBFF, RCT = 459D44, YCoCgR = 15D|01|BC
                                
                                uint8_t Green                   = Luma - FloorD((Cb + Cr) / 4); // 69 - 56 = 13
                                uint8_t Red                     = Cr   + Green;
                                uint8_t Blue                    = Cb   + Green;
                                
                                Array[View][W][H][LumaIndex]    = Red;
                                Array[View][W][H][Chroma1Index] = Green;
                                Array[View][W][H][Chroma2Index] = Blue;
                            }
                        }
                    }
                } else if (BitDepth <= 16) {
                    uint16_t ****Array  = (uint16_t****) ImageContainer_GetArray(Image);
                    
                    for (uint8_t View = 0; View < NumViews; View++) {
                        for (uint64_t W = 0; W < Width; W++) {
                            for (uint64_t H = 0; H < Height; H++) {
                                uint16_t Luma                   = Array[View][W][H][LumaIndex];
                                uint16_t Cr                     = Array[View][W][H][Chroma1Index];
                                uint16_t Cb                     = Array[View][W][H][Chroma2Index];
                                
                                uint16_t Green                  = Luma - FloorD((Cb + Cr) / 4);
                                uint16_t Red                    = Cr   + Green;
                                uint16_t Blue                   = Cb   + Green;
                                
                                Array[View][W][H][LumaIndex]    = Red;
                                Array[View][W][H][Chroma1Index] = Green;
                                Array[View][W][H][Chroma2Index] = Blue;
                            }
                        }
                    }
                }
                
                ImageChannelMap *NewMap                       = ImageChannelMap_Init(NumViews, NumChannels);
                ImageChannelMap_AddMask(NewMap, Chroma1Index, ImageMask_Red);
                ImageChannelMap_AddMask(NewMap, LumaIndex, ImageMask_Green);
                ImageChannelMap_AddMask(NewMap, Chroma2Index, ImageMask_Blue);
                ImageContainer_SetChannelMap(Image, NewMap);
                /*
                 } else {
                 Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("Unsupported Channel Mask %u"), Mask);
                 }
                 */
            }
        } else {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("ImageContainer Pointer is NULL"));
        }
    }
    
    void ReverseTransform_YCoCgR(ImageContainer *Image) {
        if (Image != NULL) {
            ImageChannelMap *Map  = ImageContainer_GetChannelMap(Image);
            uint8_t  NumChannels  = ImageChannelMap_GetNumChannels(Map);
            if (NumChannels < 2 || NumChannels > 4) {
                Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("NumChannels %u must be 3 or 4"), NumChannels);
            } else {
                //Image_ChannelMask RGBMask                         = ImageMask_Red | ImageMask_Green | ImageMask_Blue;
                //Image_ChannelMask RGBAMask                        = ImageMask_Red | ImageMask_Green | ImageMask_Blue | ImageMask_Alpha;
                
                //if (Mask == RGBMask || Mask == RGBAMask) {
                uint8_t  Chroma1Index                         = ImageChannelMap_GetChannelsIndex(Map, ImageMask_Chroma1);
                uint8_t  LumaIndex                            = ImageChannelMap_GetChannelsIndex(Map, ImageMask_Luma);
                uint8_t  Chroma2Index                         = ImageChannelMap_GetChannelsIndex(Map, ImageMask_Chroma2);
                
                uint8_t  NumViews                             = ImageChannelMap_GetNumViews(Map);
                uint8_t  BitDepth                             = ImageContainer_GetBitDepth(Image);
                uint64_t Width                                = ImageContainer_GetWidth(Image);
                uint64_t Height                               = ImageContainer_GetHeight(Image);
                
                if (BitDepth <= 8) {
                    uint8_t ****Array                         = (uint8_t****) ImageContainer_GetArray(Image);
                    
                    for (uint8_t View = 0; View < NumViews; View++) {
                        for (uint64_t W = 0; W < Width; W++) {
                            for (uint64_t H = 0; H < Height; H++) {
                                uint8_t Luma                    = Array[View][W][H][LumaIndex];
                                uint8_t Co                      = Array[View][W][H][Chroma1Index];
                                uint8_t Cg                      = Array[View][W][H][Chroma2Index];
                                
                                uint8_t Temp                    = Luma - (Cg >> 1);
                                uint8_t Green                   = Cg + Temp;
                                uint8_t Blue                    = Temp - (Co >> 1);
                                uint8_t Red                     = Blue + Co;
                                
                                Array[View][W][H][LumaIndex]    = Red;
                                Array[View][W][H][Chroma1Index] = Green;
                                Array[View][W][H][Chroma2Index] = Blue;
                            }
                        }
                    }
                } else if (BitDepth <= 16) {
                    uint16_t ****Array  = (uint16_t****) ImageContainer_GetArray(Image);
                    
                    for (uint8_t View = 0; View < NumViews; View++) {
                        for (uint64_t W = 0; W < Width; W++) {
                            for (uint64_t H = 0; H < Height; H++) {
                                uint16_t Luma                   = Array[View][W][H][LumaIndex];
                                uint16_t Co                     = Array[View][W][H][Chroma1Index];
                                uint16_t Cg                     = Array[View][W][H][Chroma2Index];
                                
                                uint16_t Temp                   = Luma - (Cg >> 1);
                                uint16_t Green                  = Cg + Temp;
                                uint16_t Blue                   = Temp - (Co >> 1);
                                uint16_t Red                    = Blue + Co;
                                
                                Array[View][W][H][LumaIndex]    = Red;
                                Array[View][W][H][Chroma1Index] = Green;
                                Array[View][W][H][Chroma2Index] = Blue;
                            }
                        }
                    }
                }
                ImageChannelMap *NewMap                             = ImageChannelMap_Init(NumViews, NumChannels);
                ImageChannelMap_AddMask(NewMap, Chroma1Index, ImageMask_Red);
                ImageChannelMap_AddMask(NewMap, LumaIndex, ImageMask_Green);
                ImageChannelMap_AddMask(NewMap, Chroma2Index, ImageMask_Blue);
                ImageContainer_SetChannelMap(Image, NewMap);
                /*
                 } else {
                 Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("Unsupported Channel Mask %u"), Mask);
                 }
                 */
            }
        } else {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("ImageContainer Pointer is NULL"));
        }
    }
    
    static void RegisterTransform_ReverseRCT(OVIA *Ovia) {
        Ovia->NumForwardTransforms                                += 1;
        uint64_t TransformIndex                                    = Ovia->NumForwardTransforms;
        Ovia->ForwardTransforms                                    = realloc(Ovia->ForwardTransforms, sizeof(OVIAColorTransform) * Ovia->NumForwardTransforms);
        
        Ovia->ForwardTransforms[TransformIndex].Transform          = ColorTransform_RCT;
        Ovia->ForwardTransforms[TransformIndex].InputChannels      = (ImageMask_Red | ImageMask_Green | ImageMask_Blue);
        Ovia->ForwardTransforms[TransformIndex].OutputChannels     = (ImageMask_Luma | ImageMask_Chroma1 | ImageMask_Chroma2);
        Ovia->ForwardTransforms[TransformIndex].NumInputChannels   = 3;
        Ovia->ForwardTransforms[TransformIndex].NumOutputChannels  = 3;
        Ovia->ForwardTransforms[TransformIndex].Function_Transform = ReverseTransform_RCT;
    }
    
    static void RegisterTransform_ReverseYCoCgR(OVIA *Ovia) {
        Ovia->NumForwardTransforms                                += 1;
        uint64_t TransformIndex                                    = Ovia->NumForwardTransforms;
        Ovia->ForwardTransforms                                    = realloc(Ovia->ForwardTransforms, sizeof(OVIAColorTransform) * Ovia->NumForwardTransforms);
        
        Ovia->ForwardTransforms[TransformIndex].Transform          = ColorTransform_YCoCgR;
        Ovia->ForwardTransforms[TransformIndex].InputChannels      = (ImageMask_Red | ImageMask_Green | ImageMask_Blue);
        Ovia->ForwardTransforms[TransformIndex].OutputChannels     = (ImageMask_Luma | ImageMask_Chroma1 | ImageMask_Chroma2);
        Ovia->ForwardTransforms[TransformIndex].NumInputChannels   = 3;
        Ovia->ForwardTransforms[TransformIndex].NumOutputChannels  = 3;
        Ovia->ForwardTransforms[TransformIndex].Function_Transform = ReverseTransform_YCoCgR;
    }
    
    static OVIACodecRegistry Register_ReverseTransformRCT = {
        .Function_ForwardTransform[ColorTransform_RCT - 1]         = &RegisterTransform_ReverseRCT,
    };
    
    static OVIACodecRegistry Register_ReverseTransformYCoCgR = {
        .Function_ForwardTransform[ColorTransform_YCoCgR - 1]      = &RegisterTransform_ReverseYCoCgR,
    };
    
#ifdef __cplusplus
}
#endif

