AC_PREREQ(2.61)
AC_INIT([libilbm], m4_esyscmd([echo -n $(cat ./version)$VERSION_SUFFIX]))
AC_CONFIG_AUX_DIR([config])
AM_INIT_AUTOMAKE([1.7.2 -Wall -Werror -Wno-extra-portability])

# Checks for programs
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AM_PROG_CC_C_O
PKG_PROG_PKG_CONFIG
AC_PATH_PROG(HELP2MAN, help2man, false)

# Checks for headers
AC_CHECK_HEADER([getopt.h], [HAVE_GETOPT_H=1], [HAVE_GETOPT_H=0])
AC_SUBST(HAVE_GETOPT_H)

# Checks for libiff libraries
LIBIFF_REQUIRED=0.1
PKG_CHECK_MODULES(LIBIFF, libiff >= $LIBIFF_REQUIRED)
AC_SUBST(LIBIFF_CFLAGS)
AC_SUBST(LIBIFF_LIBS)

# Output
AC_CONFIG_FILES([
Makefile
doc/Makefile
src/libilbm.pc
src/Makefile
src/libilbm/Makefile
src/ilbmpack/Makefile
src/ilbmpp/Makefile
src/acbm2ilbm/Makefile
src/ilbm2acbm/Makefile
tests/Makefile
])
AC_OUTPUT
