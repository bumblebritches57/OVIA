#include "../OVIA.h"

#include "../../../Dependencies/FoundationIO/Library/include/BitIO.h"
#include "../../../Dependencies/FoundationIO/Library/include/ContainerIO.h"
#include "../../../Dependencies/FoundationIO/Library/include/CryptographyIO.h"
#include "../../../Dependencies/FoundationIO/Library/include/MathIO.h"
#include "../../../Dependencies/FoundationIO/Library/include/UnicodeIO/LogIO.h"
#include "../../../Dependencies/FoundationIO/Library/include/UnicodeIO/StringIO.h"

#pragma once

#ifndef OVIA_OVIACommon_H
#define OVIA_OVIACommon_H

#ifdef __cplusplus
extern "C" {
#endif
    
    typedef enum OVIA_MediaTypes {
        MediaType_Unknown     = 0,
        MediaType_Audio2D     = 1,
        MediaType_Audio3D     = 2,
        MediaType_Image       = 3,
        MediaType_Video       = 4,
    } OVIA_MediaTypes;
    
    typedef enum OVIA_CodecTypes {
        CodecType_Unknown     = 0,
        CodecType_Decode      = 1,
        CodecType_Encode      = 2,
    } OVIA_CodecTypes;
    
    typedef struct OVIADemuxer {
        void              (*Function_ParseChunks)(void*, BitBuffer*); // Void* = PNGOptions*
    } OVIADemuxer;
    
    typedef struct OVIADecoder {
        void *           (**Function_Initialize)(void);
        void             (**Function_Parse)(void*, BitBuffer*); // Takes the Init type as a parameter
        void *           (**Function_Decode)(void*, BitBuffer*); // Returns a Container pointer
        void             (**Function_Deinitialize)(void*);
        uint8_t            *MagicID;
        uint64_t           *MagicIDSize;   // in Bits
        uint64_t           *MagicIDOffset; // in Bits
        uint64_t            NumMagicIDs;
        OVIA_MediaTypes     MediaType;
        OVIA_CodecIDs       DecoderID;
    } OVIADecoder;
    
    typedef struct OVIAEncoder {
        void *           (**Function_Initialize)(void);
        void             (**Function_WriteHeader)(void*, BitBuffer*);
        void             (**Function_Encode)(void*, BitBuffer*);
        void             (**Function_WriteFooter)(void*, BitBuffer*);
        void             (**Function_Deinitialize)(void*);
        OVIA_CodecIDs       EncoderID;
        OVIA_MediaTypes     MediaType;
        // How do we identify the encoder to choose? Maybe this should be an enum with a mapping function that maps all known codec names for example JPG, JPEG, JPE, JLS, JPEG-LS, JPEG-Lossless, LosslessJPEG to the CodecID
    } OVIAEncoder;
    
    typedef struct OVIA {
        uint64_t     NumEncoders;
        uint64_t     NumDecoders;
        OVIAEncoder *Encoders;
        OVIADecoder *Decoders;
    } OVIA;
    
    void OVIA_RegisterFunction_Decode(void (*Function_RegisterDecoder)(OVIA*, OVIADecoder*), OVIA *Ovia, OVIADecoder *Decoder);
    
    typedef struct OVIACodecs {
        OVIAEncoder  Encoders[OVIA_NumCodecs];
        OVIADecoder  Decoders[OVIA_NumCodecs];
        OVIADemuxer  Demuxers[OVIA_NumCodecs];
    } OVIACodecs;
    
    extern OVIACodecs *GlobalCodecs;
    
    
    typedef struct OVIACodecRegistry {
        // Contains a list of functions to register the decoders and encoders
        // aka An array of function pointers
        // The registration functions return nothing, and take in as parameters the OVIA pointer (which may or may not be global, as well as a void pointer to the OVIADecoder/Encoder)
        
        void (*Function_RegisterDecoder[OVIA_NumCodecs])(OVIA *);
        void (*Function_RegisterEncoder[OVIA_NumCodecs])(OVIA *);
    } OVIACodecRegistry;
    
#ifdef __cplusplus
}
#endif

#endif /* OVIA_OVIACommon_H */
