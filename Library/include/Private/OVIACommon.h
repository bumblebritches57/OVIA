#include "../OVIA.h"

#include "../../../Dependencies/FoundationIO/libFoundationIO/include/BitIO.h"
#include "../../../Dependencies/FoundationIO/libFoundationIO/include/ContainerIO.h"
#include "../../../Dependencies/FoundationIO/libFoundationIO/include/CryptographyIO.h"
#include "../../../Dependencies/FoundationIO/libFoundationIO/include/Log.h"
#include "../../../Dependencies/FoundationIO/libFoundationIO/include/Math.h"
#include "../../../Dependencies/FoundationIO/libFoundationIO/include/StringIO.h"

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
        CodecType_Unknown = 0,
        CodecType_Decode  = 1,
        CodecType_Encode  = 2,
    } OVIA_CodecTypes;
    
    typedef struct AIFOptions AIFOptions;
    
    typedef struct OVIADemuxer {
        void            (*Function_ParseChunks)(void*, BitBuffer*); // Void* = PNGOptions*
    } OVIADemuxer;
    
    typedef struct OVIADecoder {
        void *              (*Function_Initialize)(void);
        void                (*Function_Parse)(void*, BitBuffer*); // Takes the Init type as a parameter
        void *              (*Function_Decode)(void*, BitBuffer*); // Returns a Container pointer
        void                (*Function_Deinitialize)(void*);
        uint8_t               MagicIDSize;
        uint8_t               MagicIDOffset;
        OVIA_MediaTypes       MediaType;
        OVIA_CodecIDs         DecoderID;
        const uint8_t        *MagicID;
    } OVIADecoder;
    
    typedef struct OVIAMuxer {
        void            (*Function_WriteChunks)(void*, BitBuffer*);
    } OVIAMuxer;
    
    typedef struct OVIAEncoder {
        void *           (*Function_Initialize)(void);
        void             (*Function_WriteHeader)(void*,BitBuffer*);
        void             (*Function_Encode)(void*,BitBuffer*);
        void             (*Function_WriteFooter)(BitBuffer*);
        void             (*Function_Deinitialize)(void*);
        OVIA_CodecIDs      EncoderID;
        OVIA_MediaTypes    MediaType;
        // How do we identify the encoder to choose? Maybe this should be an enum with a mapping function that maps all known codec names for example JPG, JPEG, JPE, JLS, JPEG-LS, JPEG-Lossless, LosslessJPEG to the CodecID
    } OVIAEncoder;
    
    typedef struct OVIACodecs {
        OVIAEncoder  Encoders[OVIA_NumCodecs];
        OVIADecoder  Decoders[OVIA_NumCodecs];
        OVIADemuxer  Demuxers[OVIA_NumCodecs];
        OVIAMuxer    Muxers[OVIA_NumCodecs];
    } OVIACodecs;
    
    extern OVIACodecs GlobalCodecs;
    
    extern OVIADecoder GlobalDecoders[OVIA_NumCodecs];
    
    extern OVIAEncoder GlobalEncoders[OVIA_NumCodecs];
    
#ifdef __cplusplus
}
#endif

#endif /* OVIA_OVIACommon_H */
