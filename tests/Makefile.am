check_PROGRAMS = writesimpleilbm writesimpleilbm-padded readsimpleilbm checkilbm writesimplepbm readsimplepbm writesimpleacbm readsimpleacbm interleave byterun

noinst_HEADERS = simpleilbmdata.h simplepbmdata.h simpleacbmdata.h

writesimpleilbm_SOURCES = simpleilbmdata.c writesimpleilbm.c
writesimpleilbm_LDADD = ../src/libilbm/libilbm.la $(LIBIFF_LIBS)
writesimpleilbm_CFLAGS = -I../src/libilbm $(LIBIFF_CFLAGS)

writesimpleilbm_padded_SOURCES = writesimpleilbm-padded.c
writesimpleilbm_padded_LDADD = ../src/libilbm/libilbm.la $(LIBIFF_LIBS)
writesimpleilbm_padded_CFLAGS = -I../src/libilbm $(LIBIFF_CFLAGS)

readsimpleilbm_SOURCES = simpleilbmdata.c readsimpleilbm.c
readsimpleilbm_LDADD = ../src/libilbm/libilbm.la $(LIBIFF_LIBS)
readsimpleilbm_CFLAGS = -I../src/libilbm $(LIBIFF_CFLAGS)

checkilbm_SOURCES = checkilbm.c
checkilbm_LDADD = ../src/libilbm/libilbm.la $(LIBIFF_LIBS)
checkilbm_CFLAGS = -I../src/libilbm $(LIBIFF_CFLAGS)

writesimplepbm_SOURCES = simplepbmdata.c writesimplepbm.c
writesimplepbm_LDADD = ../src/libilbm/libilbm.la $(LIBIFF_LIBS)
writesimplepbm_CFLAGS = -I../src/libilbm $(LIBIFF_CFLAGS)

readsimplepbm_SOURCES = simplepbmdata.c readsimplepbm.c
readsimplepbm_LDADD = ../src/libilbm/libilbm.la $(LIBIFF_LIBS)
readsimplepbm_CFLAGS = -I../src/libilbm $(LIBIFF_CFLAGS)

writesimpleacbm_SOURCES = simpleacbmdata.c writesimpleacbm.c
writesimpleacbm_LDADD = ../src/libilbm/libilbm.la $(LIBIFF_LIBS)
writesimpleacbm_CFLAGS = -I../src/libilbm $(LIBIFF_CFLAGS)

readsimpleacbm_SOURCES = simpleacbmdata.c readsimpleacbm.c
readsimpleacbm_LDADD = ../src/libilbm/libilbm.la $(LIBIFF_LIBS)
readsimpleacbm_CFLAGS = -I../src/libilbm $(LIBIFF_CFLAGS)

interleave_SOURCES = interleave.c
interleave_LDADD = ../src/libilbm/libilbm.la $(LIBIFF_LIBS)
interleave_CFLAGS = -I../src/libilbm $(LIBIFF_CFLAGS)

byterun_SOURCES = byterun.c
byterun_LDADD = ../src/libilbm/libilbm.la $(LIBIFF_LIBS)
byterun_CFLAGS = -I../src/libilbm $(LIBIFF_CFLAGS)

TESTS = writesimpleilbm writesimpleilbm-padded readsimpleilbm writesimplepbm readsimplepbm writesimpleacbm readsimpleacbm interleave-simple.sh interleave-simple-padded.sh byterun-simple.sh byterun-simple-padded.sh

EXTRA_DIST = interleave-simple.sh interleave-simple-padded.sh byterun-simple.sh byterun-simple-padded.sh
