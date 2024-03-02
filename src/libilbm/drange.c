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
#include <libiff/field.h>
#include <libiff/io.h>
#include <libiff/util.h>
#include "ilbm.h"

IFF_Chunk *ILBM_createDRangeChunk(const IFF_ID chunkId, const IFF_Long chunkSize)
{
    ILBM_DRange *drange = (ILBM_DRange*)IFF_createChunk(chunkId, chunkSize, sizeof(ILBM_DRange));

    if(drange != NULL)
    {
        drange->min = '\0';
        drange->max = '\0';
        drange->rate = 0;
        drange->flags = 0;
        drange->ntrue = 0;
        drange->nregs = 0;
        drange->dcolor = NULL;
        drange->dindex = NULL;
        drange->nfades = 0;
        drange->pad = '\0';
        drange->dfade = NULL;
    }

    return (IFF_Chunk*)drange;
}

ILBM_DRange *ILBM_createDRange(void)
{
    return (ILBM_DRange*)ILBM_createDRangeChunk(ILBM_ID_DRNG, ILBM_DRNG_DEFAULT_SIZE);
}

static ILBM_DColor *allocateDColorInDRange(ILBM_DRange *drange)
{
    ILBM_DColor *dcolor;

    drange->dcolor = (ILBM_DColor*)realloc(drange->dcolor, (drange->ntrue + 1) * sizeof(ILBM_DColor));
    dcolor = &drange->dcolor[drange->ntrue];
    drange->ntrue++;

    return dcolor;
}

ILBM_DColor *ILBM_addDColorToDRange(ILBM_DRange *drange)
{
    ILBM_DColor *dcolor = allocateDColorInDRange(drange);
    drange->chunkSize += sizeof(ILBM_DColor);
    return dcolor;
}

static ILBM_DIndex *allocateDIndexInDRange(ILBM_DRange *drange)
{
    ILBM_DIndex *dindex;

    drange->dindex = (ILBM_DIndex*)realloc(drange->dindex, (drange->nregs + 1) * sizeof(ILBM_DIndex));
    dindex = &drange->dindex[drange->nregs];
    drange->nregs++;

    return dindex;
}

ILBM_DIndex *ILBM_addDIndexToDRange(ILBM_DRange *drange)
{
    ILBM_DIndex *dindex = allocateDIndexInDRange(drange);
    drange->chunkSize += sizeof(ILBM_DIndex);
    return dindex;
}

static ILBM_DFade *allocateDFadeInDRange(ILBM_DRange *drange)
{
    ILBM_DFade *dfade;

    drange->dfade = (ILBM_DFade*)realloc(drange->dfade, (drange->nfades + 1) * sizeof(ILBM_DFade));
    dfade = &drange->dfade[drange->nfades];
    drange->nfades++;

    return dfade;
}

ILBM_DFade *ILBM_addDFadeToDRange(ILBM_DRange *drange)
{
    ILBM_DFade *dfade = allocateDFadeInDRange(drange);
    drange->chunkSize += sizeof(ILBM_DFade);
    return dfade;
}

