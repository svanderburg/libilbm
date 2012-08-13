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
#include <libiff/io.h>
#include <libiff/util.h>
#include "ilbm.h"

#define CHUNKID "CAMG"

ILBM_Viewport *ILBM_createViewport(void)
{
    ILBM_Viewport *viewport = (ILBM_Viewport*)IFF_allocateChunk(CHUNKID, sizeof(ILBM_Viewport));
    
    if(viewport != NULL)
	viewport->chunkSize = sizeof(IFF_Long);
    
    return viewport;
}

IFF_Chunk *ILBM_readViewport(FILE *file, const IFF_Long chunkSize)
{
    ILBM_Viewport *viewport = ILBM_createViewport();
    
    if(viewport != NULL)
    {
	if(!IFF_readLong(file, &viewport->viewportMode, CHUNKID, "viewportMode"))
	{
	    ILBM_free((IFF_Chunk*)viewport);
	    return NULL;
	}
    }
    
    return (IFF_Chunk*)viewport;
}

int ILBM_writeViewport(FILE *file, const IFF_Chunk *chunk)
{
    const ILBM_Viewport *viewport = (const ILBM_Viewport*)chunk;
    
    if(!IFF_writeLong(file, viewport->viewportMode, CHUNKID, "viewportMode"))
	return FALSE;
    
    return TRUE;
}

int ILBM_checkViewport(const IFF_Chunk *chunk)
{
    return TRUE;
}

void ILBM_freeViewport(IFF_Chunk *chunk)
{
}

void ILBM_printViewport(const IFF_Chunk *chunk, const unsigned int indentLevel)
{
    const ILBM_Viewport *viewport = (const ILBM_Viewport*)chunk;
    
    IFF_printIndent(stdout, indentLevel, "viewportMode = %x;\n", viewport->viewportMode);
}

int ILBM_compareViewport(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2)
{
    const ILBM_Viewport *viewport1 = (const ILBM_Viewport*)chunk1;
    const ILBM_Viewport *viewport2 = (const ILBM_Viewport*)chunk2;
    
    if(viewport1->viewportMode != viewport2->viewportMode)
	return FALSE;
    
    return TRUE;
}
