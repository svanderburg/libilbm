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

#include "sprite.h"
#include <stdlib.h>
#include <libiff/io.h>
#include <libiff/util.h>
#include "ilbm.h"

#define CHUNKID "SPRT"

ILBM_Sprite *ILBM_createSprite(void)
{
    ILBM_Sprite *sprite = (ILBM_Sprite*)IFF_allocateChunk(CHUNKID, sizeof(ILBM_Sprite));
    
    if(sprite != NULL)
	sprite->chunkSize = sizeof(IFF_UWord);
    
    return sprite;
}

IFF_Chunk *ILBM_readSprite(FILE *file, const IFF_Long chunkSize)
{
    ILBM_Sprite *sprite = ILBM_createSprite();
    
    if(sprite != NULL)
    {
	if(!IFF_readUWord(file, &sprite->spritePrecedence, CHUNKID, "spritePrecedence"))
	{
	    ILBM_free((IFF_Chunk*)sprite);
	    return NULL;
	}
    }
    
    return (IFF_Chunk*)sprite;
}

int ILBM_writeSprite(FILE *file, const IFF_Chunk *chunk)
{
    const ILBM_Sprite *sprite = (const ILBM_Sprite*)chunk;
    
    if(!IFF_writeUWord(file, sprite->spritePrecedence, CHUNKID, "spritePrecedence"))
	return FALSE;
    
    return TRUE;
}

int ILBM_checkSprite(const IFF_Chunk *chunk)
{
    return TRUE;
}

void ILBM_freeSprite(IFF_Chunk *chunk)
{
}

void ILBM_printSprite(const IFF_Chunk *chunk, const unsigned int indentLevel)
{
    const ILBM_Sprite *sprite = (const ILBM_Sprite*)chunk;
    
    IFF_printIndent(stdout, indentLevel, "spritePrecedence = %u;\n", sprite->spritePrecedence);
}

int ILBM_compareSprite(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2)
{
    const ILBM_Sprite *sprite1 = (const ILBM_Sprite*)chunk1;
    const ILBM_Sprite *sprite2 = (const ILBM_Sprite*)chunk2;
    
    if(sprite1->spritePrecedence != sprite2->spritePrecedence)
	return FALSE;
	
    return TRUE;
}
