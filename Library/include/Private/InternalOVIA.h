#include "../../../Dependencies/FoundationIO/libFoundationIO/include/BitIO.h"
#include "../../../Dependencies/FoundationIO/libFoundationIO/include/ContainerIO.h"
#include "../../../Dependencies/FoundationIO/libFoundationIO/include/Log.h"
#include "../../../Dependencies/FoundationIO/libFoundationIO/include/Math.h"
#include "../../../Dependencies/FoundationIO/libFoundationIO/include/StringIO.h"

#include "../../include/Private/Image/Flate.h"

#pragma once

#ifndef OVIA_InternalOVIA_H
#define OVIA_InternalOVIA_H

#ifdef __cplusplus
extern "C" {
#endif
    
    typedef struct       OVIA OVIA;
    
    typedef enum OVIA_Types {
        UnknownType           = 0,
        AudioType             = 1,
        ImageType             = 2,
    } OVIA_Types;
    
    typedef enum PNMTypes {
        UnknownPNM            = 0,
        BlackAndWhitePNM      = 1,
        BinaryPNM             = 2,
        ASCIIPNM              = 3,
        PAMPNM                = 4,
    } PNMTypes;
    
    typedef enum PNMTupleTypes {
        PNM_TUPLE_Unknown     = 0,
        PNM_TUPLE_BnW         = 1,
        PNM_TUPLE_Gray        = 2,
        PNM_TUPLE_GrayAlpha   = 3,
        PNM_TUPLE_RGB         = 4,
        PNM_TUPLE_RGBAlpha    = 5,
    } PNMTupleTypes;
    
    /* Format Specific Setters */
    /* FLAC */
    void                 OVIA_FLAC_CUE_SetCatalogID(OVIA *Ovia, char *CatalogID);
    void                 OVIA_FLAC_CUE_SetIsCD(OVIA *Ovia, bool IsCD);
    void                 OVIA_FLAC_CUE_SetLeadIn(OVIA *Ovia, uint64_t LeadIn);
    void                 OVIA_FLAC_CUE_SetNumTracks(OVIA *Ovia, uint8_t NumTracks);
    void                 OVIA_FLAC_CUE_SetTrack(OVIA *Ovia, uint64_t Offset, bool IsAudio, bool PreEmphasis, UTF8 *ISRC);
    void                 OVIA_FLAC_CUE_Track_SetOffset(OVIA *Ovia, uint8_t Track, uint64_t TrackOffset);
    void                 OVIA_FLAC_Frame_SetBlockSize(OVIA *Ovia, uint16_t BlockSize);
    void                 OVIA_FLAC_Frame_SetBlockType(OVIA *Ovia, bool BlockTypeIsFixed);
    void                 OVIA_FLAC_Frame_SetChannelLayout(OVIA *Ovia, uint8_t ChannelLayout);
    void                 OVIA_FLAC_Frame_SetCodedBitDepth(OVIA *Ovia, uint8_t CodedBitDepth);
    void                 OVIA_FLAC_Frame_SetCodedBlockSize(OVIA *Ovia, uint8_t CodedBlockSize);
    void                 OVIA_FLAC_Frame_SetCodedSampleRate(OVIA *Ovia, uint8_t CodedSampleRate);
    void                 OVIA_FLAC_Frame_SetFrameNumber(OVIA *Ovia, uint64_t FrameNumber);
    void                 OVIA_FLAC_Frame_SetSampleNumber(OVIA *Ovia, uint64_t SampleNumber);
    void                 OVIA_FLAC_Frame_SetSampleRate(OVIA *Ovia, uint32_t SampleRate);
    void                 OVIA_FLAC_LPC_SetLPCCoeff(OVIA *Ovia, uint8_t CoeffNum, uint8_t Coeff);
    void                 OVIA_FLAC_LPC_SetLPCOrder(OVIA *Ovia, uint8_t LPCOrder);
    void                 OVIA_FLAC_LPC_SetLPCPrecision(OVIA *Ovia, uint8_t LPCPrecision);
    void                 OVIA_FLAC_LPC_SetLPCShift(OVIA *Ovia, uint8_t LPCShift);
    void                 OVIA_FLAC_LPC_SetNumLPCCoeffs(OVIA *Ovia, uint8_t NumLPCCoeffs);
    void                 OVIA_FLAC_LPC_SetRICEParameter(OVIA *Ovia, uint8_t Partition, uint8_t Parameter);
    void                 OVIA_FLAC_LPC_SetRICEPartitionOrder(OVIA *Ovia, uint8_t RICEPartitionOrder);
    void                 OVIA_FLAC_LPC_SetRICEPartitionType(OVIA *Ovia, uint8_t RICEPartitionType);
    void                 OVIA_FLAC_Seek_SetSeekPoint(OVIA *Ovia, uint32_t SeekPoint, uint64_t Sample, uint64_t Offset, uint16_t FrameSize);
    void                 OVIA_FLAC_SetEncodeSubset(OVIA *Ovia, bool EncodeSubset);
    void                 OVIA_FLAC_SetMaxBlockSize(OVIA *Ovia, uint16_t MaxBlockSize);
    void                 OVIA_FLAC_SetMaxFilterOrder(OVIA *Ovia, uint16_t MaxFilterOrder);
    void                 OVIA_FLAC_SetMaxFrameSize(OVIA *Ovia, uint16_t MaxFrameSize);
    void                 OVIA_FLAC_SetMaxRicePartitionOrder(OVIA *Ovia, uint8_t MaxRICEPartitionOrder);
    void                 OVIA_FLAC_SetMD5(OVIA *Ovia, uint8_t *MD5);
    void                 OVIA_FLAC_SetMinBlockSize(OVIA *Ovia, uint16_t MinBlockSize);
    void                 OVIA_FLAC_SetMinFrameSize(OVIA *Ovia, uint16_t MinFrameSize);
    void                 OVIA_FLAC_SubFrame_SetType(OVIA *Ovia, uint8_t SubframeType);
    void                 OVIA_FLAC_SubFrame_SetWastedBits(OVIA *Ovia, bool WastedBitsFlag, uint8_t NumWastedBits);
    /* FLAC */
    
    /* PNG */
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
    /* PNG */
    
    /* BMP */
    void                 OVIA_BMP_SetAMask(OVIA *Ovia, uint32_t AlphaMask);
    void                 OVIA_BMP_SetBGamma(OVIA *Ovia, uint32_t BGamma);
    void                 OVIA_BMP_SetBMask(OVIA *Ovia, uint32_t BlueMask);
    void                 OVIA_BMP_SetColorsIndexed(OVIA *Ovia, uint32_t ColorsIndexed);
    void                 OVIA_BMP_SetColorSpaceType(OVIA *Ovia, uint32_t ColorSpaceType);
    void                 OVIA_BMP_SetCompressionType(OVIA *Ovia, uint32_t CompressionType);
    void                 OVIA_BMP_SetDIBSize(OVIA *Ovia, uint32_t DIBSize);
    void                 OVIA_BMP_SetFileSize(OVIA *Ovia, uint32_t FileSize);
    void                 OVIA_BMP_SetGGamma(OVIA *Ovia, uint32_t GGamma);
    void                 OVIA_BMP_SetGMask(OVIA *Ovia, uint32_t GreenMask);
    void                 OVIA_BMP_SetHeightInMeters(OVIA *Ovia, uint32_t HeightInMeters);
    void                 OVIA_BMP_SetICC(OVIA *Ovia, uint32_t ICCIntent, uint32_t ICCSize, uint8_t *ICCPayload);
    void                 OVIA_BMP_SetIndexColorsUsed(OVIA *Ovia, uint32_t IndexColorsUsed);
    void                 OVIA_BMP_SetNumBytesUsedBySamples(OVIA *Ovia, uint32_t NumBytesUsedBySamples);
    void                 OVIA_BMP_SetOffset(OVIA *Ovia, uint32_t Offset);
    void                 OVIA_BMP_SetRGamma(OVIA *Ovia, uint32_t RGamma);
    void                 OVIA_BMP_SetRMask(OVIA *Ovia, uint32_t RedMask);
    void                 OVIA_BMP_SetWidthInMeters(OVIA *Ovia, uint32_t WidthInMeters);
    void                 OVIA_BMP_SetXCoordinate(OVIA *Ovia, uint32_t XCoordinate);
    void                 OVIA_BMP_SetYCoordinate(OVIA *Ovia, uint32_t YCoordinate);
    void                 OVIA_BMP_SetZCoordinate(OVIA *Ovia, uint32_t ZCoordinate);
    /* BMP */
    
    /* WAV */
    uint16_t             OVIA_WAV_GetBlockAlignment(OVIA *Ovia);
    void                 OVIA_WAV_SetBlockAlignment(OVIA *Ovia, uint16_t BlockAlignment);
    void                 OVIA_WAV_SetCompressionType(OVIA *Ovia, uint16_t CompressionType);
    /* WAV */
    
    /* W64 */
    uint16_t             OVIA_W64_GetBlockAlignment(OVIA *Ovia);
    void                 OVIA_W64_SetBlockAlignment(OVIA *Ovia, uint16_t BlockAlignment);
    void                 OVIA_W64_SetCompressionType(OVIA *Ovia, uint16_t CompressionType);
    /* W64 */
    
    /* PNM */
    void                 OVIA_PNM_SetPNMType(OVIA *Ovia, PNMTypes PNMType);
    void                 OVIA_PNM_SetTupleType(OVIA *Ovia, PNMTupleTypes TupleType);
    /* PNM */
    /* Format Specific Setters */
    
    /* Format Specific Getters */
    /* FLAC */
    bool                 OVIA_FLAC_CUE_GetIsCD(OVIA *Ovia);
    bool                 OVIA_FLAC_Frame_GetBlockType(OVIA *Ovia);
    bool                 OVIA_FLAC_GetEncodeSubset(OVIA *Ovia);
    uint8_t             *OVIA_FLAC_GetMD5(OVIA *Ovia);
    uint8_t              OVIA_FLAC_Frame_GetChannelLayout(OVIA *Ovia);
    uint8_t              OVIA_FLAC_Frame_GetCodedBitDepth(OVIA *Ovia);
    uint8_t              OVIA_FLAC_Frame_GetCodedBlockSize(OVIA *Ovia);
    uint8_t              OVIA_FLAC_Frame_GetCodedSampleRate(OVIA *Ovia);
    uint8_t              OVIA_FLAC_LPC_GetLPCCoeff(OVIA *Ovia, uint8_t CoeffNum);
    uint8_t              OVIA_FLAC_LPC_GetLPCOrder(OVIA *Ovia);
    uint8_t              OVIA_FLAC_LPC_GetLPCPrecision(OVIA *Ovia);
    uint8_t              OVIA_FLAC_LPC_GetLPCShift(OVIA *Ovia);
    uint8_t              OVIA_FLAC_LPC_GetNumLPCCoeffs(OVIA *Ovia);
    uint8_t              OVIA_FLAC_LPC_GetRICEParameter(OVIA *Ovia, uint8_t Partition);
    uint8_t              OVIA_FLAC_LPC_GetRICEPartitionOrder(OVIA *Ovia);
    uint8_t              OVIA_FLAC_LPC_GetRICEPartitionType(OVIA *Ovia);
    uint64_t             OVIA_FLAC_Frame_GetFrameNumber(OVIA *Ovia);
    uint64_t             OVIA_FLAC_Frame_GetSampleNumber(OVIA *Ovia);
    /* FLAC */
    
    /* PNG */
    bool                 OVIA_PNG_OFFS_GetSpecifier(OVIA *Ovia);
    bool                 OVIA_PNG_PHYS_GetUnitSpecifier(OVIA *Ovia);
    HuffmanTable         *OVIA_PNG_DAT_GetDistanceHuffmanTable(OVIA *Ovia);
    HuffmanTable         *OVIA_PNG_DAT_GetLengthLiteralHuffmanTable(OVIA *Ovia);
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
    /* PNG */
    
    /* BMP */
    uint8_t             *OVIA_BMP_GetICCPayload(OVIA *Ovia);
    uint32_t             OVIA_BMP_GetAMask(OVIA *Ovia);
    uint32_t             OVIA_BMP_GetBGamma(OVIA *Ovia);
    uint32_t             OVIA_BMP_GetBMask(OVIA *Ovia);
    uint32_t             OVIA_BMP_GetColorsIndexed(OVIA *Ovia);
    uint32_t             OVIA_BMP_GetColorSpaceType(OVIA *Ovia);
    uint32_t             OVIA_BMP_GetCompressionType(OVIA *Ovia);
    uint32_t             OVIA_BMP_GetDIBSize(OVIA *Ovia);
    uint32_t             OVIA_BMP_GetFileSize(OVIA *Ovia);
    uint32_t             OVIA_BMP_GetGGamma(OVIA *Ovia);
    uint32_t             OVIA_BMP_GetGMask(OVIA *Ovia);
    uint32_t             OVIA_BMP_GetHeightInMeters(OVIA *Ovia);
    uint32_t             OVIA_BMP_GetICCIntent(OVIA *Ovia);
    uint32_t             OVIA_BMP_GetICCSize(OVIA *Ovia);
    uint32_t             OVIA_BMP_GetIndexColorsUsed(OVIA *Ovia);
    uint32_t             OVIA_BMP_GetNumBytesUsedBySamples(OVIA *Ovia);
    uint32_t             OVIA_BMP_GetOffset(OVIA *Ovia);
    uint32_t             OVIA_BMP_GetRGamma(OVIA *Ovia);
    uint32_t             OVIA_BMP_GetRMask(OVIA *Ovia);
    uint32_t             OVIA_BMP_GetWidthInMeters(OVIA *Ovia);
    uint32_t             OVIA_BMP_GetXCoordinate(OVIA *Ovia);
    uint32_t             OVIA_BMP_GetYCoordinate(OVIA *Ovia);
    uint32_t             OVIA_BMP_GetZCoordinate(OVIA *Ovia);
    /* BMP */
    
    PNMTupleTypes        OVIA_PNM_GetTupleType(OVIA *Ovia);
    PNMTypes             OVIA_PNM_GetPNMType(OVIA *Ovia);
    
    void OVIA_SetAudioContainerPointer(OVIA *Ovia, AudioContainer *Audio);
    /* Format Specific Getters */
    
#ifdef __cplusplus
}
#endif

#endif /* OVIA_libOVIA_H */