IFF_Bool ILBM_readDRange(FILE *file, IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed)
{
    ILBM_DRange *drange = (ILBM_DRange*)chunk;
    IFF_FieldStatus status;
    IFF_UByte ntrue, nregs, nfades;
    unsigned int i;

    if((status = IFF_readUByteField(file, &drange->min, chunk, "min", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_readUByteField(file, &drange->max, chunk, "max", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_readWordField(file, &drange->rate, chunk, "rate", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_readWordField(file, &drange->flags, chunk, "flags", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_readUByteField(file, &ntrue, chunk, "ntrue", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_readUByteField(file, &nregs, chunk, "nregs", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    for(i = 0; i < ntrue; i++)
    {
        ILBM_DColor *dcolor = allocateDColorInDRange(drange);

        if((status = IFF_readUByteField(file, &dcolor->cell, chunk, "dcolor.cell", bytesProcessed)) != IFF_FIELD_MORE)
            return IFF_deriveSuccess(status);

        if((status = IFF_readUByteField(file, &dcolor->r, chunk, "dcolor.r", bytesProcessed)) != IFF_FIELD_MORE)
            return IFF_deriveSuccess(status);

        if((status = IFF_readUByteField(file, &dcolor->g, chunk, "dcolor.g", bytesProcessed)) != IFF_FIELD_MORE)
            return IFF_deriveSuccess(status);

        if((status = IFF_readUByteField(file, &dcolor->b, chunk, "dcolor.b", bytesProcessed)) != IFF_FIELD_MORE)
            return IFF_deriveSuccess(status);
    }

    for(i = 0; i < nregs; i++)
    {
        ILBM_DIndex *dindex = allocateDIndexInDRange(drange);

        if((status = IFF_readUByteField(file, &dindex->cell, chunk, "dindex.cell", bytesProcessed)) != IFF_FIELD_MORE)
            return IFF_deriveSuccess(status);

        if((status = IFF_readUByteField(file, &dindex->index, chunk, "dindex.index", bytesProcessed)) != IFF_FIELD_MORE)
            return IFF_deriveSuccess(status);
    }

    if((status = IFF_readUByteField(file, &nfades, chunk, "nfades", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_readUByteField(file, &drange->pad, chunk, "pad", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    for(i = 0; i < nfades; i++)
    {
        ILBM_DFade *dfade = allocateDFadeInDRange(drange);

        if((status = IFF_readUByteField(file, &dfade->cell, chunk, "dfade.cell", bytesProcessed)) != IFF_FIELD_MORE)
            return IFF_deriveSuccess(status);

        if((status = IFF_readUByteField(file, &dfade->fade, chunk, "dfade.fade", bytesProcessed)) != IFF_FIELD_MORE)
            return IFF_deriveSuccess(status);
    }

    return TRUE;
}

IFF_Bool ILBM_writeDRange(FILE *file, const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry, IFF_Long *bytesProcessed)
{
    const ILBM_DRange *drange = (const ILBM_DRange*)chunk;
    IFF_FieldStatus status;
    unsigned int i;

    if((status = IFF_writeUByteField(file, drange->min, chunk, "min", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_writeUByteField(file, drange->max, chunk, "max", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_writeWordField(file, drange->rate, chunk, "rate", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_writeWordField(file, drange->flags, chunk, "flags", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_writeUByteField(file, drange->ntrue, chunk, "ntrue", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_writeUByteField(file, drange->nregs, chunk, "nregs", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    for(i = 0; i < drange->ntrue; i++)
    {
        ILBM_DColor *dcolor = &drange->dcolor[i];

        if((status = IFF_writeUByteField(file, dcolor->cell, chunk, "dcolor.cell", bytesProcessed)) != IFF_FIELD_MORE)
            return IFF_deriveSuccess(status);

        if((status = IFF_writeUByteField(file, dcolor->r, chunk, "dcolor.r", bytesProcessed)) != IFF_FIELD_MORE)
            return IFF_deriveSuccess(status);

        if((status = IFF_writeUByteField(file, dcolor->g, chunk, "dcolor.g", bytesProcessed)) != IFF_FIELD_MORE)
            return IFF_deriveSuccess(status);

        if((status = IFF_writeUByteField(file, dcolor->b, chunk, "dcolor.b", bytesProcessed)) != IFF_FIELD_MORE)
            return IFF_deriveSuccess(status);
    }

    for(i = 0; i < drange->nregs; i++)
    {
        ILBM_DIndex *dindex = &drange->dindex[i];

        if((status = IFF_writeUByteField(file, dindex->cell, chunk, "dindex.cell", bytesProcessed)) != IFF_FIELD_MORE)
            return IFF_deriveSuccess(status);

        if((status = IFF_writeUByteField(file, dindex->index, chunk, "dindex.index", bytesProcessed)) != IFF_FIELD_MORE)
            return IFF_deriveSuccess(status);
    }

    if((status = IFF_writeUByteField(file, drange->nfades, chunk, "nfades", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    if((status = IFF_writeUByteField(file, drange->pad, chunk, "pad", bytesProcessed)) != IFF_FIELD_MORE)
        return IFF_deriveSuccess(status);

    for(i = 0; i < drange->nfades; i++)
    {
        ILBM_DFade *dfade = &drange->dfade[i];

        if((status = IFF_writeUByteField(file, dfade->cell, chunk, "dfade.cell", bytesProcessed)) != IFF_FIELD_MORE)
            return IFF_deriveSuccess(status);

        if((status = IFF_writeUByteField(file, dfade->fade, chunk, "dfade.fade", bytesProcessed)) != IFF_FIELD_MORE)
            return IFF_deriveSuccess(status);
    }

    return TRUE;
}

IFF_Bool ILBM_checkDRange(const IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry)
{
    return TRUE;
}

void ILBM_freeDRange(IFF_Chunk *chunk, const IFF_ChunkRegistry *chunkRegistry)
{
    ILBM_DRange *drange = (ILBM_DRange*)chunk;

    free(drange->dcolor);
    free(drange->dindex);
    free(drange->dfade);
}

void ILBM_printDRange(const IFF_Chunk *chunk, const unsigned int indentLevel, const IFF_ChunkRegistry *chunkRegistry)
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

    IFF_printIndent(stdout, indentLevel, "nfades = %u;\n", drange->nfades);
    IFF_printIndent(stdout, indentLevel, "pad = %u;\n", drange->pad);

    for(i = 0; i < drange->nfades; i++)
        IFF_printIndent(stdout, indentLevel, "{ cell = %u, fade = %u }\n", drange->dfade[i].cell, drange->dfade[i].fade);
}

IFF_Bool ILBM_compareDRange(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2, const IFF_ChunkRegistry *chunkRegistry)
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

    return TRUE;
}
