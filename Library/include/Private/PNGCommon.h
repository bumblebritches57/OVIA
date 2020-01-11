/*!
 @header              PNGCommon.h
 @author              Marcus Johnson
 @copyright           2017+
 @version             1.0.0
 @brief               This header contains code for reading and writing lossless PNG image files
 */

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
        MaxDistanceCodes           = 29,
        NumMetaCodes               = 18,
        MaxBitsPerSymbol           = 15,
        EndOfBlock                 = 256,
    } FlateConstants;
    
    static const uint16_t LengthBase[29] = { // Lens in Puff
        3,   4,   5,   6,   7,  8,  9,  10,
        11,  13,  15,  17,  19, 23, 27,  31,
        35,  43,  51,  59,  67, 83, 99, 115,
        131, 163, 195, 227, 258
    };
    
    static const uint8_t LengthAdditionalBits[29] = { // Lext in Puff
        0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 2, 2, 2, 2,
        3, 3, 3, 3, 4, 4, 4, 4,
        5, 5, 5, 5, 0
    };
    
    static const uint16_t DistanceBase[30] = { // Dists in Puff
        1,    2,    3,     4,     5,     7,    9,   13,
        17,   25,   33,    49,    65,    97,  129,  193,
        257,  385,  513,   769,  1025,  1537, 2049, 3073,
        4097, 6145, 8193, 12289, 16385, 24577
    };
    
    static const uint8_t DistanceAdditionalBits[30] = { // Dext in Puff
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
    
    static const uint8_t PNG_NumChannelsPerColorType[7] = {
        1, 0, 3, 3, 4, 0, 4
    };
    
    static const UTF8 PNG_MonthMap[12][4] = {
        u8"Jan", u8"Feb", u8"Mar", u8"Apr", u8"May", u8"Jun",
        u8"Jul", u8"Aug", u8"Sep", u8"Oct", u8"Nov", u8"Dec",
    };
    
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
    
    typedef enum Flate_BlockTypes {
        BlockType_Literal = 0,
        BlockType_Fixed   = 1,
        BlockType_Dynamic = 2,
        BlockType_Invalid = 3,
    } Flate_BlockTypes;
    
    typedef enum HuffmanTreeTypes {
        TreeType_Unknown = 0,
        TreeType_Fixed   = 1,
        TreeType_Dynamic = 2,
    } HuffmanTreeTypes;
    
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
        iENDMarker         = 0x49454E44,
    } PNG_ChunkMarkers;
    
    typedef enum PNGTextTypes {
        UnknownTextType = 0,
        tEXt            = 1,
        iTXt            = 2,
        zTXt            = 3,
    } PNGTextTypes;
    
    typedef struct HuffmanNode {
        struct HuffmanNode *Left;
        struct HuffmanNode *Right;
        int16_t             Symbol; // Code
    } HuffmanNode;
    
    typedef struct HuffmanRange {
        
        uint8_t NumBits;
    } HuffmanRange;
    
    typedef struct HuffmanTree {
        uint16_t  NumSymbols;
        uint16_t *Frequency; // Was Count
        uint16_t *Symbol; // The binary encoded value read/written from/to the stream
    } HuffmanTree;
    
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
        uint8_t   *BackgroundPaletteEntry;
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
        UTF8      *Name;
        uint8_t    SampleDepth;
    } sPLTChunk;
    
    typedef struct pCALChunk {
        double    *Parameters;
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
        /* tEXT/zTXt */
        UTF8         *Author;
        UTF8         *Title;
        UTF8         *Description;
        UTF8         *CreationTime;
        /* tEXT/zTXt */
        /* iTXt */
        UTF8         *Keyword;
        UTF8         *Language;
        UTF8         *KeywordTranslated;
        UTF8         *Text;
        uint8_t       CompressionFlag;
        uint8_t       CompressionMethod;
        /* iTXt */
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
        uint32_t     Size;
        uint16_t     LengthLiteralTreeSize;
        uint8_t      DistanceTreeSize;
        uint32_t     DictID;
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
        uint8_t      *LZ77Buffer; // 32768 bytes
        sPLTChunk   **sPLT; // May be multiple
        TextChunk   **Text; // Order doesn't matter
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
        DATChunk     *DAT;
        /* 3D */
        sTERChunk    *sTER;
        /* 3D */
        /* APNG */
        acTLChunk    *acTL;
        fcTLChunk    *fcTL;
        fdATChunk    *fdAT;
        /* APNG */
        uint8_t       NumSPLTChunks;
        uint8_t       NumTextChunks;
        bool          IsAnimated;
    } PNGOptions;
    
    void *PNGOptions_Init(void);
    
    HuffmanTree *HuffmanTree_Init(uint16_t NumSymbols);
    
    void PNG_DAT_Decode(void *Options, BitBuffer *BitB, ImageContainer *Image);
    
    HuffmanTree *HuffmanBuildTree(uint64_t NumSymbols, const uint16_t *Lengths);
    
    uint64_t ReadHuffman(BitBuffer *BitB, HuffmanTree *Tree);
    
    uint16_t PNG_Flate_ReadSymbol(BitBuffer *BitB, HuffmanTree *Tree);
    
    void PNG_Flate_ReadZlibHeader(void *Options, BitBuffer *BitB);
    
    /*!
     @abstract                  "Encodes a PNG from ImageContainer to a BitBuffer"
     @param     Image           "ImageContainer with the image to encode".
     @param     BitB            "The BitBuffer to contain the encoded png".
     @param     OptimizePNG     "Should this PNG file be optimized by trying all filters? (Huffman optimization is enabled by default)"
     */
    void        PNG_Image_Insert(ImageContainer *Image, BitBuffer *BitB, bool OptimizePNG);
    
    void        PNG_SetTextChunk(UTF8 *KeywordString, UTF8 *CommentString);
    
    uint32_t    PNG_GetNumTextChunks(void *Options);
    
    /*!
     @abstract                  "Extracts the Keyword and Comment strings from the Instance of the text chunk".
     @remark                    "If the Keyword or Comment is DEFLATE encoded, we OVIA it to a regular string".
     @param     Instance        "Which instance of the text chunk should we extract"?
     @param     Keyword         "Pointer the Keyword string is returned through".
     @return                    "Returns the actual Comment string".
     */
    UTF8       *PNG_GetTextChunk(uint32_t Instance, UTF8 *Keyword);
    
    uint32_t    PNG_GetWidth(void *Options);
    
    uint32_t    PNG_GetHeight(void *Options);
    
    uint8_t     PNG_GetBitDepth(void *Options);
    
    uint8_t     PNG_GetColorType(void *Options);
    
    bool        PNG_GetInterlaceStatus(void *Options);
    
    bool        PNG_GetStereoscopicStatus(void *Options);
    
    uint32_t    PNG_GetWhitepointX(void *Options);
    
    uint32_t    PNG_GetWhitepointY(void *Options);
    
    uint32_t    PNG_GetGamma(void *Options);
    
    UTF8       *PNG_GetColorProfileName(void *Options);
    
    uint8_t    *PNG_GetColorProfile(void *Options);
    
    /*
     @param     GammaCorrect only does anything if there is a GAMA chunk present.
     */
    ImageContainer *PNGExtractImage(void *Options, BitBuffer *BitB);
    
    typedef struct HuffmanTree HuffmanTree;
    
    uint8_t             PaethPredictor(int64_t Left, int64_t Above, int64_t UpperLeft);
    
    void                PNG_ParseChunks(void *Options, BitBuffer *BitB);
    
    void                PNG_Filter_Sub(ImageContainer *Image);
    
    void                PNG_Filter_Up(ImageContainer *Image);
    
    void                PNG_Filter_Average(ImageContainer *Image);
    
    void                PNG_Filter_Paeth(ImageContainer *Image);
    
    void                WriteIHDRChunk(void *Options, BitBuffer *BitB);
    
    void                WriteACTLChunk(void *Options, BitBuffer *BitB);
    
    void                WriteFCTLChunk(void *Options, BitBuffer *BitB);
    
    void                WriteFDATChunk(void *Options, BitBuffer *BitB);
    
    void                WriteSTERChunk(void *Options, BitBuffer *BitB);
    
    void                WriteBKGDChunk(void *Options, BitBuffer *BitB);
    
    void                WriteCHRMChunk(void *Options, BitBuffer *BitB);
    
    void                WriteGAMAChunk(void *Options, BitBuffer *BitB);
    
    void                WriteOFFSChunk(void *Options, BitBuffer *BitB);
    
    void                WriteICCPChunk(void *Options, BitBuffer *BitB);
    
    void                WriteSBITChunk(void *Options, BitBuffer *BitB);
    
    void                WriteSRGBChunk(void *Options, BitBuffer *BitB);
    
    void                WritePHYSChunk(void *Options, BitBuffer *BitB);
    
    void                WritePCALChunk(void *Options, BitBuffer *BitB);
    
    void                WriteSCALChunk(void *Options, BitBuffer *BitB);
    
    void                PNGEncodeFilterPaeth(ImageContainer *Image);
    
    void                PNGEncodeFilterSub(ImageContainer *Image);
    
    void                PNGEncodeAdam7(BitBuffer *ProgressiveImage, BitBuffer *InterlacedImage);
    
    void                OptimizeAdam7(uint8_t *Image);
    
    /*!
     @abstract       "Optimizes the image, by trying the 5 filters on each line, and keeping the best."
     */
    void                OptimizePNG(uint8_t *Image);
    
    void                PNGEncodeImage(void *Options, BitBuffer *BitB);
    
    void PNGOptions_Deinit(void *Options);
    
    
    
    
    
    
    
    
    uint64_t ReadSymbol(BitBuffer *BitB, HuffmanTree *Tree);
    
    void PNG_Flate_ReadHuffman(void *Options, BitBuffer *BitB, HuffmanTree *LengthTree, HuffmanTree *DistanceTree, ImageContainer *Image);
    
    void PNG_Flate_ReadDeflateBlock(void *Options, BitBuffer *BitB, ImageContainer *Image);
    
    HuffmanTree *PNG_Flate_BuildHuffmanTree(uint16_t *SymbolLengths, uint16_t NumSymbols);
    
#ifdef __cplusplus
}
#endif

#endif /* OVIA_PNGCommon_H */
