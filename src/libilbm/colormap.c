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

#include "colormap.h"
#include <stdlib.h>
#include <libiff/io.h>
#include <libiff/util.h>
#include "ilbm.h"

#define CHUNKID "CMAP"

ILBM_ColorMap *ILBM_createColorMap(void)
{
    ILBM_ColorMap *colorMap = (ILBM_ColorMap*)IFF_allocateChunk(CHUNKID, sizeof(ILBM_ColorMap));
    
    if(colorMap != NULL)
    {
	colorMap->chunkSize = 0;
    
	colorMap->colorRegisterLength = 0;
	colorMap->colorRegister = NULL;
    }
    
    return colorMap;
}

ILBM_ColorRegister *ILBM_addColorRegisterInColorMap(ILBM_ColorMap *colorMap)
{
    ILBM_ColorRegister *colorRegister;
    
    colorMap->colorRegister = (ILBM_ColorRegister*)realloc(colorMap->colorRegister, (colorMap->colorRegisterLength + 1) * sizeof(ILBM_ColorRegister));
    colorRegister = &colorMap->colorRegister[colorMap->colorRegisterLength];
    colorMap->colorRegisterLength++;
    
    colorMap->chunkSize += sizeof(ILBM_ColorRegister);
    
    return colorRegister;
}

IFF_Chunk *ILBM_readColorMap(FILE *file, const IFF_Long chunkSize)
{
    ILBM_ColorMap *colorMap = ILBM_createColorMap();
    
    if(colorMap != NULL)
    {
	while(colorMap->chunkSize < chunkSize)
	{
	    ILBM_ColorRegister *colorRegister = ILBM_addColorRegisterInColorMap(colorMap);
	
	    if(!IFF_readUByte(file, &colorRegister->red, CHUNKID, "colorRegister.red"))
	    {
		ILBM_free((IFF_Chunk*)colorMap);
		return NULL;
	    }
	
	    if(!IFF_readUByte(file, &colorRegister->green, CHUNKID, "colorRegister.green"))
	    {
		ILBM_free((IFF_Chunk*)colorMap);
		return NULL;
	    }
	
	    if(!IFF_readUByte(file, &colorRegister->blue, CHUNKID, "colorRegister.blue"))
	    {
		ILBM_free((IFF_Chunk*)colorMap);
		return NULL;
	    }
	}
    
	if(!IFF_readPaddingByte(file, chunkSize, CHUNKID))
	{
	    ILBM_free((IFF_Chunk*)colorMap);
	    return NULL;
	}
    }
    
    return (IFF_Chunk*)colorMap;
}

int ILBM_writeColorMap(FILE *file, const IFF_Chunk *chunk)
{
    const ILBM_ColorMap *colorMap = (const ILBM_ColorMap*)chunk;
    unsigned int i;
    
    for(i = 0; i < colorMap->colorRegisterLength; i++)
    {
	ILBM_ColorRegister *colorRegister = &colorMap->colorRegister[i];
	
	if(!IFF_writeUByte(file, colorRegister->red, CHUNKID, "colorRegister.red"))
	    return FALSE;
	
	if(!IFF_writeUByte(file, colorRegister->green, CHUNKID, "colorRegister.green"))
	    return FALSE;
	
	if(!IFF_writeUByte(file, colorRegister->blue, CHUNKID, "colorRegister.blue"))
	    return FALSE;
    }
    
    if(!IFF_writePaddingByte(file, colorMap->chunkSize, CHUNKID))
	return FALSE;
    
    return TRUE;
}

int ILBM_checkColorMap(const IFF_Chunk *chunk)
{
    return TRUE;
}

void ILBM_freeColorMap(IFF_Chunk *chunk)
{
    ILBM_ColorMap *colorMap = (ILBM_ColorMap*)chunk;
    free(colorMap->colorRegister);
}

void ILBM_printColorMap(const IFF_Chunk *chunk, const unsigned int indentLevel)
{
    const ILBM_ColorMap *colorMap = (const ILBM_ColorMap*)chunk;
    unsigned int i;
    
    for(i = 0; i < colorMap->colorRegisterLength; i++)
    {
	IFF_printIndent(stdout, indentLevel, "{ red = %x, green = %x, blue = %x };\n",
	    colorMap->colorRegister[i].red, colorMap->colorRegister[i].green, colorMap->colorRegister[i].blue);
    }
}

int ILBM_compareColorMap(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2)
{
    const ILBM_ColorMap *colorMap1 = (const ILBM_ColorMap*)chunk1;
    const ILBM_ColorMap *colorMap2 = (const ILBM_ColorMap*)chunk2;
    unsigned int i;
    
    if(colorMap1->colorRegisterLength == colorMap2->colorRegisterLength)
    {
	for(i = 0; i < colorMap1->colorRegisterLength; i++)
	{
	    if(colorMap1->colorRegister[i].red != colorMap2->colorRegister[i].red)
		return FALSE;
	
	    if(colorMap1->colorRegister[i].green != colorMap2->colorRegister[i].green)
		return FALSE;
	
	    if(colorMap1->colorRegister[i].blue != colorMap2->colorRegister[i].blue)
		return FALSE;
	}
    }
    else
	return FALSE;
	
    return TRUE;
}
