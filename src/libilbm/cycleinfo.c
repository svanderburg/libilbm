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
#include <libiff/io.h>
#include <libiff/util.h>
#include <libiff/error.h>
#include "ilbm.h"

#define CHUNKID "CCRT"

ILBM_CycleInfo *ILBM_createCycleInfo(void)
{
    ILBM_CycleInfo *cycleInfo = (ILBM_CycleInfo*)IFF_allocateChunk(CHUNKID, sizeof(ILBM_CycleInfo));
    
    if(cycleInfo != NULL)
    {
	cycleInfo->chunkSize = sizeof(IFF_Word) + 2 * sizeof(IFF_UByte) + 2 * sizeof(IFF_Long) + sizeof(IFF_Word);
	cycleInfo->pad = 0;
    }
    
    return cycleInfo;
}

IFF_Chunk *ILBM_readCycleInfo(FILE *file, const IFF_Long chunkSize)
{
    ILBM_CycleInfo *cycleInfo = ILBM_createCycleInfo();
    
    if(cycleInfo != NULL)
    {
	if(!IFF_readWord(file, &cycleInfo->direction, CHUNKID, "direction"))
	{
	    ILBM_free((IFF_Chunk*)cycleInfo);
	    return NULL;
	}
    
	if(!IFF_readUByte(file, &cycleInfo->start, CHUNKID, "start"))
	{
	    ILBM_free((IFF_Chunk*)cycleInfo);
	    return NULL;
	}
    
	if(!IFF_readUByte(file, &cycleInfo->end, CHUNKID, "end"))
	{
	    ILBM_free((IFF_Chunk*)cycleInfo);
	    return NULL;
	}
    
	if(!IFF_readLong(file, &cycleInfo->seconds, CHUNKID, "seconds"))
	{
	    ILBM_free((IFF_Chunk*)cycleInfo);
	    return NULL;
	}
    
	if(!IFF_readLong(file, &cycleInfo->microSeconds, CHUNKID, "microSeconds"))
	{
	    ILBM_free((IFF_Chunk*)cycleInfo);
	    return NULL;
	}
    
	if(!IFF_readWord(file, &cycleInfo->pad, CHUNKID, "pad"))
	{
	    ILBM_free((IFF_Chunk*)cycleInfo);
	    return NULL;
	}
    }
    
    return (IFF_Chunk*)cycleInfo;
}

int ILBM_writeCycleInfo(FILE *file, const IFF_Chunk *chunk)
{
    const ILBM_CycleInfo *cycleInfo = (const ILBM_CycleInfo*)chunk;
    
    if(!IFF_writeWord(file, cycleInfo->direction, CHUNKID, "direction"))
	return FALSE;
    
    if(!IFF_writeUByte(file, cycleInfo->start, CHUNKID, "start"))
	return FALSE;
    
    if(!IFF_writeUByte(file, cycleInfo->end, CHUNKID, "end"))
	return FALSE;
	
    if(!IFF_writeLong(file, cycleInfo->seconds, CHUNKID, "seconds"))
	return FALSE;
	
    if(!IFF_writeLong(file, cycleInfo->microSeconds, CHUNKID, "microSeconds"))
	return FALSE;
	
    if(!IFF_writeWord(file, cycleInfo->pad, CHUNKID, "pad"))
	return FALSE;
    
    return TRUE;
}

int ILBM_checkCycleInfo(const IFF_Chunk *chunk)
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

void ILBM_freeCycleInfo(IFF_Chunk *chunk)
{
}

void ILBM_printCycleInfo(const IFF_Chunk *chunk, const unsigned int indentLevel)
{
    const ILBM_CycleInfo *cycleInfo = (const ILBM_CycleInfo*)chunk;
    
    IFF_printIndent(stdout, indentLevel, "direction = %d;\n", cycleInfo->direction);
    IFF_printIndent(stdout, indentLevel, "start = %u;\n", cycleInfo->start);
    IFF_printIndent(stdout, indentLevel, "end = %u;\n", cycleInfo->end);
    IFF_printIndent(stdout, indentLevel, "seconds = %d;\n", cycleInfo->seconds);
    IFF_printIndent(stdout, indentLevel, "microSeconds = %d;\n", cycleInfo->microSeconds);
    IFF_printIndent(stdout, indentLevel, "pad = %d;\n", cycleInfo->pad);
}

int ILBM_compareCycleInfo(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2)
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
