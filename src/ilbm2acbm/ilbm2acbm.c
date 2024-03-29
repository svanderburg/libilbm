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

#include "ilbm2acbm.h"
#include "ilbm.h"
#include "ilbmimage.h"
#include "interleave.h"

int ILBM_ILBMtoACBM(const char *inputFilename, const char *outputFilename)
{
    IFF_Chunk *chunk = ILBM_read(inputFilename);

    if(chunk == NULL)
    {
        fprintf(stderr, "Error parsing ILBM file!\n");
        return 1;
    }
    else
    {
        unsigned int imagesLength;
        ILBM_Image **images = ILBM_extractImages(chunk, &imagesLength);
        int status = 0;

        if(!ILBM_checkImages(chunk, images, imagesLength))
        {
            fprintf(stderr, "Invalid ILBM file!\n");
            status = 1;
        }
        else if(imagesLength == 0)
        {
            fprintf(stderr, "No ILBM images found in IFF file!\n");
            status = 1;
        }
        else
        {
            unsigned int i;

            for(i = 0; i < imagesLength; i++)
            {
                ILBM_Image *image = images[i];

                if(image->bitMapHeader->compression == ILBM_CMP_NONE)
                {
                    if(!ILBM_convertILBMToACBM(image))
                    {
                        fprintf(stderr, "Cannot convert ILBM to ACBM image!\n");
                        status = 1;
                    }
                }
                else
                    fprintf(stderr, "WARNING: image: %d is compressed! Skipping...", i);
            }

            if(!ILBM_write(outputFilename, chunk))
            {
                fprintf(stderr, "Error writing ILBM file!\n");
                status = 1;
            }

            ILBM_freeImages(images, imagesLength);
        }

        ILBM_free(chunk);

        /* Everything has succeeded */
        return status;
    }
}
