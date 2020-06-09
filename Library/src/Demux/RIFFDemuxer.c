#include "../../include/Private/OVIACommon.h"

#if (PlatformIO_Language == PlatformIO_LanguageIsCXX)
extern "C" {
#endif
    
    uint32_t GetRIFFSize(BitBuffer *BitB) {
        uint32_t Size = 0;
        if (BitB != NULL) {
            Size      = BitBuffer_ReadBits(BitB, BitIO_ByteOrder_LSByte, BitIO_BitOrder_LSBit, 32);
        } else {
            Log(Severity_DEBUG, UnicodeIOTypes_FunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
        return Size;
    }
    /*
    void GetRIFFType(BitBuffer *BitB) {
        if (BitB != NULL) {
            Size      = (uint32_t) BitBuffer_ReadBits(BitB, BitIO_ByteOrder_LSByte, BitIO_BitOrder_LSBit, 32);
        } else {
            Log(Severity_DEBUG, UnicodeIOTypes_FunctionName, UTF8String("BitBuffer Pointer is NULL"));
        }
    }
    */
    static const MagicIDSizes RIFFMagicIDSize = {
        .NumSizes     = 1,
        .Sizes        = {
            [0]       = 4,
        },
    };
    
    static const MagicIDOffsets RIFFMagicIDOffset = {
        .NumOffsets   = 1,
        .Offsets      = {
            [0]       = 0,
        },
    };
    
    static const MagicIDNumbers RIFFMagicIDNumber = {
        .NumMagicIDs  = 1,
        .MagicNumbers = {
            [0]       = (uint8_t[4]){0x52, 0x49, 0x46, 0x46},
        },
    };
    
    static const MagicIDs RIFFMagicIDs = {
        .Sizes        = &RIFFMagicIDSize,
        .Offsets      = &RIFFMagicIDOffset,
        .Number       = &RIFFMagicIDNumber,
    };
    
    static const OVIADemuxer RIFFDemuxer = {
        .MagicID      = &RIFFMagicIDs,
        .MediaType    = MediaType_Container,
    };

#if (PlatformIO_Language == PlatformIO_LanguageIsCXX)
}
#endif
