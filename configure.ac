AC_INIT([OpenCVS],[4.2.0],[Charles Longeau <chl@openbsd.org>])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CC
AC_PROG_YACC
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_PROG_LN_S

AC_CHECK_HEADERS([sys/dirent.h dirent.h sys/time.h utimes.h])
AC_CHECK_FUNCS([fgetln strlcat strlcpy strtonum getdirentries getdents])

#comes from sudo's configure.in
AC_CHECK_FUNCS(utimes, [AC_CHECK_FUNCS(futimes futimesat, [break])], [AC_CHECK_FUNCS(futime) AC_LIBOBJ(utimes)])


#from here everything comes from portable openssh configure.ac script
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

AC_DEFINE([_GNU_SOURCE], [], [Enable GNU Extensions])

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

dnl Adding -Werror to CFLAGS early prevents configure tests from running.
dnl Add now.
CFLAGS="$CFLAGS $werror_flags"

AC_CONFIG_HEADER(config.h)
AC_CONFIG_FILES([Makefile
		openbsd-compat/Makefile
		src/Makefile
		])

AC_OUTPUT 
