/*
	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/
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

#include <ci_clip.h>
#include <ci_coll.h>
#include <ci_gettext.h>
#include <ci_plural.h>
#include <ci_clipcfg.h>

#ifdef OS_CYGWIN
#undef NO_MMAP
#define NO_MMAP
#endif

#ifndef NO_MMAP
#include <sys/mman.h>
#endif

#ifndef O_BINARY
#define O_BINARY 0
#endif

#include <gettext/typedef_struct_Locale.h>

static nls_uint32 SWAP(nls_uint32 i);

static void delete_Locale(void *item);

static char *find_msg(Locale * lp, const char *msg, int *lenp);

static unsigned long hash_string(const char *str);

static int cmp_Locale(void *p1, void *p2);

static Locale *find_locale(char *module);

static void add_locale(char *module, char *filename);

static Coll locales = { 0, 0, 0, 0, delete_Locale, cmp_Locale };

static Coll locale_names = { 0, 0, 0, 0, free, 0 };

/* [ publics */

#include <gettext/_clip_add_locale.c>
#include <gettext/_clip_gettext.c>
#include <gettext/_clip_locale_msg.c>
#include <gettext/_clip_locale_msg_plural.c>
#include <gettext/_clip_module_locale.c>

/* publics ][ statics */

#include <gettext/static_find_locale.c>
#include <gettext/static_cmp_Locale.c>
#include <gettext/static_m_access.c>
#include <gettext/static_add_locale.c>
#include <gettext/static_delete_Locale.c>
#include <gettext/static_SWAP.c>
#include <gettext/static_find_msg.c>

/* We assume to have `unsigned long int' value with at least 32 bits.  */
#define HASHWORDBITS 32

#include <gettext/static_hash_string.c>

/* ][ */
