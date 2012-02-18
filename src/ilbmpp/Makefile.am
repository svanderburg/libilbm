AM_CPPFLAGS = -DHAVE_GETOPT_H=$(HAVE_GETOPT_H)

bin_PROGRAMS = ilbmpp
noinst_HEADERS = pp.h

ilbmpp_SOURCES = main.c pp.c
ilbmpp_LDADD = ../libilbm/libilbm.la
ilbmpp_CFLAGS = -I../libilbm $(LIBIFF_CFLAGS)
