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

#if HAVE_GETOPT_H == 1
#include <getopt.h>
#elif _MSC_VER
#include <string.h>
#else
#include <unistd.h>
#endif

#include <stdio.h>
#include "acbm2ilbm.h"

#define TRUE 1
#define FALSE 0

static void printUsage(const char *command)
{
    printf(
#if _MSC_VER
    "Usage: %s [OPTION] [/i file.IFF] [/o file.IFF] file.ACBM\n\n"
#else
    "Usage: %s [OPTION] [-i file.IFF] [-o file.IFF] file.ACBM\n\n"
#endif
     , command);

    puts(
    "The command `acbm2ilbm' converts all ACBM images inside an IFF file to ILBM\n"
    "images.\n\n"
    "This command only converts uncompressed ACBM forms. Compressed forms are\n"
    "automatically skipped.\n\n"
    "If needed, these forms can be uncompressed by running the `ilbmpack' command\n"
    "first, redirecting its output to the standard output and let `acbm2ilbm' read\n"
    "from the standard input.\n"
    );

    puts(
    "Options:\n\n"
#if _MSC_VER
    "  /i FILE    Specifies the input IFF file. If no input file is given,\n"
    "             then data will be read from the standard input\n"
    "  /o FILE    Specifies the output IFF file. If no output file is given,\n"
    "             then data will be written to the standard output.\n"
    "  /?         Shows the usage of this command to the user\n"
    "  /v         Shows the version of this command to the user"
#else
    "  -i, --input-file=FILE   Specifies the input IFF file. If no input file is\n"
    "                          given, then data will be read from the standard input\n"
    "  -o, --output-file=FILE  Specifies the output IFF file. If no output file is\n"
    "                          given, then data will be written to the standard\n"
    "                          output.\n"
    "  -h, --help              Shows the usage of this command to the user\n"
    "  -v, --version           Shows the version of this command to the user"
#endif
    );
}

static void printVersion(const char *command)
{
    printf(
    "%s (" PACKAGE_NAME ") " PACKAGE_VERSION "\n\n"
    "Copyright (C) 2012-2015 Sander van der Burg\n"
    , command);
}

int main(int argc, char *argv[])
{
    char *inputFilename = NULL;
    char *outputFilename = NULL;
#if _MSC_VER
    unsigned int optind = 1;
    unsigned int i;
    int inputFilenameFollows = FALSE;
    int outputFilenameFollows = FALSE;
    
    for(i = 1; i < argc; i++)
    {
        if (inputFilenameFollows)
        {
            inputFilename = argv[i];
            inputFilenameFollows = FALSE;
            optind++;
        }
        else if (outputFilenameFollows)
        {
            outputFilename = argv[i];
            outputFilenameFollows = FALSE;
            optind++;
        }
        else if (strcmp(argv[i], "/i") == 0)
        {
            inputFilenameFollows = TRUE;
            optind++;
        }
        else if (strcmp(argv[i], "/o") == 0)
        {
            outputFilenameFollows = FALSE;
            optind++;
        }
        else if (strcmp(argv[i], "/?") == 0)
        {
            printUsage(argv[0]);
            return 0;
        }
        else if (strcmp(argv[i], "/v") == 0)
        {
            printVersion(argv[0]);
            return 0;
        }
    }
#else
    int c;
#if HAVE_GETOPT_H == 1
    int option_index = 0;
    struct option long_options[] =
    {
        {"input-file", required_argument, 0, 'i'},
        {"output-file", required_argument, 0, 'o'},
        {"help", no_argument, 0, 'h'},
        {"version", no_argument, 0, 'v'},
        {0, 0, 0, 0}
    };
#endif

    /* Parse command-line options */
    
#if HAVE_GETOPT_H == 1
    while((c = getopt_long(argc, argv, "i:o:hv", long_options, &option_index)) != -1)
#else
    while((c = getopt(argc, argv, "i:o:hv")) != -1)
#endif
    {
        switch(c)
        {
            case 'i':
                inputFilename = optarg;
                break;
            case 'o':
                outputFilename = optarg;
                break;
            case 'h':
                printUsage(argv[0]);
                return 0;
            case '?':
                printUsage(argv[0]);
                return 1;
            case 'v':
                printVersion(argv[0]);
                return 0;
        }
    }
#endif
    /* Check parameters */

    if(inputFilename == NULL && outputFilename == NULL)
    {
        fprintf(stderr, "ERROR: At least an input file or output file must be specified!\n");
        return 1;
    }
    else
        return ILBM_ACBMtoILBM(inputFilename, outputFilename);
}
