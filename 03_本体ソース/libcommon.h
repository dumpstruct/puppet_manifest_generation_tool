#ifndef _LIB_COMMON_H
#define _LIB_COMMON_H 1
/*
 * 共通ライブラリ
 *
 * 1995-2010 K.H
 * 2014 K.H  pmgt 向け最小コピー
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <strings.h>
#include <time.h>
#include <errno.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <assert.h>

#if ! defined(CYGWIN)
#include <ucontext.h>
#endif  /* ! CYGWIN */

#if ! defined(linux)

#include <signal.h>
#include <kstat.h>
#include <sys/ddi.h>  /* min(), max() */
#include <siginfo.h>

#else

#define _LARGEFILE_SOURCE
#define _FILE_OFFSET_BITS 64

#endif  /* ! linux */

/* -lnsl オプションが必須になること嫌う
#include <netdb.h>
 */

#if ! defined(__EXTENSIONS__) && ! defined(_REENTRANT)
extern char *strtok_r(char *, const char *, char **);
#endif


#if 1  /* pmgt */

#include "libbase.h"
#include "liboption.h"

#else  /* ! pmgt */

#include "libbase.h"
#include "libdebug.h"
#include "liblang.h"
#include "libstopwatch.h"
#include "liberrno.h"
#include "libchar.h"
#include "libstring.h"
#include "libstrings.h"
#include "libcalc.h"
#include "libstack.h"
#include "libnumeric.h"
#include "libunitconv.h"
#include "libmemory.h"
#include "libstdex.h"
#include "librandom.h"
#include "libbit.h"
#include "libtree.h"
#include "libuser.h"

#if ! defined(CYGWIN)
#include "libelf.h"
#endif  /* ! CYGWIN */

#include "libgroup.h"
#include "liboption.h"
#include "libcommand.h"
#include "libsound.h"
#include "libcode.h"
#include "libeuceye.h"
#include "libutf8.h"
#include "libvirtual_point.h"
#include "libprogress.h"
#include "libfile.h"
#include "libmap.h"
#include "libdir.h"
#include "libpagefile.h"
#include "libenv.h"
#include "libdate.h"
#include "libcsv.h"
#include "libdocuments.h"
#include "libnetwork.h"
#include "libvector.h"
#include "libauthority.h"
#include "libregex.h"
#include "libhtml.h"
#include "liboracle.h"
#include "liboracle_errmes.h"
#include "libwindows.h"
#include "libdlib.h"

#if ! defined(linux)

#include "libmenu.h"
#include "libgz.h"
#include "libreadline.h"
#include "libprocess.h"
#include "libsysinfo.h"
#include "libdaemon.h"
#include "libsignal.h"

#endif

#endif /* ! pmgt */

#endif /* _LIB_COMMON_H */
