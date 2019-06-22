#include "../../../Dependencies/FoundationIO/libFoundationIO/include/BitIO.h"
#include "../../../Dependencies/FoundationIO/libFoundationIO/include/ContainerIO.h"
#include "../../../Dependencies/FoundationIO/libFoundationIO/include/CryptographyIO.h"
#include "../../../Dependencies/FoundationIO/libFoundationIO/include/Log.h"
#include "../../../Dependencies/FoundationIO/libFoundationIO/include/Math.h"

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
    
    typedef enum OVIA_Flate_BlockTypes {
        Flate_LiteralBlock = 0,
        Flate_FixedBlock   = 1,
        Flate_DynamicBlock = 2,
        Flate_InvalidBlock = 3
    } OVIA_Flate_BlockTypes;
    
    typedef struct HuffmanTable {
        uint16_t *Frequency; // Count
        uint16_t *Symbols;   // HuffmanCode
        uint16_t  NumSymbols;
    } HuffmanTable;
    
    typedef struct HuffmanTree {
        uint32_t  NumSymbols;
        uint32_t *HuffmanSymbol;
        uint8_t   Value;
        /*
        HuffmanTable *LengthTable;
        HuffmanTable *DistanceTable;
         */
    } HuffmanTree;
    
    HuffmanTree *HuffmanTree_Init() {
        HuffmanTree *Tree = calloc(1, sizeof(HuffmanTree));
        return Tree;
    }
    
    void HuffmanTree_AddLengthTable(HuffmanTree *Tree, HuffmanTable *LengthTable) {
        if (Tree != NULL) {
            Tree->LengthTable = LengthTable;
        }
    }
    
    void HuffmanTree_AddDistanceTable(HuffmanTree *Tree, HuffmanTable *DistanceTable) {
        if (Tree != NULL) {
            Tree->DistanceTable = DistanceTable;
        }
    }
    
    void HuffmanTree_Deinit(HuffmanTree *Tree) {
        if (Tree != NULL) {
            free(Tree->LengthTable->Frequency);
            free(Tree->LengthTable->Symbols);
            free(Tree->LengthTable);
            free(Tree->DistanceTable->Frequency);
            free(Tree->DistanceTable->Symbols);
            free(Tree->DistanceTable);
            free(Tree);
        }
    }
    
    void OVIA_PNG_DAT_Decode(BitBuffer *BitB, ImageContainer *Image);
    
    HuffmanTable *OVIA_PNG_Huffman_BuildTree(uint64_t NumSymbols, const uint16_t *Lengths);
    
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
    
    void OVIA_PNG_Flate_ReadZlibHeader(BitBuffer *BitB);
    
    void OVIA_PNG_Flate_ReadLiteralBlock(BitBuffer *BitB);
    
    HuffmanTree *OVIA_PNG_Flate_BuildTree(BitBuffer *BitB, HuffmanTreeTypes TreeType);
    
    uint64_t ReadHuffman(HuffmanTable *Tree, BitBuffer *BitB);
    
    uint16_t OVIA_PNG_Flate_ReadSymbol(HuffmanTable *Tree, BitBuffer *BitB);
    
    typedef enum OVIA_PNG_Interlace_Types {
        PNGNotInterlaced   = 0,
        PNGInterlacedAdam7 = 1,
    } OVIA_PNG_Interlace_Types;
    
    typedef enum OVIA_PNG_Filter_Types {
        NotFiltered        = 0,
        SubFilter          = 1,
        UpFilter           = 2,
        AverageFilter      = 3,
        PaethFilter        = 4,
    } OVIA_PNG_Filter_Types;
    
    typedef enum OVIA_PNG_ColorTypes {
        PNG_Grayscale      = 0,
        PNG_RGB            = 2,
        PNG_PalettedRGB    = 3,
        PNG_GrayAlpha      = 4,
        PNG_RGBA           = 6,
    } OVIA_PNG_ColorTypes;
    
    typedef enum OVIA_PNG_ChunkMarkers {
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
    } OVIA_PNG_ChunkMarkers;
    
