/*
 * Copyright (c) 2012 Sander van der Burg
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so, 
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "ilbm.h"
#include <libiff/iff.h>
#include "bitmapheader.h"
#include "colormap.h"
#include "cmykmap.h"
#include "colornames.h"
#include "colorrange.h"
#include "cycleinfo.h"
#include "dpiheader.h"
#include "viewport.h"
#include "grab.h"
#include "sprite.h"
#include "destmerge.h"
#include "drange.h"

#define ILBM_NUM_OF_FORM_TYPES 3
#define ILBM_NUM_OF_EXTENSION_CHUNKS 12

/* An array of all application specific chunks belonging to ILBM. They must be alphabetically sorted. */
static IFF_FormExtension ilbmFormExtension[] = {
    {"BMHD", &ILBM_readBitMapHeader, &ILBM_writeBitMapHeader, &ILBM_checkBitMapHeader, &ILBM_freeBitMapHeader, &ILBM_printBitMapHeader, &ILBM_compareBitMapHeader},
    {"CAMG", &ILBM_readViewport, &ILBM_writeViewport, &ILBM_checkViewport, &ILBM_freeViewport, &ILBM_printViewport, &ILBM_compareViewport},
    {"CCRT", &ILBM_readCycleInfo, &ILBM_writeCycleInfo, &ILBM_checkCycleInfo, &ILBM_freeCycleInfo, &ILBM_printCycleInfo, &ILBM_compareCycleInfo},
    {"CMAP", &ILBM_readColorMap, &ILBM_writeColorMap, &ILBM_checkColorMap, &ILBM_freeColorMap, &ILBM_printColorMap, &ILBM_compareColorMap},
    {"CMYK", &ILBM_readCMYKMap, &ILBM_writeCMYKMap, &ILBM_checkCMYKMap, &ILBM_freeCMYKMap, &ILBM_printCMYKMap, &ILBM_compareCMYKMap},
    {"CNAM", &ILBM_readColorNames, &ILBM_writeColorNames, &ILBM_checkColorNames, &ILBM_freeColorNames, &ILBM_printColorNames, &ILBM_compareColorNames},
    {"CRNG", &ILBM_readColorRange, &ILBM_writeColorRange, &ILBM_checkColorRange, &ILBM_freeColorRange, &ILBM_printColorRange, &ILBM_compareColorRange},
    {"DEST", &ILBM_readDestMerge, &ILBM_writeDestMerge, &ILBM_checkDestMerge, &ILBM_freeDestMerge, &ILBM_printDestMerge, &ILBM_compareDestMerge},
    {"DPI ", &ILBM_readDPIHeader, &ILBM_writeDPIHeader, &ILBM_checkDPIHeader, &ILBM_freeDPIHeader, &ILBM_printDPIHeader, &ILBM_compareDPIHeader},
    {"DRNG", &ILBM_readDRange, &ILBM_writeDRange, &ILBM_checkDRange, &ILBM_freeDRange, &ILBM_printDRange, &ILBM_compareDRange},
    {"GRAB", &ILBM_readGrab, &ILBM_writeGrab, &ILBM_checkGrab, &ILBM_freeGrab, &ILBM_printGrab, &ILBM_compareGrab},
    {"SPRT", &ILBM_readSprite, &ILBM_writeSprite, &ILBM_checkSprite, &ILBM_freeSprite, &ILBM_printSprite, &ILBM_compareSprite}
};

/* An array of all form types this library should parse. They must be alphabetically sorted. */
static IFF_Extension extension[] = {
    {"ACBM", ILBM_NUM_OF_EXTENSION_CHUNKS, ilbmFormExtension},
    {"ILBM", ILBM_NUM_OF_EXTENSION_CHUNKS, ilbmFormExtension},
    {"PBM ", ILBM_NUM_OF_EXTENSION_CHUNKS, ilbmFormExtension}
};

IFF_Chunk *ILBM_read(const char *filename)
{
    return IFF_read(filename, extension, ILBM_NUM_OF_FORM_TYPES);
}

IFF_Chunk *ILBM_readFd(FILE *file)
{
    return IFF_readFd(file, extension, ILBM_NUM_OF_FORM_TYPES);
}

int ILBM_writeFd(FILE *file, const IFF_Chunk *chunk)
{
    return IFF_writeFd(file, chunk, extension, ILBM_NUM_OF_FORM_TYPES);
}

int ILBM_write(const char *filename, const IFF_Chunk *chunk)
{
    return IFF_write(filename, chunk, extension, ILBM_NUM_OF_FORM_TYPES);
}

int ILBM_check(const IFF_Chunk *chunk)
{
    return IFF_check(chunk, extension, ILBM_NUM_OF_FORM_TYPES);
}

void ILBM_free(IFF_Chunk *chunk)
{
    IFF_free(chunk, extension, ILBM_NUM_OF_FORM_TYPES);
}

void ILBM_print(const IFF_Chunk *chunk, const unsigned int indentLevel)
{
    IFF_print(chunk, 0, extension, ILBM_NUM_OF_FORM_TYPES);
}

int ILBM_compare(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2)
{
    return IFF_compare(chunk1, chunk2, extension, ILBM_NUM_OF_FORM_TYPES);
}
