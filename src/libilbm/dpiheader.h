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

#ifndef __ILBM_DPIHEADER_H
#define __ILBM_DPIHEADER_H

#include <stdio.h>
#include <libiff/ifftypes.h>
#include <libiff/group.h>
#include <libiff/chunk.h>
#include <libiff/id.h>

#define ILBM_ID_DPI IFF_MAKEID('D', 'P', 'I', ' ')

#define ILBM_DPI_DEFAULT_SIZE (2 * sizeof(IFF_UWord))

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    IFF_Group *parent;

    IFF_ID chunkId;
    IFF_Long chunkSize;

    IFF_UWord dpiX, dpiY;
}
ILBM_DPIHeader;

IFF_Chunk *ILBM_createDPIHeader(const IFF_Long chunkSize);

IFF_Bool ILBM_readDPIHeader(FILE *file, IFF_Chunk *chunk, IFF_Long *bytesProcessed);

IFF_Bool ILBM_writeDPIHeader(FILE *file, const IFF_Chunk *chunk, IFF_Long *bytesProcessed);

IFF_Bool ILBM_checkDPIHeader(const IFF_Chunk *chunk);

void ILBM_freeDPIHeader(IFF_Chunk *chunk);

void ILBM_printDPIHeader(const IFF_Chunk *chunk, const unsigned int indentLevel);

IFF_Bool ILBM_compareDPIHeader(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2);

#ifdef __cplusplus
}
#endif

#endif
