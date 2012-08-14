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
#include <ilbm.h>
#include <ilbmimage.h>
#include "simplepbmdata.h"

int main(int argc, char *argv[])
{
    IFF_Chunk *chunk = ILBM_read("lines.LBM");
    
    if(chunk == NULL)
    {
	fprintf(stderr, "Cannot open ILBM file!\n");
	return 1;
    }
    else
    {
	unsigned int imagesLength;
	ILBM_Image **images = ILBM_extractImages(chunk, &imagesLength);
	int status = 0;
	
	if(ILBM_checkImages(chunk, images, imagesLength))
	{
	    IFF_Form *form = ILBM_createTestForm();
	    
	    if(!ILBM_compare(chunk, (const IFF_Chunk*)form))
	    {
		fprintf(stderr, "The IFF file is not identical to the original!\n");
		status = 1;
	    }
	
	    ILBM_free((IFF_Chunk*)form);
	}
	else
	{
	    fprintf(stderr, "The IFF file containing PBM images is not valid!\n");
	    status = 1;
	}
    
	ILBM_freeImages(images, imagesLength);
	ILBM_free(chunk);
	
	return status;
    }
}
