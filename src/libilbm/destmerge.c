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

#include "destmerge.h"
#include <stdlib.h>
#include <libiff/field.h>
#include <libiff/io.h>
#include <libiff/util.h>
#include <libiff/error.h>
#include "ilbm.h"

IFF_Chunk *ILBM_createDestMerge(const IFF_Long chunkSize)
{
    ILBM_DestMerge *destMerge = (ILBM_DestMerge*)IFF_createChunk(ILBM_ID_DEST, chunkSize, sizeof(ILBM_DestMerge));

    if(destMerge != NULL)
    {
        destMerge->depth = '\0';
        destMerge->pad1 = '\0';
        destMerge->planePick = 0;
        destMerge->planeOnOff = 0;
        destMerge->planeMask = 0;
    }

    return (IFF_Chunk*)destMerge;
}

IFF_Bool ILBM_readDestMerge(FILE *file, IFF_Chunk *chunk, IFF_Long *bytesProcessed)
{
    ILBM_DestMerge *destMerge = (ILBM_DestMerge*)chunk;
    IFF_FieldStatus status;

    if((status = IFF_readUByteField(file, &destMerge->depth, chunk, "depth", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_readUByteField(file, &destMerge->pad1, chunk, "pad1", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_readUWordField(file, &destMerge->planePick, chunk, "planePick", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_readUWordField(file, &destMerge->planeOnOff, chunk, "planeOnOff", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_readUWordField(file, &destMerge->planeMask, chunk, "planeMask", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    return TRUE;
}

IFF_Bool ILBM_writeDestMerge(FILE *file, const IFF_Chunk *chunk, IFF_Long *bytesProcessed)
{
    const ILBM_DestMerge *destMerge = (const ILBM_DestMerge*)chunk;
    IFF_FieldStatus status;

    if((status = IFF_writeUByteField(file, destMerge->depth, chunk, "depth", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_writeUByteField(file, destMerge->pad1, chunk, "pad1", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_writeUWordField(file, destMerge->planePick, chunk, "planePick", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_writeUWordField(file, destMerge->planeOnOff, chunk, "planeOnOff", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_writeUWordField(file, destMerge->planeMask, chunk, "planeMask", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    return TRUE;
}

IFF_Bool ILBM_checkDestMerge(const IFF_Chunk *chunk)
{
    const ILBM_DestMerge *destMerge = (const ILBM_DestMerge*)chunk;

    if(destMerge->pad1 != 0)
        IFF_error("WARNING: 'DEST'.pad1 is not 0!\n");

    return TRUE;
}

void ILBM_freeDestMerge(IFF_Chunk *chunk)
{
}

void ILBM_printDestMerge(const IFF_Chunk *chunk, const unsigned int indentLevel)
{
    const ILBM_DestMerge *destMerge = (const ILBM_DestMerge*)chunk;

    IFF_printIndent(stdout, indentLevel, "depth = %u;\n", destMerge->depth);
    IFF_printIndent(stdout, indentLevel, "pad1 = %u;\n", destMerge->pad1);
    IFF_printIndent(stdout, indentLevel, "planePick = %u;\n", destMerge->planePick);
    IFF_printIndent(stdout, indentLevel, "planeOnOff = %u;\n", destMerge->planeOnOff);
    IFF_printIndent(stdout, indentLevel, "planeMask = %u;\n", destMerge->planeMask);
}

IFF_Bool ILBM_compareDestMerge(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2)
{
    const ILBM_DestMerge *destMerge1 = (const ILBM_DestMerge*)chunk1;
    const ILBM_DestMerge *destMerge2 = (const ILBM_DestMerge*)chunk2;

    if(destMerge1->depth != destMerge2->depth)
        return FALSE;

    if(destMerge1->pad1 != destMerge2->pad1)
        return FALSE;

    if(destMerge1->planePick != destMerge2->planePick)
        return FALSE;

    if(destMerge1->planeOnOff != destMerge2->planeOnOff)
        return FALSE;

    if(destMerge1->planeMask != destMerge2->planeMask)
        return FALSE;

    return TRUE;
}
