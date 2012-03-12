#include <stdlib.h>
#include <string.h>
#include "ilbm.h"
#include "ilbmimage.h"
#include "byterun.h"

int main(int argc, char *argv[])
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