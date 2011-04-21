#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])
AC_INIT([XMonad Statusbar Applet],[0.1],[tehminkeh@gmail.com])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])

AC_DEFUN([AC_PACKAGE_COMPRESSEDNAME],
         m4_combine([],m4_split(AC_PACKAGE_NAME),[],[]))
AC_DEFINE([PACKAGE_COMPRESSEDNAME],"AC_PACKAGE_COMPRESSEDNAME",
          [Define to the full name of this package minus any spaces.])
AC_SUBST([PACKAGE_COMPRESSEDNAME],'AC_PACKAGE_COMPRESSEDNAME')

AC_PREFIX_DEFAULT([/usr])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
PKG_PROG_PKG_CONFIG

# Checks for libraries.
#TODO: check for actual version numbers
PKG_CHECK_MODULES([DEPS],[glib-2.0 gtk+-2.0 libpanelapplet-2.0])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 data/Makefile
                 data/scalable/Makefile
                 data/16x16/Makefile
                 data/22x22/Makefile
                 data/24x24/Makefile
                 data/32x32/Makefile
                 data/48x48/Makefile
                 data/XMonadStatusbarApplet.server])

AC_OUTPUT
