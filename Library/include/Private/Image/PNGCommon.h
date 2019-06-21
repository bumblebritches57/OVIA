#include "../InternalOVIA.h"

#pragma once

#ifndef OVIA_PNGCommon_H
#define OVIA_PNGCommon_H

#ifdef __cplusplus
extern "C" {
#endif
    
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
    
    void                 OVIA_PNG_ACTL_SetACTL(OVIA *Ovia, uint32_t NumFrames, uint32_t Times2Loop);
    void                 OVIA_PNG_BKGD_SetBackgroundPaletteEntry(OVIA *Ovia, uint8_t PaletteEntry);
    void                 OVIA_PNG_CHRM_SetBlue(OVIA *Ovia, uint32_t BlueX, uint32_t BlueY);
    void                 OVIA_PNG_CHRM_SetGreen(OVIA *Ovia, uint32_t GreenX, uint32_t GreenY);
    void                 OVIA_PNG_CHRM_SetRed(OVIA *Ovia, uint32_t RedX, uint32_t RedY);
    void                 OVIA_PNG_CHRM_SetWhitePoint(OVIA *Ovia, uint32_t WhitePointX, uint32_t WhitePointY);
    void                 OVIA_PNG_DAT_SetArray(OVIA *Ovia, uint8_t *Array);
    void                 OVIA_PNG_DAT_SetArrayOffset(OVIA *Ovia, uint64_t ArrayOffset);
    void                 OVIA_PNG_DAT_SetArraySize(OVIA *Ovia, uint64_t ArraySize);
    void                 OVIA_PNG_DAT_SetCMF(OVIA *Ovia, uint8_t CMF);
    void                 OVIA_PNG_DAT_SetDictID(OVIA *Ovia, uint32_t DictID);
    void                 OVIA_PNG_DAT_SetDistanceHuffmanTable(OVIA *Ovia, HuffmanTable *DistanceTree);
    void                 OVIA_PNG_DAT_SetFCHECK(OVIA *Ovia, uint8_t FCHECK);
    void                 OVIA_PNG_DAT_SetFLG(OVIA *Ovia, uint8_t FLG);
    void                 OVIA_PNG_DAT_SetLengthLiteralHuffmanTable(OVIA *Ovia, HuffmanTable *LengthLiteralTree);
    void                 OVIA_PNG_FCTL_SetBlendMethod(OVIA *Ovia, uint8_t BlendMethod);
    void                 OVIA_PNG_FCTL_SetDisposeMethod(OVIA *Ovia, uint8_t DisposeMethod);
    void                 OVIA_PNG_FCTL_SetFCTL(OVIA *Ovia, const uint32_t FrameNum, const uint32_t Width, const uint32_t Height, uint32_t XOffset, uint32_t YOffset, uint16_t DelayNumerator, uint16_t DelayDenominator, uint8_t DisposalType, uint8_t BlendType);
    void                 OVIA_PNG_FCTL_SetFrameDelayDenominator(OVIA *Ovia, uint16_t FrameDelayDenominator);
    void                 OVIA_PNG_FCTL_SetFrameDelayNumerator(OVIA *Ovia, uint16_t FrameDelayNumerator);
    void                 OVIA_PNG_FCTL_SetFrameNum(OVIA *Ovia, uint32_t FrameNum);
    void                 OVIA_PNG_FCTL_SetHeight(OVIA *Ovia, uint32_t Height);
    void                 OVIA_PNG_FCTL_SetWidth(OVIA *Ovia, uint32_t Width);
    void                 OVIA_PNG_FCTL_SetXOffset(OVIA *Ovia, uint32_t XOffset);
    void                 OVIA_PNG_FCTL_SetYOffset(OVIA *Ovia, uint32_t YOffset);
    void                 OVIA_PNG_GAMA_SetGamma(OVIA *Ovia, uint32_t Gamma);
    void                 OVIA_PNG_HIST_SetNumEntries(OVIA *Ovia, uint32_t NumEntries);
    void                 OVIA_PNG_ICCP_SetCompressionType(OVIA *Ovia, uint8_t CompressionType);
    void                 OVIA_PNG_ICCP_SetProfileData(OVIA *Ovia, uint8_t *ProfileData);
    void                 OVIA_PNG_ICCP_SetProfileDataSize(OVIA *Ovia, uint64_t ProfileSize);
    void                 OVIA_PNG_ICCP_SetProfileName(OVIA *Ovia, UTF8 *ProfileName);
    void                 OVIA_PNG_IHDR_SetColorType(OVIA *Ovia, uint8_t ColorType);
    void                 OVIA_PNG_IHDR_SetIHDR(OVIA *Ovia, uint32_t Height, uint32_t Width, uint8_t BitDepth, uint8_t ColorType, const bool Interlace);
    void                 OVIA_PNG_OFFS_SetSpecifier(OVIA *Ovia, bool Specifier);
    void                 OVIA_PNG_OFFS_SetXOffset(OVIA *Ovia, int32_t XOffset);
    void                 OVIA_PNG_OFFS_SetYOffset(OVIA *Ovia, int32_t YOffset);
    void                 OVIA_PNG_PCAL_SetCalibrationName(OVIA *Ovia, UTF8 *CalibrationName);
    void                 OVIA_PNG_PHYS_SetPixelsPerUnitX(OVIA *Ovia, int32_t PixelsPerUnitX);
    void                 OVIA_PNG_PHYS_SetPixelsPerUnitY(OVIA *Ovia, int32_t PixelsPerUnitY);
    void                 OVIA_PNG_PHYS_SetUnitSpecifier(OVIA *Ovia, bool UnitSpecifier);
    void                 OVIA_PNG_PLTE_Init(OVIA *Ovia, uint64_t NumEntries);
    void                 OVIA_PNG_PLTE_SetPalette(OVIA *Ovia, uint64_t Entry, uint16_t Red, uint16_t Green, uint16_t Blue);
    void                 OVIA_PNG_SBIT_SetAlpha(OVIA *Ovia, uint8_t AlphaBitDepth);
    void                 OVIA_PNG_SBIT_SetBlue(OVIA *Ovia, uint8_t BlueBitDepth);
    void                 OVIA_PNG_SBIT_SetGray(OVIA *Ovia, uint8_t GrayBitDepth);
    void                 OVIA_PNG_SBIT_SetGreen(OVIA *Ovia, uint8_t GreenBitDepth);
    void                 OVIA_PNG_SBIT_SetRed(OVIA *Ovia, uint8_t RedBitDepth);
    void                 OVIA_PNG_SCAL_SetSCAL(OVIA *Ovia, uint8_t UnitSpecifier, double Width, double Height);
    void                 OVIA_PNG_SetAnimated(OVIA *Ovia, const bool PNGIsAnimated);
    void                 OVIA_PNG_SRGB_SetRenderingIntent(OVIA *Ovia, uint8_t RenderingIntent);
    void                 OVIA_PNG_STER_SetSterType(OVIA *Ovia, uint8_t sTERType);
    void                 OVIA_PNG_TIME_SetTime(OVIA *Ovia, uint16_t Year, uint8_t Month, uint8_t Day, uint8_t Hour, uint8_t Minute, uint8_t Second);
    void                 OVIA_PNG_TRNS_Init(OVIA *Ovia, uint64_t NumEntries);
    void                 OVIA_PNG_TRNS_SetPalette(OVIA *Ovia, uint64_t Entry, uint8_t Alpha);
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
    uint8_t              OVIA_PNG_PLTE_GetPaletteEntryBlue(OVIA *Ovia, uint64_t Entry);
    uint8_t              OVIA_PNG_PLTE_GetPaletteEntryGreen(OVIA *Ovia, uint64_t Entry);
    uint8_t              OVIA_PNG_PLTE_GetPaletteEntryRed(OVIA *Ovia, uint64_t Entry);
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
