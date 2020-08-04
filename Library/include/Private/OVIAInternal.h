#include "../../../Dependencies/FoundationIO/Library/include/TextIO/TextIOTypes.h"

#pragma once

#ifndef OVIA_OVIAInternal_H
#define OVIA_OVIAInternal_H

#if (PlatformIO_Language == PlatformIO_LanguageIsCXX)
extern "C" {
#endif

    typedef struct OVIA_MagicIDs {
        const uint16_t NumMagicIDs;
        const uint16_t MagicIDOffsetInBits;
        const uint8_t  MagicIDSizeInBits;
        const uint8_t *MagicIDNumber[];
    } OVIA_MagicIDs;

    typedef struct OVIA_Extensions {
        const uint8_t  NumExtensions;
        const UTF32   *Extensions[];
    } OVIA_Extensions;

    typedef struct OVIA_MIMETypes {
        const uint8_t NumMIMETypes;
        const UTF32  *MIMETypes[];
    } OVIA_MIMETypes;
    
#if (PlatformIO_Language == PlatformIO_LanguageIsCXX)
}
#endif

#endif /* OVIA_OVIAInternal_H */
