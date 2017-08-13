#include <stdbool.h>
#include <stdint.h>

#pragma once

#ifndef LIBMODERNPNG_libModernPNGAdler32_H
#define LIBMODERNPNG_libModernPNGAdler32_H

#ifdef __cplusplus
extern "C" {
#endif
    
    /*!
     @abstract                                    "Creates Adler32 checksum from input data".
     @return                                      "Returns the Adler32 data from the data input".
     @param             Data                      "Pointer to the data to generate the Adler hash from".
     @param             DataSize                  "Size of data".
     */
    uint32_t            GenerateAdler32(const uint8_t *Data, const uint64_t DataSize);
    
    /*!
     @abstract                                    "Generates Adler32 from the input data, and compares it to the submitted checksum".
     @return                                      "Returns whether the input data matched the provided checksum or not".
     @param             Data                      "Pointer to the data to generate the Adler hash from".
     @param             DataSize                  "Size of data".
     @param             EmbeddedAdler32           "Embedded Adler32 to compare the generated one to".
     */
    bool                VerifyAdler32(const uint8_t *Data, const uint64_t DataSize, const uint32_t EmbeddedAdler32);
    
#ifdef __cplusplus
}
#endif

#endif /* LIBMODERNPNG_libModernPNGAdler32_H */
