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
    
    typedef enum OVIA_FileFormats {
        UnknownFormat         = 0,
        AIFFormat             = 1,
        WAVFormat             = 2,
        W64Format             = 3,
        FLACFormat            = 4,
        BMPFormat             = 5,
        PNMFormat             = 6,
        PNGFormat             = 7,
    } OVIA_FileFormats;
    
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
    
    /* OVIA Setters */
    void                 OVIA_SetAudioContainerPointer(OVIA *Ovia, AudioContainer *Audio);
    void                 OVIA_SetBitDepth(OVIA *Ovia, uint64_t BitDepth);
    void                 OVIA_SetBlockSize(OVIA *Ovia, uint64_t BlockSize);
    void                 OVIA_SetFileSize(OVIA *Ovia, uint64_t FileSize);
    void                 OVIA_SetHeight(OVIA *Ovia, int64_t Height);
    void                 OVIA_SetNumChannels(OVIA *Ovia, uint64_t NumChannels);
    void                 OVIA_SetNumSamples(OVIA *Ovia, uint64_t NumSamples);
    void                 OVIA_SetSampleOffset(OVIA *Ovia, uint64_t SampleOffset);
    void                 OVIA_SetSampleRate(OVIA *Ovia, uint64_t SampleRate);
    void                 OVIA_SetTag(OVIA *Ovia, OVIA_TagTypes TagType, UTF8 *Tag);
    void                 OVIA_SetWidth(OVIA *Ovia, int64_t Width);
    /* OVIA Setters */
    
    /* OVIA Getters */
    AudioContainer      *OVIA_GetAudioContainerPointer(OVIA *Ovia);
    /* OVIA Getters */
    
    /* Format Specific Setters */
    /* FLAC */
    void                 OVIA_FLAC_CUE_SetIsCD(OVIA *Ovia, bool IsCD);
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
    void                 OVIA_FLAC_SubFrame_SetType(OVIA *Ovia, uint8_t SubframeType);
    void                 OVIA_FLAC_SubFrame_SetWastedBits(OVIA *Ovia, bool WastedBitsFlag, uint8_t NumWastedBits);
    /* FLAC */
    
    /* PNG */
    uint32_t             OVIA_PNG_DAT_GetDictID(OVIA *Ovia);
    uint8_t              OVIA_PNG_DAT_GetCompressionInfo(OVIA *Ovia);
    uint8_t              OVIA_PNG_DAT_GetCompressionMethod(OVIA *Ovia);
    uint8_t              OVIA_PNG_DAT_GetFCHECK(OVIA *Ovia);
    uint8_t              OVIA_PNG_DAT_GetFDICT(OVIA *Ovia);
    uint8_t              OVIA_PNG_DAT_GetFLEVEL(OVIA *Ovia);
    void                 OVIA_PNG_ACTL_SetACTL(OVIA *Ovia, uint32_t NumFrames, uint32_t Times2Loop);
    void                 OVIA_PNG_BKGD_SetBackgroundPaletteEntry(OVIA *Ovia, uint8_t PaletteEntry);
    void                 OVIA_PNG_DAT_SetCMF(OVIA *Ovia, uint8_t CMF);
    void                 OVIA_PNG_DAT_SetDictID(OVIA *Ovia, uint32_t DictID);
    void                 OVIA_PNG_DAT_SetFLG(OVIA *Ovia, uint8_t FLG);
    void                 OVIA_PNG_GAMA_SetGamma(OVIA *Ovia, uint32_t Gamma);
    void                 OVIA_PNG_OFFS_SetSpecifier(OVIA *Ovia, bool Specifier);
    void                 OVIA_PNG_OFFS_SetXOffset(OVIA *Ovia, int32_t XOffset);
    void                 OVIA_PNG_OFFS_SetYOffset(OVIA *Ovia, int32_t YOffset);
    void                 OVIA_PNG_PCAL_SetCalibrationName(OVIA *Ovia, UTF8 *CalibrationName);
    void                 OVIA_PNG_PHYS_SetPixelsPerUnitX(OVIA *Ovia, int32_t PixelsPerUnitX);
    void                 OVIA_PNG_PHYS_SetPixelsPerUnitY(OVIA *Ovia, int32_t PixelsPerUnitY);
    void                 OVIA_PNG_PHYS_SetUnitSpecifier(OVIA *Ovia, bool UnitSpecifier);
    void                 OVIA_PNG_SBIT_SetBlue(OVIA *Ovia, uint8_t BlueBitDepth);
    void                 OVIA_PNG_SBIT_SetGray(OVIA *Ovia, uint8_t GrayBitDepth);
    void                 OVIA_PNG_SBIT_SetGreen(OVIA *Ovia, uint8_t GreenBitDepth);
    void                 OVIA_PNG_SBIT_SetRed(OVIA *Ovia, uint8_t RedBitDepth);
    void                 OVIA_PNG_SCAL_SetSCAL(OVIA *Ovia, uint8_t UnitSpecifier, double Width, double Height);
    void                 OVIA_PNG_SRGB_SetRenderingIntent(OVIA *Ovia, uint8_t RenderingIntent);
    void                 OVIA_PNG_STER_SetSterType(OVIA *Ovia, uint8_t sTERType);
    void                 OVIA_PNG_DAT_SetFCHECK(OVIA *Ovia, uint8_t FCHECK);
    void                 OVIA_PNG_DAT_SetArray(OVIA *Ovia, uint8_t ****Array);
    void                 OVIA_PNG_DAT_SetArraySize(OVIA *Ovia, uint64_t ArraySize);
    void                 OVIA_PNG_DAT_SetArrayOffset(OVIA *Ovia, uint64_t ArrayOffset);
    void                 OVIA_PNG_DAT_SetLengthLiteralHuffmanTree(OVIA *Ovia, HuffmanTree *LengthLiteralTree);
    void                 OVIA_PNG_DAT_SetDistanceHuffmanTree(OVIA *Ovia, HuffmanTree *DistanceTree);
    /* PNG */
    /* Format Specific Setters */
    
    /* Format Specific Getters */
    /* FLAC */
    bool                 OVIA_FLAC_CUE_GetIsCD(OVIA *Ovia);
    bool                 OVIA_FLAC_Frame_GetBlockType(OVIA *Ovia);
    uint64_t             OVIA_FLAC_Frame_GetFrameNumber(OVIA *Ovia);
    uint64_t             OVIA_FLAC_Frame_GetSampleNumber(OVIA *Ovia);
    uint8_t              OVIA_FLAC_Frame_GetChannelLayout(OVIA *Ovia);
    uint8_t              OVIA_FLAC_Frame_GetCodedBitDepth(OVIA *Ovia);
    uint8_t              OVIA_FLAC_Frame_GetCodedBlockSize(OVIA *Ovia);
    uint8_t              OVIA_FLAC_Frame_GetCodedSampleRate(OVIA *Ovia);
    uint8_t              OVIA_FLAC_LPC_GetLPCOrder(OVIA *Ovia);
    uint8_t              OVIA_FLAC_LPC_GetLPCPrecision(OVIA *Ovia);
    uint8_t              OVIA_FLAC_LPC_GetLPCShift(OVIA *Ovia);
    uint8_t              OVIA_FLAC_LPC_GetNumLPCCoeff(OVIA *Ovia, uint8_t CoeffNum);
    uint8_t              OVIA_FLAC_LPC_GetNumLPCCoeffs(OVIA *Ovia);
    uint8_t              OVIA_FLAC_LPC_GetRICEParameter(OVIA *Ovia, uint8_t Partition);
    uint8_t              OVIA_FLAC_LPC_GetRICEPartitionOrder(OVIA *Ovia);
    uint8_t              OVIA_FLAC_LPC_GetRICEPartitionType(OVIA *Ovia);
    /* FLAC */
    
    /* PNG */
    bool                 OVIA_PNG_OFFS_GetSpecifier(OVIA *Ovia);
    bool                 OVIA_PNG_PHYS_GetUnitSpecifier(OVIA *Ovia);
    int32_t              OVIA_PNG_OFFS_GetXOffset(OVIA *Ovia);
    int32_t              OVIA_PNG_OFFS_GetYOffset(OVIA *Ovia);
    int32_t              OVIA_PNG_PHYS_GetPixelsPerUnitX(OVIA *Ovia);
    int32_t              OVIA_PNG_PHYS_GetPixelsPerUnitY(OVIA *Ovia);
    uint16_t             OVIA_PNG_FCTL_GetFrameDelayDenominator(OVIA *Ovia);
    uint16_t             OVIA_PNG_FCTL_GetFrameDelayNumerator(OVIA *Ovia);
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
    uint32_t             OVIA_PNG_FCTL_GetFrameNum(OVIA *Ovia);
    uint32_t             OVIA_PNG_FCTL_GetHeight(OVIA *Ovia);
    uint32_t             OVIA_PNG_FCTL_GetWidth(OVIA *Ovia);
    uint32_t             OVIA_PNG_FCTL_GetXOffset(OVIA *Ovia);
    uint32_t             OVIA_PNG_FCTL_GetYOffset(OVIA *Ovia);
    uint64_t             OVIA_PNG_ICCP_GetProfileDataSize(OVIA *Ovia);
    uint8_t              OVIA_PNG_BKGD_GetBackgroundPaletteEntry(OVIA *Ovia);
    uint8_t              OVIA_PNG_FCTL_GetBlendMethod(OVIA *Ovia);
    uint8_t              OVIA_PNG_FCTL_GetDisposeMethod(OVIA *Ovia);
    uint8_t              OVIA_PNG_ICCP_GetCompressionType(OVIA *Ovia);
    uint8_t             *OVIA_PNG_ICCP_GetProfileData(OVIA *Ovia);
    uint8_t              OVIA_PNG_iHDR_GetColorType(OVIA *Ovia);
    uint8_t              OVIA_PNG_SBIT_GetAlpha(OVIA *Ovia);
    uint8_t              OVIA_PNG_SBIT_GetBlue(OVIA *Ovia);
    uint8_t              OVIA_PNG_SBIT_GetGray(OVIA *Ovia);
    uint8_t              OVIA_PNG_SBIT_GetGreen(OVIA *Ovia);
    uint8_t              OVIA_PNG_SBIT_GetRed(OVIA *Ovia);
    uint8_t              OVIA_PNG_SRGB_GetRenderingIntent(OVIA *Ovia);
    uint8_t              OVIA_PNG_STER_GetSterType(OVIA *Ovia);
    UTF8                *OVIA_PNG_ICCP_GetProfileName(OVIA *Ovia);
    uint8_t          ****OVIA_PNG_DAT_GetArray(OVIA *Ovia);
    uint64_t             OVIA_PNG_DAT_GetArraySize(OVIA *Ovia);
    uint64_t             OVIA_PNG_DAT_GetArrayOffset(OVIA *Ovia);
    HuffmanTree         *OVIA_PNG_DAT_GetLengthLiteralHuffmanTree(OVIA *Ovia);
    HuffmanTree         *OVIA_PNG_DAT_GetDistanceHuffmanTree(OVIA *Ovia);
    /* PNG */
    
    /* BMP */
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
    
    uint16_t             OVIA_W64_GetBlockAlignment(OVIA *Ovia);
    uint16_t             OVIA_WAV_GetBlockAlignment(OVIA *Ovia);
    /* Format Specific Getters */
    
#ifdef __cplusplus
}
#endif

#endif /* OVIA_libOVIA_H */
