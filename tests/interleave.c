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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ilbm.h"
#include "ilbmimage.h"
#include "interleave.h"

int main(int argc, char *argv[])
{
    if(argc == 1)
    {
        fprintf(stderr, "This program requires an ILBM file as parameter!\n");
        return 1;
    }
    else
    {
        unsigned int imagesLength;
        ILBM_Image **images;
        IFF_Chunk *chunk; 
        int status = 0;
    
        chunk = ILBM_read(argv[1]);
    
        if(chunk == NULL)
        {
            fprintf(stderr, "Cannot open bars.ILBM\n");
            return 1;
        }
    
        images = ILBM_extractImages(chunk, &imagesLength);
    
        if(imagesLength == 1)
        {
            unsigned int i;
            ILBM_Image *image = images[0];
            unsigned int chunkSize = image->body->chunkSize;
            IFF_UByte *interleavedBitplanes = (IFF_UByte*)malloc(chunkSize * sizeof(IFF_UByte));
            
            /* Make a copy of the original interleaved bitmap */
            memcpy(interleavedBitplanes, image->body->chunkData, chunkSize);
            
            /* Deinterleave image */
            if(!ILBM_convertILBMToACBM(image))
            {
                fprintf(stderr, "Error while converting image to ACBM!\n");
                status = 1;
            }
            
            /* Interleave image */
            if(!ILBM_convertACBMToILBM(image))
            {
                fprintf(stderr, "Error while converting image to ILBM!\n");
                status = 1;
            }
            
            /* Check whether original interleaved bitmap and the new one are identical */
        
            if(chunkSize == image->body->chunkSize)
            {
                for(i = 0; i < chunkSize; i++)
                {
                    if(interleavedBitplanes[i] != image->body->chunkData[i])
                    {
                        fprintf(stderr, "Result is not the same!\n");
                        status = 1;
                        break;
                    }
                }
            }
            else
            {
                fprintf(stderr, "The chunk size does not match the original!\n");
                status = 1;
            }
            
            /* Cleanup */
            free(interleavedBitplanes);
        }
        else
        {
            fprintf(stderr, "The IFF file should contain 1 ILBM image!\n");
            status = 1;
        }
    
        ILBM_freeImages(images, imagesLength);
        ILBM_free(chunk);
    
        return status;
    }
}
