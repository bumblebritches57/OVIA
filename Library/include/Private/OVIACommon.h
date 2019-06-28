#include "../OVIA.h"

#include "../../../Dependencies/FoundationIO/libFoundationIO/include/BitIO.h"
#include "../../../Dependencies/FoundationIO/libFoundationIO/include/ContainerIO.h"
#include "../../../Dependencies/FoundationIO/libFoundationIO/include/CryptographyIO.h"
#include "../../../Dependencies/FoundationIO/libFoundationIO/include/Log.h"
#include "../../../Dependencies/FoundationIO/libFoundationIO/include/Math.h"

#pragma once

#ifndef OVIA_OVIACommon_H
#define OVIA_OVIACommon_H

#ifdef __cplusplus
extern "C" {
#endif
    
    typedef enum OVIA_CodecIDs {
        CodecID_Unknown       = 0,
        CodecID_AIF           = 1,
        CodecID_WAV           = 2,
        CodecID_W64           = 3,
        CodecID_FLAC          = 4,
        CodecID_BMP           = 5,
        CodecID_PBM_A         = 6,
        CodecID_PBM_B         = 7,
        CodecID_PGM_A         = 8,
        CodecID_PGM_B         = 9,
        CodecID_PPM_A         = 10,
        CodecID_PPM_B         = 11,
        CodecID_PAM           = 12,
        CodecID_PNG           = 13,
        OVIA_NumCodecs        = CodecID_PAM,
    } OVIA_CodecIDs;
    
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
    
    typedef struct OVIADecoder {
        void            *(*Function_Initialize)(void);
        void             (*Function_Parse)(AIFOptions*, BitBuffer*); // Takes the Init type as a parameter
        void            *(*Function_Decode)(BitBuffer*); // Returns a Container pointer
        void             (*Function_Deinitialize)(void*);
        uint8_t            MagicIDSize;
        uint8_t            MagicIDOffset;
        OVIA_MediaTypes    MediaType;
        OVIA_CodecIDs      DecoderID;
        uint8_t            MagicID[];
    } OVIADecoder;
    
    typedef struct OVIAEncoder {
        void            *(*Function_Initialize)(void);
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
    } OVIACodecs;
    
    extern OVIACodecs GlobalCodecs;
    
    void OVIA_RegisterDecoder(OVIADecoder Decoder);
    
    void OVIA_RegisterEncoder(OVIAEncoder Encoder);
    
    /*
    typedef struct OVIADecoder OVIADecoder;
    
    typedef struct OVIAEncoder OVIAEncoder;
    
    //typedef struct OVIACodecs OVIACodecs;
    
    
     */
    
#ifdef __cplusplus
}
#endif

#endif /* OVIA_OVIACommon_H */
