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

#ifndef __ILBM_COLORMAP_H
#define __ILBM_COLORMAP_H

#include <stdio.h>
#include <libiff/ifftypes.h>
#include <libiff/group.h>
#include <libiff/chunk.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    IFF_UByte red, green, blue;
}
ILBM_ColorRegister;

typedef struct
{
    IFF_Group *parent;
    
    IFF_ID chunkId;
    IFF_Long chunkSize;
    
    unsigned int colorRegisterLength;
    ILBM_ColorRegister *colorRegister;
}
ILBM_ColorMap;

ILBM_ColorMap *ILBM_createColorMap(void);

ILBM_ColorRegister *ILBM_addColorRegisterInColorMap(ILBM_ColorMap *colorMap);

IFF_Chunk *ILBM_readColorMap(FILE *file, const IFF_Long chunkSize);

int ILBM_writeColorMap(FILE *file, const IFF_Chunk *chunk);

int ILBM_checkColorMap(const IFF_Chunk *chunk);

void ILBM_freeColorMap(IFF_Chunk *chunk);

void ILBM_printColorMap(const IFF_Chunk *chunk, const unsigned int indentLevel);

int ILBM_compareColorMap(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2);

#ifdef __cplusplus
}
#endif

#endif
