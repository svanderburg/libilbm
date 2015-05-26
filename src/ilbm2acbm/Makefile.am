ilbm2acbm.1:
	$(HELP2MAN) --output=$@ --no-info --name 'Convert all ILBM images inside an IFF file to ACBM images' --libtool ./ilbm2acbm

AM_CPPFLAGS = -DHAVE_GETOPT_H=$(HAVE_GETOPT_H)

bin_PROGRAMS = ilbm2acbm
noinst_HEADERS = ilbm2acbm.h

ilbm2acbm_SOURCES = main.c ilbm2acbm.c
ilbm2acbm_LDADD = ../libilbm/libilbm.la
ilbm2acbm_CFLAGS = -I../libilbm $(LIBIFF_CFLAGS)

man1_MANS = ilbm2acbm.1
EXTRA_DIST = ilbm2acbm.1
