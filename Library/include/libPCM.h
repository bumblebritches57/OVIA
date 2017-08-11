#include <stdbool.h>
#include <stdint.h>

#include "../../Dependencies/BitIO/libBitIO/include/BitIO.h"

/*
 We should add support for BMP and PXM images as well.
 */

#pragma once

#ifndef LIBPCM_libPCM_H
#define LIBPCM_libPCM_H

#ifdef __cplusplus
extern "C" {
#endif
    
    typedef struct PCMMetadata PCMMetadata;
    
    typedef struct PCMFile PCMFile;
    
    enum libPCMFileFormats {
        UnknownFormat       = 0,
        AIFFormat           = 1,
        WAVFormat           = 2,
        W64Format           = 3,
        BMPFormat           = 4,
        PXMFormat           = 5,
    };
    
    enum AIFSpeakerMask {
        AIFFrontLeft        = 0x1,
        AIFFrontRight       = 0x2,
        AIFFrontCenter      = 0x4,
        AIFRearLeft         = 0x8,
        AIFRearRight        = 0x10,
        AIFSurround         = 0x20,
        AIFLeftCenter       = 0x40,
        AIFRightCenter      = 0x80,
    };
    
    enum WAVSpeakerMask {
        WAVFrontRight       = 0x1,
        WAVFrontLeft        = 0x2,
        WAVFrontCenter      = 0x4,
        WAVLFE              = 0x8,
        WAVBackLeft         = 0x10,
        WAVBackRight        = 0x20,
        WAVFrontLeftCenter  = 0x40,
        WAVFrontRightCenter = 0x80,
        WAVBackCenter       = 0x100,
        WAVSideLeft         = 0x200,
        WAVSideRight        = 0x400,
        WAVTopCenter        = 0x800,
        WAVTopFrontLeft     = 0x1000,
        WAVTopFrontCenter   = 0x2000,
        WAVTopFrontRight    = 0x4000,
        WAVTopBackLeft      = 0x8000,
        WAVTopBackCenter    = 0x10000,
        WAVTopBackRight     = 0x20000,
    };
    
    enum AIFChunkIDs {
        AIF_FORM            = 0x464F524D,
        AIF_AIFF            = 0x41494646,
        AIF_AIFC            = 0x41494643,
        AIF_NAME            = 0x4E414D45,
        AIF_COMM            = 0x434F4D4D,
        AIF_SSND            = 0x53534E44,
    };
    
    enum WAVChunkIDs { // odd sized chunks have a trailing 0 byte, and strings are null padded
        WAV_RIFF            = 0x52494646,
        WAV_WAVE            = 0x57415645,
        WAV_FMT             = 0x666D7420,
        WAV_LIST            = 0x4C495354,
        WAV_INFO            = 0x494E464F,
        WAV_IART            = 0x49415254, // Artist
        WAV_ICRD            = 0x49435244, // Release data
        WAV_IGNR            = 0x49474E52, // Genre
        WAV_INAM            = 0x494E414D, // Song name aka title
        WAV_IPRD            = 0x49505244, // Album name
        WAV_IPRT            = 0x49505254, // Track number
        WAV_ISFT            = 0x49534654, // Software that created it?
        WAV_CODE            = 0x434f4445, // Encoder
        WAV_ICMT            = 0x49434d54, // Comment
        WAV_ICOP            = 0x49434f50, // Copyright
        WAV_YEAR            = 0x59454152, // Year
        WAV_TRCK            = 0x5452434b, // Track
        WAV_DATA            = 0x64617461,
    };
    
    enum BMPMagic {
        BMP_BM              = 0x424D,
    };
    
    enum PXMMagic {
        PXM_P1              = 0x5031,
        PXM_P2              = 0x5032,
        PXM_P3              = 0x5033,
        PXM_P4              = 0x5034,
        PXM_P5              = 0x5035,
        PXM_P6              = 0x5036,
        PXM_P7              = 0x5037,
    };
    
    const uint8_t W64_RIFF[16] = {0x72, 0x69, 0x66, 0x66, 0x2E, 0x91, 0xCF, 0x11, 0xA5, 0xD6, 0x28, 0xDB, 0x04, 0xC1, 0x00, 0x00};
    const uint8_t W64_WAVE[16] = {0x77, 0x61, 0x76, 0x65, 0xF3, 0xAC, 0xD3, 0x11, 0x8C, 0xD1, 0x00, 0xC0, 0x4F, 0x8E, 0xDB, 0x8A};
    const uint8_t W64_FMT [16] = {0x66, 0x6D, 0x74, 0x20, 0xF3, 0xAC, 0xD3, 0x11, 0x8C, 0xD1, 0x00, 0xC0, 0x4F, 0x8E, 0xDB, 0x8A};
    const uint8_t W64_DATA[16] = {0x64, 0x61, 0x74, 0x61, 0xF3, 0xAC, 0xD3, 0x11, 0x8C, 0xD1, 0x00, 0xC0, 0x4F, 0x8E, 0xDB, 0x8A};
    const uint8_t W64_LEVL[16] = {0x6A, 0x75, 0x6E, 0x6B, 0xF3, 0xAC, 0xD3, 0x11, 0x8C, 0xD1, 0x00, 0xC0, 0x4F, 0x8E, 0xDB, 0x8A};
    const uint8_t W64_BEXT[16] = {0x62, 0x65, 0x78, 0x74, 0xf3, 0xAC, 0xD3, 0xAA, 0xD1, 0x8C, 0x00, 0xC0, 0x4F, 0x8E, 0xDB, 0x8A};
    const uint8_t W64_FACT[16] = {0x66, 0x61, 0x63, 0x74, 0xF3, 0xAC, 0xD3, 0x11, 0x8C, 0xD1, 0x00, 0xC0, 0x4F, 0x8E, 0xDB, 0x8A};
    const uint8_t W64_JUNK[16] = {0x6B, 0x6E, 0x75, 0x6A, 0xAC, 0xF3, 0x11, 0xD3, 0x8C, 0xD1, 0x00, 0xC0, 0x4F, 0x8E, 0xDB, 0x8A};
    const uint8_t W64_MRKR[16] = {0xAB, 0xF7, 0x62, 0x56, 0x39, 0x2D, 0x11, 0xD2, 0x86, 0xC7, 0x00, 0xC0, 0x4F, 0x8E, 0xDB, 0x8A};
    const uint8_t W64_SUMM[16] = {0x92, 0x5F, 0x94, 0xBC, 0x52, 0x5A, 0x11, 0xD2, 0x86, 0xDC, 0x00, 0xC0, 0x4F, 0x8E, 0xDB, 0x8A};
    const uint8_t W64_LIST[16] = {0x74, 0x73, 0x69, 0x6C, 0x91, 0x2F, 0x11, 0xCF, 0xA5, 0xD6, 0x28, 0xDB, 0x04, 0xC1, 0x00, 0x00};
    
    PCMFile *InitPCMFile(void);
    
    void     IdentifyPCMFile(PCMFile *PCM, BitBuffer *BitB);
    
    void     ParsePCMMetadata(PCMFile *PCM, BitBuffer *BitB);
    
    void     ExtractPCMSamples(PCMFile *PCM, BitBuffer *BitB, uint64_t Samples2Extract);
    
    void     DeinitPCMFile(PCMFile *PCM);
    
#ifdef __cplusplus
}
#endif

#endif /* LIBPCM_libPCM_H */
