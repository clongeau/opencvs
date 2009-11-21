#include <stdio.h>
#include <string.h>

#include "../config.h"
#include "sys-queue.h"

#ifndef __dead
# define __dead __attribute__((noreturn))
#endif

#ifdef HAVE_SYS_DIRENT_H
#include <sys/dirent.h>
#else
	#ifdef HAVE_DIRENT_H
	#include <dirent.h>
	#else
	#error "no dirent"
	#endif
#endif

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

/* From /usr/include/i386/limits.h */
#ifndef SIZE_T_MAX
#define SIZE_T_MAX UINT_MAX
#endif

#ifndef MD5_BLOCK_LENGTH
#include <sys/types.h>

#define	MD5_BLOCK_LENGTH		64
#define	MD5_DIGEST_LENGTH		16
#define	MD5_DIGEST_STRING_LENGTH	(MD5_DIGEST_LENGTH * 2 + 1)

typedef struct MD5Context {
	u_int32_t state[4];			/* state */
	u_int64_t count;			/* number of bits, mod 2^64 */
	u_int8_t buffer[MD5_BLOCK_LENGTH];	/* input buffer */
} MD5_CTX;


void	 MD5Init(MD5_CTX *);
void	 MD5Update(MD5_CTX *, const u_int8_t *, size_t);
void	 MD5Pad(MD5_CTX *);
void	 MD5Final(u_int8_t [MD5_DIGEST_LENGTH], MD5_CTX *);
void	 MD5Transform(u_int32_t [4], const u_int8_t [MD5_BLOCK_LENGTH]);
char	*MD5End(MD5_CTX *, char *);
char	*MD5File(const char *, char *);
char	*MD5FileChunk(const char *, char *, off_t, off_t);
char	*MD5Data(const u_int8_t *, size_t, char *);

#endif /* MD5_BLOCK_LENGTH */

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
