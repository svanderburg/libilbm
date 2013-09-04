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

#include "byterun.h"
#include <stdlib.h>
#include <string.h>
#include <libiff/rawchunk.h>
#include <libiff/util.h>
#include <libiff/error.h>
#include "ilbm.h"

void ILBM_unpackByteRun(ILBM_Image *image)
{
    IFF_RawChunk *body = image->body;
    
    /* Only perform decompression if the body is compressed and present */
    if(image->bitMapHeader->compression == ILBM_CMP_BYTE_RUN && body != NULL)
    {
	/* Counters */
	unsigned int count = 0;
	unsigned int readBytes = 0;
    
	/* Allocate decompressed chunk attributes */
	
	IFF_Long chunkSize = ILBM_calculateRowSize(image) * image->bitMapHeader->h * image->bitMapHeader->nPlanes;
	IFF_UByte *decompressedChunkData = (IFF_UByte*)malloc(chunkSize * sizeof(IFF_UByte));
	
	/* Perform RLE decompression */
    
	while(readBytes < body->chunkSize)
	{
	    int byte = (IFF_Byte)body->chunkData[readBytes];
	    readBytes++;
	    
	    if(byte >= 0 && byte <= 127) /* Take the next byte bytes + 1 literally */
	    {
		int i;
		
		for(i = 0; i < byte + 1; i++)
		{
		    decompressedChunkData[count] = body->chunkData[readBytes];
		    readBytes++;
		    count++;
		}
	    }
	    else if(byte >= -127 && byte <= -1) /* Replicate the next byte, -byte + 1 times */
	    {
		IFF_UByte ubyte;
		int i;
	    
		ubyte = body->chunkData[readBytes];
		readBytes++;
	    
		for(i = 0; i < -byte + 1; i++)
		{
		    decompressedChunkData[count] = ubyte;
		    count++;
		}
	    }
	    else
		IFF_error("Unknown byte run encoding byte!\n");
	}
	
	/* Free the compressed chunk data */
	free(body->chunkData);
	
	/* Add decompressed chunk data to the body chunk */
	IFF_setRawChunkData(body, decompressedChunkData, chunkSize);
    
	/* Recursively update the chunk sizes */
	IFF_updateChunkSizes((IFF_Chunk*)body);

	/* Change compression flag, since the body is no longer compressed anymore */
	image->bitMapHeader->compression = ILBM_CMP_NONE;
    }
}

typedef enum
{
    MODE_UNKNOWN = 0,
    MODE_RUN = 1,
    MODE_DUMP = 2,
    MODE_MAYBE_RUN = 3
}
PackMode;

static int addRun(unsigned int equalCount, IFF_UByte *compressedChunkData, unsigned int count, IFF_UByte previousByte)
{
    char byte = 1 - equalCount;

    compressedChunkData[count] = byte;
    count++;
    
    compressedChunkData[count] = previousByte;
    count++;
    
    return count;
}

static int addDump(unsigned int equalCount, IFF_UByte *compressedChunkData, unsigned int count, IFF_UByte *uncompressedChunkData, unsigned int readBytes)
{
    unsigned int i;
    
    compressedChunkData[count] = equalCount - 1;
    count++;

    for(i = readBytes - equalCount; i < readBytes; i++)
    {
	compressedChunkData[count] = uncompressedChunkData[i];
	count++;
    }

    return count;
}

static int packRow(IFF_UByte *uncompressedChunkData, IFF_UByte *compressedChunkData, unsigned int uncompressedOffset, unsigned int compressedOffset, const unsigned int rowSize)
{
    IFF_UByte previousByte, currentByte;
    
    PackMode mode = MODE_UNKNOWN;
    unsigned int equalCount = 1;
    
    unsigned int readBytes = uncompressedOffset;
    unsigned int count = compressedOffset;
    
    /* Read first byte */
    currentByte = uncompressedChunkData[readBytes];
    readBytes++;
    
    while(readBytes < uncompressedOffset + rowSize)
    {
	/* Shift previous byte */
	previousByte = currentByte;
	
	/* Read next byte */
	currentByte = uncompressedChunkData[readBytes];
	readBytes++;
	
	switch(mode)
	{
	    case MODE_UNKNOWN:
		equalCount++;
		
		if(previousByte == currentByte)
		    mode = MODE_RUN;
		else
		    mode = MODE_DUMP;
		
		break;
	
	    case MODE_RUN:
		if(previousByte == currentByte)
		    equalCount++;
		else
		{
		    count = addRun(equalCount, compressedChunkData, count, previousByte);
		    
		    equalCount = 1;
		    mode = MODE_UNKNOWN;
		}
		
		break;
	    
	    case MODE_DUMP:
		if(previousByte == currentByte)
		    mode = MODE_MAYBE_RUN;
		else
		    equalCount++;
		    
		break;
	
	    case MODE_MAYBE_RUN:
		if(previousByte == currentByte)
		{
		    equalCount--;
		    
		    count = addDump(equalCount, compressedChunkData, count, uncompressedChunkData, readBytes - 3);
		    
		    equalCount = 3;
		    mode = MODE_RUN;
		}
		else
		{
		    equalCount += 2;
		    mode = MODE_DUMP;
		}
		
		break;
	}
    }
    
    /* Write remaining bytes */
    switch(mode)
    {
	case MODE_RUN:
	    count = addRun(equalCount, compressedChunkData, count, previousByte);
	    break;

	case MODE_DUMP:
	case MODE_UNKNOWN:
	    count = addDump(equalCount, compressedChunkData, count, uncompressedChunkData, readBytes);
	    break;

	case MODE_MAYBE_RUN:
	    equalCount++;
	    count = addDump(equalCount, compressedChunkData, count, uncompressedChunkData, readBytes);
	    break;
    }
    
    return count;
}

void ILBM_packByteRun(ILBM_Image *image)
{
    IFF_RawChunk *body = image->body;
    
    /* Only perform decompression if the body is decompressed and present */
    if(image->bitMapHeader->compression == ILBM_CMP_NONE && body != NULL)
    {
	unsigned int readBytes = 0;
	unsigned int rowSize = ILBM_calculateRowSize(image);
	IFF_UByte *compressedChunkData = (IFF_UByte*)malloc(body->chunkSize * sizeof(IFF_UByte)); /* Scanline + 1 * height */
	unsigned int count = 0;
	
	while(readBytes < body->chunkSize)
	{
	    count = packRow(body->chunkData, compressedChunkData, readBytes, count, rowSize);
	    readBytes += rowSize;
	}
	
	/* We can shrink the size of the data, because compression makes it in most cases smaller */
	compressedChunkData = (IFF_UByte*)realloc(compressedChunkData, count * sizeof(IFF_UByte));
	
	/* Free the decompressed body data */
	free(body->chunkData);
	
	/* Attach compressed chunk data to the chunk */
	IFF_setRawChunkData(body, compressedChunkData, count);
	
	/* Recursively update the chunk sizes */
	IFF_updateChunkSizes((IFF_Chunk*)body);
	
	/* Change compression flag, since the body is compressed now */
	image->bitMapHeader->compression = ILBM_CMP_BYTE_RUN;
    }
}
