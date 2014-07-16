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

#include "drange.h"
#include <stdlib.h>
#include <libiff/io.h>
#include <libiff/util.h>
#include "ilbm.h"

#define CHUNKID "DRNG"

static void increaseChunkSizeWithFades(ILBM_DRange *drange)
{
    drange->chunkSize += 2 * sizeof(IFF_UByte);
}

ILBM_DRange *ILBM_createDRange(IFF_Word flags)
{
    ILBM_DRange *drange = (ILBM_DRange*)IFF_allocateChunk(CHUNKID, sizeof(ILBM_DRange));
    
    if(drange != NULL)
    {
	drange->chunkSize = 2 * sizeof(IFF_UByte) + 2 * sizeof(IFF_Word) + 2 * sizeof(IFF_UByte);

	drange->flags = flags;
	drange->ntrue = 0;
	drange->nregs = 0;
	drange->dcolor = NULL;
	drange->dindex = NULL;
	drange->nfades = 0;
	drange->pad = '\0';
	drange->dfade = NULL;
	
	if((flags & ILBM_RNG_FADE) == ILBM_RNG_FADE)
	    increaseChunkSizeWithFades(drange);
    }
    
    return drange;
}

ILBM_DColor *ILBM_addDColorToDRange(ILBM_DRange *drange)
{
    ILBM_DColor *dcolor;
    
    drange->dcolor = (ILBM_DColor*)realloc(drange->dcolor, (drange->ntrue + 1) * sizeof(ILBM_DColor));
    dcolor = &drange->dcolor[drange->ntrue];
    drange->ntrue++;
    
    drange->chunkSize += sizeof(ILBM_DColor);
    
    return dcolor;
}

ILBM_DIndex *ILBM_addDIndexToDRange(ILBM_DRange *drange)
{
    ILBM_DIndex *dindex;
    
    drange->dindex = (ILBM_DIndex*)realloc(drange->dindex, (drange->nregs + 1) * sizeof(ILBM_DIndex));
    dindex = &drange->dindex[drange->nregs];
    drange->nregs++;
    
    drange->chunkSize += sizeof(ILBM_DIndex);
    
    return dindex;
}

ILBM_DFade *ILBM_addDFadeToDRange(ILBM_DRange *drange)
{
    ILBM_DFade *dfade;
    
    drange->dfade = (ILBM_DFade*)realloc(drange->dfade, (drange->nfades + 1) * sizeof(ILBM_DFade));
    dfade = &drange->dfade[drange->nfades];
    drange->nfades++;
    
    drange->chunkSize += sizeof(ILBM_DFade);
    
    return dfade;
}

