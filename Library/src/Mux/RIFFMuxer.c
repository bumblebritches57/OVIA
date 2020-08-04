#include "../../include/Private/OVIACommon.h"

#if (PlatformIO_Language == PlatformIO_LanguageIsCXX)
extern "C" {
#endif
    
    /*
     
     How do we handle containers that can support the same codec?
     
     for example: RIFF, BWF, RF64, can all support PCM audio, and are more or less interchangeable.
     
     */
    
    void RIFFMuxerWriteHeader(BitBuffer *BitB) {
        if (BitB != NULL) {
            BitBuffer_WriteBits(BitB, ByteOrder_LSByteIsNearest, BitOrder_LSBitIsNearest, 32, 0x52494646);
            
            // Write the size of the stream, if it's over 2GB convert to RF64 aka write size as 0xFFFFFFFF and write a DS64 chunk
        } else {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String(""));
        }
    }
    /*
    static const OVIAMuxer RIFFMuxer = {
        .MediaType             = MediaType_Container,
        .ContainerID           = ContainerID_RIFF,
    };
    */
#if (PlatformIO_Language == PlatformIO_LanguageIsCXX)
}
#endif
