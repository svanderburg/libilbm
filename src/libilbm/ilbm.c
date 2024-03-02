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

#include "ilbm.h"
#include <libiff/iff.h>
#include <libiff/defaultregistry.h>
#include "ilbmchunkheaders.h"

static IFF_ChunkType applicationChunkTypes[] = {
#include "ilbmchunktypes.h"
};

static IFF_ChunkTypesNode applicationChunkTypesNode = {
    ILBM_NUM_OF_CHUNK_TYPES, applicationChunkTypes, NULL
};

static IFF_FormChunkTypes formChunkTypes[] = {
#include "ilbmformchunktypes.h"
};

static IFF_ChunkRegistry chunkRegistry = IFF_EXTEND_DEFAULT_REGISTRY_WITH_FORM_CHUNK_TYPES(ILBM_NUM_OF_FORM_CHUNK_TYPES, formChunkTypes);

IFF_Chunk *ILBM_readFd(FILE *file)
{
    return IFF_readFd(file, &chunkRegistry);
}

IFF_Chunk *ILBM_readFile(const char *filename)
{
    return IFF_readFile(filename, &chunkRegistry);
}

IFF_Chunk *ILBM_read(const char *filename)
{
    return IFF_read(filename, &chunkRegistry);
}

IFF_Bool ILBM_writeFd(FILE *file, const IFF_Chunk *chunk)
{
    return IFF_writeFd(file, chunk, &chunkRegistry);
}

IFF_Bool ILBM_writeFile(const char *filename, const IFF_Chunk *chunk)
{
    return IFF_writeFile(filename, chunk, &chunkRegistry);
}

IFF_Bool ILBM_write(const char *filename, const IFF_Chunk *chunk)
{
    return IFF_write(filename, chunk, &chunkRegistry);
}

IFF_Bool ILBM_check(const IFF_Chunk *chunk)
{
    return IFF_check(chunk, &chunkRegistry);
}

void ILBM_free(IFF_Chunk *chunk)
{
    IFF_free(chunk, &chunkRegistry);
}

void ILBM_print(const IFF_Chunk *chunk, const unsigned int indentLevel)
{
    IFF_print(chunk, indentLevel, &chunkRegistry);
}

IFF_Bool ILBM_compare(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2)
{
    return IFF_compare(chunk1, chunk2, &chunkRegistry);
}
