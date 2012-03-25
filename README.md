libilbm
=======
`libilbm` is a portable parser library built on top of `libiff`, for ILBM: IFF
Interleaved BitMap format, which is used by programs such as Deluxe Paint and
Graphicraft to read and write images. It offers the following features:

* Reading ILBM files
* Programmatically creating ILBM files
* Retrieving ILBM file contents
* Writing ILBM files
* ILBM conformance checking
* Byte run compression of ILBM files
* Interleaving ILBM files

This parser library implements support for all chunks described in the ILBM
specification and the 'DRNG' chunk, which is a Deluxe Paint extension. These
specifications can be found in: `doc/ILBM.asc` and `doc/ILBM.DRNG.asc`, included
in this package.

Furthermore, this parser library understands PBM files, which are created by the
PC version of Deluxe Paint. Although I couldn't find any specification, the
differences are relatively minor and I have documented them in `doc/PBM.asc`.

Prerequisites
=============
In order to build and use this package the following libraries are required:

* `libiff`, in order to parse IFF files

Installation
============
Compilation and installation of this library is straight forward, by using the
standard GNU autotools build instructions:

    $ ./configure
    $ make
    $ make install

More details about the installation process can be found in the `INSTALL` file
included in this package.

Portability
===========
Because this package is implemented in ANSI C (with the small exception that the
command line utilities use `getopt()` ), it should be fairly easy to port this
package to new platforms. So far it has been tested on the following platforms:

* Linux (`i686-linux`, `x86_64-linux`) using GCC
* AmigaOS (`m68k-amigaos`) using EGCS through Geek Gadgets

License
=======
This library is available under the MIT license

Using the ILBM library API
==========================
The ILBM parser provdes a convenient interface which can be used to create,
read, write and check ILBM files. Full API documentation can be found in the
`doc/apidox` directory of this package.

Reading ILBM files
------------------
To read an ILBM file, call the `ILBM_read()` function. An IFF file may contain
multiple ILBM images (however most IFF files represent a single ILBM file).
The `ILBM_extractImages()` function extracts all the ILBM images from an IFF
file and returns an array of `ILBM_Image` struct instances, which can be used to
convienently access its properties. The length of the array is stored in the
`imagesLength` variable:

    #include <libilbm/ilbm.h>
    #include <libilbm/ilbmimage.h>
    
    int main(int argc, char *argv[])
    {
        unsigned int imagesLength;
        IFF_Chunk *chunk = ILBM_read("input.ILBM");
        ILBM_Image **image = ILBM_extractImages(chunk, &imagesLength);
        
        /* Retrieve an image from the array and access its properties here */
        
        return 0;
    }

Programmatically creating ILBM files
------------------------------------
An ILBM file can be created by creating an instance of an `ILBM_Image` struct and
by setting its sub chunks. The following example defines an ILBM image with
a bitmap header chunk:

    #include <libilbm/ilbmimage.h>
    
    int main(int argc, char *argv[])
    {
        ILBM_Image *image = ILBM_createImage(FALSE); /* If argument is TRUE, we have a PBM image instead of ILBM */
        
        ILBM_BitMapHeader *bitMapHeader = ILBM_createBitMapHeader();
        
        /* Create bitmap header properties */
        bitMapHeader->w = 320;
        bitMapHeader->h = 200;
        bitMapHeader->x = 0;
        bitMapHeader->y = 0;
        bitMapHeader->nPlanes = 4;
        bitMapHeader->compression = ILBM_CMP_NONE;
        bitMapHeader->transparentColor = 0;
        bitMapHeader->xAspect = 11;
        bitMapHeader->yAspect = 10;
        bitMapHeader->pageWidth = 320;
        bitMapHeader->pageHeight = 200;
        
        /* Attach bitmap header to the image */
        image->bitMapHeader = bitMapHeader;
        
        return 0;
    }

