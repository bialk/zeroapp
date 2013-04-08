dnl Process this file with autoconf to produce a configure script.
AC_INIT([cpp interface to LAPACK], [2.2.0],
        [Alexander Blinov <pubbialk@list.ru>],
        [liblapackcpp.la])
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

save_LDFLAGS="$LDFLAGS"
LDFLAGS="$LDFLAGS -L/usr/X11R6/lib64 -L/usr/X11R6/lib -L$HOME/lib -lstdc++"
AC_CHECK_LIB(g2c, main, ,[echo "Im afraid we need g2c library:(."; exit 1])
AC_CHECK_LIB(blas, main, ,[echo "Im afraid we need blas library:(."; exit 1],[-lg2c])
AC_CHECK_LIB(lapack, main, ,[echo "Im afraid we need lapack library:(."; exit 1],[-lg2c -lblas])

dnl AC_CHECK_LIB(m, main, ,[echo "Im afraid we need m library:(."; exit 1])
dnl AC_CHECK_LIB(matpro, main, ,[echo "Im afraid we need matpro library:(."; exit 1])
dnl AC_CHECK_LIB(data3d, main, ,[echo "Im afraid we need data3d library. :("; exit 1] )
dnl AC_CHECK_LIB(lapackcpp, main, ,[echo "Im afraid we need data3d library. :("; exit 1] )
dnl AC_CHECK_LIB(GL, main, ,[echo "Im afraid we need GL library:(."; exit 1])
dnl AC_CHECK_LIB(GLU, main, ,[echo "Im afraid we need GLU library:(."; exit 1])
dnl AC_CHECK_LIB(jpeg, main, ,[echo "Im afraid we need jpeg library:(."; exit 1])
dnl AC_CHECK_LIB(Xmu, main, ,[echo "Im afraid we need Xmu library:(."; exit 1])
dnl AC_CHECK_LIB(glui, main, ,[echo "Im afraid we need glui library:(."; exit 1])
dnl AC_CHECK_LIB(glut, main, ,[echo "Im afraid we need glut library:(."; exit 1])
LDFLAGS="$save_LDFLAGS"

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
