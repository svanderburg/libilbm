#define ILBM_NUM_OF_EXTENSION_CHUNKS 12

{"BMHD", &ILBM_readBitMapHeader, &ILBM_writeBitMapHeader, &ILBM_checkBitMapHeader, &ILBM_freeBitMapHeader, &ILBM_printBitMapHeader, &ILBM_compareBitMapHeader},
{"CAMG", &ILBM_readViewport, &ILBM_writeViewport, &ILBM_checkViewport, &ILBM_freeViewport, &ILBM_printViewport, &ILBM_compareViewport},
{"CCRT", &ILBM_readCycleInfo, &ILBM_writeCycleInfo, &ILBM_checkCycleInfo, &ILBM_freeCycleInfo, &ILBM_printCycleInfo, &ILBM_compareCycleInfo},
{"CMAP", &ILBM_readColorMap, &ILBM_writeColorMap, &ILBM_checkColorMap, &ILBM_freeColorMap, &ILBM_printColorMap, &ILBM_compareColorMap},
{"CMYK", &ILBM_readCMYKMap, &ILBM_writeCMYKMap, &ILBM_checkCMYKMap, &ILBM_freeCMYKMap, &ILBM_printCMYKMap, &ILBM_compareCMYKMap},
{"CNAM", &ILBM_readColorNames, &ILBM_writeColorNames, &ILBM_checkColorNames, &ILBM_freeColorNames, &ILBM_printColorNames, &ILBM_compareColorNames},
{"CRNG", &ILBM_readColorRange, &ILBM_writeColorRange, &ILBM_checkColorRange, &ILBM_freeColorRange, &ILBM_printColorRange, &ILBM_compareColorRange},
{"DEST", &ILBM_readDestMerge, &ILBM_writeDestMerge, &ILBM_checkDestMerge, &ILBM_freeDestMerge, &ILBM_printDestMerge, &ILBM_compareDestMerge},
{"DPI ", &ILBM_readDPIHeader, &ILBM_writeDPIHeader, &ILBM_checkDPIHeader, &ILBM_freeDPIHeader, &ILBM_printDPIHeader, &ILBM_compareDPIHeader},
{"DRNG", &ILBM_readDRange, &ILBM_writeDRange, &ILBM_checkDRange, &ILBM_freeDRange, &ILBM_printDRange, &ILBM_compareDRange},
{"GRAB", &ILBM_readGrab, &ILBM_writeGrab, &ILBM_checkGrab, &ILBM_freeGrab, &ILBM_printGrab, &ILBM_compareGrab},
{"SPRT", &ILBM_readSprite, &ILBM_writeSprite, &ILBM_checkSprite, &ILBM_freeSprite, &ILBM_printSprite, &ILBM_compareSprite}
