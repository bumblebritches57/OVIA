#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "../../../Dependencies/BitIO/libBitIO/include/BitIO.h"

#include "../../include/libPCM.h"
#include "../../include/PCMTypes.h"
#include "../../include/WAVCommon.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    const static uint8_t BitDepth2SampleSizeInBytes[32] = {
         0,  8,  8,  8,  8,  8,  8,  8,
         8, 16, 16, 16, 16, 16, 16, 16,
        16, 24, 24, 24, 24, 24, 24, 24,
        24, 32, 32, 32, 32, 32, 32, 32
    };
    
    void ReadINFO_IART(BitInput *BitI, PCMFile *PCM, uint32_t ChunkSize) {
        char *Artist = calloc(1, ChunkSize);
        for (uint8_t Byte = 0; Byte < ChunkSize; Byte++) {
            Artist[Byte] = ReadBits(BitI, 8, true);
        }
        PCM->WAV->Meta->ArtistTag = Artist;
        if (IsOdd(ChunkSize) == true) {
            SkipBits(BitI, 8);
        }
        PCM->WAV->Meta->NumTags += 1;
    }
    
    void ReadINFO_ICRD(BitInput *BitI, PCMFile *PCM, uint32_t ChunkSize) {
        char *ReleaseDate = calloc(1, ChunkSize);
        for (uint8_t Byte = 0; Byte < ChunkSize; Byte++) {
            ReleaseDate[Byte] = ReadBits(BitI, 8, true);
        }
        PCM->WAV->Meta->ReleaseDateTag = ReleaseDate;
        if (IsOdd(ChunkSize) == true) {
            SkipBits(BitI, 8);
        }
        PCM->WAV->Meta->NumTags += 1;
    }
    
    void ReadINFO_IGNR(BitInput *BitI, PCMFile *PCM, uint32_t ChunkSize) {
        char *Genre = calloc(1, ChunkSize);
        for (uint8_t Byte = 0; Byte < ChunkSize; Byte++) {
            Genre[Byte] = ReadBits(BitI, 8, true);
        }
        PCM->WAV->Meta->GenreTag = Genre;
        if (IsOdd(ChunkSize) == true) {
            SkipBits(BitI, 8);
        }
        PCM->WAV->Meta->NumTags += 1;
    }
    
    void ReadINFO_INAM(BitInput *BitI, PCMFile *PCM, uint32_t ChunkSize) {
        char *Title = calloc(1, ChunkSize);
        for (uint8_t Byte = 0; Byte < ChunkSize; Byte++) {
            Title[Byte] = ReadBits(BitI, 8, true);
        }
        PCM->WAV->Meta->SongTitleTag = Title;
        if (IsOdd(ChunkSize) == true) {
            SkipBits(BitI, 8);
        }
        PCM->WAV->Meta->NumTags += 1;
    }
    
    void ReadINFO_IPRD(BitInput *BitI, PCMFile *PCM, uint32_t ChunkSize) {
        char *Album              = calloc(1, ChunkSize);
        for (uint8_t Byte        = 0; Byte < ChunkSize; Byte++) {
            Album[Byte]          = ReadBits(BitI, 8, true);
        }
        PCM->WAV->Meta->AlbumTag = Album;
        if (IsOdd(ChunkSize) == true) {
            SkipBits(BitI, 8);
        }
        PCM->WAV->Meta->NumTags += 1;
    }
    
    void ReadINFO_ISFT(BitInput *BitI, PCMFile *PCM, uint32_t ChunkSize) { // Encoder
        char *Encoder     = calloc(1, ChunkSize);
        for (uint8_t Byte = 0; Byte < ChunkSize; Byte++) {
            Encoder[Byte] = ReadBits(BitI, 8, true);
        }
        PCM->WAV->Meta->EncoderTag = Encoder;
        if (IsOdd(ChunkSize) == true) {
            SkipBits(BitI, 8);
        }
        PCM->WAV->Meta->NumTags += 1;
    }
    
    void ParseWavLISTChunk(BitInput *BitI, PCMFile *PCM, uint32_t ChunkSize) {
        uint32_t ShouldBeINFO  = ReadBits(BitI, 32, true);
        if (ShouldBeINFO == WAV_INFO) {
            uint32_t ChunkID   = ReadBits(BitI, 32, true);
            uint32_t ChunkSize = ReadBits(BitI, 32, true);
            
            switch (ChunkID) {
                case WAV_IART: // Artist
                    ReadINFO_IART(BitI, PCM, ChunkSize);
                    break;
                case WAV_ICRD: // Release date
                    ReadINFO_ICRD(BitI, PCM, ChunkSize);
                    break;
                case WAV_IGNR: // Genre
                    ReadINFO_IGNR(BitI, PCM, ChunkSize);
                    break;
                case WAV_INAM: // Title
                    ReadINFO_INAM(BitI, PCM, ChunkSize);
                    break;
                case WAV_IPRD: // Album
                    ReadINFO_IPRD(BitI, PCM, ChunkSize);
                    break;
                case WAV_ISFT: // Encoder
                    ReadINFO_ISFT(BitI, PCM, ChunkSize);
                    break;
                default:
                    break;
            }
        } else {
            printf("Unknown Wav LIST Chunk ID: %d\n", ShouldBeINFO);
        }
    }
    
    void ParseWavDATAChunk(BitInput *BitI, PCMFile *PCM, uint32_t ChunkSize) { //
        PCM->WAV->SampleCount            = ((ChunkSize / PCM->WAV->Channels) / Bits2Bytes(PCM->WAV->BitDepth, true));
    }
    
    void ParseWavFMTChunk(BitInput *BitI, PCMFile *PCM, uint32_t ChunkSize) {
        PCM->WAV->FMT                    = ReadBits(BitI, 32, true);
        PCM->WAV->FMTSize                = SwapEndian32(ReadBits(BitI, 32, true));
        if (PCM->WAV->FMTSize > 16) {
            PCM->WAV->FMTType            = SwapEndian16(ReadBits(BitI, 16, true));
        }
        PCM->WAV->CompressionAlgorithm   = SwapEndian16(ReadBits(BitI, 16, true));
        if (PCM->WAV->FMTSize == 16) {
            PCM->WAV->Channels           = SwapEndian16(ReadBits(BitI, 16, true));
        }
        PCM->WAV->SampleRate             = SwapEndian32(ReadBits(BitI, 32, true));
        PCM->WAV->ByteRate               = SwapEndian32(ReadBits(BitI, 32, true));
        PCM->WAV->BlockSize              = SwapEndian16(ReadBits(BitI, 16, true));
        PCM->WAV->BitDepth               = SwapEndian16(ReadBits(BitI, 16, true));
        if (PCM->WAV->FMTType == WAVFormatExtensible) {
            PCM->WAV->ExtensionSize      = SwapEndian16(ReadBits(BitI, 16, true));
            PCM->WAV->ValidBitsPerSample = SwapEndian16(ReadBits(BitI, 16, true));
            PCM->WAV->SpeakerMask        = SwapEndian32(ReadBits(BitI, 32, true));
            SkipBits(BitI, PCM->WAV->ExtensionSize - 8); // 8 bytes from ^
        }
    }
    
    void WAVExtractSamples(BitInput *BitI, PCMFile *PCM, uint64_t NumSamples2Extract) {
        for (uint8_t Channel = 0; Channel < PCM->NumChannels; Channel++) {
            for (uint64_t Sample = 0; Sample < NumSamples2Extract; Sample++) {
                PCM->Samples[Channel][Sample] = ReadBits(BitI, BitDepth2SampleSizeInBytes[PCM->BitDepth], false);
            }
        }
        PCM->NumSamples = NumSamples2Extract;
    }
    
#ifdef __cplusplus
}
#endif
