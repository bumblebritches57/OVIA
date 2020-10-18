#include "../include/StatisticsIO.h"
#include "../../Dependencies/FoundationIO/Library/include/MathIO.h" /* Included for Exponentiate */

#if (PlatformIO_Language == PlatformIO_LanguageIsCXX)
extern "C" {
#endif

    typedef struct Statistics { // Needs to work with 1D lines for PNG filtering, and 2D images for JPEG predictors
        void             *Statistics;
        uint64_t          NumElements;
        Statistics_Types  Type;
    } Statistics;

    static uint8_t Statistics_TypeSizeInBytes(Statistics_Types Type) {
        return(Type & 0x3C) >> 2;
    }

    Statistics *Statistics_Init(Statistics_Types Type, uint64_t NumElements) {
        Statistics *Stats    = calloc(1, sizeof(Statistics));
        if (Stats != NULL) {
            uint64_t MaxElements = 0ULL;
            if ((Type & Type_Integer8) == Type_Integer8) {
                MaxElements      = 256;
            } else if ((Type & Type_Integer16) == Type_Integer16) {
                MaxElements      = 65536;
            } else if ((Type & Type_Integer8) == Type_Integer8) {
                MaxElements      = 4294967296;
            } else if ((Type & Type_Integer8) == Type_Integer8) {
                MaxElements      = 18446744073709551615;
            }

            if (NumElements > 0 && NumElements < MaxElements) {
                Stats->Statistics      = calloc(NumElements, Statistics_TypeSizeInBytes(Stats->Type));
                if (Stats->Statistics != NULL) {
                    Stats->NumElements = MaxElements;
                    Stats->Type        = Type;
                }
            } else {
                Stats->Statistics      = calloc(MaxElements, Statistics_TypeSizeInBytes(Stats->Type));
                if (Stats->Statistics != NULL) {
                    Stats->NumElements = MaxElements;
                    Stats->Type        = Type;
                }
            }
        }
        return Stats;
    }

    /*
     So the general idea here:
     Create a Histogram based on Samples that way we can compare how well things compress with various predictors, without actually compressing things by seeing how well permutations perform using cluster analysis.

     For example: you have an image, you test each line line of the image that will become a PNG file with all of PNGs line filters, using the best performing one for each line.

     You can tell which one performed less based on two fators:

     The first factor is the number of seperate, non-zero elements in the histogram, less elements means better compression.

     the second factor is the relative weight of each factor, where the relative difference in values, one element with a higher count for the same indexes means more compressability.

     for example let's say there are histograms from the same image and therefore the same elements.

     Both histograms have 10 non-zero elements, they look like this:

     1:

     {47, 93, 108, 21, 20, 9, 7, 4, 2, 1}

     2:

     {192, 68, 30, 9, 7, 4, 2, 1, 1, 6}

     the second one compresses better because the count of values is more constrained.
     */

    /*
     Types:

     Just because the input values may be 8 bit, doesn't mean we can use an 8 bit variable to store the array.

     We need to use whatever type can hold as many elements as we have.

     So if theres a million elements in an image, we need a type that can hold a million variables for the counts.


     */

#if (PlatformIO_Language == PlatformIO_LanguageIsCXX)
}
#endif /* Extern C */
