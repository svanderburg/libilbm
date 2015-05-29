acbm2ilbm.1: main.c
	$(HELP2MAN) --output=$@ --no-info --name 'Convert all ACBM images inside an IFF file to ILBM images' --libtool ./acbm2ilbm

AM_CPPFLAGS = -DHAVE_GETOPT_H=$(HAVE_GETOPT_H)

bin_PROGRAMS = acbm2ilbm
noinst_HEADERS = acbm2ilbm.h
man1_MANS = acbm2ilbm.1

acbm2ilbm_SOURCES = main.c acbm2ilbm.c
acbm2ilbm_LDADD = ../libilbm/libilbm.la
acbm2ilbm_CFLAGS = -I../libilbm $(LIBIFF_CFLAGS)

EXTRA_DIST = acbm2ilbm.1
