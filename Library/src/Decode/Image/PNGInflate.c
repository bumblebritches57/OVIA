#include "../../../include/Private/InternalOVIA.h"
#include "../../../include/Private/Image/Flate.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    /*
     PNG Decode Control Flow:
     
     Get the magic, discover the file is a PNG (or PNG embedded in BMP, whatever)
     Parse the chunks in the PNG file
     get to a i/fDAT chunk,
     read the Zlib header
     Build the Huffman trees for each "block"
     take a ImageContainer as a parameter
     decode the block into the imagecontainer
     */
    
    void OVIA_PNG_Flate_ReadZlibHeader(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            uint8_t CMF    = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 8); // 0x78
            uint8_t FLAG   = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 8); // 0x01
            
            OVIA_PNG_DAT_SetCMF(Ovia, CMF);
            OVIA_PNG_DAT_SetFLG(Ovia, FLAG);
            
            bool FDICTPresent = OVIA_PNG_DAT_GetFDICT(Ovia); // No
            if (FDICTPresent == Yes) {
                uint32_t DictID = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 32);
                OVIA_PNG_DAT_SetDictID(Ovia, DictID);
            }
            
            uint8_t CompressionInfo   = OVIA_PNG_DAT_GetCompressionInfo(Ovia); // 7
            uint8_t CompressionMethod = OVIA_PNG_DAT_GetCompressionMethod(Ovia); // 8
            
            if (CompressionInfo == 7 && CompressionMethod == 8) {
                uint16_t Check = CompressionInfo << 12;
                Check         |= CompressionMethod << 8;
                Check         |= OVIA_PNG_DAT_GetFLEVEL(Ovia) << 6;
                Check         |= FDICTPresent << 5;
                Check         |= OVIA_PNG_DAT_GetFCHECK(Ovia);
                if (Check % 31 != 0) {
                    Log(Log_ERROR, __func__, U8("Invalid Flate Header %d"), Check);
                }
            } else if (CompressionInfo != 7) {
                Log(Log_ERROR, __func__, U8("Compresion Info %d is invalid"), CompressionInfo);
            } else if (CompressionMethod != 8) {
                Log(Log_ERROR, __func__, U8("Compression Method %d is invalid"), CompressionMethod);
            }
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    void OVIA_PNG_Flate_ReadLiteralBlock(OVIA *Ovia, BitBuffer *BitB) {
        if (Ovia != NULL && BitB != NULL) {
            uint8_t *Array         = (uint8_t*) OVIA_PNG_DAT_GetArray(Ovia);
            
            BitBuffer_Align(BitB, 1); // Skip the remaining 5 bits
            uint16_t Bytes2Copy    = BitBuffer_ReadBits(LSByteFirst, LSBitFirst, BitB, 16); // 0x4F42 = 20,290
            uint16_t Bytes2CopyXOR = BitBuffer_ReadBits(LSByteFirst, LSBitFirst, BitB, 16) ^ 0xFFFF; // 0xB0BD = 0x4F42
            
            if (Bytes2Copy == Bytes2CopyXOR) {
                for (uint16_t Byte = 0ULL; Byte < Bytes2Copy; Byte++) {
                    Array[Byte]    = BitBuffer_ReadBits(LSByteFirst, LSBitFirst, BitB, 8);
                }
            } else {
                Log(Log_ERROR, __func__, U8("Data Error: Bytes2Copy does not match Bytes2CopyXOR in literal block"));
            }
        } else if (Ovia == NULL) {
            Log(Log_ERROR, __func__, U8("OVIA Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
    }
    
    HuffmanTree *OVIA_PNG_Flate_BuildTree(BitBuffer *BitB, HuffmanTreeTypes TreeType) {
        HuffmanTree *Tree = NULL;
        if (BitB != NULL && TreeType != TreeType_Unknown) {
            Tree          = calloc(1, sizeof(HuffmanTree));
            if (Tree != NULL) {
                if (TreeType == TreeType_Fixed) {
                    Tree->LengthTable          = OVIA_PNG_Huffman_BuildTree(288, FixedLiteralTable);
                    Tree->DistanceTable        = OVIA_PNG_Huffman_BuildTree(32, FixedDistanceTable);
                    
                    for (uint64_t Index = 0ULL; Index < 288; Index++) {
                        uint32_t Length        = 0;
                        uint32_t Value         = ReadHuffman(Tree->LengthTable, BitB);
                        if (Value < 16) {
                            FixedLiteralTable[Index + 1] = Value;
                        } else {
                            if (Value == 16) {
                                Length         = FixedLiteralTable[Index - 1];
                                Value          = 3  + BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 2);
                            } else if (Value == 17) {
                                Value          = 3  + BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 3);
                            } else {
                                Value          = 11 + BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 7);
                            }
                            for (uint32_t Times2Copy = 0ULL; Times2Copy < Length; Times2Copy++) {
                                // What is this?
                            }
                            for (uint32_t Index = Value; Index > 0; Index--) {
                                FixedLiteralTable[Index + 1] = Length;
                            }
                        }
                    }
                } else if (TreeType == TreeType_Dynamic) {
                    uint16_t NumLengthSymbols   = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 5) + 257;
                    uint8_t  NumDistanceSymbols = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 5) + 1;
                    uint8_t  NumMetaCodeLengths = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 4) + 4;
                    
                    uint16_t MetaTable[NumMetaCodes];
                    for (uint8_t MetaCode = 0; MetaCode < NumMetaCodeLengths; MetaCode++) {
                        MetaTable[MetaCodeLengthOrder[MetaCode]] = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 3);
                    }
                    HuffmanTable *MetaTree         = OVIA_PNG_Huffman_BuildTree(19, MetaTable);
                    uint16_t RealLengths[318]      = {0};
                    for (uint64_t Index = 0ULL; Index < NumLengthSymbols + NumDistanceSymbols; Index++) {
                        uint64_t Value             = ReadHuffman(MetaTree, BitB);
                        if (Value <= 15) {
                            RealLengths[Index + 1] = Value;
                        } else {
                            uint16_t Times2Repeat  = 0;
                            if (Value == 16) {
                                Times2Repeat       = RealLengths[Index - 1];
                                Value              = 3  + BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 2);
                            } else if (Value == 17) {
                                Value              = 3  + BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 3);
                            } else {
                                Value              = 11 + BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 7);
                            }
                            for (uint16_t Index2 = Value; Index2 > 0; Index2--) {
                                RealLengths[Index2 + 1] = Times2Repeat;
                            }
                        }
                    }
                    Tree->LengthTable   = OVIA_PNG_Huffman_BuildTree(NumLengthSymbols, RealLengths);
                    Tree->DistanceTable = OVIA_PNG_Huffman_BuildTree(NumLengthSymbols + NumDistanceSymbols, RealLengths); // Change to Distance table
                }
            } else {
                Log(Log_ERROR, __func__, U8("Couldn't allocate HuffmanTree"));
            }
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        } else if (TreeType == TreeType_Unknown) {
            Log(Log_ERROR, __func__, U8("TreeType Unknown is invalid"));
        }
        return Tree;
    }
    
    void OVIA_PNG_DAT_Decode(OVIA *Ovia, BitBuffer *BitB, ImageContainer *Image) {
        if (BitB != NULL && Image != NULL) {
            uint8_t *ImageArrayBytes               = (uint8_t*) ImageContainer_GetArray(Image);
            OVIA_PNG_DAT_SetArray(Ovia, ImageArrayBytes);
            bool     IsFinalBlock                  = false;
            
            do {
                HuffmanTree *Tree                  = NULL;
                IsFinalBlock                       = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 1); // 0
                uint8_t BlockType                  = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, 2); // 0b00 = 0
                if (BlockType == Flate_LiteralBlock) {
                    OVIA_PNG_Flate_ReadLiteralBlock(Ovia, BitB);
                } else if (BlockType == Flate_FixedBlock) {
                    Tree = OVIA_PNG_Flate_BuildTree(BitB, TreeType_Fixed);
                } else if (BlockType == Flate_DynamicBlock) {
                    Tree = OVIA_PNG_Flate_BuildTree(BitB, TreeType_Dynamic);
                } else if (BlockType == Flate_InvalidBlock) {
                    Log(Log_ERROR, __func__, U8("Invalid Block"));
                }
                OVIA_PNG_Flate_Decode(Ovia, BitB); // Actually read the data
            } while (IsFinalBlock == false);
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        } else if (Image == NULL) {
            Log(Log_ERROR, __func__, U8("ImageContainer Pointer is NULL"));
        }
    }
    
    uint64_t ReadHuffman(HuffmanTable *Tree, BitBuffer *BitB) { // EQUILIVENT OF DECODE IN PUFF
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
    
    uint16_t OVIA_PNG_Flate_ReadSymbol(HuffmanTree *Tree, BitBuffer *BitB) {
        uint16_t Symbol                        = 0;
        if (Tree != NULL && BitB != NULL) {
            Symbol                             = ReadHuffman(Tree, BitB);
            /*
            do {
                if (Symbol < 256) {
                    Array[Offset]              = Symbol;
                    Offset                    += 1;
                } else if (Symbol > 256) {
                    uint16_t BaseLength        = LengthBase[Symbol - 257];
                    uint16_t ExtensionLength   = LengthAdditionalBits[Symbol - 257];
                    uint16_t LengthCode        = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, ExtensionLength) + BaseLength;
                    
                    uint16_t DistanceSymbol    = ReadHuffman(Tree->DistanceTable, BitB);
                    uint16_t BaseDistance      = DistanceBase[DistanceSymbol];
                    uint16_t ExtensionDistance = DistanceAdditionalBits[DistanceSymbol];
                    
                    uint16_t DistanceCode      = BitBuffer_ReadBits(MSByteFirst, LSBitFirst, BitB, ExtensionDistance) + BaseDistance;
                    
                    for (uint64_t Start = Offset - LengthCode; Start < Offset + DistanceCode; Start++) {
                        Array[Start] = Array[Start - DistanceCode];
                    }
                    Offset += LengthCode;
                }
            } while (Symbol != 256);
             */
        } else if (Tree == NULL) {
            Log(Log_ERROR, __func__, U8("HuffmanTree Pointer is NULL"));
        } else if (BitB == NULL) {
            Log(Log_ERROR, __func__, U8("BitBuffer Pointer is NULL"));
        }
        return Symbol;
    }
    
#ifdef __cplusplus
}
#endif
