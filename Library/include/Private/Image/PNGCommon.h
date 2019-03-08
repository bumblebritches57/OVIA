#include "../../libOVIA.h"
#include "../InternalOVIA.h"

#pragma once

#ifndef OVIA_PNGCommon_H
#define OVIA_PNGCommon_H

#ifdef __cplusplus
extern "C" {
#endif
    
    /*!
     @abstract                  "Encodes a PNG from ImageContainer to a BitBuffer"
     @param     Image           "ImageContainer with the image to encode".
     @param     BitB            "The BitBuffer to contain the encoded png".
     @param     OptimizePNG     "Should this PNG file be optimized by trying all filters? (Huffman optimization is enabled by default)"
     */
    void        OVIA_PNG_Image_Insert(ImageContainer *Image, BitBuffer *BitB, bool OptimizePNG);
    
    void        OVIA_PNG_SetTextChunk(OVIA *Ovia, UTF8 *KeywordString, UTF8 *CommentString);
    
    uint32_t    OVIA_PNG_GetNumTextChunks(OVIA *Ovia);
    
    /*!
     @abstract                  "Extracts the Keyword and Comment strings from the Instance of the text chunk".
     @remark                    "If the Keyword or Comment is DEFLATE encoded, we OVIA it to a regular string".
     @param     Ovia             "OVIA Pointer to extract the text chunk from".
     @param     Instance        "Which instance of the text chunk should we extract"?
     @param     Keyword         "Pointer the Keyword string is returned through".
     @return                    "Returns the actual Comment string".
     */
    UTF8       *OVIA_PNG_GetTextChunk(OVIA *Ovia, uint32_t Instance, UTF8 *Keyword);
    
    uint32_t    OVIA_PNG_GetWidth(OVIA *Ovia);
    
    uint32_t    OVIA_PNG_GetHeight(OVIA *Ovia);
    
    uint8_t     OVIA_PNG_GetBitDepth(OVIA *Ovia);
    
    uint8_t     OVIA_PNG_GetColorType(OVIA *Ovia);
    
    bool        OVIA_PNG_GetInterlaceStatus(OVIA *Ovia);
    
    bool        OVIA_PNG_GetStereoscopicStatus(OVIA *Ovia);
    
    uint32_t    OVIA_PNG_GetWhitepointX(OVIA *Ovia);
    
    uint32_t    OVIA_PNG_GetWhitepointY(OVIA *Ovia);
    
    uint32_t    OVIA_PNG_GetGamma(OVIA *Ovia);
    
    UTF8       *OVIA_PNG_GetColorProfileName(OVIA *Ovia);
    
    uint8_t    *OVIA_PNG_GetColorProfile(OVIA *Ovia);
    
    /*
     @param     GammaCorrect only does anything if there is a GAMA chunk present.
     */
    ImageContainer *PNGExtractImage(OVIA *Ovia, BitBuffer *BitB);
    
    typedef struct HuffmanTable HuffmanTable;
    
    uint8_t             PaethPredictor(int64_t Left, int64_t Above, int64_t UpperLeft);
    
    void                OVIA_PNG_ParseChunks(OVIA *Ovia, BitBuffer *BitB);
    
    void                OVIA_PNG_Filter_Sub(OVIA *Ovia, ImageContainer *Image);
    
    void                OVIA_PNG_Filter_Up(OVIA *Ovia, ImageContainer *Image);
    
    void                OVIA_PNG_Filter_Average(OVIA *Ovia, ImageContainer *Image);
    
    void                OVIA_PNG_Filter_Paeth(OVIA *Ovia, ImageContainer *Image);
    
    void                WriteIHDRChunk(OVIA *Ovia, BitBuffer *BitB);
    
    void                WriteACTLChunk(OVIA *Ovia, BitBuffer *BitB);
    
    void                WriteFCTLChunk(OVIA *Ovia, BitBuffer *BitB);
    
    void                WriteFDATChunk(OVIA *Ovia, BitBuffer *BitB, uint8_t *DeflatedFrameData, uint32_t DeflatedFrameDataSize);
    
    void                WriteSTERChunk(OVIA *Ovia, BitBuffer *BitB);
    
    void                WriteBKGDChunk(OVIA *Ovia, BitBuffer *BitB);
    
    void                WriteCHRMChunk(OVIA *Ovia, BitBuffer *BitB);
    
    void                WriteGAMAChunk(OVIA *Ovia, BitBuffer *BitB);
    
    void                WriteOFFSChunk(OVIA *Ovia, BitBuffer *BitB);
    
    void                WriteICCPChunk(OVIA *Ovia, BitBuffer *BitB);
    
    void                WriteSBITChunk(OVIA *Ovia, BitBuffer *BitB);
    
    void                WriteSRGBChunk(OVIA *Ovia, BitBuffer *BitB);
    
    void                WritePHYSChunk(OVIA *Ovia, BitBuffer *BitB);
    
    void                WritePCALChunk(OVIA *Ovia, BitBuffer *BitB);
    
    void                WriteSCALChunk(OVIA *Ovia, BitBuffer *BitB);
    
    void                PNGEncodeFilterPaeth(OVIA *Ovia, ImageContainer *Image);
    
    void                PNGEncodeFilterSub(OVIA *Ovia, ImageContainer *Image);
    
    void                PNGEncodeAdam7(OVIA *Ovia, BitBuffer *ProgressiveImage, BitBuffer *InterlacedImage);
    
    void                OptimizeAdam7(OVIA *Ovia, uint8_t *Image);
    
    /*!
     @abstract       "Optimizes the image, by trying the 5 filters on each line, and keeping the best."
     */
    void                OptimizePNG(OVIA *Ovia, uint8_t *Image);
    
    void                PNGEncodeImage(OVIA *Ovia, BitBuffer *BitB);
    
    static const uint8_t Adam7Level1[1] = {
        0
    };
    
    static const uint8_t Adam7Level2[1] = {
        5
    };
    
    static const uint8_t Adam7Level3[2] = {
        32, 36
    };
    
    static const uint8_t Adam7Level4[4] = {
        3, 7, 35, 39
    };
    
    static const uint8_t Adam7Level5[8] = {
        17, 19, 21, 23, 49, 51, 53, 55
    };
    
    static const uint8_t Adam7Level6[16] = {
        2,  4,  6,  8, 18, 20, 22, 24,
        34, 36, 38, 40, 50, 52, 54, 56,
    };
    
    static const uint8_t Adam7Level7[32] = {
        9, 10, 11, 12, 13, 14, 15, 16,
        25, 26, 27, 28, 29, 30, 31, 32,
        41, 42, 43, 44, 45, 46, 47, 48,
        57, 58, 59, 60, 61, 62, 63, 64
    };
    
    typedef enum OVIA_PNG_Interlace_Types {
        PNGNotInterlaced   = 0,
        PNGInterlacedAdam7 = 1,
    } OVIA_PNG_Interlace_Types;
    
    typedef enum OVIA_PNG_Filter_Types {
        NotFiltered   = 0,
        SubFilter     = 1,
        UpFilter      = 2,
        AverageFilter = 3,
        PaethFilter   = 4,
    } OVIA_PNG_Filter_Types;
    
#ifdef __cplusplus
}
#endif

#endif /* OVIA_PNGCommon_H */
