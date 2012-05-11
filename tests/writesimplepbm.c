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
#include <libiff/chunk.h>
#include <libiff/rawchunk.h>
#include <libiff/form.h>
#include "ilbm.h"
#include "ilbmimage.h"

int main(int argc, char *argv[])
{
    ILBM_BitMapHeader *bitMapHeader;
    ILBM_ColorMap *colorMap;

    IFF_Long bodyChunkSize;
    IFF_UByte *bodyChunkData;
    IFF_RawChunk *body;
    ILBM_Image *image;
    IFF_Form *form;
    
    unsigned int i;
    unsigned int count = 0;
    int status = 0;
    
    /* Define bitmap header */
    bitMapHeader = ILBM_createBitMapHeader();
    
    bitMapHeader->w = 320;
    bitMapHeader->h = 240;
    bitMapHeader->x = 0;
    bitMapHeader->y = 0;
    bitMapHeader->nPlanes = 8;
    bitMapHeader->masking = ILBM_MSK_NONE;
    bitMapHeader->compression = ILBM_CMP_NONE;
    bitMapHeader->xAspect = 20;
    bitMapHeader->yAspect = 22;
    bitMapHeader->pageWidth = 320;
    bitMapHeader->pageHeight = 240;
    
    /* Add some colors */
    colorMap = ILBM_createColorMap();
    
    for(i = 0; i < 64; i++)
    {
	ILBM_ColorRegister *colorRegister = ILBM_addColorRegisterInColorMap(colorMap);
	
	colorRegister->red = i * 4;
	colorRegister->green = i * 2;
	colorRegister->blue = i * 2;
    }
    
    for(i = 64; i < 128; i++)
    {
	ILBM_ColorRegister *colorRegister = ILBM_addColorRegisterInColorMap(colorMap);
	
	colorRegister->red = i;
	colorRegister->green = i * 2;
	colorRegister->blue = i / 2;
    }
    
    for(i = 128; i < 192; i++)
    {
	ILBM_ColorRegister *colorRegister = ILBM_addColorRegisterInColorMap(colorMap);
	
	colorRegister->red = i / 4;
	colorRegister->green = i / 2;
	colorRegister->blue = i;
    }
    
    for(i = 192; i < 256; i++)
    {
	ILBM_ColorRegister *colorRegister = ILBM_addColorRegisterInColorMap(colorMap);
	
	colorRegister->red = i;
	colorRegister->green = i;
	colorRegister->blue = i;
    }
    
    /* Set pixel data */
    
    bodyChunkSize = bitMapHeader->w * bitMapHeader->h; 
    bodyChunkData = (IFF_UByte*)malloc(bodyChunkSize * sizeof(IFF_UByte));
    body = IFF_createRawChunk("BODY");
    
    /* Each scanline has a new color from the palette */
    for(i = 0; i < bitMapHeader->h; i++)
    {
	memset(bodyChunkData + count, i, bitMapHeader->w);
	count += bitMapHeader->w;
    }
    
    /* Attach data to the body chunk */
    IFF_setRawChunkData(body, bodyChunkData, bodyChunkSize);
    
    /* Create image */
    image = ILBM_createImage(TRUE);
    
    image->bitMapHeader = bitMapHeader;
    image->colorMap = colorMap;
    image->body = body;
    
    /* Convert image to form */
    form = ILBM_convertImageToForm(image);
    
    /* Write the form */
    
    if(!ILBM_write("lines.LBM", (IFF_Chunk*)form))
    {
	fprintf(stderr, "Error writing PBM file!\n");
	status = 1;
    }
    
    /* Free stuff */
    
    ILBM_freeImage(image);
    ILBM_free((IFF_Chunk*)form);
    
    return status;
}