IFF_Chunk *ILBM_readDRange(FILE *file, const IFF_Long chunkSize)
{
    ILBM_DRange *drange = ILBM_createDRange(0);
    
    if(drange != NULL)
    {
	IFF_UByte nregs, ntrue;
	unsigned int i;
    
	if(!IFF_readUByte(file, &drange->min, CHUNKID, "min"))
	{
	    ILBM_free((IFF_Chunk*)drange);
	    return NULL;
	}
    
	if(!IFF_readUByte(file, &drange->max, CHUNKID, "max"))
	{
	    ILBM_free((IFF_Chunk*)drange);
	    return NULL;
	}
    
	if(!IFF_readWord(file, &drange->rate, CHUNKID, "rate"))
	{
	    ILBM_free((IFF_Chunk*)drange);
	    return NULL;
	}
    
	if(!IFF_readWord(file, &drange->flags, CHUNKID, "flags"))
	{
	    ILBM_free((IFF_Chunk*)drange);
	    return NULL;
	}
	
	if(!IFF_readUByte(file, &ntrue, CHUNKID, "ntrue"))
	{
	    ILBM_free((IFF_Chunk*)drange);
	    return NULL;
	}
    
	if(!IFF_readUByte(file, &nregs, CHUNKID, "nregs"))
	{
	    ILBM_free((IFF_Chunk*)drange);
	    return NULL;
	}
    
	for(i = 0; i < ntrue; i++)
	{
	    ILBM_DColor *dcolor = ILBM_addDColorToDRange(drange);
	
	    if(!IFF_readUByte(file, &dcolor->cell, CHUNKID, "dcolor.cell"))
	    {
		ILBM_free((IFF_Chunk*)drange);
		return NULL;
	    }
	
	    if(!IFF_readUByte(file, &dcolor->r, CHUNKID, "dcolor.r"))
	    {
		ILBM_free((IFF_Chunk*)drange);
		return NULL;
	    }
	
	    if(!IFF_readUByte(file, &dcolor->g, CHUNKID, "dcolor.g"))
	    {
		ILBM_free((IFF_Chunk*)drange);
		return NULL;
	    }
	
	    if(!IFF_readUByte(file, &dcolor->b, CHUNKID, "dcolor.b"))
	    {
		ILBM_free((IFF_Chunk*)drange);
		return NULL;
	    }
	}
    
	for(i = 0; i < nregs; i++)
	{
	    ILBM_DIndex *dindex = ILBM_addDIndexToDRange(drange);
    
	    if(!IFF_readUByte(file, &dindex->cell, CHUNKID, "dindex.cell"))
	    {
		ILBM_free((IFF_Chunk*)drange);
		return NULL;
	    }

	    if(!IFF_readUByte(file, &dindex->index, CHUNKID, "dindex.index"))
	    {
		ILBM_free((IFF_Chunk*)drange);
		return NULL;
	    }
	}
	
	if((drange->flags & ILBM_RNG_FADE) == ILBM_RNG_FADE)
	{
	    IFF_UByte nfades;
	    
	    increaseChunkSizeWithFades(drange);
	    
	    if(!IFF_readUByte(file, &nfades, CHUNKID, "nfades"))
	    {
	        ILBM_free((IFF_Chunk*)drange);
	        return NULL;
	    }
	    
	    if(!IFF_readUByte(file, &drange->pad, CHUNKID, "pad"))
	    {
	        ILBM_free((IFF_Chunk*)drange);
	        return NULL;
	    }
	    
	    for(i = 0; i < nfades; i++)
	    {
		ILBM_DFade *dfade = ILBM_addDFadeToDRange(drange);
		
		if(!IFF_readUByte(file, &dfade->cell, CHUNKID, "dfade.cell"))
		{
		    ILBM_free((IFF_Chunk*)drange);
		    return NULL;
		}
		
		if(!IFF_readUByte(file, &dfade->fade, CHUNKID, "dfade.fade"))
		{
		    ILBM_free((IFF_Chunk*)drange);
		    return NULL;
		}
	    }
	}
    }
    
    return (IFF_Chunk*)drange;
}

int ILBM_writeDRange(FILE *file, const IFF_Chunk *chunk)
{
    const ILBM_DRange *drange = (const ILBM_DRange*)chunk;
    unsigned int i;
    
    if(!IFF_writeUByte(file, drange->min, CHUNKID, "min"))
	return FALSE;
	
    if(!IFF_writeUByte(file, drange->max, CHUNKID, "max"))
	return FALSE;
	
    if(!IFF_writeWord(file, drange->rate, CHUNKID, "rate"))
	return FALSE;
	
    if(!IFF_writeWord(file, drange->flags, CHUNKID, "flags"))
	return FALSE;
	
    if(!IFF_writeUByte(file, drange->ntrue, CHUNKID, "ntrue"))
	return FALSE;
	
    if(!IFF_writeUByte(file, drange->nregs, CHUNKID, "nregs"))
	return FALSE;
	
    for(i = 0; i < drange->ntrue; i++)
    {
	ILBM_DColor *dcolor = &drange->dcolor[i];
	
	if(!IFF_writeUByte(file, dcolor->cell, CHUNKID, "dcolor.cell"))
	    return FALSE;
	
	if(!IFF_writeUByte(file, dcolor->r, CHUNKID, "dcolor.r"))
	    return FALSE;
	    
	if(!IFF_writeUByte(file, dcolor->g, CHUNKID, "dcolor.g"))
	    return FALSE;
	    
	if(!IFF_writeUByte(file, dcolor->b, CHUNKID, "dcolor.b"))
	    return FALSE;
    }

    for(i = 0; i < drange->nregs; i++)
    {
	ILBM_DIndex *dindex = &drange->dindex[i];
    
	if(!IFF_writeUByte(file, dindex->cell, CHUNKID, "dindex.cell"))
	    return FALSE;
	    
	if(!IFF_writeUByte(file, dindex->index, CHUNKID, "dindex.index"))
	    return FALSE;
    }
    
    if((drange->flags & ILBM_RNG_FADE) == ILBM_RNG_FADE)
    {
	if(!IFF_writeUByte(file, drange->nfades, CHUNKID, "nfades"))
	    return FALSE;
        
	if(!IFF_writeUByte(file, drange->pad, CHUNKID, "pad"))
	    return FALSE;
	
	for(i = 0; i < drange->nfades; i++)
	{
	    ILBM_DFade *dfade = &drange->dfade[i];
	
	    if(!IFF_writeUByte(file, dfade->cell, CHUNKID, "dfade.cell"))
		return FALSE;
	
	    if(!IFF_writeUByte(file, dfade->fade, CHUNKID, "dfade.fade"))
		return FALSE;
	}
    }
    
    return TRUE;
}

