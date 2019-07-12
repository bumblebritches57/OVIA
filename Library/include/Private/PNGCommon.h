#include "OVIACommon.h"

#pragma once

#ifndef OVIA_PNGCommon_H
#define OVIA_PNGCommon_H

#ifdef __cplusplus
extern "C" {
#endif
    
    typedef enum FlateConstants {
        MaxCodes                   = 288,
        MaxLiteralLengthCodes      = 286,
        MaxDistanceCodes           = 30,
        NumMetaCodes               = 19,
        MaxBitsPerSymbol           = 15,
    } FlateConstants;
    
    typedef enum Flate_BlockTypes {
        BlockType_Literal = 0,
        BlockType_Fixed   = 1,
        BlockType_Dynamic = 2,
        BlockType_Invalid = 3,
    } Flate_BlockTypes;
    
    typedef struct HuffmanTree {
        uint64_t NumSymbols;
        uint64_t Symbol; // The binary encoded value read/written from/to the stream
        uint64_t Value; // the meaning of the ymbol
    } HuffmanTree;
    
    HuffmanTree *HuffmanTree_Init(void) {
        HuffmanTree *Tree = calloc(1, sizeof(HuffmanTree));
        return Tree;
    }
    
    void PNG_DAT_Decode(BitBuffer *BitB, ImageContainer *Image);
    
    HuffmanTree *HuffmanBuildTree(uint64_t NumSymbols, const uint16_t *Lengths);
    
    static const uint16_t LengthBase[29] = {
        3,   4,   5,   6,   7,  8,  9,  10,
        11,  13,  15,  17,  19, 23, 27,  31,
        35,  43,  51,  59,  67, 83, 99, 115,
        131, 163, 195, 227, 258
    };
    
    static const uint8_t LengthAdditionalBits[29] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 2, 2, 2, 2,
        3, 3, 3, 3, 4, 4, 4, 4,
        5, 5, 5, 5, 0
    };
    
    static const uint16_t DistanceBase[30] = {
        1,    2,    3,     4,     5,     7,    9,   13,
        17,   25,   33,    49,    65,    97,  129,  193,
        257,  385,  513,   769,  1025,  1537, 2049, 3073,
        4097, 6145, 8193, 12289, 16385, 24577
    };
    
    static const uint8_t DistanceAdditionalBits[30] = {
        0,   0,  0,  0,  1,  1,  2,  2,
        3,   3,  4,  4,  5,  5,  6,  6,
        7,   7,  8,  8,  9,  9, 10, 10,
        11, 11, 12, 12, 13, 13
    }; // So, Read 5 bits for the Distance code, look it up here.
    
    static const uint8_t MetaCodeLengthOrder[19] = {
        16, 17, 18, 0,  8, 7,  9, 6,
        10,  5, 11, 4, 12, 3, 13, 2,
        14,  1, 15
    };
    
    static const uint16_t FixedLiteralTable[288] = { // Used in HuffmanTree_makeFromLengths, becomes CodeLengths
        8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8,
        9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9,
        7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7,
        8, 8, 8, 8, 8, 8, 8, 8,
    };
    
    static const uint16_t FixedDistanceTable[32] = {
        5, 5, 5, 5, 5, 5, 5, 5,
        5, 5, 5, 5, 5, 5, 5, 5,
        5, 5, 5, 5, 5, 5, 5, 5,
        5, 5, 5, 5, 5, 5, 5, 5
    };
    
    typedef enum HuffmanTreeTypes {
        TreeType_Unknown = 0,
        TreeType_Fixed   = 1,
        TreeType_Dynamic = 2,
    } HuffmanTreeTypes;
    
    HuffmanTree *PNG_Flate_BuildTree(BitBuffer *BitB, HuffmanTreeTypes TreeType);
    
    uint64_t ReadHuffman(HuffmanTree *Tree, BitBuffer *BitB);
    
    uint16_t PNG_Flate_ReadSymbol(HuffmanTree *Tree, BitBuffer *BitB);
    
    typedef enum PNG_Interlace_Types {
        PNGNotInterlaced   = 0,
        PNGInterlacedAdam7 = 1,
    } PNG_Interlace_Types;
    
    typedef enum PNG_Filter_Types {
        NotFiltered        = 0,
        SubFilter          = 1,
        UpFilter           = 2,
        AverageFilter      = 3,
        PaethFilter        = 4,
    } PNG_Filter_Types;
    
    typedef enum PNG_ColorTypes {
        PNG_Grayscale      = 0,
        PNG_RGB            = 2,
        PNG_PalettedRGB    = 3,
        PNG_GrayAlpha      = 4,
        PNG_RGBA           = 6,
    } PNG_ColorTypes;
    
    typedef enum PNG_ChunkMarkers {
        acTLMarker         = 0x6163544C,
        bKGDMarker         = 0x626B4744,
        cHRMMarker         = 0x6348524D,
        fcTLMarker         = 0x6663544C,
        fDATMarker         = 0x66444154,
        gAMAMarker         = 0x67414D41,
        hISTMarker         = 0x68495354,
        iCCPMarker         = 0x69434350,
        IDATMarker         = 0x49444154,
        iHDRMarker         = 0x49484452,
        iTXtMarker         = 0x69545874,
        oFFsMarker         = 0x6F464673,
        pCALMarker         = 0x7043414C,
        pHYsMarker         = 0x70485973,
        PLTEMarker         = 0x504C5445,
        sBITMarker         = 0x73424954,
        sCALMarker         = 0x7343414c,
        sRGBMarker         = 0x73524742,
        sTERMarker         = 0x73544552,
        tEXtMarker         = 0x74455874,
        zTXtMarker         = 0x7A545874,
        tIMEMarker         = 0x74494d45,
        tRNSMarker         = 0x74524e53,
        sPLTMarker         = 0x73504c54,
    } PNG_ChunkMarkers;
    
    typedef enum PNGTextTypes {
        UnknownTextType = 0,
        tEXt            = 1,
        iTXt            = 2,
        zTXt            = 3,
    } PNGTextTypes;
    
    typedef struct iHDRChunk {
        uint32_t            Width;
        uint32_t            Height;
        uint8_t             BitDepth;
        uint8_t             Compression;
        uint8_t             FilterMethod;
        PNG_ColorTypes ColorType;
        bool                Progressive;
        bool                Interlaced;
    } iHDRChunk;
    
    typedef struct PLTEChunk {
        uint8_t  **Palette;
        uint8_t    NumEntries;
    } PLTEChunk;
    
    typedef struct tRNSChunk {
        uint8_t  **Palette;
        uint8_t    NumEntries;
    } tRNSChunk;
    
    typedef struct bkGDChunk {
        uint8_t    BackgroundPaletteEntry;
        uint16_t   Gray;
        uint16_t   Red;
        uint16_t   Green;
        uint16_t   Blue;
    } bkGDChunk;
    
    typedef struct sTERChunk {
        bool       StereoType:1;
    } sTERChunk;
    
    typedef struct cHRMChunk { // sRGB or iCCP overrides cHRM
        uint32_t   WhitePointX;
        uint32_t   WhitePointY;
        uint32_t   RedX;
        uint32_t   RedY;
        uint32_t   GreenX;
        uint32_t   GreenY;
        uint32_t   BlueX;
        uint32_t   BlueY;
    } cHRMChunk;
    
    typedef struct gAMAChunk { // sRGB or iCCP overrides gAMA
        uint32_t   Gamma;
    } gAMAChunk;
    
    typedef struct oFFsChunk {
        int32_t    XOffset;
        int32_t    YOffset;
        bool       UnitSpecifier:1;
    } oFFsChunk;
    
    typedef struct iCCPChunk {
        UTF8      *ProfileName;
        uint8_t   *CompressedICCPProfile;
        uint64_t   CompressedICCPProfileSize;
        uint8_t    CompressionType;
    } iCCPChunk;
    
    typedef struct sBITChunk {
        uint8_t    Grayscale;
        uint8_t    Red;
        uint8_t    Green;
        uint8_t    Blue;
        uint8_t    Alpha;
    } sBITChunk;
    
    typedef struct sRGBChunk {
        uint8_t    RenderingIntent;
    } sRGBChunk;
    
    typedef struct pHYsChunk {
        uint32_t   PixelsPerUnitXAxis;
        uint32_t   PixelsPerUnitYAxis;
        uint8_t    UnitSpecifier;
    } pHYsChunk;
    
    typedef struct sPLTChunk {
        uint16_t  *Red;
        uint16_t  *Green;
        uint16_t  *Blue;
        uint16_t  *Alpha;
        uint16_t  *RelativeFrequency;
        char      *Name;
        uint8_t    SampleDepth;
    } sPLTChunk;
    
    typedef struct pCALChunk {
        UTF8      *CalibrationName;
        UTF8      *UnitName;
        uint8_t    NumParams;
        int32_t    OriginalZero;
        int32_t    OriginalMax;
        uint8_t    EquationType;
    } pCALChunk;
    
    typedef struct sCALChunk {
        double     PixelWidth; // ASCII float
        double     PixelHeight; // ASCII float
        uint8_t    UnitSpecifier;
    } sCALChunk;
    
    typedef struct hISTChunk {
        uint32_t   NumColors;
        uint16_t  *Histogram; // For each PLTE entry, there needs to be 1 array element
    } hISTChunk;
    
    typedef struct TextChunk { // Replaces:  tEXt, iTXt, zTXt
        UTF8         *Keyword;
        UTF8         *Comment;
        PNGTextTypes  TextType;
    } TextChunk;
    
    typedef struct tIMeChunk {
        uint16_t   Year;
        uint8_t    Month:4;
        uint8_t    Day:5;
        uint8_t    Hour:5;
        uint8_t    Minute:6;
        uint8_t    Second:6;
    } tIMeChunk;
    
    typedef struct DATChunk {
        uint64_t     ImageSize;
        uint64_t     ImageOffset;
        uint32_t     DictID;
        uint32_t     Size;
        uint16_t     LengthLiteralTreeSize;
        uint8_t      DistanceTreeSize;
        uint8_t      CMF;
        uint8_t      FLG;
    } DATChunk;
    
    typedef struct acTLChunk {
        uint32_t   NumFrames;
        uint32_t   TimesToLoop;
    } acTLChunk;
    
    typedef struct fcTLChunk {
        uint32_t   FrameNum;
        uint32_t   Width;
        uint32_t   Height;
        uint32_t   XOffset;
        uint32_t   YOffset;
        uint16_t   FrameDelayNumerator;
        uint16_t   FrameDelayDenominator;
        uint8_t    DisposeMethod;
        bool       BlendMethod:1;
    } fcTLChunk;
    
    typedef struct fdATChunk {
        uint32_t   FrameNum;
    } fdATChunk;
    
    typedef struct iENDChunk {
        uint32_t  Size;
    } iENDChunk;
    
    typedef struct PNGOptions {
        sPLTChunk   **sPLT; // May be multiple
        iHDRChunk    *iHDR;
        cHRMChunk    *cHRM;
        gAMAChunk    *gAMA;
        iCCPChunk    *iCCP;
        sBITChunk    *sBIT;
        sRGBChunk    *sRGB;
        PLTEChunk    *PLTE;
        bkGDChunk    *bkGD;
        hISTChunk    *hIST;
        tRNSChunk    *tRNS;
        pHYsChunk    *pHYs;
        oFFsChunk    *oFFs; // Extension, before first iDAT
        sCALChunk    *sCAL; // Extension, before first iDAT
        pCALChunk    *pCAL; // Extension, after PLTE, before iDAT
        tIMeChunk    *tIMe; // Order doesn't matter
        TextChunk    *Text; // Order doesn't matter
        DATChunk     *DAT;
        /* 3D */
        sTERChunk    *sTER;
        /* 3D */
        /* APNG */
        acTLChunk    *acTL;
        fcTLChunk    *fcTL;
        fdATChunk    *fdAT;
        /* APNG */
        bool          IsAnimated;
    } PNGOptions;
    
    void *PNGOptions_Init(void);
    
    void PNG_Flate_ReadZlibHeader(PNGOptions *PNG, BitBuffer *BitB);
    
    void PNG_Flate_ReadLiteralBlock(PNGOptions *PNG, BitBuffer *BitB);
    
    /*!
     @abstract                  "Encodes a PNG from ImageContainer to a BitBuffer"
     @param     Image           "ImageContainer with the image to encode".
     @param     BitB            "The BitBuffer to contain the encoded png".
     @param     OptimizePNG     "Should this PNG file be optimized by trying all filters? (Huffman optimization is enabled by default)"
     */
    void        PNG_Image_Insert(ImageContainer *Image, BitBuffer *BitB, bool OptimizePNG);
    
    void        PNG_SetTextChunk(UTF8 *KeywordString, UTF8 *CommentString);
    
    uint32_t    PNG_GetNumTextChunks(PNGOptions *PNG);
    
    /*!
     @abstract                  "Extracts the Keyword and Comment strings from the Instance of the text chunk".
     @remark                    "If the Keyword or Comment is DEFLATE encoded, we OVIA it to a regular string".
     @param     Ovia             "OVIA Pointer to extract the text chunk from".
     @param     Instance        "Which instance of the text chunk should we extract"?
     @param     Keyword         "Pointer the Keyword string is returned through".
     @return                    "Returns the actual Comment string".
     */
    UTF8       *PNG_GetTextChunk(uint32_t Instance, UTF8 *Keyword);
    
    uint32_t    PNG_GetWidth(PNGOptions *PNG);
    
    uint32_t    PNG_GetHeight(PNGOptions *PNG);
    
    uint8_t     PNG_GetBitDepth(PNGOptions *PNG);
    
    uint8_t     PNG_GetColorType(PNGOptions *PNG);
    
    bool        PNG_GetInterlaceStatus(PNGOptions *PNG);
    
    bool        PNG_GetStereoscopicStatus(PNGOptions *PNG);
    
    uint32_t    PNG_GetWhitepointX(PNGOptions *PNG);
    
    uint32_t    PNG_GetWhitepointY(PNGOptions *PNG);
    
    uint32_t    PNG_GetGamma(PNGOptions *PNG);
    
    UTF8       *PNG_GetColorProfileName(PNGOptions *PNG);
    
    uint8_t    *PNG_GetColorProfile(PNGOptions *PNG);
    
    /*
     @param     GammaCorrect only does anything if there is a GAMA chunk present.
     */
    ImageContainer *PNGExtractImage(PNGOptions *PNG, BitBuffer *BitB);
    
    typedef struct HuffmanTree HuffmanTree;
    
    uint8_t             PaethPredictor(int64_t Left, int64_t Above, int64_t UpperLeft);
    
    void                PNG_ParseChunks(PNGOptions *PNG, BitBuffer *BitB);
    
    void                PNG_Filter_Sub(ImageContainer *Image);
    
    void                PNG_Filter_Up(ImageContainer *Image);
    
    void                PNG_Filter_Average(ImageContainer *Image);
    
    void                PNG_Filter_Paeth(ImageContainer *Image);
    
    void                WriteIHDRChunk(PNGOptions *PNG, BitBuffer *BitB);
    
    void                WriteACTLChunk(PNGOptions *PNG, BitBuffer *BitB);
    
    void                WriteFCTLChunk(PNGOptions *PNG, BitBuffer *BitB);
    
    void                WriteFDATChunk(BitBuffer *BitB, uint8_t *DeflatedFrameData, uint32_t DeflatedFrameDataSize);
    
    void                WriteSTERChunk(PNGOptions *PNG, BitBuffer *BitB);
    
    void                WriteBKGDChunk(PNGOptions *PNG, BitBuffer *BitB);
    
    void                WriteCHRMChunk(PNGOptions *PNG, BitBuffer *BitB);
    
    void                WriteGAMAChunk(PNGOptions *PNG, BitBuffer *BitB);
    
    void                WriteOFFSChunk(PNGOptions *PNG, BitBuffer *BitB);
    
    void                WriteICCPChunk(PNGOptions *PNG, BitBuffer *BitB);
    
    void                WriteSBITChunk(PNGOptions *PNG, BitBuffer *BitB);
    
    void                WriteSRGBChunk(PNGOptions *PNG, BitBuffer *BitB);
    
    void                WritePHYSChunk(PNGOptions *PNG, BitBuffer *BitB);
    
    void                WritePCALChunk(PNGOptions *PNG, BitBuffer *BitB);
    
    void                WriteSCALChunk(PNGOptions *PNG, BitBuffer *BitB);
    
    void                PNGEncodeFilterPaeth(ImageContainer *Image);
    
    void                PNGEncodeFilterSub(ImageContainer *Image);
    
    void                PNGEncodeAdam7(BitBuffer *ProgressiveImage, BitBuffer *InterlacedImage);
    
    void                OptimizeAdam7(uint8_t *Image);
    
    /*!
     @abstract       "Optimizes the image, by trying the 5 filters on each line, and keeping the best."
     */
    void                OptimizePNG(uint8_t *Image);
    
    void                PNGEncodeImage(PNGOptions *PNG, BitBuffer *BitB);
    
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
    
    void PNGOptions_Deinit(PNGOptions *PNG);
    
#ifdef __cplusplus
}
#endif

#endif /* OVIA_PNGCommon_H */
