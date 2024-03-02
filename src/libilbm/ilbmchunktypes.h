/* An array of all extension chunks belonging to the ILBM format. They must be alphabetically sorted. */

#define ILBM_NUM_OF_CHUNK_TYPES 12

{ILBM_ID_BMHD, &ILBM_createBitMapHeaderChunk, &ILBM_readBitMapHeader, &ILBM_writeBitMapHeader, &ILBM_checkBitMapHeader, &ILBM_freeBitMapHeader, &ILBM_printBitMapHeader, &ILBM_compareBitMapHeader},
{ILBM_ID_CAMG, &ILBM_createViewportChunk, &ILBM_readViewport, &ILBM_writeViewport, &ILBM_checkViewport, &ILBM_freeViewport, &ILBM_printViewport, &ILBM_compareViewport},
{ILBM_ID_CCRT, &ILBM_createCycleInfoChunk, &ILBM_readCycleInfo, &ILBM_writeCycleInfo, &ILBM_checkCycleInfo, &ILBM_freeCycleInfo, &ILBM_printCycleInfo, &ILBM_compareCycleInfo},
{ILBM_ID_CMAP, &ILBM_createColorMapChunk, &ILBM_readColorMap, &ILBM_writeColorMap, &ILBM_checkColorMap, &ILBM_freeColorMap, &ILBM_printColorMap, &ILBM_compareColorMap},
{ILBM_ID_CMYK, &ILBM_createCMYKMapChunk, &ILBM_readCMYKMap, &ILBM_writeCMYKMap, &ILBM_checkCMYKMap, &ILBM_freeCMYKMap, &ILBM_printCMYKMap, &ILBM_compareCMYKMap},
{ILBM_ID_CNAM, &ILBM_createColorNamesChunk, &ILBM_readColorNames, &ILBM_writeColorNames, &ILBM_checkColorNames, &ILBM_freeColorNames, &ILBM_printColorNames, &ILBM_compareColorNames},
{ILBM_ID_CRNG, &ILBM_createColorRangeChunk, &ILBM_readColorRange, &ILBM_writeColorRange, &ILBM_checkColorRange, &ILBM_freeColorRange, &ILBM_printColorRange, &ILBM_compareColorRange},
{ILBM_ID_DEST, &ILBM_createDestMergeChunk, &ILBM_readDestMerge, &ILBM_writeDestMerge, &ILBM_checkDestMerge, &ILBM_freeDestMerge, &ILBM_printDestMerge, &ILBM_compareDestMerge},
{ILBM_ID_DPI, &ILBM_createDPIHeaderChunk, &ILBM_readDPIHeader, &ILBM_writeDPIHeader, &ILBM_checkDPIHeader, &ILBM_freeDPIHeader, &ILBM_printDPIHeader, &ILBM_compareDPIHeader},
{ILBM_ID_DRNG, &ILBM_createDRangeChunk, &ILBM_readDRange, &ILBM_writeDRange, &ILBM_checkDRange, &ILBM_freeDRange, &ILBM_printDRange, &ILBM_compareDRange},
{ILBM_ID_GRAB, &ILBM_createGrabChunk, &ILBM_readGrab, &ILBM_writeGrab, &ILBM_checkGrab, &ILBM_freeGrab, &ILBM_printGrab, &ILBM_compareGrab},
{ILBM_ID_SPRT, &ILBM_createSpriteChunk, &ILBM_readSprite, &ILBM_writeSprite, &ILBM_checkSprite, &ILBM_freeSprite, &ILBM_printSprite, &ILBM_compareSprite}
