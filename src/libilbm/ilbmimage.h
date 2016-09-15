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

#ifndef __ILBM_IMAGE_H
#define __ILBM_IMAGE_H

#include <libiff/form.h>
#include <libiff/rawchunk.h>
#include "bitmapheader.h"
#include "colormap.h"
#include "cmykmap.h"
#include "colornames.h"
#include "colorrange.h"
#include "cycleinfo.h"
#include "dpiheader.h"
#include "destmerge.h"
#include "grab.h"
#include "sprite.h"
#include "viewport.h"
#include "drange.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    IFF_ID formType;
    
    ILBM_BitMapHeader *bitMapHeader;
    ILBM_ColorMap *colorMap;
    ILBM_CMYKMap *cmykMap;
    ILBM_ColorNames *colorNames;
    ILBM_DPIHeader *dpiHeader;
    ILBM_Point2D *point2d;
    ILBM_DestMerge *destMerge;
    ILBM_Sprite *sprite;
    ILBM_Viewport *viewport;
    
    unsigned int colorRangeLength;
    ILBM_ColorRange **colorRange;
    
    unsigned int drangeLength;
    ILBM_DRange **drange;
    
    unsigned int cycleInfoLength;
    ILBM_CycleInfo **cycleInfo;
    
    IFF_RawChunk *body;
    IFF_RawChunk *bitplanes;
}
ILBM_Image;

ILBM_Image *ILBM_createImage(char *formType);

ILBM_Image **ILBM_extractImages(IFF_Chunk *chunk, unsigned int *imagesLength);

IFF_Form *ILBM_convertImageToForm(ILBM_Image *image);

void ILBM_freeImage(ILBM_Image *image);

void ILBM_freeImages(ILBM_Image **images, const unsigned int imagesLength);

int ILBM_checkImage(const ILBM_Image *image);

int ILBM_checkImages(const IFF_Chunk *chunk, ILBM_Image **images, const unsigned int imagesLength);

void ILBM_addColorRangeToImage(ILBM_Image *image, ILBM_ColorRange *colorRange);

void ILBM_addDRangeToImage(ILBM_Image *image, ILBM_DRange *drange);

void ILBM_addCycleInfoToImage(ILBM_Image *image, ILBM_CycleInfo *cycleInfo);

int ILBM_imageIsILBM(const ILBM_Image *image);

int ILBM_imageIsACBM(const ILBM_Image *image);

int ILBM_imageIsPBM(const ILBM_Image *image);

unsigned int ILBM_calculateRowSize(const ILBM_Image *image);

ILBM_ColorMap *ILBM_generateGrayscaleColorMap(const ILBM_Image *image);

#ifdef __cplusplus
}
#endif

#endif
