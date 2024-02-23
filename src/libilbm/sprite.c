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
#include <libiff/field.h>
#include <libiff/io.h>
#include <libiff/util.h>
#include "ilbm.h"

IFF_Chunk *ILBM_createSprite(const IFF_Long chunkSize)
{
    ILBM_Sprite *sprite = (ILBM_Sprite*)IFF_createChunk(ILBM_ID_SPRT, chunkSize, sizeof(ILBM_Sprite));

    if(sprite != NULL)
        sprite->spritePrecedence = 0;

    return (IFF_Chunk*)sprite;
}

IFF_Bool ILBM_readSprite(FILE *file, IFF_Chunk *chunk, IFF_Long *bytesProcessed)
{
    ILBM_Sprite *sprite = (ILBM_Sprite*)chunk;
    IFF_FieldStatus status;

    if((status = IFF_readUWordField(file, &sprite->spritePrecedence, chunk, "spritePrecedence", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    return TRUE;
}

IFF_Bool ILBM_writeSprite(FILE *file, const IFF_Chunk *chunk, IFF_Long *bytesProcessed)
{
    const ILBM_Sprite *sprite = (const ILBM_Sprite*)chunk;
    IFF_FieldStatus status;

    if((status = IFF_writeUWordField(file, sprite->spritePrecedence, chunk, "spritePrecedence", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    return TRUE;
}

IFF_Bool ILBM_checkSprite(const IFF_Chunk *chunk)
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

IFF_Bool ILBM_compareSprite(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2)
{
    const ILBM_Sprite *sprite1 = (const ILBM_Sprite*)chunk1;
    const ILBM_Sprite *sprite2 = (const ILBM_Sprite*)chunk2;

    if(sprite1->spritePrecedence != sprite2->spritePrecedence)
        return FALSE;

    return TRUE;
}
