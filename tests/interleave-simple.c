#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ilbm.h"
#include "ilbmimage.h"
#include "interleave.h"

int main(int argc, char *argv[])
{
    unsigned int imagesLength;
    ILBM_Image **images;
    IFF_Chunk *chunk; 
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
	unsigned int i;
	ILBM_Image *image = images[0];
	unsigned int chunkSize = image->body->chunkSize;
	IFF_UByte *interleavedBitplanes = (IFF_UByte*)malloc(chunkSize * sizeof(IFF_UByte));
	    
	/* Deinterleave image */
	IFF_UByte *bitplanes = ILBM_deinterleave(image);
	    
	/* Make a copy of the original interleaved bitmap */
	memcpy(interleavedBitplanes, image->body->chunkData, chunkSize);
	    
	/* Interleave image */
	ILBM_interleave(image, bitplanes);
	    
	/* Check whether original interleaved bitmap and the new one are identical */
	
	if(chunkSize == image->body->chunkSize)
	{
	    for(i = 0; i < chunkSize; i++)
	    {
		if(interleavedBitplanes[i] != image->body->chunkData[i])
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
	    
	/* Cleanup */
	free(interleavedBitplanes);
	free(bitplanes);
    }
    else
    {
	fprintf(stderr, "The IFF file should contain 1 ILBM image!\n");
	status = 1;
    }
    
    ILBM_freeImages(images, imagesLength);
    ILBM_free(chunk);
    
    return status;
}
