#include <stdlib.h>

#include "../OVIA.h"

#include "../../../Dependencies/FoundationIO/libFoundationIO/include/BitIO.h"
#include "../../../Dependencies/FoundationIO/libFoundationIO/include/ContainerIO.h"
#include "../../../Dependencies/FoundationIO/libFoundationIO/include/Math.h"
#include "../../../Dependencies/FoundationIO/libFoundationIO/include/StringIO.h"
#include "../../../Dependencies/FoundationIO/libFoundationIO/include/Log.h"

#pragma once

#ifndef OVIA_InternalOVIA_H
#define OVIA_InternalOVIA_H

#ifdef __cplusplus
extern "C" {
#endif
    /*
    typedef struct OVIACodecs {
        uint64_t          NumMagicIDs[OVIA_NumCodecs];
        uint8_t          *MagicID[OVIA_NumCodecs];
        uint64_t         *MagicIDSize[OVIA_NumCodecs];
        uint64_t         *MagicIDOffset[OVIA_NumCodecs];
        OVIA_MediaTypes   MediaTypes[OVIA_NumCodecs];
        OVIA_CodecTypes   CodecTypes[OVIA_NumCodecs];
        void            (*InitFunction[OVIA_NumCodecs])(void);
        void            (*ParseFunction[OVIA_NumCodecs])(OVIA*, BitBuffer*);
        Audio2DContainer *(*DecodeAudio2D[OVIA_NumCodecs])(OVIA*, BitBuffer*);
        void            (*EncodeAudio2D[OVIA_NumCodecs])(Audio2DContainer*, BitBuffer*);
        ImageContainer *(*DecodeImage[OVIA_NumCodecs])(OVIA*, BitBuffer*);
        void            (*EncodeImage[OVIA_NumCodecs])(ImageContainer*, BitBuffer*);
    } OVIACodecs;
     */
    
    typedef struct OVIADecoder {
        uint64_t            NumMagicIDs;
        uint64_t           *MagicIDSize;   // array containing the size of each magic id
        uint64_t           *MagicIDOffset; // array containing the offset in bytes for each magic id
        uint8_t           **MagicIDs;      // array of pointers to arrays of bytes one for each MagicID
        OVIA_MediaTypes     MediaType;
        void             *(*Function_Init)(void); // Void pointer to each codecs init function
        void              (*Function_Parse)(BitBuffer*);
        Audio2DContainer *(*Function_ReadAudio2D)(BitBuffer*);
        Audio3DContainer *(*Function_ReadAudio3D)(BitBuffer*);
        ImageContainer   *(*Function_ReadImage)(BitBuffer*);
    } OVIADecoder;
    
    typedef struct OVIAEncoder {
        OVIA_MediaTypes     MediaType;
        void             *(*Function_Init)(void); // Void pointer to each codecs init function
        void              (*Function_WriteHeader)(BitBuffer*);
        Audio2DContainer *(*Function_WriteAudio2D)(BitBuffer*);
        Audio3DContainer *(*Function_WriteAudio3D)(BitBuffer*);
        ImageContainer   *(*Function_WriteImage)(BitBuffer*);
    } OVIAEncoder;
    
    typedef struct OVIACodecs {
        uint64_t     NumCodecs;
        OVIAEncoder *Encoders;
        OVIADecoder *Decoders;
    } OVIACodecs;
    
#ifdef __cplusplus
}
#endif

#endif /* OVIA_libOVIA_H */
