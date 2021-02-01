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

#include "../../StreamIO.h"

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

    typedef struct RIFFOptions {
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
    } RIFFOptions;

    static const uint8_t WAVNULLBinaryGUID[] = {
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    };

    static const uint8_t W64_RIFF_GUIDString[] = {
        0x72,0x69,0x66,0x66,0x2D,0x2E,0x91,0x2D,0xCF,0x11,0x2D,0xA5,0xD6,0x2D,0x28,0xDB,0x04,0xC1,0x00,0x00,0x00
    };

    static const uint8_t W64_WAVE_GUIDString[] = {
        0x77,0x61,0x76,0x65,0x2D,0xF3,0xAC,0x2D,0xD3,0x11,0x2D,0x8C,0xD1,0x2D,0x00,0xC0,0x4F,0x8E,0xDB,0x8A,0x00
    };

    static const uint8_t W64_FMT_GUIDString[] = {
        0x66,0x6D,0x74,0x20,0x2D,0xF3,0xAC,0x2D,0xD3,0x11,0x2D,0x8C,0xD1,0x2D,0x00,0xC0,0x4F,0x8E,0xDB,0x8A,0x00
    };

    static const uint8_t W64_DATA_GUIDString[] = {
        0x64,0x61,0x74,0x61,0x2D,0xF3,0xAC,0x2D,0xD3,0x11,0x2D,0x8C,0xD1,0x2D,0x00,0xC0,0x4F,0x8E,0xDB,0x8A,0x00
    };

    static const uint8_t W64_LEVL_GUIDString[] = { // aka Peak Envelope Chunk
        0x6A,0x75,0x6E,0x6B,0x2D,0xF3,0xAC,0x2D,0xD3,0x11,0x2D,0x8C,0xD1,0x2D,0x00,0xC0,0x4F,0x8E,0xDB,0x8A,0x00
    };

    static const uint8_t W64_BEXT_GUIDString[] = {
        0x62,0x65,0x78,0x74,0x2D,0xf3,0xAC,0x2D,0xD3,0xAA,0x2D,0xD1,0x8C,0x2D,0x00,0xC0,0x4F,0x8E,0xDB,0x8A,0x00
    };

    static const uint8_t W64_FACT_GUIDString[] = {
        0x66,0x61,0x63,0x74,0x2D,0xF3,0xAC,0x2D,0xD3,0x11,0x2D,0x8C,0xD1,0x2D,0x00,0xC0,0x4F,0x8E,0xDB,0x8A,0x00
    };

    static const uint8_t W64_JUNK_GUIDString[] = {
        0x6B,0x6E,0x75,0x6A,0x2D,0xAC,0xF3,0x2D,0x11,0xD3,0x2D,0x8C,0xD1,0x2D,0x00,0xC0,0x4F,0x8E,0xDB,0x8A,0x00
    };

    static const uint8_t W64_MRKR_GUIDString[] = { // MARKER
        0xAB,0xF7,0x62,0x56,0x2D,0x39,0x45,0x2D,0x11,0xD2,0x2D,0x86,0xC7,0x2D,0x00,0xC0,0x4F,0x8E,0xDB,0x8A,0x00
    };

    static const uint8_t W64_SUMM_GUIDString[] = { // SUMMARY LIST
        0x92,0x5F,0x94,0xBC,0x2D,0x52,0x5A,0x2D,0x11,0xD2,0x2D,0x86,0xDC,0x2D,0x00,0xC0,0x4F,0x8E,0xDB,0x8A,0x00
    };

    static const uint8_t W64_LIST_GUIDString[] = {
        0x74,0x73,0x69,0x6C,0x2D,0x91,0x2F,0x2D,0x11,0xCF,0x2D,0xA5,0xD6,0x2D,0x28,0xDB,0x04,0xC1,0x00,0x00,0x00
    };

    uint64_t RIFF_CalculateByteRate(uint64_t NumChannels, uint8_t BitDepth, uint64_t SampleRate);

    uint64_t RIFF_CalculateBlockAlignment(uint64_t NumChannels, uint8_t BitDepth);

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
    
#ifdef OVIA_StreamIO_RIFF
    extern const OVIA_MagicIDs RIFFSignatures;
    
    const OVIA_MagicIDs RIFFSignatures = {
        .NumMagicIDs          = 3,
        .MagicIDs             = {
            [0]               = {
                .OffsetInBits = 0,
                .SizeInBits   = 32,
                .Signature    = (uint8_t[4]){0x52, 0x49, 0x46, 0x46}, // RIFF
            },
            [1]               = {
                .OffsetInBits = 0,
                .SizeInBits   = 32,
                .Signature    = (uint8_t[4]){0x42, 0x57, 0x36, 0x34}, // BW64
            },
            [2]               = {
                .OffsetInBits = 0,
                .SizeInBits   = 128,
                .Signature    = (uint8_t[16]){0x66, 0x66, 0x69, 0x72, 0x91, 0x2E, 0x11, 0xCF, 0xA5, 0xD6, 0x28, 0xDB, 0x04, 0xC1, 0x00, 0x00},
            },
        },
    };

    extern const OVIA_Extensions RIFFExtensions;
    
    const OVIA_Extensions RIFFExtensions = {
        .NumExtensions     = 4,
        .Extensions        = {
            [0]            = {
                .Size      = 4,
                .Extension = UTF32String("wave"),
            },
            [1]            = {
                .Size      = 3,
                .Extension = UTF32String("wav"),
            },
            [2]            = {
                .Size      = 3,
                .Extension = UTF32String("bwf"),
            },
            [3]            = {
                .Size      = 3,
                .Extension = UTF32String("w64"),
            }
        },
    };
    
    extern const OVIA_MIMETypes RIFFMIMETypes;
    
    const OVIA_MIMETypes RIFFMIMETypes = {
        .NumMIMETypes     = 4,
        .MIMETypes        = {
            [0]           = {
                .Size     = 9,
                .MIMEType = UTF32String("audio/wav"),
            },
            [1]           = {
                .Size     = 10,
                .MIMEType = UTF32String("audio/wave"),
            },
            [2]           = {
                .Size     = 11,
                .MIMEType = UTF32String("audio/x-wav"),
            },
            [3]           = {
                .Size     = 14,
                .MIMEType = UTF32String("audio/vnd.wave"),
            },
        },
    };
    
#if defined(OVIA_StreamIO_Encode)
    extern const OVIA_Stream RIFFEncoder;
    
    const OVIA_Stream RIFFEncoder = {
        .MagicID = &RIFFSignatures,
    };
#endif /* OVIA_StreamIO_Encode */
    
#if defined(OVIA_StreamIO_Decode)
    extern const OVIA_Stream RIFFDecoder;
    
    const OVIA_Stream RIFFDecoder = {
        .MagicID = &RIFFSignatures,
    };
#endif /* OVIA_StreamIO_Decode */
    
#endif /* RIFF Literals */

#if (PlatformIO_Language == PlatformIO_LanguageIsCXX)
}
#endif

#endif /* OVIA_StreamIO_RIFFStream_H */

