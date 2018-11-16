#include "../../../include/Private/Image/Flate.h"

#include "../../../../Dependencies/FoundationIO/libFoundationIO/include/Macros.h"
#include "../../../../Dependencies/FoundationIO/libFoundationIO/include/Log.h"
#include "../../../../Dependencies/FoundationIO/libFoundationIO/include/StringIO.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    void OVIA_PNG_DAT_ReadFlateHeader(FlateHeader *Header, BitBuffer *BitB) {
        if (Header != NULL && BitB != NULL) {
            Header->CINFO  = BitBuffer_ReadBits(MSByteFirst, MSBitFirst, BitB, 4); // 7
            Header->CM     = BitBuffer_ReadBits(MSByteFirst, MSBitFirst, BitB, 4); // 8
            Header->FLEVEL = BitBuffer_ReadBits(MSByteFirst, MSBitFirst, BitB, 2); // 3
            Header->FDICT  = BitBuffer_ReadBits(MSByteFirst, MSBitFirst, BitB, 1); // 0
            Header->FCHECK = BitBuffer_ReadBits(MSByteFirst, MSBitFirst, BitB, 5); // 0b11010 aka 0x1A aka 26
            
            if (Header->CINFO != 7) {
                Log(Log_ERROR, __func__, U8("Compresion Info %d is invalid"), Header->CINFO);
            } else if (Header->CM != 8) {
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
                Header->DictID = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 32);
            }
        } else if (Header == NULL) {
            Log(Log_ERROR, __func__, U8("FlateHeader Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    // Next: read two Huffman compressed trees that contain the probabilities.
    
    void OVIA_PNG_DAT_ReadHuffmanTrees(FlateHeader *Header, BitBuffer *BitB) {
        if (Header != NULL && BitB != NULL) {
            bool         IsFinalBlock = No;
            uint8_t      BlockType    = 1;
            do {
                IsFinalBlock          = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 1); // 0b0  aka 1
                BlockType             = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 2); // 0b10 aka 2
                if (BlockType == 0) {
                    // Literal
                } else if (BlockType == 1) {
                    // Read a Fixed Huffman tree
                } else if (BlockType == 2) {
                    // Read a Dynamic Huffman tree
                } else {
                    Log(Log_ERROR, __func__, U8("Invalid BlockType %d"), BlockType);
                }
            } while (IsFinalBlock != Yes);
        } else if (Header == NULL) {
            Log(Log_ERROR, __func__, U8("FlateHeader Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void OVIA_PNG_DAT_ReadFixedHuffmanTrees(FlateHeader *Header, BitBuffer *BitB) {
        if (Header != NULL && BitB != NULL) {
            
        } else if (Header == NULL) {
            Log(Log_ERROR, __func__, U8("FlateHeader Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void OVIA_PNG_DAT_ReadDynamicHuffmanTrees(FlateHeader *Header, BitBuffer *BitB) {
        if (Header != NULL && BitB != NULL) {
            
        } else if (Header == NULL) {
            Log(Log_ERROR, __func__, U8("FlateHeader Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
#ifdef __cplusplus
}
#endif
