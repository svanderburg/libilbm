/**
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

#include "ilbmimage.h"
#include "ilbm.h"
#include <stdlib.h>
#include <id.h>
#include <util.h>
#include <list.h>
#include <cat.h>
#include <form.h>
#include <error.h>

ILBM_Image *ILBM_createImage(void)
{
    return (ILBM_Image*)calloc(1, sizeof(ILBM_Image));
}

ILBM_Image **ILBM_extractImages(IFF_Chunk *chunk, unsigned int *imagesLength)
{
    unsigned int ilbmFormsLength;
    IFF_Form **ilbmForms = IFF_searchForms(chunk, "ILBM", &ilbmFormsLength);
    *imagesLength = ilbmFormsLength;
    
    if(ilbmFormsLength == 0)
    {
	IFF_error("No form with formType: 'ILBM' found!\n");
	return NULL;
    }
    else
    {
	ILBM_Image **images = (ILBM_Image**)malloc(ilbmFormsLength * sizeof(ILBM_Image*));
	unsigned int i;
	
	for(i = 0; i < ilbmFormsLength; i++)
	{
	    IFF_Form *ilbmForm = ilbmForms[i];
	    ILBM_Image *image = (ILBM_Image*)malloc(sizeof(ILBM_Image));
	    
	    image->bitMapHeader = (ILBM_BitMapHeader*)IFF_getChunkFromForm(ilbmForm, "BMHD");
	    image->colorMap = (ILBM_ColorMap*)IFF_getChunkFromForm(ilbmForm, "CMAP");
	    image->point2d = (ILBM_Point2D*)IFF_getChunkFromForm(ilbmForm, "GRAB");
	    image->destMerge = (ILBM_DestMerge*)IFF_getChunkFromForm(ilbmForm, "DEST");
	    image->sprite = (ILBM_Sprite*)IFF_getChunkFromForm(ilbmForm, "SPRT");
	    image->viewport = (ILBM_Viewport*)IFF_getChunkFromForm(ilbmForm, "CAMG");
	    image->colorRange = (ILBM_ColorRange**)IFF_getChunksFromForm(ilbmForm, "CRNG", &image->colorRangeLength);
	    image->drange = (ILBM_DRange**)IFF_getChunksFromForm(ilbmForm, "DRNG", &image->drangeLength);
	    image->cycleInfo = (ILBM_CycleInfo**)IFF_getChunksFromForm(ilbmForm, "CCRT", &image->cycleInfoLength);
	    image->body = (IFF_RawChunk*)IFF_getChunkFromForm(ilbmForm, "BODY");
	
	    images[i] = image;
	}
	
	return images;
    }
}

IFF_Form *ILBM_convertImageToForm(ILBM_Image *image)
{
    IFF_Form *form = IFF_createForm("ILBM");
    unsigned int i;
    
    if(image->bitMapHeader != NULL)
	IFF_addToForm(form, (IFF_Chunk*)image->bitMapHeader);
    
    if(image->colorMap != NULL)
	IFF_addToForm(form, (IFF_Chunk*)image->colorMap);
    
    if(image->point2d != NULL)
	IFF_addToForm(form, (IFF_Chunk*)image->point2d);

    if(image->destMerge != NULL)
	IFF_addToForm(form, (IFF_Chunk*)image->destMerge);
    
    if(image->sprite != NULL)
	IFF_addToForm(form, (IFF_Chunk*)image->sprite);
    
    if(image->viewport != NULL)
	IFF_addToForm(form, (IFF_Chunk*)image->viewport);
    
    for(i = 0; i < image->colorRangeLength; i++)
	IFF_addToForm(form, (IFF_Chunk*)image->colorRange[i]);
    
    for(i = 0; i < image->drangeLength; i++)
	IFF_addToForm(form, (IFF_Chunk*)image->drange[i]);
    
    for(i = 0; i < image->cycleInfoLength; i++)
	IFF_addToForm(form, (IFF_Chunk*)image->cycleInfo[i]);
    
    if(image->body != NULL)
	IFF_addToForm(form, (IFF_Chunk*)image->body);
    
    return form;
}

void ILBM_freeImage(ILBM_Image *image)
{
    free(image->colorRange);
    free(image->drange);
    free(image->cycleInfo);
    free(image);
}

void ILBM_freeImages(ILBM_Image **images, const unsigned int imagesLength)
{
    unsigned int i;
    
    for(i = 0; i < imagesLength; i++)
	ILBM_freeImage(images[i]);

    free(images);
}

int ILBM_checkImage(const ILBM_Image *image)
{
    if(image->bitMapHeader == NULL)
    {
	IFF_error("Error: no bitmap header defined!\n");
	return FALSE;
    }
    
    return TRUE;
}

int ILBM_checkImages(const IFF_Chunk *chunk, ILBM_Image **images, const unsigned int imagesLength)
{
    unsigned int i;
    
    /* First, check the ILBM file for corectness */
    if(!ILBM_check(chunk))
	return FALSE;
    
    /* Check the individual images inside the IFF file */
    for(i = 0; i < imagesLength; i++)
    {
	if(!ILBM_checkImage(images[i]))
	    return FALSE;
    }
    
    /* Everything seems to be correct */
    return TRUE;
}

void ILBM_addColorRangeToImage(ILBM_Image *image, ILBM_ColorRange *colorRange)
{
    image->colorRange = (ILBM_ColorRange**)realloc(image->colorRange, (image->colorRangeLength + 1) * sizeof(ILBM_ColorRange*));
    image->colorRange[image->colorRangeLength] = colorRange;
    image->colorRangeLength++;
}

void ILBM_addDRangeToImage(ILBM_Image *image, ILBM_DRange *drange)
{
    image->drange = (ILBM_DRange**)realloc(image->drange, (image->drangeLength + 1) * sizeof(ILBM_DRange*));
    image->drange[image->drangeLength] = drange;
    image->drangeLength++;
}

void ILBM_addCycleInfoToImage(ILBM_Image *image, ILBM_CycleInfo *cycleInfo)
{
    image->cycleInfo = (ILBM_CycleInfo**)realloc(image->cycleInfo, (image->cycleInfoLength + 1) * sizeof(ILBM_CycleInfo*));
    image->cycleInfo[image->cycleInfoLength] = cycleInfo;
    image->cycleInfoLength++;
}
