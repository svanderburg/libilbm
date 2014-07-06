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

#include "interleave.h"
#include <stdlib.h>
#include <string.h>
#include <libiff/id.h>
#include "ilbm.h"

#define MAX_NUM_OF_BITPLANES 32

void ILBM_deinterleaveToBitplaneMemory(const ILBM_Image *image, IFF_UByte **bitplanePointers)
{
    if(image->body != NULL)
    {
        unsigned int i;
        int count = 0; /* Offset in the interleaved source */
        int hOffset = 0; /* Horizontal offset in resulting bitplanes */
        unsigned int rowSize = ILBM_calculateRowSize(image);
        
        for(i = 0; i < image->bitMapHeader->h; i++)
        {
            unsigned int j;
            
            for(j = 0; j < image->bitMapHeader->nPlanes; j++)
            {
                memcpy(bitplanePointers[j] + hOffset, image->body->chunkData + count, rowSize);
                count += rowSize;
            }
            
            hOffset += rowSize;
        }
    }
}

IFF_UByte *ILBM_deinterleave(const ILBM_Image *image)
{
    IFF_UByte nPlanes = image->bitMapHeader->nPlanes;
    unsigned int bitplaneSize = ILBM_calculateRowSize(image) * image->bitMapHeader->h;
    IFF_UByte *result = (IFF_UByte*)malloc(bitplaneSize * nPlanes * sizeof(IFF_UByte));
    
    if(result == NULL)
        return NULL;
    else
    {
        unsigned int i;
        unsigned int offset = 0;
        IFF_UByte *bitplanePointers[MAX_NUM_OF_BITPLANES];
    
        /* Set bitplane pointers */
    
        for(i = 0; i < nPlanes; i++)
        {
            bitplanePointers[i] = result + offset;
            offset += bitplaneSize;
        }
    
        /* Deinterleave and write results to the bitplane addresses */
        ILBM_deinterleaveToBitplaneMemory(image, bitplanePointers);
    
        /* Return result */
        return result;
    }
}

int ILBM_convertILBMToACBM(ILBM_Image *image)
{
    if(IFF_compareId(image->formType, "ILBM") == 0 && image->bitMapHeader->compression == ILBM_CMP_NONE)
    {
        if(image->body != NULL)
        {
            /* Deinterleave the body */
            IFF_UByte *bitplaneData = ILBM_deinterleave(image);
            if(bitplaneData == NULL)
                return FALSE;
            
            /* The body chunk becomes a bitplanes chunk */
            IFF_createId(image->body->chunkId, "ABIT");
            free(image->body->chunkData);
            image->body->chunkData = bitplaneData;
            
            /* The reference in the image to bitplanes is updated as well */
            image->bitplanes = image->body;
            image->body = NULL;
        }
        
        /* Update form type to ACBM */
        IFF_createId(image->formType, "ACBM");
        IFF_createId(image->bitMapHeader->parent->groupType, "ACBM");
        
        return TRUE;
    }
    else
        return FALSE;
}

IFF_UByte *ILBM_interleaveFromBitplaneMemory(const ILBM_Image *image, IFF_UByte **bitplanePointers)
{
    unsigned int rowSize = ILBM_calculateRowSize(image);
    unsigned int interleavedScanLineSize = image->bitMapHeader->nPlanes * rowSize;
    unsigned int chunkSize = interleavedScanLineSize * image->bitMapHeader->h;
    IFF_UByte *result = (IFF_UByte*)malloc(chunkSize * sizeof(IFF_UByte));
    
    if(result == NULL)
        return NULL;
    else
    {
        unsigned int i;
        unsigned int bOffset = 0; /* Base offset in the interleaved bitplane data array */
    
        for(i = 0; i < image->bitMapHeader->nPlanes; i++)
        {
            unsigned int j;
            unsigned int hOffset = bOffset;
            unsigned int count = 0; /* Offset in the non-interleaved bitplane data array */
        
            for(j = 0; j < image->bitMapHeader->h; j++)
            {
                memcpy(result + hOffset, bitplanePointers[i] + count, rowSize);
            
                count += rowSize;
                hOffset += interleavedScanLineSize;
            }
        
            bOffset += rowSize;
        }
    
        /* Return the interleaved bitplane surface */
        return result;
    }
}

IFF_UByte *ILBM_interleave(const ILBM_Image *image, IFF_UByte *bitplanes)
{
    unsigned int bitplaneSize = ILBM_calculateRowSize(image) * image->bitMapHeader->h;
    unsigned int i;
    unsigned int offset = 0;
    
    IFF_UByte *bitplanePointers[MAX_NUM_OF_BITPLANES];
    
    /* Set bitplane pointers */
    for(i = 0; i < image->bitMapHeader->nPlanes; i++)
    {
        bitplanePointers[i] = bitplanes + offset;
        offset += bitplaneSize;
    }
    
    /* Deinterleave the bitplanes */
    return ILBM_interleaveFromBitplaneMemory(image, bitplanePointers);
}

int ILBM_convertACBMToILBM(ILBM_Image *image)
{
    if(IFF_compareId(image->formType, "ACBM") == 0 && image->bitMapHeader->compression == ILBM_CMP_NONE)
    {
        if(image->bitplanes != NULL)
        {
            /* Deinterleave the body */
            IFF_UByte *bitplaneData = ILBM_interleave(image, image->bitplanes->chunkData);
            if(bitplaneData == NULL)
                return FALSE;
            
            /* The bitplanes chunk becomes a body chunk */
            IFF_createId(image->bitplanes->chunkId, "BODY");
            free(image->bitplanes->chunkData);
            image->bitplanes->chunkData = bitplaneData;
            
            /* The reference in the image to bitplanes is updated as well */
            image->body = image->bitplanes;
            image->bitplanes = NULL;
        }
        
        /* Update form type to ILBM */
        IFF_createId(image->formType, "ILBM");
        IFF_createId(image->bitMapHeader->parent->groupType, "ILBM");
        
        return TRUE;
    }
    else
        return FALSE;
}
