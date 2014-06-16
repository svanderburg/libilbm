AM_CPPFLAGS = -DHAVE_GETOPT_H=$(HAVE_GETOPT_H)

bin_PROGRAMS = ilbm2acbm
noinst_HEADERS = ilbm2acbm.h

ilbm2acbm_SOURCES = main.c ilbm2acbm.c
ilbm2acbm_LDADD = ../libilbm/libilbm.la
ilbm2acbm_CFLAGS = -I../libilbm $(LIBIFF_CFLAGS)
