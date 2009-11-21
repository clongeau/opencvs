bin_PROGRAMS = opencvs

opencvs_SOURCES = cvs.c add.c admin.c annotate.c atomicio.c commit.c config.c \
		  checkout.c client.c buf.c cmd.c date.y diff.c diff3.c \
		  diff_internals.c edit.c entries.c fatal.c file.c getlog.c \
		  hash.c hash_func.c history.c log.c logmsg.c modules.c \
		  import.c init.c release.c remove.c repository.c rcs.c \
		  rcsnum.c remote.c root.c server.c status.c tag.c trigger.c \
		  worklist.c util.c update.c version.c watch.c xmalloc.c

INCLUDES = -I$(top_srcdir)/openbsd-compat/

LIBCOMPAT=$(top_srcdir)/openbsd-compat/libopenbsd-compat.a

LDADD = $(LIBCOMPAT)
