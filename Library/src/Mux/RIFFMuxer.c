#include "../../include/Private/OVIACommon.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    /*
     
     How do we handle containers that can support the same codec?
     
     for example: RIFF, BWF, RF64, can all support PCM audio, and are more or less interchangeable.
     
     */
    
    void RIFFMuxerWriteHeader(BitBuffer *BitB) {
        if (BitB != NULL) {
            BitBuffer_WriteBits(BitB, LSByteFirst, LSBitFirst, 32, 0x52494646);
            
            // Write the size of the stream, if it's over 2GB convert to RF64 aka write size as 0xFFFFFFFF and write a DS64 chunk
        } else {
            Log(Log_DEBUG, FoundationIOFunctionName, UTF8String(""));
        }
    }
    
    static const OVIAMuxer RIFFMuxer = {
        .MediaType             = MediaType_Container,
        .ContainerID           = ContainerID_RIFF,
    };
    
#ifdef __cplusplus
}
#endif
