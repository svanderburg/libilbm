ilbmpack.1: main.c
	$(HELP2MAN) --output=$@ --no-info --name 'Compress or decompress ILBM images inside an IFF file' --libtool ./ilbmpack

AM_CPPFLAGS = -DHAVE_GETOPT_H=$(HAVE_GETOPT_H)

bin_PROGRAMS = ilbmpack
noinst_HEADERS = pack.h
man1_MANS = ilbmpack.1

ilbmpack_SOURCES = main.c pack.c
ilbmpack_LDADD = ../libilbm/libilbm.la
ilbmpack_CFLAGS = -I../libilbm $(LIBIFF_CFLAGS)

EXTRA_DIST = ilbmpack.1
