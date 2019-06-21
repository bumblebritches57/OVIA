#include "../../../include/Private/InternalOVIA.h"
#include "../../../include/Private/Image/Flate.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    void OVIA_PNG_DAT_WriteZlibHeader(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            uint16_t HeaderWithoutFCHECK = OVIA_PNG_DAT_GetCompressionInfo(Ovia) << 12;
            HeaderWithoutFCHECK         |= OVIA_PNG_DAT_GetCompressionMethod(Ovia) << 8;
            HeaderWithoutFCHECK         |= OVIA_PNG_DAT_GetFLEVEL(Ovia) << 6;
            HeaderWithoutFCHECK         |= OVIA_PNG_DAT_GetFDICT(Ovia) << 5;
            OVIA_PNG_DAT_SetFCHECK(Ovia, 31 - (HeaderWithoutFCHECK % 31));
            
            BitBuffer_WriteBits(MSByteFirst, LSBitFirst, BitB, 4, OVIA_PNG_DAT_GetCompressionMethod(Ovia));
            BitBuffer_WriteBits(MSByteFirst, LSBitFirst, BitB, 4, OVIA_PNG_DAT_GetCompressionInfo(Ovia));
            
            BitBuffer_WriteBits(MSByteFirst, LSBitFirst, BitB, 5, OVIA_PNG_DAT_GetFCHECK(Ovia));
            BitBuffer_WriteBits(MSByteFirst, LSBitFirst, BitB, 1, OVIA_PNG_DAT_GetFDICT(Ovia));
            BitBuffer_WriteBits(MSByteFirst, LSBitFirst, BitB, 2, OVIA_PNG_DAT_GetFLEVEL(Ovia));
            
            if (OVIA_PNG_DAT_GetFDICT(Ovia) == true) {
                BitBuffer_WriteBits(MSByteFirst, LSBitFirst, BitB, 32, OVIA_PNG_DAT_GetDictID(Ovia));
            }
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void OVIA_PNG_Flate_WriteLiteralBlock(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            BitBuffer_Align(BitB, 1);
            // How do we know how many bytes to copy?
            uint16_t Bytes2Copy  = 0;
            uint16_t Bytes2Copy2 = Bytes2Copy ^ 0xFFFF;
            
            BitBuffer_WriteBits(LSByteFirst, LSBitFirst, BitB, 16, Bytes2Copy);
            BitBuffer_WriteBits(LSByteFirst, LSBitFirst, BitB, 16, Bytes2Copy2);
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
#ifdef __cplusplus
}
#endif
