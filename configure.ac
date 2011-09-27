dnl Process this file with autoconf to produce a configure script.
dnl This is useful to force using autoreconf 2.5.x
dnl
dnl        Templatizer
dnl 
dnl        Copyright (C) 2011 Ben Asselstine
dnl
dnl        This file is licensed under the terms of the GNU General Public 
dnl        License version 3, or (at your option) any later version.
dnl
dnl
AC_PREREQ(2.59)
AC_INIT([templatizer], [0.0.2], [bing at nym.hush.com], [templatizer])
AC_CONFIG_SRCDIR([config.h.in])
AM_CONFIG_HEADER(config.h)

AC_CANONICAL_HOST
AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE
    
dnl Checks for programs.
AC_PROG_CXX
AC_PROG_RANLIB

test "x$prefix" = xNONE && prefix=/usr

dnl Check for gtkmm

PKG_CHECK_MODULES(GTKMM, gtkmm-3.0)

AC_SUBST(GTKMM_CFLAGS)
AC_SUBST(GTKMM_LIBS)

PKG_CHECK_MODULES(LIBSIGC, sigc++-2.0)
AC_SUBST(LIBSIGC_CFLAGS)
AC_SUBST(LIBSIGC_LIBS)

PKG_CHECK_MODULES(GTKSOURCEVIEWMM, gtksourceviewmm-3.0)
AC_SUBST(GTKSOURCEVIEWMM_CFLAGS)
AC_SUBST(GTKSOURCEVIEWMM_LIBS)

PKG_CHECK_MODULES([SQLITE], sqlite3 >= 3.0.0)
AC_SUBST(SQLITE_LIBS)
AC_SUBST(SQLITE_CFLAGS)

eval TEMPLATIZER_DATADIR="$datadir/$PACKAGE_NAME"
AC_SUBST(TEMPLATIZER_DATADIR)

AC_CHECK_PROGS(RPMBUILD, rpmbuild, rpm)

AC_CONFIG_FILES([
        Makefile 
        glade/Makefile
        data/Makefile
        data/icons/Makefile
        data/icons/32x32/Makefile
        data/icons/64x64/Makefile
        data/icons/scalable/Makefile
        data/templatizer.desktop
        templatizer.spec
        src/Makefile])
AC_OUTPUT
