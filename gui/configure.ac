dnl Process this file with autoconf to produce a configure script.
dnl AC_INIT (package, version, [bug-report], [tarname])
AC_INIT([zeroapp], [0.1.0],
        [Alexander Blinov <pubbialk@list.ru>],
        [zeroapp])
AM_INIT_AUTOMAKE

dnl Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_LN_S
dnl AC_PROG_YACC

AC_PROG_INSTALL

dnl AC_PROG_RANLIB

# Extract host information.
AC_CANONICAL_HOST
AC_MSG_CHECKING([checking type of OS for specific settings])
AC_MSG_RESULT([$host_os])
dnl case "$host_os" in
dnl *linux*)
dnl 	win32=true
dnl         AC_MSG_RESULT([yes])
dnl  	AC_CHECK_TOOL(WINDRES, windres)
dnl  	;;
dnl  *)
dnl          win32=false
dnl          AC_MSG_RESULT([no])
dnl          ;;
dnl esac
dnl AM_CONDITIONAL(WIN32, test $win32 = true)
AM_CONDITIONAL(WIN32, [test $host_os = "mingw32"])

dnl AC_PROG_LIBTOOL

dnl togle between debug or release switch
dnl CXXFLAGS="-w -O5"
dnl CXXFLAGS="-w -g"
dnl or use propriate command string 
dnl ./configure CXXFLAGS="..."

dnl Checks for libraries.
save_LDFLAGS="$LDFLAGS"
LDFLAGS="$LDFLAGS -L/usr/X11R6/lib64 -L/usr/X11R6/lib -L$HOME/lib -lstdc++"
dnl AC_CHECK_LIB(matpro, main, ,[echo "Im afraid we need matpro library:(."; exit 1])
dnl AC_CHECK_LIB(sigc-2.0, main, ,[echo "Im afraid we need sigc-2.0 library:(."; exit 1])
PKG_CHECK_MODULES(SIGC,sigc++-2.0 >= 2.2.4)
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


if test -z ${WIN32_TRUE}; then

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
LDFLAGS="$save_LDFLAGS"


AC_CONFIG_FILES([Makefile])
AC_OUTPUT
