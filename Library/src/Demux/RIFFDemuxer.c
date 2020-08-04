#include "../../include/Private/OVIACommon.h"

#if (PlatformIO_Language == PlatformIO_LanguageIsCXX)
extern "C" {
#endif
    
    uint32_t GetRIFFSize(BitBuffer *BitB) {
        uint32_t Size = 0;
        if (BitB != NULL) {
            Size      = BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 32);
        } else {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
        return Size;
    }
    /*
    void GetRIFFType(BitBuffer *BitB) {
        if (BitB != NULL) {
            Size      = (uint32_t) BitBuffer_ReadBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 32);
        } else {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    */

    static const OVIA_MagicIDs RIFFMagicIDs = {
        .NumMagicIDs         = 1,
        .MagicIDOffsetInBits = 0,
        .MagicIDSizeInBits   = 32,
        .MagicIDNumber = {
            [0] = (uint8_t[4]){0x52, 0x49, 0x46, 0x46},
        },
    };
#if (PlatformIO_Language == PlatformIO_LanguageIsCXX)
}
#endif
