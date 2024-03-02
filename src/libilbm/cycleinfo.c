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

#include "cycleinfo.h"
#include <stdlib.h>
#include <libiff/field.h>
#include <libiff/io.h>
#include <libiff/util.h>
#include <libiff/error.h>
#include "ilbm.h"

IFF_Chunk *ILBM_createCycleInfoChunk(const IFF_ID chunkId, const IFF_Long chunkSize)
{
    ILBM_CycleInfo *cycleInfo = (ILBM_CycleInfo*)IFF_createChunk(chunkId, chunkSize, sizeof(ILBM_CycleInfo));

    if(cycleInfo != NULL)
    {
        cycleInfo->direction = 0;
        cycleInfo->start = '\0';
        cycleInfo->end = '\0';
        cycleInfo->seconds = 0;
        cycleInfo->microSeconds = 0;
        cycleInfo->pad = 0;
    }

    return (IFF_Chunk*)cycleInfo;
}

ILBM_CycleInfo *ILBM_createCycleInfo(void)
{
    return (ILBM_CycleInfo*)ILBM_createCycleInfoChunk(ILBM_ID_CCRT, ILBM_CCRT_DEFAULT_SIZE);
}

IFF_Bool ILBM_readCycleInfo(FILE *file, IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed)
{
    ILBM_CycleInfo *cycleInfo = (ILBM_CycleInfo*)chunk;
    IFF_FieldStatus status;

    if((status = IFF_readWordField(file, &cycleInfo->direction, chunk, "direction", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_readUByteField(file, &cycleInfo->start, chunk, "start", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_readUByteField(file, &cycleInfo->end, chunk, "end", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_readLongField(file, &cycleInfo->seconds, chunk, "seconds", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_readLongField(file, &cycleInfo->microSeconds, chunk, "microSeconds", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_readWordField(file, &cycleInfo->pad, chunk, "pad", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    return TRUE;
}

IFF_Bool ILBM_writeCycleInfo(FILE *file, const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed)
{
    const ILBM_CycleInfo *cycleInfo = (const ILBM_CycleInfo*)chunk;
    IFF_FieldStatus status;

    if((status = IFF_writeWordField(file, cycleInfo->direction, chunk, "direction", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_writeUByteField(file, cycleInfo->start, chunk, "start", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_writeUByteField(file, cycleInfo->end, chunk, "end", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_writeLongField(file, cycleInfo->seconds, chunk, "seconds", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_writeLongField(file, cycleInfo->microSeconds, chunk, "microSeconds", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_writeWordField(file, cycleInfo->pad, chunk, "pad", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    return TRUE;
}

IFF_Bool ILBM_checkCycleInfo(const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry)
{
    const ILBM_CycleInfo *cycleInfo = (const ILBM_CycleInfo*)chunk;

    if(cycleInfo->direction < -1 || cycleInfo->direction > 1)
    {
        IFF_error("'CCRT'.direction must be between -1 and 1\n");
        return FALSE;
    }

    if(cycleInfo->pad != 0)
        IFF_error("'CCRT'.pad is not 0!\n");

    return TRUE;
}

void ILBM_freeCycleInfo(IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry)
{
}

void ILBM_printCycleInfo(const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry)
{
    const ILBM_CycleInfo *cycleInfo = (const ILBM_CycleInfo*)chunk;

    IFF_printIndent(stdout, indentLevel, "direction = %d;\n", cycleInfo->direction);
    IFF_printIndent(stdout, indentLevel, "start = %u;\n", cycleInfo->start);
    IFF_printIndent(stdout, indentLevel, "end = %u;\n", cycleInfo->end);
    IFF_printIndent(stdout, indentLevel, "seconds = %d;\n", cycleInfo->seconds);
    IFF_printIndent(stdout, indentLevel, "microSeconds = %d;\n", cycleInfo->microSeconds);
    IFF_printIndent(stdout, indentLevel, "pad = %d;\n", cycleInfo->pad);
}

IFF_Bool ILBM_compareCycleInfo(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ChunkRegistry *chunkRegistry)
{
    const ILBM_CycleInfo *cycleInfo1 = (const ILBM_CycleInfo*)chunk1;
    const ILBM_CycleInfo *cycleInfo2 = (const ILBM_CycleInfo*)chunk2;

    if(cycleInfo1->direction != cycleInfo2->direction)
        return FALSE;

    if(cycleInfo1->start != cycleInfo2->start)
        return FALSE;

    if(cycleInfo1->end != cycleInfo2->end)
        return FALSE;

    if(cycleInfo1->seconds != cycleInfo2->seconds)
        return FALSE;

    if(cycleInfo1->microSeconds != cycleInfo2->microSeconds)
        return FALSE;

    if(cycleInfo1->pad != cycleInfo2->pad)
        return FALSE;

    return TRUE;
}
