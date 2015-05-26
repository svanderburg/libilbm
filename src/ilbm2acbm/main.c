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
#include "ilbm2acbm.h"

#define TRUE 1
#define FALSE 0

static void printUsage(const char *command)
{
    printf("The command `ilbm2acbm' converts all ILBM images inside an IFF file to ACBM\n");
    printf("images.\n\n");
    printf("This command only converts uncompressed ILBM forms. Compressed forms are\n");
    printf("automatically skipped.\n\n");
    printf("If needed, these forms can be uncompressed by running the `ilbmpack' command\n");
    printf("first, redirecting its output to the standard output and let `ilbm2acbm' read\n");
    printf("from the standard input.\n\n");
    
#if _MSC_VER
    printf("Usage: %s [OPTION] [/i file.IFF] [/o file.IFF] file.ILBM\n\n", command);
#else
    printf("Usage: %s [OPTION] [-i file.IFF] [-o file.IFF] file.ILBM\n\n", command);
#endif

    printf("Options:\n\n");

#if _MSC_VER
    printf("  /i    Specifies the input IFF file. If no input file is given,\n");
    printf("        then data will be read from the standard input\n");
    printf("  /o    Specifies the output IFF file. If no output file is given,\n");
    printf("        then data will be written to the standard output.\n");
    printf("  /?    Shows the usage of this command to the user\n");
    printf("  /v    Shows the version of this command to the user\n");
#else
    printf("  -i, --input-file     Specifies the input IFF file. If no input file is given,\n");
    printf("                       then data will be read from the standard input\n");
    printf("  -o, --output-file    Specifies the output IFF file. If no output file is given,\n");
    printf("                       then data will be written to the standard output.\n");
    printf("  -h, --help           Shows the usage of this command to the user\n");
    printf("  -v, --version        Shows the version of this command to the user\n");
#endif
}

static void printVersion(const char *command)
{
    printf("%s (" PACKAGE_NAME ") " PACKAGE_VERSION "\n\n", command);
    printf("Copyright (C) 2012-2015 Sander van der Burg\n");
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
            case '?':
                printUsage(argv[0]);
                return 0;
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
        return ILBM_ILBMtoACBM(inputFilename, outputFilename);
}
