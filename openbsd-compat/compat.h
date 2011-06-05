#include <sys/types.h>

#include <stdio.h>
#include <string.h>

#include "../config.h"
#include "defines.h"
#include "sys-queue.h"

#ifndef HAVE_ERR_H
#include "error.h"
#endif

#ifndef HAVE_FNMATCH
#include "fnmatch.h"
#endif

#ifdef HAVE_DIRENT_H
	#include <dirent.h>
#else
	#ifdef HAVE_SYS_DIRENT_H
		#include <sys/dirent.h>
	#else
		#error "no dirent"
	#endif /* HAVE_SYS_DIRENT_H */
#endif /* HAVE_DIRENT_H */

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#ifndef SIZE_MAX
#include <stdint.h>
#endif

/* XXX */
#ifndef MAXBSIZE
#define MAXBSIZE 4096
#endif

/* From /usr/include/pwd.h */
#ifndef _PW_NAME_LEN
#define _PW_NAME_LEN            31      /* max length, not counting NUL */
#endif

#ifndef HAVE_STRTONUM
long long strtonum(const char *nptr, long long minval, long long maxval, const char **errstr);
#endif

#ifndef HAVE_STRLCPY
size_t strlcpy(char *dst, const char *src, size_t size);
#endif

#ifndef HAVE_STRLCAT
size_t strlcat(char *dst, const char *src, size_t size);
#endif

#ifndef HAVE_FGETLN
char * fgetln(FILE *stream, size_t *len);
#endif

#ifndef HAVE_DIRNAME
char *dirname(const char *path);
#endif

#ifndef HAVE_STRSEP
char *strsep(char **stringp, const char *delim);
#endif

#if !defined(HAVE_GETOPT) || !defined(HAVE_GETOPT_OPTRESET)
int BSDgetopt(int argc, char * const *argv, const char *opts);
char	*BSDoptarg;		/* argument associated with option */
int	BSDoptind;		/* index into parent argv vector */
#endif


