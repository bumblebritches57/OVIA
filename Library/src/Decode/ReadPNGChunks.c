#include "../../../Dependencies/BitBO/libBitBO/include/BitBO.h"

#include "../../include/libModernPNG.h"
#include "../../include/Encode/EncodePNG.h"
#include "../../include/PNGTypes.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    // Hmm, for parsing chunks I should have basically a PNG muxer that pops out bit buffers with the data for each chunk...
    // So, we need to 
    
    void ParseIHDR(BitBuffer *BitB, DecodePNG *Dec, uint32_t ChunkSize) {
        Dec->iHDR->Width          = ReadBits(BitB, 32, true);
        Dec->iHDR->Height         = ReadBits(BitB, 32, true);
        Dec->iHDR->BitDepth       = ReadBits(BitB, 8, true);
        Dec->iHDR->ColorType      = ReadBits(BitB, 8, true);
        if (Dec->iHDR->ColorType == 1 || Dec->iHDR->ColorType == 5 || Dec->iHDR->ColorType >= 7) {
            Log(LOG_ALERT, "ModernPNG", "ParseiHDR", "Invalid color type: %d", Dec->iHDR->ColorType);
        }
        Dec->iHDR->Compression    = ReadBits(BitB, 8, true);
        Dec->iHDR->FilterMethod   = ReadBits(BitB, 8, true);
        Dec->iHDR->IsInterlaced   = ReadBits(BitB, 8, true);
        SkipBits(BitB, Bytes2Bits(ChunkSize - 13)); // incase the header is longer.
        uint32_t CRC              = ReadBits(BitB, 32, true);
        //VerifyCRC(Dec->iHDR, ChunkSize, 1, 1, CRC);
    }
    
    void ParsePLTE(BitBuffer *BitB, DecodePNG *Dec, uint32_t ChunkSize) { // Palette
        if (Dec->iHDR->BitDepth > 8) { // INVALID
            Log(LOG_ALERT, "ModernPNG", "ParsePLTE", "Invalid bit depth %d and palette combination\n", Dec->iHDR->BitDepth);
            SkipBits(BitB, Bytes2Bits(ChunkSize));
        } else {
            uint8_t **Palette = NULL;
            
            if (Dec->iHDR->ColorType == PNG_PalettedRGB || Dec->iHDR->ColorType == PNG_RGB) {
                Palette = calloc(3 * Dec->PLTE->NumEntries, 1);
            } else if (Dec->iHDR->ColorType == PNG_RGBA) {
                Palette = calloc(4 * Dec->PLTE->NumEntries, 1);
            }
            
            
            for (uint8_t Channel = 0; Channel < ChannelsPerColorType[Dec->iHDR->ColorType]; Channel++) {
                for (uint16_t Pixel = 0; Pixel < ChunkSize / 3; Pixel++) {
                    Palette[Channel][Pixel] = ReadBits(BitB, Dec->iHDR->BitDepth, true);
                }
            }
        }
        Dec->PLTE->CRC = ReadBits(BitB, 32, true);
    }
    
    void ParseTRNS(BitBuffer *BitB, DecodePNG *Dec, uint32_t ChunkSize) { // Transparency
        Dec->tRNS->NumEntries = ReadBits(BitB, 32, true);
        uint16_t **Entries = calloc(1, Dec->tRNS->NumEntries);
        for (uint8_t Color = 0; Color < ChannelsPerColorType[Dec->iHDR->ColorType]; Color++) {
            Entries[Color] = ReadBits(BitB, Bits2Bytes(Dec->iHDR->BitDepth, true), true);
        }
        Dec->tRNS->Palette = Entries;
        Dec->tRNS->CRC = ReadBits(BitB, 32, true);
    }
    
    void ParseBKGD(BitBuffer *BitB, DecodePNG *Dec, uint32_t ChunkSize) { // Background
        for (uint8_t Entry = 0; Entry < 3; Entry++) {
            Dec->bkGD->BackgroundPaletteEntry[Entry] = ReadBits(BitB, 8, true);
        }
        Dec->bkGD->CRC = ReadBits(BitB, 32, true);
    }
    
    void ParseCHRM(BitBuffer *BitB, DecodePNG *Dec, uint32_t ChunkSize) { // Chromaticities
        Dec->cHRM->WhitePointX = ReadBits(BitB, 32, true);
        Dec->cHRM->WhitePointY = ReadBits(BitB, 32, true);
        Dec->cHRM->RedX        = ReadBits(BitB, 32, true);
        Dec->cHRM->RedY        = ReadBits(BitB, 32, true);
        Dec->cHRM->GreenX      = ReadBits(BitB, 32, true);
        Dec->cHRM->GreenY      = ReadBits(BitB, 32, true);
        Dec->cHRM->BlueX       = ReadBits(BitB, 32, true);
        Dec->cHRM->BlueY       = ReadBits(BitB, 32, true);
        Dec->cHRM->CRC         = ReadBits(BitB, 32, true);
    }
    
    void ParseGAMA(BitBuffer *BitB, DecodePNG *Dec, uint32_t ChunkSize) { // Gamma
        Dec->gAMA->Gamma = ReadBits(BitB, 32, true);
        Dec->gAMA->CRC = ReadBits(BitB, 32, true);
    }
    
    void ParseOFFS(BitBuffer *BitB, DecodePNG *Dec, uint32_t ChunkSize) { // Image Offset
        Dec->oFFs->XOffset       = ReadBits(BitB, 32, true);
        Dec->oFFs->YOffset       = ReadBits(BitB, 32, true);
        Dec->oFFs->UnitSpecifier = ReadBits(BitB, 8, true);
        Dec->oFFs->CRC           = ReadBits(BitB, 32, true);
    }
    
    void ParsePHYS(BitBuffer *BitB, DecodePNG *Dec, uint32_t ChunkSize) { // Aspect ratio, Physical pixel size
        Dec->pHYs->PixelsPerUnitXAxis = ReadBits(BitB, 32, true);
        Dec->pHYs->PixelsPerUnitYAxis = ReadBits(BitB, 32, true);
        Dec->pHYs->UnitSpecifier      = ReadBits(BitB, 8, true);
        Dec->pHYs->CRC                = ReadBits(BitB, 32, true);
    }
    
    void ParseSCAL(BitBuffer *BitB, DecodePNG *Dec, uint32_t ChunkSize) { // Physical Scale
        char Width[80];
        Dec->sCAL->UnitSpecifier = ReadBits(BitB, 8, true);
        while (PeekBits(BitB, 8, true) != 0x00) {
            for (uint8_t Byte = 0; Byte < 80; Byte++) {
                Width[Byte] = ReadBits(BitB, 8, true);
            }
        }
        Dec->sCAL->PixelWidth = &*Width;
        
        bool Type    = ReadBits(BitB, 8, true);
        uint32_t X   = ReadBits(BitB, 32, true);
        uint32_t Y   = ReadBits(BitB, 32, true);
        uint32_t CRC = ReadBits(BitB, 32, true);
    }
    
    void ParsePCAL(BitBuffer *BitB, DecodePNG *Dec, uint32_t ChunkSize) {
        char CalibrationName[80];
        while (PeekBits(BitB, 8, true) != 0x0) {
            for (uint8_t Byte = 0; Byte < 80; Byte++) {
                CalibrationName[Byte] = ReadBits(BitB, 8, true);
            }
        }
        Dec->pCAL->CalibrationName = CalibrationName;
        
    }
    
    void ParseSBIT(BitBuffer *BitB, DecodePNG *Dec, uint32_t ChunkSize) { // Significant bits per sample
        Dec->sBIT->Red                   = ReadBits(BitB, 8, true);
        Dec->sBIT->Green                 = ReadBits(BitB, 8, true);
        Dec->sBIT->Blue                  = ReadBits(BitB, 8, true);
        Dec->sBIT->CRC                   = ReadBits(BitB, 32, true);
    }
    
    void ParseSRGB(BitBuffer *BitB, DecodePNG *Dec, uint32_t ChunkSize) {
        Dec->sRGB->RenderingIntent       = ReadBits(BitB, 8, true);
        Dec->sRGB->CRC                   = ReadBits(BitB, 32, true);
    }
    
    void ParseSTER(BitBuffer *BitB, DecodePNG *Dec, uint32_t ChunkSize) {
        Dec->Is3D = true;
        Dec->sTER->StereoType            = ReadBits(BitB, 8, true);
        uint32_t CRC                     = ReadBits(BitB, 32, true);
        
        // No matter what StereoType is used, both images are arranged side by side, and the left edge is aligned on a boundary of the 8th column in case interlacing is used.
        // The two sub images must have the same dimensions after padding is removed.
        // CROSS_FUSE_LAYOUT = 0, DIVERGING_FUSE_LAYOUT = 1
        // When CROSS_FUSE_LAYOUT, the right image is on the left, and the left image on the right.
        // When DIVERGING_FUSE_LAYOUT, The left eye view is on the left, and the right eye view is on the right.
        // The Left eye view is always in index 0 of the pixel array, so we need to swap this if nessicary.
        // So, make a function that strips padding from the stream, then make a function that decodes the sTER image the way it should be.
        // The standard recommends we use CROSS_FUSE_LAYOUT, so I'll probably end up using that.
        // So, let's say we have 2 1080p images, and we want to make a single Stereoscopic PNG from them.
        //
    }
    
    void ParseTEXt(BitBuffer *BitB, DecodePNG *Dec, uint32_t ChunkSize) { // tEXt
        uint32_t CRC = ReadBits(BitB, 32, true);
    }
    
    void ParseZTXt(BitBuffer *BitB, DecodePNG *Dec, uint32_t ChunkSize) { // Compressed text
        uint32_t CRC = ReadBits(BitB, 32, true);
    }
    
    void ParseITXt(BitBuffer *BitB, DecodePNG *Dec, uint32_t ChunkSize) { // International Text
        
        uint32_t CRC = ReadBits(BitB, 32, true);
    }
    
    void ParseTIME(BitBuffer *BitB, DecodePNG *Dec, uint32_t ChunkSize) {
        Dec->tIMe->Year                  = ReadBits(BitB, 16, true);
        Dec->tIMe->Month                 = ReadBits(BitB, 8, true);
        Dec->tIMe->Day                   = ReadBits(BitB, 8, true);
        Dec->tIMe->Hour                  = ReadBits(BitB, 8, true);
        Dec->tIMe->Minute                = ReadBits(BitB, 8, true);
        Dec->tIMe->Second                = ReadBits(BitB, 8, true);
        Dec->tIMe->CRC                   = ReadBits(BitB, 32, true);
    }
    
    /* APNG */
    void ParseACTL(BitBuffer *BitB, DecodePNG *Dec, uint32_t ChunkSize) { // Animation control, part of APNG
        Dec->IsVideo = true;
        Dec->acTL->NumFrames             = ReadBits(BitB, 32, true);
        Dec->acTL->TimesToLoop           = ReadBits(BitB, 32, true); // If 0, loop forever.
        Dec->acTL->CRC                   = ReadBits(BitB, 32, true);
    }
    
    void ParseFCTL(BitBuffer *BitB, DecodePNG *Dec, uint32_t ChunkSize) { // Frame Control, part of APNG
        Dec->fcTL->FrameNum              = ReadBits(BitB, 32, true);
        Dec->fcTL->Width                 = ReadBits(BitB, 32, true);
        Dec->fcTL->Height                = ReadBits(BitB, 32, true);
        Dec->fcTL->XOffset               = ReadBits(BitB, 32, true);
        Dec->fcTL->YOffset               = ReadBits(BitB, 32, true);
        Dec->fcTL->FrameDelayNumerator   = ReadBits(BitB, 16, true);
        Dec->fcTL->FrameDelayDenominator = ReadBits(BitB, 16, true);
        Dec->fcTL->DisposeMethod         = ReadBits(BitB, 8, true);
        Dec->fcTL->BlendMethod           = ReadBits(BitB, 8, true);
        uint32_t CRC                     = ReadBits(BitB, 32, true);
    }
    /* End APNG */
    
    void ParseIDAT(BitBuffer *BitB, DecodePNG *Dec, uint32_t ChunkSize) { // IDAT
                                                                         // DecodeINFLATE
        uint32_t CRC = ReadBits(BitB, 32, true);
    }
    
    void ParseHIST(BitBuffer *BitB, DecodePNG *Dec, uint32_t ChunkSize) {
        Dec->hIST->CRC = ReadBits(BitB, 32, true);
    }
    
    void ParseICCP(BitBuffer *BitB, DecodePNG *Dec, uint32_t ChunkSize) {
        uint8_t ProfileNameSize = 0;
        ProfileNameSize = ReadBits(BitB, 8, true);
        uint32_t CRC = ReadBits(BitB, 32, true);
    }
    
    void ParseSPLT(BitBuffer *BitB, DecodePNG *Dec, uint32_t ChunkSize) {
        uint32_t CRC = ReadBits(BitB, 32, true);
    }
    
    void ParseFDAT(BitBuffer *BitB, DecodePNG *Dec, uint32_t ChunkSize) {
        // DecodeDEFLATE
        uint32_t CRC = ReadBits(BitB, 32, true);
    }
    
    uint8_t ParsePNGMetadata(BitBuffer *BitB, DecodePNG *Dec) {
        uint64_t FileMagic    = ReadBits(BitB, 64, true);
        
        if (FileMagic == PNGMagic) {
            char     ChunkID[4];
            uint32_t ChunkSize  = ReadBits(BitB, 32, true);
            ChunkID[0] = ReadBits(BitB, 8, true);
            ChunkID[1] = ReadBits(BitB, 8, true);
            ChunkID[2] = ReadBits(BitB, 8, true);
            ChunkID[3] = ReadBits(BitB, 8, true);
            uint32_t ChunkCRC   = ReadBits(BitB, 32, true);
            
            if (strcasecmp(ChunkID, "iHDR") == 0) {        // iHDR
                ParseIHDR(BitB, Dec, ChunkSize);
            } else if (strcasecmp(ChunkID, "PLTE") == 0) { // PLTE
                Dec->PLTEExists = true;
                ParsePLTE(BitB, Dec, ChunkSize);
            } else if (strcasecmp(ChunkID, "bKGD") == 0) { // bKGD
                Dec->bkGDExists = true;
                ParseBKGD(BitB, Dec, ChunkSize);
            } else if (strcasecmp(ChunkID, "cHRM") == 0) { // cHRM
                Dec->cHRMExists = true;
                ParseCHRM(BitB, Dec, ChunkSize);
            } else if (strcasecmp(ChunkID, "gAMA") == 0) { // gAMA
                Dec->gAMAExists = true;
                ParseGAMA(BitB, Dec, ChunkSize);
            } else if (strcasecmp(ChunkID, "oFFs") == 0) { // oFFs
                Dec->oFFsExists = true;
                ParseOFFS(BitB, Dec, ChunkSize);
            } else if (strcasecmp(ChunkID, "pHYs") == 0) { // pHYs
                Dec->pHYsExists = true;
                ParsePHYS(BitB, Dec, ChunkSize);
            } else if (strcasecmp(ChunkID, "sBIT") == 0) { // sBIT
                Dec->sBITExists = true;
                ParseSBIT(BitB, Dec, ChunkSize);
            } else if (strcasecmp(ChunkID, "sCAL") == 0) { // sCAL
                Dec->sCALExists = true;
                ParseSCAL(BitB, Dec, ChunkSize);
            } else if (strcasecmp(ChunkID, "sRGB") == 0) { // sRGB
                Dec->sRGBExists = true;
                ParseSRGB(BitB, Dec, ChunkSize);
            } else if (strcasecmp(ChunkID, "sTER") == 0) { // sTER
                Dec->sTERExists = true;
                ParseSTER(BitB, Dec, ChunkSize);
            } else if (strcasecmp(ChunkID, "tEXt") == 0) { // tEXt
                Dec->TextExists = true;
                ParseTEXt(BitB, Dec, ChunkSize);
            } else if (strcasecmp(ChunkID, "zTXt") == 0) { // zTXt
                Dec->TextExists = true;
                ParseZTXt(BitB, Dec, ChunkSize);
            } else if (strcasecmp(ChunkID, "iTXt") == 0) { // iTXt
                Dec->TextExists = true;
                ParseITXt(BitB, Dec, ChunkSize);
            } else if (strcasecmp(ChunkID, "tIME") == 0) { // tIME
                Dec->tIMEExists = true;
                ParseTIME(BitB, Dec, ChunkSize);
            } else if (strcasecmp(ChunkID, "tRNS") == 0) { // tRNS
                Dec->tRNSExists = true;
                ParseTRNS(BitB, Dec, ChunkSize);
            } else if (strcasecmp(ChunkID, "hIST") == 0 && Dec->PLTEExists) { // hIST
                Dec->hISTExists = true;
                ParseHIST(BitB, Dec, ChunkSize);
            } else if (strcasecmp(ChunkID, "iCCP") == 0) { // iCCP
                Dec->iCCPExists = true;
                ParseICCP(BitB, Dec, ChunkSize);
            } else if (strcasecmp(ChunkID, "pCAL") == 0) { // pCAL
                Dec->pCALExists = true;
                ParsePCAL(BitB, Dec, ChunkSize);
            } else if (strcasecmp(ChunkID, "sPLT") == 0) { // sPLT
                Dec->sPLTExists = true;
                ParseSPLT(BitB, Dec, ChunkSize);
            } else if (strcasecmp(ChunkID, "acTL") == 0) { // acTL
                Dec->acTLExists = true;
                ParseACTL(BitB, Dec, ChunkSize);
            } else if (strcasecmp(ChunkID, "fcTL") == 0) { // fcTL
                Dec->fcTLExists = true;
                ParseFCTL(BitB, Dec, ChunkSize);
            }
        } else {
            Log(LOG_CRIT, "ModernPNG", "ParsePNGMetadata", "File Magic 0x%X is not PNG, exiting\n", FileMagic);
            exit(EXIT_FAILURE);
        }
        
        return EXIT_SUCCESS;
    }
    
    uint32_t GetPNGWidth(DecodePNG *Dec) {
        return Dec->iHDR->Width;
    }
    
    uint32_t GetPNGHeight(DecodePNG *Dec) {
        return Dec->iHDR->Height;
    }
    
    uint8_t GetPNGBitDepth(DecodePNG *Dec) {
        return Dec->iHDR->BitDepth;
    }
    
    uint8_t GetPNGColorType(DecodePNG *Dec) {
        return Dec->iHDR->ColorType;
    }
    
    bool GetPNGInterlaceStatus(DecodePNG *Dec) {
        return Dec->iHDR->IsInterlaced;
    }
    
    bool IsPNGStereoscopic(DecodePNG *Dec) {
        return Dec->Is3D;
    }
    
    uint32_t GetPNGWhitepointX(DecodePNG *Dec) {
        return Dec->cHRM->WhitePointX;
    }
    
    uint32_t GetPNGWhitepointY(DecodePNG *Dec) {
        return Dec->cHRM->WhitePointY;
    }
    
    uint32_t GetPNGGamma(DecodePNG *Dec) {
        return Dec->gAMA->Gamma;
    }
    
    const char *GetPNGColorProfileName(DecodePNG *Dec) {
        return Dec->iCCP->ProfileName;
    }
    
    uint8_t *GetColorProfile(DecodePNG *Dec) {
        return Dec->iCCP->CompressedICCPProfile;
    }
    
    
    
    
    
    
    
    
    
#ifdef __cplusplus
}
#endif