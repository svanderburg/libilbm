AM_CPPFLAGS = -DHAVE_GETOPT_H=$(HAVE_GETOPT_H)

bin_PROGRAMS = acbm2ilbm
noinst_HEADERS = acbm2ilbm.h

acbm2ilbm_SOURCES = main.c acbm2ilbm.c
acbm2ilbm_LDADD = ../libilbm/libilbm.la
acbm2ilbm_CFLAGS = -I../libilbm $(LIBIFF_CFLAGS)
