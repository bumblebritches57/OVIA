#include "../include/libPCM.h"

#include "../include/WAVCommon.h"
#include "../include/W64Common.h"
#include "../include/AIFCommon.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    
    // So, We need to accept a BitInput pointer, and start reading the input file to discover it's file type, then call the dedicated format metadata parser to get the info we need and verify it's raw PCM, and then line us up with the PCM samples, and wait for calls to ExtractSamples
    
    enum PCMFileTypes {
        Unknown = 0,
        WAV     = 1,
        W64     = 2,
        RF64    = 3,
        AIFF    = 4,
        AIFC    = 5,
    } PCMFileTypes;

    typedef struct PCMData {
        uint8_t   FileType;
        bool      MetadataHasBeenParsed;
        uint64_t  SampleRate;
        uint64_t  BitDepth;
        uint64_t  NumChannels;
        uint64_t  NumChannelAgnosticSamplesInBuffer;
        uint64_t *SampleArray[PCMMaxChannels];
    } PCMData;
    
    void FreePCMData(PCMData *PCMData2Free) {
        for (uint64_t Channel = 0; Channel < PCMData2Free->NumChannels; Channel++) {
            free(PCMData2Free->SampleArray[Channel]);
        }
        free(PCMData2Free);
    }
    
    // I want to just hand a file pointer here, and tell it to extract X samples (regardless of channel count)
    // So we need to parse the various chunks in the file, extract info of value into a format specific struct
    // then the ExtractSamples function needs to extract the samples and pass back the channel count, sample rate, and bit depth.
    // and the ExtractSamples function needs to handle all of that for all formats
    // So we need a struct to contain a bool that says if the metadata has been parsed and what sample we're on.
    // Actually which sample we're on is already being tracked by BitIO
    
    /*!
     @param NumSamples2Extract is the number of channel agnostic samples to read from the input file
     */
    void ExtractSamples(BitInput *BitI, PCMData *PCM, uint64_t NumSamples2Extract) {
        WAVHeader *WAV = calloc(sizeof(WAVHeader), 1);
        W64Header *W64 = calloc(sizeof(W64Header), 1);
        AIFHeader *AIF = calloc(sizeof(AIFHeader), 1);
        if (PCM->MetadataHasBeenParsed == false) {
            uint32_t Magic = ReadBits(BitI, 32);
            switch (Magic) {
                case WAVMagic:
                    ParseWAVMetadata(BitI, WAV);
                    break;
                case W64Magic:
                    ParseW64Metadata(BitI, W64);
                    break;
                case AIFMagic:
                    ParseAIFMetadata(BitI, AIF);
                    break;
                    
                default:
                    break;
            }
            if (PCM->FileType == WAV) {
                
            } else if (PCM->FileType == W64) {
                
            } else if (PCM->FileType == AIFF) {
                
            }
        } else {
            // just read the requested samples
            for (uint64_t Channel = 0; Channel < PCM->NumChannels; Channel++) {
                PCM->SampleArray[Channel] = calloc(NumSamples2Extract, sizeof(uint64_t));
                for (uint64_t Sample = 0; Sample < NumSamples2Extract; Sample++) {
                    PCM->SampleArray[Channel][Sample] = ReadBits(BitI, PCM->BitDepth);
                }
            }
        }
    }
    
    void ParseWAVFile(BitInput *BitI, WAVHeader *WAV) {
        char ErrorDescription[BitIOStringSize];
        
        uint32_t ChunkID   = ReadBits(BitI, 32);
        uint32_t ChunkSize = ReadBits(BitI, 32);
        
        switch (ChunkID) {
            case WAV_LIST:
                ParseWavLISTChunk(BitI, WAV, ChunkSize);
                break;
            case WAV_FMT:
                ParseWavFMTChunk(BitI, WAV, ChunkSize);
                break;
            case WAV_WAVE:
                SkipBits(BitI, 32);
                break;
            case WAV_DATA:
                ParseWavDATAChunk(BitI, WAV, ChunkSize);
                break;
                
            default:
                snprintf(ErrorDescription, BitIOStringSize, "Invalid ChunkID: 0x%X", ChunkID);
                Log(SYSError, "libPCM", "ParseWAVFile", ErrorDescription);
                break;
        }
    }
    
    void ParseW64File(BitInput *BitI, W64Header *W64) {
        uint32_t ChunkID   = ReadBits(BitI, 32);
        SkipBits(BitI, 96); // The rest of the GUID.
        uint64_t ChunkSize = ReadBits(BitI, 64);
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
    
    void ParseAIFFChunk(BitInput *BitI, AIFHeader *AIF) {
        
    }
    
    void ParseAIFCChunk(BitInput *BitI, AIFHeader *AIF) {
        
    }
    
    void ParseAIFFile(BitInput *BitI, AIFHeader *AIF) {
        // if NumFrames = 0, SNSD may not exist.
        uint32_t AIFSize = ReadBits(BitI, 32);
        uint32_t ChunkID = ReadBits(BitI, 32);
        switch (ChunkID) { // If the number of sound data bytes is odd, appened a padding sample.
            case AIF_AIFF:
                
                break;
            case AIF_AIFC:
            
            default:
                break;
        }
    }
    
    uint8_t IdentifyPCMFile(BitInput *BitI) {
        uint32_t InputMagic = ReadBits(BitI, 32);
        if (InputMagic == WAV_RIFF) {
            WAVHeader *WAV = calloc(sizeof(WAVHeader), 1);
            ParseWAVFile(BitI, WAV);
        } else if (InputMagic == W64_RIFF) {
            SkipBits(BitI, 96); // Rest of the W64 RIFF GUID
            SkipBits(BitI, 64); // RIFF ChunkSize
            W64Header *W64 = calloc(sizeof(W64Header), 1);
            ParseW64File(BitI, W64);
        } else if (InputMagic == AIF_FORM) {
            AIFHeader *AIF = calloc(sizeof(AIFHeader), 1);
            ParseAIFFile(BitI, AIF);
        }
        return 0;
    }
    
#ifdef __cplusplus
}
#endif
