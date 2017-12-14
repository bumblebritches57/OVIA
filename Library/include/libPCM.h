#include <stdbool.h>
#include <stdint.h>

#pragma once

#ifndef LIBPCM_libPCM_H
#define LIBPCM_libPCM_H

#ifdef __cplusplus
extern "C" {
#endif
    
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
    
    typedef enum PXMTypes {
        UnknownPXM            = 0,
        BlackAndWhitePXM      = 1,
        BinaryPXM             = 2,
        ASCIIPXM              = 3,
        PAMPXM                = 4,
    } PXMTypes;
    
    static const char *libPCMLibraryName = "libPCM";
    
    typedef struct     PCMFile   PCMFile;
    
    typedef struct     BitBuffer BitBuffer;
    
    typedef struct     Samples   Samples;
    
    void               IFFSkipPadding(BitBuffer *BitB, uint32_t SubChunkSize);
    
    PCMFile           *PCMFile_Init(void);
    
    void               PCMFile_Identify(PCMFile *PCM, BitBuffer *BitB);
    
    void               PCMFile_ParseMetadata(PCMFile *PCM, BitBuffer *BitB);
    
    uint8_t            PCM_GetBitDepth(PCMFile *PCM);
    
    uint64_t           PCM_GetNumChannels(PCMFile *PCM);
    
    uint64_t           PCM_GetNumSamples(PCMFile *PCM);
    
    bool               PCM_IsThereMoreMetadata(PCMFile *PCM);
    
    void               PCM_SetNumOutputSamples(PCMFile *PCM, uint64_t NumChannelIndependentSamples);
    
    void               PCM_SetOutputPXMType(PCMFile *PCM, PXMTypes PXMType);
    
    void               PCM_SetNumOutputSamples(PCMFile *PCM, uint64_t NumChannelIndependentSamples);
    
    void               PCM_ExtractSamples(PCMFile *PCM, BitBuffer *SampleArray, uint64_t NumSamples2Extract, uint32_t **ExtractedSamples);
    
    void               PCM_ExtractPixels(PCMFile *PCM, BitBuffer *PixelArray, uint64_t NumPixels2Extract, uint16_t **ExtractedPixels);
    
    void               PCM_InsertSamples(PCMFile *PCM, BitBuffer *OutputSamples, uint32_t NumSamples2Write, uint32_t **Samples2Write);
    
    void               PCM_InsertPixels(PCMFile *PCM, BitBuffer *OutputPixels, uint32_t NumPixels2Write, uint16_t **Pixels2Write);
    
    void               PCM_SetOutputFileType(PCMFile *PCM, libPCMFileFormats OutputFileType);
    
    void               PCM_WriteHeader(PCMFile *PCM, BitBuffer *BitB);
    
    void               PCMFile_Deinit(PCMFile *PCM);
    
#ifdef __cplusplus
}
#endif

#endif /* LIBPCM_libPCM_H */
