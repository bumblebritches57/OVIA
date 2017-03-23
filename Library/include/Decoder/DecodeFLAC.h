#include "libModernFLAC.h"
#pragma once

#ifdef __cplusplus
extern "C" {
#endif
    
    
    // When decoding FLAC, give the caller the option to skip extracting the images, to help embedded devices reduce memory requirements.
    
    // I want 2 ways to handle this, the first is to just decode the whole file as quick as possible, and the second is for real time applications, which decodes all metadata, and a single frame.
    
    // for the 2nd use case, I need to make a few functions, one that reads all metadata, and another that reads a frame of audio at a time.
    // The fast as possible method will just loop through all the blocks
    
    
    
    
    
    
    // Well, in the library I need to just run through one frame of audio at a time.
    // So, let's set it all up so all the user metadata is in one struct, and all the audio data is in another.
    
    typedef struct FLACDecoder {
        bool      SeekTableIsPresent;
        bool      CuesheetIsPresent;
        bool      VorbisCommentIsPresent;
        bool      PictureIsPresent;
        bool      LastMetadataBlock;
        FLACMeta *Meta;
        FLACData *Data;
        int64_t   DecodedSamples[FLACMaxSamplesInBlock];
    } FLACDecoder;
    
    void    InitFLACDecoder(FLACDecoder *FLAC);
    
    void    FLACReadFrame(BitInput *BitI, FLACDecoder *FLAC);
    
    void    FLACReadSubFrame(BitInput *BitI, FLACDecoder *FLAC, uint8_t Channel);
    
    void    FLACDecodeSubFrameVerbatim(BitInput *BitI, FLACDecoder *FLAC);
    
    void    FLACDecodeSubFrameConstant(BitInput *BitI, FLACDecoder *FLAC);
    
    void    FLACDecodeSubFrameFixed(BitInput *BitI, FLACDecoder *FLAC);
    
    void    FLACDecodeSubFrameLPC(BitInput *BitI, FLACDecoder *FLAC, uint8_t Channel);
    
    void    FLACDecodeResidual(BitInput *BitI, FLACDecoder *FLAC);
    
    void    FLACDecodeRice1Partition(BitInput *BitI, FLACDecoder *FLAC);
    
    void    FLACDecodeRice2Partition(BitInput *BitI, FLACDecoder *FLAC);
    
    void    FLACBitDepth(FLACDecoder *FLAC);
    
    void    FLACSampleRate(BitInput *BitI, FLACDecoder *FLAC);
    
    uint8_t GetBlockSizeInSamples(uint8_t BlockSize);
    
    void    FLACReadStream(BitInput *BitI, FLACDecoder *FLAC);
    
    void    FLACParseMetadata(BitInput *BitI, FLACDecoder *FLAC);
    
    void    FLACParseStreamInfo(BitInput *BitI, FLACDecoder *FLAC);
    
    void    FLACSkipPadding(BitInput *BitI, FLACDecoder *FLAC);
    
    void    FLACSkipCustom(BitInput *BitI, FLACDecoder *FLAC);
    
    void    FLACParseSeekTable(BitInput *BitI, FLACDecoder *FLAC);
    
    void    FLACParseVorbisComment(BitInput *BitI, FLACDecoder *FLAC);
    
    void    FLACParseCuesheet(BitInput *BitI, FLACDecoder *FLAC);
    
    void    FLACParsePicture(BitInput *BitI, FLACDecoder *FLAC);
    
#ifdef __cplusplus
}
#endif