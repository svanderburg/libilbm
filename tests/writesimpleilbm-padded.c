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

IFF_UByte blackScanLine[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* One extra byte + 1 padding byte required */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

IFF_UByte redScanLine[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

IFF_UByte greenScanLine[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00
};

IFF_UByte blueScanLine[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00
};

static ILBM_BitMapHeader *createTestBitMapHeader(void)
{
    ILBM_BitMapHeader *bitMapHeader = ILBM_createBitMapHeader();

    bitMapHeader->w = 161;
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

    return bitMapHeader;
}

static ILBM_ColorMap *createTestColorMap(void)
{
    ILBM_ColorMap *colorMap = ILBM_createColorMap();
    ILBM_ColorRegister *colorRegister;

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

static ILBM_Viewport *createTestViewport(void)
{
    ILBM_Viewport *viewport = ILBM_createViewport();
    viewport->viewportMode = 0x4;
    return viewport;
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
    ILBM_DRange *drange = ILBM_createDRange();
    ILBM_DIndex *dindex;

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
    IFF_RawChunk *body = (IFF_RawChunk*)IFF_createRawChunk(ILBM_ID_BODY, bodyChunkSize);
    IFF_UByte *bodyChunkData = body->chunkData;
    unsigned int i;
    unsigned int count = 0;

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

    /* Return generated body */
    return body;
}

static ILBM_Image *createTestImage(void)
{
    ILBM_ColorRange *colorRange = createTestColorRange();
    ILBM_DRange *drange = createTestDRange();
    ILBM_CycleInfo *cycleInfo = createTestCycleInfo();

    ILBM_Image *image = ILBM_createImage(ILBM_ID_ILBM);

    image->bitMapHeader = createTestBitMapHeader();
    image->colorMap = createTestColorMap();
    image->viewport = createTestViewport();
    image->point2d = createTestGrab();
    image->sprite = createTestSprite();
    image->body = createTestBody(image);

    ILBM_addColorRangeToImage(image, colorRange);
    ILBM_addDRangeToImage(image, drange);
    ILBM_addCycleInfoToImage(image, cycleInfo);

    return image;
}

int main(int argc, char *argv[])
{
    int status;

    /* Convert image to form */
    ILBM_Image *image = createTestImage();
    IFF_Form *form = ILBM_convertImageToForm(image);

    /* Write the form */

    if(ILBM_write("bars-padded.ILBM", (IFF_Chunk*)form))
        status = 0;
    else
    {
        fprintf(stderr, "Error writing ILBM file!\n");
        status = 1;
    }

    /* Free stuff */

    ILBM_freeImage(image);
    ILBM_free((IFF_Chunk*)form);

    return status;
}