int ILBM_checkDRange(const IFF_Chunk *chunk)
{
    return TRUE;
}

void ILBM_freeDRange(IFF_Chunk *chunk)
{
    ILBM_DRange *drange = (ILBM_DRange*)chunk;
    
    free(drange->dcolor);
    free(drange->dindex);
    free(drange->dfade);
}

void ILBM_printDRange(const IFF_Chunk *chunk, const unsigned int indentLevel)
{
    ILBM_DRange *drange = (ILBM_DRange*)chunk;
    unsigned int i;
    
    IFF_printIndent(stdout, indentLevel, "min = %u;\n", drange->min);
    IFF_printIndent(stdout, indentLevel, "max = %u;\n", drange->max);
    IFF_printIndent(stdout, indentLevel, "rate = %d;\n", drange->rate);
    IFF_printIndent(stdout, indentLevel, "flags = %d;\n", drange->flags);
    IFF_printIndent(stdout, indentLevel, "ntrue = %u;\n", drange->ntrue);
    IFF_printIndent(stdout, indentLevel, "nregs = %u;\n", drange->nregs);
    
    for(i = 0; i < drange->ntrue; i++)
	IFF_printIndent(stdout, indentLevel, "{ cell = %u, r = %u, g = %u, b = %u }\n", drange->dcolor[i].cell, drange->dcolor[i].r, drange->dcolor[i].g, drange->dcolor[i].b);
    
    for(i = 0; i < drange->nregs; i++)
	IFF_printIndent(stdout, indentLevel, "{ cell = %u, index = %u }\n", drange->dindex[i].cell, drange->dindex[i].index);

    if((drange->flags & ILBM_RNG_FADE) == ILBM_RNG_FADE)
    {
	IFF_printIndent(stdout, indentLevel, "nfades = %u;\n", drange->nfades);
	IFF_printIndent(stdout, indentLevel, "pad = %u;\n", drange->pad);
	
	for(i = 0; i < drange->nfades; i++)
	    IFF_printIndent(stdout, indentLevel, "{ cell = %u, fade = %u }\n", drange->dfade[i].cell, drange->dfade[i].fade);
    }
}

int ILBM_compareDRange(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2)
{
    const ILBM_DRange *drange1 = (const ILBM_DRange*)chunk1;
    const ILBM_DRange *drange2 = (const ILBM_DRange*)chunk2;
    unsigned int i;
    
    if(drange1->min != drange2->min)
	return FALSE;

    if(drange1->max != drange2->max)
	return FALSE;

    if(drange1->rate != drange2->rate)
	return FALSE;

    if(drange1->flags != drange2->flags)
	return FALSE;

    if(drange1->ntrue != drange2->ntrue)
	return FALSE;

    if(drange1->nregs != drange2->nregs)
	return FALSE;

    for(i = 0; i < drange1->ntrue; i++)
    {
	ILBM_DColor *dcolor1 = &drange1->dcolor[i];
	ILBM_DColor *dcolor2 = &drange2->dcolor[i];
	
	if(dcolor1->cell != dcolor2->cell)
	    return FALSE;
	
	if(dcolor1->r != dcolor2->r)
	    return FALSE;
	
	if(dcolor1->g != dcolor2->g)
	    return FALSE;
	
	if(dcolor1->b != dcolor2->b)
	    return FALSE;
    }
    
    for(i = 0; i < drange1->nregs; i++)
    {
	ILBM_DIndex *dindex1 = &drange1->dindex[i];
	ILBM_DIndex *dindex2 = &drange2->dindex[i];
	
	if(dindex1->cell != dindex2->cell)
	    return FALSE;
	
	if(dindex1->index != dindex2->index)
	    return FALSE;
    }

    if((drange1->flags & ILBM_RNG_FADE) == ILBM_RNG_FADE)
    {
	if(drange1->nfades != drange2->nfades)
	    return FALSE;
	
	for(i = 0; i < drange1->nfades; i++)
	{
	    ILBM_DFade *dfade1 = &drange1->dfade[i];
	    ILBM_DFade *dfade2 = &drange2->dfade[i];
	    
	    if(dfade1->cell != dfade2->cell)
		return FALSE;
	
	    if(dfade1->fade != dfade2->fade)
		return FALSE;
	}
    }

    return TRUE;
}
