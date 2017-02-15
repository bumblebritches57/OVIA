#include "libModernPNG.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    int main(int argc, const char * argv[]) {
        BitInput    *BitI  = calloc(sizeof(BitInput), 1);
        BitOutput   *BitO  = calloc(sizeof(BitOutput), 1);
        ErrorStatus *Error = calloc(sizeof(ErrorStatus), 1);
        PNGDecoder  *PNG   = calloc(sizeof(PNGDecoder), 1);
        
        InitBitInput(BitI, Error, argc, argv);
        InitBitOutput(BitO, Error, argc, argv);
        InitPNGDecoder(PNG);
        ParsePNG(BitI, PNG);
        
        // DecodePNG(BitI, PNG);
        return 0;
    }
    
#ifdef __cplusplus
}
#endif
