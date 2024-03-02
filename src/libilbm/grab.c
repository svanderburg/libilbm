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

#include "grab.h"
#include <stdlib.h>
#include <libiff/field.h>
#include <libiff/io.h>
#include <libiff/util.h>
#include "ilbm.h"

IFF_Chunk *ILBM_createGrabChunk(const IFF_ID chunkId, const IFF_Long chunkSize)
{
    ILBM_Point2D *point2d = (ILBM_Point2D*)IFF_createChunk(chunkId, chunkSize, sizeof(ILBM_Point2D));

    if(point2d != NULL)
    {
        point2d->x = 0;
        point2d->y = 0;
    }

    return (IFF_Chunk*)point2d;
}

ILBM_Point2D *ILBM_createGrab(void)
{
    return (ILBM_Point2D*)ILBM_createGrabChunk(ILBM_ID_GRAB, ILBM_GRAB_DEFAULT_SIZE);
}

IFF_Bool ILBM_readGrab(FILE *file, IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed)
{
    ILBM_Point2D *point2d = (ILBM_Point2D*)chunk;
    IFF_FieldStatus status;

    if((status = IFF_readWordField(file, &point2d->x, chunk, "x", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_readWordField(file, &point2d->y, chunk, "y", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    return TRUE;
}

IFF_Bool ILBM_writeGrab(FILE *file, const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed)
{
    const ILBM_Point2D *point2d = (const ILBM_Point2D*)chunk;
    IFF_FieldStatus status;

    if((status = IFF_writeWordField(file, point2d->x, chunk, "x", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_writeWordField(file, point2d->y, chunk, "y", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    return TRUE;
}

IFF_Bool ILBM_checkGrab(const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry)
{
    return TRUE;
}

void ILBM_freeGrab(IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry)
{
}

void ILBM_printGrab(const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry)
{
    const ILBM_Point2D *point2d = (const ILBM_Point2D*)chunk;

    IFF_printIndent(stdout, indentLevel, "x = %d;\n", point2d->x);
    IFF_printIndent(stdout, indentLevel, "y = %d;\n", point2d->y);
}

IFF_Bool ILBM_compareGrab(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ChunkRegistry *chunkRegistry)
{
    const ILBM_Point2D *point2d1 = (const ILBM_Point2D*)chunk1;
    const ILBM_Point2D *point2d2 = (const ILBM_Point2D*)chunk2;

    if(point2d1->x != point2d2->x)
        return FALSE;

    if(point2d1->y != point2d2->y)
        return FALSE;

    return TRUE;
}
