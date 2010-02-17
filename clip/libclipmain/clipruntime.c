/*
   Copyright (C) 2001  ITK
   Authors  : Uri Hnykin <uri@itk.ru>, Paul Lasarev <paul@itk.ru>, and other
   License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/
#include "ci_clipcfg.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <signal.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#ifdef OS_MINGW
#include <ltdl.h>
#else
#include <dlfcn.h>
#endif
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>

#include "ci_clip.h"
#include "ci_clipvm.h"
#include "ci_hash.h"
#include "../libclipscreen/ci_screen.h"
#include "ci_error.ch"
#include "ci_hashcode.h"
#include "ci_rational.h"
#include "../libclipscreen/ci_charset.h"
#include "ci_coll.h"
#include "license.h"

#ifdef USE_TASKS
#include "../libcliptask/ci_task.h"
#endif

#define NEW(type) ((type*)calloc(sizeof(type),1))
#define NEWVECT(type,len) ((type*)calloc(sizeof(type),(len)))
#define VAR(type,var,ini) type *var=(type*)(ini)
#define NEWVAR(type,var) type *var=(type*)calloc(sizeof(type),1)
#define c_DELETE(type,var)	{destroy_##type(var);free(var);}

#define EXPAND_MACRO

#define MIN_NUMERIC_DIFF 0.00000000000001

extern CLIP_DLLEXPORT void _clip_close_all(ClipMachine * ClipMemoryStore);
extern int clip_RDDCLOSEALL(ClipMachine * ClipMemoryStore);

#ifdef FORCEALIGN

static long
GETLONG(void *p)
{
   long r;

   memcpy(&r, p, sizeof(r));
   return r;
}

static void
SETLONG(void *ptr, long l)
{
   memcpy(ptr, &l, sizeof(l));
}

#else

#define GETLONG(p) (*(long*)(p))
#define SETLONG(ptr,l) (*(long*)(ptr)=(l))

#endif

static ClipInitStruct *init_struct = 0;
char *_clip_hostcs = "CP437";

CLIP_DLLEXPORT void
_clip_init_struct(ClipInitStruct * sp)
{
   if (!init_struct)
      init_struct = sp;
}

static const char __file__[] = __FILE__;

int search_map(ClipVarEl * items, int count, long key, int *index);
static ClipVarEl *get_map_el(ClipMapVar * a, long no);
static void remove_private_vect(ClipMachine * ClipMemoryStore, int num, void *vect);
static int descr_str(ClipMachine * ClipMemoryStore, ClipVar * vp, char **str);
static void delete_ClipVarFrame(ClipMachine * ClipMemoryStore, ClipVarFrame * localvars);

char *CLIPROOT = CLIP_ROOT;
char *CLIP_LOCALE_ROOT = CLIP_ROOT;

static VarEntry *add_private(ClipMachine * ClipMemoryStore, long hash);
static VarEntry *fetch_var(ClipMachine * ClipMemoryStore, long hash);

static void dup_ref(ClipVar * dest, ClipVar * src);
static ClipVar *clip_fetch(ClipMachine * ClipMemoryStore, int dim, int push, int store, ClipVar ** mapp, long *hashp);

int _clip_profiler = 0;
static void fini_profiler(ClipMachine * ClipMemoryStore);

/* [ ClipBuf */

void
destroy_ClipBuf(ClipBuf * buf)
{
   if (!buf)
      return;
   if (buf->buf_of_ClipBuf)
      free(buf->buf_of_ClipBuf);
}

void
setLen_ClipBuf(ClipBuf * buf, int newlen)
{
   buf->buf_of_ClipBuf = (char *) realloc(buf->buf_of_ClipBuf, newlen);
   buf->len_of_ClipBuf = newlen;
}

CLIP_DLLEXPORT int
_clip_type(ClipVar * vp)
{
   int r = 0;

   if (vp)
		r = _clip_vptr(vp)->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType;
   return r;
}

CLIP_DLLEXPORT char *
_clip_type_name(ClipVar * vp)
{
   char *rc = "U";

   if (vp)
      {
	 switch (_clip_type(vp))
	    {
	    case NUMERIC_type_of_ClipVarType:
	       rc = "N";
	       break;
	    case CHARACTER_type_of_ClipVarType:
			 if (vp->ClipType_t_of_ClipVar.memo_of_ClipType)
		  rc = "M";
	       else
		  rc = "C";
	       break;
	    case LOGICAL_type_of_ClipVarType:
	       rc = "L";
	       break;
	    case DATE_type_of_ClipVarType:
	       rc = "D";
	       break;
	    case DATETIME_type_of_ClipVarType:
	       rc = "T";
	       break;
	    case OBJECT_type_of_ClipVarType:
	       rc = "O";
	       break;
	    case PCODE_type_of_ClipVarType:
	    case CCODE_type_of_ClipVarType:
	       rc = "B";
	       break;
	    case ARRAY_type_of_ClipVarType:
	       rc = "A";
	       break;
	    case MAP_type_of_ClipVarType:
	       rc = "O";
	       break;
	    }
      }
   return rc;
}

CLIP_DLLEXPORT ClipVar *
_clip_vptr(ClipVar * vp)
{
	if (vp && vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags)
      vp = _clip_vptr(vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar);
   return vp;
}

/* ] ClipBuf */
/* [ ClipVect */

void
destroy_ClipVect(ClipVect * vect)
{
   if (!vect)
      return;
   free(vect->items_of_ClipVect);
}

void
add_ClipVect(ClipVect * vect, void *item)
{
   int count = ++(vect->count_of_ClipVect);
   vect->items_of_ClipVect = realloc(vect->items_of_ClipVect, sizeof(void *) * count);

   vect->items_of_ClipVect[count - 1] = item;
}

void
clear_ClipVect(ClipVect * vect)
{
   vect->count_of_ClipVect = 0;
   free(vect->items_of_ClipVect);
   vect->items_of_ClipVect = 0;
  /*vect->items_of_ClipVect = realloc(vect->items_of_ClipVect, sizeof(void *) * 4); */
}

void
remove_ClipVect(ClipVect * vect, int no)
{
   int count = vect->count_of_ClipVect;

   if (no < 0 || no >= count)
      return;
   vect->count_of_ClipVect = --count;
   memmove(vect->items_of_ClipVect + no, vect->items_of_ClipVect + no + 1, (count - no) * sizeof(void *));
}

void
insert_ClipVect(ClipVect * vect, void *item, int pos)
{
   int count = vect->count_of_ClipVect;

   if (pos < 0 || pos > count)
      return;
   vect->count_of_ClipVect = ++count;
   vect->items_of_ClipVect = realloc(vect->items_of_ClipVect, sizeof(void *) * count);
   memmove(vect->items_of_ClipVect + pos + 1, vect->items_of_ClipVect + pos, (count - pos - 1) * sizeof(void *));

   vect->items_of_ClipVect[pos] = item;
}

/* ] ClipVect */
/* [ ClipMachine */

static int machineCount = 0;

typedef struct
{
   long hash;
   ClipFunction *f;
   ClipBlock block;
}
FuncEntry;

static ClipMachine *cur_mp = 0;
static ClipMachine *first_mp = 0;

int
_clip_init_tty(ClipMachine * ret)
{
   int r;
   char errbuf[128];

   if (ret->screen)
      {
	 restart_tty(ret->screen->base);
	 return 0;
      }

   r = init_tty(ret->screen_base, 0, _clip_envp, 0, (ScreenPgChars *) _clip_pgtbl, errbuf, sizeof(errbuf));

   if (r >= 0)
      {
	 ret->screen = new_Screen(ret->screen_base);
	 if (r > 0)
	    _clip_logg(0, "init screen: %s", errbuf);
	 r = 0;
      }
   else
      {
	 free(ret->screen_base);
	 snprintf(ret->syserr, sizeof(ret->syserr), "%s", errbuf);
	 ret->screen_base = 0;
	 ret->screen = 0;
      }

   if (ret->screen)
      {
	 ret->wboard.bottom_of_ClipRect = ret->windows->ClipRect_rect_of_ClipWindow.bottom_of_ClipRect = ret->screen->base->Lines - 1;
	 ret->wboard.right_of_ClipRect = ret->windows->ClipRect_rect_of_ClipWindow.right_of_ClipRect = ret->screen->base->Columns - 1;
	 ret->windows->ClipRect_format_of_ClipWindow = ret->windows->ClipRect_rect_of_ClipWindow;
      }

   return r;
}

int
_clip_restore_tty(ClipMachine * ClipMemoryStore)
{
   return restore_tty(ClipMemoryStore->screen->base);
}

static int errorlevel = 0;	/* result of errorlevel() */

static HashTable *all_publics = 0;
static HashTable *all_hashnames = 0;
static HashTable *all_functions = 0;

static HashTable *all_aliases = 0;
static HashTable *all_fields = 0;
static HashTable *all_keys = 0;
static HashTable *all_store = 0;

/*
static ClipVect *all_areas = 0;
static ClipVect *all_areaStack = 0;
*/

static Container *all_container = 0;

static DBDriver *all_dbdrivers = 0;
struct _RDD_DATA_VTBL_ **all_data_drivers;
struct _RDD_INDEX_VTBL_ **all_idx_drivers;
struct _RDD_MEMO_VTBL_ **all_memo_drivers;
static int all_ndbdrivers = 0;
static int all_ndata_drivers = 0;
static int all_nidx_drivers = 0;
static int all_nmemo_drivers = 0;
static char all_def_data_driver[6];
static char all_def_idx_driver[6];
static char all_def_memo_driver[6];
static char all_def_db_driver[9];

static SQLDriver *all_sqldrivers = 0;
static int all_nsqldrivers = 0;

static HashTable *all_tasklocks = 0;
static HashTable *all_fileopens = 0;
/*static ClipWindow *all_windows = 0;*/

static int *all_kbdbuf;
static int *all_kbdptr;

int _clip_sig_flag = 0;
void _clip_signal_real(int sig);

CLIP_DLLEXPORT struct ClipMachine *
new_ClipMachine(struct Screen *screen)
{
   ClipMachine *ret;

#ifdef MEMDEBUG
   {
      static int inited = 0;

      if (!inited)
	 {
	    malloc_logpath = "_memdebug";
	    malloc_debug(MEMDEBUG);
	    inited = 1;
	 }
   }
#endif

   ret = NEW(ClipMachine);
   if (all_functions)
      ret->functions = all_functions;
   else
      ret->functions = all_functions = new_HashTable();
   ++machineCount;

   ret->dbdrivers = &all_dbdrivers;
   ret->ndbdrivers = &all_ndbdrivers;
   ret->data_drivers = &all_data_drivers;
   ret->ndata_drivers = &all_ndata_drivers;
   ret->idx_drivers = &all_idx_drivers;
   ret->nidx_drivers = &all_nidx_drivers;
   ret->memo_drivers = &all_memo_drivers;
   ret->nmemo_drivers = &all_nmemo_drivers;
   ret->def_data_driver = all_def_data_driver;
   ret->def_idx_driver = all_def_idx_driver;
   ret->def_memo_driver = all_def_memo_driver;
   ret->def_db_driver = all_def_db_driver;

   ret->sqldrivers = &all_sqldrivers;
   ret->nsqldrivers = &all_nsqldrivers;

   if (all_tasklocks)
      ret->tasklocks = all_tasklocks;
   else
      ret->tasklocks = all_tasklocks = new_HashTable();

   if (all_fileopens)
      ret->fileopens = all_fileopens;
   else
      ret->fileopens = all_fileopens = new_HashTable();

   if (all_publics)
      ret->publics = all_publics;
   else
      ret->publics = all_publics = new_HashTable();
   ret->privates = new_HashTable();
   ret->spaces = new_HashTable();

/*
	if (all_aliases)
		ret->aliases = all_aliases;
	else
		ret->aliases = all_aliases = new_HashTable();
*/
   ret->aliases = new_HashTable();
/*
	if (all_fields)
		ret->fields = all_fields;
	else
		ret->fields = all_fields = new_HashTable();
*/
   ret->fields = new_HashTable();

   if (all_keys)
      ret->keys = all_keys;
   else
      ret->keys = all_keys = new_HashTable();

   if (all_store)
      ret->store = all_store;
   else
      ret->store = all_store = new_HashTable();

   ret->next = first_mp;
   first_mp = ret;
   ret->decimals = 2;
   ret->fileCreateMode = _clip_fileStrModeToNumMode("664");
   ret->dirCreateMode = _clip_fileStrModeToNumMode("753");
   ret->flags = CONSOLE_FLAG + INTENSITY_FLAG;
   ret->date_format = strdup(CLIP_DATEFORMAT_DEFAULT);
   ret->rootpath = NULL;
   if (CLIP_CENTURY_DEFAULT)
      ret->flags += CENTURY_FLAG;
#ifdef _WIN32
   ret->flags1 += DISPBOX_FLAG;
#endif
   ret->epoch = CLIP_EPOCH_DEFAULT;
   ret->path = strdup(".");
   ret->defaul = strdup(".");
   ret->typeahead = 32;

   if (all_kbdbuf)
      ret->kbdbuf = all_kbdbuf;
   else
      ret->kbdbuf = all_kbdbuf = (int *) calloc(ret->typeahead, sizeof(int));

   ret->kbdptr = &all_kbdptr;
   *(ret->kbdptr) = ret->kbdbuf;

   ret->cursor = SC_NORMAL;
   ret->altfile = 0;
   ret->out = stdout;
   ret->delimchars = strdup("  ");
   ret->eventmask = _CLIP_INKEY_KEYBOARD;
   if (screen)
      ret->screen = screen;
   else
      {
	 ret->screen_base = NEW(ScreenBase);
		/*_clip_init_tty(ret);
		_clip_restore_tty(ret);*/
      }
   ret->mblocksize = 64;
   ret->attr.standard_of_ClipAttr = COLOR_WHITE | COLOR_BACK_BLACK;
   ret->attr.enhanced_of_ClipAttr = COLOR_BLACK | COLOR_BACK_WHITE;
   ret->attr.border_of_ClipAttr = COLOR_BLACK | COLOR_BACK_BLACK;
   ret->attr.background_of_ClipAttr = COLOR_BLACK | COLOR_BACK_BLACK;
   ret->attr.unselected_of_ClipAttr = COLOR_BLACK | COLOR_BACK_WHITE;
   ret->attr.u1_of_ClipAttr = COLOR_BLACK | COLOR_BACK_BLACK;
   ret->attr.u2_of_ClipAttr = COLOR_BLACK | COLOR_BACK_BLACK;
   ret->attr.u3_of_ClipAttr = COLOR_BLACK | COLOR_BACK_BLACK;
   ret->attr.u4_of_ClipAttr = COLOR_BLACK | COLOR_BACK_BLACK;
   ret->attr.u5_of_ClipAttr = COLOR_BLACK | COLOR_BACK_BLACK;
   ret->buf = (char *) malloc(64);
   ret->buflen = 64;
	ret->driver = init_struct->_dbdrivers_of_ClipInitStruct[0];
   ret->syserr[0] = 0;

#if 1
   ret->windows = NEW(ClipWindow);
#else
   if (all_windows)
      ret->windows = all_windows;
   else
      ret->windows = all_windows = NEW(ClipWindow);
#endif

   ret->wnum = 0;
   if (ret->screen)
      {
	 ret->wboard.bottom_of_ClipRect = ret->windows->ClipRect_rect_of_ClipWindow.bottom_of_ClipRect = ret->screen->base->Lines - 1;
	 ret->wboard.right_of_ClipRect = ret->windows->ClipRect_rect_of_ClipWindow.right_of_ClipRect = ret->screen->base->Columns - 1;
	 ret->windows->ClipRect_format_of_ClipWindow = ret->windows->ClipRect_rect_of_ClipWindow;
      }
   ret->wshadow = -1;
#if 1
   if (all_container)
      ret->container = all_container;
   else
      ret->container = all_container = NEW(Container);
#else
   ret->container = NEW(Container);
#endif

   if (all_hashnames)
      ret->hashnames = all_hashnames;
   else
      ret->hashnames = all_hashnames = new_HashTable();
   ret->print_max_deep = CLIP_MAX_PRINT_DEEP;
   ret->print_deep = 0;

   {
      int i;

      for (i = 0; i < 256; i++)
	 {
	    ret->prntbl[i] = i;
	    ret->prn_xlat[i] = i;
	    ret->term_xlat[i] = i;
	 }
   }

/*
	if (all_areas)
	{
		ret->areas = all_areas;
		ret->areaStack = all_areaStack;
	}
	else
	{
		ret->areas = all_areas = NEW(ClipVect);
		ret->areaStack = all_areaStack = NEW(ClipVect);
	}
*/
   ret->areas = NEW(ClipVect);
   ret->areaStack = NEW(ClipVect);

   ret->index_buffer_limit = 32;

   ret->flags |= ESCAPE_FLAG;
   ret->flags |= EXCLUSIVE_FLAG;
   ret->flags |= TRANSLATE_FLAG;
   ret->flags1 |= OPTIMIZE_FLAG;

  /*
     #ifdef OS_LINUX
     if (ret->out && isatty(fileno(ret->out)))
     ret->flags1 |= FLUSHOUT_FLAG;
     #endif
   */
   ret->flags1 |= FLUSHOUT_FLAG;

   return ret;
}

char *_clip_progname = 0;
int _clip_argc = 0;
char **_clip_argv = 0;
char **_clip_envp = 0;
int log_level = 1;
FILE *logg = 0;
static int exited = 0;

int _clip_raw_argc = 0;
char **_clip_raw_argv = 0;

void
_clip_exit(void)
{
  /* close all ClipMachines */

   if (exited)
      return;
   exited = 1;

   while (first_mp)
      {
	 ClipMachine *ClipMemoryStore = first_mp;

	 first_mp = first_mp->next;
	 delete_ClipMachine(ClipMemoryStore);
      }
   _clip_logg(2, "exit");
   if (logg != stderr)
      fclose(logg);

#ifdef MEMDEBUG
   malloc_shutdown();
#endif

}

#define M_OFFS(base,nl,ns) ((base)+8+nl*sizeof(long)+ns*sizeof(short))

static char *
get_env(char **envp, char *name)
{
   char *s;
   int l;
   int ln = strlen(name);

   for (; envp && (s = *envp); ++envp)
      {
	 l = strcspn(s, "=");
	 if (ln == l && !memcmp(name, s, l))
	    {
	       s += l;
	       if (*s)
		  return s + 1;
	       else
		  return s;
	    }
      }
   return 0;
}

CLIP_DLLEXPORT char *
_clip_getenv(char *name)
{
   char *r;

   r = get_env(_clip_envp, name);
   return r;
}

static int
put_env(char ***envp, char *name)
{
   char *s;
   char **ep;
   int l, len, ln;

   ln = strcspn(name, "=");
   for (ep = *envp, len = 0; ep && (s = *ep); ++ep, ++len)
      {
	 l = strcspn(s, "=");
	 if (ln == l && !memcmp(name, s, l))
	    {
	      /* owerwrite */
	       free(s);
	       *ep = strdup(name);
	       return 1;
	    }
      }

   *envp = (char **) realloc(*envp, (len + 2) * sizeof(char *));

   (*envp)[len] = strdup(name);
   (*envp)[len + 1] = 0;

   return 0;
}

CLIP_DLLEXPORT int
_clip_putenv(char *name)
{
   int r;

   r = put_env(&_clip_envp, name);
   return r;
}

CLIP_DLLEXPORT int
_clip_put_env(char *name, char *val)
{
   int l1 = strlen(name);
   int l2 = strlen(val);
   int r;

#ifdef OS_MINGW
   char *buf = (char *) malloc(l1 + l2 + 2);
#else
   char *buf = (char *) alloca(l1 + l2 + 2);
#endif

   memcpy(buf, name, l1);
   memcpy(buf + l1 + 1, val, l2);
   buf[l1] = '=';
   buf[l1 + l2 + 1] = 0;
   r = put_env(&_clip_envp, buf);
#ifdef OS_MINGW
   free(buf);
#endif

   return r;
}

static void
set_sysvars(void)
{
   char *e;

   e = get_env(_clip_envp, "CLIP_LOGLEVEL");
   if (e && *e)
      log_level = atoi(e);

}

static void
process_environ(char *root, char *name)
{
   char buf[256];
   FILE *file;

   snprintf(buf, sizeof(buf), "%s/%s", root, name);
   file = fopen(buf, "rt");
   if (!file)
      goto ret;
   while (fgets(buf, sizeof(buf), file))
      {
	 char *s, *e, *g;
	 int l;
	 int overwrite = 0;

	 s = buf;

       again:
	 l = strspn(s, " \t");
	 s += l;
	 l = strlen(s);
	 if (l < 2)
	    continue;
	 s[l - 1] = 0;
	 if (s[0] == '#')
	    continue;
	 if (s[0] == '+')
	    {
	       s++;
	       overwrite = 1;
	    }
	 else if (s[0] == '?')
	    {
	       char *g1;

	       s++;
	       e = strchr(s, '=');
	       if (e)
		  *e++ = 0;
	       else
		  continue;
	       g = strchr(e + 1, '?');
	       if (g)
		  *g++ = 0;
	       else
		  continue;
	       g1 = get_env(_clip_envp, s);
	       if (!g1)
		  continue;
	       if (strcmp(e, g1))
		  continue;
	      /* condition matched */
	       s = g;
	       goto again;
	    }

	 if (!overwrite)
	    {
	       e = strchr(s, '=');
	       if (e)
		  *e = 0;
	       g = get_env(_clip_envp, s);
	      /* don't overwrite!! */
	       if (g)
		  continue;
	       if (e)
		  *e = '=';
	    }
	 put_env(&_clip_envp, s);
      }

   fclose(file);
 ret:
   set_sysvars();
}

int _clip_debuglevel = 0;

static void
debug_enable(char *opt)
{
   char *s;

   s = strchr(opt, '=');
   if (s)
      _clip_debuglevel = atoi(s + 1);
   else
      _clip_debuglevel++;
/*_clip_logg(0, "set debug level to %d", _clip_debuglevel);*/
}

static void
license_on_start(char *opt)
{
   fprintf(stdout, "%s", CLIP_LICENSE);
   exit(0);
}

static void
profile_enable(char *opt)
{
   char *s;

   s = strchr(opt, '=');
   if (s)
      _clip_profiler = atoi(s + 1);
   else
      _clip_profiler++;
/*_clip_logg(0, "set profiledebug level to %d", _clip_debuglevel);*/
}

static int need_stop = 0;

static void
breakpoint_on_start(char *opt)
{
   debug_enable("");
   need_stop = 1;
}

struct OptionAction
{
   const char *name;
   void (*action) (char *opt);
};

static struct OptionAction hidden_options[] = {
   {"--debug", debug_enable},
   {"--profile", profile_enable},
   {"--stop", breakpoint_on_start},
   {"--license", license_on_start},
};

#define HIDDEN_NUM (sizeof(hidden_options)/sizeof(struct OptionAction))

static void
process_args(int argc, char **argv, int *Argcp, char ***Argvp)
{
   int i, j;

   for (i = 0; i < argc; i++)
      {
	 int found = 0;

	 for (j = 0; j < HIDDEN_NUM; j++)
	    {
	       struct OptionAction *op;
	       int l, l1;
	       char *ap;

	       op = hidden_options + j;
	       ap = argv[i];

	       l = strlen(op->name);
	       l1 = strlen(ap);
	       if ((l <= l1) && (l == l1 || ap[l] == '=') && !memcmp(op->name, ap, l))
		  {
		     op->action(ap);
		     found = 1;
		     break;
		  }
	    }
	 if (!found)
	    {
	       int l;

	       l = (*Argcp)++;
	       (*Argvp) = (char **) realloc(*Argvp, sizeof(char *) * (l + 1));

	       (*Argvp)[l] = argv[i];
	    }
      }
}

int
_clip_init(ClipMachine * ClipMemoryStore, int Argc, char **Argv, char **envp)
{
   char *e;
   ClipFunction **np, ****npp;
   const char **sp;
   ClipFile **fp, ****fpp;
   int ret;

   if (ClipMemoryStore->inited)
      return 0;

   if (!init_struct)
      exit(100);

   ClipMemoryStore->inited = 1;

   if (!logg)
      logg = stderr;

   if (!_clip_progname)
      {
	 int arg_c = 0;
	 char **arg_v = 0;
	 char *p1, *p2;

	 _clip_put_env("CLIP_HOSTCS", init_struct->CLIP_HOSTCS);

	 _clip_raw_argc = Argc;
	 _clip_raw_argv = Argv;

	 process_args(Argc, Argv, &arg_c, &arg_v);

	 _clip_argc = arg_c;
	 _clip_argv = arg_v;

	 e = strrchr(arg_v[0], '/');

	 if (e)
	    _clip_progname = e + 1;
	 else
	    _clip_progname = arg_v[0];

	 for (; (e = *envp); ++envp)
	    put_env(&_clip_envp, e);

	 atexit(_clip_exit);

#ifndef OS_MINGW
	 signal(SIGHUP, _clip_signal);
	 signal(SIGQUIT, _clip_signal);
	 signal(SIGKILL, _clip_signal);
	 signal(SIGPIPE, _clip_signal);
	 signal(SIGALRM, _clip_signal);
	 if (_clip_debuglevel)
	    signal(SIGUSR1, _clip_sigdebug);
	 else
	    signal(SIGUSR1, SIG_IGN);
#endif

	 signal(SIGINT, _clip_sigint);
	 signal(SIGILL, _clip_signal);
	 signal(SIGABRT, _clip_signal);
	 signal(SIGFPE, _clip_signal);
	 signal(SIGSEGV, _clip_signal);
	 signal(SIGTERM, _clip_signal);

	 set_sysvars();

	 _clip_logg(2, "start");

	/* read CLIPROOT/environment */
	 e = get_env(_clip_envp, "CLIPROOT");
	 if (e && *e)
	    CLIPROOT = strdup(e);

	 process_environ(CLIPROOT, "etc/environment");
	 e = get_env(_clip_envp, "CLIP_LANG");
	 if (e == NULL)
	    e = get_env(_clip_envp, "LANG");
	 if (e && *e)
	    {
	       char buf[256];

	       snprintf(buf, sizeof(buf), "%s/lang", CLIPROOT);
	       _clip_logg(3, "process LANG environ '%s/%s'", buf, e);
	       process_environ(buf, e);
	    }
	 e = get_env(_clip_envp, "CHARSET");
	 if (e && *e)
	    {
	       char buf[256];

	       snprintf(buf, sizeof(buf), "%s/lang", CLIPROOT);
	       _clip_logg(3, "process CHARSET environ '%s/%s'", buf, e);
	       process_environ(buf, e);
	    }
	 e = get_env(_clip_envp, "TERM");
	 if (e && *e)
	    {
	       char buf[256];

	       snprintf(buf, sizeof(buf), "%s/term", CLIPROOT);
	       _clip_logg(3, "process TERM environ '%s/%s'", buf, e);
	       process_environ(buf, e);
	    }

	 e = _clip_getenv("CLIP_LOCALE");
	 if (!e)
	    {
	       e = _clip_getenv("CLIP_LANG");
	       if (e == NULL)
		  e = _clip_getenv("LANG");
	       if (e && !strcmp(e, "C"))
		  e = 0;
	       if (!e)
		  {
		     e = _clip_getenv("LC_ALL");
		     if (e && !strcmp(e, "C"))
			e = 0;
		  }
	       if (!e)
		  {
		     e = _clip_getenv("LC_MESSAGES");
		     if (e && !strcmp(e, "C"))
			e = 0;
		  }
	       if (e)
		  _clip_put_env("CLIP_LOCALE", e);
	    }

	 e = _clip_getenv("CLIP_CLIENTCS");
	 if (!e || !*e)
	    {
	       e = _clip_getenv("CLIP_LOCALE");
	       if (e)
		  {
		     char *s;

		     s = strrchr(e, '.');
		     if (s)
			e = s + 1;
		     else
			e = 0;
		  }
	       if (!e)
		  e = _clip_getenv("CLIP_CHARSET");
	       if (!e)
		  e = _clip_getenv("CHARSET");
	       if (e)
		  {
		     char *s;

		     s = strdup(e);
		     for (e = s; *e; e++)
			*e = tolower(*e);
		     _clip_put_env("CLIP_CLIENTCS", s);
		     free(s);
		  }
	    }
	 p1 = _clip_getenv("CLIP_HOSTCS");
	 p2 = _clip_getenv("CLIP_CLIENTCS");
	 if (p1 && !strcasecmp(p1, "c") && p2 && strcasecmp(p2, "c"))
	    _clip_put_env("CLIP_HOSTCS", p2);
	 e = _clip_getenv("CLIP_HOSTCS");
	 if (e && *e)
	    {
	       _clip_logg(3, "load charset tables '%s'", e);
	       load_charset_tables(e, _clip_cmptbl, _clip_uptbl, _clip_lowtbl, _clip_isalpha_tbl, _clip_pgtbl);
	       _clip_hostcs = strdup(e);
	    }

	 p1 = _clip_getenv("CLIP_HOSTCS");
	 p2 = _clip_getenv("CLIP_CLIENTCS");
	 if (p1 && p2 && strcmp(p1, p2))
	    {
	       cons_CharsetEntry *cs1 = 0, *cs2 = 0;
	       int len1 = 0, len2 = 0;

	       _clip_logg(2, "set stream io translation  %s -> %s", p1, p2);

	       if (load_charset_name(p1, &cs1, &len1))
		  {
		     _clip_logg(0, "cannot load charset file '%s': %s", p1, strerror(errno));
		  }

	       if (load_charset_name(p2, &cs2, &len2))
		  {
		     _clip_logg(0, "cannot load charset file '%s': %s", p2, strerror(errno));
		  }

	       if (cs1 && cs2)
		  {
		     make_translation(cs1, len1, cs2, len2, _clip_outtbl);
		     make_translation(cs2, len2, cs1, len1, _clip_intbl);
		  }

	       free(cs1);
	       free(cs2);
	    }

	 e = get_env(_clip_envp, "CLIP_LOCALE_ROOT");
	 if (e && *e)
	    CLIP_LOCALE_ROOT = strdup(e);

	 e = _clip_getenv("CLIP_LOCALE");
	 if (e && *e)
	    {
	       _clip_add_locale(e);
	    }
      }

   for (npp = init_struct->_libinits; *npp; ++npp)
      {
	 for (np = **npp; *np; ++np)
	    if ((ret = _clip_main_func(ClipMemoryStore, *np, _clip_argc, _clip_argv, _clip_envp)))
	       return ret;
      }
   for (np = init_struct->_inits; *np; ++np)
      if ((ret = _clip_main_func(ClipMemoryStore, *np, _clip_argc, _clip_argv, _clip_envp)))
	 return ret;

   for (fpp = init_struct->_libcpfiles; *fpp; ++fpp)
      {
	 for (fp = **fpp; *fp; ++fp)
	    _clip_register_file(ClipMemoryStore, *fp);
      }
   for (fp = init_struct->_cpfiles; *fp; ++fp)
      {
	 ClipFile *file = *fp;

	 _clip_register_file(ClipMemoryStore, file);
      }

   for (fpp = init_struct->_libcpfiles; *fpp; ++fpp)
      {
	 for (fp = **fpp; *fp; ++fp)
	    _clip_load_inits(ClipMemoryStore, *fp);
      }
   for (fp = init_struct->_cpfiles; *fp; ++fp)
      {
	 ClipFile *file = *fp;

	 _clip_load_inits(ClipMemoryStore, file);
      }

for (sp = init_struct->_pfunctions_of_ClipInitStruct; *sp; ++sp)
      if ((ret = _clip_load(ClipMemoryStore, *sp, 0, 0)))
	 return ret;

   if (need_stop)
      {
	 need_stop = 0;
	 _clip_in_breakpoint();
      }

   return 0;
}

int
_clip_register_file(ClipMachine * ClipMemoryStore, ClipFile * file)
{
   char *modp = file->body_of_ClipFile;
   char *modbeg = M_OFFS(modp, 2, 0);
   long funcOffs = GETLONG(M_OFFS(modp, 7, 4));
   int funcnum = GETLONG(M_OFFS(modp, 4, 2));
   char *bp = (modbeg + funcOffs);
   int i;

   _clip_hash_buckets(ClipMemoryStore, file);

   for (i = 0; i < funcnum; ++i, bp += 2 * sizeof(long))
      {
	 long hash = GETLONG(bp);
	 char *func = modbeg + GETLONG(bp + sizeof(long));
	 ClipBlock block;

	 block.file_of_ClipBlock = file;
	 block.function_of_ClipBlock = func;
	 if (_clip_register_block(ClipMemoryStore, block, hash))
		 ++file->refCount_of_ClipFile;
      }

   return 0;
}

void
_clip_log_init(char *filename)
{
   static int inited = 0;
   char buf[256];

   if (filename == NULL)
      {
	 if (inited)
	    return;
	 inited = 1;
	 sprintf(buf, "%s.log", _clip_progname);
	 logg = fopen(buf, "at");
	 if (!logg)
	    {
	       fprintf(stderr, "cannot open log file '%s'", buf);
	       logg = stderr;
	    }
      }
   else
      {
	 inited = 1;
	 if (logg != stderr)
	    fclose(logg);
	 logg = fopen(filename, "at");
	 if (!logg)
	    {
	       fprintf(stderr, "cannot open log file '%s'", buf);
	       logg = stderr;
	    }
      }
}

int
_clip_loglevel(int newlevel)
{
   int ret = log_level;

   if (newlevel >= 0)
      log_level = newlevel;
   return ret;
}

void
_clip_out_log(char *buf, int len)
{
   _clip_log_init(NULL);
   fwrite(buf, len, 1, logg);
}

void
_clip_flush_log()
{
   fflush(logg);
}

CLIP_DLLEXPORT void
_clip_logg(int level, const char *fmt, ...)
{
   va_list ap;
   time_t tt;
   struct tm *tp;

   if (level > log_level)
      return;

   _clip_log_init(NULL);
   if (!logg)
      goto ret;
   tt = time(0);
   tp = localtime(&tt);

   fprintf(logg, "%02d:%02d:%02d %s:%lu:%d ", tp->tm_hour, tp->tm_min, tp->tm_sec, _clip_progname, (unsigned long) getpid(), level);
   va_start(ap, fmt);
   vfprintf(logg, _clip_gettext(fmt), ap);
   va_end(ap);
   fputc('\n', logg);
   fflush(logg);

 ret:
   ;
}

void
_clip_signal(int sig)
{
   static int oldsig = 0;
   static int cnt = 0;

   if (cnt >= 10)
      exit(100 + sig);
   if (sig == oldsig)
      cnt++;
   else
      cnt = 0;
   oldsig = sig;
   _clip_logg(0, "CLIP VM debug signal %d #%d", sig, cnt);
   _clip_sig_flag = sig;
}

void
_clip_signal_real(int sig)
{
   const char *msg;
   ClipFrame *fp;
   ClipMachine *ClipMemoryStore;

   switch (sig)
      {
      case 0:
	 return;
#ifndef OS_MINGW
      case SIGHUP:
	 msg = "SIGHUP";
	 break;
      case SIGQUIT:
	 msg = "SIGQUIT";
	 break;
      case SIGKILL:
	 msg = "SIGKILL";
	 break;
      case SIGPIPE:
	 msg = "SIGPIPE";
	 break;
      case SIGALRM:
	 msg = "SIGALRM";
	 break;
#endif
      case SIGINT:
	 msg = "SIGINT";
	 break;
      case SIGILL:
	 msg = "SIGILL";
	 break;
      case SIGABRT:
	 msg = "SIGABRT";
	 break;
      case SIGFPE:
	 msg = "SIGFPE";
	 break;
      case SIGSEGV:
	 msg = "SIGSEGV";
	 break;
      case SIGTERM:
	 msg = "SIGTERM";
	 break;
      default:
	 msg = "UNKNOWN";
	 break;
      }
   ClipMemoryStore = first_mp;	/*cur_ClipMachine(); */
   _clip_sig_flag = 0;
   _clip_trap_err(ClipMemoryStore, EG_SIGNAL, 0, 0, "SIGNAL", sig, msg);
   _clip_call_errblock(ClipMemoryStore, EG_SIGNAL);

   _clip_logg(0, "got signal %s, exiting...", msg);
   fprintf(stderr, "\n%s: got signal %s, exiting...\n", _clip_progname, msg);
   fflush(stderr);

	for (fp = cur_mp->fp; fp; fp = fp->ClipFrame_up_of_ClipFrame)
      {
			_clip_logg(0, "trace: file '%s' line %d", fp->filename_of_ClipFrame, fp->line_of_ClipFrame);
			fprintf(stderr, "trace: file '%s' line %d\n", fp->filename_of_ClipFrame, fp->line_of_ClipFrame);
	 fflush(stderr);
      }

/*_clip_exit();*/
   exit(100 + sig);
}

ClipMachine *
cur_ClipMachine()
{
  /* thread-depend code */
   return cur_mp;
}

static VarEntry *
new_VarEntry(long hash)
{
   NEWVAR(VarEntry, ret);
	ret->hash_of_VarEntry = hash;
   return ret;
}

static void
delete_VarEntry(ClipMachine * ClipMemoryStore, VarEntry * vp)
{
   if (!vp)
      return;
	if (vp->VarEntry_next_of_VarEntry)
		delete_VarEntry(ClipMemoryStore, vp->VarEntry_next_of_VarEntry);
	_clip_destroy(ClipMemoryStore, &vp->ClipVar_var_of_VarEntry);
   free(vp);
}

static void
delete_VarTable(ClipMachine * ClipMemoryStore, HashTable * hp)
{
   int r;

   for (r = HashTable_first(hp); r; r = HashTable_next(hp))
      {
	 VAR(VarEntry, vp, HashTable_current(hp));
	 delete_VarEntry(ClipMemoryStore, vp);
      }
   delete_HashTable(hp);
}

static void
clear_VarTable(ClipMachine * ClipMemoryStore, HashTable * hp)
{
   int r;

   for (r = HashTable_first(hp); r; r = HashTable_next(hp))
      {
	 VAR(VarEntry, vp, HashTable_current(hp));
	 delete_VarEntry(ClipMemoryStore, vp);
      }
   HashTable_clear(hp);
}

static void
delete_spaces(ClipMachine * ClipMemoryStore, HashTable * hp)
{
   int r;

   for (r = HashTable_first(hp); r; r = HashTable_next(hp))
      {
	 VAR(HashTable, vp, HashTable_current(hp));
	 delete_VarTable(ClipMemoryStore, vp);
      }
   delete_HashTable(hp);
}

static void
delete_Functions(ClipMachine * ClipMemoryStore, HashTable * hp)
{
   int r;

   for (r = HashTable_first(hp); r; r = HashTable_next(hp))
      {
	 VAR(FuncEntry, fp, HashTable_current(hp));
	/*if (!fp->f)
	   delete_ClipFile(ClipMemoryStore, fp->block.file); */
	 free(fp);
      }
   delete_HashTable(hp);
}

void
delete_ClipMachine(ClipMachine * ClipMemoryStore)
{
   FILE *out;
   int i;
   ClipFunction **np, ****npp;
   int r;

   if (ClipMemoryStore->deleted)
      return;
   ClipMemoryStore->deleted = 1;

   if (_clip_profiler)
      {
	 fini_profiler(ClipMemoryStore);
      }

   if (ClipMemoryStore == first_mp)
      first_mp = ClipMemoryStore->next;
   else
      {
	 ClipMachine *ClipMemoryStorep;
	 for (ClipMemoryStorep = first_mp; ClipMemoryStorep && ClipMemoryStorep->next; ClipMemoryStorep = ClipMemoryStorep->next)
	    {
	       if (ClipMemoryStorep->next == ClipMemoryStore)
		  {
		     ClipMemoryStorep->next = ClipMemoryStore->next;
		     break;
		  }
	    }
      }

  /*if (machineCount == 1) */
   {
      _clip_close_all(ClipMemoryStore);
      clip_RDDCLOSEALL(ClipMemoryStore);
   }

   for (i = ClipMemoryStore->nexits - 1; i >= 0; --i)
      {
	 _clip_main_code(ClipMemoryStore, ClipMemoryStore->exits + i, _clip_argc, _clip_argv, _clip_envp);
	 destroy_Block(ClipMemoryStore, ClipMemoryStore->exits + i);
      }

   for (i = ClipMemoryStore->ncexits - 1; i >= 0; --i)
      _clip_main_func(ClipMemoryStore, ClipMemoryStore->cexits[i], _clip_argc, _clip_argv, _clip_envp);

   for (npp = init_struct->_libexits; *npp; ++npp)
      {
	 for (np = **npp; *np; ++np)
	    _clip_main_func(ClipMemoryStore, *np, _clip_argc, _clip_argv, _clip_envp);
      }
   for (np = init_struct->_exits; *np; ++np)
      _clip_main_func(ClipMemoryStore, *np, _clip_argc, _clip_argv, _clip_envp);

   clear_VarTable(ClipMemoryStore, ClipMemoryStore->privates);
  /*clear_VarTable(ClipMemoryStore, ClipMemoryStore->publics); */

   for (i = 0; i < ClipMemoryStore->dlls.count_of_ClipVect; ++i)
#ifdef OS_MINGW
      lt_dlclose(ClipMemoryStore->dlls.items_of_ClipVect[i]);
#else
      dlclose(ClipMemoryStore->dlls.items_of_ClipVect[i]);
#endif

   out = (FILE *) ClipMemoryStore->out;
   if (out != stdout)
      fclose(out);
   out = (FILE *) ClipMemoryStore->extra;
   if (out)
      fclose(out);
   out = (FILE *) ClipMemoryStore->alt;
   if (out)
      fclose(out);
   _clip_close_printer(ClipMemoryStore);

   if (ClipMemoryStore->pbuf)
      {
	 destroy_Buf(ClipMemoryStore->pbuf);
	 free(ClipMemoryStore->pbuf);
      }

   delete_VarTable(ClipMemoryStore, ClipMemoryStore->privates);
   delete_spaces(ClipMemoryStore, ClipMemoryStore->spaces);
   delete_HashTable(ClipMemoryStore->aliases);
   delete_HashTable(ClipMemoryStore->fields);

   if (machineCount == 1)
      {
	 delete_VarTable(ClipMemoryStore, ClipMemoryStore->publics);
	 all_publics = 0;
	 all_aliases = 0;
	 all_fields = 0;
	 for (r = HashTable_first(ClipMemoryStore->store); r; r = HashTable_next(ClipMemoryStore->store))
	    free(HashTable_current(ClipMemoryStore->store));
	 delete_HashTable(ClipMemoryStore->store);
	 all_store = 0;
      }

   if (machineCount == 1)
      {
	 for (r = HashTable_first(ClipMemoryStore->hashnames); r; r = HashTable_next(ClipMemoryStore->hashnames))
	    free(HashTable_current(ClipMemoryStore->hashnames));
	 delete_HashTable(ClipMemoryStore->hashnames);
	 all_hashnames = 0;
      }

   _clip_delete(ClipMemoryStore, ClipMemoryStore->trapVar);
   _clip_delete(ClipMemoryStore, ClipMemoryStore->errorblock);
   _clear_trap(ClipMemoryStore);

/*	if (machineCount == 1)
	{*/
   destroy_ClipVect(ClipMemoryStore->areas);
   destroy_ClipVect(ClipMemoryStore->areaStack);
   free(ClipMemoryStore->areas);
   free(ClipMemoryStore->areaStack);
/*	}*/

   free(ClipMemoryStore->date_format);
   free(ClipMemoryStore->path);
   free(ClipMemoryStore->defaul);
   free(ClipMemoryStore->altfile);
   free(ClipMemoryStore->extrafile);
   if (ClipMemoryStore->rootpath != NULL)
      free(ClipMemoryStore->rootpath);
   free(ClipMemoryStore->delimchars);
   if (machineCount == 1)
      {
	 delete_Functions(ClipMemoryStore, ClipMemoryStore->functions);
	 all_functions = 0;
      }

   free(ClipMemoryStore->buf);

   for (i = 0; i < ClipMemoryStore->history.count_of_ClipVect; ++i)
      free(ClipMemoryStore->history.items_of_ClipVect[i]);

   destroy_ClipVect(&ClipMemoryStore->history);

   free(ClipMemoryStore->exits);
   free(ClipMemoryStore->cexits);
  /*free(*ClipMemoryStore->dbdrivers); */

   if (machineCount == 1)
      free(ClipMemoryStore->kbdbuf);

   destroy_ClipVect(&ClipMemoryStore->dlls);

   if (ClipMemoryStore->screen_base)
      {
	 if (ClipMemoryStore->screen)
	    delete_Screen(ClipMemoryStore->screen);
	 destroy_tty(ClipMemoryStore->screen_base);
	 free(ClipMemoryStore->screen_base);
      }

  /*if ( machineCount == 1 ) */
   {
     /*all_windows = 0; */
      for (i = 0; i <= ClipMemoryStore->wnum; i++)
	 {
	    ClipWindow *wp = ClipMemoryStore->windows + i;

		 if (wp->save_of_ClipWindow)
			 free(wp->save_of_ClipWindow);
	 }
      free(ClipMemoryStore->windows);
   }

   --machineCount;

   free(ClipMemoryStore);
}

CLIP_DLLEXPORT void
_clear_trap(ClipMachine * ClipMemoryStore)
{
   int i;

   for (i = 0; i < ClipMemoryStore->trapTrace.count_of_ClipVect; ++i)
      free(ClipMemoryStore->trapTrace.items_of_ClipVect[i]);
   clear_ClipVect(&ClipMemoryStore->trapTrace);
}

CLIP_DLLEXPORT void
_clip_log_call(ClipMachine * ClipMemoryStore)
{
   ClipFrame *fp = ClipMemoryStore->fp;
   if (_clip_profiler)
      _clip_start_profiler(ClipMemoryStore);

   if (log_level >= 5)
      {
			_clip_logg(5, "C call: proc '%s' file '%s' line %d", fp->procname_of_ClipFrame ? fp->procname_of_ClipFrame : "unknown", fp->filename_of_ClipFrame, fp->line_of_ClipFrame);
      }
}

CLIP_DLLEXPORT int
_clip_errorblock(ClipMachine * ClipMemoryStore, ClipVar * vp, int genCode)
{
   int r;
   ClipVar res;
   memset(&res, 0, sizeof(res));

   r = _clip_errorblock_res(ClipMemoryStore, vp, genCode, &res);

   _clip_destroy(ClipMemoryStore, &res);

   if (!r)
      r = _clip_log(&res);
   else
      r = 0;

   return 0;
}

CLIP_DLLEXPORT int
_clip_errorblock_res(ClipMachine * ClipMemoryStore, ClipVar * vp, int genCode, ClipVar * res)
{
   int r;

   if (ClipMemoryStore->noerrblock)
      return 0;

  /* default err handler */
   if (!ClipMemoryStore->errorblock || (_clip_type(ClipMemoryStore->errorblock) != CCODE_type_of_ClipVarType && _clip_type(ClipMemoryStore->errorblock) != PCODE_type_of_ClipVarType))
      {
	 char *msg = 0;
	 int i;

	 descr_str(ClipMemoryStore, vp, &msg);
	 if (!msg)
	    msg = "unknown runtime error";
	 printf("\n%s: system errorblock:\n", _clip_progname);
	 printf("%s: runtime error: %s\n", _clip_progname, msg);
	 _clip_logg(0, "%s: systerm errorblock: runtime error: %s", _clip_progname, msg);
	 for (i = 0; i < ClipMemoryStore->trapTrace.count_of_ClipVect; ++i)
	    {
	       printf("trace: %s\n", (char *) ClipMemoryStore->trapTrace.items_of_ClipVect[i]);
	       _clip_logg(0, "trace: %s", (char *) ClipMemoryStore->trapTrace.items_of_ClipVect[i]);
	    }
	 clip___QUIT(ClipMemoryStore);
      }

   ClipMemoryStore->errorlevel++;
   if (ClipMemoryStore->errorlevel > CLIP_MAX_ERRORBLOCK_DEEP)
      {
	 int i;

	 printf("%s: errorblock nested too deep (%d):\n", _clip_progname, CLIP_MAX_ERRORBLOCK_DEEP);
	 _clip_logg(0, "%s: errorblock nested too deep (%d): ", _clip_progname, CLIP_MAX_ERRORBLOCK_DEEP);
	 for (i = 0; i < ClipMemoryStore->trapTrace.count_of_ClipVect; ++i)
	    {
	       printf("trace: %s\n", (char *) ClipMemoryStore->trapTrace.items_of_ClipVect[i]);
	       _clip_logg(0, "trace: %s", (char *) ClipMemoryStore->trapTrace.items_of_ClipVect[i]);
	    }

	 clip___QUIT(ClipMemoryStore);
      }

   if (_clip_type(vp) == MAP_type_of_ClipVarType)
      {
	 double d = 0;

	 _clip_mgetn(ClipMemoryStore, vp, HASH_gencode, &d);
	 if (d == 0.0)
	    {
	       _clip_var_num(genCode, res);
	       _clip_madd(ClipMemoryStore, vp, HASH_gencode, res);
	       _clip_destroy(ClipMemoryStore, res);
	    }
      }

   r = _clip_eval(ClipMemoryStore, ClipMemoryStore->errorblock, 1, vp, res);
   ClipMemoryStore->errorlevel--;

   return r;
}

static int
do_call(ClipMachine * ClipMemoryStore, ClipFunction f, int argc, int isProc, int rest, ClipVarFrame * uplocals)
{
   int ret, _argc = ClipMemoryStore->argc;
   ClipFrame *fp = ClipMemoryStore->fp;
   ClipVar *bp = ClipMemoryStore->bp;
	ClipVarFrame *s_up = ClipMemoryStore->fp->ClipVarFrame_localvars_of_ClipFrame;

   ClipMemoryStore->argc = argc;
	ClipMemoryStore->bp = fp->ClipVar_sp_of_ClipFrame;
   if (uplocals)
		ClipMemoryStore->fp->ClipVarFrame_localvars_of_ClipFrame = uplocals;

   if (_clip_profiler)
      {
	 _clip_stop_profiler(ClipMemoryStore);
      }

   if (log_level >= 6)
      {
			_clip_logg(6, "C call from: proc '%s' file '%s' line %d", fp->procname_of_ClipFrame ? fp->procname_of_ClipFrame : "unknown", fp->filename_of_ClipFrame, fp->line_of_ClipFrame);
      }

   for (;;)
      {
	 int dnext = 0;

	 if (_clip_debuglevel)
	    {
	       dnext = _clip_debugnext;
	      /*printf("-dnext=%d line %d\n", dnext, fp->line); */
	       _clip_debug(ClipMemoryStore);
	    }

#ifdef USE_TASKS
	 Task_yield();
#endif

	 ret = f(ClipMemoryStore);

	 if (_clip_sig_flag)
	    _clip_signal_real(_clip_sig_flag);

	 if (_clip_debuglevel)
	    {
	      /*printf("+dnext=%d _clip_debugnext=%d line %d\n", dnext, _clip_debugnext, fp->line); */
	       if (!dnext && _clip_debugnext)
		  {
		     _clip_debugnext = 0;
/*_clip_debug(ClipMemoryStore);*/
		  }
	    }

	 if (log_level >= 6)
		 _clip_logg(6, "C return=%d to: proc '%s' file '%s' line %d", ret, fp->procname_of_ClipFrame ? fp->procname_of_ClipFrame : "unknown", fp->filename_of_ClipFrame, fp->line_of_ClipFrame);

	 if (!ret)
	    break;

	 if (!ClipMemoryStore->trapVar)
	    _clip_generr(ClipMemoryStore, ret);

	 if (ret < 0)
	    break;

	 if (!_clip_errorblock(ClipMemoryStore, ClipMemoryStore->trapVar, ret))
	    {
	       ret = -ret;
	       break;
	    }
	 ClipMemoryStore->argc = _argc;
	 ClipMemoryStore->fp = fp;
	 ClipMemoryStore->bp = bp;
	 ClipMemoryStore->fp->ClipVarFrame_localvars_of_ClipFrame = s_up;
      }

   ClipMemoryStore->argc = _argc;
   ClipMemoryStore->fp = fp;
   ClipMemoryStore->bp = bp;
	ClipMemoryStore->fp->ClipVarFrame_localvars_of_ClipFrame = s_up;

   if (isProc)
      ++argc;
   for (; argc; --argc)
      {
			--(fp->ClipVar_sp_of_ClipFrame);
	_clip_destroy(ClipMemoryStore, fp->ClipVar_sp_of_ClipFrame);
      }

   if (_clip_profiler)
      {
	 _clip_start_profiler(ClipMemoryStore);
	ClipMemoryStore->pbucket->callno_of_ProfileBucket--;
      }

   return ret;
}

static void
init_profiler(ClipMachine * ClipMemoryStore)
{
   if (ClipMemoryStore->profiler)
      return;

   ClipMemoryStore->profiler = new_HashTable();
}

#if 0
static void
delete_profile_bucket(void *bpp)
{
   ProfileBucket *bp = (ProfileBucket *) bpp;
   if (!bp)
      return;
   free(bp->procname);
   free(bp->filename);
#if 0
   destroy_Coll(&(bp->callby));
#endif
   free(bp);
}

static int
cmp_profile_bucket(void *p1, void *p2)
{
   long h1 = ((ProfileBucket *) p1)->hash;
   long h2 = ((ProfileBucket *) p2)->hash;
   if (h1 < h2)
      return -1;
   else if (h1 > h2)
      return 1;
   else
      return 0;
}

#endif

static ProfileBucket *
find_profile_bucket_name(ClipMachine * ClipMemoryStore, const char *procname, const char *filename)
{
   long hash;
   ProfileBucket *bp;

   hash = _clip_casehashbytes(0, procname, strlen(procname));
   hash = _clip_casehashbytes(hash, filename, strlen(filename));

   init_profiler(ClipMemoryStore);
   bp = (ProfileBucket *) HashTable_fetch(ClipMemoryStore->profiler, hash);
   if (!bp)
      {
	 bp = NEW(ProfileBucket);
	bp->hash_of_ProfileBucket = hash;
	bp->procname_of_ProfileBucket = strdup(procname);
	bp->filename_of_ProfileBucket = strdup(filename);
#if 0
	 init_Coll(&bp->callby, delete_profile_bucket, cmp_profile_bucket);
#endif
	 HashTable_insert(ClipMemoryStore->profiler, bp, hash);
      }

   return bp;
}

static ProfileBucket *
find_profile_bucket(ClipMachine * ClipMemoryStore)
{
   const char *fs = "", *ps = "";

	if (ClipMemoryStore->fp && ClipMemoryStore->fp->procname_of_ClipFrame)
		ps = ClipMemoryStore->fp->procname_of_ClipFrame;
	if (ClipMemoryStore->fp && ClipMemoryStore->fp->filename_of_ClipFrame)
		fs = ClipMemoryStore->fp->filename_of_ClipFrame;

   return find_profile_bucket_name(ClipMemoryStore, ps, fs);
}

#ifndef timer_add
#define	timer_add(a, b, result)							  \
  do {										  \
	(result)->tv_sec = (a)->tv_sec + (b)->tv_sec;				  \
	(result)->tv_usec = (a)->tv_usec + (b)->tv_usec;				  \
	if ((result)->tv_usec >= 1000000)						  \
	  {										  \
	++(result)->tv_sec;							  \
	(result)->tv_usec -= 1000000;						  \
	  }										  \
  } while (0)
#endif

#ifndef timer_sub
#define timer_sub(a, b, result) \
   do {                         \
	   (result)->tv_sec = (a)->tv_sec - (b)->tv_sec;     \
	   (result)->tv_usec = (a)->tv_usec - (b)->tv_usec;  \
	   if ((result)->tv_usec < 0) {                      \
		  --(result)->tv_sec;                        \
		  (result)->tv_usec += 1000000;              \
	   }                                                 \
   } while (0)
#endif

static int
start_profiler(ClipMachine * ClipMemoryStore, ProfileBucket * bp)
{
   ProfileBucket *pbp;
   struct timeval tv;

   pbp = ClipMemoryStore->pbucket;

	if (ClipMemoryStore->pbucket && ClipMemoryStore->pbucket->started_of_ProfileBucket)
      _clip_stop_profiler(ClipMemoryStore);

	if (bp->started_of_ProfileBucket)
      {
	 ClipMemoryStore->pbucket = bp;
	 _clip_stop_profiler(ClipMemoryStore);
      }

   gettimeofday(&tv, 0);
	bp->timeval_start_of_ProfileBucket = tv;
   ClipMemoryStore->pbucket = bp;
	bp->started_of_ProfileBucket = 1;
	bp->callno_of_ProfileBucket++;

   return 0;
}

int CLIP_DLLEXPORT
_clip_start_profiler(ClipMachine * ClipMemoryStore)
{
   ProfileBucket *bp;

   bp = find_profile_bucket(ClipMemoryStore);

   return start_profiler(ClipMemoryStore, bp);
}

int CLIP_DLLEXPORT
_clip_start_profiler_name(ClipMachine * ClipMemoryStore, const char *procname, const char *filename)
{
   ProfileBucket *bp;

   if (!_clip_profiler)
      return 0;

   bp = find_profile_bucket_name(ClipMemoryStore, procname, filename);

   return start_profiler(ClipMemoryStore, bp);
}

static int
stop_profiler(ClipMachine * ClipMemoryStore, ProfileBucket * bp, struct timeval *tv)
{
   struct timeval dt, rt;

	if (!bp->started_of_ProfileBucket)
      return 0;

	timer_sub(tv, &bp->timeval_start_of_ProfileBucket, &dt);

	timer_add(&bp->timeval_self_of_ProfileBucket, &dt, &rt);
	bp->timeval_self_of_ProfileBucket = rt;
	bp->started_of_ProfileBucket = 0;

   return 0;
}

int CLIP_DLLEXPORT
_clip_stop_profiler(ClipMachine * ClipMemoryStore)
{
   struct timeval tv;
   ProfileBucket *bp;

   gettimeofday(&tv, 0);
   bp = ClipMemoryStore->pbucket;
   if (!bp)
      bp = find_profile_bucket(ClipMemoryStore);

   return stop_profiler(ClipMemoryStore, bp, &tv);
}

int CLIP_DLLEXPORT
_clip_stop_profiler_name(ClipMachine * ClipMemoryStore, const char *procname, const char *filename)
{
   struct timeval tv;
   ProfileBucket *bp;

   if (!_clip_profiler)
      return 0;

   gettimeofday(&tv, 0);
   bp = find_profile_bucket_name(ClipMemoryStore, procname, filename);

   return stop_profiler(ClipMemoryStore, bp, &tv);
}

static int
cmp_profile_out(void *p1, void *p2)
{
   ProfileBucket *bp1 = (ProfileBucket *) p1;
   ProfileBucket *bp2 = (ProfileBucket *) p2;

	if (bp1->timeval_self_of_ProfileBucket.tv_sec > bp2->timeval_self_of_ProfileBucket.tv_sec)
      return -1;
	else if (bp1->timeval_self_of_ProfileBucket.tv_sec < bp2->timeval_self_of_ProfileBucket.tv_sec)
      return 1;
	else if (bp1->timeval_self_of_ProfileBucket.tv_usec > bp2->timeval_self_of_ProfileBucket.tv_usec)
      return -1;
	else if (bp1->timeval_self_of_ProfileBucket.tv_usec < bp2->timeval_self_of_ProfileBucket.tv_usec)
      return 1;
   else
      return 0;
#if 0
   {
      int r;
      r = strcasecmp(bp1->procname, bp2->procname);
      if (r)
	 return r;
      r = strcasecmp(bp1->filename, bp2->filename);
      return r;
   }
#endif
}

static void
fini_profiler(ClipMachine * ClipMemoryStore)
{
   Coll coll;
   int r, i;
   FILE *out;
   char path[256];
   struct timeval total;
   unsigned long ms, us;

   init_profiler(ClipMemoryStore);
  /* generate report here */

   init_Coll(&coll, 0, cmp_profile_out);

   snprintf(path, sizeof(path), "%s.pro", _clip_progname);
   out = fopen(path, "w");
   if (!out)
      out = stderr;

   for (r = HashTable_first(ClipMemoryStore->profiler); r; r = HashTable_next(ClipMemoryStore->profiler))
      {
	 ProfileBucket *bp = (ProfileBucket *) HashTable_current(ClipMemoryStore->profiler);
	 insert_Coll(&coll, bp);
      }

   total.tv_sec = 0;
   total.tv_usec = 0;
   for (i = 0; i < coll.count; i++)
      {
	 ProfileBucket *bp = (ProfileBucket *) coll.items[i];
	 struct timeval tv;

	if (!bp->procname_of_ProfileBucket[0] && !bp->filename_of_ProfileBucket[0])
	    continue;

	ms = bp->timeval_self_of_ProfileBucket.tv_sec * 1000 + bp->timeval_self_of_ProfileBucket.tv_usec / 1000;
	us = bp->timeval_self_of_ProfileBucket.tv_usec % 1000;

	 tv = total;
	 timer_add(&tv, &bp->timeval_self_of_ProfileBucket, &total);

	 fprintf(out, "%18s %-13s %8ld calls %8lu.%03lu ms\n", bp->procname_of_ProfileBucket, bp->filename_of_ProfileBucket, bp->callno_of_ProfileBucket, ms, us);
      }

   ms = total.tv_sec * 1000 + total.tv_usec / 1000;
   us = total.tv_usec % 1000;
   fprintf(out, "---------------------------------\n");
   fprintf(out, "total registered %8lu.%03lu ms\n", ms, us);

   destroy_Coll(&coll);

   if (out != stderr)
      fclose(out);
}

static int
_clip_refclone(ClipMachine * ClipMemoryStore, ClipVar * dest, ClipVar * src)
{
	if (src->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType == F_MREF_ClipFlags)
      {
	 _clip_destroy(ClipMemoryStore, dest);
	 dup_ref(dest, src);
	 return 0;
      }
   else
      return _clip_clone(ClipMemoryStore, dest, src);
}

CLIP_DLLEXPORT int
_clip_eval(ClipMachine * ClipMemoryStore, ClipVar * blockp, int argc, ClipVar * stackp, ClipVar * retp)
{
#ifdef OS_MINGW
   ClipVar *stack = (ClipVar *) malloc(sizeof(ClipVar) * (argc + 1));
#else
   ClipVar *stack = (ClipVar *) alloca(sizeof(ClipVar) * (argc + 1));
#endif
   ClipFrame frame = { stack, stack + 1 + argc, __file__, __LINE__, 0, 0, 0, 0, 0, 0, "eval",
      argc + 1, 0
   };
   enum ClipVarType type = _clip_type(blockp);
   int i, j, r;

   if (type != PCODE_type_of_ClipVarType && type != CCODE_type_of_ClipVarType)
      {
	 _clip_trap_str(ClipMemoryStore, __file__, __LINE__, "clip_eval: non-code argument");
#ifdef OS_MINGW
	 free(stack);
#endif
	 return _clip_call_errblock(ClipMemoryStore, 1);
      }

   if (_clip_profiler)
      {
	 _clip_stop_profiler(ClipMemoryStore);
      }

   blockp = _clip_vptr(blockp);
   memset(stack, 0, sizeof(ClipVar) * (argc + 1));

   for (i = 1, j = 0; j < argc; ++j, ++i)
      {
	 _clip_refclone(ClipMemoryStore, stack + i, stackp + j);
      }

frame.ClipFrame_up_of_ClipFrame = ClipMemoryStore->fp;
   ClipMemoryStore->fp = &frame;

   if (_clip_profiler)
      {
	 _clip_stop_profiler(ClipMemoryStore);
      }

   if (type == CCODE_type_of_ClipVarType)
      r = _clip_func(ClipMemoryStore, blockp->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipFunction_function_of_ClipCodeVar, argc, 0,
blockp->ClipCodeVar_c_of_ClipVar.ClipVarFrame_uplocals_of_ClipCodeVar);
   else
      r = _clip_code_func(ClipMemoryStore, blockp->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipBlock_block_of_ClipCodeVar, argc, 0,
blockp->ClipCodeVar_c_of_ClipVar.ClipVarFrame_uplocals_of_ClipCodeVar);

		ClipMemoryStore->fp = frame.ClipFrame_up_of_ClipFrame;
   if (retp)
      *retp = stack[0];
   else
      _clip_destroy(ClipMemoryStore, stack + 0);

   if (_clip_profiler)
      {
	 _clip_start_profiler(ClipMemoryStore);
	ClipMemoryStore->pbucket->callno_of_ProfileBucket--;
      }

#ifdef OS_MINGW
   free(stack);
#endif
   return r;
}

int
clip_CLIP(ClipMachine * ClipMemoryStore)
{
   char *funcname = _clip_parc(ClipMemoryStore, 1);
   int i, parcount = _clip_parinfo(ClipMemoryStore, 0);
   ClipVar *params, *retVal = RETPTR(ClipMemoryStore);

   if (funcname == NULL)
      {
	 int r;

	 r = _clip_trap_err(ClipMemoryStore, EG_ARG, 0, 0, __FILE__, __LINE__, "CLIP");
	 return _clip_call_errblock(ClipMemoryStore, r);
      }
   params = malloc(parcount * sizeof(ClipVar));
   for (i = 2; i <= parcount; i++)
      params[i - 2] = *_clip_spar(ClipMemoryStore, i);

   _clip_clip(ClipMemoryStore, funcname, parcount - 1, params, retVal);

   free(params);
   return 0;
}

int
clip_CLIPA(ClipMachine * ClipMemoryStore)
{
   char *funcname = _clip_parc(ClipMemoryStore, 1);
   int parcount;
   ClipVar *params, *retVal = RETPTR(ClipMemoryStore);
   ClipVar *ap = _clip_par(ClipMemoryStore, 2);

	if (!funcname || !ap || ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType)
      {
	 int r;

	 r = _clip_trap_err(ClipMemoryStore, EG_ARG, 0, 0, __FILE__, __LINE__, "CLIPA");
	 return _clip_call_errblock(ClipMemoryStore, r);
      }

   parcount = ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
   params = ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar;

   _clip_clip(ClipMemoryStore, funcname, parcount, params, retVal);

   return 0;
}

CLIP_DLLEXPORT int
_clip_clip(ClipMachine * ClipMemoryStore, const char *funcname, int argc, ClipVar * stackp, ClipVar * retp)
{
   ClipVar *stack;
   ClipFrame frame = { 0, 0, __file__, __LINE__, 0, 0, 0, 0, 0, 0, "clip", 0, 0 };
   int i, j, r;
   ClipFunction *fp;
   ClipBlock *bp;
   long hash;

   hash = _clip_casehashbytes(0, funcname, strlen(funcname));
   if (!_clip_get_function(ClipMemoryStore, hash, &fp, &bp))
      {
	 return 1;
      }

   stack = (ClipVar *) malloc(sizeof(ClipVar) * (argc + 1));
	frame.ClipVar_stack_of_ClipFrame = stack;
	frame.ClipVar_sp_of_ClipFrame = stack + 1 + argc;
   memset(stack, 0, sizeof(ClipVar) * (argc + 1));

   for (i = 1, j = 0; j < argc; ++j, ++i)
      _clip_clone(ClipMemoryStore, stack + i, stackp + j);

	frame.ClipFrame_up_of_ClipFrame = ClipMemoryStore->fp;
   ClipMemoryStore->fp = &frame;

   if (fp)
      r = _clip_func(ClipMemoryStore, fp, argc, 0, 0);
   else
      r = _clip_code_func(ClipMemoryStore, bp, argc, 0, 0);

	ClipMemoryStore->fp = frame.ClipFrame_up_of_ClipFrame;
   if (retp)
      *retp = stack[0];
   else
      _clip_destroy(ClipMemoryStore, stack + 0);

   free(stack);
   return r;
}

CLIP_DLLEXPORT void
_clip_push_func(struct ClipMachine *ClipMemoryStore, ClipFunction f, int nlocals, int nolocals)
{
   ClipVar *sp;
   ClipVar *vp;
   ClipFrame *fp = ClipMemoryStore->fp;

   vp = NEW(ClipVar);

	vp->ClipType_t_of_ClipVar.count_of_ClipType = 1;
	vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CCODE_type_of_ClipVarType;
	vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   vp->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipFunction_function_of_ClipCodeVar = f;

   if (nlocals)
      {
	 ClipVarFrame *localvars = (ClipVarFrame *) calloc(1,
							   sizeof(ClipVarFrame) + nlocals * sizeof(ClipVar));

								localvars->ClipVar_vars_of_ClipVarFrame = (ClipVar *) (localvars + 1);
								memcpy(localvars->ClipVar_vars_of_ClipVarFrame, ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - nlocals, nlocals * sizeof(ClipVar));
								localvars->refcount_of_ClipVarFrame = 1;
								localvars->size_of_ClipVarFrame = nlocals;
	 vp->ClipCodeVar_c_of_ClipVar.ClipVarFrame_uplocals_of_ClipCodeVar = localvars;
	 ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame -= nlocals;
      }
	   else if (!nolocals && fp->ClipVarFrame_localvars_of_ClipFrame && fp->ClipVarFrame_localvars_of_ClipFrame->refcount_of_ClipVarFrame > 0)
      {
			fp->ClipVarFrame_localvars_of_ClipFrame->refcount_of_ClipVarFrame++;
			vp->ClipCodeVar_c_of_ClipVar.ClipVarFrame_uplocals_of_ClipCodeVar = fp->ClipVarFrame_localvars_of_ClipFrame;
      }

sp = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame++;
   CLIP_CHECK_STACK;

   CLEAR_CLIPVAR(sp);
	sp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR_ClipFlags;
	sp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CCODE_type_of_ClipVarType;
   sp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = vp;

}

CLIP_DLLEXPORT int
_clip_func(ClipMachine * ClipMemoryStore, ClipFunction f, int argc, int rest, ClipVarFrame * uplocals)
{
   int r;
   ClipFrame *inMacro = ClipMemoryStore->inMacro;

   ClipMemoryStore->inMacro = 0;
   r = do_call(ClipMemoryStore, f, argc, 0, rest, uplocals);
   ClipMemoryStore->inMacro = inMacro;
   return r;
}

CLIP_DLLEXPORT int
_clip_func_name(struct ClipMachine *ClipMemoryStore, int argc)
{
   ClipFrame *fp = ClipMemoryStore->fp;
	ClipVar *vp = _clip_vptr(fp->ClipVar_sp_of_ClipFrame - argc - 2);
   long hash;
   int ret;

	if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
      hash = _clip_casehashbytes(0, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
	else if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
      {
			if (vp->ClipType_t_of_ClipVar.memo_of_ClipType)
	    hash = (long) rational_toDouble(vp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
	 else
	    hash = (long) vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
      }
   else
      {
	 _clip_trap_str(ClipMemoryStore, __file__, __LINE__, "call by name not from CHARACTER_type_of_ClipVarType or NUMERIC_type_of_ClipVarType value");
	 return _clip_call_errblock(ClipMemoryStore, 1);
      }
   if ((ret = _clip_func_hash(ClipMemoryStore, hash, argc, 0, 0)))
      return ret;
	--fp->ClipVar_sp_of_ClipFrame;
	_clip_destroy(ClipMemoryStore, fp->ClipVar_sp_of_ClipFrame - 1);
	fp->ClipVar_sp_of_ClipFrame[-1] = fp->ClipVar_sp_of_ClipFrame[0];

   return 0;
}

static void
init_machine(ClipMachine * ClipMemoryStore)
{
  /* thread-depend code */
   cur_mp = ClipMemoryStore;
}

CLIP_DLLEXPORT int
_clip_proc(struct ClipMachine *ClipMemoryStore, ClipFunction f, int argc, int rest, ClipVarFrame * uplocals)
{
   int r;
   ClipFrame *inMacro = ClipMemoryStore->inMacro;

   ClipMemoryStore->inMacro = 0;
   r = do_call(ClipMemoryStore, f, argc, 1, rest, uplocals);
   ClipMemoryStore->inMacro = inMacro;
   return r;
}

CLIP_DLLEXPORT int
_clip_register(ClipMachine * ClipMemoryStore, ClipFunction func, const char *name)
{
   return _clip_register_hash(ClipMemoryStore, func, _clip_hashstr(name));
}

CLIP_DLLEXPORT int
_clip_register_hash(ClipMachine * ClipMemoryStore, ClipFunction func, long hash)
{
   NEWVAR(FuncEntry, fp);
   fp->f = func;
   fp->hash = hash;
#if 0
   if (HashTable_insert(ClipMemoryStore->functions, fp, fp->hash))
      return 1;
   else
      {
	 free(fp);
	 return 0;
      }
#else
   HashTable_store(ClipMemoryStore->functions, fp, fp->hash);
   return 1;
#endif
}

int
_clip_register_block(ClipMachine * ClipMemoryStore, ClipBlock block, long hash)
{
   NEWVAR(FuncEntry, fp);
   fp->f = 0;
   fp->block = block;
   fp->hash = hash;
  /*HashTable_remove(ClipMemoryStore->functions, hash); */
  /*HashTable_insert(ClipMemoryStore->functions, fp, fp->hash); */
   HashTable_store(ClipMemoryStore->functions, fp, fp->hash);
  /*if (HashTable_insert(ClipMemoryStore->functions, fp, fp->hash)) */
  /*if (HashTable_store(ClipMemoryStore->functions, fp, fp->hash))
     return 1;
     else
     {
     free(fp);
     return 0;
     }
   */
   return 1;
}

int
_clip_get_function(ClipMachine * ClipMemoryStore, long hash, ClipFunction ** fpp, ClipBlock ** bpp)
{
   ClipFunction *fp;
   FuncEntry *vp;

   vp = (FuncEntry *) HashTable_fetch(ClipMemoryStore->functions, hash);
   if (!vp)
      {
	 fp = init_struct->_clip_builtin(hash);
	 if (fp)
	    {
	       *fpp = fp;
	       *bpp = 0;
	       return 1;
	    }
	 return 0;
      }

   if (vp->f)
      {
	 *fpp = vp->f;
	 *bpp = 0;
      }
   else
      {
	 *fpp = 0;
	 *bpp = &vp->block;
      }

   return 1;
}

CLIP_DLLEXPORT int
_clip_call_errblock(ClipMachine * ClipMemoryStore, int ret)
{
   if (!ret)
      return 0;

   if (!ClipMemoryStore->trapVar)
      _clip_generr(ClipMemoryStore, ret);

   if (ret < 0)
      return ret;

   if (!_clip_errorblock(ClipMemoryStore, ClipMemoryStore->trapVar, ret))
      ret = -ret;
   return ret;
}

CLIP_DLLEXPORT int
_clip_func_hash(struct ClipMachine *ClipMemoryStore, long hash, int argc, int rest, ClipVarFrame * uplocals)
{
   ClipFunction *fp;
   ClipBlock *bp;
   int r = 0;
   ClipFrame *inMacro = ClipMemoryStore->inMacro;

   ClipMemoryStore->inMacro = 0;
   if (!_clip_get_function(ClipMemoryStore, hash, &fp, &bp))
      {
	 char buf[64];
	 VarEntry *vep;
	 vep = fetch_var(ClipMemoryStore, hash);

	if (vep && (vep->ClipVar_var_of_VarEntry.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType ||
vep->ClipVar_var_of_VarEntry.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType))
	    {
	      /* support FoxPro syntax aVar(nElement) */
	       ClipVar *vp;
			 ClipVar *ap = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - argc - 1;
			 _clip_clone(ClipMemoryStore, ap, &vep->ClipVar_var_of_VarEntry);
	       vp = clip_fetch(ClipMemoryStore, argc, 1, 0, 0, 0);
	       if (vp)
		  goto ret;
	    }
		 if (vep && (vep->ClipVar_var_of_VarEntry.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == PCODE_type_of_ClipVarType ||
vep->ClipVar_var_of_VarEntry.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CCODE_type_of_ClipVarType))
	    {
	      /* if function not found - call codeblock from memvar */
	       int _argc = ClipMemoryStore->argc;
	       ClipFrame *fp = ClipMemoryStore->fp;
	       ClipVar *bp = ClipMemoryStore->bp;

	       ClipVar res;
	       memset(&res, 0, sizeof(res));
	       ClipMemoryStore->argc = argc;
			 ClipMemoryStore->bp = fp->ClipVar_sp_of_ClipFrame;
			 r = _clip_eval(ClipMemoryStore, &(vep->ClipVar_var_of_VarEntry), ClipMemoryStore->argc, ARGPTR(ClipMemoryStore, 1), &res);
			 //r = _clip_eval(ClipMemoryStore, &(vep->ClipVar_var_of_VarEntry), ClipMemoryStore->argc,0, &res);
	       ClipMemoryStore->argc = _argc;
	       ClipMemoryStore->fp = fp;
	       ClipMemoryStore->bp = bp;
	       argc++;
	       for (; argc; --argc)
		  {
			  --(fp->ClipVar_sp_of_ClipFrame);
			  _clip_destroy(ClipMemoryStore, fp->ClipVar_sp_of_ClipFrame);
		  }
	       _clip_push(ClipMemoryStore, &res);
	       _clip_destroy(ClipMemoryStore, &res);
	       r = 0;
	       goto ret;
	    }
#if 1
	 {
	   /* call error with args */
	    int _argc = ClipMemoryStore->argc;
	    ClipFrame *fp = ClipMemoryStore->fp;
	    ClipVar *bp = ClipMemoryStore->bp;
	    ClipVar res;
	    memset(&res, 0, sizeof(res));

	    ClipMemoryStore->argc = argc;
		 ClipMemoryStore->bp = fp->ClipVar_sp_of_ClipFrame;
	    _clip_hash_name(ClipMemoryStore, hash, buf, sizeof(buf));
	    r = _clip_trap_err(ClipMemoryStore, EG_NOFUNC, 0, 0, 0, 0, buf);
	    ClipMemoryStore->argc = _argc;
	    ClipMemoryStore->fp = fp;
	    ClipMemoryStore->bp = bp;
	    if (!ClipMemoryStore->trapVar)
	       _clip_generr(ClipMemoryStore, r);
	    _clip_errorblock_res(ClipMemoryStore, ClipMemoryStore->trapVar, r, &res);
	    argc++;
	    for (; argc; --argc)
	       {
				 --(fp->ClipVar_sp_of_ClipFrame);
				 _clip_destroy(ClipMemoryStore, fp->ClipVar_sp_of_ClipFrame);
	       }
	    _clip_push(ClipMemoryStore, &res);
	    _clip_destroy(ClipMemoryStore, &res);
	    r = 0;
	    goto ret;
	 }
#else
	 {
	   /* old code for call error without args */
	    _clip_hash_name(ClipMemoryStore, hash, buf, sizeof(buf));
	    r = _clip_trap_err(ClipMemoryStore, EG_NOFUNC, 0, 0, 0, 0, buf);
	    r = _clip_call_errblock(ClipMemoryStore, r);
	 }
#endif
      }
   if (fp)
      {
	 char *oldbuf = 0, *buf = 0;

	 if (log_level >= 4)
	    {
			 if (ClipMemoryStore->fp->procname_of_ClipFrame)
		  {
		     buf = (char *) calloc(1, 64);
		     _clip_hash_name(ClipMemoryStore, hash, buf, 64);
			  oldbuf = (char *) ClipMemoryStore->fp->procname_of_ClipFrame;
			  ClipMemoryStore->fp->procname_of_ClipFrame = buf;
		  }
	    }

	 r = _clip_func(ClipMemoryStore, fp, argc, rest, uplocals);

	 if (log_level >= 4)
	    {
	       if (oldbuf)
		  {
			  ClipMemoryStore->fp->procname_of_ClipFrame = oldbuf;
		     free(buf);
		  }
	    }

      }
   else
      r = _clip_code_func(ClipMemoryStore, bp, argc, rest, uplocals);

 ret:
   ClipMemoryStore->inMacro = inMacro;
   return r;
}

CLIP_DLLEXPORT int
_clip_proc_hash(struct ClipMachine *ClipMemoryStore, long hash, int argc, int rest, ClipVarFrame * uplocals)
{
   ClipFunction *fp;
   ClipBlock *bp;
   int r = 0;
   ClipFrame *inMacro = ClipMemoryStore->inMacro;

   ClipMemoryStore->inMacro = 0;
   if (!_clip_get_function(ClipMemoryStore, hash, &fp, &bp))
      {
	 char buf[64];

	 _clip_hash_name(ClipMemoryStore, hash, buf, sizeof(buf));
	 r = _clip_trap_err(ClipMemoryStore, EG_NOFUNC, 0, 0, 0, 0, buf);
	 return _clip_call_errblock(ClipMemoryStore, r);
      }

   if (fp)
      {
	 char *oldbuf = 0, *buf = 0;

	 if (log_level >= 4)
	    {
			 if (ClipMemoryStore->fp->procname_of_ClipFrame)
		  {
		     buf = (char *) calloc(1, 64);
		     _clip_hash_name(ClipMemoryStore, hash, buf, 64);
			  oldbuf = (char *) ClipMemoryStore->fp->procname_of_ClipFrame;
			  ClipMemoryStore->fp->procname_of_ClipFrame = buf;
		  }
	    }

	 r = _clip_proc(ClipMemoryStore, fp, argc, rest, uplocals);

	 if (log_level >= 4)
	    {
	       if (oldbuf)
		  {
			  ClipMemoryStore->fp->procname_of_ClipFrame = oldbuf;
		     free(buf);
		  }

	    }
      }
   else
      r = _clip_code_proc(ClipMemoryStore, bp, argc, rest, uplocals);

   ClipMemoryStore->inMacro = inMacro;
   return r;
}

CLIP_DLLEXPORT void
_clip_return(ClipMachine * ClipMemoryStore)
{
	ClipMemoryStore->bp[-ClipMemoryStore->argc - 1] = *(--(ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame));
}

CLIP_DLLEXPORT void
_clip_pop(ClipMachine * ClipMemoryStore)
{
	_clip_destroy(ClipMemoryStore, --(ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame));
}

static void
delete_ClipVarFrame(ClipMachine * ClipMemoryStore, ClipVarFrame * localvars)
{
	if (localvars && localvars->refcount_of_ClipVarFrame > 0 && ((--(localvars->refcount_of_ClipVarFrame)) < 1))
      {
	 int i;

	for (i = 0; i < localvars->size_of_ClipVarFrame; i++)
		_clip_destroy(ClipMemoryStore, localvars->ClipVar_vars_of_ClipVarFrame + i);
	 free(localvars);
      }
}

CLIP_DLLEXPORT void
_clip_destroy_locals(ClipMachine * ClipMemoryStore)
{
	if (ClipMemoryStore->fp->ClipVarFrame_localvars_of_ClipFrame)
      {
			if (ClipMemoryStore->fp->ClipVarFrame_localvars_of_ClipFrame->refcount_of_ClipVarFrame == 0)
	    {
	       int i;

			 for (i = 0; i < ClipMemoryStore->fp->ClipVarFrame_localvars_of_ClipFrame->size_of_ClipVarFrame; i++)
				 _clip_destroy(ClipMemoryStore, ClipMemoryStore->fp->ClipVarFrame_localvars_of_ClipFrame->ClipVar_vars_of_ClipVarFrame + i);
	    }
      }
}

#if 0
static void
resume_mp(ClipMachine * ClipMemoryStore)
{
   ClipFrame *fp = ClipMemoryStore->fp;

   for (; fp->sp > fp->stack;)
      {
	 --(fp->sp);
	 _clip_destroy(ClipMemoryStore, fp->sp);
      }
}
#endif
#define MAX_LOOP_DEEP 1024

static int
calc_loopcount(ClipMachine * ClipMemoryStore, ClipVar * vp, ClipVarFrame * vfp, int deep)
{
   int r = 0, i;

   vp = _clip_vptr(vp);

   if (deep > MAX_LOOP_DEEP)
      return 0;
   deep++;

	switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
      {
      case CCODE_type_of_ClipVarType:
      case PCODE_type_of_ClipVarType:
	 if (vp->ClipCodeVar_c_of_ClipVar.ClipVarFrame_uplocals_of_ClipCodeVar == vfp)
	    r = 1;
	 break;
      case MAP_type_of_ClipVarType:
			for (i = 0; i < vp->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar; i++)
	    {
			 ClipVar *vpp = &(vp->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[i].ClipVar_var_of_ClipVarEl);

	       r += calc_loopcount(ClipMemoryStore, vpp, vfp, deep);
	    }
	 break;
      case ARRAY_type_of_ClipVarType:
	 for (i = 0; i < vp->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar; i++)
	    {
	       ClipVar *vpp = vp->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i;

	       r += calc_loopcount(ClipMemoryStore, vpp, vfp, deep);
	    }
	 break;
      default:
	 break;
      }
   return r;
}

CLIP_DLLEXPORT ClipVar *
_clip_add_tempvar(ClipMachine * ClipMemoryStore, ClipVar * vp)
{
   ClipFrame *fp;
   ClipVarVect *vvp;
   ClipVar *np;

   if (!vp)
      return 0;

   fp = ClipMemoryStore->fp;
	vvp = fp->ClipVarVect_tempvars_of_ClipFrame;
   if (!vvp)
      {
	 vvp = NEW(ClipVarVect);
	fp->ClipVarVect_tempvars_of_ClipFrame = vvp;
      }

vvp->ClipVar_items_of_ClipVarVect = (ClipVar *) realloc(vvp->ClipVar_items_of_ClipVarVect, (vvp->count_of_ClipVarVect + 1) * sizeof(ClipVar));
np = vvp->ClipVar_items_of_ClipVarVect + vvp->count_of_ClipVarVect;
vvp->count_of_ClipVarVect++;

   _clip_clone(ClipMemoryStore, np, vp);

   return np;
}

CLIP_DLLEXPORT void
_clip_resume(ClipMachine * ClipMemoryStore, int nlocals, int nreflocals)
{
   int i;
   ClipFrame *fp = ClipMemoryStore->fp;
	ClipVarFrame *vfp = fp->ClipVarFrame_localvars_of_ClipFrame;

	for (; fp->ClipVar_sp_of_ClipFrame > fp->ClipVar_stack_of_ClipFrame;)
      {
			--(fp->ClipVar_sp_of_ClipFrame);
	_clip_destroy(ClipMemoryStore, fp->ClipVar_sp_of_ClipFrame);
      }

/*      resume_mp(ClipMemoryStore); */

   if (vfp)
      {
			for (i = 0; i < nlocals && i < vfp->size_of_ClipVarFrame; ++i)
				_clip_destroy(ClipMemoryStore, vfp->ClipVar_vars_of_ClipVarFrame + i);

			for (; i < nlocals + nreflocals && i < vfp->size_of_ClipVarFrame; ++i)
	    {
			 ClipVar *vp = vfp->ClipVar_vars_of_ClipVarFrame + i;

/*#if 1
			if ((vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR) && calc_loopcount(ClipMemoryStore, vp, vfp, 0) == vfp->refcount - 1)
			{
					_clip_destroy(ClipMemoryStore, vp);
			}
#endif
*/
#if 1
if ((vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags) && vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar->ClipType_t_of_ClipVar.count_of_ClipType == 1 &&
	(calc_loopcount(ClipMemoryStore, vp, vfp, 0) == vfp->refcount_of_ClipVarFrame - 1))
		  _clip_destroy(ClipMemoryStore, vp);
#else
if ((vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags)
	&& (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType || vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType))
		  {
		     if (vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar->ClipType_t_of_ClipVar.count > 1)
			vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar->ClipType_t_of_ClipVar.count--;
		     else
			_clip_destroy(ClipMemoryStore, vp);
		  }
#endif
	    }

	 delete_ClipVarFrame(ClipMemoryStore, vfp);
      }

if (fp->privates_of_ClipFrame)
      {
			long *p = fp->privates_of_ClipFrame;
	 int num = *p;

	 ++p;
	 remove_private_vect(ClipMemoryStore, num, p);
free(fp->privates_of_ClipFrame);
      }

if (fp->ClipVarVect_tempvars_of_ClipFrame)
      {
			ClipVarVect *vvp = fp->ClipVarVect_tempvars_of_ClipFrame;

			for (i = 0; i < vvp->count_of_ClipVarVect; i++)
		 _clip_destroy(ClipMemoryStore, vvp->ClipVar_items_of_ClipVarVect + i);

	 free(vvp->ClipVar_items_of_ClipVarVect);
	 free(vvp);
fp->ClipVarVect_tempvars_of_ClipFrame = 0;
      }

ClipMemoryStore->fp = ClipMemoryStore->fp->ClipFrame_up_of_ClipFrame;
}

CLIP_DLLEXPORT void
_clip_vresume(ClipMachine * ClipMemoryStore, int num, ClipVar * vect)
{
  /*resume_mp(ClipMemoryStore); */
   for (; num; --num, ++vect)
      _clip_destroy(ClipMemoryStore, vect);
}

CLIP_DLLEXPORT void
_clip_delete(ClipMachine * ClipMemoryStore, ClipVar * vp)
{
   if (!vp)
      return;
   _clip_destroy(ClipMemoryStore, vp);
   free(vp);
}

CLIP_DLLEXPORT void
_clip_destroy(ClipMachine * ClipMemoryStore, ClipVar * vp)
{
   if (!vp)
      return;

	switch (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType)
      {
      case F_MPTR_ClipFlags:
      case F_MREF_ClipFlags:
	 if ((--(vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar->ClipType_t_of_ClipVar.count_of_ClipType)) <= 0)
	    {
	       _clip_destroy(ClipMemoryStore, vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar);
	       free(vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar);
	    }
	 CLEAR_CLIPVAR(vp);
	 return;
      case F_MSTAT_ClipFlags:
	 return;
      case F_NONE_ClipFlags:
	 break;
      }

switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
      {
      case CHARACTER_type_of_ClipVarType:
	 free(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
	 break;
      case NUMERIC_type_of_ClipVarType:
			if (vp->ClipType_t_of_ClipVar.memo_of_ClipType)
	    {
	       rational_destroy(vp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
	    }
	 break;
      case ARRAY_type_of_ClipVarType:
	 {
	    int i;

	    for (i = vp->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar - 1; i >= 0; --i)
	       _clip_destroy(ClipMemoryStore, vp->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i);
	    free(vp->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar);
	 }
	 break;
      case MAP_type_of_ClipVarType:
	 {
	    int i;

#if 1
	    int ind;

	   /* try run `destroy` member */
		if (search_map(vp->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar, vp->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar, HASH_destroy, &ind))
	       {
		  ClipVar *ep;
		  int type;

		ep = &(vp->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[ind].ClipVar_var_of_ClipVarEl);
		  type = _clip_type(ep);

		  if (type == PCODE_type_of_ClipVarType || type == CCODE_type_of_ClipVarType)
		     {
			ClipVar vpp;

			memset(&vpp, 0, sizeof(vpp));
		  vpp.ClipType_t_of_ClipVar = vp->ClipType_t_of_ClipVar;
			vpp.ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR_ClipFlags;
			vpp.ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = vp;
			vp->ClipType_t_of_ClipVar.count_of_ClipType++;
			_clip_eval(ClipMemoryStore, _clip_vptr(ep), 1, &vpp, 0);
		     }
	       }
#endif

for (i = vp->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar - 1; i >= 0; --i)
	_clip_destroy(ClipMemoryStore, &(vp->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[i].ClipVar_var_of_ClipVarEl));
		 free(vp->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar);
	 }
	 break;
      case CCODE_type_of_ClipVarType:

	 delete_ClipVarFrame(ClipMemoryStore, vp->ClipCodeVar_c_of_ClipVar.ClipVarFrame_uplocals_of_ClipCodeVar);
	 break;

      case PCODE_type_of_ClipVarType:

	 delete_ClipVarFrame(ClipMemoryStore, vp->ClipCodeVar_c_of_ClipVar.ClipVarFrame_uplocals_of_ClipCodeVar);
	 destroy_Block(ClipMemoryStore, vp->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipBlock_block_of_ClipCodeVar);
	 free(vp->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipBlock_block_of_ClipCodeVar);

	 break;
      default:
	 break;
      }
   CLEAR_CLIPVAR(vp);
}

CLIP_DLLEXPORT ClipVar *
_clip_ref_destroy(ClipMachine * ClipMemoryStore, ClipVar * vp)
{
  /*if (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType != F_MREF_ClipFlags) */
   _clip_destroy(ClipMemoryStore, vp);
   return vp;
}

int
type_weight(int type)
{
   int r = 0;

   switch (type)
      {
      case ARRAY_type_of_ClipVarType:
	 r = 0;
	 break;
      case CCODE_type_of_ClipVarType:
      case PCODE_type_of_ClipVarType:
	 r = 1;
	 break;
      case CHARACTER_type_of_ClipVarType:
	 r = 2;
	 break;
      case LOGICAL_type_of_ClipVarType:
	 r = 3;
	 break;
      case DATE_type_of_ClipVarType:
	 r = 4;
	 break;
      case DATETIME_type_of_ClipVarType:
	 r = 5;
      case NUMERIC_type_of_ClipVarType:
	 r = 6;
	 break;
      case UNDEF_type_of_ClipVarType:
      default:
	 r = 7;
	 break;
      }
   return r;
}

CLIP_DLLEXPORT int
_clip_cmp(ClipMachine * ClipMemoryStore, ClipVar * lval, ClipVar * rval, int *res, int bytype)
{
   ClipVar *Rval = rval, *Lval = lval;
   int ltype, rtype;
   int l = 0;

   Rval = _clip_vptr(Rval);
   Lval = _clip_vptr(Lval);

   ltype = _clip_type(Lval), rtype = _clip_type(Rval);

   if (bytype && ltype != rtype)
      {
	 int wl, wr;

	 wl = type_weight(ltype);
	 wr = type_weight(rtype);
	 if (wl < wr)
	    l = -1;
	 else if (wl > wr)
	    l = 1;
	 else
	    l = 0;
      }
   else if (ltype == CHARACTER_type_of_ClipVarType && rtype == CHARACTER_type_of_ClipVarType)
      {
	 int rrlen = Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
	 int rlen = rrlen;
	 int lllen = Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
	 int llen = lllen;
	 char *s;

	 for (s = Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf + rlen - 1; rlen; --rlen, --s)
	    if (*s != ' ' /* && *s != '\n' */ )
	       break;

	 if (!(ClipMemoryStore->flags & EXACT_FLAG))
	    {			/* SET EXACT OFF - empty rstr, len must match for equal */
	       if (rrlen == 0)
		  l = 0;
#if 1
	       else if (lllen == 0 && rrlen > 0)
		  l = -1;
	       else		/* if ( rrlen > lllen ) */
		  {
		     if (lllen > rrlen)
			lllen = rrlen;
		     l = _clip_strnncmp(Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, lllen, rrlen);
		  }
#else
	       else if (llen < rrlen)
		  {
		     l = _clip_strncmp(Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, llen);
		     if (!l)
			l = -1;
		  }
	       else
		  l = _clip_strncmp(Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, rrlen);
#endif
	    }
	 else
	   /* SET EXACT ON - all but trailing spaces */
	    {
	       if (llen == 0 && rlen > 0 /* llen > rlen */ )
		  l = -1;
	       else
		  {
		     for (s = Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf + rlen - 1; rlen && *s == ' '; --rlen, --s)
			;
		     for (s = Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf + llen - 1; llen && *s == ' '; --llen, --s)
			;
		     if (rlen == 0 && llen > 0)
			l = 1;
		     else if (rlen > 0 && llen == 0)
			l = -1;
		     else
			l = _clip_strnncmp(Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, llen, rlen);
		  }
	    }
      }
   else if (ltype == DATE_type_of_ClipVarType && rtype == DATE_type_of_ClipVarType)
      {
			l = (Lval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar < Rval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar ? -1 : (Lval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar >
			Rval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar ? 1 : 0));
      }
   else if (ltype == DATETIME_type_of_ClipVarType && rtype == DATETIME_type_of_ClipVarType)
      {
			l = (Lval->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar < Rval->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar ? -1 :
(Lval->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar > Rval->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar ? 1 : 0));
	 if (l == 0)
		 l = (Lval->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar < Rval->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar ? -1 :
(Lval->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar > Rval->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar ? 1 : 0));
      }
   else if (ltype == NUMERIC_type_of_ClipVarType && rtype == NUMERIC_type_of_ClipVarType)
      {
			if (!Lval->ClipType_t_of_ClipVar.memo_of_ClipType && !Rval->ClipType_t_of_ClipVar.memo_of_ClipType)
	    {
	       double diff;

	       diff = fabs(Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar - Rval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
	       if (diff < (Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar * MIN_NUMERIC_DIFF))
		  l = 0;
	       else if (Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar < Rval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar)
		  l = -1;
	       else if (Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar > Rval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar)
		  l = 1;
	       else
		  l = 0;
	      /*
	         l = (Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar < Rval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar ? -1 : (Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar >
Rval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar ? 1 : 0));
	       */
	    }
	 else
	    {
			 if (Lval->ClipType_t_of_ClipVar.memo_of_ClipType && Rval->ClipType_t_of_ClipVar.memo_of_ClipType)
		  l = rational_cmp(Lval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, Rval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
	       else
		  {
		     rational *tmp;

			  if (Lval->ClipType_t_of_ClipVar.memo_of_ClipType)
			{
			   tmp = rational_double_init(Rval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
			   l = rational_cmp(Lval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, tmp);
			}
		     else
			{
			   tmp = rational_double_init(Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
			   l = rational_cmp(tmp, Rval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
			}
		     rational_destroy(tmp);
		  }
	    }
      }
   else if (ltype == LOGICAL_type_of_ClipVarType && rtype == LOGICAL_type_of_ClipVarType)
      {
			if (Lval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar && !Rval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
	    l = 1;
		 else if (!Lval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar && Rval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
	    l = -1;
	 else
	    l = 0;
      }
   else if (ltype == ARRAY_type_of_ClipVarType && rtype == ARRAY_type_of_ClipVarType)
      {
	 l = (Lval != Rval);
      }
   else if (ltype == MAP_type_of_ClipVarType && rtype == MAP_type_of_ClipVarType)
      {
	 l = (Lval != Rval);
      }
   else if (ltype == OBJECT_type_of_ClipVarType && rtype == OBJECT_type_of_ClipVarType)
      {
			l = (Lval->ClipObjVar_o_of_ClipVar.obj_of_ClipObjVar != Rval->ClipObjVar_o_of_ClipVar.obj_of_ClipObjVar);
      }
   else if (ltype == CCODE_type_of_ClipVarType && rtype == CCODE_type_of_ClipVarType)
      {
	 l = (Lval->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipFunction_function_of_ClipCodeVar != Rval->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipFunction_function_of_ClipCodeVar);
      }
   else if (ltype == PCODE_type_of_ClipVarType && rtype == PCODE_type_of_ClipVarType)
      {
	 l = (Lval->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipBlock_block_of_ClipCodeVar != Rval->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipBlock_block_of_ClipCodeVar);
      }
   else if ((ltype == PCODE_type_of_ClipVarType && rtype == CCODE_type_of_ClipVarType) || (ltype == CCODE_type_of_ClipVarType && rtype == PCODE_type_of_ClipVarType))
      {
	 l = 0;
      }
   else if (ltype == UNDEF_type_of_ClipVarType)
      {
	 l = (rtype == UNDEF_type_of_ClipVarType ? 0 : -1);
      }
   else if (rtype == UNDEF_type_of_ClipVarType)
      {
	 l = (ltype == UNDEF_type_of_ClipVarType ? 0 : 1);
      }
   else
      {
	 return _clip_trap_operation(ClipMemoryStore, __file__, __LINE__, "==", Lval);
	/*return _clip_call_errblock(ClipMemoryStore, 1); */
      }

   *res = l;
   return 0;
}

const char *
_clip_strstr(const char *src, int slen, const char *dst, int dlen)
{
   const char *end, *de, *s = src, *d = dst;

  /*
     if(!dlen)
     return src;
   */
   for (end = src + slen, de = dst + dlen - 1; src < end; src++)
      if (*src == *d)
	 {
	    s = src;
	    for (; src < end && d < de && *(++src) == *(++d);)
	       ;
	    if (d == de && *src == *d)
	       return s;
	    src = s;
	    d = dst;
	 }
   return 0;
}

static int
do_assign(ClipMachine * ClipMemoryStore, ClipVar * lval, ClipVar * rval, int op)
{
   int ret = 0;
   int l;
   ClipVar *Rval = rval, *Lval = lval;
   int ltype, rtype;

   Rval = _clip_vptr(Rval);
   Lval = _clip_vptr(Lval);

   ltype = _clip_type(Lval);
   rtype = _clip_type(Rval);

   if (ltype == MAP_type_of_ClipVarType)
      {
	 int ind;
	 long hash = 0;

	 switch (op)
	    {
	    case '+':
	       hash = HASH_operator_add;
	       break;
	    case '-':
	       hash = HASH_operator_sub;
	       break;
	    case '*':
	       hash = HASH_operator_mul;
	       break;
	    case '/':
	       hash = HASH_operator_div;
	       break;
	    case '%':
	       hash = HASH_operator_mod;
	       break;
	    case '^':
	       hash = HASH_operator_pow;
	       break;
	    case '|':
	       hash = HASH_operator_or;
	       break;
	    case '&':
	       hash = HASH_operator_and;
	       break;
	    case '$':
	       hash = HASH_operator_in;
	       break;
	    case 'e':
	       hash = HASH_operator_eq;
	       break;
	    case 'E':
	       hash = HASH_operator_eeq;
	       break;
	    case 'N':
	       hash = HASH_operator_neq;
	       break;
	    case '<':
	       hash = HASH_operator_lt;
	       break;
	    case '>':
	       hash = HASH_operator_gt;
	       break;
	    case 'L':
	       hash = HASH_operator_le;
	       break;
	    case 'G':
	       hash = HASH_operator_ge;
	       break;
	    }

	 if (hash)
	    {
	       ClipVar *vp = Lval;

	      /* try run `destroy` member */
			if (search_map(vp->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar, vp->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar, hash, &ind))
		  {
		     ClipVar *ep;
		     int type;

			  ep = &(vp->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[ind].ClipVar_var_of_ClipVarEl);
		     type = _clip_type(ep);

		     if (type == PCODE_type_of_ClipVarType || type == CCODE_type_of_ClipVarType)
			{
			   int i;
			   ClipVar stack[2], res;

			   memset(stack, 0, sizeof(stack));
			   memset(&res, 0, sizeof(res));

			   stack[0].ClipType_t_of_ClipVar = vp->ClipType_t_of_ClipVar;
			   stack[0].ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR_ClipFlags;
				stack[0].ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = vp;
				vp->ClipType_t_of_ClipVar.count_of_ClipType++;

			   _clip_clone(ClipMemoryStore, stack + 1, rval);

			   ret = _clip_eval(ClipMemoryStore, _clip_vptr(ep), 2, stack, &res);

			   _clip_clone(ClipMemoryStore, lval, &res);

			   for (i = 0; i < 2; i++)
			      _clip_destroy(ClipMemoryStore, stack + i);

			   _clip_destroy(ClipMemoryStore, &res);
			}
		     return ret;
		  }
	    }
      }

   switch (op)
      {
      case '+':
	 if (ltype == CHARACTER_type_of_ClipVarType && rtype == CHARACTER_type_of_ClipVarType)
	    {
	       int len = Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf + Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;

			 if (Lval->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType == F_MSTAT_ClipFlags)
		  {
		     char *s = (char *) malloc(len + 1);

			  Lval->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
		     memcpy(s, Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
		     Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = s;
		  }
	       else
		  Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = (char *) realloc(Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, len + 1);
	       memcpy(Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf + Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf,
Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf + 1);
	       Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = len;
			 Lval->ClipType_t_of_ClipVar.MACRO = Rval->ClipType_t_of_ClipVar.MACRO;
	    }
	 else if (ltype == NUMERIC_type_of_ClipVarType && rtype == NUMERIC_type_of_ClipVarType)
	    {
	       _clip_math_operation(ClipMemoryStore, op, Lval, Rval);
	    }
	 else if (ltype == DATE_type_of_ClipVarType && rtype == NUMERIC_type_of_ClipVarType)
	    {
			 if (Rval->ClipType_t_of_ClipVar.memo_of_ClipType)
				 Lval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar += rational_toDouble(Rval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
	       else
				 Lval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar += Rval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
	    }
	 else if (ltype == NUMERIC_type_of_ClipVarType && rtype == DATE_type_of_ClipVarType)
	    {
	       double tmp;

			 if (Lval->ClipType_t_of_ClipVar.memo_of_ClipType)
		  tmp = rational_toDouble(Lval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
	       else
		  tmp = Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
		  Lval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATE_type_of_ClipVarType;
		  Lval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = Rval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar;
		  Lval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar += tmp;
	    }
	 else if (ltype == DATETIME_type_of_ClipVarType && rtype == NUMERIC_type_of_ClipVarType)
	    {
	       long tmp;

			 if (Rval->ClipType_t_of_ClipVar.memo_of_ClipType)
		  tmp = (long) rational_toDouble(Rval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
	       else
		  tmp = (long) Rval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
		  Lval->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar += tmp / (24 * 60 * 60);
		  Lval->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar += (tmp % (24 * 60 * 60)) * 1000;
	    }
	 else if (ltype == NUMERIC_type_of_ClipVarType && rtype == DATETIME_type_of_ClipVarType)
	    {
	       long tmp;

			 if (Lval->ClipType_t_of_ClipVar.memo_of_ClipType)
		  tmp = (long) rational_toDouble(Lval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
	       else
		  tmp = (long) Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
		  Lval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATETIME_type_of_ClipVarType;
		  Lval->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar = Rval->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar + tmp / (24 * 60 * 60);
		  Lval->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar = Rval->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar + (tmp % (24 * 60 * 60)) * 1000;
	    }
	 else
	    {
	       return _clip_trap_operation(ClipMemoryStore, __file__, __LINE__, "+", Lval);
	      /*return 1; */
	    }
	 break;
      case '-':
	 if (ltype == CHARACTER_type_of_ClipVarType && rtype == CHARACTER_type_of_ClipVarType)
	    {
	       int llen = Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
	       int len, slen = 0;
	       char *s;

	       for (s = Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf + llen - 1; llen; --llen, --s, ++slen)
		  if (*s != ' ' && *s != '\t')
		     break;
	       len = llen + Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;

	       if (Lval->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType == F_MSTAT_ClipFlags)
		  {
		     char *s = (char *) malloc(len + 1);

			  Lval->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
		     memcpy(s, Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
		     Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = s;
		  }
	       else
		  Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = (char *) realloc(Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, len + slen + 1);
	       memcpy(Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf + llen, Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf,
Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf + 1);
	       memset(Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf + len, ' ', slen);
	       Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[len + slen] = 0;
	       Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = len + slen;
			 Lval->ClipType_t_of_ClipVar.MACRO = Rval->ClipType_t_of_ClipVar.MACRO;
	    }
	 else if (ltype == NUMERIC_type_of_ClipVarType && rtype == NUMERIC_type_of_ClipVarType)
	    {
	       _clip_math_operation(ClipMemoryStore, op, Lval, Rval);
	    }
	 else if (ltype == DATE_type_of_ClipVarType && rtype == NUMERIC_type_of_ClipVarType)
	    {
			 if (Rval->ClipType_t_of_ClipVar.memo_of_ClipType)
				 Lval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar -= rational_toDouble(Rval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
	       else
				 Lval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar -= Rval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
	    }
	 else if (ltype == DATETIME_type_of_ClipVarType && rtype == DATETIME_type_of_ClipVarType)
	    {
			 long tmp = (Lval->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar - Rval->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar) * 24 * 60 * 60;

			 tmp += (Lval->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar - Rval->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar) / 1000;

	       Lval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	       Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = tmp;
	    }
	 else if (ltype == DATETIME_type_of_ClipVarType && rtype == NUMERIC_type_of_ClipVarType)
	    {
	       long tmp;

			 if (Rval->ClipType_t_of_ClipVar.memo_of_ClipType)
		  tmp = (long) rational_toDouble(Rval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
	       else
		  tmp = (long) Rval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
		  Lval->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar -= tmp / (24 * 60 * 60);
		  Lval->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar -= (tmp % (24 * 60 * 60)) * 1000;
		  if (Lval->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar < 0)
		  {
			  Lval->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar--;
			  Lval->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar = (24 * 60 * 60 * 1000) + Lval->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar;
		  }
	    }
	 else if (ltype == NUMERIC_type_of_ClipVarType && rtype == DATE_type_of_ClipVarType)
	    {
	       double tmp;

			 if (Lval->ClipType_t_of_ClipVar.memo_of_ClipType)
		  tmp = rational_toDouble(Lval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
	       else
		  tmp = Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
		  Lval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATE_type_of_ClipVarType;
		  Lval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = Rval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar;
		  Lval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar -= tmp;
	    }
	 else if (ltype == DATE_type_of_ClipVarType && rtype == DATE_type_of_ClipVarType)
	    {
			 Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = Lval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar - Rval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar;
			 Lval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
			 Lval->ClipType_t_of_ClipVar.len_of_ClipType = 0;
			 Lval->ClipType_t_of_ClipVar.dec_of_ClipType = 0;
			 Lval->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	    }
	 else
	    {
	       return _clip_trap_operation(ClipMemoryStore, __file__, __LINE__, "-", Lval);
	      /*return 1; */
	    }
	 break;
      case '*':
	 if (ltype == NUMERIC_type_of_ClipVarType && rtype == NUMERIC_type_of_ClipVarType)
	    {
	       _clip_math_operation(ClipMemoryStore, op, Lval, Rval);
	    }
	 else
	    {
	       return _clip_trap_operation(ClipMemoryStore, __file__, __LINE__, "*", Lval);
	      /*return 1; */
	    }
	 break;
      case '/':
	 if (ltype == NUMERIC_type_of_ClipVarType && rtype == NUMERIC_type_of_ClipVarType)
	    {
	       _clip_math_operation(ClipMemoryStore, op, Lval, Rval);
	    }
	 else
	    {
	       return _clip_trap_operation(ClipMemoryStore, __file__, __LINE__, "/", Lval);
	      /*return 1; */
	    }
	 break;
      case '%':
	 if (ltype == NUMERIC_type_of_ClipVarType && rtype == NUMERIC_type_of_ClipVarType)
	    {
			 if (!Lval->ClipType_t_of_ClipVar.memo_of_ClipType && !Rval->ClipType_t_of_ClipVar.memo_of_ClipType)
		  {
		     Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = fmod(Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, Rval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
			  Lval->ClipType_t_of_ClipVar.len_of_ClipType = 0;
			  Lval->ClipType_t_of_ClipVar.dec_of_ClipType = 0;
			  Lval->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
		  }
	       else
		  {
		     rational *tmp = 0;

			  if (Lval->ClipType_t_of_ClipVar.memo_of_ClipType && Rval->ClipType_t_of_ClipVar.memo_of_ClipType)
			{
			   tmp = rational_fmod(Lval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, Rval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
			}
		     else
			{
				if (Lval->ClipType_t_of_ClipVar.memo_of_ClipType)
			      {
				 rational *tmp1;

				 tmp1 = rational_double_init(Rval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
				 tmp = rational_fmod(Lval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, tmp1);
				 rational_destroy(tmp1);
			      }
			   else
			      {
				 rational *tmp1;

				 tmp1 = rational_double_init(Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
				 tmp = rational_fmod(tmp1, Lval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
				 rational_destroy(tmp1);
			      }
			}
		     rational_destroy(Lval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
		     Lval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar = tmp;
		  }

	    }
	 else
	    {
	       return _clip_trap_operation(ClipMemoryStore, __file__, __LINE__, "%", Lval);
	      /*return 1; */
	    }
	 break;
      case '^':
	 if (ltype == NUMERIC_type_of_ClipVarType && rtype == NUMERIC_type_of_ClipVarType)
	    {
			 if (Lval->ClipType_t_of_ClipVar.memo_of_ClipType)
		  {
		     long tmp;

			  if (Rval->ClipType_t_of_ClipVar.memo_of_ClipType)
			tmp = (long) rational_toDouble(Rval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
		     else
			tmp = (long) Rval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
		     rational_powa(Lval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, tmp);
		  }
	       else
		  {
		     Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = pow(Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, Rval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
		     if (!finite(Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar))
			Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = 0;
			Lval->ClipType_t_of_ClipVar.len_of_ClipType = 0;
			Lval->ClipType_t_of_ClipVar.dec_of_ClipType = 0;
		  }
	    }
	 else
	    {
	       return _clip_trap_operation(ClipMemoryStore, __file__, __LINE__, "**", Lval);
	      /*return 1; */
	    }
	 break;
      case '$':
	 if (ltype == CHARACTER_type_of_ClipVarType && rtype == CHARACTER_type_of_ClipVarType)
	    {
	       const char *s = _clip_strstr(Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf,
					    Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);

	       _clip_destroy(ClipMemoryStore, lval);
			 lval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = (s ? 1 : 0);
			 lval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
	    }
	 else if (rtype == MAP_type_of_ClipVarType)
	    {
	       int ind, r;
	       long no;

	       switch (ltype)
		  {
		  case NUMERIC_type_of_ClipVarType:
			  if (Lval->ClipType_t_of_ClipVar.memo_of_ClipType)
			no = rational_toDouble(Lval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
		     else
			no = Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
		     break;
		  case CHARACTER_type_of_ClipVarType:
		    /*no = _clip_casehashbytes(0, Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf); */
		     no = _clip_hashbytes(0, Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
		     break;
		  default:
		     goto inv_in_arg;
		  }

r = search_map(Rval->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar, Rval->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar, no, &ind);
	       _clip_destroy(ClipMemoryStore, lval);
			 lval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = (r ? 1 : 0);
			 lval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
	    }
	 else if (rtype == ARRAY_type_of_ClipVarType)
	    {
	       int i, c, r, res = 0;

	       c = Rval->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;

	       for (i = 0; i < c; ++i)
		  {
		     ClipVar *vp = Rval->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i;

		     r = _clip_cmp(ClipMemoryStore, Lval, vp, &res, 1);
		     if (r)
			continue;
		     if (res)
			break;
		  }
	       _clip_destroy(ClipMemoryStore, lval);
			 lval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = (!res ? 1 : 0);
			 lval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
	    }
	 else
	    {
	     inv_in_arg:
	       return _clip_trap_operation(ClipMemoryStore, __file__, __LINE__, "$", Lval);
	      /*return 1; */
	    }
	 break;
      case 'E':
      case 'e':
      case 'N':
#if 1				/* uri added : oper != work as =, not == */
	 if ((op == 'E') && (ltype == CHARACTER_type_of_ClipVarType && rtype == CHARACTER_type_of_ClipVarType))
#else
	 if ((op == 'E' || op == 'N') && (ltype == CHARACTER_type_of_ClipVarType && rtype == CHARACTER_type_of_ClipVarType))
#endif
#if 1
	   /*l = !strcmp(Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf); */
	    {
	       int ll, lr;

	       ll = Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
	       lr = Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;

	       if (ll != lr)
		  l = 0;
	       else
		  l = !memcmp(Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, ll);
	    }
#else
/*Rust begin */
	    {
	       int len = Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf < Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf ?
Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf : Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;

	       l = !_clip_strncmp(Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, len);
	    }
/*Rust end */
#endif
	 else
	    {
	       if ((ret = _clip_cmp(ClipMemoryStore, lval, rval, &l, 0)))
		  return ret;
	       l = !l;
	    }

	 _clip_destroy(ClipMemoryStore, lval);
	 switch (op)
	    {
	    case 'E':
	    case 'e':
			 lval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = l;
	       break;
	    default:
			 lval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = !l;
	       break;
	    }
		 lval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
	 break;
      case '|':
	 if (ltype == LOGICAL_type_of_ClipVarType && rtype == LOGICAL_type_of_ClipVarType)
	    {
			 l = (Lval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar || Rval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar);
	    }
	 else
	    {
	       return _clip_trap_operation(ClipMemoryStore, __file__, __LINE__, "OR", lval);
	      /*return 1; */
	    }
		 lval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = l;
	 lval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
	 break;
      case '&':
	 if (ltype == LOGICAL_type_of_ClipVarType && rtype == LOGICAL_type_of_ClipVarType)
	    {
			 l = (Lval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar && Rval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar);
	    }
	 else
	    {
	       return _clip_trap_operation(ClipMemoryStore, __file__, __LINE__, "AND", lval);
	      /*return 1; */
	    }
		 lval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = l;
	 lval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
	 break;
      case '<':
      case '>':
      case 'L':
      case 'G':
	 if (ltype == CHARACTER_type_of_ClipVarType && rtype == CHARACTER_type_of_ClipVarType)
	    {
	       l = _clip_strnncmp(Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf,
Lval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, Rval->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
#if 1				/* added 20030118 for test '? "" <="0"' */
	       ret = _clip_cmp(ClipMemoryStore, lval, rval, &l, 0);
#endif
	      //printf("\nl=%d,ret=%d\n",l,ret);
	       _clip_destroy(ClipMemoryStore, lval);
	    }
	 else if (ltype == DATE_type_of_ClipVarType && rtype == DATE_type_of_ClipVarType)
	    {
			 if (Lval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar < Rval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar)
		  l = -1;
		  else if (Lval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar > Rval->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar)
		  l = 1;
	       else
		  l = 0;
	    }
	 else if (ltype == DATETIME_type_of_ClipVarType && rtype == DATETIME_type_of_ClipVarType)
	    {
			 if (Lval->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar < Rval->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar)
		  l = -1;
		  else if (Lval->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar > Rval->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar)
		  l = 1;
	       else
		  l = 0;
	       if (l == 0)
		  {
			  if (Lval->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar < Rval->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar)
			l = -1;
			else if (Lval->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar > Rval->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar)
			l = 1;
		     else
			l = 0;
		  }
	    }
	 else if (ltype == NUMERIC_type_of_ClipVarType && rtype == NUMERIC_type_of_ClipVarType)
	    {
			 if (!Lval->ClipType_t_of_ClipVar.memo_of_ClipType && !Rval->ClipType_t_of_ClipVar.memo_of_ClipType)
		  {
		     double diff;

		     diff = fabs(Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar - Rval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
		     if (diff < (Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar * MIN_NUMERIC_DIFF))
			l = 0;
		     else if (Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar < Rval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar)
			l = -1;
		     else if (Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar > Rval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar)
			l = 1;
		     else
			l = 0;
		  }
	       else
		  {
			  if (Lval->ClipType_t_of_ClipVar.memo_of_ClipType && Rval->ClipType_t_of_ClipVar.memo_of_ClipType)
			l = rational_cmp(Lval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, Rval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
		     else
			{
			   rational *tmp = NULL;

				if (Lval->ClipType_t_of_ClipVar.memo_of_ClipType)
			      {
				 tmp = rational_double_init(Rval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
				 l = rational_cmp(Lval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, tmp);
			      }
			   else
			      {
				 tmp = rational_double_init(Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
				 l = rational_cmp(tmp, Rval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
			      }
			   rational_destroy(tmp);
			}
		  }
	       _clip_destroy(ClipMemoryStore, lval);
	    }
	 else if (ltype == LOGICAL_type_of_ClipVarType && rtype == LOGICAL_type_of_ClipVarType)
	    {
			 if (Lval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar && !Rval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
		  l = -1;
		  else if (!Lval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar && Rval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
		  l = 1;
	       else
		  l = 0;
	    }
	 else if (ltype == OBJECT_type_of_ClipVarType && rtype == OBJECT_type_of_ClipVarType)
	    {
			 l = Lval->ClipObjVar_o_of_ClipVar.rtti_of_ClipObjVar->cmp_of_ClipObjRtti(ClipMemoryStore, Lval->ClipObjVar_o_of_ClipVar.obj_of_ClipObjVar, Lval->ClipObjVar_o_of_ClipVar.rtti_of_ClipObjVar,
Rval);
	       _clip_destroy(ClipMemoryStore, lval);
	    }
	 else
	    {
	       return _clip_trap_operation(ClipMemoryStore, __file__, __LINE__, "==", lval);
	      /*return 1; */
	    }
	 switch (op)
	    {
	    case 'L':
			 lval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = (l <= 0 ? 1 : 0);
	       break;
	    case 'G':
			 lval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = (l >= 0 ? 1 : 0);
	       break;
	    case '>':
			 lval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = (l > 0 ? 1 : 0);
	       break;
	    case '<':
			 lval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = (l < 0 ? 1 : 0);
	       break;
	    default:
			 lval->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = l;
	    }
		 lval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
		 lval->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	 break;
      }

   return ret;
}

CLIP_DLLEXPORT int
_clip_op(ClipMachine * ClipMemoryStore, int op)
{
   int ret;
	ClipVar *rval = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - 1;
	ClipVar *lval = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - 2;

   if ((ret = do_assign(ClipMemoryStore, lval, rval, op)))
      return ret;
  /*return _clip_call_errblock(ClipMemoryStore, ret); */
   _clip_destroy(ClipMemoryStore, rval);
	--ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame;
   return 0;
}

CLIP_DLLEXPORT int
_clip_not(ClipMachine * ClipMemoryStore)
{
	ClipVar *vp = _clip_vptr(ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - 1);

	if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
      {
	 _clip_trap_str(ClipMemoryStore, __file__, __LINE__, "non-logical type in NOT operator");
	 return _clip_call_errblock(ClipMemoryStore, 1);
      }
	   vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = !vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
   return 0;
}

CLIP_DLLEXPORT int
_clip_minus(ClipMachine * ClipMemoryStore)
{
	ClipVar *vp = _clip_vptr(ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - 1);

	if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != NUMERIC_type_of_ClipVarType)
      {
	 _clip_trap_str(ClipMemoryStore, __file__, __LINE__, "non-numeric type in MINUS operator");
	 return _clip_call_errblock(ClipMemoryStore, 1);
      }
	   if (vp->ClipType_t_of_ClipVar.memo_of_ClipType)
      rational_inverse(vp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
   else
      vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = -vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
   return 0;
}

CLIP_DLLEXPORT int
_clip_opassign(struct ClipMachine *ClipMemoryStore, void *Lval, int op)
{
   int ret;
	ClipVar *rval = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - 1;

   if ((ret = do_assign(ClipMemoryStore, (ClipVar *) Lval, rval, op)))
      return ret;
   _clip_destroy(ClipMemoryStore, rval);
	--ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame;
   return 0;
}

CLIP_DLLEXPORT int
_clip_opiassign(struct ClipMachine *ClipMemoryStore, void *Lval, int op)
{
   int ret;
	ClipVar *rval = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - 1;
   ClipVar *lval = (ClipVar *) Lval;

	if ((ret = do_assign(ClipMemoryStore, lval, ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - 1, op)))
      return ret;
   _clip_destroy(ClipMemoryStore, rval);
   if ((ret = _clip_mclone(ClipMemoryStore, rval, lval)))
      return ret;
   return 0;
}

CLIP_DLLEXPORT int
_clip_assign(struct ClipMachine *ClipMemoryStore, void *Lval)
{
	ClipVar *rval = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - 1;
   ClipVar *lval = (ClipVar *) Lval;
   int ret;

   if (lval)
      {
			if ((lval->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags) && lval->ClipType_t_of_ClipVar.field_of_ClipType && lval->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar)
	    {
	       return _clip_assign_field(ClipMemoryStore, lval->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar->fieldhash_of_ClipFieldDef,
lval->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar->areahash_of_ClipFieldDef);
	    }
	 else
	    {
			 if (!(lval->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags) && (rval->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType == F_MREF_ClipFlags)
				 && lval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType
			    && !(rval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType || rval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType))
		  {
		     _clip_dup(ClipMemoryStore, lval, _clip_vptr(rval));
		  }
#if 0
	       else if (lval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType || lval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
		  {
		     _clip_destroy(ClipMemoryStore, lval);
		     ret = _clip_mclone(ClipMemoryStore, lval, rval);
		  }
#endif
	       else if ((ret = _clip_mclone(ClipMemoryStore, lval, rval)))
		  return ret;
	    }
		 lval->ClipType_t_of_ClipVar.field_of_ClipType = 0;
		 if (lval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
			 lval->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
      }

   _clip_destroy(ClipMemoryStore, rval);
	ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame--;
   return 0;
}

CLIP_DLLEXPORT int
_clip_refassign(ClipMachine * ClipMemoryStore, void *Lval, int isTop)
{
	ClipVar *rval = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - 1;
   ClipVar *lval = (ClipVar *) Lval;

   _clip_destroy(ClipMemoryStore, lval);

	if (!(rval->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags))
      return 1;
   *lval = *rval;
   if (isTop)
		ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame--;
   else
      {
	 rval->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar->ClipType_t_of_ClipVar.count_of_ClipType++;
      }

   return 0;
}

CLIP_DLLEXPORT int
_clip_iassign(struct ClipMachine *ClipMemoryStore, void *Lval)
{
   int ret;
	ClipVar *rval = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - 1;
   ClipVar *lval = (ClipVar *) Lval;

	if ((lval->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags) && lval->ClipType_t_of_ClipVar.field_of_ClipType && lval->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar)
      {
	 ret = _clip_iassign_field(ClipMemoryStore, lval->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar->fieldhash_of_ClipFieldDef,
lval->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar->areahash_of_ClipFieldDef);
      }
   else
      {
			if (!(lval->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags) && (rval->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType == F_MREF_ClipFlags)
				&& lval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType
		      && !(rval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType || rval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType))
	    {
	       ret = _clip_dup(ClipMemoryStore, lval, _clip_vptr(rval));
	    }
	 else
	    ret = _clip_mclone(ClipMemoryStore, lval, rval);
	 lval->ClipType_t_of_ClipVar.field_of_ClipType = 0;
	if (lval->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
		lval->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
      }

   return ret;
}

static ClipVar *
find_lval(ClipMachine * ClipMemoryStore, char *name, int namelen)
{
   VarEntry *vep;
   ClipVar *lval = 0;
   long hash;
   char *fname = 0;
   int fl = 0, r, ndim = 0;
   long *dim = 0;

   r = _clip_parse_name(ClipMemoryStore, name, namelen, 0, 0, &fname, &fl, &dim, &ndim);
   if (r == 2)			/* field */
      return 0;

   hash = _clip_casehashbytes(0, fname, fl);
   vep = fetch_var(ClipMemoryStore, hash);

   if (vep)
		lval = &vep->ClipVar_var_of_VarEntry;

   if (lval && r == 3)
      {
	 lval = _clip_aref(ClipMemoryStore, lval, ndim, dim);
	 free(dim);
      }

   return lval;
}

CLIP_DLLEXPORT int
_clip_macroassign(ClipMachine * ClipMemoryStore, int op, int isTop, int fieldprec)
{
   int r;
	ClipVar *rval = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - 2;
	ClipVar *vp = _clip_vptr(ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - 1);
   ClipVar *lval = 0;

	if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType)
      {
	 _clip_trap_printf(ClipMemoryStore, __file__, __LINE__, "macro assign with non-character argument (%s)", _clip_typename(vp));
	 return _clip_call_errblock(ClipMemoryStore, 1);
      }

   if (op == '=')
      r = _clip_nameassign(ClipMemoryStore, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, rval,
fieldprec);
   else
      {
	 lval = find_lval(ClipMemoryStore, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);

	 if (!lval)
	    {
	       _clip_trap_printf(ClipMemoryStore, __file__, __LINE__, "no variable name: '%.*s'", vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf,
vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
	       return _clip_call_errblock(ClipMemoryStore, 1);
	    }

	 if (op == '=')
	    r = _clip_mclone(ClipMemoryStore, lval, rval);
	 else
	    r = do_assign(ClipMemoryStore, lval, rval, op);
      }

   if (r)
      return r;

	_clip_destroy(ClipMemoryStore, --(ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame));

   if (isTop)
		_clip_destroy(ClipMemoryStore, --(ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame));

   return 0;
}

CLIP_DLLEXPORT void
_clip_pop_fieldhash(ClipMachine * ClipMemoryStore, long *area, long *field)
{
   ClipVar *vp, *vpp;

	vpp = --(ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame);
   *area = -1;
   *field = -1;

   vp = _clip_vptr(vpp);

	switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
      {
      case CHARACTER_type_of_ClipVarType:
	 {
	    int al = 0, fl = 0;
	    char *aname = 0, *fname = 0;

	    _clip_parse_name(ClipMemoryStore, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, &aname, &al,
&fname, &fl, 0, 0);
	    if (aname)
	       *area = _clip_casehashbytes(0, aname, al);
	    if (fname)
	       *field = _clip_casehashbytes(0, fname, fl);
	 }
	 break;
      case NUMERIC_type_of_ClipVarType:
	 *field = _clip_double(vp);
	 break;
      case OBJECT_type_of_ClipVarType:
			*field = vp->ClipObjVar_o_of_ClipVar.rtti_of_ClipObjVar->hash_of_ClipObjRtti(ClipMemoryStore, vp->ClipObjVar_o_of_ClipVar.obj_of_ClipObjVar, vp->ClipObjVar_o_of_ClipVar.rtti_of_ClipObjVar);
	 break;
      default:
	 break;
      }

   _clip_destroy(ClipMemoryStore, vpp);
}

CLIP_DLLEXPORT int
_clip_cond(ClipMachine * ClipMemoryStore, int *ifp)
{
   int r = _clip_tcond(ClipMemoryStore, ifp);

   if (r)
      return r;

	ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame--;
	_clip_destroy(ClipMemoryStore, ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame);
   return 0;
}

CLIP_DLLEXPORT int
_clip_tcond(ClipMachine * ClipMemoryStore, int *ifp)
{
	ClipVar *vp = _clip_vptr(ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - 1);

	if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
      {
	 _clip_trap_str(ClipMemoryStore, __file__, __LINE__, "non-logical var in condition");
	 return _clip_call_errblock(ClipMemoryStore, 1);
      }
	   if (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags)
			*ifp = vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
   else
		*ifp = vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;

   return 0;
}

CLIP_DLLEXPORT void *
_clip_memdup(const void *src, int len)
{
   void *ret = malloc(len + 1);

   memcpy(ret, src, len);
   ((char *) ret)[len] = 0;
   return ret;
}

CLIP_DLLEXPORT int
_clip_mclone(struct ClipMachine *ClipMemoryStore, ClipVar * dest, ClipVar * src)
{
	if (dest->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType == F_MREF_ClipFlags && dest->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar->ClipType_t_of_ClipVar.count_of_ClipType > 1)
      {
	 ClipVar *dp, *srcp;
	 int count, ret;

	 dp = dest->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
	 srcp = src;
	count = dp->ClipType_t_of_ClipVar.count_of_ClipType;
	 ret = 0;

	 switch (src->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType)
	    {
	    case F_MREF_ClipFlags:
	       srcp = src->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
	       if (srcp == dp)
		  break;
	    case F_MPTR_ClipFlags:
			 if (src->ClipType_t_of_ClipVar.field_of_ClipType && src->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar)
		  {
		     dest->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar = src->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar;
			  dest->ClipType_t_of_ClipVar.field_of_ClipType = 1;
		  }
	    default:
	       _clip_destroy(ClipMemoryStore, dp);
	       ret = _clip_clone(ClipMemoryStore, dp, srcp);
			 dp->ClipType_t_of_ClipVar.count_of_ClipType = count;
			 dest->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = srcp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType;
			 dest->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MREF_ClipFlags;
			 dest->ClipType_t_of_ClipVar.field_of_ClipType = 0;
	       break;
	    }
	 return ret;
      }
   else
      {
	 _clip_destroy(ClipMemoryStore, dest);
	 return _clip_clone(ClipMemoryStore, dest, src);
      }
}

CLIP_DLLEXPORT int
_clip_clone(struct ClipMachine *ClipMemoryStore, ClipVar * dest, ClipVar * src)
{
   ClipType t;

   CLEAR_CLIPVAR(dest);
   if (!src)
      return 0;

   t = src->ClipType_t_of_ClipVar;
	switch (src->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType)
      {
      case F_MPTR_ClipFlags:
	 dup_ref(dest, src);
	 return 0;
      case F_MREF_ClipFlags:
			switch (src->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
	    {
	    case MAP_type_of_ClipVarType:
	    case ARRAY_type_of_ClipVarType:
	    case CCODE_type_of_ClipVarType:
	    case PCODE_type_of_ClipVarType:
	       dup_ref(dest, src);
	       return 0;
	    default:
	       return _clip_clone(ClipMemoryStore, dest, src->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar);
	    }
      case F_MSTAT_ClipFlags:
	 t.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
      case F_NONE_ClipFlags:
	 switch (src->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
	    {
	    case UNDEF_type_of_ClipVarType:
	       break;
	    case NUMERIC_type_of_ClipVarType:
	       if (src->ClipType_t_of_ClipVar.memo_of_ClipType)
		  {
			  dest->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
			  dest->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
			  dest->ClipType_t_of_ClipVar.len_of_ClipType = src->ClipType_t_of_ClipVar.len_of_ClipType;
			  dest->ClipType_t_of_ClipVar.dec_of_ClipType = src->ClipType_t_of_ClipVar.dec_of_ClipType;
			  dest->ClipType_t_of_ClipVar.memo_of_ClipType = 1;
		     dest->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar = rational_copy(src->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
		  }
	       else
		  *dest = *src;
	       break;
	    case LOGICAL_type_of_ClipVarType:
	    case DATE_type_of_ClipVarType:
	    case DATETIME_type_of_ClipVarType:
	       *dest = *src;
	       break;
	    case CCODE_type_of_ClipVarType:
	    case PCODE_type_of_ClipVarType:
	       *dest = *src;
			 if (src->ClipCodeVar_c_of_ClipVar.ClipVarFrame_uplocals_of_ClipCodeVar && src->ClipCodeVar_c_of_ClipVar.ClipVarFrame_uplocals_of_ClipCodeVar->refcount_of_ClipVarFrame)
				 src->ClipCodeVar_c_of_ClipVar.ClipVarFrame_uplocals_of_ClipCodeVar->refcount_of_ClipVarFrame++;
	       return 0;
	    case CHARACTER_type_of_ClipVarType:
	       {
		  int len = src->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;

		  dest->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = (char *) _clip_memdup(src->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, len);
		  dest->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = len;

	       }
	       break;
	    case OBJECT_type_of_ClipVarType:
			 src->ClipObjVar_o_of_ClipVar.rtti_of_ClipObjVar->clone_of_ClipObjRtti(ClipMemoryStore, dest, src);
	       break;
	    default:
	       break;
	    }
      }
	   dest->ClipType_t_of_ClipVar = t;
   return 0;
}

CLIP_DLLEXPORT int
_clip_dup(ClipMachine * ClipMemoryStore, ClipVar * dest, ClipVar * src)
{
	dest->ClipType_t_of_ClipVar = src->ClipType_t_of_ClipVar;

	switch (src->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType)
      {
      case F_MPTR_ClipFlags:
      case F_MREF_ClipFlags:
#if 0
	 {
		 enum ClipVarType type = src->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType;

	    if (type == CCODE_type_of_ClipVarType || type == PCODE_type_of_ClipVarType)
	       {
		  dest->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = src->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
		 ++(src->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar->ClipType_t_of_ClipVar.count);
		  return 0;
	       }
	 }
#endif
	 src = src->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
	 dest->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	 break;
      case F_MSTAT_ClipFlags:
	/*
	 *dest = *src;
	 return 0;
	 */
      case F_NONE_ClipFlags:
	 break;
      }

switch (src->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
      {
      case NUMERIC_type_of_ClipVarType:
	 if (src->ClipType_t_of_ClipVar.memo_of_ClipType)
	    {
			 dest->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
			 dest->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
			 dest->ClipType_t_of_ClipVar.len_of_ClipType = src->ClipType_t_of_ClipVar.len_of_ClipType;
			 dest->ClipType_t_of_ClipVar.dec_of_ClipType = src->ClipType_t_of_ClipVar.dec_of_ClipType;
			 dest->ClipType_t_of_ClipVar.memo_of_ClipType = 1;
	       dest->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar = rational_copy(src->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
	    }
	 else
	    *dest = *src;
	 break;
      case LOGICAL_type_of_ClipVarType:
      case DATE_type_of_ClipVarType:
      case DATETIME_type_of_ClipVarType:
	 *dest = *src;
	 break;
      case CCODE_type_of_ClipVarType:
      case PCODE_type_of_ClipVarType:
	 {
		/**dest = *src;*/
	    CLEAR_CLIPVAR(dest);
		 dest->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR_ClipFlags;
		 dest->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = src->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType;
	    dest->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = src;

	    src->ClipType_t_of_ClipVar.count_of_ClipType++;
#if 0
vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = src->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType;
vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	    vp->c = src->c;
		 if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == PCODE_type_of_ClipVarType && vp->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipBlock_block_of_ClipCodeVar->file)
	       vp->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipBlock_block_of_ClipCodeVar->file->refCount++;
#if 0
if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == PCODE_type_of_ClipVarType)
	       {
		  ClipBlock *bp;

		  bp = NEW(ClipBlock);
		  *bp = *(src->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipBlock_block_of_ClipCodeVar);
		  vp->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipBlock_block_of_ClipCodeVar = bp;
		 /*bp->file->refCount++; */
	       }
	    else
	       vp->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipFunction_function_of_ClipCodeVar = src->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipFunction_function_of_ClipCodeVar;
#endif
#endif
	 }
#if 0
	 if (src->ClipCodeVar_c_of_ClipVar.ClipVarFrame_uplocals_of_ClipCodeVar && src->ClipCodeVar_c_of_ClipVar.ClipVarFrame_uplocals_of_ClipCodeVar->refcount)
	    src->ClipCodeVar_c_of_ClipVar.ClipVarFrame_uplocals_of_ClipCodeVar->refcount++;
#endif
	 break;
      case CHARACTER_type_of_ClipVarType:
	 {
	    int len = src->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;

	    dest->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = (char *) _clip_memdup(src->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, len);
	    dest->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = len;
	 }
	 break;
      case OBJECT_type_of_ClipVarType:
			src->ClipObjVar_o_of_ClipVar.rtti_of_ClipObjVar->clone_of_ClipObjRtti(ClipMemoryStore, dest, src);
	 break;
      case ARRAY_type_of_ClipVarType:
	 {
	    ClipVar *ap = NEW(ClipVar);
	    int count = src->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar, i;

		 dest->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = ARRAY_type_of_ClipVarType;
		 dest->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR_ClipFlags;
	    dest->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = ap;
		 dest->ClipType_t_of_ClipVar.field_of_ClipType = 0;
	    dest->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar = 0;
		 ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = ARRAY_type_of_ClipVarType;
		 ap->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
		 ap->ClipType_t_of_ClipVar.count_of_ClipType = 1;
	    ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar = (ClipVar *) malloc(sizeof(ClipVar) * count);
	    ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar = count;
	    for (i = 0; i < count; i++)
	       {
		  ClipVar *src_p, *tg_p;

		  tg_p = ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i;
		  src_p = src->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i;
		  _clip_dup(ClipMemoryStore, tg_p, src_p);
	       }
	 }
	 break;
      case MAP_type_of_ClipVarType:
	 {
	    ClipVar *ap = NEW(ClipVar);
		 int count = src->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar, i;

		 dest->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = MAP_type_of_ClipVarType;
		 dest->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR_ClipFlags;
	    dest->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = ap;
		 dest->ClipType_t_of_ClipVar.field_of_ClipType = 0;
	    dest->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar = 0;
		 ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = MAP_type_of_ClipVarType;
		 ap->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
		 ap->ClipType_t_of_ClipVar.count_of_ClipType = 1;
		 ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar = (ClipVarEl *) malloc(sizeof(ClipVarEl) * count);
		 ap->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar = count;
	    for (i = 0; i < count; i++)
	       {
		  ClipVarEl *src_p, *tg_p;

		 tg_p = ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar + i;
		 src_p = src->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar + i;
		 _clip_dup(ClipMemoryStore, &tg_p->ClipVar_var_of_ClipVarEl, &src_p->ClipVar_var_of_ClipVarEl);
		 tg_p->no_of_ClipVarEl = src_p->no_of_ClipVarEl;
	       }
	 }
	 break;
      default:
	 break;
      }
   return 0;
}

CLIP_DLLEXPORT int
_clip_push(ClipMachine * ClipMemoryStore, void *vpp)
{
	ClipVar *sp = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame;
   ClipVar *vp = (ClipVar *) vpp;
   int ret;

	if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType && (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags) &&
vp->ClipType_t_of_ClipVar.field_of_ClipType &&vp->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar)
      {
	 ret = _clip_take_field(ClipMemoryStore, vp->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar->fieldhash_of_ClipFieldDef,
vp->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar->areahash_of_ClipFieldDef, vp);
	 if (ret)
	    return ret;
      }

   ret = _clip_clone(ClipMemoryStore, sp, vp);

	++(ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame);

  /*
     if (!(ClipMemoryStore->flags1 & NOEXPAND_MACRO_FLAG))
     {
   */
   if (!ret)
      {
			if (sp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType && sp->ClipType_t_of_ClipVar.MACRO)
	    ret = _clip_expand_var(ClipMemoryStore, sp);
      }
  /*} */

   CLIP_CHECK_STACK;

   return ret;
}

CLIP_DLLEXPORT void
_clip_push_nil(ClipMachine * ClipMemoryStore)
{
   ClipFrame *fp = ClipMemoryStore->fp;
	fp->ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = UNDEF_type_of_ClipVarType;
	fp->ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	fp->ClipVar_sp_of_ClipFrame++;
}

CLIP_DLLEXPORT void
_clip_push_true(ClipMachine * ClipMemoryStore)
{
   ClipFrame *fp = ClipMemoryStore->fp;
	fp->ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
	fp->ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	fp->ClipVar_sp_of_ClipFrame->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = 1;
	fp->ClipVar_sp_of_ClipFrame++;
}

CLIP_DLLEXPORT void
_clip_push_false(ClipMachine * ClipMemoryStore)
{
   ClipFrame *fp = ClipMemoryStore->fp;
	fp->ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
	fp->ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	fp->ClipVar_sp_of_ClipFrame->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = 0;
	fp->ClipVar_sp_of_ClipFrame++;
}

CLIP_DLLEXPORT int
_clip_push_static(ClipMachine * ClipMemoryStore, void *vpp)
{
   int r;

   r = _clip_push(ClipMemoryStore, vpp);
   if (!r)
      {
	 ClipVar *vp;

	vp = _clip_vptr(ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - 1);
	if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CCODE_type_of_ClipVarType || vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == PCODE_type_of_ClipVarType)
	    {
			 if (!vp->ClipCodeVar_c_of_ClipVar.ClipVarFrame_uplocals_of_ClipCodeVar && ClipMemoryStore->fp->ClipVarFrame_localvars_of_ClipFrame)
		  {
			  ClipMemoryStore->fp->ClipVarFrame_localvars_of_ClipFrame->refcount_of_ClipVarFrame++;
			  vp->ClipCodeVar_c_of_ClipVar.ClipVarFrame_uplocals_of_ClipCodeVar = ClipMemoryStore->fp->ClipVarFrame_localvars_of_ClipFrame;
		  }
	    }
      }
   return r;
}

CLIP_DLLEXPORT void
_clip_push_hash(ClipMachine * ClipMemoryStore, long hash)
{
	ClipVar *sp = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame;

   CLEAR_CLIPVAR(sp);
	sp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	sp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	sp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	sp->ClipType_t_of_ClipVar.len_of_ClipType = 10;
	sp->ClipType_t_of_ClipVar.dec_of_ClipType = 0;
   sp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = hash;

	ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame++;
   CLIP_CHECK_STACK;
}

CLIP_DLLEXPORT int
_clip_ref(ClipMachine * ClipMemoryStore, void *Vp, int noadd)
{
   ClipVar *nv;
   ClipVar *vp = (ClipVar *) Vp;
	ClipVar *sp = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame;
   CLEAR_CLIPVAR(sp);

   if (!vp)
      return EG_NOVAR;

	if (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags)
      {
	 if (noadd)
	    *sp = *vp;
	 else
	    {
	       dup_ref(sp, vp);
			 sp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MREF_ClipFlags;
	    }

++(ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame);
	 CLIP_CHECK_STACK;
	 return 0;
      }

   nv = NEW(ClipVar);

	if (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MSTAT_ClipFlags)
      {
			/*vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType &= ~F_MSTAT_ClipFlags; */
	 _clip_clone(ClipMemoryStore, nv, vp);
      }
   else
      *nv = *vp;

	nv->ClipType_t_of_ClipVar.count_of_ClipType = noadd ? 1 : 2;
	vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MREF_ClipFlags /*mptr?F_MPTR_ClipFlags:F_MREF_ClipFlags */ ;
   vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = nv;
	vp->ClipType_t_of_ClipVar.field_of_ClipType = 0;
   vp->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar = 0;

	sp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = nv->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType;
	sp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MREF_ClipFlags /*mptr?F_MPTR_ClipFlags:F_MREF_ClipFlags */ ;
   sp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = nv;
	sp->ClipType_t_of_ClipVar.field_of_ClipType = 0;
   sp->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar = 0;

	++(ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame);
   CLIP_CHECK_STACK;
   return 0;
}

CLIP_DLLEXPORT long
_clip_hash(ClipMachine * ClipMemoryStore, ClipVar * vp)
{
   vp = _clip_vptr(vp);

   switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
      {
      case CHARACTER_type_of_ClipVarType:
	 return _clip_hashbytes(0, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
      case NUMERIC_type_of_ClipVarType:
	 return _clip_double(vp);
      case DATE_type_of_ClipVarType:
			return vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar;
      case LOGICAL_type_of_ClipVarType:
			return vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
      case OBJECT_type_of_ClipVarType:
			return vp->ClipObjVar_o_of_ClipVar.rtti_of_ClipObjVar->hash_of_ClipObjRtti(ClipMemoryStore, vp->ClipObjVar_o_of_ClipVar.obj_of_ClipObjVar, vp->ClipObjVar_o_of_ClipVar.rtti_of_ClipObjVar);
      default:
	 break;
      }
   return 0;
}

static long
calc_casehash(ClipMachine * ClipMemoryStore, ClipVar * vp, int trim)
{
   if (!vp)
      return 0;
   vp = _clip_vptr(vp);

	switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
      {
      case CHARACTER_type_of_ClipVarType:
	 {
	    char *s = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
	    int l = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;

	    if (trim)
	       {
		  char *e;

		  for (e = s + l - 1; e >= s; e--)
		     {
			switch (*e)
			   {
			   case ' ':
			   case '\t':
			   case '\r':
			   case '\n':
			      l--;
			      continue;
			   }
			break;
		     }
	       }
	    return _clip_casehashbytes(0, s, l);
	 }
      case NUMERIC_type_of_ClipVarType:
	 return _clip_double(vp);
      case DATE_type_of_ClipVarType:
			return vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar;
      case LOGICAL_type_of_ClipVarType:
			return vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
      case OBJECT_type_of_ClipVarType:
			return vp->ClipObjVar_o_of_ClipVar.rtti_of_ClipObjVar->hash_of_ClipObjRtti(ClipMemoryStore, vp->ClipObjVar_o_of_ClipVar.obj_of_ClipObjVar, vp->ClipObjVar_o_of_ClipVar.rtti_of_ClipObjVar);
      default:
	 break;
      }
   return 0;
}

CLIP_DLLEXPORT long
_clip_casehash(ClipMachine * ClipMemoryStore, ClipVar * vp)
{
   return calc_casehash(ClipMemoryStore, vp, 0);
}

CLIP_DLLEXPORT long
_clip_pop_hash(ClipMachine * ClipMemoryStore)
{
	ClipVar *vp = --(ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame);
   long r;

   r = calc_casehash(ClipMemoryStore, vp, 1);
   _clip_destroy(ClipMemoryStore, vp);

   return r;
}

CLIP_DLLEXPORT long
_clip_pop_shash(ClipMachine * ClipMemoryStore)
{
	ClipVar *vp = --(ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame);
   long r;

   r = _clip_hash(ClipMemoryStore, vp);
   _clip_destroy(ClipMemoryStore, vp);

   return r;
}

/* ] ClipMachine */
/* [ param */

CLIP_DLLEXPORT void
_clip_param(ClipMachine * ClipMemoryStore, int no, int lno)
{
   ClipVar *param = _clip_ref_local(ClipMemoryStore, lno);

   _clip_destroy(ClipMemoryStore, param);

   if (!param)
      return;
   if (no < ClipMemoryStore->argc)
      {
	 ClipVar *vp = ClipMemoryStore->bp - (ClipMemoryStore->argc - no);

	if (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType == F_MREF_ClipFlags)
	    {
	       _clip_destroy(ClipMemoryStore, param);
	       *param = *vp;
			 vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar->ClipType_t_of_ClipVar.count_of_ClipType++;
	    }
	 else
	    _clip_mclone(ClipMemoryStore, param, vp);
	 if (param->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags && param->ClipType_t_of_ClipVar.field_of_ClipType)
	    {
			 param->ClipType_t_of_ClipVar.field_of_ClipType = 0;
	       param->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar = 0;
	    }
      }
   else
      _clip_destroy(ClipMemoryStore, param);
}

CLIP_DLLEXPORT ClipVar *
_clip_ref_param(ClipMachine * ClipMemoryStore, int no, ClipVar * params)
{
   if (no >= ClipMemoryStore->argc)
      return params + no;
   else
      return ClipMemoryStore->bp - (ClipMemoryStore->argc - no);
}

/* ] param */
/* [ memvars */

CLIP_DLLEXPORT void
_clip_memvar_public(ClipMachine * ClipMemoryStore, long hash)
{
   VAR(VarEntry, vp, HashTable_fetch(ClipMemoryStore->publics, hash));
   if (!vp)
      {
	 VarEntry *ep = new_VarEntry(hash);

	ep->ClipVar_var_of_VarEntry.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
	 HashTable_insert(ClipMemoryStore->publics, ep, hash);
      }
}

CLIP_DLLEXPORT void
_clip_add_private(ClipMachine * ClipMemoryStore, long hash)
{
   add_private(ClipMemoryStore, hash);
}

CLIP_DLLEXPORT void
_clip_memvar_private(ClipMachine * ClipMemoryStore, long hash)
{
   add_private(ClipMemoryStore, hash);
}

CLIP_DLLEXPORT int
_clip_is_public(ClipMachine * ClipMemoryStore, long hash)
{
   VAR(VarEntry, vp, HashTable_fetch(ClipMemoryStore->publics, hash));

   return vp ? 0 : -1;
}

CLIP_DLLEXPORT int
_clip_is_private(ClipMachine * ClipMemoryStore, long hash)
{
   VAR(VarEntry, vp, HashTable_fetch(ClipMemoryStore->privates, hash));

   return vp ? 0 : -1;
}

CLIP_DLLEXPORT int
_clip_is_memvar(ClipMachine * ClipMemoryStore, long hash)
{
   return (!_clip_is_private(ClipMemoryStore, hash) || !_clip_is_public(ClipMemoryStore, hash)) ? 0 : 1;
}

CLIP_DLLEXPORT struct _HashTable *
_clip_space(ClipMachine * ClipMemoryStore, long space)
{
   VAR(HashTable, hp, HashTable_fetch(ClipMemoryStore->spaces, space));
   if (!hp)
      {
	 hp = new_HashTable();
	 HashTable_insert(ClipMemoryStore->spaces, hp, space);
      }
   return hp;
}

CLIP_DLLEXPORT ClipVar *
_clip_memvar_space(ClipMachine * ClipMemoryStore, struct _HashTable * hp, long hash, int newvar)
{
   VarEntry *vp = (VarEntry *) HashTable_fetch(hp, hash);

   if (!vp)
      {
	 if (!newvar)
	    return 0;
	 vp = new_VarEntry(hash);
	 HashTable_insert(hp, vp, hash);
      }

return &(vp->ClipVar_var_of_VarEntry);
}

CLIP_DLLEXPORT void
_clip_memvar_param(ClipMachine * ClipMemoryStore, long hash, int no)
{
   ClipVar *cp = 0;
   VarEntry *vp;

   if (no >= 0 && no < ClipMemoryStore->argc)
      cp = ClipMemoryStore->bp - (ClipMemoryStore->argc - no);

   vp = add_private(ClipMemoryStore, hash);
   if (cp)
      {
			_clip_destroy(ClipMemoryStore, &vp->ClipVar_var_of_VarEntry);
			vp->ClipVar_var_of_VarEntry = *cp;
			if ((vp->ClipVar_var_of_VarEntry.ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags) && vp->ClipVar_var_of_VarEntry.ClipType_t_of_ClipVar.field_of_ClipType)
	    {
			 vp->ClipVar_var_of_VarEntry.ClipType_t_of_ClipVar.field_of_ClipType = 0;
			 vp->ClipVar_var_of_VarEntry.ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar = 0;
	    }
	 CLEAR_CLIPVAR(cp);
      }
}

static void
remove_private_vect(ClipMachine * ClipMemoryStore, int num, void *vect)
{
   long *lp = (long *) vect;

   for (; num > 0; --num, ++lp)
      {
	 long hash = GETLONG(lp);
	 VarEntry *vp = (VarEntry *) HashTable_fetch(ClipMemoryStore->privates, hash);

	 if (!vp)
	    continue;
	 if (vp->VarEntry_next_of_VarEntry)
	    {
	       HashTable_remove(ClipMemoryStore->privates, hash);
			 HashTable_store(ClipMemoryStore->privates, vp->VarEntry_next_of_VarEntry, hash);
			 vp->VarEntry_next_of_VarEntry = 0;
	    }
	 else
	    {
	       HashTable_remove(ClipMemoryStore->privates, hash);
	    }
	 delete_VarEntry(ClipMemoryStore, vp);
      }
}

CLIP_DLLEXPORT void
_clip_remove_privates(ClipMachine * ClipMemoryStore, int num, ... /*long hash */ )
{
   va_list ap;

   va_start(ap, num);
   remove_private_vect(ClipMemoryStore, num, ap);
   va_end(ap);
}

CLIP_DLLEXPORT void
_clip_vremove_privates(ClipMachine * ClipMemoryStore, int num, long *vect)
{
   remove_private_vect(ClipMemoryStore, num, vect);
}

static VarEntry *
add_private(ClipMachine * ClipMemoryStore, long hash)
{
   long *p;
   VarEntry *vp, *np;
   ClipFrame *fp;

   if (ClipMemoryStore->inMacro)
      fp = ClipMemoryStore->inMacro;
   else
      fp = ClipMemoryStore->fp;

   if (fp)
      {
			p = fp->privates_of_ClipFrame;
	 if (p)
	    {
	       int n;
	       long c;

	       for (n = *p, p++; n >= 0; n--, p++)
		  if (hash == GETLONG(p))
		     {
			vp = (VarEntry *) HashTable_fetch(ClipMemoryStore->privates, hash);
			if (vp)
			   return vp;
		     }
	        p = fp->privates_of_ClipFrame;
	       c = GETLONG(p);
	       p = (long *) realloc(p, (c + 2) * sizeof(long));

	       SETLONG(p, c + 1);
	       SETLONG(p + c + 1, hash);
			fp->privates_of_ClipFrame = p;
	    }
	 else
	    {
			 p = fp->privates_of_ClipFrame = (long *) malloc(sizeof(long) * 2);

	       SETLONG(p, 1);
	       SETLONG(p + 1, hash);
	    }
      }

   vp = (VarEntry *) HashTable_fetch(ClipMemoryStore->privates, hash);
   np = new_VarEntry(hash);
   if (!vp)
      HashTable_insert(ClipMemoryStore->privates, np, hash);
   else
      {
			np->VarEntry_next_of_VarEntry = vp;
	 HashTable_remove(ClipMemoryStore->privates, hash);
	 HashTable_store(ClipMemoryStore->privates, np, hash);
      }
   return np;
}

static VarEntry *
fetch_var(ClipMachine * ClipMemoryStore, long hash)
{
   VarEntry *vp = (VarEntry *) HashTable_fetch(ClipMemoryStore->privates, hash);

   if (!vp)
      vp = (VarEntry *) HashTable_fetch(ClipMemoryStore->publics, hash);
   return vp;
}

static ClipVar *
fetch_obj(ClipMachine * ClipMemoryStore, ClipVar * obj, long hash)
{
	if (obj->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != MAP_type_of_ClipVarType)
      return 0;
   return _clip_mget(ClipMemoryStore, obj, hash);
}

CLIP_DLLEXPORT int
_clip_memvar(ClipMachine * ClipMemoryStore, long hash)
{
   VarEntry *vp;

   vp = fetch_var(ClipMemoryStore, hash);

   if (!vp)
      {
#if 0
	 vp = add_private(ClipMemoryStore, hash);
#else
	 char buf[64];
	 _clip_hash_name(ClipMemoryStore, hash, buf, sizeof(buf));
	 _clip_trap_printf(ClipMemoryStore, __file__, __LINE__, "no memvar variable '%s'", buf);
	 return _clip_call_errblock(ClipMemoryStore, EG_NOVAR);
#endif
      }
	   _clip_push(ClipMemoryStore, &vp->ClipVar_var_of_VarEntry);
   return 0;
}

CLIP_DLLEXPORT int
_clip_fmemvar(ClipMachine * ClipMemoryStore, long hash)
{
   VarEntry *vp;

   if (ClipMemoryStore->obj)
      {
	 ClipVar *vpp;

	 vpp = fetch_obj(ClipMemoryStore, _clip_vptr(ClipMemoryStore->obj), hash);
	 if (vpp)
	    {
	       _clip_push(ClipMemoryStore, vpp);
	       return 0;
	    }
      }

   if (!_clip_try_field(ClipMemoryStore, hash))
      return 0;

   vp = fetch_var(ClipMemoryStore, hash);

   if (!vp)
      {
	 char buf[64];

	/*int r; */

	 _clip_hash_name(ClipMemoryStore, hash, buf, sizeof(buf));
	 _clip_trap_printf(ClipMemoryStore, __file__, __LINE__, "no variable '%s'", buf);
	/*r = _clip_trap_err(ClipMemoryStore, EG_NOVAR, 0, 0, 0, 0, buf); */
	 return _clip_call_errblock(ClipMemoryStore, EG_NOVAR);
      }
  /*vp = add_private(ClipMemoryStore, hash); */
  _clip_push(ClipMemoryStore, &vp->ClipVar_var_of_VarEntry);
   return 0;
}

CLIP_DLLEXPORT int
_clip_memvarf(ClipMachine * ClipMemoryStore, long hash)
{
   VarEntry *vp;

   vp = fetch_var(ClipMemoryStore, hash);

   if (!vp)
      {
	 char buf[64];

	/*int r; */

	 if (!_clip_try_field(ClipMemoryStore, hash))
	    return 0;

	 _clip_hash_name(ClipMemoryStore, hash, buf, sizeof(buf));
	 _clip_trap_printf(ClipMemoryStore, __file__, __LINE__, "no variable '%s'", buf);
	/*r = _clip_trap_err(ClipMemoryStore, EG_NOVAR, 0, 0, 0, 0, buf); */
	 return _clip_call_errblock(ClipMemoryStore, EG_NOVAR);
      }
	   _clip_push(ClipMemoryStore, &vp->ClipVar_var_of_VarEntry);
   return 0;
}

CLIP_DLLEXPORT int
_clip_ref_fmemvar(ClipMachine * ClipMemoryStore, long hash)
{
   VarEntry *vp;

   if (!_clip_try_field(ClipMemoryStore, hash))
      return 0;

   vp = fetch_var(ClipMemoryStore, hash);

   if (!vp)
      {
#if 1
	 vp = add_private(ClipMemoryStore, hash);
#else
	 char buf[64];

	/*int r; */

	 _clip_hash_name(ClipMemoryStore, hash, buf, sizeof(buf));
	 _clip_trap_printf(ClipMemoryStore, __file__, __LINE__, "no variable '%s'", buf);
	/*r = _clip_trap_err(ClipMemoryStore, EG_NOVAR, 0, 0, 0, 0, buf); */
	 return _clip_call_errblock(ClipMemoryStore, EG_NOVAR);
#endif
      }
	   _clip_ref(ClipMemoryStore, &vp->ClipVar_var_of_VarEntry, 0);
   return 0;
}

CLIP_DLLEXPORT ClipVar *
_clip_ref_memvar(ClipMachine * ClipMemoryStore, long hash)
{
   VarEntry *vp = fetch_var(ClipMemoryStore, hash);

   if (!vp)
      {
#if 1
	 vp = add_private(ClipMemoryStore, hash);
#else
	 char buf[64];
	 _clip_hash_name(ClipMemoryStore, hash, buf, sizeof(buf));
	 _clip_trap_printf(ClipMemoryStore, __file__, __LINE__, "no variable '%s'", buf);
	 _clip_call_errblock(ClipMemoryStore, EG_NOVAR);
	 return 0;
#endif
      }
	   return &vp->ClipVar_var_of_VarEntry;
}

CLIP_DLLEXPORT ClipVar *
_clip_ref_memvar_noadd(ClipMachine * ClipMemoryStore, long hash)
{
   VarEntry *vp = fetch_var(ClipMemoryStore, hash);

   if (!vp)
      {
#if 0
	 return 0;
#else
	 char buf[64];
	 _clip_hash_name(ClipMemoryStore, hash, buf, sizeof(buf));
	 _clip_trap_printf(ClipMemoryStore, __file__, __LINE__, "no memvar variable '%s'", buf);
	 _clip_call_errblock(ClipMemoryStore, EG_NOVAR);
	 return 0;
#endif
      }
	   return &vp->ClipVar_var_of_VarEntry;
}

CLIP_DLLEXPORT int
_clip_public(ClipMachine * ClipMemoryStore, long hash)
{
   VarEntry *vp = (VarEntry *) HashTable_fetch(ClipMemoryStore->publics, hash);

   if (!vp)
      {
	 char buf[64];

	 _clip_hash_name(ClipMemoryStore, hash, buf, sizeof(buf));
	 _clip_trap_printf(ClipMemoryStore, __file__, __LINE__, "try access to unknown public var '%s'", buf);
	 return _clip_call_errblock(ClipMemoryStore, 1);
      }
	   _clip_push(ClipMemoryStore, &vp->ClipVar_var_of_VarEntry);
   return 0;
}

CLIP_DLLEXPORT ClipVar *
_clip_ref_public(ClipMachine * ClipMemoryStore, long hash)
{
   VarEntry *vp = (VarEntry *) HashTable_fetch(ClipMemoryStore->publics, hash);

   if (!vp)
      {
	 char buf[64];

	 _clip_hash_name(ClipMemoryStore, hash, buf, sizeof(buf));
	 _clip_trap_printf(ClipMemoryStore, __file__, __LINE__, "try access to unknown public var '%s'", buf);
	 return 0;
      }
	   return &vp->ClipVar_var_of_VarEntry;
}

CLIP_DLLEXPORT ClipVar *
_clip_ref_public_noadd(ClipMachine * ClipMemoryStore, long hash)
{
   VarEntry *vp = (VarEntry *) HashTable_fetch(ClipMemoryStore->publics, hash);

   if (!vp)
      return 0;
   else
		return &vp->ClipVar_var_of_VarEntry;
}

/* ] memvars */
/* [ traps */

/* retrapper - add filename&line info into trapStack */
CLIP_DLLEXPORT void
_clip_trap(ClipMachine * ClipMemoryStore, const char *filename, int line)
{
   char *s;
   char buf[256];

   snprintf(buf, 256, "file '%s' line %d", filename, line);
#if 0
   if (!ClipMemoryStore->noerrblock)
      _clip_logg(4, "trace: %s", buf);
#endif
   s = strdup(buf);
   add_ClipVect(&ClipMemoryStore->trapTrace, s);
}

/* trap head of stack */
CLIP_DLLEXPORT void
_clip_trap_pop(ClipMachine * ClipMemoryStore)
{
	ClipVar *vp = --(ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame), *Vp = NEW(ClipVar);

   *Vp = *vp;
   ClipMemoryStore->trapVar = Vp;
}

CLIP_DLLEXPORT void
_clip_trap_str(ClipMachine * ClipMemoryStore, const char *filename, int line, const char *str)
{
   ClipVar *vp;

   vp = _clip_new_error(ClipMemoryStore, _clip_gettext(str), 2, 0, 0, 0, 0, 0, 0, 0, 0, 0);
   _clip_trap_var(ClipMemoryStore, filename, line, vp);
}

CLIP_DLLEXPORT int
_clip_trap_operation(ClipMachine * ClipMemoryStore, const char *filename, int line, const char *oper, ClipVar * lval)
{
   ClipVar *vp;
   int r;
   char buf[256];
   snprintf(buf, 256, _clip_gettext("invalid arguments for  '%s' operation"), oper);

   vp = _clip_new_error(ClipMemoryStore, buf, 2, 0, 0, 0, 0, 0, 0, 0, 0, oper);
   _clip_trap_var(ClipMemoryStore, filename, line, vp);

   if (!ClipMemoryStore->trapVar)
      _clip_generr(ClipMemoryStore, 1);

   _clip_destroy(ClipMemoryStore, lval);
   r = _clip_errorblock_res(ClipMemoryStore, ClipMemoryStore->trapVar, 1, lval);

   if (!r && _clip_type(lval) == UNDEF_type_of_ClipVarType)
      r = 1;

   return r;
}

CLIP_DLLEXPORT void
_clip_trap_printf(ClipMachine * ClipMemoryStore, const char *filename, int line, const char *fmt, ...)
{
   va_list ap;
   char buf[256];

   va_start(ap, fmt);
   vsnprintf(buf, sizeof(buf), _clip_gettext(fmt), ap);
   va_end(ap);
   _clip_trap_str(ClipMemoryStore, filename, line, buf);
}

CLIP_DLLEXPORT void
_clip_trap_printv(ClipMachine * ClipMemoryStore, const char *filename, int line, const char *fmt, void *ap)
{
   char buf[256];

   vsnprintf(buf, sizeof(buf), _clip_gettext(fmt), ap);
   _clip_trap_str(ClipMemoryStore, filename, line, buf);
}

CLIP_DLLEXPORT void
_clip_trap_var(ClipMachine * ClipMemoryStore, const char *filename, int line, ClipVar * vp)
{
   _clip_delete(ClipMemoryStore, ClipMemoryStore->trapVar);
   _clear_trap(ClipMemoryStore);
   if (vp)
      {
	/*vp = _clip_vptr(vp); */
	 ClipMemoryStore->trapVar = vp;
      }
   else
      {
	 ClipMemoryStore->trapVar = NEW(ClipVar);
      }
   _clear_trap(ClipMemoryStore);
   _clip_trap(ClipMemoryStore, filename, line);
}

CLIP_DLLEXPORT void
_clip_trap_invargv(ClipMachine * ClipMemoryStore, const char *filename, int line)
{
   _clip_trap_str(ClipMemoryStore, filename, line, "invalid arguments");
}

/* ] traps */
/* [ arrays */

/* create from stack */
CLIP_DLLEXPORT void
_clip_sarray(ClipMachine * ClipMemoryStore, int n)
{
	ClipVar arr, *ap, *sp = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame;
   int i;

   ap = NEW(ClipVar);
   CLEAR_CLIPVAR(&arr);
   arr.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = ARRAY_type_of_ClipVarType;
   arr.ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR_ClipFlags /*F_MREF_ClipFlags */ ;
	arr.ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = ap;
	ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = ARRAY_type_of_ClipVarType;
	ap->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	ap->ClipType_t_of_ClipVar.count_of_ClipType = 1;
   ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar = (ClipVar *) malloc(sizeof(ClipVar) * n);
   ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar = n;
   for (i = 0; i < n; i++)
      {
	 ClipVar *vpp = ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i;

	 *vpp = sp[-n + i];
	vpp->ClipType_t_of_ClipVar.field_of_ClipType = 0;
      }
   sp[-n] = arr;
	ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame -= n - 1;
}

static void
new_array(ClipVar * vp, int n, long *dims)
{
   ClipVar *ap;
   int i, size;

   CLEAR_CLIPVAR(vp);
   if (!n)
      {
			vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = 0;
			vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	 return;
      }

   --n;
   size = GETLONG(dims);
   ++dims;

   ap = NEW(ClipVar);
	vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = ARRAY_type_of_ClipVarType;
	vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR_ClipFlags;
   vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = ap;
	ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = ARRAY_type_of_ClipVarType;
	ap->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	ap->ClipType_t_of_ClipVar.count_of_ClipType = 1;
   ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar = (ClipVar *) malloc(sizeof(ClipVar) * size);
   ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar = size;
   for (i = 0; i < size; i++)
      new_array(&ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar[i], n, dims);
}

CLIP_DLLEXPORT void
_clip_dimarray(ClipMachine * ClipMemoryStore, int n)
{
	ClipVar *sp = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame;
   int i;
   long *dims;

#ifdef OS_MINGW
   dims = (long *) malloc(sizeof(long) * n);
#else
   dims = (long *) alloca(sizeof(long) * n);
#endif

   for (i = 0; i < n; i++)
      dims[i] = _clip_long(sp - n + i);

	ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame -= n - 1;
	new_array(ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - 1, n, dims);
#ifdef OS_MINGW
   free(dims);
#endif
}

/* create with n dimentions */
CLIP_DLLEXPORT void
_clip_newarray(ClipMachine * ClipMemoryStore, int n, ...)
{
	ClipVar *sp = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame;
   va_list ap;

   va_start(ap, n);
   new_array(sp, n, (long *) ap);
   va_end(ap);

	++(ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame);
   CLIP_CHECK_STACK;
}

CLIP_DLLEXPORT void
_clip_vnewarray(ClipMachine * ClipMemoryStore, int n, long *vect)
{
	ClipVar *sp = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame;

   new_array(sp, n, vect);
	++(ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame);
   CLIP_CHECK_STACK;
}

CLIP_DLLEXPORT int
_clip_array(ClipMachine * ClipMemoryStore, ClipVar * dest, int ndim, long *dims)
{
   new_array(dest, ndim, dims);
   return 0;
}

CLIP_DLLEXPORT int
_clip_adel(ClipMachine * ClipMemoryStore, ClipVar * ap, int ndim, long *dims)
{
   int c;
   long ind;

   ap = _clip_vptr(ap);
	if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType || !ndim)
      {
	 _clip_trap_printf(ClipMemoryStore, __file__, __LINE__, "adel for non-array object");
	 return _clip_call_errblock(ClipMemoryStore, 1);
      }

   c = ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
   ind = GETLONG(dims);

   if (ind < 0 || ind >= c)
      {
	 _clip_trap_printf(ClipMemoryStore, __file__, __LINE__, "index for adel is out-of range: ind=%d, count=%d", ind + 1, c);
	 return _clip_call_errblock(ClipMemoryStore, 1);
      }

   if (ndim > 1)
      {
	 ClipVar *app;

	 app = ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + ind;
	 return _clip_adel(ClipMemoryStore, app, ndim - 1, dims + 1);
      }

   _clip_destroy(ClipMemoryStore, ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + ind);
   memmove(ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + ind, ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + ind + 1, (c - ind - 1) * sizeof(ClipVar));
   memset(ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + c - 1, 0, sizeof(ClipVar));

   return 0;
}

CLIP_DLLEXPORT int
_clip_asize(ClipMachine * ClipMemoryStore, ClipVar * ap, int ndim, long *dims)
{
   int c;
   long i, ind;

   ap = _clip_vptr(ap);

	if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType || !ndim)
      {
	 _clip_trap_printf(ClipMemoryStore, __file__, __LINE__, "asize for non-array object");
	 return _clip_call_errblock(ClipMemoryStore, 1);
      }

   c = ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
   ind = GETLONG(dims);

   if (ndim > 1)
      {
	 ClipVar *app;

	 ind = dims[0];
	 if (ind < 0 || ind >= c)
	    {
	       _clip_trap_printf(ClipMemoryStore, __file__, __LINE__, "index for asize is out-of range: ind=%d, count=%d", ind + 1, c);
	       return _clip_call_errblock(ClipMemoryStore, 1);
	    }
	 app = ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + ind;
	 return _clip_asize(ClipMemoryStore, app, ndim - 1, dims + 1);
      }

   for (i = ind; i < c; ++i)
      _clip_destroy(ClipMemoryStore, ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i);

   ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar = (ClipVar *) realloc(ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar, sizeof(ClipVar) * ind);
   ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar = ind;

   for (i = c; i < ind; ++i)
      memset(ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i, 0, sizeof(ClipVar));

   return 0;
}

CLIP_DLLEXPORT int
_clip_ains(ClipMachine * ClipMemoryStore, ClipVar * ap, int ndim, long *dims)
{
   int c;
   long ind;

   ap = _clip_vptr(ap);

	if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType || !ndim)
      {
	 _clip_trap_printf(ClipMemoryStore, __file__, __LINE__, "ains for non-array object");
	 return _clip_call_errblock(ClipMemoryStore, 1);
      }

   c = ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
   ind = GETLONG(dims);
   if (ind < 0 || ind >= c)
      {
	 _clip_trap_printf(ClipMemoryStore, __file__, __LINE__, "index for ains is out-of range: ind=%d, count=%d", ind + 1, c);
	 return _clip_call_errblock(ClipMemoryStore, 1);
      }
   if (ndim > 1)
      {
	 ClipVar *app;

	 app = ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + ind;
	 return _clip_ains(ClipMemoryStore, app, ndim - 1, dims + 1);
      }

   if (c > 0)
      {
	 _clip_destroy(ClipMemoryStore, &ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar[c - 1]);
	 memmove(ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + ind + 1, ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + ind, (c - ind - 1) * sizeof(ClipVar));
      }
   memset(ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + ind, 0, sizeof(ClipVar));

   return 0;
}

CLIP_DLLEXPORT int
_clip_aadd(ClipMachine * ClipMemoryStore, ClipVar * ap, ClipVar * vp)
{
   int c;

   ap = _clip_vptr(ap);

	if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType)
      return 1;

   c = ++ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
   ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar = (ClipVar *) realloc(ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar, sizeof(ClipVar) * c);
   memset(ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + c - 1, 0, sizeof(ClipVar));
   _clip_clone(ClipMemoryStore, ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + c - 1, vp);

   return 0;
}

CLIP_DLLEXPORT ClipVar *
_clip_aref(ClipMachine * ClipMemoryStore, ClipVar * ap, int ndim, long *dims)
{
   int c;
   long ind;
   ClipVar *app = 0;

   ap = _clip_vptr(ap);

	if ((ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType && ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != MAP_type_of_ClipVarType) || !ndim)
      {
	 _clip_trap_printf(ClipMemoryStore, __file__, __LINE__, "aget for non-array object");
	 return 0;
      }

if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
      {
	 app = _clip_mget(ClipMemoryStore, ap, dims[0]);
      }
   else
      {
	 c = ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
	 ind = GETLONG(dims);

	 if (ind < 0 || ind >= c)
	    {
	       _clip_trap_printf(ClipMemoryStore, __file__, __LINE__, "index for aget is out-of range: ind=%d, count=%d", ind + 1, c);
	       return 0;
	    }

	 app = ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + ind;
      }

   if (ndim > 1)
      {
	 return _clip_aref(ClipMemoryStore, app, ndim - 1, dims + 1);
      }

   return app;
}

CLIP_DLLEXPORT int
_clip_aget(ClipMachine * ClipMemoryStore, ClipVar * ap, ClipVar * resp, int ndim, long *dims)
{
   ClipVar *app;

   app = _clip_aref(ClipMemoryStore, ap, ndim, dims);
   if (app)
      return _clip_clone(ClipMemoryStore, resp, app);
   else
      return EG_NOVAR;
}

CLIP_DLLEXPORT int
_clip_aset(ClipMachine * ClipMemoryStore, ClipVar * ap, ClipVar * vp, int ndim, long *dims)
{
   int c;
   long ind;
   ClipVar *app;

   ap = _clip_vptr(ap);

	if ((ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType && ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != MAP_type_of_ClipVarType) || !ndim)
      {
	 _clip_trap_printf(ClipMemoryStore, __file__, __LINE__, "aset for non-array object");
	 return _clip_call_errblock(ClipMemoryStore, 1);
      }

if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
      {
	 app = _clip_mget(ClipMemoryStore, ap, dims[0]);
      }
   else
      {
	 c = ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
	 ind = GETLONG(dims);

	 if (ind < 0 || ind >= c)
	    {
	       _clip_trap_printf(ClipMemoryStore, __file__, __LINE__, "index for aset is out-of range: ind=%d, count=%d", ind + 1, c);
	       return _clip_call_errblock(ClipMemoryStore, 1);
	    }

	 app = ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + ind;
      }

   if (ndim > 1)
      {
	 return _clip_aset(ClipMemoryStore, app, vp, ndim - 1, dims + 1);
      }

/*_clip_destroy(ClipMemoryStore, app);*/
   return _clip_mclone(ClipMemoryStore, app, vp);
}

CLIP_DLLEXPORT void
_clip_unref_arr(ClipMachine * ClipMemoryStore)
{
	ClipVar *sp = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - 1, *ap;
   int type = _clip_type(sp);

	if ((type != ARRAY_type_of_ClipVarType && type != MAP_type_of_ClipVarType) || (sp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType != F_MPTR_ClipFlags &&
sp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType !=F_MREF_ClipFlags))
      return;
   ap = _clip_vptr(sp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar);

   _clip_dup(ClipMemoryStore, sp, ap);
}

static void
print_dim(ClipMachine * ClipMemoryStore, char *buf, int size, int dim, int Dim, long *Vect)
{
   char *s = buf, *e = buf + size - 20;
   int i;

   for (i = 0, *s = 0; i < Dim - dim && s < e; ++i)
      {
	 char bb[64];

	 s = s + strlen(s);
	 _clip_hash_name(ClipMemoryStore, GETLONG(Vect + i), bb, sizeof(bb));
	 sprintf(s, "[%s]", bb);
      }
}

int
search_map(ClipVarEl * items, int count, long key, int *index)
{
   int l, h, res, i, c;
   long ln;

   l = 0;
   h = count - 1;
   res = 0;
   while (l <= h)
      {
	 i = (l + h) >> 1;
	ln = items[i].no_of_ClipVarEl;
	 if (ln < key)
	    c = -1;
	 else if (ln > key)
	    c = 1;
	 else
	    c = 0;

	 if (c < 0)
	    l = i + 1;
	 else
	    {
	       h = i - 1;
	       if (c == 0)
		  {
		     res = 1;
		     l = i;
		  }
	    }
      }
   if (index)
      *index = l;
   return res;
}

static ClipVarEl *
get_map_el(ClipMapVar * a, long no)
{
   int ind;

	if (search_map(a->ClipVarEl_items_of_ClipMapVar, a->count_of_ClipMapVar, no, &ind))
		return a->ClipVarEl_items_of_ClipMapVar + ind;
   else
      return 0;
}

CLIP_DLLEXPORT int
_clip_map(ClipMachine * ClipMemoryStore, ClipVar * vp)
{
   ClipVar *ap;

   _clip_destroy(ClipMemoryStore, vp);
   ap = NEW(ClipVar);
	vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = MAP_type_of_ClipVarType;
	vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR_ClipFlags;
   vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = ap;
	ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = MAP_type_of_ClipVarType;
	ap->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	ap->ClipType_t_of_ClipVar.count_of_ClipType = 1;
	ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar= 0;
	ap->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar = 0;

   return 0;
}

CLIP_DLLEXPORT int
_clip_mputn(ClipMachine * ClipMemoryStore, ClipVar * ap, long no, double n)
{
   ClipVar v;
   int r;

   memset(&v, 0, sizeof(v));
   _clip_var_num(n, &v);
   r = _clip_madd(ClipMemoryStore, ap, no, &v);
   _clip_destroy(ClipMemoryStore, &v);

   return r;
}

CLIP_DLLEXPORT int
_clip_mputl(ClipMachine * ClipMemoryStore, ClipVar * ap, long no, int l)
{
   ClipVar v;
   int r;

   memset(&v, 0, sizeof(v));
   _clip_var_log(l, &v);
   r = _clip_madd(ClipMemoryStore, ap, no, &v);
   _clip_destroy(ClipMemoryStore, &v);

   return r;
}

CLIP_DLLEXPORT int
_clip_mputc(ClipMachine * ClipMemoryStore, ClipVar * ap, long no, char *str, int len)
{
   ClipVar v;
   int r;

   memset(&v, 0, sizeof(v));
   _clip_var_str(str, len, &v);
   r = _clip_madd(ClipMemoryStore, ap, no, &v);
   _clip_destroy(ClipMemoryStore, &v);

   return r;
}

CLIP_DLLEXPORT int
_clip_madd(ClipMachine * ClipMemoryStore, ClipVar * ap, long no, ClipVar * vp)
{
   int ind, c;
   ClipVarEl *vlp;

   ap = _clip_vptr(ap);

	if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != MAP_type_of_ClipVarType)
      {
	 _clip_trap_printf(ClipMemoryStore, __file__, __LINE__, "madd for non-map object");
	 return _clip_call_errblock(ClipMemoryStore, 1);
      }

c = ap->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar;

	if (search_map(ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar, c, no, &ind))
      {
			vlp = ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar + ind;
			_clip_destroy(ClipMemoryStore, &vlp->ClipVar_var_of_ClipVarEl);
      }
   else
      {
			ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar = (ClipVarEl *) realloc(ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar, (c + 1) * sizeof(ClipVarEl));
	 if (ind < c)
		 memmove(ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar + ind + 1, ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar + ind, (c - ind /* -1 */ ) * sizeof(ClipVarEl));
	 ap->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar++;
	vlp = ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar + ind;
	 memset(vlp, 0, sizeof(ClipVarEl));
	 vlp->no_of_ClipVarEl = no;
      }
	   _clip_mclone(ClipMemoryStore, &vlp->ClipVar_var_of_ClipVarEl, vp);

   return 0;
}

CLIP_DLLEXPORT int
_clip_mdel(ClipMachine * ClipMemoryStore, ClipVar * ap, long no)
{
   int ind;

   ap = _clip_vptr(ap);

	if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != MAP_type_of_ClipVarType)
      {
	 _clip_trap_printf(ClipMemoryStore, __file__, __LINE__, "mdel for non-map object");
	 return _clip_call_errblock(ClipMemoryStore, 1);
      }
	   if (search_map(ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar, ap->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar, no, &ind))
      {
			_clip_destroy(ClipMemoryStore, &((ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar + ind)->ClipVar_var_of_ClipVarEl));

			ap->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar--;
	if (ind < ap->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar)
		 memmove(ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar + ind, ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar + ind + 1, (ap->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar -
ind) * sizeof(ClipVarEl));
      }
   return 0;
}

CLIP_DLLEXPORT int
_clip_mind(ClipMachine * ClipMemoryStore, ClipVar * ap, long no, int *ind)
{
   ap = _clip_vptr(ap);
	if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != MAP_type_of_ClipVarType)
      {
	 _clip_trap_printf(ClipMemoryStore, __file__, __LINE__, "mind for non-map object");
	 return _clip_call_errblock(ClipMemoryStore, 1);
      }
	   return !search_map(ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar, ap->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar, no, ind);
}

static int
test_maponerror(ClipMachine * ClipMemoryStore, ClipVar * map, char *member)
{
   int ind, ret = 1;

  /* try run `onerror` member */
  if (search_map(map->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar, map->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar, HASH_onerror, &ind))
      {
	 ClipVar *ep;

	 map->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
  ep = _clip_vptr(&(map->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[ind].ClipVar_var_of_ClipVarEl));

	if (ep->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == PCODE_type_of_ClipVarType || ep->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CCODE_type_of_ClipVarType)
	    {
	       int i;
	       ClipVar stack[3];
			 ClipVar *rval = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - 1;

	       memset(stack, 0, sizeof(stack));

			 stack[0].ClipType_t_of_ClipVar = map->ClipType_t_of_ClipVar;
	       stack[0].ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR_ClipFlags;
			 stack[0].ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = map;
			 map->ClipType_t_of_ClipVar.count_of_ClipType++;

	       _clip_var_str(member, strlen(member), stack + 1);
	       _clip_clone(ClipMemoryStore, stack + 2, rval);

	       _clip_eval(ClipMemoryStore, _clip_vptr(ep), 3, stack, rval);

	       for (i = 0; i < 3; i++)
		  _clip_destroy(ClipMemoryStore, stack + i);
	       ret = 0;
	    }
		 map->ClipType_t_of_ClipVar.memo_of_ClipType = 1;
      }
   return ret;
}

static ClipVar *
fetch_arr(ClipMachine * ClipMemoryStore, ClipVar * ap, int dim, long *vect, int Dim, long *Vect, int store, ClipVar ** mapp, long *hashp)
{
   long no;
   ClipVar *vp = NULL;
   ClipVarEl *vlp = 0;
   int isMap, type;
   int pdim;
   long *pvect;
   ClipVar *pap;

   if (!dim)
      return ap;

   type = _clip_type(ap);

   isMap = (type == MAP_type_of_ClipVarType ? 1 : 0);

   if (type == CHARACTER_type_of_ClipVarType)
      {
	 long pos, len = 1, slen;
	 char *buf;
	ClipVar *sp = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame, var;

	 pos = _clip_hash(ClipMemoryStore, sp - dim);
	 pos--;
	 if (dim > 1)
	    len = _clip_hash(ClipMemoryStore, sp - dim + 1);
	 if (len < 1)
	    len = 1;

	 vp = _clip_vptr(ap);
	 slen = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
	 buf = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;

	 CLEAR_CLIPVAR(&var);
	 if (pos < 0 || pos >= slen)
	    {
	       pos = 0;
	       len = 0;
	    }
	 if (pos + len > slen)
	    len = slen - pos;
	 _clip_var_str(buf + pos, len, &var);
	 vp = _clip_add_tempvar(ClipMemoryStore, &var);
	 _clip_destroy(ClipMemoryStore, &var);
	 return vp;
      }

   if (type != ARRAY_type_of_ClipVarType && !isMap)
      {
	 char buf[64];

	 print_dim(ClipMemoryStore, buf, sizeof(buf), dim - 1, Dim, Vect);
	_clip_trap_printf(ClipMemoryStore, ClipMemoryStore->fp->filename_of_ClipFrame, ClipMemoryStore->fp->line_of_ClipFrame, "try fetch element from non-array object%s%s", buf[0] ? " " : "", buf);
	 _clip_call_errblock(ClipMemoryStore, EG_BOUND);
	 return NULL;
      }

   pap = ap;
   pvect = vect;
   pdim = dim;

   ap = _clip_vptr(ap);
   no = GETLONG(vect);

   ++vect;
   --dim;

   if (isMap)
      {
	 int ind;
	int c = ap->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar;

	if (search_map(ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar, c, no, &ind))
		vlp = ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar + ind;
	 else if (store)
	    {
	      /*addvar: */
			ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar = (ClipVarEl *) realloc(ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar, (c + 1) * sizeof(ClipVarEl));
	       if (ind < c)
				 memmove(ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar + ind + 1, ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar + ind, (c - ind /*-1*/ ) * sizeof(ClipVarEl));
			 ap->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar++;
			 vlp = ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar + ind;
	       memset(vlp, 0, sizeof(ClipVarEl));
			 vlp->no_of_ClipVarEl = no;
	    }
	 else
	    {
	       if ((ClipMemoryStore->flags1 & MAPERR_FLAG))
		  {
		    /*goto addvar; */
			 vp = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame;
		     memset(vp, 0, sizeof(ClipVar));
		     vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = vp;
		     return vp;
		  }
	       else
		  {
		     char buf[64];

		     _clip_hash_name(ClipMemoryStore, no, buf, sizeof(buf));
		     if (test_maponerror(ClipMemoryStore, ap, buf))
			{
				_clip_trap_printf(ClipMemoryStore, ClipMemoryStore->fp->filename_of_ClipFrame, ClipMemoryStore->fp->line_of_ClipFrame, "map fetch error: index %s", buf);
			   _clip_call_errblock(ClipMemoryStore, EG_BOUND);
			   return 0;
			}
		     else
			{
			   vp = fetch_arr(ClipMemoryStore, pap, pdim, pvect, Dim, Vect, store, mapp, hashp);
			   return vp;
			}
		  }
	    }

if (store && !dim && ap->ClipType_t_of_ClipVar.memo_of_ClipType && mapp && hashp)
	    {
	       *mapp = ap;
	       *hashp = no;
	    }
      }
   else if (no < 1 || no > ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar)
      {
	 char buf[64];

	 print_dim(ClipMemoryStore, buf, sizeof(buf), dim, Dim, Vect);
	_clip_trap_printf(ClipMemoryStore, ClipMemoryStore->fp->filename_of_ClipFrame, ClipMemoryStore->fp->line_of_ClipFrame, "out-of-bounds array %s, index=%d, count=%d", buf, no,
ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar);
	 _clip_call_errblock(ClipMemoryStore, EG_BOUND);
	 return NULL;
      }
   if (isMap)
      {
			vp = fetch_arr(ClipMemoryStore, &vlp->ClipVar_var_of_ClipVarEl, dim, vect, Dim, Vect, store, mapp, hashp);
      }
   else
      vp = fetch_arr(ClipMemoryStore, ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + no - 1, dim, vect, Dim, Vect, store, mapp, hashp);

   return vp;
}

static int
clip_count(ClipVar * vp)
{
   int count;

	if (vp && vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags)
      {
	 count = vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar->ClipType_t_of_ClipVar.count_of_ClipType + clip_count(vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar);
      }
   else
      count = 0;
   return count;
}

static ClipVar *
clip_fetch(ClipMachine * ClipMemoryStore, int dim, int push, int store, ClipVar ** mapp, long *hashp)
{
   ClipFrame *fp = ClipMemoryStore->fp;
	ClipVar *sp = fp->ClipVar_sp_of_ClipFrame;
   ClipVar *ap = sp - dim - 1;
   ClipVar arr;
   ClipVar *vp = NULL;
  /*ClipVar *app; */
   int i;
#ifdef OS_MINGW
   long *vect = (long *) malloc(sizeof(long) * dim);
#else
   long *vect = (long *) alloca(sizeof(long) * dim);
#endif

   memset(&arr, 0, sizeof(ClipVar));
   _clip_clone(ClipMemoryStore, &arr, ap);

   for (i = 0; i < dim; ++i)
      {
	 long h;
	 void *p;

	 h = _clip_hash(ClipMemoryStore, sp - dim + i);
	 p = vect + i;

	 *(long *) (p) = (h);
	/*SETLONG(vect + i, _clip_hash(ClipMemoryStore, sp - dim + i)); */
      }

   vp = fetch_arr(ClipMemoryStore, &arr, dim, vect, dim, vect, store, mapp, hashp);

   if (!vp)
      return 0;

   for (i = dim + 1; i; --i)
      {
			sp = --(fp->ClipVar_sp_of_ClipFrame);
	 _clip_destroy(ClipMemoryStore, sp);
      }

   if (push)
      {
	 if (!vp)
	    {
			 vp = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame++;
	       CLIP_CHECK_STACK;
	       memset(vp, 0, sizeof(ClipVar));
	    }
	 else
	    _clip_push(ClipMemoryStore, vp);
      }

   if (!vp)
      {
			_clip_trap_str(ClipMemoryStore, ClipMemoryStore->fp->filename_of_ClipFrame, ClipMemoryStore->fp->line_of_ClipFrame, "map/array fetch error");
	 _clip_call_errblock(ClipMemoryStore, EG_BOUND);
      }

#if 0
   app = _clip_vptr(&arr);
   if (app->ClipType_t_of_ClipVar.count == 1 && !push)
#else
   if (clip_count(&arr) == 1 && !push)
#endif
      {
	 vp = _clip_add_tempvar(ClipMemoryStore, vp);
      }

   _clip_destroy(ClipMemoryStore, &arr);
#ifdef OS_MINGW
   free(vect);
#endif
   return vp;
}

CLIP_DLLEXPORT int
_clip_ifetch(ClipMachine * ClipMemoryStore, int dim)
{
   return clip_fetch(ClipMemoryStore, dim, 1, 0, 0, 0) ? 0 : 1;
}

CLIP_DLLEXPORT int
_clip_fetch(ClipMachine * ClipMemoryStore, int dim)
{
   return clip_fetch(ClipMemoryStore, dim, 0, 0, 0, 0) ? 0 : 1;
}

CLIP_DLLEXPORT ClipVar *
_clip_fetchref(ClipMachine * ClipMemoryStore, int dim)
{
   ClipVar *vp;

   vp = clip_fetch(ClipMemoryStore, dim, 0, 0, 0, 0);
  /*printf("%p\n",vp); */
   return vp;
}

static void
test_modify(ClipMachine * ClipMemoryStore, ClipVar * map, long hash)
{
   int ind;

  /* try run `modify` member */
  if (search_map(map->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar, map->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar, HASH_modify, &ind))
      {
	 ClipVar *ep;

	map->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
  ep = _clip_vptr(&(map->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[ind].ClipVar_var_of_ClipVarEl));

	 if (ep->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == PCODE_type_of_ClipVarType || ep->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CCODE_type_of_ClipVarType)
	    {
	       int i;
	       ClipVar stack[3];
			 ClipVar *rval = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - 1;

	       memset(stack, 0, sizeof(stack));

			 stack[0].ClipType_t_of_ClipVar = map->ClipType_t_of_ClipVar;
	       stack[0].ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR_ClipFlags;
			 stack[0].ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = map;
	       map->ClipType_t_of_ClipVar.count_of_ClipType++;

	       _clip_var_num(hash, stack + 1);
	       _clip_clone(ClipMemoryStore, stack + 2, rval);

	       _clip_eval(ClipMemoryStore, _clip_vptr(ep), 3, stack, rval);

	       for (i = 0; i < 3; i++)
		  _clip_destroy(ClipMemoryStore, stack + i);
	    }
		 map->ClipType_t_of_ClipVar.memo_of_ClipType = 1;
      }
}

CLIP_DLLEXPORT int
_clip_istore(ClipMachine * ClipMemoryStore, int dim, int byref)
{
   ClipVar *map = 0;
   long hash = 0;
   ClipVar *vp = clip_fetch(ClipMemoryStore, dim, 0, 1, &map, &hash);

   if (!vp)
      return 1;

   if (map)
      test_modify(ClipMemoryStore, map, hash);

   if (byref == 2)
      _clip_destroy(ClipMemoryStore, vp);
   else if (byref == 1)
      _clip_refassign(ClipMemoryStore, vp, 0);
   else
      _clip_iassign(ClipMemoryStore, vp);
   return 0;
}

CLIP_DLLEXPORT int
_clip_store(ClipMachine * ClipMemoryStore, int dim, int byref)
{
   ClipVar *map = 0;
   long hash = 0;
   ClipVar *vp = clip_fetch(ClipMemoryStore, dim, 0, 1, &map, &hash);

   if (!vp)
      return 1;

   if (map)
      test_modify(ClipMemoryStore, map, hash);

   if (byref == 2)
      {
	 _clip_destroy(ClipMemoryStore, vp);
	CLEAR_CLIPVAR(ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame);
	ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame++;
	 CLIP_CHECK_STACK;
      }
   else if (byref == 1)
      _clip_refassign(ClipMemoryStore, vp, 1);
   else
      _clip_assign(ClipMemoryStore, vp);
   return 0;
}

CLIP_DLLEXPORT int
_clip_opistore(ClipMachine * ClipMemoryStore, int dim, int op)
{
   ClipVar *vp = clip_fetch(ClipMemoryStore, dim, 0, 1, 0, 0);

   if (!vp)
      return 1;
   _clip_opiassign(ClipMemoryStore, vp, op);
   return 0;
}

CLIP_DLLEXPORT int
_clip_opstore(ClipMachine * ClipMemoryStore, int dim, int op)
{
   ClipVar *vp = clip_fetch(ClipMemoryStore, dim, 0, 1, 0, 0);

   if (!vp)
      return 1;
   _clip_opassign(ClipMemoryStore, vp, op);
   return 0;
}

/* ] arrays */
/* [ var */

CLIP_DLLEXPORT int
_clip_log(ClipVar * vp)
{
   vp = _clip_vptr(vp);

	switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
      {
      case NUMERIC_type_of_ClipVarType:
			if (vp->ClipType_t_of_ClipVar.memo_of_ClipType)
	    return rational_empty(vp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
	 else
	    return vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar == 0 ? 0 : 1;
      case LOGICAL_type_of_ClipVarType:
			return vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
      case CHARACTER_type_of_ClipVarType:
	 return vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf == 0 ? 0 : 1;
      case ARRAY_type_of_ClipVarType:
	 return vp->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar == 0 ? 0 : 1;
      case MAP_type_of_ClipVarType:
			return vp->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar == 0 ? 0 : 1;
      default:
	 break;
      }
   return 0;
}

CLIP_DLLEXPORT long
_clip_long(ClipVar * vp)
{
   vp = _clip_vptr(vp);

	switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
      {
      case NUMERIC_type_of_ClipVarType:
			if (vp->ClipType_t_of_ClipVar.memo_of_ClipType)
	    return (long) rational_toDouble(vp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
	 else
	    return (long) vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
      case DATE_type_of_ClipVarType:
			return vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar;
      case LOGICAL_type_of_ClipVarType:
			return vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
      case CHARACTER_type_of_ClipVarType:
	 return vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
      case ARRAY_type_of_ClipVarType:
	 return vp->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
      case MAP_type_of_ClipVarType:
			return vp->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar;
      default:
	 break;
      }
   return 0;
}

CLIP_DLLEXPORT double
_clip_double(ClipVar * vp)
{
   vp = _clip_vptr(vp);

	switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
      {
      case NUMERIC_type_of_ClipVarType:
			if (vp->ClipType_t_of_ClipVar.memo_of_ClipType)
	    return rational_toDouble(vp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
	 else
	    return vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
      case DATE_type_of_ClipVarType:
			return vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar;
      case LOGICAL_type_of_ClipVarType:
			return vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
      case CHARACTER_type_of_ClipVarType:
	 return vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
      case ARRAY_type_of_ClipVarType:
	 return vp->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
      case MAP_type_of_ClipVarType:
			return vp->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar;
      default:
	 break;
      }
   return 0;
}

static int
descr_str(ClipMachine * ClipMemoryStore, ClipVar * vp, char **str)
{
   int l;

   if (!vp)
      return 0;
   vp = _clip_vptr(vp);

	switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
      {
      case CHARACTER_type_of_ClipVarType:
	 return _clip_strFromVar(ClipMemoryStore, vp, str, &l);
	 break;
      case MAP_type_of_ClipVarType:
	 {
	    int ind, l;

	    if (!_clip_mind(ClipMemoryStore, vp, HASH_description, &ind))
	       {
		  ClipVar *ep, *ap;

		  ap = _clip_vptr(vp);

		 ep = &(ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[ind].ClipVar_var_of_ClipVarEl);
		  return _clip_strFromVar(ClipMemoryStore, ep, str, &l);
	       }
	 }
	 break;
      default:
	 break;
      }
   return 0;

}

CLIP_DLLEXPORT int
_clip_strFromVar(ClipMachine * ClipMemoryStore, ClipVar * vp, char **str, int *lenp)
{
   if (!vp)
      return 0;

   vp = _clip_vptr(vp);

   switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
      {
      case CHARACTER_type_of_ClipVarType:
	 {
	    ClipBuf buf = { 0, 0 };

		 int ret = _clip_expand(ClipMemoryStore, &buf, &vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar);

	    if (!ret)
	       {
		  *str = buf.buf_of_ClipBuf;
		  *lenp = buf.len_of_ClipBuf;
	       }
	    else
	       {
		  free(buf.buf_of_ClipBuf);
		  *str = 0;
		  *lenp = 0;
	       }
	    return ret;
	 }
      default:
	 *lenp = 0;
      }
   return 0;
}

CLIP_DLLEXPORT int
_clip_eval_macro(ClipMachine * ClipMemoryStore, char *str, int len, ClipVar * dest)
{
   char *s = str, *e;
   int simple = 1;

   if (len == 0)
      {
	 dest->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = UNDEF_type_of_ClipVarType;
	 dest->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	 return 0;
      }

   if (!(isalpha(*s) || *s == '_'))
      simple = 0;
   else
      for (s = str + 1, e = str + len; s < e; ++s)
	 if (!(isalnum(*s) || *s == '_'))
	    {
	       simple = 0;
	       break;
	    }

   if (simple)
      {
	 VarEntry *vp;
	 long hash = _clip_casehashbytes(0, str, len);

	 if (!_clip_take_field(ClipMemoryStore, hash, -1, dest))
	    return 0;

	 vp = fetch_var(ClipMemoryStore, hash);

	 if (!vp)
	    {
	       if (ClipMemoryStore->obj)
		  {
		     ClipVar *vpp;

		     vpp = fetch_obj(ClipMemoryStore, _clip_vptr(ClipMemoryStore->obj), hash);
		     if (vpp)
			return _clip_clone(ClipMemoryStore, dest, vpp);
		  }
	    }

	 if (!vp)
	    {
	       if (ClipMemoryStore->noerrblock)
		  {
		     ClipVar var;

		     memset(&var, 0, sizeof(var));
		     return _clip_clone(ClipMemoryStore, dest, &var);
		  }
	       else
		  {
		     _clip_trap_printf(ClipMemoryStore, __file__, __LINE__, "no variable name: '%.*s'", len, str);
		     return _clip_call_errblock(ClipMemoryStore, 1);
		    /*vp = add_private(ClipMemoryStore, hash); */
		  }
	    }
		 return _clip_clone(ClipMemoryStore, dest, &vp->ClipVar_var_of_VarEntry);
      }
   else
      {
	 ClipBlock block;
	 int ret = _clip_compile_Block(ClipMemoryStore, str, len, &block);
	 ClipVar stack[1];
	 ClipFrame frame = { stack, stack + 1, __file__, __LINE__, 0, 0, 0, 0, 0, 0,
	    "eval_macro", 1, 0
	 };
	 ClipFrame *inMacro = ClipMemoryStore->inMacro;

	 if (ret)
	    return -1;
	 memset(stack, 0, sizeof(stack));

	 ClipMemoryStore->inMacro = ClipMemoryStore->fp;
	 frame.ClipFrame_up_of_ClipFrame = ClipMemoryStore->fp;
	 ClipMemoryStore->fp = &frame;

	 ret = _clip_code_func(ClipMemoryStore, &block, 0, 0, 0);

	 ClipMemoryStore->inMacro = inMacro;
	 ClipMemoryStore->fp = frame.ClipFrame_up_of_ClipFrame;
	 *dest = stack[0];
	 destroy_Block(ClipMemoryStore, &block);
	 if (ret)
	    return 1;
	 else
	    return 0;
      }
}

CLIP_DLLEXPORT int
_clip_expand_var(ClipMachine * ClipMemoryStore, ClipVar * vp)
{
   ClipBuf dest;
   int ret;
   char *s, *e;

   vp = _clip_vptr(vp);

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
      {
			ret = _clip_expand(ClipMemoryStore, &dest, &(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar));
	 if (ret)
	    return ret;
      }
   else
      {
	 _clip_destroy(ClipMemoryStore, vp);
	 vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	 vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MSTAT_ClipFlags;
	 vp->ClipType_t_of_ClipVar.MACRO = 0;
	 vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = "";
	 vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = 0;
	 return 0;
      }
   _clip_destroy(ClipMemoryStore, vp);
   vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = 0;
   vp->ClipType_t_of_ClipVar.MACRO = 1;
	vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar = dest;
   for (s = dest.buf_of_ClipBuf, e = dest.buf_of_ClipBuf + dest.len_of_ClipBuf; s < e; ++s)
      if (*s == '&')
	 {
	    ret = 1;
	    break;
	 }
   if (!ret)
      vp->ClipType_t_of_ClipVar.MACRO = 0;
   return 0;
}

static int
find_macro(char *s, char *e, char **beg, char **end, char **next, int *dot)
{
   *dot = 0;
   if (s >= e)
      return 0;
   for (; s < e; ++s)
      if (*s == '&')
	 {
	    ++s;
	    if (s >= e)
	       return 0;
	    *beg = s;
	    if (*s == '.')
	       *dot = 1;
	    if (*s == '\\')
	       {
		  ++s;
		  if (s >= e)
		     return 0;
		  switch (*s)
		     {
		     case 'x':
		     case 'X':
		     case '0':
		     case '1':
		     case '2':
		     case '3':
		     case '4':
		     case '5':
		     case '6':
		     case '7':
			goto cont;
		     }
		  ++s;
		  *end = s;
		  *next = s;
		  return 1;
	       }
	    else
	       {
		  if (*s == '.')
		     *dot = 1;

		cont:

		  for (; s <= e; ++s)
		     if (s == e || !(isalnum(*s) || *s == '_'))
			{
			   *end = s;
			   if (*s == '.')
			      {
				 ++s;
				 *dot = 1;
			      }
			   *next = s;
			   return 1;
			}
	       }
	    *end = s;
	    ++s;
	    *next = s;
	 }
   return 0;
}

static int
octdig(int ch)
{
   return ch - '0';
}

static int
hexdig(int ch)
{
   switch (ch)
      {
      case '0':
	 return 0;
      case '1':
	 return 1;
      case '2':
	 return 2;
      case '3':
	 return 3;
      case '4':
	 return 4;
      case '5':
	 return 5;
      case '6':
	 return 6;
      case '7':
	 return 7;
      case '8':
	 return 8;
      case '9':
	 return 9;
      case 'A':
      case 'a':
	 return 10;
      case 'B':
      case 'b':
	 return 11;
      case 'C':
      case 'c':
	 return 12;
      case 'D':
      case 'd':
	 return 13;
      case 'E':
      case 'e':
	 return 14;
      case 'F':
      case 'f':
	 return 15;
      }
   return 0;
}

CLIP_DLLEXPORT int
_clip_expand(ClipMachine * ClipMemoryStore, ClipBuf * dest, ClipBuf * src)
{
   char *s, *e, *beg, *end, *next;
   int dot;
   OutBuf buf;

   init_Buf(&buf);

   s = src->buf_of_ClipBuf;
   e = s + src->len_of_ClipBuf;
   for (; find_macro(s, e, &beg, &end, &next, &dot); s = next)
      {
	 long hash;
	 VarEntry *vp;
	 ClipVar *vpp;
	 int ch;

	 putBuf_Buf(&buf, s, (beg - 1) - s);
	 if (end == beg)
	    {
	       putByte_Buf(&buf, '&');
	       if (dot)
		  putByte_Buf(&buf, '.');
	       continue;
	    }

	 if (*beg == '\\')
	    {
	       ++beg;
	       if (end == beg)
		  {
		     putByte_Buf(&buf, '&');
		     putByte_Buf(&buf, '\\');
		     continue;
		  }

	       switch ((ch = *beg))
		  {
		  case 'x':
		  case 'X':
		     {
			++beg;
			if ((end - beg) != 2)
			   {
			    undo:
			      putByte_Buf(&buf, '&');
			      putByte_Buf(&buf, '\\');
			      while (beg < end)
				 putByte_Buf(&buf, *beg++);
			      continue;
			   }
			else
			   {
			      unsigned char b1, b2;

			      b1 = hexdig(beg[0]);
			      b2 = hexdig(beg[1]);
			      putByte_Buf(&buf, b1 << 4 | b2);
			   }
		     }
		     continue;
		  case '0':
		  case '1':
		  case '2':
		  case '3':
		  case '4':
		  case '5':
		  case '6':
		  case '7':
		     {
			unsigned char b1, b2, b3;

			if ((end - beg) != 3)
			   goto undo;
			b1 = octdig(beg[0]);
			b2 = octdig(beg[1]);
			b3 = octdig(beg[2]);
			putByte_Buf(&buf, b1 << 6 | b2 << 3 | b3);
		     }
		     continue;
		  default:
		     if ((end - beg) != 1)
			goto undo;
		     switch (ch)
			{
			case 'n':
			   putByte_Buf(&buf, '\n');
			   break;
			case 'r':
			   putByte_Buf(&buf, '\r');
			   break;
			case 't':
			   putByte_Buf(&buf, '\t');
			   break;
			case 'v':
			   putByte_Buf(&buf, '\v');
			   break;
			case 'a':
			   putByte_Buf(&buf, 7);
			   break;
			case 'e':
			   putByte_Buf(&buf, 27);
			   break;
			default:
			   putByte_Buf(&buf, ch);
			   break;
			}
		     continue;
		  }
	    }

	 hash = _clip_casehashbytes(0, beg, end - beg);
	 vp = fetch_var(ClipMemoryStore, hash);
#if 0
	 if (!vp)
	    {
	       _clip_trap_str(ClipMemoryStore, __file__, __LINE__, "undefined macro-variable in string");
	       goto _trap;
	    }
#endif
#if 0
	 if (!(vp->var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType & CHARACTER_type_of_ClipVarType))
	    {
	       _clip_trap_str(ClipMemoryStore, __file__, __LINE__, "non-character result of macro in string");
	       goto _trap;
	    }
#endif
	 if (vp && !(ClipMemoryStore->flags1 & NOEXPAND_MACRO_FLAG))
	    {
			 vpp = _clip_vptr(&vp->ClipVar_var_of_VarEntry);
	       switch (vpp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
		  {
		  case CHARACTER_type_of_ClipVarType:
		     putBuf_Buf(&buf, vpp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, vpp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
		     break;
		  case NUMERIC_type_of_ClipVarType:
		     {
			if (vpp->ClipType_t_of_ClipVar.memo_of_ClipType)
			   {
			      char *bf;

			      if (vpp->ClipType_t_of_ClipVar.len_of_ClipType)
				 bf = rational_toString(vpp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, 10, vpp->ClipType_t_of_ClipVar.dec_of_ClipType, 0);
			      else
				 bf = rational_toString(vpp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, 10, ClipMemoryStore->decimals, 0);
			      putBuf_Buf(&buf, bf, strlen(bf));
			      free(bf);
			   }
			else
			   {
			      char bf[32];

			      if (vpp->ClipType_t_of_ClipVar.len_of_ClipType)
				 snprintf(bf, sizeof(bf), "%*.*f", vpp->ClipType_t_of_ClipVar.len_of_ClipType, vpp->ClipType_t_of_ClipVar.dec_of_ClipType, vpp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
			      else
				 snprintf(bf, sizeof(bf), "%g", vpp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
			      putBuf_Buf(&buf, bf, strlen(bf));
			   }
		     }
		     break;
		  case LOGICAL_type_of_ClipVarType:
			  if (vpp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
			putBuf_Buf(&buf, ".T.", 3);
		     else
			putBuf_Buf(&buf, ".F.", 3);
		     break;
		  case DATE_type_of_ClipVarType:
		     {
			int dd, mm, yy, ww;
			char bf[32];

			_clip_cdate(vpp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar, &dd, &mm, &yy, &ww);
			sprintf(bf, "%02d/%02d/%d", dd, mm, yy);
			putBuf_Buf(&buf, bf, strlen(bf));
		     }
		     break;
		  case DATETIME_type_of_ClipVarType:
		     {
			int len;
			char *r = _clip_ttoc(ClipMemoryStore, vpp->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar, vpp->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar, &len,
					     ClipMemoryStore->date_format, ClipMemoryStore->hours,
					     ClipMemoryStore->seconds);

			putBuf_Buf(&buf, r, len);
			free(r);
		     }
		     break;
		  default:
		     goto badtype;
		     break;
		  }
	    }
	 else
	    {
	     badtype:
	       putByte_Buf(&buf, '&');
	       while (beg < end)
		  putByte_Buf(&buf, *beg++);
	       if (dot)
		  putByte_Buf(&buf, '.');
	    }
      }
  /* rest of string */
   if (s < e)
      putBuf_Buf(&buf, s, e - s);

	dest->len_of_ClipBuf = buf.ptr_of_OutBuf - buf.buf_of_OutBuf;
   putByte_Buf(&buf, 0);
	dest->buf_of_ClipBuf = buf.buf_of_OutBuf;
   return 0;
}

CLIP_DLLEXPORT int
_clip_macro(ClipMachine * ClipMemoryStore)
{
	ClipVar *vp = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - 1;

   vp = _clip_vptr(vp);

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
      {
	 int l = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
	 char *s = _clip_memdup(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, l);
	ClipVar *vpp = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - 1;
	 int r;

	 _clip_destroy(ClipMemoryStore, vpp);

	 r = _clip_eval_macro(ClipMemoryStore, s, l, vpp);
	 free(s);

	 return r;
      }
   else
      {
	 _clip_trap_printf(ClipMemoryStore, __file__, __LINE__, "macro operation with non-character argument (%s)", _clip_typename(vp));
	 return _clip_call_errblock(ClipMemoryStore, 1);
      }
}

CLIP_DLLEXPORT void
_clip_var_str(const char *str, int len, ClipVar * dest)
{
   dest->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = (char *) _clip_memdup(str, len);
   dest->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = len;
   dest->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   dest->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   dest->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
}

CLIP_DLLEXPORT void
_clip_var_num(double num, ClipVar * vp)
{
   vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
   vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   vp->ClipType_t_of_ClipVar.len_of_ClipType = 0;
   vp->ClipType_t_of_ClipVar.dec_of_ClipType = 0;
   vp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
   vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = num;
}

CLIP_DLLEXPORT void
_clip_var_log(int val, ClipVar * vp)
{
   vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
   vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = val;
}

/* ] var */
/* [ pcount */

CLIP_DLLEXPORT void
_clip_pcount(ClipMachine * ClipMemoryStore)
{
	ClipVar *vp = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame;

   vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
   vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   vp->ClipType_t_of_ClipVar.len_of_ClipType = 0;
   vp->ClipType_t_of_ClipVar.dec_of_ClipType = 0;
   vp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
   vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = ClipMemoryStore->argc;
	ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame++;
   CLIP_CHECK_STACK;
}

CLIP_DLLEXPORT void
_clip_pshift(ClipMachine * ClipMemoryStore)
{
   --ClipMemoryStore->argc;
   _clip_pcount(ClipMemoryStore);
}

CLIP_DLLEXPORT int
_clip_parn(ClipMachine * ClipMemoryStore)
{
   int no;
	ClipVar *sp = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - 1;

   no = _clip_long(sp);

   _clip_destroy(ClipMemoryStore, sp);

   if (no < 1 || no > ClipMemoryStore->argc)
      return 0;

	ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame--;
   _clip_push(ClipMemoryStore, ARGPTR(ClipMemoryStore, no));
   return 0;
}

/* ] pcount */
/* [ Object calls */

static int obj_call(struct ClipMachine *ClipMemoryStore, int argc, long hash, int call);

#define CALL_CALL 0
#define CALL_GET 1
#define CALL_SET 2

CLIP_DLLEXPORT int
_clip_call(struct ClipMachine *ClipMemoryStore, int argc, long hash)
{
   return obj_call(ClipMemoryStore, argc, hash, CALL_CALL);
}

static int
obj_call(struct ClipMachine *ClipMemoryStore, int argc, long hash, int call)
{
	ClipVar *op = _clip_vptr(ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - argc);
   int ret = 0;
   int _argc = ClipMemoryStore->argc;
   ClipFrame *fp = ClipMemoryStore->fp;
   ClipVar *bp = ClipMemoryStore->bp;
   ClipFrame *inMacro = ClipMemoryStore->inMacro;

   ClipMemoryStore->inMacro = 0;
   switch (op->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
      {
      case OBJECT_type_of_ClipVarType:
	 ClipMemoryStore->argc = argc;
	 ClipMemoryStore->bp = fp->ClipVar_sp_of_ClipFrame;

	 switch (call)
	    {
	    case CALL_CALL:
			 ret = op->ClipObjVar_o_of_ClipVar.rtti_of_ClipObjVar->call_of_ClipObjRtti(ClipMemoryStore, op->ClipObjVar_o_of_ClipVar.obj_of_ClipObjVar, op->ClipObjVar_o_of_ClipVar.rtti_of_ClipObjVar,
hash);
	       break;
	    case CALL_GET:
			 ret = op->ClipObjVar_o_of_ClipVar.rtti_of_ClipObjVar->get_of_ClipObjRtti(ClipMemoryStore, op->ClipObjVar_o_of_ClipVar.obj_of_ClipObjVar, op->ClipObjVar_o_of_ClipVar.rtti_of_ClipObjVar,
hash);
	       break;
	    case CALL_SET:
			 ret = op->ClipObjVar_o_of_ClipVar.rtti_of_ClipObjVar->set_of_ClipObjRtti(ClipMemoryStore, op->ClipObjVar_o_of_ClipVar.obj_of_ClipObjVar, op->ClipObjVar_o_of_ClipVar.rtti_of_ClipObjVar,
hash);
	       break;
	    }

	 ClipMemoryStore->argc = _argc;
	 ClipMemoryStore->fp = fp;
	 ClipMemoryStore->bp = bp;
	 for (; argc; --argc)
	    {
			 --(fp->ClipVar_sp_of_ClipFrame);
			 _clip_destroy(ClipMemoryStore, fp->ClipVar_sp_of_ClipFrame);
	    }

	 break;
      case MAP_type_of_ClipVarType:
	 {
	    ClipVarEl *vlp;
	    ClipVar *vpp;
	    enum ClipVarType type;

		 vlp = get_map_el(&op->ClipMapVar_m_of_ClipVar, hash);
	    if (!vlp)
	       {
		  char buf[32];

		  _clip_hash_name(ClipMemoryStore, hash, buf, sizeof(buf));
		  _clip_trap_printf(ClipMemoryStore, __file__, __LINE__, "no method %s", buf);
		  ret = EG_NOMETHOD;
		  break;
	       }
		    vpp = &vlp->ClipVar_var_of_ClipVarEl;
	    type = _clip_type(vpp);

	    if (type != CCODE_type_of_ClipVarType && type != PCODE_type_of_ClipVarType)
	       {
		  if (argc == 1)
		     {
				  ClipVar *rp = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - argc - 1;

			_clip_clone(ClipMemoryStore, rp, vpp);
		  --(fp->ClipVar_sp_of_ClipFrame);
		 _clip_destroy(ClipMemoryStore, fp->ClipVar_sp_of_ClipFrame);
			ret = 0;
		     }
		  else
		     {
			char buf[32];

			_clip_hash_name(ClipMemoryStore, hash, buf, sizeof(buf));
			_clip_trap_printf(ClipMemoryStore, __file__, __LINE__, "method %s is not code", buf);
			ret = EG_NOMETHOD;
		     }
		  break;
	       }

	    vpp = _clip_vptr(vpp);

	    if (type == CCODE_type_of_ClipVarType)
	       ret = _clip_func(ClipMemoryStore, vpp->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipFunction_function_of_ClipCodeVar, argc, 0,
vpp->ClipCodeVar_c_of_ClipVar.ClipVarFrame_uplocals_of_ClipCodeVar);
	    else
	       ret = _clip_code_func(ClipMemoryStore, vpp->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipBlock_block_of_ClipCodeVar, argc, 0,
vpp->ClipCodeVar_c_of_ClipVar.ClipVarFrame_uplocals_of_ClipCodeVar);

	 }
	 break;
      default:
	 ret = _clip_func_hash(ClipMemoryStore, hash, argc, 0, 0);
	 break;
      }

   ClipMemoryStore->inMacro = inMacro;
   if (ret)
      return _clip_call_errblock(ClipMemoryStore, ret);

   return ret;
}

CLIP_DLLEXPORT int
_clip_get(struct ClipMachine *ClipMemoryStore, long hash)
{
   return obj_call(ClipMemoryStore, 0, hash, CALL_GET);
}

CLIP_DLLEXPORT int
_clip_set(struct ClipMachine *ClipMemoryStore, long hash)
{
   return obj_call(ClipMemoryStore, 1, hash, CALL_SET);
}

/* ] Object calls */
/* [ OutBuf */

void
init_Buf(OutBuf * bp)
{
	bp->buf_of_OutBuf = bp->ptr_of_OutBuf = (char *) malloc(64);
	bp->end_of_OutBuf = bp->buf_of_OutBuf + 64;
}

void
destroy_Buf(OutBuf * bp)
{
   if (!bp)
      return;
	if (bp->buf_of_OutBuf)
		free(bp->buf_of_OutBuf);
}

static void
grow_buf(OutBuf * bp, int min)
{
	int size = bp->end_of_OutBuf - bp->buf_of_OutBuf;
   int dsize = size / 2;
	int pos = bp->ptr_of_OutBuf - bp->buf_of_OutBuf;

   if (min < 16)
      min = 16;
   if (dsize < min)
      dsize = min;
   size += dsize;
	bp->buf_of_OutBuf = realloc(bp->buf_of_OutBuf, size);
	bp->end_of_OutBuf = bp->buf_of_OutBuf + size;
	bp->ptr_of_OutBuf = bp->buf_of_OutBuf + pos;
}

void
putByte_Buf(OutBuf * bp, unsigned char b)
{
	*bp->ptr_of_OutBuf = b;
	++(bp->ptr_of_OutBuf);
	if (bp->ptr_of_OutBuf >= bp->end_of_OutBuf)
      grow_buf(bp, 0);
}

void
putBuf_Buf(OutBuf * bp, char *p, int len)
{
	if ((bp->end_of_OutBuf - bp->ptr_of_OutBuf) <= len)
      grow_buf(bp, len + 1);
	memcpy(bp->ptr_of_OutBuf, p, len);
	bp->ptr_of_OutBuf += len;
}

void
putShort_Buf(OutBuf * bp, short s)
{
   putBuf_Buf(bp, (char *) &s, sizeof(s));
}

void
putLong_Buf(OutBuf * bp, long l)
{
   putBuf_Buf(bp, (char *) &l, sizeof(l));
}

void
putInt32_Buf(OutBuf * bp, long l)
{
   int r = (int) l;
   putBuf_Buf(bp, (char *) &r, sizeof(r));
}

void
putDouble_Buf(OutBuf * bp, double d)
{
   putBuf_Buf(bp, (char *) &d, sizeof(d));
}

void
putStr_Buf(OutBuf * bp, char *str)
{
   putBuf_Buf(bp, str, strlen(str) + 1);
}

/* ] OutBuf */
/* [ utils */
/*
 *    �����-> ������ *    ww - �������� (0 - �� 1 - �����)
 */
CLIP_DLLEXPORT void
_clip_cdate(long ju, int *dd, int *mm, int *yy, int *ww)
{
   long alf, a, b, c, d, e;

   alf = (long) ((ju - 1867216.25) / 36524.25);
   a = ju < 2299161L ? ju : ju + 1L + alf - alf / 4;
   b = a + 1524;
   c = (long) ((b - 122.1) / 365.25);
   d = (long) (365.25 * c);
   e = (long) ((b - d) / 30.6001);
   *dd = b - d - (long) (30.6001 * e);
   if (e < 14)
      *mm = e - 1;
   else
      *mm = e - 13;
   if (*mm > 2)
      *yy = c - 4716;
   else
      *yy = c - 4715;
   *ww = (ju + 1) % 7L;
   if (*yy <= 0)
      *yy = 1;
}

/*
 *    ������-> ����� */
CLIP_DLLEXPORT long
_clip_jdate(int dd, int mm, int yy)
{
   int a, b;

   b = 0;
   if (mm <= 2)
      {
	 yy--;
	 mm += 12;
      }
   if ((yy * 10000.0 + mm * 100.0 + dd) >= 15821015.0)
      {
	 a = yy / 100;
	 b = 2 - a + a / 4;
      }
   return ((long) (365.25 * yy - (yy ? 0.0 : 0.75)) + (long) (30.6001 * (mm + 1) + dd + 1720995 + b));
}

CLIP_DLLEXPORT int
_clip_parinfo(ClipMachine * ClipMemoryStore, int num)
{
   if (num == 0)
      return ClipMemoryStore->argc;
   if (num < 0 || num > ClipMemoryStore->argc)
      return 0;

   return _clip_type(ClipMemoryStore->bp - (ClipMemoryStore->argc - (num - 1)));
}

CLIP_DLLEXPORT int
_clip_parni(ClipMachine * ClipMemoryStore, int num)
{
   return (int) _clip_parnd(ClipMemoryStore, num);
}

CLIP_DLLEXPORT long
_clip_parnl(ClipMachine * ClipMemoryStore, int num)
{
   return (long) _clip_parnd(ClipMemoryStore, num);
}

CLIP_DLLEXPORT ClipVar *
_clip_par(ClipMachine * ClipMemoryStore, int num)
{
   if (num > 0 && num <= ClipMemoryStore->argc)
      {
	 ClipVar *vp = (ClipMemoryStore->bp - (ClipMemoryStore->argc - (num - 1)));

	 return _clip_vptr(vp);
      }
   return 0;
}

CLIP_DLLEXPORT ClipVar *
_clip_spar(ClipMachine * ClipMemoryStore, int num)
{
   if (num > 0 && num <= ClipMemoryStore->argc)
      {
	 ClipVar *vp = (ClipMemoryStore->bp - (ClipMemoryStore->argc - (num - 1)));

	 return vp;
      }
   return 0;
}

CLIP_DLLEXPORT int
_clip_par_isref(ClipMachine * ClipMemoryStore, int num)
{
   ClipVar *vp = _clip_spar(ClipMemoryStore, num);

   if (!vp)
      return 0;
   if (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags)
      return 1;
   return 0;
}

CLIP_DLLEXPORT int
_clip_par_assign(ClipMachine * ClipMemoryStore, int num, ClipVar * vp)
{
   ClipVar *lp = _clip_spar(ClipMemoryStore, num);

   if (!lp || !vp)
      return -1;
   if (!(lp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags))
      return 1;
   return _clip_mclone(ClipMemoryStore, lp, vp);
}

CLIP_DLLEXPORT int
_clip_par_assign_str(ClipMachine * ClipMemoryStore, int num, char *str, int len)
{
   return _clip_par_assign_str_m(ClipMemoryStore, num, _clip_memdup(str, len), len);
}

CLIP_DLLEXPORT int
_clip_par_assign_str_m(ClipMachine * ClipMemoryStore, int num, char *str, int len)
{
   ClipVar v;
   int r;

   if (!_clip_par_isref(ClipMemoryStore, num))
      return -1;
   CLEAR_CLIPVAR(&v);
	v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = str;
	v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = len;
   v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   v.ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   v.ClipType_t_of_ClipVar.memo_of_ClipType = 0;
   r = _clip_par_assign(ClipMemoryStore, num, &v);
   _clip_destroy(ClipMemoryStore, &v);
   return r;
}

CLIP_DLLEXPORT double
_clip_parnd(ClipMachine * ClipMemoryStore, int num)
{
   if (num > 0 && num <= ClipMemoryStore->argc)
      {
	 ClipVar *vp = _clip_vptr((ClipMemoryStore->bp - (ClipMemoryStore->argc - (num - 1))));

	 if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType && !vp->ClipType_t_of_ClipVar.memo_of_ClipType)
	    {
	       return vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
	    }
	 if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType && vp->ClipType_t_of_ClipVar.memo_of_ClipType)
	    return rational_toDouble(vp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
      }
   return 0;
}

CLIP_DLLEXPORT int
_clip_parp(ClipMachine * ClipMemoryStore, int num, int *len, int *dec)
{
   *len = *dec = 0;

   if (num > 0 && num <= ClipMemoryStore->argc)
      {
	 ClipVar *vp = (ClipMemoryStore->bp - (ClipMemoryStore->argc - (num - 1)));

	 if (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags)
	    vp = vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
	 *len = vp->ClipType_t_of_ClipVar.len_of_ClipType;
	 *dec = vp->ClipType_t_of_ClipVar.dec_of_ClipType;
	 return 0;
      }

   return -1;
}

CLIP_DLLEXPORT int
_clip_parl(ClipMachine * ClipMemoryStore, int num)
{
   if (num > 0 && num <= ClipMemoryStore->argc)
      {
	 ClipVar *vp = (ClipMemoryStore->bp - (ClipMemoryStore->argc - (num - 1)));
	 enum ClipVarType type = _clip_type(vp);

	 if (type == LOGICAL_type_of_ClipVarType)
	    {
	       if (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags)
		  vp = vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
		  return vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
	    }
      }
   return 0;
}

CLIP_DLLEXPORT char *
_clip_parcl(ClipMachine * ClipMemoryStore, int num, int *len)
{
   if (num > 0 && num <= ClipMemoryStore->argc)
      {
	 ClipVar *vp = (ClipMemoryStore->bp - (ClipMemoryStore->argc - (num - 1)));
	 enum ClipVarType type = _clip_type(vp);

	 if (type == CHARACTER_type_of_ClipVarType)
	    {
	       if (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags)
		  vp = vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
	       if (len)
		  *len = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
	       return vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
	    }
      }
   return 0;
}

CLIP_DLLEXPORT char *
_clip_parc(ClipMachine * ClipMemoryStore, int num)
{
   return _clip_parcl(ClipMemoryStore, num, 0);
}

CLIP_DLLEXPORT void
_clip_retndp(ClipMachine * ClipMemoryStore, double n, int len, int dec)
{
   ClipVar *vp = ClipMemoryStore->bp - ClipMemoryStore->argc - 1;

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType && vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf != NULL)
      free(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);

   vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
   vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   vp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
   vp->ClipType_t_of_ClipVar.len_of_ClipType = len;
   vp->ClipType_t_of_ClipVar.dec_of_ClipType = dec;
   if (!finite(n))
      n = 0;
   vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = n;
}

CLIP_DLLEXPORT void
_clip_retni(ClipMachine * ClipMemoryStore, int n)
{
   _clip_retndp(ClipMemoryStore, n, 0, 0);
}

CLIP_DLLEXPORT void
_clip_retnl(ClipMachine * ClipMemoryStore, long n)
{
   _clip_retndp(ClipMemoryStore, n, 0, 0);
}

CLIP_DLLEXPORT void
_clip_retnd(ClipMachine * ClipMemoryStore, double n)
{
   _clip_retndp(ClipMemoryStore, n, 0, 0);
}

CLIP_DLLEXPORT void
_clip_retnr(ClipMachine * ClipMemoryStore, rational * r, int len, int dec)
{
   ClipVar *vp = ClipMemoryStore->bp - ClipMemoryStore->argc - 1;

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType && vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf != NULL)
      free(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);

   vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
   vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   vp->ClipType_t_of_ClipVar.len_of_ClipType = len;
   vp->ClipType_t_of_ClipVar.dec_of_ClipType = dec;
   vp->ClipType_t_of_ClipVar.memo_of_ClipType = 1;
   vp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar = r;
}

CLIP_DLLEXPORT void
_clip_retl(ClipMachine * ClipMemoryStore, int n)
{
   ClipVar *vp = ClipMemoryStore->bp - ClipMemoryStore->argc - 1;

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType && vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf != NULL)
      free(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);

   vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
   vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = n;
}

CLIP_DLLEXPORT void
_clip_retcn(ClipMachine * ClipMemoryStore, char *s, int l)
{
   ClipVar *vp = ClipMemoryStore->bp - ClipMemoryStore->argc - 1;

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType && vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf != NULL)
      free(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);

   vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;

   if (!s)
      {
	 l = 0;
	 s = "";
      }

   vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = _clip_memdup(s, l);
   vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = l;
}

CLIP_DLLEXPORT void
_clip_retcn_m(ClipMachine * ClipMemoryStore, char *s, int l)
{
   ClipVar *vp = RETPTR(ClipMemoryStore);

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType && vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf != NULL)
      free(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
   memset(vp, 0, sizeof(ClipVar));
   vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;

   vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = s;
   vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = l;
}

CLIP_DLLEXPORT void
_clip_retc(ClipMachine * ClipMemoryStore, char *s)
{
   int l = 0;

   if (s)
      l = strlen(s);
   _clip_retcn(ClipMemoryStore, s, l);
}

CLIP_DLLEXPORT int
_clip_storclen(ClipMachine * ClipMemoryStore, const char *str, int len, int num, int ind)
{
#if 1
   ClipVar *vp = _clip_spar(ClipMemoryStore, num);
   ClipVar lp;

   if (!vp)
      return 0;

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
      {
	 if (ind < 0 || ind >= vp->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar)
	    return 0;
	 vp = _clip_vptr(vp->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + ind);
      }

   if (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType == F_MSTAT_ClipFlags)
      return 0;

   _clip_var_str(str, len, &lp);
   _clip_mclone(ClipMemoryStore, vp, &lp);
   _clip_destroy(ClipMemoryStore, &lp);
#else
   ClipVar *vp = _clip_par(ClipMemoryStore, num);

   if (!vp)
      return 0;

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
      {
	 if (ind < 0 || ind >= vp->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar)
	    return 0;
	 vp = _clip_vptr(vp->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + ind);
      }

   if (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType == F_MSTAT_ClipFlags)
      return 0;

   _clip_destroy(ClipMemoryStore, vp);
   _clip_var_str(str, len, vp);
#endif
   return 1;
}

CLIP_DLLEXPORT int
_clip_storc(ClipMachine * ClipMemoryStore, const char *str, int num, int ind)
{
   return _clip_storclen(ClipMemoryStore, str, strlen(str), num, ind);
}

CLIP_DLLEXPORT int
_clip_stornd(ClipMachine * ClipMemoryStore, double d, int num, int ind)
{
#if 1
   ClipVar *vp = _clip_spar(ClipMemoryStore, num);
   ClipVar lp;

   if (!vp)
      return 0;

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
      {
	 if (ind < 0 || ind >= vp->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar)
	    return 0;
	 vp = _clip_vptr(vp->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + ind);
      }

   if (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType == F_MSTAT_ClipFlags)
      return 0;

   _clip_var_num(d, &lp);
   _clip_mclone(ClipMemoryStore, vp, &lp);
   _clip_destroy(ClipMemoryStore, &lp);
#else
   ClipVar *vp = _clip_par(ClipMemoryStore, num);

   if (!vp)
      return 0;

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
      {
	 if (ind < 0 || ind >= vp->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar)
	    return 0;
	 vp = _clip_vptr(vp->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + ind);
      }

   if (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType == F_MSTAT_ClipFlags)
      return 0;

   _clip_destroy(ClipMemoryStore, vp);
   _clip_var_num(d, vp);
#endif
   return 1;
}

CLIP_DLLEXPORT int
_clip_storni(ClipMachine * ClipMemoryStore, int d, int num, int ind)
{
   return _clip_stornd(ClipMemoryStore, d, num, ind);
}

CLIP_DLLEXPORT int
_clip_stornl(ClipMachine * ClipMemoryStore, long d, int num, int ind)
{
   return _clip_stornd(ClipMemoryStore, d, num, ind);
}

CLIP_DLLEXPORT int
_clip_storl(ClipMachine * ClipMemoryStore, int val, int num, int ind)
{
   ClipVar *vp = _clip_par(ClipMemoryStore, num);

   if (!vp)
      return 0;

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
      {
	 if (ind < 0 || ind >= vp->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar)
	    return 0;
	 vp = _clip_vptr(vp->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + ind);
      }

   if (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType == F_MSTAT_ClipFlags)
      return 0;

   _clip_destroy(ClipMemoryStore, vp);
   _clip_var_log(val, vp);

   return 1;
}

static int
incr_var(ClipMachine * ClipMemoryStore, ClipVar * vp, int isIncr)
{
   vp = _clip_vptr(vp);

   switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
      {
      case NUMERIC_type_of_ClipVarType:
	 if (vp->ClipType_t_of_ClipVar.memo_of_ClipType)
	    {
	       rational *tmp = rational_long_init((long) 1);

	       rational_adda(vp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, tmp);
	       rational_destroy(tmp);
	    }
	 else
	    vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar += (isIncr ? 1 : -1);
	 return 0;
      case DATE_type_of_ClipVarType:
			vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar += (isIncr ? 1 : -1);
	 return 0;
      default:
	 _clip_trap_str(ClipMemoryStore, __file__, __LINE__, "invalid type in INCR/DECR operation");
	 return _clip_call_errblock(ClipMemoryStore, 1);
      }
}

CLIP_DLLEXPORT int
_clip_incr(struct ClipMachine *ClipMemoryStore, void *lval, int isIncr, int isPost)
{
   int ret;
   ClipVar *vp = (ClipVar *) lval;

   if (isPost)
      {
	 if ((ret = _clip_push(ClipMemoryStore, vp)))
	    return ret;
	 if ((ret = incr_var(ClipMemoryStore, vp, isIncr)))
	    return ret;
      }
   else
      {
	 if ((ret = incr_var(ClipMemoryStore, vp, isIncr)))
	    return ret;
	 if ((ret = _clip_push(ClipMemoryStore, vp)))
	    return ret;
      }
   return 0;
}

CLIP_DLLEXPORT ClipVar *
_clip_unptr(ClipVar * vp)
{
   return _clip_vptr(vp);
}

CLIP_DLLEXPORT int
_clip_find_obj(ClipMachine * ClipMemoryStore, long otype, ClipObjRtti ** rtti)
{
   ClipObjRtti **rp;

	for (rp = init_struct->_objrtti_of_ClipInitStruct; *rp; ++rp)
		if ((*rp)->hashId_of_ClipObjRtti == otype)
	 {
	    *rtti = *rp;
	    return 0;
	 }

   return 1;
}

static int
do_main(ClipMachine * ClipMemoryStore, long hash, ClipFunction * func, ClipBlock * block, int argc, char **argv, char **envp)
{
   int i, ret;
#ifdef OS_MINGW
   ClipVar *_stack = (ClipVar *) malloc(sizeof(ClipVar) * (argc + 1));
#else
   ClipVar *_stack = (ClipVar *) alloca(sizeof(ClipVar) * (argc + 1));
#endif
   ClipFrame _frame = { _stack, _stack, "" /*__FILE__*//* ": do_main()" */ , /*__LINE__*/ 0, 0,
      0, 0, 0, 0, 0, 0, argc + 1, 0
   };

	_frame.ClipFrame_up_of_ClipFrame = ClipMemoryStore->fp;
   ClipMemoryStore->fp = &_frame;
   if (_clip_init(ClipMemoryStore, argc, argv, envp))
      goto _trap;
   argc = _clip_argc;
   argv = _clip_argv;

   if (!argc)
      argc = 1;
   ClipMemoryStore->argc = argc - 1;
	_frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = UNDEF_type_of_ClipVarType;
	_frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	_frame.ClipVar_sp_of_ClipFrame++;
   CLIP_CHECK_STACK;
	for (i = 1; i < argc; ++i, ++_frame.ClipVar_sp_of_ClipFrame)
		_clip_var_str(argv[i], strlen(argv[i]), _frame.ClipVar_sp_of_ClipFrame);

   init_machine(ClipMemoryStore);
   if (func)
      ret = do_call(ClipMemoryStore, func, argc - 1, 0, 0, 0);
   else if (block)
      ret = _clip_code_func(ClipMemoryStore, block, argc - 1, 0, 0);
   else
      ret = _clip_func_hash(ClipMemoryStore, hash, argc - 1, 0, 0);

   if (ret)
      {
	 _clip_trap(ClipMemoryStore, __file__, __LINE__);
	 goto _trap;
      }

   ret = _clip_long(_stack);
   goto _return;
 _trap:
   {
      char *msg = 0;

      descr_str(ClipMemoryStore, ClipMemoryStore->trapVar, &msg);
      if (!msg)
	 msg = "unknown runtime error";
      printf("\n%s: runtime error: %s\n", _clip_progname, msg);
      _clip_logg(1, "%s: runtime error: %s", _clip_progname, msg);
      for (i = 0; i < ClipMemoryStore->trapTrace.count_of_ClipVect; ++i)
	 {
	    printf("trace: %s\n", (char *) ClipMemoryStore->trapTrace.items_of_ClipVect[i]);
	    _clip_logg(1, "trace: %s", (char *) ClipMemoryStore->trapTrace.items_of_ClipVect[i]);
	 }
      ret = 1;
   }
 _return:
   _clip_resume(ClipMemoryStore, 0, 0);
   if (ret == 0)
      ret = errorlevel;
#ifdef OS_MINGW
   free(_stack);
#endif
   return ret;
}

CLIP_DLLEXPORT int
_clip_main(ClipMachine * ClipMemoryStore, long hash, int argc, char **argv, char **envp)
{
   int ret;

#ifdef USE_TASK
   if (ClipMemoryStore->main_func_level == 0)
      Task_INIT();
#endif
   ClipMemoryStore->main_func_level++;
   ret = do_main(ClipMemoryStore, hash, NULL, NULL, argc, argv, envp);
   ClipMemoryStore->main_func_level--;
   if (ClipMemoryStore->main_func_level == 0)
      delete_ClipMachine(ClipMemoryStore);
   return ret;
}

CLIP_DLLEXPORT int
_clip_main_func(ClipMachine * ClipMemoryStore, ClipFunction func, int argc, char **argv, char **envp)
{
   int ret;
   ClipMemoryStore->main_func_level++;
   ret = do_main(ClipMemoryStore, 0, func, NULL, argc, argv, envp);
   ClipMemoryStore->main_func_level--;
   if (ClipMemoryStore->main_func_level == 0)
      delete_ClipMachine(ClipMemoryStore);
   return ret;
}

CLIP_DLLEXPORT int
_clip_main_code(ClipMachine * ClipMemoryStore, ClipBlock * block, int argc, char **argv, char **envp)
{
   int ret;
   ClipMemoryStore->main_func_level++;
   ret = do_main(ClipMemoryStore, 0, NULL, block, argc, argv, envp);
   ClipMemoryStore->main_func_level--;
   if (ClipMemoryStore->main_func_level == 0)
      delete_ClipMachine(ClipMemoryStore);
   return ret;
}

CLIP_DLLEXPORT const char *
_clip_typename(ClipVar * vp)
{
   enum ClipVarType type = _clip_type(vp);

   if (!vp)
      return "NIL";
   switch (type)
      {
      case UNDEF_type_of_ClipVarType:
	 return "UNDEF";
      case NUMERIC_type_of_ClipVarType:
	 return "NUMERIC";
      case CHARACTER_type_of_ClipVarType:
	 return "CHARACTER";
      case LOGICAL_type_of_ClipVarType:
	 return "LOGICAL";
      case DATE_type_of_ClipVarType:
	 return "DATE";
      case OBJECT_type_of_ClipVarType:
	 return "OBJECT";
      case PCODE_type_of_ClipVarType:
	 return "PCODE";
      case CCODE_type_of_ClipVarType:
	 return "CCODE";
      case ARRAY_type_of_ClipVarType:
	 return "ARRAY";
      case MAP_type_of_ClipVarType:
	 return "MAP";
      case MEMO_type_of_ClipVarType:
	 return "MEMO";
      case DATETIME_type_of_ClipVarType:
	 return "DATETIME";
      }
   return "UNKNOWN";
}

/* ] utils */

static int
cmp_hash_bucket(const void *p1, const void *p2)
{
   unsigned long h1 = (unsigned long) GETLONG(p1);
	unsigned long h2 = ((ClipHashBucket *) p2)->hash_of_ClipHashBucket;

   if (h1 < h2)
      return -1;
   else if (h1 > h2)
      return 1;
   else
      return 0;
}

CLIP_DLLEXPORT void
_clip_hash_name(ClipMachine * ClipMemoryStore, long hash, char *buf, int buflen)
{
   ClipFrame *fp;
   char *s = 0;

   if (!buflen)
      return;
   else
      buf[0] = 0;

   switch (hash)
      {
      case HASH_main:
	 s = "MAIN";
	 break;
      case HASH_name:
	 s = "NAME";
	 break;
      case HASH_filename:
	 s = "filename";
	 break;
      case HASH_operation:
	 s = "operation";
	 break;
      case HASH_tries:
	 s = "tries";
	 break;
      case HASH_canSubstitute:
	 s = "canSubstitute";
	 break;
      case HASH_canRetry:
	 s = "canRetry";
	 break;
      case HASH_canDefault:
	 s = "canDefault";
	 break;
      case HASH_osCode:
	 s = "osCode";
	 break;
      case HASH_genCode:
	 s = "genCode";
	 break;
      case HASH_severity:
	 s = "severity";
	 break;
      case HASH_description:
	 s = "description";
	 break;
      case HASH_args:
	 s = "args";
	 break;
      case HASH_cargo:
	 s = "cargo";
	 break;
      case HASH_subCode:
	 s = "subCode";
	 break;
      case HASH_subSystem:
	 s = "subSystem";
	 break;
      case HASH_syserr:
	 s = "syserr";
	 break;
      }

   if (s)
      {
	 strncpy(buf, s, buflen);
	 return;
      }

   s = (char *) HashTable_fetch(ClipMemoryStore->hashnames, hash);
   if (s)
      {
	 strncpy(buf, s, buflen);
	 return;
      }

for (fp = ClipMemoryStore->fp; fp; fp = fp->ClipFrame_up_of_ClipFrame)
      {
			if (fp->names_of_ClipFrame)
	    {
	       ClipHashBucket *bp;

			 bp = (ClipHashBucket *) bsearch(&hash, fp->names_of_ClipFrame->buckets_of_ClipHashNames, fp->names_of_ClipFrame->num_of_ClipHashNames, sizeof(ClipHashBucket), cmp_hash_bucket);
	       if (bp)
		  {
			  strncpy(buf, fp->names_of_ClipFrame->mem_of_ClipHashNames + bp->offs_of_ClipHashBucket, buflen);
		     return;
		  }
	    }
      }

   snprintf(buf, buflen, "0x%08lx", hash);
}

CLIP_DLLEXPORT long
_clip_pardj(ClipMachine * ClipMemoryStore, int num)
{
   if (num > 0 && num <= ClipMemoryStore->argc)
      {
	 ClipVar *vp = (ClipMemoryStore->bp - (ClipMemoryStore->argc - (num - 1)));
	 enum ClipVarType type = _clip_type(vp);

	 if (type == DATE_type_of_ClipVarType)
	    {
	       if (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags)
		  vp = vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
		  return vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar;
	    }
      }
   return 0;
}

CLIP_DLLEXPORT long
_clip_pardtj(ClipMachine * ClipMemoryStore, int num, long *time)
{
   if (num > 0 && num <= ClipMemoryStore->argc)
      {
	 ClipVar *vp = (ClipMemoryStore->bp - (ClipMemoryStore->argc - (num - 1)));
	 enum ClipVarType type = _clip_type(vp);

	 if (type == DATETIME_type_of_ClipVarType)
	    {
	       if (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags)
		  vp = vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
		  *time = vp->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar;
		  return vp->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar;
	    }
      }
   return 0;
}

CLIP_DLLEXPORT long
_clip_pardc(ClipMachine * ClipMemoryStore, int num, int *yy, int *mm, int *dd, int *ww)
{
   long julian = _clip_pardj(ClipMemoryStore, num);

   _clip_cdate(julian, dd, mm, yy, ww);
   return julian;
}

CLIP_DLLEXPORT void
_clip_retdj(ClipMachine * ClipMemoryStore, long julian)
{
   ClipVar *vp = RETPTR(ClipMemoryStore);

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType && vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf != NULL)
      free(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);

   vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATE_type_of_ClipVarType;
   vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   vp->ClipType_t_of_ClipVar.len_of_ClipType = 8;
   vp->ClipType_t_of_ClipVar.dec_of_ClipType = 0;
	vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = julian;
}

CLIP_DLLEXPORT void
_clip_retdtj(ClipMachine * ClipMemoryStore, long julian, long time)
{
   ClipVar *vp = RETPTR(ClipMemoryStore);

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType && vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf != NULL)
      free(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);

   vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATETIME_type_of_ClipVarType;
   vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   vp->ClipType_t_of_ClipVar.len_of_ClipType = 8;
   vp->ClipType_t_of_ClipVar.dec_of_ClipType = 0;
	vp->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar = julian;
	vp->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar = time;
}

CLIP_DLLEXPORT void
_clip_retdc(ClipMachine * ClipMemoryStore, int yy, int mm, int dd)
{
   _clip_retdj(ClipMemoryStore, _clip_jdate(dd, mm, yy));
}

CLIP_DLLEXPORT int
_clip_stordj(ClipMachine * ClipMemoryStore, long julian, int num, int ind)
{
   ClipVar *vp = _clip_par(ClipMemoryStore, num);

   if (!vp)
      return 0;

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
      {
	 if (ind < 0 || ind >= vp->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar)
	    return 0;
	 vp = _clip_vptr(vp->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + ind);
	 goto assign;
      }
   else if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == DATE_type_of_ClipVarType)
      {
       assign:
	 if (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType == F_MSTAT_ClipFlags)
	    return 0;
	 vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = julian;
      }
   else
      {
	 return 0;
      }

   return 1;
}

CLIP_DLLEXPORT int
_clip_stordc(ClipMachine * ClipMemoryStore, int yy, int mm, int dd, int num, int ind)
{
   return _clip_stordj(ClipMemoryStore, _clip_jdate(dd, mm, yy), num, ind);
}

#define DD_BUF_LEN 	7

long
_clip_str_to_date(char *str, char *format, int epoch)
{
   int i, j, len;
   int d = 0, m = 0, y = 0;
   int b[3] = { 0, 0, 0 };
   int mms[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
   char ch = 0;

   if (epoch == 0)
      epoch = CLIP_EPOCH_DEFAULT;
   if (format == NULL)
      format = CLIP_DATEFORMAT_DEFAULT;

   len = strlen(format);
   for (i = 0, j = 0; i < len; i++)
      {
	 if (ch == format[i])
	    continue;
	 switch (format[i])
	    {
	    case 'y':
	    case 'Y':
	       y = j;
	       break;
	    case 'm':
	    case 'M':
	       m = j;
	       break;
	    case 'd':
	    case 'D':
	       d = j;
	       break;
	    case ' ':
	       break;
	    default:
	       j++;
	       break;
	    }
	 if (j > 2)
	    break;
	 ch = format[i];
      }
   if (j < 2)
      return 0;

   len = strlen(str);

   for (i = 0; i < len && !isdigit(str[i]); i++);

   for (j = 0, b[0] = i; i < len && j < 2; i++)
      {
	 ch = str[i];
	 if ((ch < '0' || ch > '9') && ch != ' ')
	    {
	       for (; i < len && !isdigit(str[i]); i++);
	       j++;
	       b[j] = i;
	    }
      }
   if (j < 2)
      return 0;
   m = atoi(str + b[m]);
   d = atoi(str + b[d]);
   y = atoi(str + b[y]);
   if (m == 0 && d == 0 && y == 0)
      return 0;
   if (m > 12 || d > 31)
      return 0;
  /*
     printf("\nctod m=%d,d=%d,mms[m]=%d\n",m,d,mms[m]);
   */
   if (m > 0 && (d > mms[m - 1]))
      return 0;

   i = epoch % 100;
   j = epoch / 100;
   if (y < 100)
      {
	 if (y < i)
	    y += (j + 1) * 100;
	 else
	    y += j * 100;
      }
   return _clip_jdate(d, m, y);
}

CLIP_DLLEXPORT void
_clip_datetostr(long date, char *buf)
{
   int dd, mm, yy, ww;
   char *sss = "0123456789";

   buf[8] = 0;
   if (date == 0)
      {
	 memset(buf, ' ', 8);
	 return;
      }
   _clip_cdate(date, &dd, &mm, &yy, &ww);
   buf[7] = sss[dd % 10];
   buf[6] = sss[dd / 10];
   buf[5] = sss[mm % 10];
   buf[4] = sss[mm / 10];

   buf[3] = sss[yy % 10];
   yy /= 10;
   buf[2] = sss[yy % 10];
   yy /= 10;
   buf[1] = sss[yy % 10];
   yy /= 10;
   buf[0] = sss[yy];
}

CLIP_DLLEXPORT char *
_clip_date_to_str(long date, char *format)
{
   int i, fl;
   int dl = 1, ml = 1, yl = 1;
   int dd, mm, yy, ww;
   char ch, ch2;
   char ds[DD_BUF_LEN + 1], ms[DD_BUF_LEN + 1], ys[DD_BUF_LEN + 1];
   char *ret;

   if (format == NULL)
      format = CLIP_DATEFORMAT_DEFAULT;
   fl = strlen(format);
   ret = malloc(fl + 1);
   memset(ret, 32, fl - 1);
   ret[fl] = 0;
   memset(ds, 32, DD_BUF_LEN);
   ds[DD_BUF_LEN] = 0;
   memset(ms, 32, DD_BUF_LEN);
   ms[DD_BUF_LEN] = 0;
   memset(ys, 32, DD_BUF_LEN);
   ys[DD_BUF_LEN] = 0;

   _clip_cdate(date, &dd, &mm, &yy, &ww);
   if (date != 0)
      {
	 snprintf(ds, DD_BUF_LEN + 1, "%07d", dd);
	 snprintf(ms, DD_BUF_LEN + 1, "%07d", mm);
	 snprintf(ys, DD_BUF_LEN + 1, "%07d", yy);
      }
   for (i = fl; i >= 0; i--)
      {
	 ch = format[i];
	 switch (ch)
	    {
	    case 'd':
	    case 'D':
	       ch2 = ds[DD_BUF_LEN - dl];
	       dl++;
	       break;
	    case 'm':
	    case 'M':
	       ch2 = ms[DD_BUF_LEN - ml];
	       ml++;
	       break;
	    case 'y':
	    case 'Y':
	       ch2 = ys[DD_BUF_LEN - yl];
	       yl++;
	       break;
	    default:
	       ch2 = ch;
	       break;
	    }
	 ret[i] = ch2;
	/*printf("%c%c%d%d%d.",ch,ch2,dl,ml,yl); */
      }
   return ret;
}

/*
   1) returns number of characters matched, -1 if failed.
   (Can return 0 on patterns like "" or "$")
   2) does not require pattern to match to end of string
   3) case-independent
 */

CLIP_DLLEXPORT int
_clip_glob_match(const char *string, const char *pattern, int caseflag)
{
   char c2;
   int match = 0;		/* # of chars matched */

   while (1)
      {
	/* If at end of pattern, success! */
	 if (*pattern == 0)
	    {
#if 0
	       return match;
#else
	      /* uri added: bug with pattern length < string length */
	       if (*string == 0)
		  return match;
	       else
		  return (-1);
#endif
	    }

	/* If last pattern character is '$', verify that entire
	 * string has been matched.
	 */
	 if ((*pattern == '$') && (pattern[1] == 0))
	    {
	       if (*string == 0)
		  return (match);
	       else
		  return (-1);
	    }

	/* Check for a "*" as the next pattern character.  It matches
	 * any substring.  We handle this by calling ourselves
	 * recursively for each postfix of string, until either we
	 * match or we reach the end of the string.
	 */

	 if (*pattern == '*')
	    {

	       int head_len;
	       const char *tail;

	       pattern += 1;
	       if (*pattern == 0)
		  {
		     return (strlen(string) + match);	/* DEL */
		  }

	      /* find longest match - switched to this on 12/31/93 */
	       head_len = strlen(string);	/* length before tail */
	       tail = string + head_len;
	      /*printf("pattern='%s' tail='%s'\n", pattern, tail); */
	       while (head_len >= 0)
		  {
		     int rc;

		     if (-1 != (rc = _clip_glob_match(tail, pattern, caseflag)))
			{
			  /*printf("rc=%d match=%d head_len=%d tail='%s' str='%s' patt='%s'\n", rc, match, head_len, tail,
			     sstr, patt); */
			   return rc + match + head_len;	/* DEL */
			}
		     tail--;
		     head_len--;
		  }

	       return -1;	/* DEL */
	    }

#if 1
	 if (*pattern == '?')
	    {
	       if (!*string)
		  {
		     for (; *pattern; pattern++)
			if (*pattern != '?' && *pattern != '*')
			   return -1;
		     return match;
		  }
	    }
#endif
	/*
	 * after this point, all patterns must match at least one
	 * character, so check this
	 */

	 if (*string == 0)
	    return -1;

#if 1
	/* Check for a "?" as the next pattern character.  It matches
	 * any single character.
	 */

	 if (*pattern == '?')
	    {
	       goto thisCharOK;
	    }
#endif
	/* Check for a "[" as the next pattern character.  It is followed
	 * by a list of characters that are acceptable, or by a range
	 * (two characters separated by "-").
	 */

	 if (*pattern == '[')
	    {
	       pattern += 1;
	       while (1)
		  {
		     if ((*pattern == ']') || (*pattern == 0))
			{
			   return -1;	/* was 0; DEL */
			}
		     if (*pattern == *string)
			{
			   break;
			}
		     if (pattern[1] == '-')
			{
			   c2 = pattern[2];
			   if (c2 == 0)
			      {
				 return -1;	/* DEL */
			      }
			   if ((*pattern <= *string) && (c2 >= *string))
			      {
				 break;
			      }
			   if ((*pattern >= *string) && (c2 <= *string))
			      {
				 break;
			      }
			   pattern += 2;
			}
		     pattern += 1;
		  }

/* OOPS! Found a bug in vanilla Tcl - have sent back to Ousterhout */
/* but he hasn't integrated it yet. - DEL */

#if 0
	       while ((*pattern != ']') && (*pattern != 0))
#else
	       while (*pattern != ']')
		  {
		     if (*pattern == 0)
			{
			   pattern--;
			   break;
			}
#endif
		     pattern += 1;
		  }
	       goto thisCharOK;
	    }

	/* If the next pattern character is backslash, strip it off
	 * so we do exact matching on the character that follows.
	 */

	 if (*pattern == '%')
	    {
	       pattern += 1;
	       if (*pattern == 0)
		  {
		     return -1;
		  }
	       switch (*pattern)
		  {
		  case 'r':
		     if (*string != '\r')
			return -1;
		     break;
		  case 'n':
		     if (*string != '\n')
			return -1;
		     break;
		  case 's':
		     if (*string != ' ')
			return -1;
		     break;
		  case 'e':
		     if (*string != '\033')
			return -1;
		     break;
		  default:
		     if (*string != *pattern)
			return -1;
		     break;
		  }
	    }
	 else if ((caseflag && _clip_toupper(*pattern) != _clip_toupper(*string)) || (!caseflag && *pattern != *string))
	    {
	       return -1;
	    }

       thisCharOK:
	 pattern += 1;
	 string += 1;
	 match++;
      }
}

typedef struct
{
   void *item;
   void (*destroy) (void *);
}
item_bucket;

CLIP_DLLEXPORT void *
_clip_fetch_item(ClipMachine * ClipMemoryStore, long hash)
{
   item_bucket *ip;

   ip = (item_bucket *) HashTable_fetch(ClipMemoryStore->store, hash);
   if (ip)
      return ip->item;
   else
      return 0;
}

CLIP_DLLEXPORT void
_clip_store_item(ClipMachine * ClipMemoryStore, long hash, void *item)
{
   item_bucket *ip;

   _clip_free_item(ClipMemoryStore, hash);
   ip = NEW(item_bucket);
   ip->item = item;
   HashTable_store(ClipMemoryStore->store, ip, hash);
}

CLIP_DLLEXPORT void
_clip_store_item_destroy(ClipMachine * ClipMemoryStore, long hash, void *item, void (*destroy) (void *))
{
   item_bucket *ip;

   _clip_free_item(ClipMemoryStore, hash);
   ip = NEW(item_bucket);
   ip->item = item;
   ip->destroy = destroy;
   HashTable_store(ClipMemoryStore->store, ip, hash);
}

CLIP_DLLEXPORT void
_clip_free_item(ClipMachine * ClipMemoryStore, long hash)
{
   item_bucket *ip;

   ip = (item_bucket *) HashTable_remove(ClipMemoryStore->store, hash);
   if (ip)
      {
	 if (ip->destroy)
	    ip->destroy(ip->item);
	 else
	    free(ip->item);
	 free(ip);
      }
}

CLIP_DLLEXPORT void
_clip_remove_item(ClipMachine * ClipMemoryStore, long hash)
{
   void *rp;

   rp = HashTable_remove(ClipMemoryStore->store, hash);
   free(rp);
}

/*
 * CONTAINER
 */

CLIP_DLLEXPORT int
_clip_compare_c_item(const void *p1, const void *p2)
{
	return ((ContainerItem *) p1)->key_of_ContainerItem - ((ContainerItem *) p2)->key_of_ContainerItem;
}

CLIP_DLLEXPORT int
_clip_compare_c_item_type(const void *p1, const void *p2)
{
	return ((ContainerItem *) p1)->type_of_ContainerItem - ((ContainerItem *) p2)->type_of_ContainerItem;
}

CLIP_DLLEXPORT void *
_clip_fetch_c_item(ClipMachine * ClipMemoryStore, int key, int type)
{
   ContainerItem citem, *ci = NULL;

   if (ClipMemoryStore->container == NULL)
      return NULL;

	citem.key_of_ContainerItem = key;
	ci = bsearch(&citem, ClipMemoryStore->container->ContainerItem_items_of_Container, ClipMemoryStore->container->len_of_Container, sizeof(ContainerItem), _clip_compare_c_item);
	return (ci && ci->type_of_ContainerItem == type ? ci->item_of_ContainerItem : NULL);
}

CLIP_DLLEXPORT void *
_clip_fetch_c_item_type(ClipMachine * ClipMemoryStore, int type, int *key)
{
   ContainerItem citem, *ci = NULL;

   if (ClipMemoryStore->container == NULL)
      return NULL;

   if (key)
      *key = -1;
	citem.type_of_ContainerItem = type;
	ci = bsearch(&citem, ClipMemoryStore->container->ContainerItem_items_of_Container, ClipMemoryStore->container->len_of_Container, sizeof(ContainerItem), _clip_compare_c_item_type);
   if (ci && key)
		*key = ci->key_of_ContainerItem;
	return (ci ? ci->item_of_ContainerItem : NULL);
}

CLIP_DLLEXPORT int
_clip_destroy_c_item(ClipMachine * ClipMemoryStore, int key, int type)
{
   int len;
   Container *cn = ClipMemoryStore->container;
   ContainerItem citem, *ci = NULL;

   if (cn == NULL)
      return 0;
	citem.key_of_ContainerItem = key;
	ci = bsearch(&citem, cn->ContainerItem_items_of_Container, cn->len_of_Container, sizeof(ContainerItem), _clip_compare_c_item);
	if (ci == NULL || ci->type_of_ContainerItem != type)
      return 0;

   if (ci->destroy)
		ci->destroy(ci->item_of_ContainerItem);

	len = cn->len_of_Container * sizeof(ContainerItem) - (ci - cn->ContainerItem_items_of_Container + 1) * sizeof(ContainerItem);

  /*printf( "_destroy_: items=%lx, item=%lx, len=%d\n", cn->items, ci, len ); */

   if (len > 0)
      memmove(ci, ci + 1, len);

	cn->len_of_Container--;

   return 1;
}

CLIP_DLLEXPORT int
_clip_store_c_item(ClipMachine * ClipMemoryStore, void *item, int type, void (*destroy) (void *))
{
   Container *cn = NULL;
   ContainerItem citem;
   int i = 0;

	citem.item_of_ContainerItem = item;
	citem.type_of_ContainerItem = type;
   citem.destroy = destroy;

   cn = ClipMemoryStore->container;

  /* search for first unused key */
  for (i = 0; i < cn->len_of_Container; i++)
      {
			if (i != cn->ContainerItem_items_of_Container[i].key_of_ContainerItem)
	    break;
      }
	   citem.key_of_ContainerItem = i;

  cn->ContainerItem_items_of_Container = (ContainerItem *) realloc(cn->ContainerItem_items_of_Container, (cn->len_of_Container + 1) * sizeof(ContainerItem));

  cn->ContainerItem_items_of_Container[cn->len_of_Container++] = citem;

	qsort(cn->ContainerItem_items_of_Container, cn->len_of_Container, sizeof(ContainerItem), _clip_compare_c_item);

  /*printf( "arr:" );
     for( i=0; i < cn->len; i++ ) {
     printf( "<it=%lx, f=%lx>", &(cn->items)[i], (cn->items)[i].item );
     }
     printf( "\n" ); */

  return citem.key_of_ContainerItem;
}

/*
 * CONTAINER end
 */

static char *
_clip_normalize_path(char *_path)
{
  /* translate "path1/../path2" -> "path2" */
   int pos[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   int pos2[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   int flag[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   int i, beg, end, len, cur = 1, cur2 = 0;
   char *tmp;
   char *path;

   path = strdup(_path);

   i = 0;
   if (path[i] == '/' || path[i] == '\\')
      {
	 i++;
	 pos[0] = 1;
      }
   for (len = strlen(path); i < len; i++)
      {
	 if (path[i] == '.')
	    flag[cur]++;
	 else if (path[i] == '/' || path[i] == '\\')
	    {
	       pos[cur] = i + 1;
	       cur++;
	    }
	 else
	    flag[cur]--;
	 if (cur > 15)
	    break;
      }
   for (cur = 0, cur2 = -1; cur < 16; cur++)
      {
	 if (flag[cur] == 2)	/* subpath ==".." */
	    cur2--;
	 else
	    cur2++;
	 if (cur2 < 0)
	    cur2 = 0;
	//printf("\ncur=%d,flag=%d,cur2=%d",cur,flag[cur],cur2);
	 pos2[cur2] = cur;
      }

   tmp = strdup(path);
  //printf("\npos[0]=%d,pos[1]=%d,pos[2]=%d",pos[0],pos[1],pos[2]);
  //printf("\npo2[0]=%d,po2[1]=%d,po2[2]=%d",pos2[0],pos2[1],pos2[2]);
   memset(path, 0, len);
   for (i = 0, len = 0; i < 16; i++)
      {
	 beg = pos[pos2[i]];
	 end = pos[pos2[i] + 1];
	 if (end == 0)
	    {
	       end = strlen(tmp);
	       i = 200;
	    }
	 strncpy(path + len, tmp + beg, end - beg);
	 len += (end - beg);
	//printf("\ni=%d,pos2=%d,beg=%d,end=%d,tmp=%s,str=%s\n",i,pos2[i],beg,end,tmp,path);
      }
   return path;
}

CLIP_DLLEXPORT int
_clip_translate_path(ClipMachine * ClipMemoryStore, const char *str, char *buf, int buflen)
{
#ifdef _WIN32
   if ((memcmp(str, "\\\\", 2) == 0) || (memcmp(str, "//", 2) == 0))
      {
	/* network resource */
	 strncpy(buf, str, buflen);
	 return 0;
      }

   if (str != NULL && (*str == '/' || *str == '\\') && memcmp(str, "/cygdrive/", 10))
      {
	/* add default drive */
	 char *s = _clip_fetch_item(ClipMemoryStore, CLIP_CUR_DRIVE);
	 if (ClipMemoryStore->rootpath == NULL)
	    snprintf(buf, buflen - 1, "%s%s", s, str);
	 else
	    {
	       char *_str = NULL;
	       _str = _clip_normalize_path((char *) str);
	       snprintf(buf, buflen - 1, "%s%s%s", s, ClipMemoryStore->rootpath, _str);
	       free(_str);
	    }
	 return 0;
      }
   if (str != NULL && strlen(str) >= 2 && str[1] == ':' && str[2] != '/' && str[2] != '\\')
      {
	/* drive with default path */
	 char *def_path;

	 def_path = _clip_fetch_item(ClipMemoryStore, _hash_cur_dir[toupper(*str) - 65]);
	 snprintf(buf, buflen - 1, "%c:%s%s\\%s", *str, ClipMemoryStore->rootpath, def_path, str + 2);
	 return 0;
      }
   if (ClipMemoryStore->rootpath == NULL)
      snprintf(buf, buflen - 1, "%s", str);
   else
      {
	 char *_str = NULL;
	 char *tmp = strdup(str);
	 _str = _clip_normalize_path(tmp);
	 snprintf(buf, buflen - 1, "%s%s", ClipMemoryStore->rootpath, _str);
	 free(_str);
	 free(tmp);
      }
   return 0;
#else
   char *s, *e;
   char *root = 0;
   int bl, ch;

   buf[0] = 0;
   if (strchr(str, '|'))
      {
	 strncpy(buf, str, buflen);
	 buf[buflen - 1] = 0;
	 return 0;
      }

   if ((s = strchr(str, ':')) && s == str + 1)
      {
	 char dn[3];
	 long dh;

	 dn[0] = toupper(*str);
	 dn[1] = ':';
	 dn[2] = 0;
	 dh = _clip_hashstr(dn);
	 if ((root = _clip_fetch_item(ClipMemoryStore, dh)) == NULL)
	    {
	       return -1;
	    }

	 str = s + 1;
	 while ((ch = *str) && (ch == '\\' || ch == '/'))
	    ++str;
      }

   bl = 0;
   if (root)
      {
	 snprintf(buf, buflen - 1, "%s", root);
	 bl = strlen(buf);
	 if (bl > 0 && (buf[bl - 1] == '/' || buf[bl - 1] == '\\'))
	    --bl;
	 buf[bl++] = '/';
      }
  /*
     else if (str[0] == '.' && str[1] == '.')
     {
     strncpy(buf, str, buflen);
     return 1;
     }
   */
   else if (*str != '/' /*&& *str != '\\' */ )
      {
	 if (ClipMemoryStore->rootpath == NULL)
	    {
	       getcwd(buf, buflen);
	       bl = strlen(buf);
	       buf[bl++] = '/';
	    }
      }
   if (ClipMemoryStore->rootpath != NULL)
      {
	 char *_str = NULL;
	 char *tmp = strdup(buf);
	 snprintf(buf, buflen, "%s%s", ClipMemoryStore->rootpath, tmp);
	 free(tmp);
	 bl = strlen(buf);

	 tmp = strdup(str);
	 _str = (char *) _clip_normalize_path(tmp);
	 free(tmp);
	 snprintf(buf + bl, buflen - bl, "%s", _str);
	 free(_str);
      }
   else
      {
	 snprintf(buf + bl, buflen - bl, "%s", str);
      }
   bl = strlen(buf);
   for (e = buf + bl, s = buf; s < e; ++s)
      {
	 if (*s == '\\')
	    *s = '/';
      }
   s = strrchr(buf, '/');
   if (s && s != buf)
      {
	 char ocwd[PATH_MAX];
	 char fname[PATH_MAX];
	 char ch;

	 getcwd(ocwd, sizeof(ocwd));

	 strncpy(fname, s + 1, sizeof(fname));

	 ch = *s;
	 *s = 0;
	 if (chdir(buf) == 0)
	    {
	       getcwd(buf, buflen);
	       strcat(buf, "/");
	       strcat(buf, fname);
	       chdir(ocwd);
	    }
	 else
	    *s = ch;
      }
   bl = strlen(buf);
   if (ClipMemoryStore->flags & TRANSLATE_FLAG)
      {
#if 1
	 int i, j;
	 for (i = 0, j = 0; i < bl; i++)
	    {
	       if (buf[i] == ' ')
		  continue;
	       buf[j] = tolower((unsigned char) (buf[i]));
	       j++;
	    }
	 buf[j] = 0;
#else
	 char *tmp;
	 for (e = buf + bl, s = buf; s < e; ++s)
	    *s = tolower(*(unsigned char *) s);
	 for (tmp = buf + strlen(buf) - 1; *tmp == ' ' && tmp >= buf; tmp--);
	 tmp++;
	 *tmp = 0;
#endif
      }
   return 0;
#endif
}

CLIP_DLLEXPORT void
_clip_mptr(ClipMachine * ClipMemoryStore, ClipVar * refs, int num)
{
   while (num--)
      {
	 ClipVar *vp = NEW(ClipVar);

	 refs->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = UNDEF_type_of_ClipVarType;
	 refs->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MREF_ClipFlags;
	 refs->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = vp;
	 refs->ClipType_t_of_ClipVar.field_of_ClipType = 0;
	 refs->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar = 0;
	 vp->ClipType_t_of_ClipVar.count_of_ClipType = 1;

	 refs++;
      }
}

CLIP_DLLEXPORT ClipVar *
_clip_new_error(ClipMachine * ClipMemoryStore,
		const char *description, int severity, int genCode, int osCode, int canDefault, int canRetry, int canSubstitute, int tries, const char *subSystem, int subCode, const char *operation)
{
   ClipVar *rp = NEW(ClipVar);
   ClipVar var;
   char *s;
   long dim;
   int i;

   _clip_map(ClipMemoryStore, rp);

   memset(&var, 0, sizeof(var));

  /* 'cargo' */

   dim = ClipMemoryStore->argc;
   _clip_array(ClipMemoryStore, &var, 1, &dim);
   for (i = 0; i < ClipMemoryStore->argc; i++)
      {
	 dim = i;
	 _clip_aset(ClipMemoryStore, &var, ARGPTR(ClipMemoryStore, i + 1), 1, &dim);
      }
   _clip_madd(ClipMemoryStore, rp, HASH_args, &var);
   _clip_destroy(ClipMemoryStore, &var);

   _clip_var_str(description, strlen(description), &var);
   _clip_madd(ClipMemoryStore, rp, HASH_description, &var);
   _clip_destroy(ClipMemoryStore, &var);

   _clip_var_num(severity, &var);
   _clip_madd(ClipMemoryStore, rp, HASH_severity, &var);
   _clip_destroy(ClipMemoryStore, &var);

   _clip_var_num(genCode, &var);
   _clip_madd(ClipMemoryStore, rp, HASH_genCode, &var);
   _clip_destroy(ClipMemoryStore, &var);

  /* 'osCode' */
   _clip_var_num(osCode, &var);
   _clip_madd(ClipMemoryStore, rp, HASH_osCode, &var);
   _clip_destroy(ClipMemoryStore, &var);

  /* 'canDefault' */
   _clip_var_log(canDefault, &var);
   _clip_madd(ClipMemoryStore, rp, HASH_canDefault, &var);
   _clip_destroy(ClipMemoryStore, &var);

  /* 'canRetry' */
   _clip_var_log(canRetry, &var);
   _clip_madd(ClipMemoryStore, rp, HASH_canRetry, &var);
   _clip_destroy(ClipMemoryStore, &var);

  /* 'canSubstitute' */
   _clip_var_log(canSubstitute, &var);
   _clip_madd(ClipMemoryStore, rp, HASH_canSubstitute, &var);
   _clip_destroy(ClipMemoryStore, &var);

  /* 'tries' */
   _clip_var_num(tries, &var);
   _clip_madd(ClipMemoryStore, rp, HASH_tries, &var);
   _clip_destroy(ClipMemoryStore, &var);

  /* 'filename' */
   if (ClipMemoryStore->fp)
		s = (char *) ClipMemoryStore->fp->filename_of_ClipFrame;
   else
      s = "";
   _clip_var_str(s, strlen(s), &var);
   _clip_madd(ClipMemoryStore, rp, HASH_filename, &var);
   _clip_destroy(ClipMemoryStore, &var);

  /* 'operation' */
   s = (char *) operation;
   if (!s)
		s = (char *) ClipMemoryStore->fp->procname_of_ClipFrame;
   if (!s)
      s = "";
   _clip_var_str(s, strlen(s), &var);
   _clip_madd(ClipMemoryStore, rp, HASH_operation, &var);
   _clip_destroy(ClipMemoryStore, &var);

  /* 'subSystem' */
   s = (char *) subSystem;
   if (!s)
      s = "";
   _clip_var_str(s, strlen(s), &var);
   _clip_madd(ClipMemoryStore, rp, HASH_subSystem, &var);
   _clip_destroy(ClipMemoryStore, &var);

  /* 'subCode' */
   _clip_var_num(subCode, &var);
   _clip_madd(ClipMemoryStore, rp, HASH_subCode, &var);
   _clip_destroy(ClipMemoryStore, &var);

  /* 'syserr' */
   _clip_var_str(ClipMemoryStore->syserr, strlen(ClipMemoryStore->syserr), &var);
   ClipMemoryStore->syserr[0] = 0;
   _clip_madd(ClipMemoryStore, rp, HASH_syserr, &var);
   _clip_destroy(ClipMemoryStore, &var);

   return rp;
}

CLIP_DLLEXPORT const char *
_clip_errname(int code)
{
   char *msg;

   switch (code)
      {
      default:
	 msg = _clip_gettext("unknown error");
	 break;
      case EG_ARG:
	 msg = _clip_gettext("invalid argument");
	 break;
      case EG_BOUND:
	 msg = _clip_gettext("bound error");
	 break;
      case EG_STROVERFLOW:
	 msg = _clip_gettext("string too long");
	 break;
      case EG_NUMOVERFLOW:
	 msg = _clip_gettext("number too big");
	 break;
      case EG_ZERODIV:
	 msg = _clip_gettext("divide by zero");
	 break;
      case EG_NUMERR:
	 msg = _clip_gettext("number error");
	 break;
      case EG_SYNTAX:
	 msg = _clip_gettext("syntax error");
	 break;
      case EG_COMPLEXITY:
	 msg = _clip_gettext("syntax too complex");
	 break;

      case EG_MEM:
	 msg = _clip_gettext("memory error");
	 break;
      case EG_NOFUNC:
	 msg = _clip_gettext("no function");
	 break;
      case EG_NOMETHOD:
	 msg = _clip_gettext("no method");
	 break;
      case EG_NOVAR:
	 msg = _clip_gettext("no variable");
	 break;
      case EG_NOALIAS:
	 msg = _clip_gettext("no alias");
	 break;
      case EG_NOVARMETHOD:
	 msg = _clip_gettext("no varmethod");
	 break;

      case EG_CREATE:
	 msg = _clip_gettext("create error");
	 break;
      case EG_OPEN:
	 msg = _clip_gettext("open error");
	 break;
      case EG_CLOSE:
	 msg = _clip_gettext("close error");
	 break;
      case EG_READ:
	 msg = _clip_gettext("read error");
	 break;
      case EG_WRITE:
	 msg = _clip_gettext("write error");
	 break;
      case EG_PRINT:
	 msg = _clip_gettext("print error");
	 break;

      case EG_UNSUPPORTED:
	 msg = _clip_gettext("unsupported");
	 break;
      case EG_LIMIT:
	 msg = _clip_gettext("limit exhaust");
	 break;
      case EG_CORRUPTION:
	 msg = _clip_gettext("corruption detected");
	 break;
      case EG_DATATYPE:
	 msg = _clip_gettext("wrong datatype");
	 break;
      case EG_DATAWIDTH:
	 msg = _clip_gettext("wrong datawidth");
	 break;
      case EG_NOTABLE:
	 msg = _clip_gettext("no table");
	 break;
      case EG_NOORDER:
	 msg = _clip_gettext("no order");
	 break;
      case EG_SHARED:
	 msg = _clip_gettext("share violation");
	 break;
      case EG_UNLOCKED:
	 msg = _clip_gettext("unlocked");
	 break;
      case EG_READONLY:
	 msg = _clip_gettext("readonly");
	 break;
      case EG_APPENDLOCK:
	 msg = _clip_gettext("appendlock");
	 break;
      case EG_BADALIAS:
	 msg = _clip_gettext("bad alias");
	 break;
      case EG_LOCK:
	 msg = _clip_gettext("locking error");
	 break;
      case EG_SIGNAL:
	 msg = _clip_gettext("system signal");
	 break;
      }

   return msg;
}

CLIP_DLLEXPORT void
_clip_generr(ClipMachine * ClipMemoryStore, int code)
{
   if (ClipMemoryStore->trapVar)
      return;

   if (code < 0)
      code = -code;

   ClipMemoryStore->trapVar = _clip_new_error(ClipMemoryStore, _clip_errname(code), 2, code, 0, 0, 0, 0, 0, 0, 0, 0);
}

CLIP_DLLEXPORT int
_clip_trap_err(ClipMachine * ClipMemoryStore, int genCode, int canDefault, int canRetry, const char *subSystem, int subCode, const char *operation)
{
   ClipVar *vp;

   vp = _clip_new_error(ClipMemoryStore, _clip_errname(genCode), 2, genCode, 0, canDefault, canRetry, 0, 1, subSystem, subCode, operation);

	_clip_trap_var(ClipMemoryStore, ClipMemoryStore->fp ? ClipMemoryStore->fp->filename_of_ClipFrame : "", ClipMemoryStore->fp ? ClipMemoryStore->fp->line_of_ClipFrame : 0, vp);

   return genCode;
}

CLIP_DLLEXPORT ClipVar *
_clip_mget(ClipMachine * ClipMemoryStore, ClipVar * vp, long no)
{
   int ind;

   if (!_clip_mind(ClipMemoryStore, vp, no, &ind))
      {
	 ClipVar *ep, *ap;

	 ap = _clip_vptr(vp);
	ep = &(ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[ind].ClipVar_var_of_ClipVarEl);
	 return ep;
      }

   return 0;
}

CLIP_DLLEXPORT int
_clip_mtype(ClipMachine * ClipMemoryStore, ClipVar * vp, long no)
{
   int ind;

   if (!_clip_mind(ClipMemoryStore, vp, no, &ind))
      {
	 ClipVar *ap;

	 ap = _clip_vptr(vp);
	return ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[ind].ClipVar_var_of_ClipVarEl.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType;
      }

   return UNDEF_type_of_ClipVarType;
}

CLIP_DLLEXPORT int
_clip_mgetn(ClipMachine * ClipMemoryStore, ClipVar * vp, long no, double *n)
{
   int ind;

   if (!_clip_mind(ClipMemoryStore, vp, no, &ind))
      {
	 ClipVar *ep, *ap;

	 ap = _clip_vptr(vp);

	ep = &(ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[ind].ClipVar_var_of_ClipVarEl);
	 *n = _clip_double(ep);
	 return 0;
      }
   return -1;
}

CLIP_DLLEXPORT int
_clip_mgetl(ClipMachine * ClipMemoryStore, ClipVar * vp, long no, int *n)
{
   int ind;

   if (!_clip_mind(ClipMemoryStore, vp, no, &ind))
      {
	 ClipVar *ep, *ap;

	 ap = _clip_vptr(vp);

	ep = &(ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[ind].ClipVar_var_of_ClipVarEl);
	 *n = _clip_log(ep);
	 return 0;
      }
   return -1;
}

CLIP_DLLEXPORT int
_clip_mgetc(ClipMachine * ClipMemoryStore, ClipVar * vp, long no, char **strp, int *lenp)
{
   int ind;

   if (!_clip_mind(ClipMemoryStore, vp, no, &ind))
      {
	 ClipVar *ep, *ap;
	 int r;

	 ap = _clip_vptr(vp);

	ep = &(ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[ind].ClipVar_var_of_ClipVarEl);
	 r = _clip_strFromVar(ClipMemoryStore, ep, strp, lenp);
	 return r;
      }
   return -1;
}

CLIP_DLLEXPORT void
_clip_upstr(char *s, int len)
{
   unsigned char *us = (unsigned char *) s;

   for (; len; us++, len--)
      {
	 int c = *us;

	 *us = _clip_uptbl[c];
      }
}

CLIP_DLLEXPORT int
_clip_toupper(int c)
{
   return _clip_uptbl[c];
}

CLIP_DLLEXPORT int
_clip_tolower(int c)
{
   return _clip_lowtbl[c];
}

CLIP_DLLEXPORT int
_clip_calc_hash(ClipMachine * ClipMemoryStore)
{
	ClipVar *vp = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - 1, *vvp;
   long hash = -1;

   vvp = _clip_vptr(vp);

   if (vvp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
      {
	 if (vvp->ClipType_t_of_ClipVar.memo_of_ClipType)
	    hash = (long) rational_toDouble(vvp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
	 else
	    hash = (long) vvp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
      }
   else if (vvp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
      hash = _clip_casehashbytes(0, vvp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, vvp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);

   _clip_destroy(ClipMemoryStore, vp);
   vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
   vp->ClipType_t_of_ClipVar.len_of_ClipType = 10;
   vp->ClipType_t_of_ClipVar.dec_of_ClipType = 0;
   vp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
   vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = hash;

   return 0;
}

CLIP_DLLEXPORT int
_clip_calc_hash2(ClipMachine * ClipMemoryStore, int rev)
{
	ClipVar *vp = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - 1, *vvp;
   long hash = -1, hash2 = -1;

   vvp = _clip_vptr(vp);

   if (vvp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
      {
	 if (vvp->ClipType_t_of_ClipVar.memo_of_ClipType)
	    hash = (long) rational_toDouble(vvp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
	 else
	    hash = (long) vvp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
      }
   else if (vvp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
      {
	 char *s = vvp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
	 int l = vvp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
	 char *e;

	 e = strstr(s, "->");
	 if (e)
	    {
	       hash = _clip_casehashword(e + 2, strlen(e + 2));
	       hash2 = _clip_casehashword(s, e - s);
	    }
	 else
	    hash = _clip_casehashword(s, l);
      }

   _clip_destroy(ClipMemoryStore, vp);
   vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
   vp->ClipType_t_of_ClipVar.len_of_ClipType = 10;
   vp->ClipType_t_of_ClipVar.dec_of_ClipType = 0;
   vp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
   vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = (rev ? hash2 : hash);

	vp = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame;
	++(ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame);
   vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
   vp->ClipType_t_of_ClipVar.len_of_ClipType = 10;
   vp->ClipType_t_of_ClipVar.dec_of_ClipType = 0;
   vp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
   vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = (rev ? hash : hash2);

   CLIP_CHECK_STACK;
   return 0;
}

CLIP_DLLEXPORT void
_clip_math_operation(ClipMachine * ClipMemoryStore, int op, ClipVar * Lval, ClipVar * Rval)
{
   rational *tmp1 = NULL, *tmp2 = NULL;
   double d1, d2;
   int ldec = Lval->ClipType_t_of_ClipVar.dec_of_ClipType;
   int rdec = Rval->ClipType_t_of_ClipVar.dec_of_ClipType;
   int llen = Lval->ClipType_t_of_ClipVar.len_of_ClipType;
   int rlen = Rval->ClipType_t_of_ClipVar.len_of_ClipType;

   if (ClipMemoryStore->flags & RATIONAL_FLAG)
      {
	 if (!Lval->ClipType_t_of_ClipVar.memo_of_ClipType)
	    tmp1 = rational_double_init(Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
	 else
	    tmp1 = Lval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar;
	 if (!Rval->ClipType_t_of_ClipVar.memo_of_ClipType)
	    tmp2 = rational_double_init(Rval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
	 else
	    tmp2 = Rval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar;
	 switch (op)
	    {
	    case '-':
	       rational_suba(tmp1, tmp2);
	       break;
	    case '+':
	       rational_adda(tmp1, tmp2);
	       break;
	    case '/':
	       rational_diva(tmp1, tmp2);
	       break;
	    case '*':
	       rational_mula(tmp1, tmp2);
	       break;

	    }
	 if (!Lval->ClipType_t_of_ClipVar.memo_of_ClipType)
	    {
	       Lval->ClipType_t_of_ClipVar.memo_of_ClipType = 1;
	       Lval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar = tmp1;
	    }
	 if (!Rval->ClipType_t_of_ClipVar.memo_of_ClipType)
	    rational_destroy(tmp2);
      }
   else
      {
	 if (Lval->ClipType_t_of_ClipVar.memo_of_ClipType)
	    d1 = rational_toDouble(Lval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
	 else
	    d1 = Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
	 if (Rval->ClipType_t_of_ClipVar.memo_of_ClipType)
	    d2 = rational_toDouble(Rval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
	 else
	    d2 = Rval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
	 switch (op)
	    {
	    case '-':
	       d1 -= d2;
	       break;
	    case '+':
	       d1 += d2;
	       break;
	    case '/':
	       if (d2 == 0.0)
		  d1 = 0.0;
	       else
		  d1 /= d2;
	       break;
	    case '*':
	       d1 *= d2;
	       break;
	    }
	 if (Lval->ClipType_t_of_ClipVar.memo_of_ClipType)
	    rational_destroy(Lval->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
	 Lval->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	 Lval->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = d1;
      }
   switch (op)
      {
      case '-':
	 llen = (llen < rlen ? rlen : llen);
	 ldec = (ldec < rdec ? rdec : ldec);
	 break;
      case '+':
	 llen = (llen < rlen ? rlen : llen);
	 ldec = (ldec < rdec ? rdec : ldec);
	 break;
      case '/':
	 if (ldec == 0)
	    {
	       ldec += ClipMemoryStore->decimals;
	       llen += ClipMemoryStore->decimals + 1;
	    }
	 break;
      case '*':
	 llen += Rval->ClipType_t_of_ClipVar.len_of_ClipType;
	 ldec += Rval->ClipType_t_of_ClipVar.dec_of_ClipType;
	 break;
      }
   if (ldec > 5)
      ldec = 5;
   if (ClipMemoryStore->flags & FIXED_FLAG)
      ldec = ClipMemoryStore->decimals;
   if (ldec > 0 && llen <= 10 + ldec)
      llen = 11 + ldec;
   if (llen > 20)
      llen = 20;
   Lval->ClipType_t_of_ClipVar.len_of_ClipType = llen;
   Lval->ClipType_t_of_ClipVar.dec_of_ClipType = ldec;
   return;
}

CLIP_DLLEXPORT int
_clip_forstep(ClipMachine * ClipMemoryStore, int *_if)
{
   double d_step, d_to, d_val;
	ClipVar *step = _clip_vptr(ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - 1);
	ClipVar *to = _clip_vptr(ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - 2);
	ClipVar *val = _clip_vptr(ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - 3);

   if (step->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != NUMERIC_type_of_ClipVarType
       || (to->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != NUMERIC_type_of_ClipVarType
	   && to->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != DATE_type_of_ClipVarType)
       || (val->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != NUMERIC_type_of_ClipVarType && val->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != DATE_type_of_ClipVarType))
      return EG_ARG;

   d_step = _clip_double(step);
   d_to = _clip_double(to);
   d_val = _clip_double(val);

   *_if = 1;

   if (d_step > 0)
      {
	 if (d_val > d_to)
	    *_if = 0;
      }
   else if (d_step < 0)
      {
	 if (d_val < d_to)
	    *_if = 0;
      }

   _clip_pop(ClipMemoryStore);
   _clip_pop(ClipMemoryStore);
   _clip_pop(ClipMemoryStore);

   return 0;
}

CLIP_DLLEXPORT int
_clip_map_first(ClipMachine * ClipMemoryStore, int keys, int *_if)
{
	ClipVar *rp = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - 1;
   ClipVar *arr = _clip_vptr(rp);
   ClipVar *vp = 0;
   long no = 0;

   *_if = 0;

   if (arr->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
      {
			arr->ClipMapVar_m_of_ClipVar.current_of_ClipMapVar = 0;
			if (arr->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar > 0)
	    {
			 no = arr->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[0].no_of_ClipVarEl;
			 vp = &(arr->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[0].ClipVar_var_of_ClipVarEl);
	       *_if = 1;
	    }
      }
   else if (arr->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
      {
	 arr->ClipArrVar_a_of_ClipVar.current_of_ClipArrVar = 0;
	 if (arr->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar > 0)
	    {
	       no = 1;
	       vp = arr->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar;
	       *_if = 1;
	    }
      }
   else
      return EG_ARG;

   if (*_if)
      {
	 _clip_destroy(ClipMemoryStore, rp);
	 if (keys)
	    _clip_var_num(no, rp);
	 else
	    _clip_clone(ClipMemoryStore, rp, vp);
      }
   else
      _clip_pop(ClipMemoryStore);

   return 0;
}

CLIP_DLLEXPORT int
_clip_map_next(ClipMachine * ClipMemoryStore, int keys, int *_if)
{
	ClipVar *rp = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - 1;
   ClipVar *arr = _clip_vptr(rp);
   ClipVar *vp = 0;
   long no = 0;

   *_if = 0;

   if (arr->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
      {
			arr->ClipMapVar_m_of_ClipVar.current_of_ClipMapVar += 1;
			if (arr->ClipMapVar_m_of_ClipVar.current_of_ClipMapVar < arr->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar)
	    {
			 no = arr->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[arr->ClipMapVar_m_of_ClipVar.current_of_ClipMapVar].no_of_ClipVarEl;
			 vp = &(arr->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[arr->ClipMapVar_m_of_ClipVar.current_of_ClipMapVar].ClipVar_var_of_ClipVarEl);
	       *_if = 1;
	    }
      }
   else if (arr->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
      {
	 arr->ClipArrVar_a_of_ClipVar.current_of_ClipArrVar += 1;
	 if (arr->ClipArrVar_a_of_ClipVar.current_of_ClipArrVar < arr->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar)
	    {
	       no = arr->ClipArrVar_a_of_ClipVar.current_of_ClipArrVar + 1;
	       vp = arr->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + arr->ClipArrVar_a_of_ClipVar.current_of_ClipArrVar;
	       *_if = 1;
	    }
      }
   else
      return EG_ARG;

   if (*_if)
      {
	 _clip_destroy(ClipMemoryStore, rp);
	 if (keys)
	    _clip_var_num(no, rp);
	 else
	    _clip_clone(ClipMemoryStore, rp, vp);
      }
   else
      _clip_pop(ClipMemoryStore);

   return 0;
}

CLIP_DLLEXPORT void
_clip_swap(ClipMachine * ClipMemoryStore, int offset)
{
   ClipVar v;

	v = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame[-1];
	memmove(ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - offset, ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - offset - 1, offset * sizeof(ClipVar));
	ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame[-offset - 1] = v;
}

CLIP_DLLEXPORT void
_clip_push_locale(ClipMachine * ClipMemoryStore)
{
	ClipVar *vp = _clip_vptr(ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - 1);
   char *s = 0;

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
      {
	 char *e, *p, *m;
	 int i;

	 p = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
	 for (i = 0, e = p; *e && *e != 1; e++, i++)
	    ;
	 e++;
	 m = (char *) malloc(i + 1);
	 memcpy(m, p, i);
	 m[i] = 0;

	 _clip_locale_msg(m, e, &s);
#if 0
	 for (e = s; *e; e++)
	    *e = _clip_intbl[*(unsigned char *) e];
#endif
	 free(m);
      }
   else
      s = strdup("");

	vp = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - 1;
   _clip_destroy(ClipMemoryStore, vp);
   _clip_var_str(s, strlen(s), vp);
   free(s);
}

static ClipVar *
local_ref(ClipMachine * ClipMemoryStore, int no)
{
   ClipVar *vp;
	ClipVarFrame *lp = ClipMemoryStore->fp->ClipVarFrame_localvars_of_ClipFrame;

	if (lp && no < lp->size_of_ClipVarFrame)
		vp = lp->ClipVar_vars_of_ClipVarFrame + no;
   else
      {
	 int nlocals = no + 1;
	 ClipVarFrame *localvars = (ClipVarFrame *) realloc(lp,
							    sizeof(ClipVarFrame) + nlocals * sizeof(ClipVar));

								 localvars->ClipVar_vars_of_ClipVarFrame = (ClipVar *) (localvars + 1);
	 if (lp)
	    {
			 memset(localvars->ClipVar_vars_of_ClipVarFrame + no, 0, (nlocals - localvars->size_of_ClipVarFrame) * sizeof(ClipVar));
	    }
	 else
	    {
			 memset(localvars->ClipVar_vars_of_ClipVarFrame, 0, nlocals * sizeof(ClipVar));
			 localvars->refcount_of_ClipVarFrame = 1;
	    }
		 localvars->size_of_ClipVarFrame = nlocals;
	 ClipMemoryStore->fp->ClipVarFrame_localvars_of_ClipFrame = localvars;

	 vp = localvars->ClipVar_vars_of_ClipVarFrame + no;
      }

   if (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags && vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType &
F_MPTR_ClipFlags)
      {
#if 1
	 if (vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType == F_MPTR_ClipFlags &&
	     (vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType ||
vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType))
	    {
	    }
	 else
#endif
	    vp = vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
      }

   return vp;
}

CLIP_DLLEXPORT int
_clip_push_local(ClipMachine * ClipMemoryStore, int no)
{
   return _clip_push(ClipMemoryStore, local_ref(ClipMemoryStore, no));
}

CLIP_DLLEXPORT ClipVar *
_clip_ref_local(ClipMachine * ClipMemoryStore, int no)
{
   return local_ref(ClipMemoryStore, no);
}

CLIP_DLLEXPORT ClipVar *
_clip_pop_ref(ClipMachine * ClipMemoryStore)
{
	return ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
}

CLIP_DLLEXPORT int
_clip_unref(ClipMachine * ClipMemoryStore, ClipVar * lval)
{
   _clip_destroy(ClipMemoryStore, lval);
   return 0;
}

CLIP_DLLEXPORT int
_clip_refmacro(ClipMachine * ClipMemoryStore)
{
   VarEntry *vep;
   long fhash = 0, ahash;
   char *fname = 0, *aname = 0;
   int fl = 0, al = 0, r;
   ClipVar *lval = 0;
	ClipVar *sp = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - 1;
   ClipVar *vp = _clip_vptr(sp);
   long *dim = 0;
   int ndim = 0;

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType)
      {
	 _clip_trap_printf(ClipMemoryStore, __file__, __LINE__, "macro reference with non-character argument (%s)", _clip_typename(vp));
	 return 0;
      }

   r = _clip_parse_name(ClipMemoryStore, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, &aname, &al,
&fname, &fl, &dim, &ndim);

   if (aname)
      ahash = _clip_casehashbytes(0, aname, al);
   else
      ahash = -1;

   if (fname)
      fhash = _clip_casehashbytes(0, fname, fl);

   _clip_destroy(ClipMemoryStore, sp);
	ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame--;

   if (r == 2)			/* field */
      {
	 return _clip_field(ClipMemoryStore, fhash, ahash);
      }
   else if (r == 1)		/* memvar */
      {
      }
   else
     /* field/memvar */
      {
	 if (!_clip_try_field(ClipMemoryStore, fhash))
	    return 0;
      }

   vep = fetch_var(ClipMemoryStore, fhash);

   if (vep)
		lval = &vep->ClipVar_var_of_VarEntry;

   if (!lval)
      {
	 _clip_trap_printf(ClipMemoryStore, __file__, __LINE__, "no variable name: '%.*s'", vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf,
vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
	 return _clip_call_errblock(ClipMemoryStore, 1);
      }
   else if (r == 3)
      {
	 lval = _clip_aref(ClipMemoryStore, lval, ndim, dim);
	 free(dim);
      }

   _clip_ref(ClipMemoryStore, lval, 0);

   return 0;
}

static void
dup_ref(ClipVar * dest, ClipVar * src)
{
   if (dest == src || src->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar == dest)
      return;
   CLEAR_CLIPVAR(dest);
   dest->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = src->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType;
   dest->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = src->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType;
   dest->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = src->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
   dest->ClipType_t_of_ClipVar.field_of_ClipType = src->ClipType_t_of_ClipVar.field_of_ClipType;
   dest->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar = src->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar;
   ++(src->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar->ClipType_t_of_ClipVar.count_of_ClipType);
}

CLIP_DLLEXPORT ClipFieldDef *
_clip_get_fielddef(ClipMachine * ClipMemoryStore, long areahash, long namehash)
{
   long hash;
   ClipFieldDef *fp;

   hash = _clip_hashbytes(0, (char *) &areahash, sizeof(areahash));
   hash = _clip_hashbytes(hash, (char *) &namehash, sizeof(namehash));

   fp = (ClipFieldDef *) HashTable_fetch(ClipMemoryStore->fields, hash);
   if (!fp)
      {
	 fp = (ClipFieldDef *) calloc(1, sizeof(ClipFieldDef));
	 fp->areahash_of_ClipFieldDef = areahash;
	 fp->fieldhash_of_ClipFieldDef = namehash;
	 HashTable_insert(ClipMemoryStore->fields, fp, hash);
      }

   return fp;
}

static void
choose_quot(char *str, int l, char *lquot, char *rquot)
{
   char *s;
   int ch, n = 0;

   do
      {
	 int lq = *lquot;
	 int rq = *rquot;

	 for (s = str; (ch = *s); ++s)
	    if (ch == lq || ch == rq)
	       break;
	 if (!ch)
	    return;
	 switch (*lquot)
	    {
	    case '"':
	       *lquot = '\'';
	       *rquot = '\'';
	       break;
	    case '\'':
	    case '[':
	       *lquot = '"';
	       *rquot = '"';
	       break;
	    }
      }
   while (n++ < 3);
}

CLIP_DLLEXPORT int
_clip_catstr(ClipMachine * ClipMemoryStore, int num)
{
   int i;
   char *s = 0, buf[128], *s1;
   int l = 0, l1, len, dec;
   ClipVar *rp;

   for (i = 0; i < num; i++)
      {
			ClipVar *vp = _clip_vptr(ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - num + i);

	 switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
	    {
	    default:
	       snprintf(buf, sizeof(buf), "NIL");
	       s1 = buf;
	       l1 = 3;
	       break;
	    case CHARACTER_type_of_ClipVarType:
	       l1 = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
	       s1 = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
	       break;
	    case NUMERIC_type_of_ClipVarType:
	       len = vp->ClipType_t_of_ClipVar.len_of_ClipType;
	       dec = vp->ClipType_t_of_ClipVar.dec_of_ClipType;
	       if (!len)
		  {
		     len = 10;
		     dec = 2;
		  }
	       if (!vp->ClipType_t_of_ClipVar.memo_of_ClipType)
		  {
		     int c;

		     snprintf(buf, sizeof(buf), "%*.*f", len, dec, vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
		     for (s1 = buf + strlen(buf); s1 > buf; --s1)
			if ((*s1) == ',')
			   (*s1) = '.';
		     for (s1 = buf; (c = *s1) && c == ' '; s1++)
			;
		     l1 = strlen(s1);
		  }
	       else
		  {
		     if (len)
			s1 = rational_toString(vp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, 10, dec, 0);
		     else
			{
			   s1 = rational_toString(vp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, len, dec, 0);
			   snprintf(buf, sizeof(buf), "%s", s1);
			   free(s1);
			}
		     l1 = strlen(buf);
		  }
	       break;
	    }

	 {
	    s = (char *) realloc(s, l + l1 + 1);
	    memcpy(s + l, s1, l1);
	    s[l + l1] = 0;
	    l += l1;
	 }

_clip_destroy(ClipMemoryStore, ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - num + i);
      }

ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame -= num;
rp = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame;

   rp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = s;
   rp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = l;
   rp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   rp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   rp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;

	ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame++;
   CLIP_CHECK_STACK;

   return 0;
}

CLIP_DLLEXPORT int
_clip_quot(ClipMachine * ClipMemoryStore)
{
	ClipVar *vp = ClipMemoryStore->fp->ClipVar_sp_of_ClipFrame - 1;
   ClipVar *rp = _clip_vptr(vp);
   char *s;
   int l;

   if (rp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
      {
	 l = rp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
	 s = (char *) calloc(1, l + 3);
	 memcpy(s + 1, rp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, l);
	 s[0] = '"';
	 s[l + 1] = '"';
	 choose_quot(s, l + 2, s, s + l + 1);
	 s[l + 2] = 0;
	 l += 2;
      }
   else if (rp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == DATE_type_of_ClipVarType)
      {
	 int dd, mm, yy, ww;

	_clip_cdate(rp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar, &dd, &mm, &yy, &ww);
	 s = malloc(17);
	 snprintf(s, 17, "STOD(\"%04d%02d%02d\")", yy, mm, dd);
	 l = 16;
      }
   else if (rp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == DATETIME_type_of_ClipVarType)
      {
			s = _clip_ttoc(ClipMemoryStore, rp->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar, rp->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar, &l, ClipMemoryStore->date_format,
ClipMemoryStore->hours, ClipMemoryStore->seconds);
      }
   else if (rp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == LOGICAL_type_of_ClipVarType)
      {
	 s = malloc(4);
	snprintf(s, 4, "%s", rp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar ? ".T." : ".F.");
	 l = 3;
      }
   else
      return 0;

   _clip_destroy(ClipMemoryStore, vp);

   vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = s;
   vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = l;
   vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   vp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;

   return 0;
}

CLIP_DLLEXPORT int
_clip_strcmp(const char *str1, const char *str2)
{
   int l1 = strlen(str1);
   int l2 = strlen(str2);

   return _clip_strnncmp(str1, str2, l1, l2);
}

CLIP_DLLEXPORT int
_clip_strncmp(const char *str1, const char *str2, int len)
{
   int l1 = strlen(str1);
   int l2 = strlen(str2);

   if (l1 > len)
      l1 = len;
   if (l2 > len)
      l2 = len;
   return _clip_strnncmp(str1, str2, l1, l2);
}

CLIP_DLLEXPORT int
_clip_strnncmp(const char *str1, const char *str2, int len1, int len2)
{
   unsigned char ch1 = 0, ch2 = 0;
   const unsigned char *us1, *us2;

   for (us1 = (const unsigned char *) str1, us2 = (const unsigned char *) str2; len1 && len2; us1++, us2++, len1--, len2--)
      {
	 ch1 = *(unsigned char *) us1;
	 ch2 = *(unsigned char *) us2;
	 if (ch1 < 32 && ch2 > 31)
	    ch1 = 0;

	 if (ch1 > 31 && ch2 < 32)
	    ch2 = 0;

	 ch1 = _clip_cmptbl[ch1];
	 ch2 = _clip_cmptbl[ch2];

	 if (ch1 < ch2)
	    return -(ch2 - ch1);
	 else if (ch1 > ch2)
	    return ch1 - ch2;
      }

   if (len1)
      return ch1;
   else if (len2)
      return -ch2;
   return 0;
}

CLIP_DLLEXPORT int
_clip_strcasecmp(const char *str1, const char *str2)
{
   int l1 = strlen(str1);
   int l2 = strlen(str2);

   return _clip_strnncasecmp(str1, str2, l1, l2);
}

CLIP_DLLEXPORT int
_clip_strncasecmp(const char *str1, const char *str2, int len)
{
   int l1 = strlen(str1);
   int l2 = strlen(str2);

   if (l1 > len)
      l1 = len;
   if (l2 > len)
      l2 = len;
   return _clip_strnncasecmp(str1, str2, l1, l2);
}

CLIP_DLLEXPORT int
_clip_strnncasecmp(const char *str1, const char *str2, int len1, int len2)
{
   unsigned char ch1 = 0, ch2 = 0;
   const unsigned char *us1, *us2;

   for (us1 = (const unsigned char *) str1, us2 = (const unsigned char *) str2; len1 && len2; us1++, us2++, len1--, len2--)
      {
	 ch1 = _clip_uptbl[*us1];
	 ch2 = _clip_uptbl[*us2];
	 if (ch1 < 32 && ch2 > 31)
	    ch1 = 0;

	 if (ch1 > 31 && ch2 < 32)
	    ch2 = 0;

	 ch1 = _clip_cmptbl[ch1];
	 ch2 = _clip_cmptbl[ch2];

	 if (ch1 < ch2)
	    return -(ch2 - ch1);
	 else if (ch1 > ch2)
	    return ch1 - ch2;
      }

   if (len1)
      return ch1;
   else if (len2)
      return -ch2;
   return 0;
}

void
_clip_unix_path(char *p, int doslike)
{
#if 1
   int i, j, len = strlen(p);
   char *buf = p;
   for (i = 0, j = 0; i < len; i++)
      {
	 if (doslike && buf[i] == ' ')
	    continue;
	 if (buf[i] == '\\')
	    buf[j] = '/';
	 if (doslike)
	    buf[j] = tolower((unsigned char) (buf[i]));
	 j++;
      }
   buf[j] = 0;
#else
   while (*p)
      {
	 if (*p == '\\')
	    *p = '/';
	 else if (tolow)
	    *p = tolower(*p);
	 p++;
      }
#endif
}

int
_clip_absolute_path(ClipMachine * ClipMemoryStore, const char *path, char *p, int len)
{
   char op[PATH_MAX];
   char *e = p;

#ifndef _WIN32
   const char *s = path;

   p[0] = 0;
   if (path[1] == ':')
      {
	 char dn[3];
	 char *root;

	 dn[0] = toupper(*s);
	 dn[1] = ':';
	 dn[2] = 0;
	 root = _clip_fetch_item(ClipMemoryStore, _clip_hashstr(dn));
	 if (!root)
	    return 0;
	 strncpy(p, root, len - 1);
	 e = p + strlen(p);
	 if (*(e - 1) != '/')
	    {
	       *e = '/';
	       e++;
	       *e = 0;
	    }
	 s += 2;
	 if (path[2] == '\\' || path[2] == '/')
	    s++;
      }
   strncpy(e, s, p + len - e - 2);
#else
   if ((memcmp(path, "\\\\", 2) == 0) || (memcmp(path, "//", 2) == 0))
      {
	/* network resource */
	 strncpy(p, path, len);
      }
   else
      {
	 if (path != NULL && (*path == '/' || *path == '\\') && memcmp(path, "/cygdrive/", 10))
	    {

	      /* add default drive */
	       char *s = _clip_fetch_item(ClipMemoryStore, CLIP_CUR_DRIVE);

	       snprintf(p, len - 1, "%s%s", s, path);
	    }
	 else
	    strncpy(p, path, len);
      }
#endif
   _clip_unix_path(p, ClipMemoryStore->flags & TRANSLATE_FLAG);

   errno = 0;
   getcwd(op, sizeof(op));
   if (errno)
      return 1;
   chdir(p);
/*	if (errno)
		return 1;*/
   errno = 0;
   getcwd(p, len);
   if (errno)
      return 1;
   chdir(op);
   if (errno)
      return 1;

   e = p + strlen(p) - 1;
   if (*e != '/' && e - p < len - 2)
      {
	 *(e + 1) = '/';
	 *(e + 2) = 0;
      }

   return 0;
}

CLIP_DLLEXPORT int
_clip_path(ClipMachine * ClipMemoryStore, const char *fn, char *p, int len, int create)
{
   char path[PATH_MAX];
   const char *s, *s1 = NULL, *s2 = NULL;

   if (strchr(fn, '|'))
      {
	 strncpy(p, fn, len);
	 p[len - 1] = 0;
	 return 0;
      }

   if ((memcmp(fn, "\\\\", 2) == 0) || (memcmp(fn, "//", 2) == 0))
      {
	 strncpy(p, fn, len);
      }
   else if ((s = strrchr(fn, '/')) || (s1 = strrchr(fn, '\\')) || (s2 = strrchr(fn, ':')))
      {
	 char path[PATH_MAX];
	 int f = 0;

	 if (!s1)
	    s1 = strrchr(fn, '\\');
	 if (!s2)
	    s2 = strrchr(fn, ':');
	 s = (s > s1) ? s : s1;
	 if (s2 > s)
	    f = 1;
	 s = (s > s2) ? s : s2;
	 memcpy(path, fn, s - fn + f);
	 path[s - fn + f] = 0;
	 if (_clip_absolute_path(ClipMemoryStore, path, p, len))
	    return 1;
	 strncat(p, s + 1, len - strlen(p) - 1);
	 _clip_unix_path(p, ClipMemoryStore->flags & TRANSLATE_FLAG);
      }
   else
      {
	 if (_clip_absolute_path(ClipMemoryStore, ClipMemoryStore->defaul, p, len))
	    return 1;
	 strncat(p, fn, len - strlen(p) - 1);
	 _clip_unix_path(p, ClipMemoryStore->flags & TRANSLATE_FLAG);
	 if (!create && access(p, F_OK))
	    {
	       char *c, *c1 = path;

	       strncpy(path, ClipMemoryStore->path, sizeof(path) - 1);
	       while (c1 != (char *) 1 && access(p, F_OK))
		  {
		     c = strchr(c1, ';');
		     if (c)
			{
			   *c = 0;
			}
		     else
			{
			   c = strchr(c1, ',');
			   if (c)
			      *c = 0;
			}
		     if (_clip_absolute_path(ClipMemoryStore, c1, p, len))
			return 1;
		     strncat(p, fn, len - strlen(p) - 1);
		     _clip_unix_path(p, ClipMemoryStore->flags & TRANSLATE_FLAG);
		     c1 = c + 1;
		  }
	    }
      }
   return 0;
}

typedef struct
{
   char *src;
   char *dst;
   unsigned char tbl[256];
   unsigned char *pg;		/*[128] */
}
TranslateTable;

static int
cmp_TranslateTable(void *p1, void *p2)
{
   TranslateTable *tp1 = (TranslateTable *) p1;
   TranslateTable *tp2 = (TranslateTable *) p2;
   int r;

   r = strcasecmp(tp1->src, tp2->src);
   if (r)
      return r;

   return strcasecmp(tp1->dst, tp2->dst);
}

static void
delete_TranslateTable(void *p)
{
   TranslateTable *tp = (TranslateTable *) p;

   if (!tp)
      return;

   free(tp->src);
   free(tp->dst);
   if (tp->pg)
      free(tp->pg);

   free(tp);
}

static Coll *tr_charsets = 0;

static unsigned char
pg2pgch(unsigned char ch)
{
   switch (ch)
      {
      case PG_HLINE:
	 return PGCH_HLINE;
      case PG_LLCORNER:
	 return PGCH_LLCORNER;
      case PG_LRCORNER:
	 return PGCH_LRCORNER;
      case PG_TTEE:
	 return PGCH_TTEE;
      case PG_RTEE:
	 return PGCH_RTEE;
      case PG_LTEE:
	 return PGCH_LTEE;
      case PG_BTEE:
	 return PGCH_BTEE;
      case PG_ULCORNER:
	 return PGCH_ULCORNER;
      case PG_URCORNER:
	 return PGCH_URCORNER;
      case PG_VLINE:
	 return PGCH_VLINE;

      case PG_HLINE2:
	 return PGCH_HLINE2;
      case PG_LLCORNER2:
	 return PGCH_LLCORNER2;
      case PG_LRCORNER2:
	 return PGCH_LRCORNER2;
      case PG_TTEE2:
	 return PGCH_TTEE2;
      case PG_RTEE2:
	 return PGCH_RTEE2;
      case PG_LTEE2:
	 return PGCH_LTEE2;
      case PG_BTEE2:
	 return PGCH_BTEE2;
      case PG_ULCORNER2:
	 return PGCH_ULCORNER2;
      case PG_URCORNER2:
	 return PGCH_URCORNER2;
      case PG_VLINE2:
	 return PGCH_VLINE2;

      case PG_PLUS:
	 return PGCH_PLUS;
      case PG_PLUS2:
	 return PGCH_PLUS2;
      case PG_BLOCK:
	 return PGCH_BLOCK;

      case PG_LARROW:
	 return PGCH_LARROW;
      case PG_RARROW:
	 return PGCH_RARROW;

      }
   return 0;
}

CLIP_DLLEXPORT int
_clip_translate_charset(char *p1, char *p2, unsigned char *str, unsigned char *ostr, int len)
{
   cons_CharsetEntry *cs1 = 0, *cs2 = 0;
   int len1 = 0, len2 = 0;
   int ind;
   TranslateTable stbl, *tp = 0;

   if (!p1 || !p2 || !str || !ostr)
      return EG_ARG;

   if (!strcasecmp(p1, p2))
      {
	 memcpy(ostr, str, len);
	 return 0;
      }

   stbl.src = p1;
   stbl.dst = p2;

   if (tr_charsets && search_Coll(tr_charsets, &stbl, &ind))
      {
	 tp = (TranslateTable *) tr_charsets->items[ind];
	 goto translate;
      }

   if (load_charset_name(p1, &cs1, &len1))
      {
	 _clip_logg(2, "translate_charset: cannot load charset file '%s': %s", p1, strerror(errno));
      }

   if (load_charset_name(p2, &cs2, &len2))
      {
	 _clip_logg(2, "translate_charset: cannot load charset file '%s': %s", p2, strerror(errno));
      }

   if (cs1 && cs2)
      {
	 int src_pg, dst_pg;

	 tp = (TranslateTable *) calloc(1, sizeof(TranslateTable));
	 tp->src = strdup(p1);
	 tp->dst = strdup(p2);
	 make_translation(cs1, len1, cs2, len2, tp->tbl);

	 src_pg = !charset_have_pg(cs1, len1);
	 dst_pg = !charset_have_pg(cs2, len2);
	 if (src_pg && !dst_pg)
	    {
	       unsigned char *pg;

	       pg = (unsigned char *) malloc(256);
	       load_charset_tables(p1, 0, 0, 0, 0, pg);

	       memmove(pg, pg + 128, 128);
	       tp->pg = (unsigned char *) realloc(pg, 128);
	    }

	 if (!tr_charsets)
	    tr_charsets = new_Coll(delete_TranslateTable, cmp_TranslateTable);
	 insert_Coll(tr_charsets, tp);
      }

   free(cs1);
   free(cs2);

 translate:
   if (tp)
      {
	 int i;
	 int s, s1;

	 for (i = 0; i < len; i++)
	    {
	       s = str[i];
	       if ((s & 0x80) && tp->pg && (s1 = tp->pg[s & 0x7f]))
		  ostr[i] = pg2pgch(s1);
	       else
		  ostr[i] = tp->tbl[s];
	    }
      }
   else
      {
	 memcpy(ostr, str, len);
      }

   return 0;
}

/*
*	ALTD(0) - disable debugging
*	ALTD(1) - enable debugging
*	ALTD(2) - enable debugging and wait connection
*/

int
clip_ALTD(ClipMachine * ClipMemoryStore)
{
   if (!_clip_debuglevel)
      {
	 _clip_debuglevel = 1;
#ifndef OS_MINGW
	 signal(SIGUSR1, _clip_sigdebug);
#endif
      }

   _clip_in_breakpoint();
   return 0;
}

void
_clip_check_stack(ClipMachine * ClipMemoryStore)
{
   ClipFrame *fp = ClipMemoryStore->fp;

	if (fp && fp->ClipVar_sp_of_ClipFrame > (fp->ClipVar_stack_of_ClipFrame + fp->stklen_of_ClipFrame))
      {
	 _clip_call_errblock(ClipMemoryStore, EG_MEM);
	_clip_logg(0, "stack check failed at line %d file %d", fp->line_of_ClipFrame, fp->filename_of_ClipFrame ? fp->filename_of_ClipFrame : "unknown");
	 exit(1);
      }
}

CLIP_DLLEXPORT char *
_clip_host_charset()
{
   return _clip_hostcs;
}

int
clip_ERRORLEVEL(ClipMachine * ClipMemoryStore)
{
   int level = errorlevel;

   if (ClipMemoryStore->argc > 0)
      errorlevel = _clip_parni(ClipMemoryStore, 1);

   _clip_retni(ClipMemoryStore, level);
   return 0;
}

int
clip___QUIT(ClipMachine * ClipMemoryStore)
{
#ifdef USE_TASKS
/*	Task_killAll();*/
#endif
   exit(errorlevel);
}
