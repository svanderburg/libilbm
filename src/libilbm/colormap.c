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
#include <string.h>
#include <libiff/field.h>
#include <libiff/io.h>
#include <libiff/util.h>
#include "ilbm.h"

IFF_Chunk *ILBM_createColorMap(const IFF_Long chunkSize)
{
    ILBM_ColorMap *colorMap = (ILBM_ColorMap*)IFF_allocateChunk(ILBM_ID_CMAP, chunkSize, sizeof(ILBM_ColorMap));

    if(colorMap != NULL)
    {
        colorMap->colorRegisterLength = 0;
        colorMap->colorRegister = NULL;
    }

    return (IFF_Chunk*)colorMap;
}

static ILBM_ColorRegister *allocateColorRegisterInColorMap(ILBM_ColorMap *colorMap)
{
    ILBM_ColorRegister *colorRegister;

    colorMap->colorRegister = (ILBM_ColorRegister*)realloc(colorMap->colorRegister, (colorMap->colorRegisterLength + 1) * sizeof(ILBM_ColorRegister));
    colorRegister = &colorMap->colorRegister[colorMap->colorRegisterLength];
    memset(colorRegister, '\0', sizeof(ILBM_ColorRegister));
    colorMap->colorRegisterLength++;

    return colorRegister;
}

ILBM_ColorRegister *ILBM_addColorRegisterInColorMap(ILBM_ColorMap *colorMap)
{
    ILBM_ColorRegister *colorRegister = allocateColorRegisterInColorMap(colorMap);
    colorMap->chunkSize += sizeof(ILBM_ColorRegister);
    return colorRegister;
}

IFF_Bool ILBM_readColorMap(FILE *file, IFF_Chunk *chunk, IFF_Long *bytesProcessed)
{
    ILBM_ColorMap *colorMap = (ILBM_ColorMap*)chunk;
    IFF_FieldStatus status;

    while(*bytesProcessed < colorMap->chunkSize)
    {
        ILBM_ColorRegister *colorRegister = allocateColorRegisterInColorMap(colorMap);

        if((status = IFF_readUByteField(file, &colorRegister->red, chunk, "colorRegister.red", bytesProcessed)) != IFF_FIELD_MORE)
            return IFF_deriveSuccess(status);

        if((status = IFF_readUByteField(file, &colorRegister->green, chunk, "colorRegister.green", bytesProcessed)) != IFF_FIELD_MORE)
            return IFF_deriveSuccess(status);

        if((status = IFF_readUByteField(file, &colorRegister->blue, chunk, "colorRegister.blue", bytesProcessed)) != IFF_FIELD_MORE)
            return IFF_deriveSuccess(status);
    }

    return TRUE;
}

IFF_Bool ILBM_writeColorMap(FILE *file, const IFF_Chunk *chunk, IFF_Long *bytesProcessed)
{
    const ILBM_ColorMap *colorMap = (const ILBM_ColorMap*)chunk;
    IFF_FieldStatus status;
    unsigned int i;

    for(i = 0; i < colorMap->colorRegisterLength; i++)
    {
        ILBM_ColorRegister *colorRegister = &colorMap->colorRegister[i];

        if((status = IFF_writeUByteField(file, colorRegister->red, chunk, "colorRegister.red", bytesProcessed)) != IFF_FIELD_MORE)
            return IFF_deriveSuccess(status);

        if((status = IFF_writeUByteField(file, colorRegister->green, chunk, "colorRegister.green", bytesProcessed)) != IFF_FIELD_MORE)
            return IFF_deriveSuccess(status);

        if((status = IFF_writeUByteField(file, colorRegister->blue, chunk, "colorRegister.blue", bytesProcessed)) != IFF_FIELD_MORE)
            return IFF_deriveSuccess(status);
    }

    return TRUE;
}

IFF_Bool ILBM_checkColorMap(const IFF_Chunk *chunk)
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

IFF_Bool ILBM_compareColorMap(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2)
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
