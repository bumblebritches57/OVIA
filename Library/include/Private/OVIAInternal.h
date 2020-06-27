#include "../../../Dependencies/FoundationIO/Library/include/UnicodeIO/UnicodeIOTypes.h"

#pragma once

#ifndef OVIA_OVIAInternal_H
#define OVIA_OVIAInternal_H

#if (PlatformIO_Language == PlatformIO_LanguageIsCXX)
extern "C" {
#endif
    
    typedef struct OVIA_MagicIDs {
        uint16_t NumMagicIDs;
        uint16_t MagicIDOffsetInBits;
        uint8_t  MagicIDSizeInBits;
        uint8_t *MagicIDNumber[];
    } OVIA_MagicIDs;
    
    typedef struct OVIA_Extensions {
        uint8_t NumExtensions;
        UTF32  *Extensions[];
    } OVIA_Extensions;
    
    typedef struct OVIA_MIMETypes {
        uint8_t NumMIMETypes;
        UTF32  *MIMETypes[];
    } OVIA_MIMETypes;
    
#if (PlatformIO_Language == PlatformIO_LanguageIsCXX)
}
#endif

#endif /* OVIA_OVIAInternal_H */
