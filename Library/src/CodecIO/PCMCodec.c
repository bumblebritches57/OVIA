#include "../../../Dependencies/FoundationIO/Library/include/TextIO/LogIO.h"
#include "../../include/StreamIO.h"
#include "../../include/Private/MediaIO.h"

#if (PlatformIO_Language == PlatformIO_LanguageIsCXX)
extern "C" {
#endif

    /*
     This is basically a stub for WAV/AIF/W64/etc formats.

     So we need to be able to extract a specified number of chanell agnostic samples from a basic format.

     and for images extract samples from the specific format.
     */


    uint64_t SamplesIO_ExtractAudioSamplesFromFile(FILE *AudioFile, Audio2DContainer *Container, BufferIO_ByteOrders ByteOrder, BufferIO_BitOrders BitOrder, uint64_t NumSamples2Extract) {
        uint64_t NumSamplesExtracted = 0ULL;
        if (AudioFile != NULL && Container != NULL && NumSamples2Extract > 0) {
            uint8_t SampleSize = 0;
            /*
             Get the BitWidth, BitPadding, byte and bit orders and boom
             */
        } else if (AudioFile == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("AudioFile Pointer is NULL"));
        } else if (Container == NULL) {
            Log(Severity_DEBUG, PlatformIO_FunctionName, UTF8String("Container Pointer is NULL"));
        } else if (NumSamples2Extract == 0) {
            Log(Severity_USER, PlatformIO_FunctionName, UTF8String("NumSamples2Extract: %llu is Invalud"), NumSamples2Extract);
        }
        return NumSamplesExtracted;
    }


#if (PlatformIO_Language == PlatformIO_LanguageIsCXX)
}
#endif
