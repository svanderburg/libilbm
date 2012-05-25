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

#include <stdlib.h>
#include <string.h>
#include "ilbm.h"
#include "ilbmimage.h"
#include "byterun.h"

int main(int argc, char *argv[])
{
    if(argc == 1)
    {
	fprintf(stderr, "This program requires an ILBM file as parameter!\n");
	return 1;
    }
    else
    {
	IFF_Chunk *chunk;
	unsigned int imagesLength;
	ILBM_Image **images;
	int status = 0;
    
	chunk = ILBM_read("bars.ILBM");
    
	if(chunk == NULL)
	{
	    fprintf(stderr, "Cannot open bars.ILBM\n");
	    return 1;
	}
    
	images = ILBM_extractImages(chunk, &imagesLength);
    
	if(imagesLength == 1)
	{
	    ILBM_Image *image = images[0];
	    IFF_Long chunkSize = image->body->chunkSize;
	    IFF_UByte *uncompressedBitplanes = (IFF_UByte*)malloc(chunkSize * sizeof(IFF_UByte));
	    unsigned int i;
	
	    /* Make a copy of the original uncompressed bitplanes */
	    memcpy(uncompressedBitplanes, image->body->chunkData, chunkSize);
	
	    /* Compress the body */
	    ILBM_packByteRun(image);
	
	    /* Uncompress the body */
	    ILBM_unpackByteRun(image);
	
	    /* Check whether the uncompressed body is identical to the original one */
	
	    if(chunkSize == image->body->chunkSize)
	    {
		for(i = 0; i < chunkSize; i++)
		{
		    if(uncompressedBitplanes[i] != image->body->chunkData[i])
		    {
			fprintf(stderr, "Result is not the same!\n");
			status = 1;
			break;
		    }
		}
	    }
	    else
	    {
		fprintf(stderr, "The chunk size does not match the original!\n");
		status = 1;
	    }
	
	    free(uncompressedBitplanes);
	}
    
	ILBM_freeImages(images, imagesLength);
	ILBM_free(chunk);
    
	return status;
    }
}