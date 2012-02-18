#include <stdio.h>
#include "ilbm.h"
#include "ilbmimage.h"

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
	fprintf(stderr, "ERROR: An ILBM file must be given!\n");
	return 1;
    }
    else
    {
	
	IFF_Chunk *chunk = ILBM_read(argv[1]);
	
	if(chunk == NULL)
	{
	    fprintf(stderr, "Cannot open ILBM file!\n");
	    return 1;
	}
	else
	{
	    int status;
	    unsigned int imagesLength;
	    ILBM_Image **images = ILBM_extractImages(chunk, &imagesLength);
	
	    if(ILBM_checkImages(chunk, images, imagesLength))
		status = 0;
	    else
	    {
		fprintf(stderr, "This is not a valid ILBM file!\n");
		status = 1;
	    }
	    
	    ILBM_freeImages(images, imagesLength);
	    ILBM_free(chunk);
	
	    return status;
	}
    }
}
