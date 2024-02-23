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
#include <libiff/field.h>
#include <libiff/io.h>
#include <libiff/util.h>
#include <libiff/error.h>
#include "ilbm.h"

IFF_Chunk *ILBM_createColorRange(const IFF_Long chunkSize)
{
    ILBM_ColorRange *colorRange = (ILBM_ColorRange*)IFF_createChunk(ILBM_ID_CRNG, chunkSize, sizeof(ILBM_ColorRange));

    if(colorRange != NULL)
    {
        colorRange->pad1 = 0;
        colorRange->rate = 0;
        colorRange->active = 0;
        colorRange->low = '\0';
        colorRange->high = '\0';
    }

    return (IFF_Chunk*)colorRange;
}

IFF_Bool ILBM_readColorRange(FILE *file, IFF_Chunk *chunk, IFF_Long *bytesProcessed)
{
    ILBM_ColorRange *colorRange = (ILBM_ColorRange*)chunk;
    IFF_FieldStatus status;

    if((status = IFF_readWordField(file, &colorRange->pad1, chunk, "pad1", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_readWordField(file, &colorRange->rate, chunk, "rate", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_readWordField(file, &colorRange->active, chunk, "active", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_readUByteField(file, &colorRange->low, chunk, "low", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_readUByteField(file, &colorRange->high, chunk, "high", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    return TRUE;
}

IFF_Bool ILBM_writeColorRange(FILE *file, const IFF_Chunk *chunk, IFF_Long *bytesProcessed)
{
    const ILBM_ColorRange *colorRange = (const ILBM_ColorRange*)chunk;
    IFF_FieldStatus status;

    if((status = IFF_writeWordField(file, colorRange->pad1, chunk, "pad1", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_writeWordField(file, colorRange->rate, chunk, "rate", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_writeWordField(file, colorRange->active, chunk, "active", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_writeUByteField(file, colorRange->low, chunk, "low", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_writeUByteField(file, colorRange->high, chunk, "high", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    return TRUE;
}

IFF_Bool ILBM_checkColorRange(const IFF_Chunk *chunk)
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

IFF_Bool ILBM_compareColorRange(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2)
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
