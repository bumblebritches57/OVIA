/*!
 @header              OVIACommon.h
 @author              Marcus Johnson
 @copyright           2016+
 @version             1.0.0
 @brief               This header contains code for reading and writing lossless PNM image files
 */

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
    
    typedef struct OVIADecoder {
        void *           (**Function_Initialize)(void);
        void *           (**Function_Decode)(void*, BitBuffer*); // Returns a Container pointer, takes Options and BitBuffer pointer
        void             (**Function_Read)(void*, BitBuffer*); // Takes the Init type as a parameter
        void             (**Function_Deinitialize)(void*);
        uint8_t           **MagicID;
        uint64_t           *MagicIDSizeInBits;
        uint64_t           *MagicIDOffsetInBits;
        uint64_t            NumMagicIDs;
        OVIA_MediaTypes     MediaType;
        OVIA_CodecIDs       DecoderID;
    } OVIADecoder;
    
    typedef struct OVIAEncoder {
        void *           (**Function_Initialize)(void);
        void             (**Function_WriteHeader)(void *Options, BitBuffer *BitB);
        void             (**Function_Encode)(void *Options, void *Contanier, BitBuffer *BitB); // Image*/Audio*, BitBuffer
        void             (**Function_WriteFooter)(void *Options, BitBuffer *BitB);
        void             (**Function_Deinitialize)(void *Options);
        OVIA_CodecIDs       EncoderID;
        OVIA_MediaTypes     MediaType;
        // How do we identify the encoder to choose? Maybe this should be an enum with a mapping function that maps all known codec names for example JPG, JPEG, JPE, JLS, JPEG-LS, JPEG-Lossless, LosslessJPEG to the CodecID
    } OVIAEncoder;
    
    typedef struct OVIAColorTransform {
        void                 (*Function_Transform)(ImageContainer*);
        ImageChannelMask      InputChannels;
        ImageChannelMask      OutputChannels;
        OVIA_ColorTransforms  Transform;
        uint8_t               NumInputChannels;
        uint8_t               NumOutputChannels;
    } OVIAColorTransform;
    
    typedef struct OVIA {
        OVIAEncoder          *Encoders;
        OVIADecoder          *Decoders;
        OVIAColorTransform   *ForwardTransforms;
        OVIAColorTransform   *ReverseTransforms;
        uint64_t              NumEncoders;
        uint64_t              NumDecoders;
        uint64_t              NumForwardTransforms;
        uint64_t              NumReverseTransforms;
    } OVIA;
    
    void OVIA_RegisterFunction_Decode(void (*Function_RegisterDecoder)(OVIA*, OVIADecoder*), OVIA *Ovia, OVIADecoder *Decoder);
    
    typedef struct OVIACodecs {
        OVIAEncoder  Encoders[OVIA_NumCodecs];
        OVIADecoder  Decoders[OVIA_NumCodecs];
    } OVIACodecs;
    
    extern OVIACodecs *GlobalCodecs;
    
    
    typedef struct OVIACodecRegistry {
        // Contains a list of functions to register the decoders and encoders
        // aka An array of function pointers
        // The registration functions return nothing, and take in as parameters the OVIA pointer (which may or may not be global, as well as a void pointer to the OVIADecoder/Encoder)
        
        void (*Function_RegisterDecoder[OVIA_NumCodecs])(OVIA *);
        void (*Function_RegisterEncoder[OVIA_NumCodecs])(OVIA *);
        void (*Function_ForwardTransform[OVIA_NumTransforms])(OVIA *);
    } OVIACodecRegistry;
    
#ifdef __cplusplus
}
#endif

#endif /* OVIA_OVIACommon_H */
