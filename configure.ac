AC_INIT([OpenCVS],[4.5.0],[Charles Longeau <chl@openbsd.org>])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CC
AC_PROG_YACC
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_PROG_LN_S

# special check for Solaris
# stolen from vlc's configure.ac
case "${host_os}" in
  solaris*)
    # _POSIX_PTHREAD_SEMANTICS is needed to get the POSIX ctime_r
    # Perhaps it is useful other places as well? 
    CFLAGS_save="${CFLAGS_save} -D_POSIX_PTHREAD_SEMANTICS"; CFLAGS="${CFLAGS_save}"
    ;;
esac

# taken from portable openssh configure.ac
AC_ARG_WITH(Werror,
	[  --with-Werror           Build main code with -Werror],
	[
		if test -n "$withval"  &&  test "x$withval" != "xno"; then
			werror_flags="-Werror"
			if test "x${withval}" != "xyes"; then
				werror_flags="$withval"
			fi
		fi
	]
)


AC_CHECK_HEADERS([ \
		  sys/dirent.h \
		  sys/time.h \
		  err.h \
		  dirent.h \
		  utimes.h \
		  ])
AC_CHECK_FUNCS([ \
		fgetln \
		getdents \
		getdirentries \
		getopt \
		strlcat \
		strlcpy \
		strtonum \
		tzset \
		])

# utimes comes from sudo's configure.in
AC_CHECK_FUNCS(utimes, [AC_CHECK_FUNCS(futimes futimesat, [break])], [AC_CHECK_FUNCS(futime) AC_LIBOBJ(utimes)])

# fnmatch check is inspirred from a check from aclocal.m4

dnl
dnl check for working fnmatch(3)
dnl

AC_CACHE_CHECK([for working fnmatch with FNM_CASEFOLD], ac_cv_func_fnmatch, [
	AC_TRY_RUN([#include <fnmatch.h>
main() { exit(fnmatch("/*/bin/echo *", "/usr/bin/echo just a test", FNM_CASEFOLD)); }],
       [ac_cv_func_fnmatch="yes"],
       [ac_cv_func_fnmatch="no"],
       [ac_cv_func_fnmatch="no"])
])
if test "x$ac_cv_func_fnmatch" = "xyes" ; then
	AC_DEFINE(HAVE_FNMATCH, 1, [define if you have working fnmatch with FNM_CASEFOLD])
	have_fnmatch=1
fi




# taken from portable openssh configure.ac

dnl Make sure prototypes are defined for these before using them.
AC_CHECK_DECL(strsep,
	[AC_CHECK_FUNCS(strsep)],
	[],
	[
#ifdef HAVE_STRING_H
# include <string.h>
#endif
	])


