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

#include "cmykmap.h"
#include <stdlib.h>
#include <libiff/field.h>
#include <libiff/io.h>
#include <libiff/util.h>
#include "ilbm.h"

IFF_Chunk *ILBM_createCMYKMap(const IFF_Long chunkSize)
{
    ILBM_CMYKMap *cmykMap = (ILBM_CMYKMap*)IFF_createChunk(ILBM_ID_CMYK, chunkSize, sizeof(ILBM_CMYKMap));

    if(cmykMap != NULL)
    {
        cmykMap->cmykRegisterLength = 0;
        cmykMap->cmykRegister = NULL;
    }

    return (IFF_Chunk*)cmykMap;
}

static ILBM_CMYKRegister *allocateCMYKRegisterInCMYKMap(ILBM_CMYKMap *cmykMap)
{
    ILBM_CMYKRegister *cmykRegister;

    cmykMap->cmykRegister = (ILBM_CMYKRegister*)realloc(cmykMap->cmykRegister, (cmykMap->cmykRegisterLength + 1) * sizeof(ILBM_CMYKRegister));
    cmykRegister = &cmykMap->cmykRegister[cmykMap->cmykRegisterLength];
    cmykMap->cmykRegisterLength++;

    return cmykRegister;
}

ILBM_CMYKRegister *ILBM_addCMYKRegisterInCMYKMap(ILBM_CMYKMap *cmykMap)
{
    ILBM_CMYKRegister *cmykRegister = allocateCMYKRegisterInCMYKMap(cmykMap);
    cmykMap->chunkSize += sizeof(ILBM_CMYKRegister);
    return cmykRegister;
}

IFF_Bool ILBM_readCMYKMap(FILE *file, IFF_Chunk *chunk, IFF_Long *bytesProcessed)
{
    ILBM_CMYKMap *cmykMap = (ILBM_CMYKMap*)chunk;
    IFF_FieldStatus status;

    while(*bytesProcessed < cmykMap->chunkSize)
    {
        ILBM_CMYKRegister *cmykRegister = allocateCMYKRegisterInCMYKMap(cmykMap);

        if((status = IFF_readUByteField(file, &cmykRegister->cyan, chunk, "cmykRegister.cyan", bytesProcessed)) != IFF_FIELD_MORE)
            return IFF_deriveSuccess(status);

        if((status = IFF_readUByteField(file, &cmykRegister->magenta, chunk, "cmykRegister.magenta", bytesProcessed)) != IFF_FIELD_MORE)
            return IFF_deriveSuccess(status);

        if((status = IFF_readUByteField(file, &cmykRegister->yellow, chunk, "cmykRegister.yellow", bytesProcessed)) != IFF_FIELD_MORE)
            return IFF_deriveSuccess(status);

        if((status = IFF_readUByteField(file, &cmykRegister->black, chunk, "cmykRegister.black", bytesProcessed)) != IFF_FIELD_MORE)
            return IFF_deriveSuccess(status);
    }

    return TRUE;
}

IFF_Bool ILBM_writeCMYKMap(FILE *file, const IFF_Chunk *chunk, IFF_Long *bytesProcessed)
{
    const ILBM_CMYKMap *cmykMap = (const ILBM_CMYKMap*)chunk;
    IFF_FieldStatus status;
    unsigned int i;

    for(i = 0; i < cmykMap->cmykRegisterLength; i++)
    {
        ILBM_CMYKRegister *cmykRegister = &cmykMap->cmykRegister[i];

        if((status = IFF_writeUByteField(file, cmykRegister->cyan, chunk, "cmykRegister.cyan", bytesProcessed)) != IFF_FIELD_MORE)
            return IFF_deriveSuccess(status);

        if((status = IFF_writeUByteField(file, cmykRegister->magenta, chunk, "cmykRegister.magenta", bytesProcessed)) != IFF_FIELD_MORE)
            return IFF_deriveSuccess(status);

        if((status = IFF_writeUByteField(file, cmykRegister->yellow, chunk, "cmykRegister.yellow", bytesProcessed)) != IFF_FIELD_MORE)
            return IFF_deriveSuccess(status);

        if((status = IFF_writeUByteField(file, cmykRegister->black, chunk, "cmykRegister.black", bytesProcessed)) != IFF_FIELD_MORE)
            return IFF_deriveSuccess(status);
    }

    return TRUE;
}

IFF_Bool ILBM_checkCMYKMap(const IFF_Chunk *chunk)
{
    return TRUE;
}

void ILBM_freeCMYKMap(IFF_Chunk *chunk)
{
    ILBM_CMYKMap *cmykMap = (ILBM_CMYKMap*)chunk;
    free(cmykMap->cmykRegister);
}

void ILBM_printCMYKMap(const IFF_Chunk *chunk, const unsigned int indentLevel)
{
    const ILBM_CMYKMap *cmykMap = (const ILBM_CMYKMap*)chunk;
    unsigned int i;

    for(i = 0; i < cmykMap->cmykRegisterLength; i++)
    {
        IFF_printIndent(stdout, indentLevel, "{ cyan = %x, magenta = %x, yellow = %x, black = %x };\n",
            cmykMap->cmykRegister[i].cyan, cmykMap->cmykRegister[i].magenta, cmykMap->cmykRegister[i].yellow, cmykMap->cmykRegister[i].black);
    }
}

IFF_Bool ILBM_compareCMYKMap(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2)
{
    const ILBM_CMYKMap *cmykMap1 = (const ILBM_CMYKMap*)chunk1;
    const ILBM_CMYKMap *cmykMap2 = (const ILBM_CMYKMap*)chunk2;
    unsigned int i;

    if(cmykMap1->cmykRegisterLength == cmykMap2->cmykRegisterLength)
    {
        for(i = 0; i < cmykMap1->cmykRegisterLength; i++)
        {
            if(cmykMap1->cmykRegister[i].cyan != cmykMap2->cmykRegister[i].cyan)
                return FALSE;

            if(cmykMap1->cmykRegister[i].magenta != cmykMap2->cmykRegister[i].magenta)
                return FALSE;

            if(cmykMap1->cmykRegister[i].yellow != cmykMap2->cmykRegister[i].yellow)
                return FALSE;

            if(cmykMap1->cmykRegister[i].black != cmykMap2->cmykRegister[i].black)
                return FALSE;
        }
    }
    else
        return FALSE;

    return TRUE;
}
