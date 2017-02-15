#pragma once

#include "/usr/local/Packages/libBitIO/include/BitIO.h"
#include "/usr/local/Packages/libBitIO/include/UUID.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    extern enum libPCMConstants {
        WAVMagic            = 0x52494646,
        W64Magic            = 0x72696666,
        AIFMagic            = 0x464F524D,
        PCMMaxChannels      = 8,
        PCMMaxSampleRate    = 192000,
        W64HeaderSize       = 104,
        WAVFormatExtensible = 0xFFFE,
    } libPCMConstants;
    
    typedef enum SpeakerMask {
        FrontRight       =     0x1,
        FrontLeft        =     0x2,
        FrontCenter      =     0x4,
        LFE              =     0x8,
        BackLeft         =    0x10,
        BackRight        =    0x20, // yes
        FrontLeftCenter  =    0x40, // yes
        FrontRightCenter =    0x80,
        BackCenter       =   0x100,
        SideLeft         =   0x200,
        SideRight        =   0x400,
        TopCenter        =   0x800,
        TopFrontLeft     =  0x1000,
        TopFrontCenter   =  0x2000,
        TopFrontRight    =  0x4000, // yes
        TopBackLeft      =  0x8000,
        TopBackCenter    = 0x10000,
        TopBackRight     = 0x20000,
    } SpeakerMask;
    
    typedef struct PCMMetadata {
        uint8_t  NumTags;
        char    *ArtistTag;
        char    *ReleaseDateTag;
        char    *GenreTag;
        char    *AlbumTag;
        char    *SongTitleTag;
        char    *EncoderTag;
    } PCMMetadata;
    
    typedef struct PCMData {
        uint64_t  SampleRate;
        uint64_t  BitDepth;
        uint64_t  NumChannels;
        uint64_t  NumChannelAgnosticSamplesInBuffer;
    } PCMData;
    
    typedef struct W64Header {
        uint16_t FormatType;
        uint16_t Channels;
        uint32_t SampleRate;
        uint32_t ByteRate;
        uint16_t BlockAlign;
        uint16_t BitDepth;
        uint64_t DataSize;
        uint64_t NumSamples; // Channel independent
    } W64Header;
    
    typedef struct WAVHeader {
        uint32_t RIFF;
        uint32_t RIFFSize; // Everything except RIFF or RIFF size aka -8
        uint32_t WAVE;
        uint32_t FMT;
        uint32_t FMTSize; // Size of the format chunk
        uint16_t FMTType;
        uint16_t CompressionAlgorithm;
        uint16_t Channels;
        // START WAVE_FORMAT_EXTENSIBLE
        uint16_t ValidBitsPerSample;
        uint32_t SpeakerMask; // is actually 18 bits long
                              // END WAVE_FORMAT_EXTENSIBLE
        uint32_t SampleRate;
        uint32_t ByteRate;
        uint16_t BlockSize; // Padding
        uint16_t BitDepth; // BitDepth
        uint16_t ExtensionSize;
        uint32_t DATAChunk;
        uint32_t AudioDataSize;
        uint64_t SampleCount;
        uint8_t  HeaderSize;
        PCMMetadata *Meta;
    } WAVHeader;
    
    typedef struct AIFHeader {
        uint16_t     Channels;
        uint32_t     NumFrames; // More accurately described as number of channel agnostic samples.
        uint16_t     BitDepth; // Samples are padded to the next complete byte
        
        /* SNSD */
        uint32_t     Offset;
        uint32_t     BlockSize;
        
        PCMMetadata *Meta;
    } AIFHeader;
    
    extern enum W64ChunkIDs {
        W64_RIFF = 0x72696666,
        W64_WAVE = 0x77617665,
        W64_FMT  = 0x666D7420,
        W64_DATA = 0x64617461,
        W64_LEVL = 0x6C76656C,
        W64_JUNK = 0x6B6E756A,
        W64_BEXT = 0x74786562,
        W64_FACT = 0x74636166,
        W64_LIST = 0x7473696C,
        W64_MRKR = 0xABF76256,
        W64_SUMM = 0x925F94BC,
    } W64ChunkIDs;
    
    extern enum WAVChunkIDs { // odd sized chunks have a trailing 0 byte, and strings are null padded
        WAV_RIFF = 0x52494646,
        WAV_WAVE = 0x57415645,
        WAV_FMT  = 0x666D7420,
        WAV_LIST = 0x4C495354,
        WAV_INFO = 0x494E464F,
        WAV_IART = 0x49415254, // Artist
        WAV_ICRD = 0x49435244, // Release data
        WAV_IGNR = 0x49474E52, // Genre
        WAV_INAM = 0x494E414D, // Song name aka title
        WAV_IPRD = 0x49505244, // Album name
        WAV_IPRT = 0x49505254, // Track number
        WAV_ISFT = 0x49534654, // Software that created it?
        WAV_CODE = 0x434f4445, // Encoder
        WAV_ICMT = 0x49434d54, // Comment
        WAV_ICOP = 0x49434f50, // Copyright
        WAV_YEAR = 0x59454152, // Year
        WAV_TRCK = 0x5452434b, // Track
        WAV_DATA = 0x64617461,
    } WAVChunkIDs;
    
    extern enum AIFChunkIDs {
        AIF_FORM = 0x464F524D,
        AIF_AIFF = 0x41494646,
        AIF_AIFC = 0x41494643,
        AIF_NAME = 0x4E414D45,
        AIF_COMM = 0x434F4D4D,
        AIF_SSND = 0x53534E44,
    } AIFChunkIDs;
    
    extern enum PCMFileTypes {
        Unknown_Type = 0,
        WAV_Type     = 1,
        RF64_Type    = 2,
        W64_Type     = 3,
        AIFF_Type    = 4,
    } PCMFileTypes;
    
    typedef struct PCMFile {
        uint8_t      FileType;
        bool         FilledRequest; // Were there enough samples left to fill the request?
        uint32_t     NumSamples; // Channel agnostic
        uint32_t     ChannelMask;
        bool         MetadataHasBeenParsed;
        WAVHeader   *WAV;
        PCMMetadata *Meta;
        PCMData     *Data;
        uint32_t    *Samples;
    } PCMFile;
    
    // So if it's a WAV file the input file metadata reader thing will set it to WAV, then read the meta into Meta
    // Then once it comes across the equilivent of a data chunk, it will set Data to the pointer, then for extracting
    // samples it will read the type call the correct reader and extract the requested samples.
    
    void IdentifyPCMFile(BitInput *BitI, PCMFile *PCM);
    void ParseWAVFile(BitInput *BitI, PCMFile *PCM);
    void ParseW64File(BitInput *BitI, W64Header *W64);
    
#ifdef __cplusplus
}
#endif
