check_PROGRAMS = writesimpleilbm readsimpleilbm checkilbm writesimplepbm readsimplepbm interleave-simple byterun-simple

writesimpleilbm_SOURCES = writesimpleilbm.c
writesimpleilbm_LDADD = ../src/libilbm/libilbm.la $(LIBIFF_LIBS)
writesimpleilbm_CFLAGS = -I../src/libilbm $(LIBIFF_CFLAGS)

readsimpleilbm_SOURCES = readsimpleilbm.c
readsimpleilbm_LDADD = ../src/libilbm/libilbm.la $(LIBIFF_LIBS)
readsimpleilbm_CFLAGS = -I../src/libilbm $(LIBIFF_CFLAGS)

checkilbm_SOURCES = checkilbm.c
checkilbm_LDADD = ../src/libilbm/libilbm.la $(LIBIFF_LIBS)
checkilbm_CFLAGS = -I../src/libilbm $(LIBIFF_CFLAGS)

writesimplepbm_SOURCES = writesimplepbm.c
writesimplepbm_LDADD = ../src/libilbm/libilbm.la $(LIBIFF_LIBS)
writesimplepbm_CFLAGS = -I../src/libilbm $(LIBIFF_CFLAGS)

readsimplepbm_SOURCES = readsimplepbm.c
readsimplepbm_LDADD = ../src/libilbm/libilbm.la $(LIBIFF_LIBS)
readsimplepbm_CFLAGS = -I../src/libilbm $(LIBIFF_CFLAGS)

interleave_simple_SOURCES = interleave-simple.c
interleave_simple_LDADD = ../src/libilbm/libilbm.la $(LIBIFF_LIBS)
interleave_simple_CFLAGS = -I../src/libilbm $(LIBIFF_CFLAGS)

byterun_simple_SOURCES = byterun-simple.c
byterun_simple_LDADD = ../src/libilbm/libilbm.la $(LIBIFF_LIBS)
byterun_simple_CFLAGS = -I../src/libilbm $(LIBIFF_CFLAGS)

TESTS = writesimpleilbm readsimpleilbm check-missing-BMHD.sh writesimplepbm readsimplepbm interleave-simple byterun-simple
