#include "../include/W64Common.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    void ParseW64Metadata(BitInput *BitI, W64Header *W64) {
        SkipBits(BitI, 96); // Skip the rest of the RIFF GUID
        SkipBits(BitI, 64); // RIFF Size
        uint32_t ChunkID = ReadBits(BitI, 32, true);
        SkipBits(BitI, 96); // Rest of the chunk GUID
        uint64_t ChunkSize = ReadBits(BitI, 64, true);
        switch (ChunkID) {
            case W64_FMT:
                ParseW64FMTChunk(BitI, W64);
                break;
            case W64_BEXT:
                ParseW64BEXTChunk(BitI, W64);
                break;
                
            default:
                break;
        }
    }
    
    /* Format decoding */
    void ParseW64FMTChunk(BitInput *BitI, W64Header *W64) {
        W64->FormatType  = ReadBits(BitI, 16, true);
        W64->Channels    = ReadBits(BitI, 16, true);
        W64->SampleRate  = ReadBits(BitI, 32, true);
        W64->ByteRate    = ReadBits(BitI, 32, true);
        W64->BlockAlign  = ReadBits(BitI, 16, true);
        W64->BitDepth    = ReadBits(BitI, 16, true);
    }
    
    void ParseW64BEXTChunk(BitInput *BitI, W64Header *W64) {
        
    }
    
    void ParseW64DataChunk(BitInput *BitI, W64Header *W64, uint32_t ChunkSize) { // return the number of samples read
        W64->NumSamples = (((ChunkSize - 24 / W64->BlockAlign) / W64->Channels) / W64->BitDepth);
    }
    
    void ParseW64LEVL(BitInput *BitI, W64Header *W64) { // aka Peak Envelope Chunk
        
    }
    
#ifdef __cplusplus
}
#endif

