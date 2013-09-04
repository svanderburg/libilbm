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

#ifndef __ILBM_INTERLEAVE_H
#define __ILBM_INTERLEAVE_H

#include <libiff/ifftypes.h>
#include "ilbmimage.h"

#ifdef __cplusplus
extern "C" {
#endif

void ILBM_deinterleaveToBitplaneMemory(const ILBM_Image *image, IFF_UByte **bitplanePointers);

IFF_UByte *ILBM_deinterleave(const ILBM_Image *image);

void ILBM_interleaveFromBitplaneMemory(ILBM_Image *image, IFF_UByte **bitplanePointers);

void ILBM_interleave(ILBM_Image *image, IFF_UByte *bitplanes);

#ifdef __cplusplus
}
#endif

#endif
