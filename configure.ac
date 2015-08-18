#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.63)
AC_INIT([SystemCASS],[1.0.0])
AC_CANONICAL_TARGET

AC_CONFIG_HEADER([config.h])

AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

AC_CHECK_PROG(has_latex, latex, yes)
AC_CHECK_PROG(has_bibtex, bibtex, yes)
AC_CHECK_PROG(has_fig2dev, fig2dev, yes)
AC_CHECK_PROG(has_ps2pdf, ps2pdf, yes)
AC_CHECK_PROG(has_dvips, dvips, yes)


# User choices
AC_DEFUN([MY_ARG_ENABLE],
            [AC_ARG_ENABLE(m4_translit([[$1]], [_], [-]),
               [AS_HELP_STRING([--enable-m4_translit([$1], [_], [-])],
                               [$2 (default is $3)])],
						   [do_$1=${enableval}],
						   [do_$1=$3])
             AS_IF([test x${do_$1} != "xno"],
			 AC_DEFINE(m4_toupper([CONFIG_$1]), [], [$2]),
			 )])

MY_ARG_ENABLE(debug, [Enable debugging], [no])

MY_ARG_ENABLE(default_runtime_compilation, [Compile scheduling code], [no])

# Also set -DNDEBUG when not debugging, this disables assert()s
AS_IF([test "x$do_debug"  = "xyes"], [CXXFLAGS="-g"         ],
      [test "x$do_debug" != "xyes"], [CXXFLAGS="-O2 -DNDEBUG"])

case $target_os in
	 linux*)
		AC_DEFINE(CONFIG_OS_LINUX,1,We have a linux system)
		AC_SUBST(SC_LIB_TARGET,linux)
		;;
	 darwin*)
		AC_DEFINE(CONFIG_OS_DARWIN,1,We have a MacOS system)
		AC_SUBST(SC_LIB_TARGET,macosx)
		;;
	 *)
		AC_SUBST(SC_LIB_TARGET,unknown)
		;;
esac

AC_ARG_WITH([pat],
            [AS_HELP_STRING([--with-pat=PATH],
              [Use PAT trace format, this needs Alliance in PATH])],
            [],
            [with_pat=no])

# Checks for libraries.
AS_IF([test "x$with_pat" != "xno"],
	  ALLIANCE_CFLAGS="-I$with_pat/include"
	  [AC_CHECK_HEADER([pat.h],,
		   [AC_MSG_ERROR([You asked for PAT trace format but no pat.h dnl
can be found. Try --with-pat=/search/dir/])])
      AC_SEARCH_LIBS([pat_message], [Pat],
		   AC_DEFINE(CONFIG_PAT_TRACE_FORMAT,1,Use PAT trace format),
		   AC_MSG_ERROR([You asked for PAT trace format but no libpat dnl
can be found. Try --with-pat=/search/dir/]),
		   [-lMut -lPpt -lPgn])]
	  AC_SUBST(ALLIANCE_PATH, $withval)
	  AC_SUBST(ALLIANCE_CFLAGS)
)

AM_CONDITIONAL(WITH_ALLIANCE, test x$with_pat != xno)

AC_ARG_WITH([soclib],
            [AS_HELP_STRING([--with-soclib=PATH],
              [Use SoCLib in PATH to compile examples])],
            [AC_SUBST(SOCLIB_PATH, $withval)],
			[with_soclib=no])
AM_CONDITIONAL(HAS_SOCLIB, test x$with_soclib != xno)

# OpenMP
AC_OPENMP([C])
AC_CHECK_LIB(dl, dlopen, , )
AC_CHECK_LIB(gomp, omp_get_thread_num, , )

AM_CONDITIONAL(BUILD_DOCS,
[test x$has_latex$has_bibtex$has_fig2dev$has_ps2pdf$has_dvips = xyesyesyesyesyes])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdint.h stdlib.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_INT32_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_STRUCT_TM
AC_TYPE_UINT64_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_STRFTIME
AC_CHECK_FUNCS([memset strchr strdup strstr])


AC_CONFIG_FILES([
  Makefile
  src/Makefile
  doc/Makefile
])
AC_OUTPUT

