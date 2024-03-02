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

#include "dpiheader.h"
#include <stdlib.h>
#include <libiff/field.h>
#include <libiff/io.h>
#include <libiff/util.h>
#include "ilbm.h"

IFF_Chunk *ILBM_createDPIHeaderChunk(const IFF_ID chunkId, const IFF_Long chunkSize)
{
    ILBM_DPIHeader *dpiHeader = (ILBM_DPIHeader*)IFF_createChunk(chunkId, chunkSize, sizeof(ILBM_DPIHeader));

    if(dpiHeader != NULL)
    {
        dpiHeader->dpiX = 0;
        dpiHeader->dpiY = 0;
    }

    return (IFF_Chunk*)dpiHeader;
}

ILBM_DPIHeader *ILBM_createDPIHeader(void)
{
    return (ILBM_DPIHeader*)ILBM_createDPIHeaderChunk(ILBM_ID_DPI, ILBM_DPI_DEFAULT_SIZE);
}

IFF_Bool ILBM_readDPIHeader(FILE *file, IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed)
{
    ILBM_DPIHeader *dpiHeader = (ILBM_DPIHeader*)chunk;
    IFF_FieldStatus status;

    if((status = IFF_readUWordField(file, &dpiHeader->dpiX, chunk, "dpiX", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_readUWordField(file, &dpiHeader->dpiY, chunk, "dpiY", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    return TRUE;
}

IFF_Bool ILBM_writeDPIHeader(FILE *file, const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed)
{
    const ILBM_DPIHeader *dpiHeader = (const ILBM_DPIHeader*)chunk;
    IFF_FieldStatus status;

    if((status = IFF_writeUWordField(file, dpiHeader->dpiX, chunk, "dpiX", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_writeUWordField(file, dpiHeader->dpiY, chunk, "dpiY", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    return TRUE;
}

IFF_Bool ILBM_checkDPIHeader(const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry)
{
    return TRUE;
}

void ILBM_freeDPIHeader(IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry)
{
}

void ILBM_printDPIHeader(const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry)
{
    const ILBM_DPIHeader *dpiHeader = (const ILBM_DPIHeader*)chunk;

    IFF_printIndent(stdout, indentLevel, "dpiX = %u;\n", dpiHeader->dpiX);
    IFF_printIndent(stdout, indentLevel, "dpiY = %u;\n", dpiHeader->dpiY);
}

IFF_Bool ILBM_compareDPIHeader(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ChunkRegistry *chunkRegistry)
{
    const ILBM_DPIHeader *dpiHeader1 = (const ILBM_DPIHeader*)chunk1;
    const ILBM_DPIHeader *dpiHeader2 = (const ILBM_DPIHeader*)chunk2;

    if(dpiHeader1->dpiX != dpiHeader2->dpiX)
        return FALSE;

    if(dpiHeader1->dpiY != dpiHeader2->dpiY)
        return FALSE;

    return TRUE;
}
