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
#include <libiff/io.h>
#include <libiff/util.h>
#include "ilbm.h"

#define CHUNKID "GRAB"

ILBM_Point2D *ILBM_createGrab(void)
{
    ILBM_Point2D *point2d = (ILBM_Point2D*)IFF_allocateChunk(CHUNKID, sizeof(ILBM_Point2D));
    
    if(point2d != NULL)
	point2d->chunkSize = 2 * sizeof(IFF_Word);
    
    return point2d;
}

IFF_Chunk *ILBM_readGrab(FILE *file, const IFF_Long chunkSize)
{
    ILBM_Point2D *point2d = ILBM_createGrab();
    
    if(point2d != NULL)
    {
	if(!IFF_readWord(file, &point2d->x, CHUNKID, "x"))
	{
	    ILBM_free((IFF_Chunk*)point2d);
	    return NULL;
	}
    
	if(!IFF_readWord(file, &point2d->y, CHUNKID, "y"))
	{
	    ILBM_free((IFF_Chunk*)point2d);
	    return NULL;
	}
    }
    
    return (IFF_Chunk*)point2d;
}

int ILBM_writeGrab(FILE *file, const IFF_Chunk *chunk)
{
    const ILBM_Point2D *point2d = (const ILBM_Point2D*)chunk;
    
    if(!IFF_writeWord(file, point2d->x, CHUNKID, "x"))
	return FALSE;
	
    if(!IFF_writeWord(file, point2d->y, CHUNKID, "y"))
	return FALSE;
	
    return TRUE;
}

int ILBM_checkGrab(const IFF_Chunk *chunk)
{
    return TRUE;
}

void ILBM_freeGrab(IFF_Chunk *chunk)
{
}

void ILBM_printGrab(const IFF_Chunk *chunk, const unsigned int indentLevel)
{
    const ILBM_Point2D *point2d = (const ILBM_Point2D*)chunk;
    
    IFF_printIndent(stdout, indentLevel, "x = %d;\n", point2d->x);
    IFF_printIndent(stdout, indentLevel, "y = %d;\n", point2d->y);
}

int ILBM_compareGrab(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2)
{
    const ILBM_Point2D *point2d1 = (const ILBM_Point2D*)chunk1;
    const ILBM_Point2D *point2d2 = (const ILBM_Point2D*)chunk2;
    
    if(point2d1->x != point2d2->x)
	return FALSE;
    
    if(point2d1->y != point2d2->y)
	return FALSE;
    
    return TRUE;
}
