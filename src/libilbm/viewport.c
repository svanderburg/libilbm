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

#include "viewport.h"
#include <stdlib.h>
#include <libiff/field.h>
#include <libiff/io.h>
#include <libiff/util.h>
#include "ilbm.h"

IFF_Chunk *ILBM_createViewportChunk(const IFF_ID chunkId, const IFF_Long chunkSize)
{
    ILBM_Viewport *viewport = (ILBM_Viewport*)IFF_createChunk(chunkId, chunkSize, sizeof(ILBM_Viewport));

    if(viewport != NULL)
        viewport->viewportMode = 0;

    return (IFF_Chunk*)viewport;
}

ILBM_Viewport *ILBM_createViewport(void)
{
    return (ILBM_Viewport*)ILBM_createViewportChunk(ILBM_ID_CAMG, ILBM_CAMG_DEFAULT_SIZE);
}

IFF_Bool ILBM_readViewport(FILE *file, IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed)
{
    ILBM_Viewport *viewport = (ILBM_Viewport*)chunk;
    IFF_FieldStatus status;

    if((status = IFF_readLongField(file, &viewport->viewportMode, chunk, "viewportMode", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    return TRUE;
}

IFF_Bool ILBM_writeViewport(FILE *file, const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed)
{
    const ILBM_Viewport *viewport = (const ILBM_Viewport*)chunk;
    IFF_FieldStatus status;

    if((status = IFF_writeLongField(file, viewport->viewportMode, chunk, "viewportMode", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    return TRUE;
}

IFF_Bool ILBM_checkViewport(const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry)
{
    return TRUE;
}

void ILBM_freeViewport(IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry)
{
}

void ILBM_printViewport(const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry)
{
    const ILBM_Viewport *viewport = (const ILBM_Viewport*)chunk;

    IFF_printIndent(stdout, indentLevel, "viewportMode = %x;\n", viewport->viewportMode);
}

IFF_Bool ILBM_compareViewport(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ChunkRegistry *chunkRegistry)
{
    const ILBM_Viewport *viewport1 = (const ILBM_Viewport*)chunk1;
    const ILBM_Viewport *viewport2 = (const ILBM_Viewport*)chunk2;

    if(viewport1->viewportMode != viewport2->viewportMode)
        return FALSE;

    return TRUE;
}
