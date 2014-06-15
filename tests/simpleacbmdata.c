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

#include "simpleilbmdata.h"
#include <stdlib.h>
#include <string.h>
#include <ilbmimage.h>

IFF_Form *ILBM_createTestForm()
{
    ILBM_BitMapHeader *bitMapHeader;
    ILBM_ColorRegister *colorRegister;
    ILBM_ColorMap *colorMap;
    ILBM_Viewport *viewport;

    unsigned int bitplaneSize, offset;
    IFF_Long bitplanesChunkSize;
    IFF_UByte *bitplanesChunkData;
    IFF_RawChunk *bitplanes;
    ILBM_Image *image;
    IFF_Form *form;

    /* Define bitmap header */
    bitMapHeader = ILBM_createBitMapHeader();
    
    bitMapHeader->w = 320;
    bitMapHeader->h = 200;
    bitMapHeader->x = 0;
    bitMapHeader->y = 0;
    bitMapHeader->nPlanes = 2;
    bitMapHeader->masking = ILBM_MSK_NONE;
    bitMapHeader->compression = ILBM_CMP_NONE;
    bitMapHeader->xAspect = 20;
    bitMapHeader->yAspect = 22;
    bitMapHeader->pageWidth = 320;
    bitMapHeader->pageHeight = 200;
    
    /* Add some colors */
    colorMap = ILBM_createColorMap();
    
    colorRegister = ILBM_addColorRegisterInColorMap(colorMap);
    colorRegister->red = 0xf0;
    colorRegister->green = 0;
    colorRegister->blue = 0;
    
    colorRegister = ILBM_addColorRegisterInColorMap(colorMap);
    colorRegister->red = 0;
    colorRegister->green = 0;
    colorRegister->blue = 0xf0;
    
    colorRegister = ILBM_addColorRegisterInColorMap(colorMap);
    colorRegister->red = 0x50;
    colorRegister->green = 0;
    colorRegister->blue = 0;
    
    colorRegister = ILBM_addColorRegisterInColorMap(colorMap);
    colorRegister->red = 0;
    colorRegister->green = 0;
    colorRegister->blue = 0x50;
    
    /* Set viewport */
    viewport = ILBM_createViewport();
    viewport->viewportMode = 0x4;
    
    /* Create image */
    image = ILBM_createImage("ACBM");
    
    image->bitMapHeader = bitMapHeader;
    image->colorMap = colorMap;
    image->viewport = viewport;
    
    /* Set pixel data */
    bitplaneSize = ILBM_calculateRowSize(image) * bitMapHeader->h;
    bitplanesChunkSize = bitplaneSize * bitMapHeader->nPlanes;
    bitplanesChunkData = (IFF_UByte*)malloc(bitplanesChunkSize * sizeof(IFF_UByte));
    bitplanes = IFF_createRawChunk("ABIT");
    
    /* Compose first bitplane that uses 0 for odd pixels and 1 for even pixels */
    memset(bitplanesChunkData, 0x5, bitplaneSize);
    offset = bitplaneSize;
    
    /* The second plane has an upperhalf that consists of 0s and a lowerhalf that consists of 1s */
    memset(bitplanesChunkData + offset, 0, bitplaneSize / 2);
    offset += bitplaneSize / 2;
    memset(bitplanesChunkData + offset, 0xff, bitplaneSize / 2);
    
    /* Attach data to the body chunk */
    IFF_setRawChunkData(bitplanes, bitplanesChunkData, bitplanesChunkSize);
    
    /* Attach bitplanes to the image */
    image->bitplanes = bitplanes;
    
    /* Convert image to form */
    form = ILBM_convertImageToForm(image);
    
    /* Free stuff */
    ILBM_freeImage(image);
    
    /* Return generated form */
    return form;
}
