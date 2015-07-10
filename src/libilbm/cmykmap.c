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
#include <libiff/io.h>
#include <libiff/util.h>
#include "ilbm.h"

#define CHUNKID "CMYK"

ILBM_CMYKMap *ILBM_createCMYKMap(void)
{
    ILBM_CMYKMap *cmykMap = (ILBM_CMYKMap*)IFF_allocateChunk(CHUNKID, sizeof(ILBM_CMYKMap));
    
    if(cmykMap != NULL)
    {
        cmykMap->chunkSize = 0;
    
        cmykMap->cmykRegisterLength = 0;
        cmykMap->cmykRegister = NULL;
    }
    
    return cmykMap;
}

ILBM_CMYKRegister *ILBM_addCMYKRegisterInCMYKMap(ILBM_CMYKMap *cmykMap)
{
    ILBM_CMYKRegister *cmykRegister;
    
    cmykMap->cmykRegister = (ILBM_CMYKRegister*)realloc(cmykMap->cmykRegister, (cmykMap->cmykRegisterLength + 1) * sizeof(ILBM_CMYKRegister));
    cmykRegister = &cmykMap->cmykRegister[cmykMap->cmykRegisterLength];
    cmykMap->cmykRegisterLength++;
    
    cmykMap->chunkSize += sizeof(ILBM_CMYKRegister);
    
    return cmykRegister;
}

IFF_Chunk *ILBM_readCMYKMap(FILE *file, const IFF_Long chunkSize)
{
    ILBM_CMYKMap *cmykMap = ILBM_createCMYKMap();
    
    if(cmykMap != NULL)
    {
        while(cmykMap->chunkSize < chunkSize)
        {
            ILBM_CMYKRegister *cmykRegister = ILBM_addCMYKRegisterInCMYKMap(cmykMap);
        
            if(!IFF_readUByte(file, &cmykRegister->cyan, CHUNKID, "cmykRegister.cyan"))
            {
                ILBM_free((IFF_Chunk*)cmykMap);
                return NULL;
            }
            
            if(!IFF_readUByte(file, &cmykRegister->magenta, CHUNKID, "cmykRegister.magenta"))
            {
                ILBM_free((IFF_Chunk*)cmykMap);
                return NULL;
            }
        
            if(!IFF_readUByte(file, &cmykRegister->yellow, CHUNKID, "cmykRegister.yellow"))
            {
                ILBM_free((IFF_Chunk*)cmykMap);
                return NULL;
            }
        
            if(!IFF_readUByte(file, &cmykRegister->black, CHUNKID, "cmykRegister.black"))
            {
                ILBM_free((IFF_Chunk*)cmykMap);
                return NULL;
            }
        }
    }
    
    return (IFF_Chunk*)cmykMap;
}

int ILBM_writeCMYKMap(FILE *file, const IFF_Chunk *chunk)
{
    const ILBM_CMYKMap *cmykMap = (const ILBM_CMYKMap*)chunk;
    unsigned int i;
    
    for(i = 0; i < cmykMap->cmykRegisterLength; i++)
    {
        ILBM_CMYKRegister *cmykRegister = &cmykMap->cmykRegister[i];
        
        if(!IFF_writeUByte(file, cmykRegister->cyan, CHUNKID, "cmykRegister.cyan"))
            return FALSE;
        
        if(!IFF_writeUByte(file, cmykRegister->magenta, CHUNKID, "cmykRegister.magenta"))
            return FALSE;
        
        if(!IFF_writeUByte(file, cmykRegister->yellow, CHUNKID, "cmykRegister.yellow"))
            return FALSE;
        
        if(!IFF_writeUByte(file, cmykRegister->black, CHUNKID, "cmykRegister.black"))
            return FALSE;
    }
    
    return TRUE;
}

int ILBM_checkCMYKMap(const IFF_Chunk *chunk)
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

int ILBM_compareCMYKMap(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2)
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
