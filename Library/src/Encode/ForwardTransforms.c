#include "../../include/Private/OVIACommon.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    void ForwardTransform_RCT(ImageContainer *Image) {
        if (Image != NULL) {
            ImageChannelMap *Map  = ImageContainer_GetChannelMap(Image);
            uint8_t NumChannels  = ImageChannelMap_GetNumChannels(Map);
            if (NumChannels < 2 || NumChannels > 4) {
                Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("NumChannels %u must be 3 or 4"), NumChannels);
            } else {
                //ImageChannelMask RGBMask                         = ImageMask_Red | ImageMask_Green | ImageMask_Blue;
                //ImageChannelMask RGBAMask                        = ImageMask_Red | ImageMask_Green | ImageMask_Blue | ImageMask_Alpha;
                
                //if (Mask == RGBMask || Mask == RGBAMask) {
                uint8_t  RedIndex                             = ImageChannelMap_GetChannelsIndex(Image, ImageMask_Red);
                uint8_t  GreenIndex                           = ImageChannelMap_GetChannelsIndex(Image, ImageMask_Green);
                uint8_t  BlueIndex                            = ImageChannelMap_GetChannelsIndex(Image, ImageMask_Blue);
                
                uint8_t  NumViews                             = ImageChannelMap_GetNumViews(Map);
                uint8_t  BitDepth                             = ImageContainer_GetBitDepth(Image);
                uint64_t Width                                = ImageContainer_GetWidth(Image);
                uint64_t Height                               = ImageContainer_GetHeight(Image);
                
                if (BitDepth <= 8) {
                    uint8_t ****Array                         = (uint8_t****) ImageContainer_GetArray(Image);
                    
                    for (uint8_t View = 0; View < NumViews; View++) {
                        for (uint64_t W = 0; W < Width; W++) {
                            for (uint64_t H = 0; H < Height; H++) {
                                uint8_t Red                   = Array[View][W][H][RedIndex];
                                uint8_t Green                 = Array[View][W][H][GreenIndex];
                                uint8_t Blue                  = Array[View][W][H][BlueIndex];
                                
                                // RGB = 00BBFF, RCT = 459D44, YCoCgR =
                                
                                uint8_t Cr                    = Red - Green;
                                uint8_t Y                     = FloorD((Red + (Green * 2) + Blue) / 4);
                                uint8_t Cb                    = Blue - Green;
                                
                                Array[View][W][H][RedIndex]   = Cr;
                                Array[View][W][H][GreenIndex] = Y;
                                Array[View][W][H][BlueIndex]  = Cb;
                            }
                        }
                    }
                } else if (BitDepth <= 16) {
                    uint16_t ****Array  = (uint16_t****) ImageContainer_GetArray(Image);
                    
                    for (uint8_t View = 0; View < NumViews; View++) {
                        for (uint64_t W = 0; W < Width; W++) {
                            for (uint64_t H = 0; H < Height; H++) {
                                uint16_t Red                  = Array[View][W][H][RedIndex];
                                uint16_t Green                = Array[View][W][H][GreenIndex];
                                uint16_t Blue                 = Array[View][W][H][BlueIndex];
                                
                                uint16_t Cr                   = Red - Green;
                                uint16_t Y                    = (Red + (Green * 2) + Blue) / 4;
                                uint16_t Cb                   = Blue - Green;
                                
                                Array[View][W][H][RedIndex]   = Cr;
                                Array[View][W][H][GreenIndex] = Y;
                                Array[View][W][H][BlueIndex]  = Cb;
                            }
                        }
                    }
                }
                
                ImageChannelMap *NewMap                       = ImageChannelMap_Init(NumViews, NumChannels);
                ImageChannelMap_AddMask(NewMap, RedIndex, ImageMask_Chroma1);
                ImageChannelMap_AddMask(NewMap, GreenIndex, ImageMask_Luma);
                ImageChannelMap_AddMask(NewMap, BlueIndex, ImageMask_Chroma2);
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
    
    void ForwardTransform_YCoCgR(ImageContainer *Image) {
        if (Image != NULL) {
            ImageChannelMap *Map  = ImageContainer_GetChannelMap(Image);
            uint8_t  NumChannels  = ImageChannelMap_GetNumChannels(Map);
            
            if (NumChannels >= 3 && NumChannels <= 4) {
                /*
                 
                 How do we make sure that only Red Green and Blue are present
                 
                 */
                //if (Mask == RGBMask || Mask == RGBAMask) {
                uint8_t  RedIndex                             = ImageChannelMap_GetChannelsIndex(Image, ImageMask_Red);
                uint8_t  GreenIndex                           = ImageChannelMap_GetChannelsIndex(Image, ImageMask_Green);
                uint8_t  BlueIndex                            = ImageChannelMap_GetChannelsIndex(Image, ImageMask_Blue);
                
                uint8_t  NumViews                             = ImageChannelMap_GetNumViews(Map);
                uint8_t  BitDepth                             = ImageContainer_GetBitDepth(Image);
                uint64_t Width                                = ImageContainer_GetWidth(Image);
                uint64_t Height                               = ImageContainer_GetHeight(Image);
                
                if (BitDepth <= 8) {
                    uint8_t ****Array                         = (uint8_t****) ImageContainer_GetArray(Image);
                    
                    for (uint8_t View = 0; View < NumViews; View++) {
                        for (uint64_t W = 0; W < Width; W++) {
                            for (uint64_t H = 0; H < Height; H++) {
                                uint8_t Red                   = Array[View][W][H][RedIndex];
                                uint8_t Green                 = Array[View][W][H][GreenIndex];
                                uint8_t Blue                  = Array[View][W][H][BlueIndex];
                                
                                uint8_t Co                    = Red   - Blue;
                                uint8_t Temp                  = Blue  + (Co >> 1);
                                uint8_t Cg                    = Green - Temp;
                                uint8_t Y                     = Temp  + (Cg >> 1);
                                
                                Array[View][W][H][RedIndex]   = Co;
                                Array[View][W][H][GreenIndex] = Y;
                                Array[View][W][H][BlueIndex]  = Cg;
                            }
                        }
                    }
                } else if (BitDepth <= 16) {
                    uint16_t ****Array  = (uint16_t****) ImageContainer_GetArray(Image);
                    
                    for (uint8_t View = 0; View < NumViews; View++) {
                        for (uint64_t W = 0; W < Width; W++) {
                            for (uint64_t H = 0; H < Height; H++) {
                                uint16_t Red                  = Array[View][W][H][RedIndex];
                                uint16_t Green                = Array[View][W][H][GreenIndex];
                                uint16_t Blue                 = Array[View][W][H][BlueIndex];
                                
                                uint16_t Co                   = Red   - Blue;
                                uint16_t Temp                 = Blue  + (Co >> 1);
                                uint16_t Cg                   = Green - Temp;
                                uint16_t Y                    = Temp  + (Cg >> 1);
                                
                                Array[View][W][H][RedIndex]   = Co;
                                Array[View][W][H][GreenIndex] = Y;
                                Array[View][W][H][BlueIndex]  = Cg;
                            }
                        }
                    }
                }
                
                ImageChannelMap *NewMap                       = ImageChannelMap_Init(NumViews, NumChannels);
                ImageChannelMap_AddMask(NewMap, RedIndex, ImageMask_Chroma1);
                ImageChannelMap_AddMask(NewMap, GreenIndex, ImageMask_Luma);
                ImageChannelMap_AddMask(NewMap, BlueIndex, ImageMask_Chroma2);
                ImageContainer_SetChannelMap(Image, NewMap);
                /*
                 } else {
                 Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("Unsupported Channel Mask %u"), Mask);
                 }
                 */
            } else {
                Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("NumChannels %hhu must be 3 or 4"), NumChannels);
            }
        } else {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String("ImageContainer Pointer is NULL"));
        }
    }
    
    static void RegisterTransform_ForwardRCT(OVIA *Ovia) {
        Ovia->NumForwardTransforms                                += 1;
        uint64_t TransformIndex                                    = Ovia->NumForwardTransforms;
        Ovia->ForwardTransforms                                    = realloc(Ovia->ForwardTransforms, sizeof(OVIAColorTransform) * Ovia->NumForwardTransforms);
        
        Ovia->ForwardTransforms[TransformIndex].Transform          = ColorTransform_RCT;
        Ovia->ForwardTransforms[TransformIndex].InputChannels      = (ImageMask_Red | ImageMask_Green | ImageMask_Blue);
        Ovia->ForwardTransforms[TransformIndex].OutputChannels     = (ImageMask_Luma | ImageMask_Chroma1 | ImageMask_Chroma2);
        Ovia->ForwardTransforms[TransformIndex].NumInputChannels   = 3;
        Ovia->ForwardTransforms[TransformIndex].NumOutputChannels  = 3;
        Ovia->ForwardTransforms[TransformIndex].Function_Transform = ForwardTransform_RCT;
    }
    
    static void RegisterTransform_ForwardYCoCgR(OVIA *Ovia) {
        Ovia->NumForwardTransforms                                += 1;
        uint64_t TransformIndex                                    = Ovia->NumForwardTransforms;
        Ovia->ForwardTransforms                                    = realloc(Ovia->ForwardTransforms, sizeof(OVIAColorTransform) * Ovia->NumForwardTransforms);
        
        Ovia->ForwardTransforms[TransformIndex].Transform          = ColorTransform_YCoCgR;
        Ovia->ForwardTransforms[TransformIndex].InputChannels      = (ImageMask_Red | ImageMask_Green | ImageMask_Blue);
        Ovia->ForwardTransforms[TransformIndex].OutputChannels     = (ImageMask_Luma | ImageMask_Chroma1 | ImageMask_Chroma2);
        Ovia->ForwardTransforms[TransformIndex].NumInputChannels   = 3;
        Ovia->ForwardTransforms[TransformIndex].NumOutputChannels  = 3;
        Ovia->ForwardTransforms[TransformIndex].Function_Transform = ForwardTransform_YCoCgR;
    }
    
    static OVIACodecRegistry Register_ForwardTransformRCT = {
        .Function_ForwardTransform[ColorTransform_RCT - 1]         = &RegisterTransform_ForwardRCT,
    };
    
    static OVIACodecRegistry Register_ForwardTransformYCoCgR = {
        .Function_ForwardTransform[ColorTransform_YCoCgR - 1]      = &RegisterTransform_ForwardYCoCgR,
    };
    
#ifdef __cplusplus
}
#endif
