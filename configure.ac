#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(libtimeUtils, 20190321)
AC_CONFIG_SRCDIR([src/timeUtils.h])
AM_INIT_AUTOMAKE
AC_PROG_RANLIB

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
AX_BOOST_BASE([1.48],, [AC_MSG_ERROR([This program needs Boost, but it was not found in your system])])
AX_BOOST_DATE_TIME

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 src/Makefile]
                 tests/Makefile)
AC_OUTPUT
