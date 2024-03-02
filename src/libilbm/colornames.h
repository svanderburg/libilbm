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

#ifndef __ILBM_COLORNAMES_H
#define __ILBM_COLORNAMES_H

#include <stdio.h>
#include <libiff/ifftypes.h>
#include <libiff/group.h>
#include <libiff/chunk.h>
#include <libiff/id.h>

#define ILBM_ID_CNAM IFF_MAKEID('C', 'N', 'A', 'M')

#define ILBM_CNAM_DEFAULT_SIZE 0

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    IFF_Group *parent;

    IFF_ID chunkId;
    IFF_Long chunkSize;

    IFF_UWord startingColor;
    IFF_UWord endingColor;

    unsigned int colorNamesLength;
    char **colorNames;
}
ILBM_ColorNames;

IFF_Chunk *ILBM_createColorNamesChunk(const IFF_ID chunkId, const IFF_Long chunkSize);

ILBM_ColorNames *ILBM_createColorNames(void);

void ILBM_addColorName(ILBM_ColorNames *colorNames, char *colorName);

IFF_Bool ILBM_readColorNames(FILE *file, IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed);

IFF_Bool ILBM_writeColorNames(FILE *file, const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed);

IFF_Bool ILBM_checkColorNames(const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry);

void ILBM_freeColorNames(IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry);

void ILBM_printColorNames(const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry);

IFF_Bool ILBM_compareColorNames(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ChunkRegistry *chunkRegistry);

#ifdef __cplusplus
}
#endif

#endif