Retrieving ILBM file contents
-----------------------------
To retrieve properties of ILBM images inside an IFF file, you must take all
possible ILBM FORMs into account, which can be retrieved through the
`ILBM_extractImages()` function. By iterating over the resulting array, you can
retrieve the ILBM image properties by accessing members of each individual
`ILBM_Image` struct instance.

    #include <libilbm/ilbmimage.h>
    
    int main(int argc, char *argv[])
    {
        ILBM_Image **images;
        unsigned int i, imagesLength;
        
        /* Open and extract ILBM images here */
        
        for(i = 0; i < imagesLength; i++)
        {
            ILBM_Image *image = images[i];
            ILBM_BitMapHeader *bitMapHeader = image->bitMapHeader; /* Struct representing bitmap header properties */
            ILBM_ColorMap *colorMap = image->colorMap; /* Struct containing the color palette */
            IFF_RawChunk *body = image->body; /* Body chunk containing compressed or uncompressed planar graphics data */
            
            /* Retrieve more properties here */
        }
        
        return 0;
    }

Writing ILBM files
------------------
To write an `ILBM_Image` struct instance to a file, it must first be converted
to an IFF FORM using the `ILBM_convertImageToForm()` function. Then the resulting
FORM can written to disk using the `ILBM_write()` function.

    #include <libilbm/ilbmimage.h>
    #include <libilbm/ilbm.h>
    
    int main(int argc, char *argv[])
    {
        ILBM_Image *image;
        IFF_Form *form;
        
        /* Create an ILBM image here */
        
        form = ILBM_convertImageToForm(image);
        
        if(ILBM_write("output.ILBM", (IFF_Chunk*)form))
            return 0; /* File has been successfully written */
        else
            return 1; /* Writing failed */
    }

ILBM conformance checking
-------------------------
Typically when opening an IFF file containing ILBM images, you want to check
whether the file is valid. This can be done by invoking the `ILBM_checkImages()`
function, which checks whether the IFF file is valid and it checks whether are
required ILBM chunks are present.

    #include <libilbm/ilbmimage.h>
    
    int main(int argc, char *argv[])
    {
        IFF_Chunk *chunk;
        ILBM_Image **images;
        unsigned int imagesLength;

        /* Open an IFF file and extract ILBM images here */

        if(ILBM_checkImages(chunks, images, imagesLength))
            return 0; /* IFF file including ILBM images are valid */
        else
            return 1; /* IFF file including ILBM images are not valid */
    }

Byte run compression of ILBM files
----------------------------------
To compress the body of an ILBM image the `ILBM_unpackByteRun()` function can be
used. To decompress the body of an ILBM image the `ILBM_packByteRun` function can
be used.

    #include <libilbm/ilbmimage.h>
    #include <libilbm/byterun.h>

    int main(int argc, char *argv[])
    {
        ILBM_Image *image;
        
        /* Open an IFF file and extract an ILBM image here */
        
        ILBM_unpackByteRun(image); /* Now the body of the image is decompressed */
        ILBM_packByteRun(image); /* Now the body of the image is compressed */
        
        return 0;
    }

Interleaving ILBM files
-----------------------
In body of an ILBM image, bitplanes are _interleaved_, which means that they
have to be rendered line by line. The `ILBM_deinterleave()` function renders the
image in an pre-allocated piece of memory so that it can be displayed. On
non-Amiga hardware you need to convert planar graphics data into chunky or true
color graphics. `libamivideo` can be used for this purpose.

The `ILBM_interleave()` function interleaves given planar screen data, so that
they can be stored in an ILBM file.

NOTE: These functions should only be used for ILBM images and not for PBM images.
The `ILBM_imageIsPBM()` function can be used to check for this.

    #include <libilbm/ilbmimage.h>
    #include <libilbm/interleave.h>
    
    int main(int argc, char *argv[])
    {
        ILBM_Image *image;
        IFF_UByte *bitplanes;
        
        /* Open an IFF file and extract an ILBM image here */
        
        if(!ILBM_imageIsPBM(image)) /* It makes no sense for PBM images */
        {
            bitplanes = ILBM_deinterleave(image); /* Produce a deinterleaved version of the body in the resulting array */
        
            ILBM_interleave(image, bitplanes); /* Interleave the given bitplanes in the body of the image */
        
        return 0;
    }

Command-line utilities
======================
Apart from an API to handle ILBM files, this package also includes a number of
command-line utilities to make usage of ILBM files more convenient:

* `ilbmpp` can be used to pretty print an ILBM file into a textual representation, so that it can be manually inspected
* `ilbmpack` can be used to compress or decompress all ILBM images in an IFF file.

Consult the manual pages of these tools for more information.
