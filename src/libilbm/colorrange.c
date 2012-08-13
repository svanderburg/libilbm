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

#include "colorrange.h"
#include <stdlib.h>
#include <libiff/io.h>
#include <libiff/util.h>
#include <libiff/error.h>
#include "ilbm.h"

#define CHUNKID "CRNG"

ILBM_ColorRange *ILBM_createColorRange(void)
{
    ILBM_ColorRange *colorRange = (ILBM_ColorRange*)IFF_allocateChunk(CHUNKID, sizeof(ILBM_ColorRange));
    
    if(colorRange != NULL)
    {
	colorRange->chunkSize = 3 * sizeof(IFF_Word) + 2 * sizeof(IFF_UByte);
	colorRange->pad1 = 0;
    }
    
    return colorRange;
}

IFF_Chunk *ILBM_readColorRange(FILE *file, const IFF_Long chunkSize)
{
    ILBM_ColorRange *colorRange = ILBM_createColorRange();
    
    if(colorRange != NULL)
    {
	if(!IFF_readWord(file, &colorRange->pad1, CHUNKID, "pad1"))
	{
	    ILBM_free((IFF_Chunk*)colorRange);
	    return NULL;
	}
    
	if(!IFF_readWord(file, &colorRange->rate, CHUNKID, "rate"))
	{
	    ILBM_free((IFF_Chunk*)colorRange);
	    return NULL;
	}
    
	if(!IFF_readWord(file, &colorRange->active, CHUNKID, "active"))
	{
	    ILBM_free((IFF_Chunk*)colorRange);
	    return NULL;
	}
    
	if(!IFF_readUByte(file, &colorRange->low, CHUNKID, "low"))
	{
	    ILBM_free((IFF_Chunk*)colorRange);
	    return NULL;
	}
    
	if(!IFF_readUByte(file, &colorRange->high, CHUNKID, "high"))
	{
	    ILBM_free((IFF_Chunk*)colorRange);
	    return NULL;
	}
    }
    
    return (IFF_Chunk*)colorRange;
}

int ILBM_writeColorRange(FILE *file, const IFF_Chunk *chunk)
{
    const ILBM_ColorRange *colorRange = (const ILBM_ColorRange*)chunk;
    
    if(!IFF_writeWord(file, colorRange->pad1, CHUNKID, "pad1"))
	return FALSE;
    
    if(!IFF_writeWord(file, colorRange->rate, CHUNKID, "rate"))
	return FALSE;
    
    if(!IFF_writeWord(file, colorRange->active, CHUNKID, "active"))
	return FALSE;
    
    if(!IFF_writeUByte(file, colorRange->low, CHUNKID, "low"))
	return FALSE;
    
    if(!IFF_writeUByte(file, colorRange->high, CHUNKID, "high"))
	return FALSE;
    
    return TRUE;
}

int ILBM_checkColorRange(const IFF_Chunk *chunk)
{
    const ILBM_ColorRange *colorRange = (const ILBM_ColorRange*)chunk;

    if(colorRange->pad1 != 0)
	IFF_error("WARING: 'CRNG'.pad1 is not 0!\n");
	
    return TRUE;
}

void ILBM_freeColorRange(IFF_Chunk *chunk)
{
}

void ILBM_printColorRange(const IFF_Chunk *chunk, const unsigned int indentLevel)
{
    ILBM_ColorRange *colorRange = (ILBM_ColorRange*)chunk;
    
    IFF_printIndent(stdout, indentLevel, "pad1 = %d;\n", colorRange->pad1);
    IFF_printIndent(stdout, indentLevel, "rate = %d;\n", colorRange->rate);
    IFF_printIndent(stdout, indentLevel, "active = %d;\n", colorRange->active);
    IFF_printIndent(stdout, indentLevel, "low = %u;\n", colorRange->low);
    IFF_printIndent(stdout, indentLevel, "high = %u;\n", colorRange->high);
}

int ILBM_compareColorRange(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2)
{
    const ILBM_ColorRange *colorRange1 = (const ILBM_ColorRange*)chunk1;
    const ILBM_ColorRange *colorRange2 = (const ILBM_ColorRange*)chunk2;
    
    if(colorRange1->pad1 != colorRange2->pad1)
	return FALSE;
    
    if(colorRange1->rate != colorRange2->rate)
	return FALSE;
    
    if(colorRange1->active != colorRange2->active)
	return FALSE;

    if(colorRange1->low != colorRange2->low)
	return FALSE;

    if(colorRange1->high != colorRange2->high)
	return FALSE;
    
    return TRUE;
}
