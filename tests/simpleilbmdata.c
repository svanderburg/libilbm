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
#include <ilbm.h>
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

static ILBM_BitMapHeader *createTestBitMapHeader(void)
{
    ILBM_BitMapHeader *bitMapHeader = ILBM_createBitMapHeader();

    bitMapHeader->w = 160;
    bitMapHeader->h = 120;
    bitMapHeader->x = 0;
    bitMapHeader->y = 0;
    bitMapHeader->nPlanes = 2;
    bitMapHeader->masking = ILBM_MSK_NONE;
    bitMapHeader->compression = ILBM_CMP_NONE;
    bitMapHeader->transparentColor = 0;
    bitMapHeader->xAspect = 20;
    bitMapHeader->yAspect = 22;
    bitMapHeader->pageWidth = 320;
    bitMapHeader->pageHeight = 200;

    return bitMapHeader;
}

static ILBM_ColorMap *createTestColorMap(void)
{
    ILBM_ColorRegister *colorRegister;
    ILBM_ColorMap *colorMap = ILBM_createColorMap();

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

    return colorMap;
}

static ILBM_CMYKMap *createTestCMYKMap(void)
{
    ILBM_CMYKRegister *cmykRegister;
    ILBM_CMYKMap *cmykMap = ILBM_createCMYKMap();

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

    return cmykMap;
}

static ILBM_ColorNames *createTestColorNames(void)
{
    ILBM_ColorNames *colorNames = ILBM_createColorNames();

    colorNames->startingColor = 0;
    colorNames->endingColor = 3;

    ILBM_addColorName(colorNames, "black");
    ILBM_addColorName(colorNames, "red");
    ILBM_addColorName(colorNames, "green");
    ILBM_addColorName(colorNames, "blue");

    return colorNames;
}

static ILBM_Viewport *createTestViewport(void)
{
    ILBM_Viewport *viewport = ILBM_createViewport();
    viewport->viewportMode = 0x4;
    return viewport;
}

static ILBM_DPIHeader *createTestDPIHeader(void)
{
    ILBM_DPIHeader *dpiHeader = ILBM_createDPIHeader();

    dpiHeader->dpiX = 100;
    dpiHeader->dpiY = 100;

    return dpiHeader;
}

static ILBM_Point2D *createTestGrab(void)
{
    ILBM_Point2D *point2d = ILBM_createGrab();

    point2d->x = 10;
    point2d->y = 20;

    return point2d;
}

static ILBM_Sprite *createTestSprite(void)
{
    ILBM_Sprite *sprite = ILBM_createSprite();
    sprite->spritePrecedence = 1;
    return sprite;
}

static ILBM_ColorRange *createTestColorRange(void)
{
    ILBM_ColorRange *colorRange = ILBM_createColorRange();

    colorRange->rate = 8192;
    colorRange->active = 1;
    colorRange->low = 0;
    colorRange->high = 3;

    return colorRange;
}

static ILBM_DRange *createTestDRange(void)
{
    ILBM_DIndex *dindex;
    ILBM_DRange *drange = ILBM_createDRange(0);

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

    return drange;
}

static ILBM_CycleInfo *createTestCycleInfo(void)
{
    ILBM_CycleInfo *cycleInfo = ILBM_createCycleInfo();

    cycleInfo->direction = 1;
    cycleInfo->start = 0;
    cycleInfo->end = 3;
    cycleInfo->seconds = 0;
    cycleInfo->microSeconds = 100;

    return cycleInfo;
}

static IFF_RawChunk *createTestBody(const ILBM_Image *image)
{
    unsigned int rowSize = ILBM_calculateRowSize(image) * image->bitMapHeader->nPlanes;
    IFF_Long bodyChunkSize = rowSize * image->bitMapHeader->h;
    IFF_UByte *bodyChunkData = (IFF_UByte*)malloc(bodyChunkSize * sizeof(IFF_UByte));
    IFF_RawChunk *body = IFF_createRawChunk(ILBM_ID_BODY);
    unsigned int count = 0;
    unsigned int i;

    /* Create a red scanline block */

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

    /* Return generated body chunk */
    return body;
}

static ILBM_Image *createTestImage(void)
{
    ILBM_ColorRange *colorRange;
    ILBM_DRange *drange;
    ILBM_CycleInfo *cycleInfo;

    ILBM_Image *image;

    /* Define a color range */
    colorRange = createTestColorRange();

    /* Define a dynamic range */
    drange = createTestDRange();

    /* Define a cycle info range */
    cycleInfo = createTestCycleInfo();

    /* Create image */
    image = ILBM_createImage(ILBM_ID_ILBM);

    image->bitMapHeader = createTestBitMapHeader();
    image->colorMap = createTestColorMap();
    image->cmykMap = createTestCMYKMap();
    image->colorNames = createTestColorNames();
    image->viewport = createTestViewport();
    image->dpiHeader = createTestDPIHeader();
    image->point2d = createTestGrab();
    image->sprite = createTestSprite();
    image->body = createTestBody(image);

    ILBM_addColorRangeToImage(image, colorRange);
    ILBM_addDRangeToImage(image, drange);
    ILBM_addCycleInfoToImage(image, cycleInfo);

    /* Return generated image */
    return image;
}

IFF_Form *ILBM_createTestForm(void)
{
    ILBM_Image *image = createTestImage();
    IFF_Form *form = ILBM_convertImageToForm(image); /* Convert image to form */

    /* Free stuff */
    ILBM_freeImage(image);

    /* Return generated form */
    return form;
}
