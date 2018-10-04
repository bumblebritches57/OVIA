#include "../../../include/Private/Image/Flate.h"

#include "../../../../Dependencies/FoundationIO/libFoundationIO/include/BitIO.h"
#include "../../../../Dependencies/FoundationIO/libFoundationIO/include/Log.h"
#include "../../../../Dependencies/FoundationIO/libFoundationIO/include/StringIO.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    /*
     We need to loop over potentially multiple iDAT chunks, and each iDAT chunk needs to have it's Fate header read as well as a bunch of other stuff.
     */
    
    void OVIA_PNG_ReadFlateHeader(FlateHeader *Header, BitBuffer *BitB) {
        if (Header != NULL && BitB != NULL) {
            Header->CINFO  = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 4); // 7
            Header->CM     = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 4); // 8
            Header->FLEVEL = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 2); // 1
            Header->FDICT  = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 1); // 0
            Header->FCHECK = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 5); // 30
            
            if (Header->CINFO > 7) {
                Log(Log_ERROR, __func__, U8("Compresion Info %d is invalid"), Header->CINFO);
            }
            
            if (Header->CM != 8) {
                Log(Log_ERROR, __func__, U8("Compression Method %d is invalid"), Header->CM);
            }
            
            uint16_t Check = Header->CINFO << 12;
            Check         |= Header->CM << 8;
            Check         |= Header->FLEVEL << 6;
            Check         |= Header->FDICT << 5;
            Check         |= Header->FCHECK;
            
            if (Check % 31 != 0) {
                Log(Log_ERROR, __func__, U8("Invalid Flate Header %d"), Check);
            }
            
            if (Header->FDICT == 1) {
                // Read the dictionary
                // Error if the dictionary is unknown
            }
        } else if (Header == NULL) {
            Log(Log_ERROR, __func__, U8("FlateHeader Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
#ifdef __cplusplus
}
#endif
