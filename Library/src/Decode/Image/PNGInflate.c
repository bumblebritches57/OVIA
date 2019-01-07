#include "../../../include/libOVIA.h"
#include "../../../include/Private/InternalOVIA.h"
#include "../../../include/Private/Image/Flate.h"

#include "../../../../Dependencies/FoundationIO/libFoundationIO/include/Log.h"
#include "../../../../Dependencies/FoundationIO/libFoundationIO/include/StringIO.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    void OVIA_PNG_Flate_ReadZlibHeader(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            uint8_t CMF    = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 8);
            uint8_t FLAG   = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 8);
            
            OVIA_PNG_DAT_SetCMF(Ovia, CMF);
            OVIA_PNG_DAT_SetFLG(Ovia, FLAG);
            
            bool FDICTPresent = OVIA_PNG_DAT_GetFDICT(Ovia);
            if (FDICTPresent) {
                uint32_t DictID = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 32);
                OVIA_PNG_DAT_SetDictID(Ovia, DictID);
            }
            
            uint8_t CompressionInfo   = OVIA_PNG_DAT_GetCompressionInfo(Ovia);
            uint8_t CompressionMethod = OVIA_PNG_DAT_GetCompressionMethod(Ovia);
            
            if (CompressionInfo != 7) {
                Log(Log_ERROR, __func__, U8("Compresion Info %d is invalid"), CompressionInfo);
            } else if (CompressionMethod != 8) {
                Log(Log_ERROR, __func__, U8("Compression Method %d is invalid"), CompressionMethod);
            }
            
            uint16_t Check = CompressionInfo << 12;
            Check         |= CompressionMethod << 8;
            Check         |= OVIA_PNG_DAT_GetFLEVEL(Ovia) << 6;
            Check         |= FDICTPresent << 5;
            Check         |= OVIA_PNG_DAT_GetFCHECK(Ovia);
            if (Check % 31 != 0) {
                Log(Log_ERROR, __func__, U8("Invalid Flate Header %d"), Check);
            }
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void OVIA_PNG_Flate_ReadLiteralBlock(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            uint8_t *Array     = (uint8_t*) OVIA_PNG_DAT_GetArray(Ovia);
            
            BitBuffer_Align(BitB, 1);
            uint16_t Bytes2Copy    = BitBuffer_ReadBits(LSByteFirst, LSBitFirst, BitB, 16);
            uint16_t Bytes2CopyXOR = BitBuffer_ReadBits(LSByteFirst, LSBitFirst, BitB, 16) ^ 0xFFFF;
            
            for (uint16_t Byte = 0ULL; Byte < Bytes2Copy; Byte++) {
                Array[Byte] = BitBuffer_ReadBits(LSByteFirst, LSBitFirst, BitB, 8);
            }
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void OVIA_PNG_Flate_ReadFixedBlock(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            // Build 2 trees, one for lengths, one for distances.
            HuffmanTable *LengthTree    = OVIA_PNG_Huffman_BuildTree(288, FixedLiteralTable);
            HuffmanTable *DistanceTree  = OVIA_PNG_Huffman_BuildTree(29, FixedDistanceTable);
            
            OVIA_PNG_DAT_SetLengthLiteralHuffmanTable(Ovia, LengthTree);
            OVIA_PNG_DAT_SetDistanceHuffmanTable(Ovia, DistanceTree);
            
            for (uint64_t Index = 0ULL; Index < 288; Index++) {
                uint32_t Length        = 0;
                uint32_t Value         = ReadHuffman(LengthTree, BitB);
                if (Value < 16) {
                    FixedLiteralTable[Index + 1] = Value;
                } else {
                    if (Value == 16) {
                        Length         = FixedLiteralTable[Index - 1];
                        Value          = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 2) + 3;
                    } else if (Value == 17) {
                        Value          = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 3) + 3;
                    } else {
                        Value          = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 7) + 11;
                    }
                    for (uint32_t Times2Copy = 0ULL; Times2Copy < Length; Times2Copy++) {
                        
                    }
                    for (uint32_t Index = Value; Index > 0; Index--) {
                        FixedLiteralTable[Index + 1] = Length;
                    }
                }
            }
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void OVIA_PNG_Flate_ReadTreeDynamic(OVIA *Ovia, BitBuffer *BitB) {
        uint16_t *Table                     = NULL;
        if (BitB != NULL) {
            Table                           = calloc(MaxCodes, sizeof(uint16_t));
            if (Table != NULL) {
                uint16_t NumLengthSymbols   = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 5) + 257;
                uint8_t  NumDistanceSymbols = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 5) + 1;
                uint8_t  NumMetaCodeLengths = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 4) + 4;
                
                uint16_t MetaTable[NumMetaCodes];
                for (uint8_t MetaCode = 0; MetaCode < NumMetaCodeLengths; MetaCode++) {
                    MetaTable[MetaCodeLengthOrder[MetaCode]] = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 3);
                }
                HuffmanTable *MetaTree = OVIA_PNG_Huffman_BuildTree(19, MetaTable);
                uint16_t RealLengths[316] = {0};
                for (uint64_t Index = 0ULL; Index < NumLengthSymbols + NumDistanceSymbols; Index++) {
                    uint64_t Value        = ReadHuffman(RealLengths, BitB);
                    if (Value >= 0 && Value <= 15) {
                        RealLengths[Index + 1] = Value;
                    } else {
                        uint16_t Times2Repeat = 0;
                        if (Value == 16) {
                            Times2Repeat   = RealLengths[Index - 1];
                            Value          = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 2) + 3;
                        } else if (Value == 17) {
                            Value          = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 3) + 3;
                        } else {
                            Value          = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 7) + 11;
                        }
                        for (uint16_t Index2 = Value; Index2 > 0; Index2--) {
                            RealLengths[Index2 + 1] = Times2Repeat;
                        }
                    }
                }
                
                HuffmanTable *LengthTree   = NULL;
                HuffmanTable *DistanceTree = NULL;
                
                LengthTree = OVIA_PNG_Huffman_BuildTree(NumLengthSymbols, RealLengths);
                OVIA_PNG_DAT_SetLengthLiteralHuffmanTable(Ovia, LengthTree);
                
                DistanceTree = OVIA_PNG_Huffman_BuildTree(NumLengthSymbols + NumDistanceSymbols, RealLengths); // Change to Distance table
                OVIA_PNG_DAT_SetDistanceHuffmanTable(Ovia, DistanceTree);
                
                OVIA_PNG_Flate_ReadSymbol(Ovia, BitB);
            } else {
                Log(Log_ERROR, __func__, U8("Couldn't allocate Table"));
            }
        } else {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void OVIA_PNG_DAT_Decode(OVIA *Ovia, BitBuffer *BitB, ImageContainer *Image) {
        if (BitB != NULL && Image != NULL) {
            uint8_t *ImageArrayBytes               = (uint8_t*) ImageContainer_GetArray(Image);
            OVIA_PNG_DAT_SetArray(Ovia, ImageArrayBytes);
            bool     IsFinalBlock                  = false;
            
            do {
                IsFinalBlock                       = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 1); // 0
                uint8_t BlockType                  = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 2); // 0b10 = 2
                if (BlockType == Flate_LiteralBlock) {
                    OVIA_PNG_Flate_ReadLiteralBlock(Ovia, BitB);
                } else if (BlockType == Flate_FixedBlock) {
                    OVIA_PNG_Flate_ReadFixedBlock(Ovia, BitB);
                } else if (BlockType == Flate_DynamicBlock) {
                    OVIA_PNG_Flate_ReadTreeDynamic(Ovia, BitB);
                } else if (BlockType == Flate_InvalidBlock) {
                    Log(Log_ERROR, __func__, U8("Invalid Block"));
                }
            } while (IsFinalBlock == false);
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        } else if (Image == NULL) {
            Log(Log_ERROR, __func__, U8("ImageContainer Pointer is NULL"));
        }
    }
    
    uint64_t ReadHuffman(HuffmanTable *Tree, BitBuffer *BitB) {
        uint64_t Symbol = 0ULL;
        if (Tree != NULL && BitB != NULL) {
            uint32_t FirstSymbolOfLength = 0;
            for (uint8_t Bit = 1; Bit <= MaxBitsPerSymbol; Bit++) {
                Symbol <<= 1;
                Symbol  |= BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 1);
                if (Symbol - Tree->Frequency[Bit] < FirstSymbolOfLength) {
                    Symbol = Tree->Symbols[Bit + (Symbol - FirstSymbolOfLength)];
                }
            }
        } else if (Tree == NULL) {
            Log(Log_ERROR, __func__, U8("HuffmanTable Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
        return Symbol;
    }
    
    void OVIA_PNG_Flate_ReadSymbol(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            uint16_t Symbol                    = 0;
            uint8_t *Array                     = OVIA_PNG_DAT_GetArray(Ovia);
            uint64_t Offset                    = OVIA_PNG_DAT_GetArrayOffset(Ovia);
            
            HuffmanTable *LengthLiterals       = OVIA_PNG_DAT_GetLengthLiteralHuffmanTable(Ovia);
            HuffmanTable *Distances            = OVIA_PNG_DAT_GetDistanceHuffmanTable(Ovia);
            do {
                Symbol                         = ReadHuffman(LengthLiterals, BitB);
                if (Symbol < 256) {
                    Array[Offset]              = Symbol;
                    Offset                    += 1;
                } else if (Symbol > 256) {
                    uint16_t BaseLength        = LengthBase[Symbol - 257];
                    uint16_t ExtensionLength   = LengthAdditionalBits[Symbol - 257];
                    uint16_t LengthCode        = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, ExtensionLength) + BaseLength;
                    
                    uint16_t DistanceSymbol    = ReadHuffman(Distances, BitB);
                    uint16_t BaseDistance      = DistanceBase[DistanceSymbol];
                    uint16_t ExtensionDistance = DistanceAdditionalBits[DistanceSymbol];
                    
                    uint16_t DistanceCode      = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, ExtensionDistance) + BaseDistance;
                    
                    for (uint64_t Start = Offset - LengthCode; Start < Offset + DistanceCode; Start++) {
                        Array[Start] = Array[Start - DistanceCode];
                    }
                    Offset += LengthCode;
                }
            } while (Symbol != 256);
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
#ifdef __cplusplus
}
#endif
