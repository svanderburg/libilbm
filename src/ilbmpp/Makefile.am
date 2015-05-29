ilbmpp.1: main.c
	$(HELP2MAN) --output=$@ --no-info --name ' Displays a textual representation of an IFF/ILBM file on the standard output' --libtool ./ilbmpp

AM_CPPFLAGS = -DHAVE_GETOPT_H=$(HAVE_GETOPT_H)

bin_PROGRAMS = ilbmpp
noinst_HEADERS = pp.h
man1_MANS = ilbmpp.1

ilbmpp_SOURCES = main.c pp.c
ilbmpp_LDADD = ../libilbm/libilbm.la
ilbmpp_CFLAGS = -I../libilbm $(LIBIFF_CFLAGS)

EXTRA_DIST = ilbmpp.1
