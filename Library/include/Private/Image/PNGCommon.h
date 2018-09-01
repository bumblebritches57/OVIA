#include "../../libOVIA.h"

#include "../../../../Dependencies/FoundationIO/libFoundationIO/include/BitIO.h"
#include "../../../../Dependencies/FoundationIO/libFoundationIO/include/Log.h"
#include "../../../../Dependencies/FoundationIO/libFoundationIO/include/Math.h"
#include "../../../../Dependencies/FoundationIO/libFoundationIO/include/ContainerIO.h"

#pragma once

#ifndef OVIA_PNGCommon_H
#define OVIA_PNGCommon_H

#ifdef __cplusplus
extern "C" {
#endif
    
#define PNGMagic 0x89504E470D0A1A0A
    
    enum libModernPNGChunkMarkers {
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
    };
    
    typedef enum PNGColorTypes {
        PNG_Grayscale      = 0,
        PNG_RGB            = 2,
        PNG_PalettedRGB    = 3,
        PNG_GrayAlpha      = 4,
        PNG_RGBA           = 6,
    } PNGColorTypes;
    
    typedef enum libModernPNGFilterTypes {
        NotFiltered   = 0,
        SubFilter     = 1,
        UpFilter      = 2,
        AverageFilter = 3,
        PaethFilter   = 4,
    } libModernPNGFilterTypes;
    
    enum libModernPNGInterlaceType {
        PNGNotInterlaced   = 0,
        PNGInterlacedAdam7 = 1,
    };
    
    static const UTF8 ModernPNGNumber2Month[12][4] = {
        U8("Jan"), U8("Feb"), U8("Mar"), U8("Apr"), U8("May"), U8("Jun"),
        U8("Jul"), U8("Aug"), U8("Sep"), U8("Oct"), U8("Nov"), U8("Ovia"),
    };
    
    static const uint8_t ModernPNGChannelsPerColorType[7] = {
        1, 0, 3, 3, 4, 0, 4
    };
    
    uint32_t    CalculateSTERPadding(const uint32_t Width);
    
    struct PNGOVIAr {
        uint16_t    ***OVIAdImage;
        struct acTL   *acTL;
        struct bkGD   *bkGD;
        struct cHRM   *cHRM;
        struct fcTL   *fcTL;
        struct fdAT   *fdAT;
        struct gAMA   *gAMA;
        struct hIST   *hIST;
        struct iCCP   *iCCP;
        struct iHDR   *iHDR;
        struct oFFs   *oFFs;
        struct pCAL   *pCAL;
        struct pHYs   *pHYs;
        struct PLTE   *PLTE;
        struct sBIT   *sBIT;
        struct sCAL   *sCAL;
        struct sRGB   *sRGB;
        struct sTER   *sTER;
        struct Text   *Text;
        struct tIMe   *tIMe;
        struct tRNS   *tRNS;
        uint32_t       NumTextChunks;
        uint32_t       CurrentFrame;
        uint32_t       LineWidth;
        uint32_t       LinePadding;
        bool           PNGIsAnimated:1;
        bool           PNGIs3D:1;
        bool           acTLExists:1;
        bool           bkGDExists:1;
        bool           cHRMExists:1;
        bool           fcTLExists:1;
        bool           gAMAExists:1;
        bool           hISTExists:1;
        bool           iCCPExists:1;
        bool           oFFsExists:1;
        bool           pCALExists:1;
        bool           pHYsExists:1;
        bool           PLTEExists:1;
        bool           sBITExists:1;
        bool           sCALExists:1;
        bool           sPLTExists:1;
        bool           sRGBExists:1;
        bool           sTERExists:1;
        bool           TextExists:1;
        bool           tIMEExists:1;
        bool           tRNSExists:1;
    };
    
    struct PNGEncoder {
        struct acTL   *acTL;
        struct bkGD   *bkGD;
        struct cHRM   *cHRM;
        struct fcTL   *fcTL;
        struct fdAT   *fdAT;
        struct gAMA   *gAMA;
        struct hIST   *hIST;
        struct iCCP   *iCCP;
        struct iHDR   *iHDR;
        struct oFFs   *oFFs;
        struct pCAL   *pCAL;
        struct pHYs   *pHYs;
        struct PLTE   *PLTE;
        struct sBIT   *sBIT;
        struct sCAL   *sCAL;
        struct sRGB   *sRGB;
        struct sTER   *sTER;
        struct Text   *Text;
        struct tIMe   *tIMe;
        struct tRNS   *tRNS;
        uint32_t       NumTextChunks;
        uint32_t       CurrentFrame;
        uint32_t       LineWidth;
        uint32_t       LinePadding;
        bool           PNGIsAnimated:1;
        bool           PNGIs3D:1;
        bool           acTLExists:1;
        bool           bkGDExists:1;
        bool           cHRMExists:1;
        bool           fcTLExists:1;
        bool           gAMAExists:1;
        bool           hISTExists:1;
        bool           iCCPExists:1;
        bool           oFFsExists:1;
        bool           pCALExists:1;
        bool           pHYsExists:1;
        bool           PLTEExists:1;
        bool           sBITExists:1;
        bool           sCALExists:1;
        bool           sPLTExists:1;
        bool           sRGBExists:1;
        bool           sTERExists:1;
        bool           TextExists:1;
        bool           tIMEExists:1;
        bool           tRNSExists:1;
    };
    
    /*!
     @abstract                  "Encodes a PNG from RawImage2Encode to a BitBuffer"
     @param     Enc             "Pointer to EncodePNG struct containing all the metadata about the image to be encoded"
     @param     RawImage2Encode "Pointer to raw array containing the image, supports 2D array containing stereoscopic frames"
     @param     InterlacePNG    "Should this PNG file be interlaced using the Adam7 algorithm for progressive download?"
     @param     OptimizePNG     "Should this PNG file be optimized by trying all filters? (Huffman optimization is enabled by default)"
     */
    BitBuffer  *EncodePNGImage(OVIA *Ovia, void ****RawImage2Encode, bool InterlacePNG, bool OptimizePNG);
    
    /*!
     @abstract                  "OVIAs a PNG from a bitbuffer to an array"
     @param     Ovia             "Pointer to OVIA struct containing all the metadata about the image to be OVIAd"
     @param     PNGFile         "Pointer to raw array containing the image, supports 2D array containing stereoscopic frames"
     */
    uint16_t ***OVIAImage(OVIA *Ovia, BitBuffer *PNGFile);
    
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
    
    void PNGInsertImage(OVIA *Ovia, BitBuffer *BitB, ImageContainer *Image);
    
    typedef struct LZ77Tuple LZ77Tuple;
    
    typedef struct HuffmanNode HuffmanNode;
    
    typedef struct HuffmanTree HuffmanTree;
    
    uint32_t GenerateCRC32(BitBuffer *BitB, const uint64_t ChunkSize);
    bool     VerifyCRC32(BitBuffer *BitB, uint64_t ChunkSize);
    
    /*!
     @abstract                                    "Generates CRC from data".
     @param             Data2CRC                  "Pointer to a BitBuffer containing data to CRC".
     @param             BitOffset                 "Which bit should we start CRCing?".
     @param             BitLength                 "How many bits should we CRC?"
     @param             Polynomial                "The Polynomial in Normal representation".
     @param             PolySize                  "The size of the polynomial in bits".
     @param             PolyInit                  "Initialization value".
     */
    uint64_t            GenerateCRC(BitBuffer *Data2CRC, const uint64_t BitOffset, const uint64_t BitLength, const uint64_t Polynomial, const uint8_t PolySize, const uint64_t PolyInit);
    
    /*!
     @abstract                                    "Computes the CRC of DataBuffer, and compares it to the submitted CRC".
     @param             Data2CRC                  "Pointer to a BitBuffer containing data to CRC".
     @param             BitOffset                 "Which bit should we start CRCing?".
     @param             BitLength                 "How many bits should we CRC?"
     @param             Polynomial                "The Polynomial in Normal representation".
     @param             PolySize                  "The size of the polynomial in bits".
     @param             PolyInit                  "Initialization value".
     @param             PrecomputedCRC            "The precomputed resulting CRC of Data2CRC, to compare the generated CRC with".
     */
    bool                VerifyCRC(BitBuffer *Data2CRC, const uint64_t BitOffset, const uint64_t BitLength, const uint64_t Polynomial, const uint8_t PolySize, const uint64_t PolyInit, const uint64_t PrecomputedCRC);
    
    /*!
     @abstract                                    "Creates Adler32 checksum from input data".
     @return                                      "Returns the Adler32 data from the data input".
     @param             Data                      "Pointer to the data to generate the Adler hash from".
     @param             DataSize                  "Size of data".
     */
    uint32_t            GenerateAdler32(const uint8_t *Data, const uint64_t DataSize);
    
    /*!
     @abstract                                    "Generates Adler32 from the input data, and compares it to the submitted checksum".
     @return                                      "Returns whether the input data matched the provided checksum or not".
     @param             Data                      "Pointer to the data to generate the Adler hash from".
     @param             DataSize                  "Size of data".
     @param             EmbeddedAdler32           "Embedded Adler32 to compare the generated one to".
     */
    bool                VerifyAdler32(const uint8_t *Data, const uint64_t DataSize, const uint32_t EmbeddedAdler32);
    
    uint8_t    PaethPredictor(int64_t Left, int64_t Above, int64_t UpperLeft);
    
    uint8_t    ParsePNGMetadata(OVIA *Ovia, BitBuffer *BitB);
    
    void       PNGOVIASubFilter(OVIA *Ovia, uint8_t ***InflatedData, uint8_t ***DeFilteredData, size_t Line);
    
    void       PNGOVIANonFilter(OVIA *Ovia, uint8_t ***InflatedData, uint8_t ***DeFilteredData, size_t Line);
    
    void       PNGOVIAUpFilter(OVIA *Ovia, uint8_t ***InflatedData, uint8_t ***DeFilteredData, size_t Line);
    
    void       PNGOVIAAverageFilter(OVIA *Ovia, uint8_t ***InflatedData, uint8_t ***DeFilteredData, size_t Line);
    
    void       PNGOVIAPaethFilter(OVIA *Ovia, uint8_t ***InflatedData, uint8_t ***DeFilteredData, size_t Line);
    
    void       PNGOVIAFilter(OVIA *Ovia, void ***InflatedData);
    
    void WriteIHDRChunk(OVIA *Ovia, BitBuffer *BitB);
    
    void WriteACTLChunk(OVIA *Ovia, BitBuffer *BitB);
    
    void WriteFCTLChunk(OVIA *Ovia, BitBuffer *BitB);
    
    void WriteFDATChunk(OVIA *Ovia, BitBuffer *BitB, uint8_t *DeflatedFrameData, uint32_t DeflatedFrameDataSize);
    
    void WriteSTERChunk(OVIA *Ovia, BitBuffer *BitB);
    
    void WriteBKGDChunk(OVIA *Ovia, BitBuffer *BitB);
    
    void WriteCHRMChunk(OVIA *Ovia, BitBuffer *BitB);
    
    void WriteGAMAChunk(OVIA *Ovia, BitBuffer *BitB);
    
    void WriteOFFSChunk(OVIA *Ovia, BitBuffer *BitB);
    
    void WriteICCPChunk(OVIA *Ovia, BitBuffer *BitB);
    
    void WriteSBITChunk(OVIA *Ovia, BitBuffer *BitB);
    
    void WriteSRGBChunk(OVIA *Ovia, BitBuffer *BitB);
    
    void WritePHYSChunk(OVIA *Ovia, BitBuffer *BitB);
    
    void WritePCALChunk(OVIA *Ovia, BitBuffer *BitB);
    
    void WriteSCALChunk(OVIA *Ovia, BitBuffer *BitB);
    
    void PNGEncodeFilterPaeth(OVIA *Ovia, uint8_t *Line, size_t LineSize);
    
    void PNGEncodeFilterSub(OVIA *Ovia, uint8_t *Line, size_t NumPixels);
    
    void PNGEncodeAdam7(OVIA *Ovia, BitBuffer *ProgressiveImage, BitBuffer *InterlacedImage);
    
    void OptimizeAdam7(OVIA *Ovia, uint8_t ****Image);
    
    /*!
     @abstract       "Optimizes the image, by trying the 5 filters on each line, and keeping the best."
     */
    void OptimizePNG(OVIA *Ovia, uint8_t ****Image);
    
    void PNGEncodeImage(OVIA *Ovia, BitBuffer *BitB);
    
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
    
    /*
     enum Adam7Positions {
     Adam7Level1    =  0,
     Adam7Level2    =  5,
     Adam7Level3_1  = 32,
     Adam7Level3_2  = 36,
     Adam7Level4_1  =  3,
     Adam7Level4_2  =  7,
     Adam7Level4_3  = 35,
     Adam7Level4_4  = 39,
     Adam7Level5_1  = 17,
     Adam7Level5_2  = 19,
     Adam7Level5_3  = 21,
     Adam7Level5_4  = 23,
     Adam7Level5_5  = 49,
     Adam7Level5_6  = 51,
     Adam7Level5_7  = 53,
     Adam7Level5_8  = 55,
     Adam7Level6_1  =  2,
     Adam7Level6_2  =  4,
     Adam7Level6_3  =  6,
     Adam7Level6_4  =  8,
     Adam7Level6_5  = 18,
     Adam7Level6_6  = 20,
     Adam7Level6_7  = 22,
     Adam7Level6_8  = 24,
     Adam7Level6_9  = 34,
     Adam7Level6_10 = 36,
     Adam7Level6_11 = 38,
     Adam7Level6_12 = 40,
     Adam7Level6_13 = 50,
     Adam7Level6_14 = 52,
     Adam7Level6_15 = 54,
     Adam7Level6_16 = 56,
     Adam7Level7_1  = 9,
     Adam7Level7_2  = 10,
     Adam7Level7_3  = 11,
     Adam7Level7_4  = 12,
     Adam7Level7_5  = 13,
     Adam7Level7_6  = 14,
     Adam7Level7_7  = 15,
     Adam7Level7_8  = 16,
     Adam7Level7_9  = 25,
     Adam7Level7_10 = 26,
     Adam7Level7_11 = 27,
     Adam7Level7_12 = 28,
     Adam7Level7_13 = 29,
     Adam7Level7_14 = 30,
     Adam7Level7_15 = 31,
     Adam7Level7_16 = 32,
     Adam7Level7_17 = 41,
     Adam7Level7_18 = 42,
     Adam7Level7_19 = 43,
     Adam7Level7_20 = 44,
     Adam7Level7_21 = 45,
     Adam7Level7_22 = 46,
     Adam7Level7_23 = 47,
     Adam7Level7_24 = 48,
     Adam7Level7_25 = 57,
     Adam7Level7_26 = 58,
     Adam7Level7_27 = 59,
     Adam7Level7_28 = 60,
     Adam7Level7_29 = 61,
     Adam7Level7_30 = 62,
     Adam7Level7_31 = 63,
     Adam7Level7_32 = 64,
     };
     */
    
#ifdef __cplusplus
}
#endif

#endif /* OVIA_PNGCommon_H */
