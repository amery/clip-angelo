/*
	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

	Revision 1.13  2003/09/02 14:27:42  clip
	changes for MINGW from
	Mauricio Abre <maurifull@datafull.com>
	paul

*/

#include "ci_clip.h"
#include <sys/types.h>
#include <sys/stat.h>

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <locale.h>
#include <dirent.h>
#include <ctype.h>

#include "ci_clipcfg.h"

#ifndef OS_MINGW
#include <sys/mman.h>
#endif

#include "ci_gettext.h"
#include "ci_clic.h"
#include "ci_plural.h"

#ifdef HAVE_ICONV
#include "cl_iconv.h"
#else
#include "libclipscreen/ci_charset.h"
#endif

#ifndef O_BINARY
#define O_BINARY 0
#endif

#ifdef OS_CYGWIN
#define NO_MMAP
#endif
#include "clicgt/typedef_struct_Locale.h"

static Locale *lp = 0;

static Locale *init_locale(char *module);

static int inited = 0;

static char *find_msg(Locale * lp, const char *msgid, int *lenp);

static nls_uint32 SWAP(nls_uint32 i);

static unsigned long hash_string(const char *str);

static char *tr_charset(Locale * lp, char *msg);

#include "clicgt/static_m_access.h"
#include "clicgt/static_init_locale.h"
#include "clicgt/static_nls_uint32_SWAP.h"
#include "clicgt/static_find_msg.h"

/* We assume to have `unsigned long int' value with at least 32 bits.  */
#define HASHWORDBITS 32

/* Defines the so called `hashpjw' function by P.J. Weinberger
   [see Aho/Sethi/Ullman, COMPILERS: Principles, Techniques and Tools,
   1986, 1987 Bell Telephone Laboratories, Inc.]  */
#include "clicgt/static_hash_string.h"

#include "clicgt/_clic_gettext.h"
#include "clicgt/_clic_ngettext.h"
#include "clicgt/tr_charset.h"

#if 0

static void
delete_Locale(void *item)
{
   Locale   *lp;

   if (!item)
      return;

   lp = (Locale *) item;

#ifndef NO_MMAP
   if (lp->use_mmap)
      munmap((caddr_t) lp->data, lp->mmap_size);
   else
#endif
      free(lp->data);
   if (lp->fd != -1)
      close(lp->fd);
   free(lp->name);
   free(lp->module);
   free(lp->charset);
   if (lp->pd)
      plural_delete(lp->pd);

#ifdef HAVE_ICONV
   if (lp->it != (iconv_t) - 1 && lp->it != (iconv_t) - 2)
      iconv_close(lp->it);
#endif

   free(lp);
}

#endif
