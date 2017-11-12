#include "../../Dependencies/BitIO/libBitIO/include/BitIO.h"

#pragma once

#ifndef LIBPCM_libPCM_H
#define LIBPCM_libPCM_H

#ifdef __cplusplus
extern "C" {
#endif
    
    enum libPCMConstants {
        PXMMagicSize          = 2,
        PXMCommentStart       = 0x23,
        PXMFieldSeperator     = 0x20,
        PXMEndField           = 0x0A,
    };
    
    enum WAVFormatCode {
        PCM                   = 1,
    };
    
    typedef enum libPCMFileFormats {
        UnknownFormat         = 0,
        AIFFormat             = 1,
        WAVFormat             = 2,
        WAVFormatExtensible   = 4,
        W64Format             = 5,
        BMPFormat             = 6,
        PXMFormat             = 7,
    } libPCMFileFormats;
    
    enum AIFSpeakerMask {
        AIFFrontLeft          = 0x1,
        AIFFrontRight         = 0x2,
        AIFFrontCenter        = 0x4,
        AIFRearLeft           = 0x8,
        AIFRearRight          = 0x10,
        AIFSurround           = 0x20,
        AIFLeftCenter         = 0x40,
        AIFRightCenter        = 0x80,
    };
    
    enum WAVSpeakerMask {
        WAVFrontRight         = 0x1,
        WAVFrontLeft          = 0x2,
        WAVFrontCenter        = 0x4,
        WAVLFE                = 0x8,
        WAVBackLeft           = 0x10,
        WAVBackRight          = 0x20,
        WAVFrontLeftCenter    = 0x40,
        WAVFrontRightCenter   = 0x80,
        WAVBackCenter         = 0x100,
        WAVSideLeft           = 0x200,
        WAVSideRight          = 0x400,
        WAVTopCenter          = 0x800,
        WAVTopFrontLeft       = 0x1000,
        WAVTopFrontCenter     = 0x2000,
        WAVTopFrontRight      = 0x4000,
        WAVTopBackLeft        = 0x8000,
        WAVTopBackCenter      = 0x10000,
        WAVTopBackRight       = 0x20000,
    };
    
    enum AIFChunkIDs {
        AIF_FORM              = 0x464F524D,
        AIF_AIFF              = 0x41494646,
        AIF_AIFC              = 0x41494643,
        AIF_NAME              = 0x4E414D45,
        AIF_COMM              = 0x434F4D4D,
        AIF_SSND              = 0x53534E44,
        AIF_ID3               = 0x49443320,
        AIF_MARK              = 0x4d41524b,
        AIF_INST              = 0x494e5354,
        AIF_MIDI              = 0x4d494449,
        AIF_AESD              = 0x41455344,
        AIF_AAPL              = 0x4150504c,
        AIF_COMT              = 0x434f4d54,
        AIF_AUTH              = 0x41555448,
        AIF_ANNO              = 0x414e4e4f,
    };
    
    enum WAVChunkIDs { // odd sized chunks have a trailing 0 byte, and strings are null padded, this is for all RIFF based formats.
        WAV_RIFF              = 0x52494646,
        WAV_WAVE              = 0x57415645,
        WAV_FMT               = 0x666D7420,
        WAV_LIST              = 0x4C495354,
        WAV_INFO              = 0x494E464F,
        WAV_IART              = 0x49415254, // Artist
        WAV_ICRD              = 0x49435244, // Release data
        WAV_IGNR              = 0x49474E52, // Genre
        WAV_INAM              = 0x494E414D, // Song name aka title
        WAV_IPRD              = 0x49505244, // Album name
        WAV_IPRT              = 0x49505254, // Track number
        WAV_ISFT              = 0x49534654, // Software that created it?
        WAV_CODE              = 0x434f4445, // Encoder
        WAV_ICMT              = 0x49434d54, // Comment
        WAV_ICOP              = 0x49434f50, // Copyright
        WAV_YEAR              = 0x59454152, // Year
        WAV_TRCK              = 0x5452434b, // Track
        WAV_DATA              = 0x64617461,
    };
    
    enum BMPMagic {
        BMP_BM                = 0x424D,
        BMP_BitmapArray       = 0x4141,
        BMP_ColorIcon         = 0x4349,
        BMP_ColorPointer      = 0x4350,
        BMP_StructIcon        = 0x4943,
        BMP_Pointer           = 0x5054,
    };
    
    enum BMPCompressionMethods {
        BMP_RGB               = 0,
        BMP_RLE_8Bit          = 1,
        BMP_RLE_4Bit          = 2,
        BMP_BitFields         = 3,
        BMP_JPEG              = 4,
        BMP_PNG               = 5,
        BMP_RGBABitFields     = 6,
        BMP_CMYK              = 11,
        BMP_CMYK_RLE_8Bit     = 12,
        BMP_CMYK_RLE_4Bit     = 13,
    };
    
    enum PXMMagic {
        PXM_PBMA = 0x5031, // PortableBitMapASCII
        PXM_PBMB = 0x5034, // PortableBitMapBinary
        PXM_PGMA = 0x5032, // PortableGrayMapASCII
        PXM_PGMB = 0x5035, // PortableGrayMapBinary
        PXM_PPMA = 0x5033, // PortablePixMapASCII
        PXM_PPMB = 0x5036, // PortablePixMapBinary
        PXM_PAMB = 0x5037, // PortableAnyMap
    };
    
    typedef enum PXMTupleTypes {
        PXM_TUPLE_Unknown   = 0,
        PXM_TUPLE_BnW       = 1,
        PXM_TUPLE_Gray      = 2,
        PXM_TUPLE_GrayAlpha = 3,
        PXM_TUPLE_RGB       = 4,
        PXM_TUPLE_RGBAlpha  = 5,
    } PXMTupleTypes;
    
    typedef enum PXMTypes {
        UnknownPXM            = 0,
        BlackAndWhitePXM      = 1,
        BinaryPXM             = 2,
        ASCIIPXM              = 3,
        PAMPXM                = 4,
    } PXMTypes;
    
    static const char *libPCMLibraryName = "libPCM";
    
    typedef struct PCMFile PCMFile;
    
    PCMFile   *PCMFile_Init(void);
    
    void       PCMFile_Identify(PCMFile *PCM, BitBuffer *BitB);
    
    void       PCMFile_ParseMetadata(PCMFile *PCM, BitBuffer *BitB);
    
    uint32_t **PCM_ExtractSamples(PCMFile *PCM, BitBuffer *SampleArray, uint64_t NumSamples2Extract);
    
    bool       IsThereMoreMetadata(PCMFile *PCM);
    
    void       PCMSetOutputFileType(PCMFile *PCM, libPCMFileFormats OutputFileType);
    
    void       PCMSetOutputPXMType(PCMFile *PCM, PXMTypes PXMType);
    
    void       PCMSetNumOutputSamples(PCMFile *PCM, uint64_t NumChannelIndependentSamples);
    
    uint8_t    PCMGetBitDepth(PCMFile *PCM);
    
    uint64_t   PCMGetNumChannels(PCMFile *PCM);
    
    uint64_t   PCMGetNumSamples(PCMFile *PCM);
    
    void       PCMFileDeinit(PCMFile *PCM);
    
#ifdef __cplusplus
}
#endif

#endif /* LIBPCM_libPCM_H */