#define PNGMagic 0x89504E470D0A1A0A
    
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
        OVIA_PNG_ColorTypes ColorType;
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
    
    /*!
     @abstract                  "Encodes a PNG from ImageContainer to a BitBuffer"
     @param     Image           "ImageContainer with the image to encode".
     @param     BitB            "The BitBuffer to contain the encoded png".
     @param     OptimizePNG     "Should this PNG file be optimized by trying all filters? (Huffman optimization is enabled by default)"
     */
    void        OVIA_PNG_Image_Insert(ImageContainer *Image, BitBuffer *BitB, bool OptimizePNG);
    
    void        OVIA_PNG_SetTextChunk(UTF8 *KeywordString, UTF8 *CommentString);
    
    uint32_t    OVIA_PNG_GetNumTextChunks(OVIA *Ovia);
    
    /*!
     @abstract                  "Extracts the Keyword and Comment strings from the Instance of the text chunk".
     @remark                    "If the Keyword or Comment is DEFLATE encoded, we OVIA it to a regular string".
     @param     Ovia             "OVIA Pointer to extract the text chunk from".
     @param     Instance        "Which instance of the text chunk should we extract"?
     @param     Keyword         "Pointer the Keyword string is returned through".
     @return                    "Returns the actual Comment string".
     */
    UTF8       *OVIA_PNG_GetTextChunk(uint32_t Instance, UTF8 *Keyword);
    
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
    ImageContainer *PNGExtractImage(BitBuffer *BitB);
    
    typedef struct HuffmanTable HuffmanTable;
    
    uint8_t             PaethPredictor(int64_t Left, int64_t Above, int64_t UpperLeft);
    
    void                OVIA_PNG_ParseChunks(BitBuffer *BitB);
    
    void                OVIA_PNG_Filter_Sub(ImageContainer *Image);
    
    void                OVIA_PNG_Filter_Up(ImageContainer *Image);
    
    void                OVIA_PNG_Filter_Average(ImageContainer *Image);
    
    void                OVIA_PNG_Filter_Paeth(ImageContainer *Image);
    
    void                WriteIHDRChunk(BitBuffer *BitB);
    
    void                WriteACTLChunk(BitBuffer *BitB);
    
    void                WriteFCTLChunk(BitBuffer *BitB);
    
    void                WriteFDATChunk(BitBuffer *BitB, uint8_t *DeflatedFrameData, uint32_t DeflatedFrameDataSize);
    
    void                WriteSTERChunk(BitBuffer *BitB);
    
    void                WriteBKGDChunk(BitBuffer *BitB);
    
    void                WriteCHRMChunk(BitBuffer *BitB);
    
    void                WriteGAMAChunk(BitBuffer *BitB);
    
    void                WriteOFFSChunk(BitBuffer *BitB);
    
    void                WriteICCPChunk(BitBuffer *BitB);
    
    void                WriteSBITChunk(BitBuffer *BitB);
    
    void                WriteSRGBChunk(BitBuffer *BitB);
    
    void                WritePHYSChunk(BitBuffer *BitB);
    
    void                WritePCALChunk(BitBuffer *BitB);
    
    void                WriteSCALChunk(BitBuffer *BitB);
    
    void                PNGEncodeFilterPaeth(ImageContainer *Image);
    
    void                PNGEncodeFilterSub(ImageContainer *Image);
    
    void                PNGEncodeAdam7(BitBuffer *ProgressiveImage, BitBuffer *InterlacedImage);
    
    void                OptimizeAdam7(uint8_t *Image);
    
    /*!
     @abstract       "Optimizes the image, by trying the 5 filters on each line, and keeping the best."
     */
    void                OptimizePNG(uint8_t *Image);
    
    void                PNGEncodeImage(BitBuffer *BitB);
    
    void                 OVIA_PNG_ACTL_SetACTL(uint32_t NumFrames, uint32_t Times2Loop);
    void                 OVIA_PNG_BKGD_SetBackgroundPaletteEntry(uint8_t PaletteEntry);
    void                 OVIA_PNG_CHRM_SetBlue(uint32_t BlueX, uint32_t BlueY);
    void                 OVIA_PNG_CHRM_SetGreen(uint32_t GreenX, uint32_t GreenY);
    void                 OVIA_PNG_CHRM_SetRed(uint32_t RedX, uint32_t RedY);
    void                 OVIA_PNG_CHRM_SetWhitePoint(uint32_t WhitePointX, uint32_t WhitePointY);
    void                 OVIA_PNG_DAT_SetArray(uint8_t *Array);
    void                 OVIA_PNG_DAT_SetArrayOffset(uint64_t ArrayOffset);
    void                 OVIA_PNG_DAT_SetArraySize(uint64_t ArraySize);
    void                 OVIA_PNG_DAT_SetCMF(uint8_t CMF);
    void                 OVIA_PNG_DAT_SetDictID(uint32_t DictID);
    void                 OVIA_PNG_DAT_SetDistanceHuffmanTable(HuffmanTable *DistanceTree);
    void                 OVIA_PNG_DAT_SetFCHECK(uint8_t FCHECK);
    void                 OVIA_PNG_DAT_SetFLG(uint8_t FLG);
    void                 OVIA_PNG_DAT_SetLengthLiteralHuffmanTable(HuffmanTable *LengthLiteralTree);
    void                 OVIA_PNG_FCTL_SetBlendMethod(uint8_t BlendMethod);
    void                 OVIA_PNG_FCTL_SetDisposeMethod(uint8_t DisposeMethod);
    void                 OVIA_PNG_FCTL_SetFCTL(const uint32_t FrameNum, const uint32_t Width, const uint32_t Height, uint32_t XOffset, uint32_t YOffset, uint16_t DelayNumerator, uint16_t DelayDenominator, uint8_t DisposalType, uint8_t BlendType);
    void                 OVIA_PNG_FCTL_SetFrameDelayDenominator(uint16_t FrameDelayDenominator);
    void                 OVIA_PNG_FCTL_SetFrameDelayNumerator(uint16_t FrameDelayNumerator);
    void                 OVIA_PNG_FCTL_SetFrameNum(uint32_t FrameNum);
    void                 OVIA_PNG_FCTL_SetHeight(uint32_t Height);
    void                 OVIA_PNG_FCTL_SetWidth(uint32_t Width);
    void                 OVIA_PNG_FCTL_SetXOffset(uint32_t XOffset);
    void                 OVIA_PNG_FCTL_SetYOffset(uint32_t YOffset);
    void                 OVIA_PNG_GAMA_SetGamma(uint32_t Gamma);
    void                 OVIA_PNG_HIST_SetNumEntries(uint32_t NumEntries);
    void                 OVIA_PNG_ICCP_SetCompressionType(uint8_t CompressionType);
    void                 OVIA_PNG_ICCP_SetProfileData(uint8_t *ProfileData);
    void                 OVIA_PNG_ICCP_SetProfileDataSize(uint64_t ProfileSize);
    void                 OVIA_PNG_ICCP_SetProfileName(UTF8 *ProfileName);
    void                 OVIA_PNG_IHDR_SetColorType(uint8_t ColorType);
    void                 OVIA_PNG_IHDR_SetIHDR(uint32_t Height, uint32_t Width, uint8_t BitDepth, uint8_t ColorType, const bool Interlace);
    void                 OVIA_PNG_OFFS_SetSpecifier(bool Specifier);
    void                 OVIA_PNG_OFFS_SetXOffset(int32_t XOffset);
    void                 OVIA_PNG_OFFS_SetYOffset(int32_t YOffset);
    void                 OVIA_PNG_PCAL_SetCalibrationName(UTF8 *CalibrationName);
    void                 OVIA_PNG_PHYS_SetPixelsPerUnitX(int32_t PixelsPerUnitX);
    void                 OVIA_PNG_PHYS_SetPixelsPerUnitY(int32_t PixelsPerUnitY);
    void                 OVIA_PNG_PHYS_SetUnitSpecifier(bool UnitSpecifier);
    void                 OVIA_PNG_PLTE_Init(uint64_t NumEntries);
    void                 OVIA_PNG_PLTE_SetPalette(uint64_t Entry, uint16_t Red, uint16_t Green, uint16_t Blue);
    void                 OVIA_PNG_SBIT_SetAlpha(uint8_t AlphaBitDepth);
    void                 OVIA_PNG_SBIT_SetBlue(uint8_t BlueBitDepth);
    void                 OVIA_PNG_SBIT_SetGray(uint8_t GrayBitDepth);
    void                 OVIA_PNG_SBIT_SetGreen(uint8_t GreenBitDepth);
    void                 OVIA_PNG_SBIT_SetRed(uint8_t RedBitDepth);
    void                 OVIA_PNG_SCAL_SetSCAL(uint8_t UnitSpecifier, double Width, double Height);
    void                 OVIA_PNG_SetAnimated(const bool PNGIsAnimated);
    void                 OVIA_PNG_SRGB_SetRenderingIntent(uint8_t RenderingIntent);
    void                 OVIA_PNG_STER_SetSterType(uint8_t sTERType);
    void                 OVIA_PNG_TIME_SetTime(uint16_t Year, uint8_t Month, uint8_t Day, uint8_t Hour, uint8_t Minute, uint8_t Second);
    void                 OVIA_PNG_TRNS_Init(uint64_t NumEntries);
    void                 OVIA_PNG_TRNS_SetPalette(uint64_t Entry, uint8_t Alpha);
    bool                 OVIA_PNG_OFFS_GetSpecifier(OVIA *Ovia);
    bool                 OVIA_PNG_PHYS_GetUnitSpecifier(OVIA *Ovia);
    HuffmanTable        *OVIA_PNG_DAT_GetDistanceHuffmanTable(OVIA *Ovia);
    HuffmanTable        *OVIA_PNG_DAT_GetLengthLiteralHuffmanTable(OVIA *Ovia);
    int32_t              OVIA_PNG_OFFS_GetXOffset(OVIA *Ovia);
    int32_t              OVIA_PNG_OFFS_GetYOffset(OVIA *Ovia);
    int32_t              OVIA_PNG_PHYS_GetPixelsPerUnitX(OVIA *Ovia);
    int32_t              OVIA_PNG_PHYS_GetPixelsPerUnitY(OVIA *Ovia);
    uint8_t             *OVIA_PNG_DAT_GetArray(OVIA *Ovia);
    uint8_t             *OVIA_PNG_ICCP_GetProfileData(OVIA *Ovia);
    uint8_t              OVIA_PNG_BKGD_GetBackgroundPaletteEntry(OVIA *Ovia);
    uint8_t              OVIA_PNG_DAT_GetCompressionInfo(OVIA *Ovia);
    uint8_t              OVIA_PNG_DAT_GetCompressionMethod(OVIA *Ovia);
    uint8_t              OVIA_PNG_DAT_GetFCHECK(OVIA *Ovia);
    uint8_t              OVIA_PNG_DAT_GetFDICT(OVIA *Ovia);
    uint8_t              OVIA_PNG_DAT_GetFLEVEL(OVIA *Ovia);
    uint8_t              OVIA_PNG_FCTL_GetBlendMethod(OVIA *Ovia);
    uint8_t              OVIA_PNG_FCTL_GetDisposeMethod(OVIA *Ovia);
    uint8_t              OVIA_PNG_ICCP_GetCompressionType(OVIA *Ovia);
    uint8_t              OVIA_PNG_iHDR_GetColorType(OVIA *Ovia);
    uint8_t              OVIA_PNG_PLTE_GetPaletteEntryBlue(uint64_t Entry);
    uint8_t              OVIA_PNG_PLTE_GetPaletteEntryGreen(uint64_t Entry);
    uint8_t              OVIA_PNG_PLTE_GetPaletteEntryRed(uint64_t Entry);
    uint8_t              OVIA_PNG_SBIT_GetAlpha(OVIA *Ovia);
    uint8_t              OVIA_PNG_SBIT_GetBlue(OVIA *Ovia);
    uint8_t              OVIA_PNG_SBIT_GetGray(OVIA *Ovia);
    uint8_t              OVIA_PNG_SBIT_GetGreen(OVIA *Ovia);
    uint8_t              OVIA_PNG_SBIT_GetRed(OVIA *Ovia);
    uint8_t              OVIA_PNG_SRGB_GetRenderingIntent(OVIA *Ovia);
    uint8_t              OVIA_PNG_STER_GetSterType(OVIA *Ovia);
    uint8_t              OVIA_PNG_TIME_GetDay(OVIA *Ovia);
    uint8_t              OVIA_PNG_TIME_GetHour(OVIA *Ovia);
    uint8_t              OVIA_PNG_TIME_GetMinute(OVIA *Ovia);
    uint8_t              OVIA_PNG_TIME_GetMonth(OVIA *Ovia);
    uint8_t              OVIA_PNG_TIME_GetSecond(OVIA *Ovia);
    uint16_t             OVIA_PNG_FCTL_GetFrameDelayDenominator(OVIA *Ovia);
    uint16_t             OVIA_PNG_FCTL_GetFrameDelayNumerator(OVIA *Ovia);
    uint16_t             OVIA_PNG_TIME_GetYear(OVIA *Ovia);
    uint32_t             OVIA_PNG_ACTL_GetNumFrames(OVIA *Ovia);
    uint32_t             OVIA_PNG_ACTL_GetTimes2Loop(OVIA *Ovia);
    uint32_t             OVIA_PNG_CHRM_GetBlueX(OVIA *Ovia);
    uint32_t             OVIA_PNG_CHRM_GetBlueY(OVIA *Ovia);
    uint32_t             OVIA_PNG_CHRM_GetGreenX(OVIA *Ovia);
    uint32_t             OVIA_PNG_CHRM_GetGreenY(OVIA *Ovia);
    uint32_t             OVIA_PNG_CHRM_GetRedX(OVIA *Ovia);
    uint32_t             OVIA_PNG_CHRM_GetRedY(OVIA *Ovia);
    uint32_t             OVIA_PNG_CHRM_GetWhitePointX(OVIA *Ovia);
    uint32_t             OVIA_PNG_CHRM_GetWhitePointY(OVIA *Ovia);
    uint32_t             OVIA_PNG_DAT_GetDictID(OVIA *Ovia);
    uint32_t             OVIA_PNG_FCTL_GetFrameNum(OVIA *Ovia);
    uint32_t             OVIA_PNG_FCTL_GetHeight(OVIA *Ovia);
    uint32_t             OVIA_PNG_FCTL_GetWidth(OVIA *Ovia);
    uint32_t             OVIA_PNG_FCTL_GetXOffset(OVIA *Ovia);
    uint32_t             OVIA_PNG_FCTL_GetYOffset(OVIA *Ovia);
    uint64_t             OVIA_PNG_DAT_GetArrayOffset(OVIA *Ovia);
    uint64_t             OVIA_PNG_DAT_GetArraySize(OVIA *Ovia);
    uint64_t             OVIA_PNG_ICCP_GetProfileDataSize(OVIA *Ovia);
    UTF8                *OVIA_PNG_ICCP_GetProfileName(OVIA *Ovia);
    
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
    
#ifdef __cplusplus
}
#endif

#endif /* OVIA_PNGCommon_H */