AC_CHECK_DECL(LLONG_MAX, have_llong_max=1, , [#include <limits.h>])

if test "$GCC" = "yes" || test "$GCC" = "egcs"; then
	CFLAGS="$CFLAGS -Wall -Wpointer-arith -Wuninitialized"
	GCC_VER=`$CC -v 2>&1 | $AWK '/gcc version /{print $3}'`
	case $GCC_VER in
		1.*) no_attrib_nonnull=1 ;;
		2.8* | 2.9*)
		     CFLAGS="$CFLAGS -Wsign-compare"
		     no_attrib_nonnull=1
		     ;;
		2.*) no_attrib_nonnull=1 ;;
		3.*) CFLAGS="$CFLAGS -Wsign-compare" ;;
		4.*) CFLAGS="$CFLAGS -Wsign-compare -Wno-pointer-sign" ;;
		*) ;;
	esac

	if test -z "$have_llong_max"; then
		# retry LLONG_MAX with -std=gnu99, needed on some Linuxes
		unset ac_cv_have_decl_LLONG_MAX
		saved_CFLAGS="$CFLAGS"
		CFLAGS="$CFLAGS -std=gnu99"
		AC_CHECK_DECL(LLONG_MAX,
		    [have_llong_max=1],
		    [CFLAGS="$saved_CFLAGS"],
		    [#include <limits.h>]
		)
	fi
fi

if test "x$no_attrib_nonnull" != "x1" ; then
	AC_DEFINE(HAVE_ATTRIBUTE__NONNULL__, 1, [Have attribute nonnull])
fi

# Check datatype sizes
AC_CHECK_SIZEOF(char, 1)
AC_CHECK_SIZEOF(short int, 2)
AC_CHECK_SIZEOF(int, 4)
AC_CHECK_SIZEOF(long int, 4)
AC_CHECK_SIZEOF(long long int, 8)

# Sanity check long long for some platforms (AIX)
if test "x$ac_cv_sizeof_long_long_int" = "x4" ; then
	ac_cv_sizeof_long_long_int=0
fi

# compute LLONG_MIN and LLONG_MAX if we don't know them.
if test -z "$have_llong_max"; then
	AC_MSG_CHECKING([for max value of long long])
	AC_RUN_IFELSE(
		[AC_LANG_SOURCE([[
#include <stdio.h>
/* Why is this so damn hard? */
#ifdef __GNUC__
# undef __GNUC__
#endif
#define __USE_ISOC99
#include <limits.h>
#define DATA "conftest.llminmax"
#define my_abs(a) ((a) < 0 ? ((a) * -1) : (a))

/*
 * printf in libc on some platforms (eg old Tru64) does not understand %lld so
 * we do this the hard way.
 */
static int
fprint_ll(FILE *f, long long n)
{
	unsigned int i;
	int l[sizeof(long long) * 8];

	if (n < 0)
		if (fprintf(f, "-") < 0)
			return -1;
	for (i = 0; n != 0; i++) {
		l[i] = my_abs(n % 10);
		n /= 10;
	}
	do {
		if (fprintf(f, "%d", l[--i]) < 0)
			return -1;
	} while (i != 0);
	if (fprintf(f, " ") < 0)
		return -1;
	return 0;
}

int main(void) {
	FILE *f;
	long long i, llmin, llmax = 0;

	if((f = fopen(DATA,"w")) == NULL)
		exit(1);

#if defined(LLONG_MIN) && defined(LLONG_MAX)
	fprintf(stderr, "Using system header for LLONG_MIN and LLONG_MAX\n");
	llmin = LLONG_MIN;
	llmax = LLONG_MAX;
#else
	fprintf(stderr, "Calculating  LLONG_MIN and LLONG_MAX\n");
	/* This will work on one's complement and two's complement */
	for (i = 1; i > llmax; i <<= 1, i++)
		llmax = i;
	llmin = llmax + 1LL;	/* wrap */
#endif

	/* Sanity check */
	if (llmin + 1 < llmin || llmin - 1 < llmin || llmax + 1 > llmax
	    || llmax - 1 > llmax || llmin == llmax || llmin == 0
	    || llmax == 0 || llmax < LONG_MAX || llmin > LONG_MIN) {
		fprintf(f, "unknown unknown\n");
		exit(2);
	}

	if (fprint_ll(f, llmin) < 0)
		exit(3);
	if (fprint_ll(f, llmax) < 0)
		exit(4);
	if (fclose(f) < 0)
		exit(5);
	exit(0);
}
		]])],
		[
			llong_min=`$AWK '{print $1}' conftest.llminmax`
			llong_max=`$AWK '{print $2}' conftest.llminmax`

			AC_MSG_RESULT($llong_max)
			AC_DEFINE_UNQUOTED(LLONG_MAX, [${llong_max}LL],
			    [max value of long long calculated by configure])
			AC_MSG_CHECKING([for min value of long long])
			AC_MSG_RESULT($llong_min)
			AC_DEFINE_UNQUOTED(LLONG_MIN, [${llong_min}LL],
			    [min value of long long calculated by configure])
		],
		[
			AC_MSG_RESULT(not found)
		],
		[
			AC_MSG_WARN([cross compiling: not checking])
		]
	)
fi

# More checks for data types
AC_CACHE_CHECK([for u_int type], ac_cv_have_u_int, [
	AC_TRY_COMPILE(
		[ #include <sys/types.h> ],
		[ u_int a; a = 1;],
		[ ac_cv_have_u_int="yes" ],
		[ ac_cv_have_u_int="no" ]
	)
])
if test "x$ac_cv_have_u_int" = "xyes" ; then
	AC_DEFINE(HAVE_U_INT, 1, [define if you have u_int data type])
	have_u_int=1
fi

AC_CACHE_CHECK([for intXX_t types], ac_cv_have_intxx_t, [
	AC_TRY_COMPILE(
		[ #include <sys/types.h> ],
		[ int8_t a; int16_t b; int32_t c; a = b = c = 1;],
		[ ac_cv_have_intxx_t="yes" ],
		[ ac_cv_have_intxx_t="no" ]
	)
])
if test "x$ac_cv_have_intxx_t" = "xyes" ; then
	AC_DEFINE(HAVE_INTXX_T, 1, [define if you have intxx_t data type])
	have_intxx_t=1
fi

if (test -z "$have_intxx_t" && \
	   test "x$ac_cv_header_stdint_h" = "xyes")
then
    AC_MSG_CHECKING([for intXX_t types in stdint.h])
	AC_TRY_COMPILE(
		[ #include <stdint.h> ],
		[ int8_t a; int16_t b; int32_t c; a = b = c = 1;],
		[
			AC_DEFINE(HAVE_INTXX_T)
			AC_MSG_RESULT(yes)
		],
		[ AC_MSG_RESULT(no) ]
	)
fi

AC_CACHE_CHECK([for int64_t type], ac_cv_have_int64_t, [
	AC_TRY_COMPILE(
		[
#include <sys/types.h>
#ifdef HAVE_STDINT_H
# include <stdint.h>
#endif
#include <sys/socket.h>
#ifdef HAVE_SYS_BITYPES_H
# include <sys/bitypes.h>
#endif
		],
		[ int64_t a; a = 1;],
		[ ac_cv_have_int64_t="yes" ],
		[ ac_cv_have_int64_t="no" ]
	)
])
if test "x$ac_cv_have_int64_t" = "xyes" ; then
	AC_DEFINE(HAVE_INT64_T, 1, [define if you have int64_t data type])
fi

AC_CACHE_CHECK([for u_intXX_t types], ac_cv_have_u_intxx_t, [
	AC_TRY_COMPILE(
		[ #include <sys/types.h> ],
		[ u_int8_t a; u_int16_t b; u_int32_t c; a = b = c = 1;],
		[ ac_cv_have_u_intxx_t="yes" ],
		[ ac_cv_have_u_intxx_t="no" ]
	)
])
if test "x$ac_cv_have_u_intxx_t" = "xyes" ; then
	AC_DEFINE(HAVE_U_INTXX_T, 1, [define if you have u_intxx_t data type])
	have_u_intxx_t=1
fi

if test -z "$have_u_intxx_t" ; then
    AC_MSG_CHECKING([for u_intXX_t types in sys/socket.h])
	AC_TRY_COMPILE(
		[ #include <sys/socket.h> ],
		[ u_int8_t a; u_int16_t b; u_int32_t c; a = b = c = 1;],
		[
			AC_DEFINE(HAVE_U_INTXX_T)
			AC_MSG_RESULT(yes)
		],
		[ AC_MSG_RESULT(no) ]
	)
fi

AC_CACHE_CHECK([for u_int64_t types], ac_cv_have_u_int64_t, [
	AC_TRY_COMPILE(
		[ #include <sys/types.h> ],
		[ u_int64_t a; a = 1;],
		[ ac_cv_have_u_int64_t="yes" ],
		[ ac_cv_have_u_int64_t="no" ]
	)
])
if test "x$ac_cv_have_u_int64_t" = "xyes" ; then
	AC_DEFINE(HAVE_U_INT64_T, 1, [define if you have u_int64_t data type])
	have_u_int64_t=1
fi

if test -z "$have_u_int64_t" ; then
    AC_MSG_CHECKING([for u_int64_t type in sys/bitypes.h])
	AC_TRY_COMPILE(
		[ #include <sys/bitypes.h> ],
		[ u_int64_t a; a = 1],
		[
			AC_DEFINE(HAVE_U_INT64_T)
			AC_MSG_RESULT(yes)
		],
		[ AC_MSG_RESULT(no) ]
	)
fi

if test -z "$have_u_intxx_t" ; then
	AC_CACHE_CHECK([for uintXX_t types], ac_cv_have_uintxx_t, [
		AC_TRY_COMPILE(
			[
#include <sys/types.h>
			],
			[ uint8_t a; uint16_t b; uint32_t c; a = b = c = 1; ],
			[ ac_cv_have_uintxx_t="yes" ],
			[ ac_cv_have_uintxx_t="no" ]
		)
	])
	if test "x$ac_cv_have_uintxx_t" = "xyes" ; then
		AC_DEFINE(HAVE_UINTXX_T, 1,
			[define if you have uintxx_t data type])
	fi
fi

if test -z "$have_uintxx_t" ; then
    AC_MSG_CHECKING([for uintXX_t types in stdint.h])
	AC_TRY_COMPILE(
		[ #include <stdint.h> ],
		[ uint8_t a; uint16_t b; uint32_t c; a = b = c = 1;],
		[
			AC_DEFINE(HAVE_UINTXX_T)
			AC_MSG_RESULT(yes)
		],
		[ AC_MSG_RESULT(no) ]
	)
fi

if (test -z "$have_u_intxx_t" || test -z "$have_intxx_t" && \
	   test "x$ac_cv_header_sys_bitypes_h" = "xyes")
then
	AC_MSG_CHECKING([for intXX_t and u_intXX_t types in sys/bitypes.h])
	AC_TRY_COMPILE(
		[
#include <sys/bitypes.h>
		],
		[
			int8_t a; int16_t b; int32_t c;
			u_int8_t e; u_int16_t f; u_int32_t g;
			a = b = c = e = f = g = 1;
		],
		[
			AC_DEFINE(HAVE_U_INTXX_T)
			AC_DEFINE(HAVE_INTXX_T)
			AC_MSG_RESULT(yes)
		],
		[AC_MSG_RESULT(no)]
	)
fi


AC_CACHE_CHECK([for u_char], ac_cv_have_u_char, [
	AC_TRY_COMPILE(
		[
#include <sys/types.h>
		],
		[ u_char foo; foo = 125; ],
		[ ac_cv_have_u_char="yes" ],
		[ ac_cv_have_u_char="no" ]
	)
])
if test "x$ac_cv_have_u_char" = "xyes" ; then
	AC_DEFINE(HAVE_U_CHAR, 1, [define if you have u_char data type])
fi

dnl IRIX and Solaris 2.5.1 have dirname() in libgen
AC_CHECK_FUNCS(dirname, [AC_CHECK_HEADERS(libgen.h)] ,[
	AC_CHECK_LIB(gen, dirname,[
		AC_CACHE_CHECK([for broken dirname],
			ac_cv_have_broken_dirname, [
			save_LIBS="$LIBS"
			LIBS="$LIBS -lgen"
			AC_RUN_IFELSE(
				[AC_LANG_SOURCE([[
#include <libgen.h>
#include <string.h>

int main(int argc, char **argv) {
    char *s, buf[32];

    strncpy(buf,"/etc", 32);
    s = dirname(buf);
    if (!s || strncmp(s, "/", 32) != 0) {
	exit(1);
    } else {
	exit(0);
    }
}
				]])],
				[ ac_cv_have_broken_dirname="no" ],
				[ ac_cv_have_broken_dirname="yes" ],
				[ ac_cv_have_broken_dirname="no" ],
			)
			LIBS="$save_LIBS"
		])
		if test "x$ac_cv_have_broken_dirname" = "xno" ; then
			LIBS="$LIBS -lgen"
			AC_DEFINE(HAVE_DIRNAME)
			AC_CHECK_HEADERS(libgen.h)
		fi
	])
])


AC_CACHE_CHECK([for size_t], ac_cv_have_size_t, [
	AC_TRY_COMPILE(
		[
#include <sys/types.h>
		],
		[ size_t foo; foo = 1235; ],
		[ ac_cv_have_size_t="yes" ],
		[ ac_cv_have_size_t="no" ]
	)
])
if test "x$ac_cv_have_size_t" = "xyes" ; then
	AC_DEFINE(HAVE_SIZE_T, 1, [define if you have size_t data type])
fi

AC_CACHE_CHECK([for ssize_t], ac_cv_have_ssize_t, [
	AC_TRY_COMPILE(
		[
#include <sys/types.h>
		],
		[ ssize_t foo; foo = 1235; ],
		[ ac_cv_have_ssize_t="yes" ],
		[ ac_cv_have_ssize_t="no" ]
	)
])
if test "x$ac_cv_have_ssize_t" = "xyes" ; then
	AC_DEFINE(HAVE_SSIZE_T, 1, [define if you have ssize_t data type])
fi

AC_CACHE_CHECK([for sa_family_t], ac_cv_have_sa_family_t, [
	AC_TRY_COMPILE(
		[
#include <sys/types.h>
#include <sys/socket.h>
		],
		[ sa_family_t foo; foo = 1235; ],
		[ ac_cv_have_sa_family_t="yes" ],
		[ AC_TRY_COMPILE(
		  [
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
		],
		[ sa_family_t foo; foo = 1235; ],
		[ ac_cv_have_sa_family_t="yes" ],

		[ ac_cv_have_sa_family_t="no" ]
	)]
	)
])
if test "x$ac_cv_have_sa_family_t" = "xyes" ; then
	AC_DEFINE(HAVE_SA_FAMILY_T, 1,
		[define if you have sa_family_t data type])
fi

AC_CACHE_CHECK([for pid_t], ac_cv_have_pid_t, [
	AC_TRY_COMPILE(
		[
#include <sys/types.h>
		],
		[ pid_t foo; foo = 1235; ],
		[ ac_cv_have_pid_t="yes" ],
		[ ac_cv_have_pid_t="no" ]
	)
])
if test "x$ac_cv_have_pid_t" = "xyes" ; then
	AC_DEFINE(HAVE_PID_T, 1, [define if you have pid_t data type])
fi

AC_CACHE_CHECK([for mode_t], ac_cv_have_mode_t, [
	AC_TRY_COMPILE(
		[
#include <sys/types.h>
		],
		[ mode_t foo; foo = 1235; ],
		[ ac_cv_have_mode_t="yes" ],
		[ ac_cv_have_mode_t="no" ]
	)
])
if test "x$ac_cv_have_mode_t" = "xyes" ; then
	AC_DEFINE(HAVE_MODE_T, 1, [define if you have mode_t data type])
fi


AC_SEARCH_LIBS(basename, gen, AC_DEFINE(HAVE_BASENAME, 1,
	[Define if you have the basename function.]))

AC_CACHE_CHECK([whether getopt has optreset support],
		ac_cv_have_getopt_optreset, [
	AC_TRY_LINK(
		[
#include <getopt.h>
		],
		[ extern int optreset; optreset = 0; ],
		[ ac_cv_have_getopt_optreset="yes" ],
		[ ac_cv_have_getopt_optreset="no" ]
	)
])
if test "x$ac_cv_have_getopt_optreset" = "xyes" ; then
	AC_DEFINE(HAVE_GETOPT_OPTRESET, 1,
		[Define if your getopt(3) defines and uses optreset])
fi

# end of portable openssh configure.ac inclusions

# d_fileno and d_ test taken from apr

dnl
dnl APR_CHECK_DIRENT_FILENO
dnl
dnl  Decide if d_fileno or d_ino are available in the dirent
dnl  structure on this platform.  Single UNIX Spec says d_ino,
dnl  BSD uses d_fileno.  Undef to find the real beast.
dnl
AC_CACHE_CHECK([for dirent.d_fileno or .d_ino],
	ac_cv_dirent_inode,
	AC_TRY_COMPILE([
		#include <dirent.h>
		#ifdef d_ino
		#undef d_ino
		#endif
		],
		[ struct dirent de; de.d_fileno = 1; ],
		[ ac_cv_dirent_inode="d_fileno" ],
		[ ac_cv_dirent_inode="none" ]
	)
)

if test "x$ac_cv_dirent_inode" = "xd_fileno"; then
	AC_DEFINE(HAVE_DIRENT_FILENO, "d_fileno", [Define if dirent has d_fileno member])
else
	AC_TRY_COMPILE([
		#include <dirent.h>
		#ifdef d_fileno
		#undef d_fileno
		#endif
		],
	       	[ struct dirent de; de.d_ino = 1; ],
	       	[ ac_cv_dirent_inode="d_ino"],
		[ ac_cv_dirent_inode="none" ]
	)

	if test "$ac_cv_dirent_inode" = "d_ino"; then
		AC_DEFINE(HAVE_DIRENT_INO, "d_ino", [Define if dirent has d_ino member])
	fi
fi

dnl
dnl APR_CHECK_DIRENT_TYPE
dnl
dnl  Decide if d_type is available in the dirent structure 
dnl  on this platform.  Not part of the Single UNIX Spec
dnl
AC_CACHE_CHECK([for dirent.d_type],
		ac_cv_dirent_d_type,
			AC_TRY_LINK([
				#include <dirent.h>
				],
			       	[ struct dirent de; de.d_type = DT_REG; ],
				[ ac_cv_dirent_d_type="yes" ],
				[ ac_cv_dirent_d_type="no" ]
			)
)

if test "x$ac_cv_dirent_d_type" = "xyes"; then
	AC_DEFINE(HAVE_DIRENT_TYPE, "d_type", [Define if dirent has d_type member])
fi


# taken from gnash configure.ac
# (tm_gmtoff and timezone checks)
AC_CACHE_CHECK([whether struct tm has tm_gmtoff], ac_cv_tm_gmtoff, [
	AC_TRY_LINK([
/* ctime(1) says "The glibc version of struct tm has additional fields
 * defined  when _BSD_SOURCE was set before including <time.h>"
 * In practice, you don't need to define it yourself (tested on glibc-2.2.1 
 * and 2.3.6) but if you *do* define it yourself, it makes *all* functions
 * favour BSD-like behaviour over of GNU/POSIX, which seems dangerous.
 */
// #define _BSD_SOURCE 1
#include <time.h>
], 
		[ struct tm tm; long l = tm.tm_gmtoff; ], 
		[ ac_cv_tm_gmtoff="yes" ],
		[ ac_cv_tm_gmtoff="no" ]
	)
])

if test "x$ac_cv_tm_gmtoff" = "xyes" ; then
	AC_DEFINE(HAVE_TM_GMTOFF, [1], [struct tm has member tm_gmtoff])
fi

AC_CACHE_CHECK([whether timezone is a long], ac_cv_long_timezone, [
	AC_TRY_LINK([
/* On Linux/glibc, tzset(3) says "extern long timezone;" (seconds West of GMT)
 * but on BSD char *timezone(int,int) is a function returning a string name.
 * The BSD function timegm() may be the equivalent, but this should
 * not be necessary because on BSD the code should use tm.tm_gmtoff instead
 * (use of long timezone is a fallback strategy for when tm_gmtoff exists not).
 */
#include <stdio.h>
#include <time.h>
extern long timezone;
  ],
    [ printf("%ld", timezone); ], 
    [ ac_cv_long_timezone="yes" ],
    [ ac_cv_long_timezone="no" ]
   )
])

if test "x$ac_cv_long_timezone" = "xyes" ; then
	AC_DEFINE(HAVE_LONG_TIMEZONE, [1], [extern timezone is a long integer, not a function])
fi


AC_DEFINE([_GNU_SOURCE], [], [Enable GNU Extensions])

dnl Adding -Werror to CFLAGS early prevents configure tests from running.
dnl Add now.
CFLAGS="$CFLAGS $werror_flags"

AC_CONFIG_HEADER(config.h)
AC_CONFIG_FILES([Makefile
		openbsd-compat/Makefile
		src/Makefile
		])

AC_OUTPUT 
