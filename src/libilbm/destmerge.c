/**
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
#include <io.h>
#include <id.h>
#include <util.h>
#include <error.h>
#include "ilbm.h"

#define CHUNKID "DEST"

ILBM_DestMerge *ILBM_createDestMerge(void)
{
    ILBM_DestMerge *destMerge = (ILBM_DestMerge*)IFF_allocateChunk(CHUNKID, sizeof(ILBM_DestMerge));
    
    if(destMerge != NULL)
    {
	destMerge->chunkSize = 2 * sizeof(IFF_UByte) + 3 * sizeof(IFF_UWord);
	destMerge->pad1 = '\0';
    }
    
    return destMerge;
}

IFF_Chunk *ILBM_readDestMerge(FILE *file, const IFF_Long chunkSize)
{
    ILBM_DestMerge *destMerge = ILBM_createDestMerge();
    
    if(destMerge != NULL)
    {
	if(!IFF_readUByte(file, &destMerge->depth, CHUNKID, "depth"))
	{
	    ILBM_free((IFF_Chunk*)destMerge);
	    return NULL;
	}
    
	if(!IFF_readUByte(file, &destMerge->pad1, CHUNKID, "pad1"))
	{
	    ILBM_free((IFF_Chunk*)destMerge);
	    return NULL;
	}

	if(!IFF_readUWord(file, &destMerge->planePick, CHUNKID, "planePick"))
	{
	    ILBM_free((IFF_Chunk*)destMerge);
	    return NULL;
	}
    
	if(!IFF_readUWord(file, &destMerge->planeOnOff, CHUNKID, "planeOnOff"))
	{
	    ILBM_free((IFF_Chunk*)destMerge);
	    return NULL;
	}
    
	if(!IFF_readUWord(file, &destMerge->planeMask, CHUNKID, "planeMask"))
	{
	    ILBM_free((IFF_Chunk*)destMerge);
	    return NULL;
	}
    }
    
    return (IFF_Chunk*)destMerge;
}

int ILBM_writeDestMerge(FILE *file, const IFF_Chunk *chunk)
{
    ILBM_DestMerge *destMerge = (ILBM_DestMerge*)chunk;
    
    if(!IFF_writeUByte(file, destMerge->depth, CHUNKID, "depth"))
	return FALSE;
    
    if(!IFF_writeUByte(file, destMerge->pad1, CHUNKID, "pad1"))
	return FALSE;
    
    if(!IFF_writeUWord(file, destMerge->planePick, CHUNKID, "planePick"))
	return FALSE;
    
    if(!IFF_writeUWord(file, destMerge->planeOnOff, CHUNKID, "planeOnOff"))
	return FALSE;
    
    if(!IFF_writeUWord(file, destMerge->planeMask, CHUNKID, "planeMask"))
	return FALSE;
    
    return TRUE;
}

int ILBM_checkDestMerge(const IFF_Chunk *chunk)
{
    ILBM_DestMerge *destMerge = (ILBM_DestMerge*)chunk;
    
    if(destMerge->pad1 != 0)
	IFF_error("WARNING: 'DEST'.pad1 is not 0!\n");
    
    return TRUE;
}

void ILBM_freeDestMerge(IFF_Chunk *chunk)
{
}

void ILBM_printDestMerge(const IFF_Chunk *chunk, const unsigned int indentLevel)
{
    ILBM_DestMerge *destMerge = (ILBM_DestMerge*)chunk;
    
    IFF_printIndent(stdout, indentLevel, "depth = %d;\n", destMerge->depth);
    IFF_printIndent(stdout, indentLevel, "pad1 = %d;\n", destMerge->pad1);
    IFF_printIndent(stdout, indentLevel, "planePick = %d;\n", destMerge->planePick);
    IFF_printIndent(stdout, indentLevel, "planeOnOff = %d;\n", destMerge->planeOnOff);
    IFF_printIndent(stdout, indentLevel, "planeMask = %d;\n", destMerge->planeMask);
}
