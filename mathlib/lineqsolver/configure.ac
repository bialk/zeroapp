dnl Process this file with autoconf to produce a configure script.
AC_INIT([Sparce linear system solver], [0.01],
        [Alexander Blinov <pubbialk@list.ru>],
        [solv_lineq])
AM_INIT_AUTOMAKE

dnl Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_LN_S
dnl AC_PROG_YACC

AC_PROG_INSTALL



# Extract host information.
AC_CANONICAL_HOST
AC_MSG_CHECKING([checking for type of os for specific settings])
AC_MSG_RESULT([$host_os])
dnl case "$host_os" in
dnl *linux*)
dnl     win32=true
dnl         AC_MSG_RESULT([yes])
dnl     AC_CHECK_TOOL(WINDRES, windres)
dnl     ;;
dnl  *)
dnl          win32=false
dnl          AC_MSG_RESULT([no])
dnl          ;;
dnl esac
dnl AM_CONDITIONAL(WIN32, test $win32 = true)
AM_CONDITIONAL(WIN32, [test $host_os = "mingw32"])





dnl togle between debug or release switch
dnl CXXFLAGS="-w -O5"
dnl CXXFLAGS="-w -g"
dnl or use appropriate command string 
dnl ./configure CXXFLAGS="..."

dnl Checks for libraries.

save_LDFLAGS="$LDFLAGS"
LDFLAGS="$LDFLAGS -L$HOME/lib -lstdc++ -m32"
dnl AC_CHECK_LIB(stdc++, main, ,[echo "Im afraid we need m library:(."; exit 1])
dnl AC_CHECK_LIB(matpro, main, ,[echo "Im afraid we need matpro library:(."; exit 1])
dnl AC_CHECK_LIB(data3d, main, ,[echo "Im afraid we need data3d library. :("; exit 1] )
dnl AC_CHECK_LIB(lapackcpp, main, ,[echo "Im afraid we need data3d library. :("; exit 1] )
AC_CHECK_LIB(spooles_I32, main, ,[echo "Im afraid we need spooles_I32 library. :("; exit 1],[-lm])
LDFLAGS="$save_LDFLAGS"

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
