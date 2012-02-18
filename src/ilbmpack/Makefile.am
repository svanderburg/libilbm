AM_CPPFLAGS = -DHAVE_GETOPT_H=$(HAVE_GETOPT_H)

bin_PROGRAMS = ilbmpack
noinst_HEADERS = pack.h

ilbmpack_SOURCES = main.c pack.c
ilbmpack_LDADD = ../libilbm/libilbm.la
ilbmpack_CFLAGS = -I../libilbm $(LIBIFF_CFLAGS)
