#include "../../include/Private/StreamIO/RIFFStream.h"
#include "../../../Dependencies/FoundationIO/Library/include/TextIO/LogIO.h"
#include "../../../Dependencies/FoundationIO/Library/include/MathIO.h"

#ifdef __cplusplus
extern "C" {
#endif

    void *RIFFOptions_Init(void) {
        void *Options = calloc(1, sizeof(RIFFOptions));
        return Options;
    }

    void RIFFSkipPadding(BitBuffer *BitB, uint32_t SubChunkSize) {
        if (IsOdd(SubChunkSize) == true) {
            BitBuffer_Seek(BitB, 8);
        }
    }

    uint64_t RIFF_CalculateByteRate(uint64_t NumChannels, uint8_t BitDepth, uint64_t SampleRate) {
        return NumChannels * SampleRate * (BitDepth / 8);
    }

    uint64_t RIFF_CalculateBlockAlignment(uint64_t NumChannels, uint8_t BitDepth) {
        return NumChannels * (BitDepth / 8);
    }

    Audio_ChannelMask WAV_ChannelMap2AudioMask(uint64_t ChannelMask) {
        Audio_ChannelMask AudioMask = 0;
        if (ChannelMask & 0x1) {
            AudioMask += AudioMask_FrontLeft;
        } else if (ChannelMask & 0x2) {
            AudioMask += AudioMask_FrontRight;
        } else if (ChannelMask & 0x4) {
            AudioMask += AudioMask_FrontCenter;
        } else if (ChannelMask & 0x8) {
            AudioMask += AudioMask_LFE;
        } else if (ChannelMask & 0x10) {
            AudioMask += AudioMask_RearLeft;
        } else if (ChannelMask & 0x20) {
            AudioMask += AudioMask_RearRight;
        } else if (ChannelMask & 0x40) {
            AudioMask += AudioMask_FrontCenterLeft;
        } else if (ChannelMask & 0x80) {
            AudioMask += AudioMask_FrontCenterRight;
        } else if (ChannelMask & 0x100) {
            AudioMask += AudioMask_RearCenter;
        } else if (ChannelMask & 0x200) {
            AudioMask += AudioMask_SurroundLeft;
        } else if (ChannelMask & 0x400) {
            AudioMask += AudioMask_SurroundRight;
        } else if (ChannelMask & 0x800) {
            AudioMask += AudioMask_TopCenter;
        } else if (ChannelMask & 0x1000) {
            AudioMask += AudioMask_TopFrontLeft;
        } else if (ChannelMask & 0x2000) {
            AudioMask += AudioMask_TopFrontCenter;
        } else if (ChannelMask & 0x4000) {
            AudioMask += AudioMask_TopFrontRight;
        } else if (ChannelMask & 0x8000) {
            AudioMask += AudioMask_TopRearLeft;
        } else if (ChannelMask & 0x10000) {
            AudioMask += AudioMask_TopRearCenter;
        } else if (ChannelMask & 0x20000) {
            AudioMask += AudioMask_TopRearRight;
        }
        return AudioMask;
    }

    void RIFFOptions_Deinit(void *Options) {
        RIFFOptions *PCM = Options;
        free(PCM);
    }

#ifdef __cplusplus
}
#endif
