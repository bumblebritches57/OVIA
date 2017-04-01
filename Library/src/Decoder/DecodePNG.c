#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "../../../Dependencies/BitIO/libBitIO/include/BitIO.h"

#include "../../include/libModernPNG.h"
#include "../../include/Decoder/DecodePNG.h"
#include "../../include/PNGTypes.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    
    
    /*
     bool VerifyChunkCRC(BitInput *BitI, uint32_t ChunkSize) {
     // So basically we need to read ChunkSize bytes into an array, then read the following 4 bytes as the CRC
     // then run VerifyCRC over the buffer, and finally compare the generated CRC with the extracted one, and return whether they match.
     // Then call SkipBits(BitI, Bytes2Bits(ChunkSize)); to reset to the beginning of the chunk
     uint8_t *Buffer2CRC = calloc(1, ChunkSize);
     for (uint32_t Byte = 0; Byte < ChunkSize; Byte++) {
     Buffer2CRC[Byte] = BitI->Buffer[Bits2Bytes(, false)];
     
     }
     uint32_t ChunkCRC = ReadBits(BitI, 32, true);
     bool CRCsMatch = VerifyCRC(Buffer2CRC, ChunkSize, 0x82608EDB, 32, 0xFFFFFFFF, ChunkCRC);
     SkipBits(BitI, -Bytes2Bits(ChunkSize));
     return CRCsMatch;
     }
     */
    
    void CalculateSTERPadding(DecodePNG *Dec) {
        Dec->LinePadding = 7 - ((Dec->iHDR->Width - 1) % 8);
        Dec->LineWidth   = (Dec->iHDR->Width * 2) + Dec->LinePadding;
    }
    
    void PNGDecodeNonFilter(DecodePNG *Dec, uint8_t *DeEntropyedData, uint8_t *DeFilteredData, size_t Line) {
        for (size_t Byte = 1; Byte < Bits2Bytes(Dec->iHDR->BitDepth, true); Byte++) {
            DeFilteredData[Byte - 1] = DeEntropyedData[Byte]; // Remove filter indicating byte
        }
    }
    
    void PNGDecodeSubFilter(DecodePNG *Dec, uint8_t **DeEntropyedData, uint8_t **DeFilteredData, size_t Line) {
        for (size_t Byte = 1; Byte < Bits2Bytes(Dec->iHDR->BitDepth, true); Byte++) {
            DeFilteredData[Line][Byte - 1] = (DeEntropyedData[Line][Byte] + DeEntropyedData[Line][Byte+1]) & 0xFF;
        }
    }
    
    void PNGDecodeUpFilter(DecodePNG *Dec, uint8_t **DeEntropyedData, uint8_t **DeFilteredData, size_t Line) {
        for (size_t Byte = 1; Byte < Bits2Bytes(Dec->iHDR->BitDepth, true); Byte++) {
            DeFilteredData[Line][Byte - 1] = DeEntropyedData[Line][Byte] + DeEntropyedData[Line - 1][Byte] & 0xFF;
        }
    }
    
    void PNGDecodeAverageFilter(DecodePNG *Dec, uint8_t **DeEntropyedData, uint8_t **DeFilteredData, size_t Line) {
        uint8_t PixelSize = Bits2Bytes(Dec->iHDR->BitDepth, true);
        for (size_t Byte = 1; Byte < Bits2Bytes(Dec->iHDR->BitDepth, true); Byte++) {
            uint8_t Average = floor((DeEntropyedData[Line][Byte - (PixelSize)] + DeEntropyedData[Line - 1][Byte]) / 2);
            DeFilteredData[Line][Byte - 1] = DeEntropyedData[Line][Byte] + Average;
        }
    }
    
    uint8_t PaethPredictor(int64_t Left, int64_t Above, int64_t UpperLeft) {
        int64_t Guess     = Left + Above - UpperLeft;
        int64_t DistanceA = llabs(Guess - Left);
        int64_t DistanceB = llabs(Guess - Above);
        int64_t DistanceC = llabs(Guess - UpperLeft);
        
        uint8_t Output = 0;
        if (DistanceA <= DistanceB && DistanceA < DistanceC) {
            Output = DistanceA;
        } else if (DistanceB < DistanceC) {
            Output = DistanceB;
        } else {
            Output = DistanceC;
        }
        return Output;
    }
    
    void PNGDecodePaethFilter(DecodePNG *Dec, uint8_t **DeEntropyedData, uint8_t **DeFilteredData, size_t Line) {
        // Filtering is applied to bytes, not pixels
        uint8_t PixelSize = Bits2Bytes(Dec->iHDR->BitDepth, true);
        for (size_t Byte = 1; Byte < Bits2Bytes(Dec->iHDR->BitDepth, true); Byte++) {
            if (Line == 0) { // Assume top and top left = 0
                DeFilteredData[Line][Byte] = PaethPredictor(DeEntropyedData[Line][Byte], 0, 0);
            } else {
                DeFilteredData[Line][Byte] = PaethPredictor(DeEntropyedData[Line][Byte], DeEntropyedData[Line][Byte - PixelSize], DeEntropyedData[Line - 1][Byte - PixelSize]);
            }
        }
    }
    
    void PNGDecodeFilter(DecodePNG *Dec, uint8_t ***InflatedBuffer) {
        char Error[BitIOStringSize];
        
        uint8_t DeFilteredData[Dec->iHDR->Height][Dec->iHDR->Width - 1];
        
        for (size_t Line = 0; Line < Dec->iHDR->Height; Line++) {
            uint8_t FilterType = *InflatedBuffer[Line][0];
            switch (FilterType) {
                case 0:
                    // copy the Line except byte 0 (the filter indication byte) to the output buffer.
                    PNGDecodeNonFilter(Dec, InflatedBuffer, DeFilteredData, Line);
                    break;
                case 1:
                    // SubFilter
                    PNGDecodeSubFilter(Dec, InflatedBuffer, DeFilteredData, Line);
                    break;
                case 2:
                    // UpFilter
                    PNGDecodeUpFilter(Dec, InflatedBuffer, DeFilteredData, Line);
                    break;
                case 3:
                    // AverageFilter
                    PNGDecodeAverageFilter(Dec, InflatedBuffer, DeFilteredData, Line);
                    break;
                case 4:
                    // PaethFilter
                    PNGDecodePaethFilter(Dec, InflatedBuffer, DeFilteredData, Line);
                    break;
                default:
                    snprintf(Error, BitIOStringSize, "Filter type: %d is invalid\n", FilterType);
                    Log(LOG_ERR, "ModernPNG", "PNGDecodeFilteredLine", Error);
                    break;
            }
        }
    }
    
    void DecodePNGData(BitInput *BitI, DecodePNG *Dec) {
        // read the iDAT/fDAT chunk header, then do the other stuff.
        while (BytesRemainingInFile(BitI) > 0) { // 12 is the start of IEND
            uint32_t ChunkSize = ReadBits(BitI, 32, true);
            uint32_t ChunkID   = ReadBits(BitI, 32, true);
            
            if (strcasecmp(ChunkID, "iDAT") == 0) {
                ParseIDAT(BitI, Dec, ChunkSize);
            } else if (strcasecmp(ChunkID, "acTL") == 0) {
                
            } else if (strcasecmp(ChunkID, "fdAT") == 0) {
                ParseFDAT(BitI, Dec, ChunkSize);
            }
        }
    }
    
    void PNGDecodeImage(BitInput *BitI, DecodePNG *Dec, uint16_t *DecodedImage) {
        // Parse all chunks except iDAT, fDAT, and iEND first.
        // When you come across an iDAT or fDAT. you need to store the start address, then return to parsing the meta chunks, then at the end  decode the i/f DATs.
        ParsePNGMetadata(BitI, Dec);
        
        if (Dec->Is3D == true) {
            
        }
        while (BytesRemainingInFile(BitI) > 0) {
            
        }
    }
    
    uint16_t **DecodeAdam7(DecodePNG *Dec, uint16_t **DecodedImage) {
        // Break the image into 8x8 blocks.
        // MARK: if the image is not a multiple of 8, I assume you pad the edge blocks?
        
        
        
        return 0;
    }
    
    // Let's do this library right, by adding attach and delete functions for the various chunks, and let's also have a fancy function that applies color profiles to the pixels.
    // that's kinda a lot of work tho...
    
    void SeperateStereoImage(DecodePNG *Dec, uint16_t **DecodedImage) {
        
    }
    
    void PNGReadMetadata(BitInput *BitI, DecodePNG *Dec) {
        ParsePNGMetadata(BitI, Dec);
    }
    
#ifdef __cplusplus
}
#endif
