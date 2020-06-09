/*!
 @header              AIFCommon.h
 @author              Marcus Johnson
 @copyright           2017+
 @version             1.0.0
 @brief               This header contains code for reading and writing lossless AIF audio files
 */

#include "OVIACommon.h"

#pragma once

#ifndef OVIA_Containers_H
#define OVIA_Containers_H

#if (PlatformIO_Language == PlatformIO_LanguageIsCXX)
extern "C" {
#endif

    /*
     OVIA 2.0:

     We have OVIA create as much as possible.

     so theres two prongs to this approach:

     1: Containers/Formats:
     Specify the extensions, mime types, supported codecs, byte and bit orders, MagicIDs, ChunkIDs, chunk layout restrictions and orderings, everything about the physical format known.
     2: Codecs:
     This is for the specific data compression stuff, for example
     PNG uses Zlib so implement a Zlib encoder and decoder thats reusable anywhere, Adam7 interlacting, PNG filter types.
     FLAC uses RICE coding so implement a RICE encoder and decoder that's reusable anywhere.
     JPEG uses Huffman coding so implement a reusable Huffman encoder/decoder
     BMP is a PCM format so just dump the raw samples correctly

     */

#if (PlatformIO_Language == PlatformIO_LanguageIsCXX)
}
#endif

#endif /* OVIA_Containers_H */
