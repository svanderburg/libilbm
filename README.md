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
* Deinterleaving ILBM files

This parser library implements support for all chunks described in the ILBM
specification that can be found in `doc/ILBM.asc`. Besides the chunk types
described in the standard, it supports the following extensions:

* The 'DRNG' dynamic color range chunk, which is a Deluxe Paint extension. The
  specification can be found in: `doc/ILBM.DRNG.asc`
* Forms of type 'ACBM', which store planar graphics data non-interleaved. The
  specification can be found in: `doc/ACBM.asc`
* Forms of type 'PBM ', which are created by the PC version of Deluxe Paint.
  Although I couldn't find any specification, the differences are relatively
  minor and I have documented them in `doc/PBM.asc`.
* A couple of other extension chunks whose specifications can be found in the
  `doc/` folder.

Prerequisites
=============
In order to build and use this package the following libraries are required:

* `libiff`, in order to parse IFF files

Installation on Unix-like systems
=================================
Compilation and installation of this library on Unix-like systems is straight
forward, by using the standard GNU autotools build instructions:

    $ ./configure
    $ make
    $ make install

More details about the installation process can be found in the `INSTALL` file
included in this package.

Building with Visual C++
========================
This package can also be built with Visual C++ for Windows platforms. The
solution file resides in `src/libilbm.sln` that can be opened in Visual Studio
to edit or build it. Alternatively, you can also use `MSBuild` to compile it:

    $ MSBuild libilbm.sln

To make any builds work you must have built `libiff` first. By default, the
project file looks for the `libiff` folder that resides in the parent directory
of the current solution.

You can also specify the location of the `libiff` includes and `libiff` libraries
through property parameters:

    $ MSBuild /p:libiffIncludePath:..\..\..\libiff\src /p:libiffLibPath:..\..\..\libiff\src\Debug libilbm.sln

The output is produced in the `Debug/` directory.

Portability
===========
Because this package is implemented in ANSI C (with the small exception that the
command line utilities use `getopt()` ), it should be fairly easy to port this
package to new platforms. So far it has been tested on the following platforms:

* Linux (`i686-linux`, `x86_64-linux`) using GCC
* Cygwin (`i686-cygwin`, `x86_64-cygwin`) using GCC
* AmigaOS (`m68k-amigaos`) using EGCS through Geek Gadgets
* Windows (`i686-windows`) using Visual C++ 2013

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

```C
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
```

Programmatically creating ILBM files
------------------------------------
An ILBM file can be created by creating an instance of an `ILBM_Image` struct and
by setting its sub chunks. The following example defines an ILBM image with
a bitmap header chunk:

```C
#include <libilbm/ilbmimage.h>

int main(int argc, char *argv[])
{
    /*
     * Specifies that we want to create an ILBM file. Also, the ACBM and PBM
     * types can be used to create these corresponding images.
     */
    ILBM_Image *image = ILBM_createImage("ILBM");
    
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
```

Retrieving ILBM file contents
-----------------------------
To retrieve properties of ILBM images inside an IFF file, you must take all
possible ILBM FORMs into account, which can be retrieved through the
`ILBM_extractImages()` function. By iterating over the resulting array, you can
retrieve the ILBM image properties by accessing members of each individual
`ILBM_Image` struct instance.

```C
#include <libilbm/ilbmimage.h>

int main(int argc, char *argv[])
{
    IFF_Chunk *chunk;
    ILBM_Image **images;
    unsigned int i, imagesLength;
    
    /* Open or create ILBM forms here */
    
    images = ILBM_extractImages(chunk, &imagesLength);
    
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
```

Writing ILBM files
------------------
To write an `ILBM_Image` struct instance to a file, it must first be converted
to an IFF FORM using the `ILBM_convertImageToForm()` function. Then the resulting
FORM can written to disk using the `ILBM_write()` function.

```C
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
```

ILBM conformance checking
-------------------------
Typically when opening an IFF file containing ILBM images, you want to check
whether the file is valid. This can be done by invoking the `ILBM_checkImages()`
function, which checks whether the IFF file is valid and it checks whether are
required ILBM chunks are present.

```C
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
```

Byte run compression of ILBM files
----------------------------------
To compress the body of an ILBM image the `ILBM_unpackByteRun()` function can be
used. To decompress the body of an ILBM image the `ILBM_packByteRun` function can
be used.

```C
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
```

Deinterleaving ILBM files
-------------------------
In body of an ILBM image, bitplanes are _interleaved_, which means that they
have to be rendered line by line. The `ILBM_deinterleave()` function renders the
image in an pre-allocated piece of memory so that it can be displayed. On
non-Amiga hardware you need to convert planar graphics data into chunky or true
color graphics. `libamivideo` can be used for this purpose.

The `ILBM_interleave()` function interleaves given planar screen data, so that
they can be stored in an ILBM file.

NOTE: These functions should only be used for ILBM images and not for PBM or ACBM
images. The `ILBM_imageIsILBM()` function can be used to check for this.

```C
#include <libilbm/ilbmimage.h>
#include <libilbm/interleave.h>

int main(int argc, char *argv[])
{
    ILBM_Image *image;
    
    /* Open an IFF file and extract an ILBM image here */
    
    if(ILBM_imageIsILBM(image)) /* It makes no sense for PBM or ACBM images */
    {
        IFF_UByte *bitplanes = ILBM_deinterleave(image); /* Produce a deinterleaved version of the body in the resulting array */
        IFF_UByte *interleavedBitplanes = ILBM_interleave(image, bitplanes); /* Interleave the given bitplanes and returns the resulting interleaved bitplane surface */
    }
    
    return 0;
}
```

Besides the interaving functions that return arrays, interleaving can also be
done directly from and to bitplane surfaces in memory on a real Amiga through
`ILBM_deinterleaveFromBitplaneMemory()` and `ILBM_interleaveToBitplaneMemory()`.

Conversion from an ILBM image (that contains interleaved bitplane surfaces in its
body) to an ACBM image (that contains a non-interleaved bitplane chunk) and the
opposite process can be done through `ILBM_convertILBMToACBM()` and
`ILBM_convertACBMToILBM()`.

Command-line utilities
======================
Apart from an API to handle ILBM files, this package also includes a number of
command-line utilities to make usage of ILBM files more convenient:

* `ilbmpp` can be used to pretty print an ILBM file into a textual representation, so that it can be manually inspected.
* `ilbmpack` can be used to compress or decompress all ILBM images in an IFF file.
* `ilbm2acbm` can be used to convert ILBM files to ACBM files.
* `acbm2ilbm` can be used to convert ACBM files to ILBM files.

Consult the manual pages of these tools for more information.
