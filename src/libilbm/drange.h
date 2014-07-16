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

#ifndef __ILBM_DRANGE_H
#define __ILBM_DRANGE_H

#include <stdio.h>
#include <libiff/ifftypes.h>
#include <libiff/group.h>
#include <libiff/chunk.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ILBM_DRANGE_60_STEPS_PER_SECOND 16384

#define ILBM_RNG_ACTIVE 1
#define ILBM_RNG_DP_RESERVED 4
#define ILBM_RNG_FADE 8

typedef struct
{
    IFF_UByte cell;
    IFF_UByte r, g, b;
}
ILBM_DColor;

typedef struct
{
    IFF_UByte cell;
    IFF_UByte index;
}
ILBM_DIndex;

typedef struct
{
    IFF_UByte cell;
    IFF_UByte fade;
}
ILBM_DFade;

typedef struct
{
    IFF_Group *parent;
    
    IFF_ID chunkId;
    IFF_Long chunkSize;

    IFF_UByte min;
    IFF_UByte max;
    IFF_Word rate;
    IFF_Word flags;
    IFF_UByte ntrue;
    IFF_UByte nregs;
    
    ILBM_DColor *dcolor;
    ILBM_DIndex *dindex;
    
    IFF_UByte nfades;
    IFF_UByte pad;
    ILBM_DFade *dfade;
}
ILBM_DRange;

ILBM_DRange *ILBM_createDRange(IFF_Word flags);

ILBM_DColor *ILBM_addDColorToDRange(ILBM_DRange *drange);

ILBM_DIndex *ILBM_addDIndexToDRange(ILBM_DRange *drange);

ILBM_DFade *ILBM_addDFadeToDRange(ILBM_DRange *drange);

IFF_Chunk *ILBM_readDRange(FILE *file, const IFF_Long chunkSize);

int ILBM_writeDRange(FILE *file, const IFF_Chunk *chunk);

int ILBM_checkDRange(const IFF_Chunk *chunk);

void ILBM_freeDRange(IFF_Chunk *chunk);

void ILBM_printDRange(const IFF_Chunk *chunk, const unsigned int indentLevel);

int ILBM_compareDRange(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2);

#ifdef __cplusplus
}
#endif

#endif
