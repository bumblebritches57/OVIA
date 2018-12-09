#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif
    
    /*
     API Design:
     
     We need to be able to update the MD5 one frame at a time, that's the biggest impact on the API design period.
     
     Also, 64 bit.
     
     MD5 is LSByte, MSBit first.
     
     + = % 4294967296 aka % 256, but 32 bit.
     
     <<< = Rotate left, NOT shift.
     
     not(X) = ~
     
     X v Y  = |
     
     XY     = X & Y
     
     Each block must be padded to be 448 bits.
     
     Padding is not optional, every block MUST be 448 bits.
     
     there's 4 variables, A, B, C, D.
     
     A is initalized to: 0x01234567
     B is initalized to: 0x89ABCDEF
     C is initalized to: 0xFEDCBA98
     D is initalized to: 0x76543210
     */
    
    uint32_t F(uint32_t X, uint32_t Y, uint32_t Z) {
        return Z ^ (X & (Y ^ Z));
    }
    
    uint32_t G(uint32_t X, uint32_t Y, uint32_t Z) {
        return Y ^ (Z & (X ^ Y));
    }
    
    uint32_t H(uint32_t X, uint32_t Y, uint32_t Z) {
        return X ^ Y ^ Z;
    }
    
    uint32_t I(uint32_t X, uint32_t Y, uint32_t Z) {
        return Y ^ (X | ~Z);
    }
    
    uint32_t TableFunction(uint8_t Radians) {
        return 4294967296 * fabs(sin(Radians));
    }
    
    
    
#ifdef __cplusplus
}
#endif
