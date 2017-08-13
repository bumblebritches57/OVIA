#include "../include/Private/ModernPNGAdler32.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    uint32_t GenerateAdler32(const uint8_t *Data, const uint64_t DataSize) {
        uint32_t Adler  = 1;
        uint32_t Sum1   = Adler & 0xFFFF;
        uint32_t Sum2   = (Adler >> 16) & 0xFFFF;
        
        for (uint64_t Byte = 0; Byte < DataSize; Byte++) {
            Sum1 += Data[Byte] % 65521;
            Sum2 += Sum1 % 65521;
        }
        return (Sum2 << 16) + Sum1;
    }
    
    bool     VerifyAdler32(const uint8_t *Data, const uint64_t DataSize, const uint32_t EmbeddedAdler32) {
        uint32_t GeneratedAdler32 = GenerateAdler32(Data, DataSize);
        if (GeneratedAdler32 != EmbeddedAdler32) {
            return false;
        } else {
            return true;
        }
    }
    
#ifdef __cplusplus
}
#endif
