#include "../Dependencies/FoundationIO/Library/include/TestIO.h"
#include "../Library/include/OVIA.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    bool Test_OVIARegistration(void) {
        bool TestPassed              = Yes;
        OVIA *Ovia                   = OVIA_Init();
        if (Ovia != NULL) {
            // Global
        }
        return TestPassed;
    }
    
    bool Test_OVIA(void) {
        bool TestPassed              = Yes;
        
        // I want to test that the registration functions are working
        
        
        
        BitBuffer *BitB              = BitBuffer_Init(8);
        Entropy   *Random            = Entropy_Init(8000000);
        
        for (uint64_t Loop = 0ULL; Loop < 1000000; Loop++) {
            uint8_t    NumBits2Extract = Entropy_GenerateInteger(Random, 3); // 6
            int64_t    RandomInteger   = Entropy_GenerateInteger(Random, NumBits2Extract);
            uint8_t    ByteOrder       = Entropy_GenerateInteger(Random, 1) + 1;
            uint8_t    BitOrder        = Entropy_GenerateInteger(Random, 1) + 1;
            
            BitBuffer_WriteBits(BitB, ByteOrder, BitOrder, NumBits2Extract, RandomInteger);
            BitBuffer_Seek(BitB, -(NumBits2Extract));
            int64_t ReadInteger        = BitBuffer_ReadBits(BitB, ByteOrder, BitOrder, NumBits2Extract);
            BitBuffer_Erase(BitB); // Clear the BitBuffer in between each run just to be sure.
        }
        
        return TestPassed;
    }
    
    int main(int argc, const char *argv[]) {
        return Test_OVIA();
    }
    
#ifdef __cplusplus
}
#endif
