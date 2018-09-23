#include "../../libOVIA.h"
#include "../InternalOVIA.h"

// PGM, PBM, PPM, along with PAM.

#pragma once

#ifndef OVIA_PNMCommon_H
#define OVIA_PNMCommon_H

#ifdef __cplusplus
extern "C" {
#endif
    
    enum PNMCommon {
        PNMMagicSize          = 2,
        PNMCommentStart       = 0x23,
        PNMFieldSeperator     = 0x20,
        PNMEndField           = 0x0A,
    };
    
    enum PNMMagic {
        PNM_PBMA = 0x5031, // PortableBitMapASCII
        PNM_PBMB = 0x5034, // PortableBitMapBinary
        PNM_PGMA = 0x5032, // PortableGrayMapASCII
        PNM_PGMB = 0x5035, // PortableGrayMapBinary
        PNM_PPMA = 0x5033, // PortablePixMapASCII
        PNM_PPMB = 0x5036, // PortablePixMapBinary
        PNM_PAMB = 0x5037, // PortableAnyMap
    };
    
    void        PNMIdentifyFileType(OVIA *Ovia, BitBuffer *BitB);
    
    void        PNMParseMetadata(OVIA *Ovia, BitBuffer *BitB);
    
    ImageContainer *PNMExtractImage(OVIA *Ovia, BitBuffer *BitB);
    
    void PNMInsertFrame(OVIA *Ovia, ImageContainer *Image, BitBuffer *BitB);
    
    void PNMWriteHeader(OVIA *Ovia, BitBuffer *BitB);
    
#ifdef __cplusplus
}
#endif

#endif /* OVIA_PNMCommon_H */
