#include "../include/Private/TransformIO.h"
#include "../include/Private/OVIACommon.h"
#include "../../Dependencies/FoundationIO/Library/include/TextIO/LogIO.h"
#include "../../Dependencies/FoundationIO/Library/include/MathIO.h"
#include "../../Dependencies/FoundationIO/Library/include/BufferIO.h"

#if (PlatformIO_Language == PlatformIO_LanguageIsCXX)
extern "C" {
#endif

    typedef struct OVIAPacket {
        uint8_t   *Buffer;
        uint64_t   SizeInBits;
    } OVIAPacket;

#if (PlatformIO_Language == PlatformIO_LanguageIsCXX)
}
#endif
