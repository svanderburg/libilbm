#include <stdio.h>
#include <libiff/id.h>
#include "ilbm.h"
#include "ilbmimage.h"

int main(int argc, char *argv[])
{
    int status = 0;
    IFF_Chunk *chunk;
    unsigned int imagesLength;
    ILBM_Image **images; 
    
    chunk = ILBM_read("lines.LBM");
    
    if(chunk == NULL)
    {
	fprintf(stderr, "Cannot open ILBM file!\n");
	return 1;
    }
    
    images = ILBM_extractImages(chunk, &imagesLength);
    
    if(ILBM_checkImages(chunk, images, imagesLength))
    {
	/* We should have exactly 1 PBM image in this IFF file */
	
	if(imagesLength == 1)
	{
	    ILBM_Image *image = images[0];
	    ILBM_BitMapHeader *bitMapHeader = image->bitMapHeader;
	    ILBM_ColorMap *colorMap = image->colorMap;
	    
	    /* The form type of the image should be 'PBM ' */
	    
	    if(IFF_compareId(image->formType, "PBM ") != 0)
	    {
		fprintf(stderr, "The image formType should be: 'PBM '\n");
		status = 1;
	    }
	    
	    /* Check bitmap header properties */
	    
	    if(bitMapHeader->w != 320)
	    {
		fprintf(stderr, "bitMapHeader.w should be: 320\n");
		status = 1;
	    }
	    
	    if(bitMapHeader->h != 240)
	    {
		fprintf(stderr, "bitMapHeader.h should be: 240\n");
		status = 1;
	    }
	    
	    if(bitMapHeader->x != 0)
	    {
		fprintf(stderr, "bitMapHeader.x should be: 0\n");
		status = 1;
	    }
	    
	    if(bitMapHeader->y != 0)
	    {
		fprintf(stderr, "bitMapHeader.y should be: 0\n");
		status = 1;
	    }
	    
	    if(bitMapHeader->nPlanes != 8)
	    {
		fprintf(stderr, "bitMapHeader.nPlanes should be: 8\n");
		status = 1;
	    }
	    
	    if(bitMapHeader->masking != ILBM_MSK_NONE)
	    {
		fprintf(stderr, "bitMapHeader.masking should be: %d\n", ILBM_MSK_NONE);
		status = 1;
	    }
	    
	    if(bitMapHeader->compression != ILBM_CMP_NONE)
	    {
		fprintf(stderr, "bitMapHeader.compression should be: %d\n", ILBM_CMP_NONE);
		status = 1;
	    }
	    
	    if(bitMapHeader->xAspect != 20)
	    {
		fprintf(stderr, "bitMapHeader.xAspect should be: 20\n");
		status = 1;
	    }
	    
	    if(bitMapHeader->yAspect != 22)
	    {
		fprintf(stderr, "bitMapHeader.yAspect should be: 22\n");
		status = 1;
	    }
	    
	    if(bitMapHeader->pageWidth != 320)
	    {
		fprintf(stderr, "bitMapHeader.pageWidth should be: 320\n");
		status = 1;
	    }
	    
	    if(bitMapHeader->pageHeight != 240)
	    {
		fprintf(stderr, "bitMapHeader.pageHeight should be: 240\n");
		status = 1;
	    }
	    
	    /* The colormap should consist of 256 colors */
	    
	    if(colorMap->colorRegisterLength != 256)
	    {
		fprintf(stderr, "The colormap should consist of 256 colors!\n");
		status = 1;
	    }
	}
	else
	{
	    fprintf(stderr, "The IFF file should contain exactly 1 PBM form!\n");
	    status = 1;
	}
    }
    else
    {
	fprintf(stderr, "The IFF file containing PBM images is not valid!\n");
	status = 1;
    }
    
    ILBM_freeImages(images, imagesLength);
    ILBM_free(chunk);
	
    return status;
}
