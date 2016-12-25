#include "../include/DecodeFLAC.h"
#include "../include/EncodeFLAC.h"

void FLACDecodeFile(int argc, const char *argv[]) {
    BitInput    *BitI  = calloc(sizeof(BitInput), 1);
    BitOutput   *BitO  = calloc(sizeof(BitOutput), 1);
    ErrorStatus *Error = calloc(sizeof(ErrorStatus), 1);
    FLACFile    *FLAC  = calloc(sizeof(FLACFile), 1);
    InitBitInput(BitI, Error, argc, argv);
    InitBitOutput(BitO, Error, argc, argv);
    InitDecodeFLACFile(FLAC);
    
    uint32_t FileMagic = ReadBits(BitI, 32);
    
    if (FileMagic != FLACMagic) {
        // Not a FLAC file
        char Error[1024];
        snprintf(Error, 1024, "Not a FLAC file, magic was: 0x%X\n", FileMagic);
        Log(SYSError, "NewFLAC", "FLACDecodeFile", Error);
    } else {
        SkipBits(BitI, 32);
        FLACReadMetadata(BitI, FLAC);
        while (BitI->FileSize > 0) {
            FLACReadFrame(BitI, FLAC);
        }
    }
}

















int main(int argc, const char *argv[]) {
    BitInput    *BitI  = calloc(sizeof(BitInput), 1);
    BitOutput   *BitO  = calloc(sizeof(BitOutput), 1);
    ErrorStatus *Error = calloc(sizeof(ErrorStatus), 1);
    FLACFile    *FLAC  = calloc(sizeof(FLACFile), 1);
    InitBitInput(BitI, Error, argc, argv);
    InitBitOutput(BitO, Error, argc, argv);
    InitFLACFile(FLAC);

    /*
    uint64_t What[8] = {0};
    for (uint8_t Run = 0; Run < 8; Run++) { // Loop over ReadBits to test it.
        uint8_t Bits2Read = MyRand(1, 64);
        uint64_t OutputData = ReadBits(BitI, Bits2Read);
        printf("ReadBits Test: Bits2Read = %d, OutputData = 0x%llx", Bits2Read, What[Run]);
    }
     */

    //uint32_t DecodedAudio[FLACMaxChannels][FLACMaxSamplesInBlock];

    uint32_t   *DecodedAudio;

    uint32_t InputfileMagic = ReadBits(BitI, 32); // SwapEndian32(ReadBits(BitI, 32))

    if (InputfileMagic == FLACMagic) {
        while (BitI->FilePosition + BitI->BitsAvailable < Bytes2Bits(BitI->FileSize)) {
            uint16_t Signature = PeekBits(BitI, 14);
            if (Signature == FLACFrameMagic) {
                SkipBits(BitI, 14);
                FLACReadFrame(BitI, FLAC);
            } else {
                FLACReadMetadata(BitI, FLAC, FLACMaxPicBuffer);
            }
        }
    } else {
        // error
        char Description[BitIOStringSize];
        snprintf(Description, BitIOStringSize, "Input file is not a FLAC file.\n");
        Log(SYSPanic, "NewFLAC", "main", Description);
        exit(EXIT_FAILURE);
    }


/*
    bool     IsLastMetadataBlock = 0;

    if (InputfileMagic == FLACMagic) {
        for (uint64_t Byte = 0; Byte < BitI->FileSize; Byte++) {
            IsLastMetadataBlock      = ReadBits(BitI, 1);
            if (IsLastMetadataBlock == false) { // Loop over the first "lastmetadatablock", because it's still metadata...
                FLACReadMetadata(BitI, FLAC, FLACMaxPicBuffer);
            } else {
                FLACReadFrame(BitI, FLAC);
            }
        }
    } else if (InputfileMagic == FLACFrameMagic) {
        FLACReadFrame(BitI, FLAC);
    }


    } else if (InputfileMagic == WAVEMagic) {
        //FLACEncodeFile(argc, argv);
    } else {
        char ErrorDescription[BitIOStringSize];
        snprintf(ErrorDescription, BitIOStringSize, "Invalid Input file Magic: 0x%x", InputfileMagic);
        Log(SYSCritical, NULL, InvalidMarker, "NewFLAC", "main", ErrorDescription);
    }
 */
    return EXIT_SUCCESS;
}
