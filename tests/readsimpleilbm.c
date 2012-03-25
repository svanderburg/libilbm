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
    
    chunk = ILBM_read("bars.ILBM");
    
    if(chunk == NULL)
    {
	fprintf(stderr, "Cannot open ILBM file!\n");
	return 1;
    }
    
    images = ILBM_extractImages(chunk, &imagesLength);
    
    if(ILBM_checkImages(chunk, images, imagesLength))
    {
	/* We should have exactly 1 ILBM image in this IFF file */
	
	if(imagesLength == 1)
	{
	    ILBM_Image *image = images[0];
	    ILBM_BitMapHeader *bitMapHeader = image->bitMapHeader;
	    ILBM_ColorMap *colorMap = image->colorMap;
	    ILBM_Viewport *viewport = image->viewport;
	    ILBM_Point2D *point2d = image->point2d;
	    ILBM_Sprite *sprite = image->sprite;
	    
	    /* The form type of the image should be 'ILBM' */
	    
	    if(IFF_compareId(image->formType, "ILBM") != 0)
	    {
		fprintf(stderr, "The image formType should be: 'ILBM'\n");
		status = 1;
	    }
	    
	    /* Check bitmap header properties */
	    
	    if(bitMapHeader->w != 160)
	    {
		fprintf(stderr, "bitMapHeader.w should be: 160\n");
		status = 1;
	    }
	    
	    if(bitMapHeader->h != 120)
	    {
		fprintf(stderr, "bitMapHeader.h should be: 120\n");
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
	    
	    if(bitMapHeader->nPlanes != 2)
	    {
		fprintf(stderr, "bitMapHeader.nPlanes should be: 2\n");
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
	    
	    if(bitMapHeader->pageHeight != 200)
	    {
		fprintf(stderr, "bitMapHeader.pageHeight should be: 200\n");
		status = 1;
	    }
	    
	    /* The colormap should consist of 4 colors */
	    
	    if(colorMap->colorRegisterLength == 4)
	    {
		ILBM_ColorRegister *colorRegister = &colorMap->colorRegister[0];
		
		if((colorRegister->red != 0) || (colorRegister->green != 0) || (colorRegister->blue != 0))
		{
		    fprintf(stderr, "Color register 0 values should be (0x0, 0x0, 0x0)\n");
		    status = 1;
		}
		
		colorRegister = &colorMap->colorRegister[1];
		
		if((colorRegister->red != 0xf0) || (colorRegister->green != 0) || (colorRegister->blue != 0))
		{
		    fprintf(stderr, "Color register 1 values should be (0xf0, 0x0, 0x0)\n");
		    status = 1;
		}
		
		colorRegister = &colorMap->colorRegister[2];
		
		if((colorRegister->red != 0) || (colorRegister->green != 0xf0) || (colorRegister->blue != 0))
		{
		    fprintf(stderr, "Color register 2 values should be (0x0, 0xf0, 0x0)\n");
		    status = 1;
		}
		
		colorRegister = &colorMap->colorRegister[3];
		
		if((colorRegister->red != 0) || (colorRegister->green != 0) || (colorRegister->blue != 0xf0))
		{
		    fprintf(stderr, "Color register 2 values should be (0x0, 0x0, 0xf0)\n");
		    status = 1;
		}
	    }
	    else
	    {
		fprintf(stderr, "The colormap should consist of 4 colors!\n");
		status = 1;
	    }
	    
	    /* Viewport value should be 0x4 */
	    
	    if(viewport->viewportMode != 0x4)
	    {
		fprintf(stderr, "Viewport value should be 0x4\n");
		status = 1;
	    }
	    
	    /* Grab coordinates should be (10, 20) */
	    
	    if(point2d->x != 10)
	    {
		fprintf(stderr, "Grab x should be: 10\n");
		status = 1;
	    }
	    
	    if(point2d->y != 20)
	    {
		fprintf(stderr, "Grab y should be: 20\n");
		status = 1;
	    }
	    
	    /* Sprite precendence should be 1 */
	    
	    if(sprite->spritePrecedence != 1)
	    {
		fprintf(stderr, "Sprite precendence should be 1\n");
		status = 1;
	    }
	    
	    /* We have 1 color range */
	    
	    if(image->colorRangeLength == 1)
	    {
		ILBM_ColorRange *colorRange = image->colorRange[0];
		
		if(colorRange->rate != 8192)
		{
		    fprintf(stderr, "Color range rate must be 8192!\n");
		    status = 1;
		}
		
		if(colorRange->active != 1)
		{
		    fprintf(stderr, "Color range active must be 1\n");
		    status = 1;
		}
		
		if(colorRange->low != 0)
		{
		    fprintf(stderr, "Color low range must be 0\n");
		    status = 1;
		}
		
		if(colorRange->high != 3)
		{
		    fprintf(stderr, "Color high range must be 3\n");
		    status = 1;
		}
	    }
	    else
	    {
		fprintf(stderr, "The image should have 1 color range!\n");
		status = 1;
	    }
	    
	    /* We have 1 dynamic range */
	    
	    if(image->drangeLength == 1)
	    {
		ILBM_DRange *drange = image->drange[0];
		
		if(drange->min != 0)
		{
		    fprintf(stderr, "drange min must be 0\n");
		    status = 1;
		}
		
		if(drange->max != 3)
		{
		    fprintf(stderr, "drange max must be 3\n");
		    status = 1;
		}
		
		if(drange->rate != 8192)
		{
		    fprintf(stderr, "drange rate must be: 8192\n");
		    status = 1;
		}
		
		if(drange->flags != ILBM_RNG_ACTIVE)
		{
		    fprintf(stderr, "drange flags must be: %d\n", ILBM_RNG_ACTIVE);
		    status = 1;
		}
		
		if(drange->ntrue != 0)
		{
		    fprintf(stderr, "drange ntrue must be 0\n");
		    status = 1;
		}
		
		if(drange->nregs == 4)
		{
		    ILBM_DIndex *dindex;
		    
		    dindex = &drange->dindex[0];
		    
		    if(dindex->cell != 0 && dindex->index != 0)
		    {
			fprintf(stderr, "DIndex cell 0 must refer to index 0\n");
			status = 1;
		    }
		    
		    dindex = &drange->dindex[1];
		    
		    if(dindex->cell != 1 && dindex->index != 2)
		    {
			fprintf(stderr, "DIndex cell 1 must refer to index 2\n");
			status = 1;
		    }
		    
		    dindex = &drange->dindex[2];
		    
		    if(dindex->cell != 2 && dindex->index != 1)
		    {
			fprintf(stderr, "DIndex cell 2 must refer to index 1\n");
			status = 1;
		    }
		    
		    dindex = &drange->dindex[3];
		    
		    if(dindex->cell != 3 && dindex->index != 3)
		    {
			fprintf(stderr, "DIndex cell 3 must refer to index 3\n");
			status = 1;
		    }
		}
		else
		{
		    fprintf(stderr, "drange nregs must be 4\n");
		    status = 1;
		}
	    }
	    else
	    {
		fprintf(stderr, "The image should have 1 drange!\n");
		status = 1;
	    }
	    
	    if(image->cycleInfoLength == 1)
	    {
		ILBM_CycleInfo *cycleInfo = image->cycleInfo[0];
		
		if(cycleInfo->direction != 1)
		{
		    fprintf(stderr, "Cycle info direction must be 1\n");
		    status = 1;
		}
		
		if(cycleInfo->start != 0)
		{
		    fprintf(stderr, "Cycle info start must be 0\n");
		    status = 1;
		}
		
		if(cycleInfo->end != 3)
		{
		    fprintf(stderr, "Cycle info end must be 0\n");
		    status = 1;
		}
		
		if(cycleInfo->seconds != 0)
		{
		    fprintf(stderr, "Cycle info seconds must be 0\n");
		    status = 1;
		}
		
		if(cycleInfo->microSeconds != 100)
		{
		    fprintf(stderr, "Cycle info micro seconds must be 100\n");
		    status = 1;
		}
	    }
	    else
	    {
		fprintf(stderr, "The image should have 1 cycle info!\n");
		status = 1;
	    }
	}
	else
	{
	    fprintf(stderr, "The IFF file should contain exactly 1 ILBM form!\n");
	    status = 1;
	}
    }
    else
    {
	fprintf(stderr, "The IFF file containing ILBM images is not valid!\n");
	status = 1;
    }
    
    ILBM_freeImages(images, imagesLength);
    ILBM_free(chunk);
	
    return status;
}
