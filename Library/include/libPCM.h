#include "../../Dependencies/BitIO/libBitIO/include/BitIO.h"

#pragma once

#ifndef LIBPCM_H
#define LIBPCM_H

#ifdef __cplusplus
extern "C" {
#endif
    
    enum libPCMConstants {
        WAVMagic            = 0x52494646,
        W64Magic            = 0x72696666,
        AIFMagic            = 0x464F524D,
        PCMMaxChannels      = 8,
        PCMMaxSampleRate    = 192000,
        W64HeaderSize       = 104,
        WAVFormatExtensible = 0xFFFE,
    };

    enum SpeakerMask {
        FrontRight          = 0x1,
        FrontLeft           = 0x2,
        FrontCenter         = 0x4,
        LFE                 = 0x8,
        BackLeft            = 0x10,
        BackRight           = 0x20,// yes
        FrontLeftCenter     = 0x40,// yes
        FrontRightCenter    = 0x80,
        BackCenter          = 0x100,
        SideLeft            = 0x200,
        SideRight           = 0x400,
        TopCenter           = 0x800,
        TopFrontLeft        = 0x1000,
        TopFrontCenter      = 0x2000,
        TopFrontRight       = 0x4000,// yes
        TopBackLeft         = 0x8000,
        TopBackCenter       = 0x10000,
        TopBackRight        = 0x20000,
    };

    enum PCMFileTypes {
        Unknown_Type        = 0,
        WAV_File            = 1,
        RF64_File           = 2,
        W64_File            = 3,
        AIFF_File           = 4,
    };
    
    typedef struct W64Header W64Header;
    
    typedef struct WAVHeader WAVHeader;
    
    typedef struct AIFHeader AIFHeader;
    
    typedef struct PCMFile PCMFile;
    
    typedef struct PCMMetadata PCMMetadata;
    
    // So if it's a WAV file the input file metadata reader thing will set it to WAV, then read the meta into Meta
    // Then once it comes across the equilivent of a data chunk, it will set Data to the pointer, then for extracting
    // samples it will read the type call the correct reader and extract the requested samples.
    
    void     IdentifyPCMFile(BitInput *BitI, PCMFile *PCM);
    void     ParseWAVFile(BitInput *BitI, PCMFile *PCM);
    void     ParseW64File(BitInput *BitI, W64Header *W64);
    PCMFile *InitPCMFile(void);
    void     ClosePCMFile(PCMFile *PCM);
    
#ifdef __cplusplus
}
#endif

#endif /* LIBPCM_H */
