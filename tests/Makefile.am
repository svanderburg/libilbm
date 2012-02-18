check_PROGRAMS = writesimpleilbm readsimpleilbm checkilbm

writesimpleilbm_SOURCES = writesimpleilbm.c
writesimpleilbm_LDADD = ../src/libilbm/libilbm.la $(LIBIFF_LIBS)
writesimpleilbm_CFLAGS = -I../src/libilbm $(LIBIFF_CFLAGS)

readsimpleilbm_SOURCES = readsimpleilbm.c
readsimpleilbm_LDADD = ../src/libilbm/libilbm.la $(LIBIFF_LIBS)
readsimpleilbm_CFLAGS = -I../src/libilbm $(LIBIFF_CFLAGS)

checkilbm_SOURCES = checkilbm.c
checkilbm_LDADD = ../src/libilbm/libilbm.la $(LIBIFF_LIBS)
checkilbm_CFLAGS = -I../src/libilbm $(LIBIFF_CFLAGS)

TESTS = writesimpleilbm readsimpleilbm check-missing-BMHD.sh
