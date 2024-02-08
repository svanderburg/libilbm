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

#ifndef __ILBM_CMYKMAP_H
#define __ILBM_CMYKMAP_H

#include <stdio.h>
#include <libiff/ifftypes.h>
#include <libiff/group.h>
#include <libiff/chunk.h>
#include <libiff/id.h>

#define ILBM_ID_CMYK IFF_MAKEID('C', 'M', 'Y', 'K')

#define ILBM_CMYK_DEFAULT_SIZE 0

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    IFF_UByte cyan, magenta, yellow, black;
}
ILBM_CMYKRegister;

typedef struct
{
    IFF_Group *parent;

    IFF_ID chunkId;
    IFF_Long chunkSize;

    unsigned int cmykRegisterLength;
    ILBM_CMYKRegister *cmykRegister;
}
ILBM_CMYKMap;

IFF_Chunk *ILBM_createCMYKMap(const IFF_Long chunkSize);

ILBM_CMYKRegister *ILBM_addCMYKRegisterInCMYKMap(ILBM_CMYKMap *cmykMap);

IFF_Bool ILBM_readCMYKMap(FILE *file, IFF_Chunk *chunk, IFF_Long *bytesProcessed);

IFF_Bool ILBM_writeCMYKMap(FILE *file, const IFF_Chunk *chunk, IFF_Long *bytesProcessed);

IFF_Bool ILBM_checkCMYKMap(const IFF_Chunk *chunk);

void ILBM_freeCMYKMap(IFF_Chunk *chunk);

void ILBM_printCMYKMap(const IFF_Chunk *chunk, const unsigned int indentLevel);

IFF_Bool ILBM_compareCMYKMap(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2);

#ifdef __cplusplus
}
#endif

#endif
