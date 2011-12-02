dnl Process this file with autoconf to produce a configure script.
dnl AC_INIT (package, version, [bug-report], [tarname])
AC_PREREQ([2.65])
AC_INIT([zeroapp], [0.1.0],
        [Alexander Blinov <pubbialk@list.ru>],
        [zeroapp])
AM_INIT_AUTOMAKE

AC_CONFIG_SRCDIR([src/mainini.cpp])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CPP
AC_PROG_INSTALL

# Extract host information.
AC_CANONICAL_HOST
AC_MSG_CHECKING([checking type of OS for specific settings])
AC_MSG_RESULT([$host_os])
AM_CONDITIONAL(MINGW32, [test x$host_os = x"mingw32"])


# we also looking in home directory
LDFLAGS="-L$HOME/lib $LDFLAGS"

AC_CHECK_LIB(m, main, ,[echo "Im afraid we need m library:(."; exit 1])
AC_CHECK_LIB(lapackcpp, main, ,[echo "Im afraid we need lapackcpp library. :("; exit 1] )
AC_CHECK_LIB(freeimage, main, ,[echo "Im afraid we need freeimage library. :("; exit 1] )
AC_CHECK_LIB(spooles_I32, main, ,[echo "Im afraid we need spooles_I32 library. :("; exit 1],[-lm])
AC_CHECK_LIB(fltk, main, ,[echo "Im afraid we need fltk library:(."; exit 1])
AC_CHECK_LIB(fltk_gl, main, ,[echo "Im afraid we need fltk_gl library:(."; exit 1])
AC_CHECK_LIB(fltk_images, main, ,[echo "Im afraid we need fltk_images library:(."; exit 1])
AC_CHECK_LIB(fltk_forms, main, ,[echo "Im afraid we need fltk_forms library:(."; exit 1])
AC_CHECK_LIB(jpeg,  jpeg_start_decompress, ,[echo "Im afraid we need jpeg library:(."; exit 1])
AC_CHECK_LIB(pthread,  main, ,[echo "Im afraid we need pthread library:(."; exit 1])
AC_CHECK_LIB(sigc-2.0, main, ,[echo "Im afraid we need sigc-2.0 library:(."; exit 1])
dnl PKG_CHECK_MODULES does now work on mingw (macros is not defined)
dnl PKG_CHECK_MODULES(SIGC,sigc++-2.0 >= 2.2.4)


if test x${host_os} = x"mingw32"; then

AC_CHECK_LIB(flu, main, ,[echo "Im afraid we need flu library:(."; exit 1],[-lopengl32])

else

AC_CHECK_LIB(GL, main, ,[echo "Im afraid we need GL library:(."; exit 1])
AC_CHECK_LIB(GLU, main, ,[echo "Im afraid we need GLU library:(."; exit 1])
AC_CHECK_LIB(Xmu, main, ,[echo "Im afraid we need Xmu library:(."; exit 1])
AC_CHECK_LIB(png, main, ,[echo "Im afraid we need png library:(."; exit 1])
AC_CHECK_LIB(flu, main, ,[echo "Im afraid we need flu library:(."; exit 1],[-lGL])

fi



dnl AC_CHECK_LIB(glui, main, ,[echo "Im afraid we need glui library:(."; exit 1],[-lglut])
dnl AC_CHECK_LIB(glut, main, ,[echo "Im afraid we need glut library:(."; exit 1],[-lGL -lGLU])
dnl LDFLAGS="$save_LDFLAGS"


AC_CONFIG_FILES([Makefile
		 src/Makefile])

dnl AC_CONFIG_SUBDIRS([mathlib/lapackcpp
dnl                    mathlib/lineqsolver
dnl                   mathlib/matpro])

AC_OUTPUT
