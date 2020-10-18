/*!
 @header              RIFFStream.h
 @author              Marcus Johnson
 @copyright           2020+
 @version             1.0.0
 @brief               This header contains code for RIFF streams.
 @todo                WAVStream -> RIFFStream; handles RIFF, BWF, RIFF64 or whatever the GUID one is called.
 Has Substreams.
 is multiplexible
 */

#include "StreamIO.h"

#pragma once

#ifndef OVIA_StreamIO_RIFFStream_H
#define OVIA_StreamIO_RIFFStream_H

#if (PlatformIO_Language == PlatformIO_LanguageIsCXX)
extern "C" {
#endif

    typedef enum WAVSpeakerMask {
        WAVMask_FrontRight         = 0x1,
        WAVMask_FrontLeft          = 0x2,
        WAVMask_FrontCenter        = 0x4,
        WAVMask_LFE                = 0x8,
        WAVMask_BackLeft           = 0x10,
        WAVMask_BackRight          = 0x20,
        WAVMask_FrontLeftCenter    = 0x40,
        WAVMask_FrontRightCenter   = 0x80,
        WAVMask_BackCenter         = 0x100,
        WAVMask_SideLeft           = 0x200,
        WAVMask_SideRight          = 0x400,
        WAVMask_TopCenter          = 0x800,
        WAVMask_TopFrontLeft       = 0x1000,
        WAVMask_TopFrontCenter     = 0x2000,
        WAVMask_TopFrontRight      = 0x4000,
        WAVMask_TopBackLeft        = 0x8000,
        WAVMask_TopBackCenter      = 0x10000,
        WAVMask_TopBackRight       = 0x20000,
    } WAVSpeakerMask;

    typedef enum WAVChunkIDs { // odd sized chunks have a trailing 0 byte, and strings are null padded, this is for WAV, and W64.
        WAVChunkID_Unspecified = 0,
        WAVChunkID_RIFF        = 0x52494646,
        WAVChunkID_BW64        = 0x42573634, // Literally the same as RIFF, just diff chunk id
        WAVChunkID_DS64        = 0x64733634,
        WAVChunkID_WAVE        = 0x57415645,
        WAVChunkID_FMT         = 0x666D7420,
        WAVChunkID_LIST        = 0x4C495354,
        WAVChunkID_INFO        = 0x494E464F,
        WAVChunkID_DATA        = 0x64617461,
        WAVChunkID_JUNK        = 0,
        WAVChunkID_IART        = 0x49415254, // Artist
        WAVChunkID_ICRD        = 0x49435244, // Release data
        WAVChunkID_IGNR        = 0x49474E52, // Genre
        WAVChunkID_INAM        = 0x494E414D, // Song name aka title
        WAVChunkID_IPRD        = 0x49505244, // Album name
        WAVChunkID_IPRT        = 0x49505254, // Track number
        WAVChunkID_ISFT        = 0x49534654, // Software that created it?
        WAVChunkID_CODE        = 0x434f4445, // Encoder
        WAVChunkID_ICMT        = 0x49434d54, // Comment
        WAVChunkID_ICOP        = 0x49434f50, // Copyright
        WAVChunkID_YEAR        = 0x59454152, // Year
        WAVChunkID_TRCK        = 0x5452434b, // Track
    } WAVChunkIDs;
    
    typedef struct WAVInfo {
        UTF8 *Title;
        UTF8 *Artist;
        UTF8 *Album;
        UTF8 *ReleaseDate;
        UTF8 *Genre;
        UTF8 *CreationSoftware;
    } WAVInfo;

    typedef struct BEXTChunk {
        UTF8 *Description;
        UTF8 *Originator;
        UTF8 *OriginatorRef;
        UTF8 *OriginatorDate;
    } BEXTChunk;

    typedef struct WAVOptions {
        WAVInfo   *Info;
        BEXTChunk *BEXT;
        uint64_t   NumSamplesWritten;
        uint64_t   SampleRate;
        uint64_t   ByteRate;
        uint64_t   BlockAlign;
        uint64_t   SpeakerMask;
        uint64_t   ChannelMask;
        uint16_t   CompressionFormat;
        uint16_t   BlockAlignment;
        uint16_t   NumChannels;
        uint16_t   BitDepth;
        uint16_t   ValidBitsPerSample;
    } WAVOptions;

    /*
     StreamCapabilities:
     Audio only, ImageOnly, VideoOnly actually make it ORable
     StreamLimitations: StreamSizeLimitation: numbytes
     We also need substream registration, like RIFF can hold WAV chunks, AVI chunks, WebP chunks, etc.


     */

    /*
     So what is the architecture?

     A Stream is the physical data format that can contain everything.
     A Stream has one or more Substreams that contain the actual encoded data.

     RIFFStream = RIFF/RF64?

     W64Stream = W64
     */

#if (PlatformIO_Language == PlatformIO_LanguageIsCXX)
}
#endif

#endif /* OVIA_StreamIO_RIFFStream_H */

