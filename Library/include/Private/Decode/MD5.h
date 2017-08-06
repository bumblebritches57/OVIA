#include <stdbool.h>
#include <stdint.h>

#pragma once

#ifndef LIBBITIO_MD5_H
#define LIBBITIO_MD5_H

#ifdef __cplusplus
extern "C" {
#endif
    
    /*!
     @abstract Generates MD5 hash from Buffer data.
     @param PadData if set to one, we will pad each block, if set to 0 we won't.
     */
    char *GenerateMD5(uint8_t *Buffer, size_t BufferSize, bool PadData, char *MD5String);
    
    bool VerifyMD5(uint8_t *Buffer, size_t BufferSize, bool PadData, char *MD5String, char *PrecomputedMD5);
    
#ifdef __cplusplus
}
#endif

#endif /* LIBBITIO_MD5_H */
