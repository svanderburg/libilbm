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

#ifndef __ILBM_BITMAPHEADER_H
#define __ILBM_BITMAPHEADER_H

#include <stdio.h>
#include <libiff/ifftypes.h>
#include <libiff/group.h>
#include <libiff/chunk.h>
#include <libiff/id.h>

#define ILBM_ID_BMHD IFF_MAKEID('B', 'M', 'H', 'D')

#define ILBM_BMHD_DEFAULT_SIZE (2 * sizeof(IFF_UWord) + 2 * sizeof(IFF_Word) + 4 * sizeof(IFF_UByte) + sizeof(IFF_UWord) + 2 * sizeof(IFF_UByte) + 2 * sizeof(IFF_Word))

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    ILBM_MSK_NONE = 0,
    ILBM_MSK_HAS_MASK = 1,
    ILBM_MSK_HAS_TRANSPARENT_COLOR = 2,
    ILBM_MSK_LASSO = 3
}
ILBM_Masking;

typedef enum
{
    ILBM_CMP_NONE = 0,
    ILBM_CMP_BYTE_RUN = 1
}
ILBM_Compression;

typedef struct
{
    IFF_Group *parent;

    IFF_ID chunkId;
    IFF_Long chunkSize;

    IFF_UWord w, h;
    IFF_Word x, y;
    IFF_UByte nPlanes;
    IFF_UByte masking;
    IFF_UByte compression;
    IFF_UByte pad1;
    IFF_UWord transparentColor;
    IFF_UByte xAspect, yAspect;
    IFF_Word pageWidth, pageHeight;
}
ILBM_BitMapHeader;

IFF_Chunk *ILBM_createBitMapHeaderChunk(const IFF_ID chunkId, const IFF_Long chunkSize);

ILBM_BitMapHeader *ILBM_createBitMapHeader(void);

IFF_Bool ILBM_readBitMapHeader(FILE *file, IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed);

IFF_Bool ILBM_writeBitMapHeader(FILE *file, const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed);

IFF_Bool ILBM_checkBitMapHeader(const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry);

void ILBM_freeBitMapHeader(IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry);

void ILBM_printBitMapHeader(const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry);

IFF_Bool ILBM_compareBitMapHeader(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ChunkRegistry *chunkRegistry);

unsigned int ILBM_calculateNumOfColors(const ILBM_BitMapHeader *bitMapHeader);

#ifdef __cplusplus
}
#endif

#endif
