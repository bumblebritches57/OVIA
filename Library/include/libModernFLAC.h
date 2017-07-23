#include "../../Dependencies/libPCM/Dependencies/BitIO/libBitIO/include/BitIO.h"

#include "../../Dependencies/libPCM/libPCM/include/libPCM.h"

#pragma once

#ifndef LIBMODERNFLAC_LIBMODERNFLAC_H
#define LIBMODERNFLAC_LIBMODERNFLAC_H

#ifdef __cplusplus
extern "C" {
#endif

    enum FLACConstants {
        FLACMagic                                                   = 0x664c6143, // fLaC
        FLACFrameMagic                                              =     0x3FFE,
        FLACMaxChannels                                             =          8,
        FLACMaxSamplesInBlock                                       =      65535,
        FLACMaxChannelsInBlock                                      =          1,
        FLACMaxBitDepth                                             =         32,
        FLACMaxSampleRate                                           =     655350,
        FLACMaxMIMEString                                           =         32,
        FLACMaxPicDescriptionString                                 =        128,
        FLACMaxPicBuffer                                            =    6750000, // 1500x1500 max
        FLACMD5Size                                                 =         16,
        FLACMaxCueTracks                                            =        100,
        FLACMaxVorbisComments                                       =       1024,
        FLACMaxVorbisCommentSize                                    =       1024,
        FLACMaxSeekPoints                                           =        255, // 100 points in a 5 hour movie at 5 minutes.
        FLACVorbisMaxComments                                       =        100,
        FLACISRCSize                                                =         12,
        FLACMaxLPCCoefficents                                       =         32,
        FLACMedizCatalogNumberSize                                  =        128,
        FLACMaxCoeffs                                               =        882,
        FLACMaxRicePartitions                                       =         16,
    };

    enum FLACPictureTypes {
        Other                                                       =          0,
        FileIcon                                                    =          1, // 32x32, PNG only
        OtherFileIcon                                               =          2,
        FrontCover                                                  =          3, // main album art
        BackCover                                                   =          4,
        LeafletPage                                                 =          5,
        LabelSide                                                   =          6, // no clue
        LeadPerformer                                               =          7,
        Performer                                                   =          8,
        Conducter                                                   =          9,
        BandOrOrchestra                                             =         10,
        Composer                                                    =         11,
        Writer                                                      =         12,
        RecordingLocation                                           =         13,
        DuringRecording                                             =         14,
        DuringPerformance                                           =         15,
        Screenshot                                                  =         16,
        XiphLogo                                                    =         17,
        Illustration                                                =         18,
        BandLogo                                                    =         19,
        PublisherLogo                                               =         20,
    };

    enum FLACSpeakerLayout {
        Mono                                                        =          0,
        Left_Right                                                  =          1,
        Left_Right_Center                                           =          2,
        Left_Right_BackLeft_BackRight                               =          3,
        Left_Right_Center_BackLeft_BackRight                        =          4,
        Left_Right_Center_LFE_BackLeft_BackRight                    =          5,
        Left_Right_Center_LFE_BackCenter_SideLeft_SideRight         =          6,
        Left_Right_Center_LFE_BackLeft_BackRight_SideLeft_SideRight =          7,
        Left_Diff                                                   =          8,
        Right_Diff                                                  =          9,
        Average_Diff                                                =         10,
    };

    enum FLACPredictionTypes {
        Verbatim                                                    =          0,
        Constant                                                    =          1,
        FixedLinear                                                 =          2,
        FIRLinear                                                   =          3,
    };

    enum FLACResidualTypes {
        Simple                                                      =          0,
        Complex                                                     =          1,
    };

    enum FLACMetadataTypes {
        StreamInfo                                                  =          0, // 0x30 = 0 in ascii, 0x31 = 1, etc.
        Padding                                                     =          1,
        Custom                                                      =          2, // Application aka App specific
        SeekTable                                                   =          3,
        VorbisComment                                               =          4,
        Cuesheet                                                    =          5,
        Picture                                                     =          6,
    };

    enum FLACSubFrameTypes {
        Subframe_Constant                                           =          0,
        Subframe_Verbatim                                           =          1,
        Subframe_Fixed                                              =          8,
        Subframe_LPC                                                =         63,
    };

    enum FLACBlockType {
        FixedBlockSize                                              =          0,
        VariableBlockSize                                           =          1,
    };

    enum FLACRicePartitionType {
        RICE1                                                       =          0,
        RICE2                                                       =          1,
    };
    
    typedef struct FLACEncoder EncodeFLAC;
    
    typedef struct FLACDecoder DecodeFLAC;
    
    EncodeFLAC *InitFLACEncoder(void);
    
    DecodeFLAC *InitFLACDecoder(void);
    
    void        CloseFLACEncoder(EncodeFLAC *Enc);
    
    void        CloseFLACDecoder(DecodeFLAC *Dec);
    
    /* Decode specific functions */
    
    void        FLACReadFrame(BitInput *BitI, DecodeFLAC *Dec);
    
    void        FLACReadSubFrame(BitInput *BitI, DecodeFLAC *Dec, uint8_t Channel);
    
    void        FLACDecodeSubFrameVerbatim(BitInput *BitI, DecodeFLAC *Dec);
    
    void        FLACDecodeSubFrameConstant(BitInput *BitI, DecodeFLAC *Dec);
    
    void        FLACDecodeSubFrameFixed(BitInput *BitI, DecodeFLAC *Dec);
    
    void        FLACDecodeSubFrameLPC(BitInput *BitI, DecodeFLAC *Dec, uint8_t Channel);
    
    void        DecodeFLACesidual(BitInput *BitI, DecodeFLAC *Dec);
    
    void        DecodeFLACice1Partition(BitInput *BitI, DecodeFLAC *Dec);
    
    void        DecodeFLACice2Partition(BitInput *BitI, DecodeFLAC *Dec);
    
    void        FLACBitDepth(DecodeFLAC *Dec);
    
    void        FLACSampleRate(BitInput *BitI, DecodeFLAC *Dec);
    
    uint8_t     GetBlockSizeInSamples(uint8_t BlockSize);
    
    void        FLACReadStream(BitInput *BitI, DecodeFLAC *Dec);
    
    void        FLACParseMetadata(BitInput *BitI, DecodeFLAC *Dec);
    
    void        FLACParseStreamInfo(BitInput *BitI, DecodeFLAC *Dec);
    
    void        FLACSkipPadding(BitInput *BitI, DecodeFLAC *Dec);
    
    void        FLACSkipCustom(BitInput *BitI, DecodeFLAC *Dec);
    
    void        FLACParseSeekTable(BitInput *BitI, DecodeFLAC *Dec);
    
    void        FLACParseVorbisComment(BitInput *BitI, DecodeFLAC *Dec);
    
    void        FLACParseCuesheet(BitInput *BitI, DecodeFLAC *Dec);
    
    void        FLACParsePicture(BitInput *BitI, DecodeFLAC *Dec);
    
    /* Encode specific functions */
    
    int8_t      EncodeFLACFile(PCMFile *PCM, BitOutput *BitO, EncodeFLAC *Enc);
    
    
#ifdef __cplusplus
}
#endif

#endif /* LIBMODERNFLAC_LIBMODERNFLAC_H */
