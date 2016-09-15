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

static IFF_UByte blackScanLine[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static IFF_UByte redScanLine[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static IFF_UByte greenScanLine[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

static IFF_UByte blueScanLine[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

IFF_Form *ILBM_createTestForm()
{
    ILBM_BitMapHeader *bitMapHeader;
    ILBM_ColorRegister *colorRegister;
    ILBM_ColorMap *colorMap;
    ILBM_CMYKRegister *cmykRegister;
    ILBM_CMYKMap *cmykMap;
    ILBM_ColorNames *colorNames;
    ILBM_Viewport *viewport;
    ILBM_DPIHeader *dpiHeader;
    ILBM_Point2D *point2d;
    ILBM_Sprite *sprite;
    ILBM_ColorRange *colorRange;
    ILBM_DRange *drange;
    ILBM_DIndex *dindex;
    ILBM_CycleInfo *cycleInfo;

    unsigned int rowSize;
    IFF_Long bodyChunkSize;
    IFF_UByte *bodyChunkData;
    IFF_RawChunk *body;
    ILBM_Image *image;
    unsigned int count = 0;
    unsigned int i;
    IFF_Form *form;

    /* Define bitmap header */
    bitMapHeader = ILBM_createBitMapHeader();
    
    bitMapHeader->w = 160;
    bitMapHeader->h = 120;
    bitMapHeader->x = 0;
    bitMapHeader->y = 0;
    bitMapHeader->nPlanes = 2;
    bitMapHeader->masking = ILBM_MSK_NONE;
    bitMapHeader->compression = ILBM_CMP_NONE;
    bitMapHeader->xAspect = 20;
    bitMapHeader->yAspect = 22;
    bitMapHeader->pageWidth = 320;
    bitMapHeader->pageHeight = 200;
    
    /* Add some RGB colors */
    colorMap = ILBM_createColorMap();
    
    colorRegister = ILBM_addColorRegisterInColorMap(colorMap);
    colorRegister->red = 0;
    colorRegister->green = 0;
    colorRegister->blue = 0;
    
    colorRegister = ILBM_addColorRegisterInColorMap(colorMap);
    colorRegister->red = 0xf0;
    colorRegister->green = 0;
    colorRegister->blue = 0;
    
    colorRegister = ILBM_addColorRegisterInColorMap(colorMap);
    colorRegister->red = 0;
    colorRegister->green = 0xf0;
    colorRegister->blue = 0;
    
    colorRegister = ILBM_addColorRegisterInColorMap(colorMap);
    colorRegister->red = 0;
    colorRegister->green = 0;
    colorRegister->blue = 0xf0;
    
    /* Add some CYMK colors */
    cmykMap = ILBM_createCMYKMap();
    
    cmykRegister = ILBM_addCMYKRegisterInCMYKMap(cmykMap);
    cmykRegister->cyan = 0xff;
    cmykRegister->magenta = 0;
    cmykRegister->yellow = 0;
    cmykRegister->black = 0;
    
    cmykRegister = ILBM_addCMYKRegisterInCMYKMap(cmykMap);
    cmykRegister->cyan = 0;
    cmykRegister->magenta = 0xff;
    cmykRegister->yellow = 0;
    cmykRegister->black = 0;
    
    cmykRegister = ILBM_addCMYKRegisterInCMYKMap(cmykMap);
    cmykRegister->cyan = 0;
    cmykRegister->magenta = 0;
    cmykRegister->yellow = 0xff;
    cmykRegister->black = 0;
    
    cmykRegister = ILBM_addCMYKRegisterInCMYKMap(cmykMap);
    cmykRegister->cyan = 0;
    cmykRegister->magenta = 0;
    cmykRegister->yellow = 0;
    cmykRegister->black = 0xff;
    
    /* Add some color names */
    colorNames = ILBM_createColorNames();
    colorNames->startingColor = 0;
    colorNames->endingColor = 3;
    
    ILBM_addColorName(colorNames, "black");
    ILBM_addColorName(colorNames, "red");
    ILBM_addColorName(colorNames, "green");
    ILBM_addColorName(colorNames, "blue");
    
    /* Set viewport */
    
    viewport = ILBM_createViewport();
    viewport->viewportMode = 0x4;
    
    /* Define DPI header */
    
    dpiHeader = ILBM_createDPIHeader();
    dpiHeader->dpiX = 100;
    dpiHeader->dpiY = 100;
    
    /* Define grab */
    
    point2d = ILBM_createGrab();
    point2d->x = 10;
    point2d->y = 20;
    
    /* Define sprite */
    
    sprite = ILBM_createSprite();
    sprite->spritePrecedence = 1;
    
    /* Define a color range */
    
    colorRange = ILBM_createColorRange();
    colorRange->rate = 8192;
    colorRange->active = 1;
    colorRange->low = 0;
    colorRange->high = 3;
    
    /* Define a dynamic range */
    
    drange = ILBM_createDRange(0);
    drange->min = 0;
    drange->max = 3;
    drange->rate = 8192;
    drange->flags = ILBM_RNG_ACTIVE;
    
    dindex = ILBM_addDIndexToDRange(drange);
    dindex->cell = 0;
    dindex->index = 0;
    
    dindex = ILBM_addDIndexToDRange(drange);
    dindex->cell = 1;
    dindex->index = 2;
    
    dindex = ILBM_addDIndexToDRange(drange);
    dindex->cell = 2;
    dindex->index = 1;
    
    dindex = ILBM_addDIndexToDRange(drange);
    dindex->cell = 3;
    dindex->index = 3;
    
    /* Define a cycle info range */
    
    cycleInfo = ILBM_createCycleInfo();
    cycleInfo->direction = 1;
    cycleInfo->start = 0;
    cycleInfo->end = 3;
    cycleInfo->seconds = 0;
    cycleInfo->microSeconds = 100;
    
    /* Create image */
    image = ILBM_createImage("ILBM");
    
    image->bitMapHeader = bitMapHeader;
    image->colorMap = colorMap;
    image->cmykMap = cmykMap;
    image->colorNames = colorNames;
    image->viewport = viewport;
    image->point2d = point2d;
    image->dpiHeader = dpiHeader;
    image->sprite = sprite;
    
    ILBM_addColorRangeToImage(image, colorRange);
    ILBM_addDRangeToImage(image, drange);
    ILBM_addCycleInfoToImage(image, cycleInfo);
    
    /* Set pixel data */
    
    /* Create a red scanline block */
    
    rowSize = ILBM_calculateRowSize(image) * bitMapHeader->nPlanes;
    bodyChunkSize = rowSize * bitMapHeader->h;
    bodyChunkData = (IFF_UByte*)malloc(bodyChunkSize * sizeof(IFF_UByte));
    body = IFF_createRawChunk("BODY");
    
    for(i = 0; i < 39; i++)
    {
	memcpy(bodyChunkData + count, redScanLine, rowSize * sizeof(IFF_UByte));
	count += rowSize;
    }
    
    /* Add black scanline */
    
    memcpy(bodyChunkData + count, blackScanLine, rowSize * sizeof(IFF_UByte));
    count += rowSize;
    
    /* Create a green scanline block */
    
    for(i = 40; i < 79; i++)
    {
	memcpy(bodyChunkData + count, greenScanLine, rowSize * sizeof(IFF_UByte));
	count += rowSize;
    }
    
    /* Add black scanline */
    
    memcpy(bodyChunkData + count, blackScanLine, rowSize * sizeof(IFF_UByte));
    count += rowSize;
    
    /* Create a blue scanline block */
    
    for(i = 80; i < 119; i++)
    {
	memcpy(bodyChunkData + count, blueScanLine, rowSize * sizeof(IFF_UByte));
	count += rowSize;
    }
    
    /* Add black scanline */
    memcpy(bodyChunkData + count, blackScanLine, rowSize * sizeof(IFF_UByte));
    count += rowSize;
    
    /* Attach data to the body chunk */
    IFF_setRawChunkData(body, bodyChunkData, bodyChunkSize);
    
    /* Attach body the image */
    image->body = body;
    
    /* Convert image to form */
    form = ILBM_convertImageToForm(image);
    
    /* Free stuff */
    ILBM_freeImage(image);
    
    /* Return generated form */
    return form;
}
