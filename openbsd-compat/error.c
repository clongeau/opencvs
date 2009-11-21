/*
 * Copyright (c) 2004-2005 Todd C. Miller <Todd.Miller@courtesan.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "config.h"

#ifndef HAVE_ERR_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <config.h>
#include <compat.h>
#include "error.h"

#ifndef lint
__unused static const char rcsid[] = "$Sudo: error.c,v 1.7 2005/11/18 01:39:58 millert Exp $";
#endif /* lint */

static void _warning	__P((int, const char *, va_list));
       void cleanup	__P((int));

void
#ifdef __STDC__
err(int eval, const char *fmt, ...)
#else
err(eval, fmt, va_alist)
	int eval;
	const char *fmt;
	va_dcl
#endif
{
	va_list ap;
#ifdef __STDC__
	va_start(ap, fmt);
#else
	va_start(ap);
#endif
	_warning(1, fmt, ap);
	va_end(ap);
	cleanup(0);
	exit(eval);
}

void
#ifdef __STDC__
errx(int eval, const char *fmt, ...)
#else
errx(eval, fmt, va_alist)
	int eval;
	const char *fmt;
	va_dcl
#endif
{
	va_list ap;
#ifdef __STDC__
	va_start(ap, fmt);
#else
	va_start(ap);
#endif
	_warning(0, fmt, ap);
	va_end(ap);
	cleanup(0);
	exit(eval);
}

void
#ifdef __STDC__
warn(const char *fmt, ...)
#else
warn(fmt, va_alist)
	const char *fmt;
	va_dcl
#endif
{
	va_list ap;
#ifdef __STDC__
	va_start(ap, fmt);
#else
	va_start(ap);
#endif
	_warning(1, fmt, ap);
	va_end(ap);
}

void
#ifdef __STDC__
warnx(const char *fmt, ...)
#else
warnx(fmt, va_alist)
	const char *fmt;
	va_dcl
#endif
{
	va_list ap;
#ifdef __STDC__
	va_start(ap, fmt);
#else
	va_start(ap);
#endif
	_warning(0, fmt, ap);
	va_end(ap);
}

static void
_warning(use_errno, fmt, ap)
	int use_errno;
	const char *fmt;
	va_list ap;
{
	int serrno = errno;

	fputs(getprogname(), stderr);
	if (fmt != NULL) {
		fputs(": ", stderr);
		vfprintf(stderr, fmt, ap);
	}
	if (use_errno) {
	    fputs(": ", stderr);
	    fputs(strerror(serrno), stderr);
	}
	putc('\n', stderr);
}

#endif /* !HAVE_ERR_H */
