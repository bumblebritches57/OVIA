/*!
 @header          StatisticsIO.h
 @author          Marcus Johnson
 @copyright       2020+
 @version         1.0.0
 @brief           This header contains types, functions, and tables for measuring entropy of arrays.
 */

#include "../../Dependencies/FoundationIO/Library/include/PlatformIO.h"         /* Included for Platform Independence macros */

#pragma once

#ifndef OVIA_StatisticsIO_H
#define OVIA_StatisticsIO_H

#if (PlatformIO_Language == PlatformIO_LanguageIsCXX)
extern "C" {
#endif

    /*
     So, for all the basic types, we need an array that we can get pointers to and do our work

     So, if we need an 8bit array, the init function will take a parameter saying the number of elements needed for the array, if the number of elements is less than 2^ElementSizeInBits, we use it, if not we use 2^ElementSizeInBits
     */

    typedef enum Statistics_Types {
        Type_Unknown    = 0,
        Type_IsUnsigned = 1,
        Type_IsSigned   = 2,
        Type_Integer8   = 4,
        Type_Integer16  = 8,
        Type_Integer32  = 16,
        Type_Integer64  = 32,
    } Statistics_Types;

    typedef struct Statistics Statistics;

#if (PlatformIO_Language == PlatformIO_LanguageIsCXX)
}
#endif /* Extern C */

#endif /* OVIA_StatisticsIO_H */
