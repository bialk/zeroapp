dnl Process this file with autoconf to produce a configure script.
AC_INIT([matpro library], [2.2.0],
        [Alexander Blinov <pubbialk@list.ru>],
        [libmatpro.la])
AM_INIT_AUTOMAKE

dnl Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_LN_S
dnl AC_PROG_YACC

AC_PROG_INSTALL

dnl AC_PROG_RANLIB

AC_PROG_LIBTOOL

dnl togle between debug or release switch
dnl CXXFLAGS="-w -O5"
dnl CXXFLAGS="-w -g"
dnl or use propriate command string 
dnl ./configure CXXFLAGS="..."

dnl Checks for libraries.

dnl save_LDFLAGS="$LDFLAGS"
dnl LDFLAGS="$LDFLAGS -L/usr/X11R6/lib64 -L/usr/X11R6/lib -L$HOME/lib -lstdc++"
dnl AC_CHECK_LIB(lapack, main, ,[echo "Im afraid we need lapack library:(."; exit 1])
dnl LDFLAGS="$save_LDFLAGS"

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
