#include "../../../include/Private/Image/Flate.h"

#include "../../../../Dependencies/FoundationIO/libFoundationIO/include/Macros.h"
#include "../../../../Dependencies/FoundationIO/libFoundationIO/include/Log.h"
#include "../../../../Dependencies/FoundationIO/libFoundationIO/include/StringIO.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    void OVIA_PNG_DAT_WriteFlateHeader(FlateHeader *Header, BitBuffer *BitB) {
        if (Header != NULL && BitB != NULL) {
            uint16_t HeaderWithoutFCHECK = Header->CINFO << 12;
            HeaderWithoutFCHECK         |= Header->CM << 8;
            HeaderWithoutFCHECK         |= Header->FLEVEL << 6;
            HeaderWithoutFCHECK         |= Header->FDICT << 5;
            Header->FCHECK               = 31 - (HeaderWithoutFCHECK % 31);
            
            BitBuffer_WriteBits(MSByteFirst, MSBitFirst, BitB, 4, Header->CINFO);
            BitBuffer_WriteBits(MSByteFirst, MSBitFirst, BitB, 4, Header->CM);
            BitBuffer_WriteBits(MSByteFirst, MSBitFirst, BitB, 4, Header->FLEVEL);
            BitBuffer_WriteBits(MSByteFirst, MSBitFirst, BitB, 4, Header->FDICT);
            BitBuffer_WriteBits(MSByteFirst, MSBitFirst, BitB, 4, Header->FCHECK);
            
            if (Header->FDICT == Yes) {
                // Get the Dictionary ID somehow.
                BitBuffer_WriteBits(MSByteFirst, MSBitFirst, BitB, 32, Header->DictID);
            }
        } else if (Header == NULL) {
            Log(Log_ERROR, __func__, U8("FlateHeader Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    // Next, write the 2 Huffman encoded trees.
    
#ifdef __cplusplus
}
#endif
