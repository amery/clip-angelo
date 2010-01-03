/*
   Copyright (C) 2001  ITK
   Author   : Paul Lasarev <paul@itk.ru>
   License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/
#include "ci_clipcfg.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#ifdef HAVE_MMAN_H
#include <sys/mman.h>
#endif
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#ifdef OS_MINGW
#include <ltdl.h>
#define dlerror lt_dlerror
#else
#include <dlfcn.h>
#endif
#include <ctype.h>

#include "ci_clip.h"
#include "ci_clipvm.h"
#include "ci_hash.h"
#include "ci_ar.h"
#include "ci_coll.h"

#ifdef USE_TASKS
#include "../libcliptask/ci_task.h"
#endif
#ifndef RTLD_NOW
#define RTLD_NOW DL_LAZY
#endif

#define NEW(type) ((type*)calloc(sizeof(type),1))
#define NEWVECT(type,len) ((type*)calloc(sizeof(type),(len)))
#define VAR(type,var,ini) type *var=(type*)(ini)
#define NEWVAR(type,var) type *var=(type*)calloc(sizeof(type),1)
#define c_DELETE(type,var)	{destroy_##type(var);free(var);}
#define M_OFFS(base,nl,ns) ((base)+8+nl*sizeof(long)+ns*sizeof(short))
#define F_OFFS(fp,nl,ns,nb) ((fp)+(nl)*sizeof(long)+(ns)*sizeof(short)+(nb))

/*static void get_name(char *func, char **sp); */
static void get_str(char *modbeg, int no, char **sp, int *len);

static void get_num(char *modbeg, int no, double *sp, int *len, int *dec);

static void get_static(ClipMachine * ClipMachineMemory, ClipFile * file,
		       ClipVar * statics, char *modbeg, int no, ClipVar ** dest);
static void get_func(char *modbeg, int no, char **fp);

static int run_vm(ClipMachine * ClipMachineMemory, ClipBlock * bp);

static void make_func(struct ClipFile *file, ClipCodeVar * dest, long *hash);

static char *get_proc_name(ClipCodeVar * dest);

/* [ load pcode */

#ifdef FORCEALIGN

static short
GETSHORT(void *p)
{
   short     r;

   memcpy(&r, p, sizeof(r));
   return r;
}

static long
GETLONG(void *p)
{
   long      r;

   memcpy(&r, p, sizeof(r));
   return r;
}

static void
SETLONG(void *ptr, long l)
{
   memcpy(ptr, &l, sizeof(l));
}

static void
SETINT(void *ptr, int l)
{
   memcpy(ptr, &l, sizeof(l));
}

static short
get_short(void *ptr)
{
   short   **pp = (short **) ptr;

   short     r;

   memcpy(&r, pp, sizeof(r));
   (*pp)++;
   return r;
}

static long
get_long(void *ptr)
{
   long    **pp = (long **) ptr;

   long      r;

   memcpy(&r, pp, sizeof(r));
   (*pp)++;
   return r;
}

static unsigned char
get_byte(void *ptr)
{
   unsigned char **pp = (unsigned char **) ptr;

   return *(*pp)++;
}

#else

#define GETSHORT(p) (*(short*)(p))
#define GETLONG(p) (*(long*)(p))
#define SETLONG(ptr,l) (*(long*)(ptr)=(l))
#define SETINT(ptr,l) (*(int*)(ptr)=(l))

static short
get_short(void *ptr)
{
   short   **pp = (short **) ptr;

   return *(*pp)++;
}

static long
get_long(void *ptr)
{
   long    **pp = (long **) ptr;

   return *(*pp)++;
}

static unsigned char
get_byte(void *ptr)
{
   unsigned char **pp = (unsigned char **) ptr;

   return *(*pp)++;
}

#endif

#define GET_SHORT(pp) get_short(&(pp))
#define GET_LONG(pp) get_long(&(pp))
#define GET_BYTE(pp) get_byte(&(pp))

static void
get_str(char *modbeg, int no, char **sp, int *len)
{
   short     strOffs = GETSHORT(F_OFFS(modbeg, 4, 3, 0));

   char     *bp = (modbeg + strOffs);

   bp += 2 * no * sizeof(long);

   SETINT(len, GET_LONG(bp));
   *sp = modbeg + GETLONG(bp);
}

static int
load_pobj(ClipMachine * ClipMachineMemory, struct ClipFile *file, const char *name, int mallocMem)
{
   struct stat st;

   int       fd;

   long      len;

   char     *sp, *end, *modbeg;

   memset(file, 0, sizeof(*file));

   if (stat(name, &st) != 0)
    {
       _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "cannot stat file '%s': %s", name, strerror(errno));
       return _clip_call_errblock(ClipMachineMemory, 1);
    }

   if (!S_ISREG(st.st_mode))
    {
       _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "is not regular file '%s'", name);
       return _clip_call_errblock(ClipMachineMemory, 1);
    }

   len = st.st_size;
   if (len < 38)
    {
       _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "is not pobj file '%s'", name);
       return _clip_call_errblock(ClipMachineMemory, 1);
    }

   fd = open(name, O_RDONLY);
   if (fd < 0)
    {
       _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "open file '%s' error: %s", name, strerror(errno));
       return _clip_call_errblock(ClipMachineMemory, 1);
    }

   file->bodyMem_of_ClipFile = mallocMem;
   file->bodySize_of_ClipFile = len;
#ifdef HAVE_MMAN_H
   if (file->bodyMem_of_ClipFile == 0)
    {
       file->body_of_ClipFile = mmap(NULL, len, PROT_READ, MAP_SHARED, fd, 0);
#ifndef OS_CYGWIN
       close(fd);
       fd = -1;
#endif
       file->fd_of_ClipFile = fd;
       if (file->body_of_ClipFile == (char *) -1)
	{
	   _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "mmap file '%s' error: %s", name, strerror(errno));
	   return _clip_call_errblock(ClipMachineMemory, -1);
	}
    }
   else
#endif
    {
       char     *s;

       int       readed, r;

       file->bodyMem_of_ClipFile = 1;
       file->body_of_ClipFile = (char *) malloc(len);
       for (s = file->body_of_ClipFile, readed = 0; readed < len; readed += r, s += r)
	{
	   int       portion = 4096;

	   if ((len - readed) < portion)
	      portion = len - readed;
	   r = read(fd, s, portion);
	   if (r < portion)
	    {
	       close(fd);
	       free(file->body_of_ClipFile);
	       _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "read file '%s' error: %s", name, strerror(errno));
	       return _clip_call_errblock(ClipMachineMemory, -1);
	    }
	   close(fd);
	   file->fd_of_ClipFile = -1;
	}

    }

   sp = file->mem_of_ClipFile = file->body_of_ClipFile;
   end = sp + len;

   if (sp[0] == '#' && sp[1] == '!')
    {
       for (sp += 2; sp < end; sp++)
	  if (*sp == '\n')
	   {
	      sp++;
	      break;
	   }
    }

   file->body_of_ClipFile = sp;

   if (memcmp(sp, "!<pobj>\n", 8))
    {
       _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, " '%s' is not valid pobj file: invalid magic", name);
       destroy_ClipFile(ClipMachineMemory, file);
       return _clip_call_errblock(ClipMachineMemory, 1);
    }

   sp += 8;
   sp += sizeof(long);

  /*
     if (GETLONG( sp != (len - 8 - 2 * sizeof(long))))
     {
     _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, " '%s' is not valid pobj file: invalid length", name);
     destroy_ClipFile(ClipMachineMemory, file);
     return 1;
     }
   */
   sp += sizeof(long);

   modbeg = sp;
   sp += sizeof(long);

   file->nstatics_of_ClipFile = GETLONG(sp);

   file->ClipVar_statics_of_ClipFile = (ClipVar *) calloc(sizeof(ClipVar), file->nstatics_of_ClipFile + 1);
   file->staticsMem_of_ClipFile = 1;

   sp += sizeof(short) * 7 + sizeof(long) * 4;

  /*++sp; */
   file->name_of_ClipFile = strdup(sp);

   _clip_hash_buckets(ClipMachineMemory, file);

   return 0;
}

void
_clip_hash_buckets(ClipMachine * ClipMachineMemory, ClipFile * file)
{
   char     *hp = 0;

   int       len = 0;

   int       count = 0;

   int       i;

   int       nstrings;

   int       nstatics;

   ClipHashNames *hash_names;

   ClipHashBucket *buckets;

   char     *modbeg;

   ClipVarDef *statics;

   if (file->hash_names_of_ClipFile)
      return;

   modbeg = file->body_of_ClipFile + 8 + 2 * sizeof(long);

   nstrings = GETSHORT(F_OFFS(modbeg, 2, 1, 0));
   get_str(modbeg, nstrings - 1, &hp, &len);

   count = GETLONG(F_OFFS(hp, 0, 0, 0));
   nstatics = GETLONG(F_OFFS(hp, 1, 0, 0));

   hash_names = NEW(ClipHashNames);
   if (count)
      buckets = (ClipHashBucket *) calloc(sizeof(ClipHashBucket), count);
   else
      buckets = 0;
   hash_names->num_of_ClipHashNames = count;
   hash_names->buckets_of_ClipHashNames = buckets;
   hash_names->mem_of_ClipHashNames = F_OFFS(hp, 2 + nstatics + 2 * count, 0, 0);

   for (i = 0; i < count; i++)
    {
       long      hash, offs;

       hash = buckets[i].hash_of_ClipHashBucket = GETLONG(F_OFFS(hp, 2 + nstatics + 2 * i, 0, 0));
       offs = buckets[i].offs_of_ClipHashBucket = GETLONG(F_OFFS(hp, 2 + nstatics + 2 * i + 1, 0, 0));
       if (_clip_debuglevel)
	{
	   char     *str, *s;

	   s = (char *) HashTable_fetch(ClipMachineMemory->hashnames, hash);
	   if (!s)
	    {
	       str = hash_names->mem_of_ClipHashNames + offs;
	       s = strdup(str);
	       HashTable_store(ClipMachineMemory->hashnames, s, hash);
	    }
	}
    }

   file->hash_names_of_ClipFile = hash_names;

   statics = file->ClipVarDef_staticDefs_of_ClipFile = (ClipVarDef *) calloc(sizeof(ClipVarDef), nstatics + 1);

   file->ClipVarDef_staticDefs_of_ClipFile[0].name_of_ClipVarDef = nstatics;
   for (i = 0; i < nstatics; i++)
    {
       statics[i + 1].name_of_ClipVarDef = GETLONG(F_OFFS(hp, 2 + i, 0, 0));
       statics[i + 1].ClipVar_vp_of_ClipVarDef = file->ClipVar_statics_of_ClipFile + i + 1;
    }
}

static Coll *loaded_dlls = 0;

static int
load_dll(ClipMachine * ClipMachineMemory, const char *name, struct Coll *names, ClipVar * resp)
{
   void     *hp;

   char      buf[256], *s, *e;

   char      uname[128];

   const char **spp;

   ClipModule *entry;

   ClipFunction **fpp;

   ClipNameEntry *np;

   ClipFile **cpp;

   struct DBFuncTable **dpp;

   int       l, ret = 0, i;

   s = strrchr(name, '/');
   if (s)
      snprintf(buf, sizeof(buf), "%s", name);
   else
      snprintf(buf, sizeof(buf), "%s/lib/%s ", CLIPROOT, name);

   if (!loaded_dlls)
    {
       loaded_dlls = new_Coll(free, strcmp);
    }
   else
    {
       if (search_Coll(loaded_dlls, buf, 0))
	  return 0;
    }

#ifdef OS_MINGW
   hp = lt_dlopen(buf);
#else
   hp = dlopen(buf, RTLD_NOW);
#endif
   if (!hp)
    {
       _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "shared loading problem: %s: file %s", dlerror(), buf);
       return _clip_call_errblock(ClipMachineMemory, 1);
    }

   insert_Coll(loaded_dlls, strdup(buf));

/*
   path/name.ext -> name_module entry symbol
 */

   s = strrchr(name, '/');
   if (!s)
      s = (char *) name;
   else
      s = s + 1;
   e = strchr(s, '.');
   if (e)
      l = e - s;
   else
      l = strlen(s);
   if (l > sizeof(uname))
      l = sizeof(uname);

   for (i = 0; i < l; i++, s++)
    {
       if (*s == '-')
	  uname[i] = '_';
       else
	  uname[i] = toupper(*s);
    }
   uname[l] = 0;
   snprintf(buf, sizeof(buf), "clip__MODULE_%s", uname);

#ifdef OS_MINGW
   entry = (ClipModule *) lt_dlsym(hp, buf);
#else
   entry = (ClipModule *) dlsym(hp, buf);
#endif

   if (!entry)
    {
       _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__,
			 "shared '%s' fetch name '%s' problem: %s", name, buf, dlerror());
       return _clip_call_errblock(ClipMachineMemory, 1);
    }

   for (np = entry->ClipNameEntry_cfunctions_of_ClipModule; np && np->ClipFunction_function_of_ClipNameEntry; ++np)
      _clip_register_hash(ClipMachineMemory, np->ClipFunction_function_of_ClipNameEntry, np->hash_of_ClipNameEntry);

   for (fpp = entry->ClipFunction_inits_of_ClipModule; fpp && *fpp; ++fpp)
      _clip_main_func(ClipMachineMemory, *fpp, _clip_argc, _clip_argv, _clip_envp);

   for (fpp = entry->ClipFunction_exits_of_ClipModule; fpp && *fpp; ++fpp)
    {
       ClipMachineMemory->cexits =
	(ClipFunction **) realloc(ClipMachineMemory->cexits, (ClipMachineMemory->ncexits + 1) * sizeof(ClipFunction *));
       ClipMachineMemory->cexits[ClipMachineMemory->ncexits] = *fpp;
       ++ClipMachineMemory->ncexits;
    }

   for (spp = entry->pfunctions_of_ClipModule; spp && *spp; ++spp)
      if (_clip_load(ClipMachineMemory, *spp, 0, 0))
	 ++ret;
/*
	if (entry->cpfile && _clip_register_file(ClipMachineMemory, entry->cpfile))
		++ret;
*/
   for (cpp = entry->cpfiles_of_ClipModule; cpp && *cpp; ++cpp)
      if (_clip_register_file(ClipMachineMemory, *cpp))
	 ++ret;

   for (dpp = entry->dbdrivers_of_ClipModule; dpp && *dpp; ++dpp)
      if (_clip_register_driver(ClipMachineMemory, *dpp))
	 ++ret;

   add_ClipVect(&ClipMachineMemory->dlls, hp);

   return ret;
}

CLIP_DLLEXPORT int
_clip_load_inits(ClipMachine * ClipMachineMemory, ClipFile * file)
{
   long      funcOffs = GETLONG(M_OFFS(file->body_of_ClipFile, 7, 4));

   char     *modbeg = M_OFFS(file->body_of_ClipFile, 2, 0);

   int       pubnum = GETLONG(M_OFFS(file->body_of_ClipFile, 4, 2));

   int       initnum = GETSHORT(M_OFFS(file->body_of_ClipFile, 8, 4));

   int       exitnum = GETSHORT(M_OFFS(file->body_of_ClipFile, 8, 5));

   ClipBlock b;

   ClipCodeVar c;

   int       i;

   long      hash;

   c.u.ClipBlock_block_of_ClipCodeVar = &b;

   file->pos_of_ClipFile = (long *) (modbeg + funcOffs);
   file->pos_of_ClipFile += 2 * pubnum;
  /*file->end = file->pos + 2 * num; */
   for (i = 0; i < initnum; ++i)
    {
       make_func(file, &c, &hash);
       file->pos_of_ClipFile += 2;
       _clip_main_code(ClipMachineMemory, c.u.ClipBlock_block_of_ClipCodeVar, _clip_argc, _clip_argv, _clip_envp);
    }
   for (i = 0; i < exitnum; ++i)
    {
       make_func(file, &c, &hash);
       file->pos_of_ClipFile += 2;
       ClipMachineMemory->exits =
	(ClipBlock *) realloc(ClipMachineMemory->exits, (ClipMachineMemory->nexits + 1) * sizeof(ClipBlock));
       ClipMachineMemory->exits[ClipMachineMemory->nexits] = *c.u.ClipBlock_block_of_ClipCodeVar;
       ++file->refCount_of_ClipFile;
       ++ClipMachineMemory->nexits;
    }
   return 0;
}

CLIP_DLLEXPORT int
_clip_load(ClipMachine * ClipMachineMemory, const char *name, struct Coll *names, ClipVar * resp)
{
   ClipFile *file = 0;

   int       r = 0;

   ClipBlock b;

   ClipCodeVar c;

   long      hash;

   char     *e = strrchr(name, '.');

   char     *s = strrchr(name, '/');

   c.u.ClipBlock_block_of_ClipCodeVar = &b;
   if (e && (!s || s < e))
    {
       if (!strcmp(e, ".po") || !strcmp(e, ".pa"))
	{
	   file = NEW(ClipFile);
	   r = load_pobj(ClipMachineMemory, file, name, 0);
	}
       else if (!strcasecmp(e, DLLREALSUFF))
	{
	   r = load_dll(ClipMachineMemory, name, names, resp);
	   return r;
	}
       else
	{
	   _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "unknown file type: '%s'", name);
	   return _clip_call_errblock(ClipMachineMemory, 1);
	}
    }
   else
    {
       char      path[256];

       snprintf(path, sizeof(path), "%s.po", name);
       if (!access(path, R_OK))
	{
	   file = NEW(ClipFile);
	   r = load_pobj(ClipMachineMemory, file, path, 0);
	}
       else
	{
	   snprintf(path, sizeof(path), "%s.pa", name);
	   if (!access(path, R_OK))
	    {
	       file = NEW(ClipFile);
	       r = load_pobj(ClipMachineMemory, file, path, 0);
	    }
	   else
	    {
	       snprintf(path, sizeof(path), "%s%s", name, DLLREALSUFF);
	       if (!access(path, R_OK))
		{
		   r = load_dll(ClipMachineMemory, path, names, resp);
		   return r;
		}
	       else
		{
		   _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "cannot load file: '%s'", name);
		   return _clip_call_errblock(ClipMachineMemory, 1);
		}
	    }
	}
    }

   if (r)
      return r;

   ++file->refCount_of_ClipFile;

   for (r = _clip_first_File(file, &c, &hash); r; r = _clip_next_File(file, &c, &hash))
    {
       if (c.ClipType_type_of_ClipCodeVar.ClipVartype_type_of_ClipType == CCODE_type_of_ClipVarType)
	{
	   if (resp)
	    {
	       ClipVar  *vp;

	       vp = NEW(ClipVar);

	       CLEAR_CLIPVAR(resp);
	       resp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR_ClipFlags;
	       resp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CCODE_type_of_ClipVarType;
	       resp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = vp;

	       vp->ClipType_t_of_ClipVar.count_of_ClipType = 1;
	       vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CCODE_type_of_ClipVarType;
	       vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	       vp->ClipCodeVar_c_of_ClipVar.u.ClipFunction_function_of_ClipCodeVar = c.u.ClipFunction_function_of_ClipCodeVar;
	       ++file->refCount_of_ClipFile;
	       break;
	    }
	   else
	    {
	       if (_clip_register_hash(ClipMachineMemory, c.u.ClipFunction_function_of_ClipCodeVar, hash))
		  ++file->refCount_of_ClipFile;
	    }
	}
       else if (c.ClipType_type_of_ClipCodeVar.ClipVartype_type_of_ClipType == PCODE_type_of_ClipVarType)
	{
	   if (resp)
	    {
	       ClipVar  *vp;

	       ClipBlock *bp;

	       vp = NEW(ClipVar);
	       bp = NEW(ClipBlock);

	       CLEAR_CLIPVAR(resp);
	       resp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR_ClipFlags;
	       resp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = PCODE_type_of_ClipVarType;
	       resp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = vp;

	       vp->ClipType_t_of_ClipVar.count_of_ClipType = 1;
	       vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = PCODE_type_of_ClipVarType;
	       vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	       *bp = *c.u.ClipBlock_block_of_ClipCodeVar;
	       vp->ClipCodeVar_c_of_ClipVar.u.ClipBlock_block_of_ClipCodeVar = bp;
	       ++file->refCount_of_ClipFile;
	       break;
	    }
	   else
	    {
	       if (_clip_register_block(ClipMachineMemory, *c.u.ClipBlock_block_of_ClipCodeVar, hash))
		{
		   char     *name;

		   if (names)
		    {
		       name = get_proc_name(&c);
		       append_Coll(names, strdup(name));
		    }

		   ++file->refCount_of_ClipFile;
		}
	    }
	}
    }

   _clip_load_inits(ClipMachineMemory, file);

   delete_ClipFile(ClipMachineMemory, file);

   return 0;
}

void
delete_ClipFile(ClipMachine * ClipMachineMemory, ClipFile * fp)
{
   if (destroy_ClipFile(ClipMachineMemory, fp))
      free(fp);
}

int
destroy_ClipFile(ClipMachine * ClipMachineMemory, ClipFile * fp)
{
   int       i;

   if (!fp)
      return 0;
   --fp->refCount_of_ClipFile;
   if (fp->refCount_of_ClipFile > 0)
      return 0;

   switch (fp->bodyMem_of_ClipFile)
    {
#ifndef OS_MINGW
#ifdef HAVE_MMAN_H
    case 0:
       munmap(fp->mem_of_ClipFile, fp->bodySize_of_ClipFile);
       break;
#endif
#endif
    case 1:
       free(fp->mem_of_ClipFile);
       break;
    }
   if (fp->fd_of_ClipFile > 2)
      close(fp->fd_of_ClipFile);

   for (i = 0; i < fp->nstatics_of_ClipFile; ++i)
      _clip_destroy(ClipMachineMemory, fp->ClipVar_statics_of_ClipFile + i);

   switch (fp->staticsMem_of_ClipFile)
    {
#ifndef OS_MINGW
#ifdef HAVE_MMAN_H
    case 0:
       munmap((void *) fp->ClipVar_statics_of_ClipFile, sizeof(ClipVar) * fp->nstatics_of_ClipFile);
       break;
#endif
#endif
    case 1:
       free(fp->ClipVar_statics_of_ClipFile);
       break;
    }

   free(fp->name_of_ClipFile);
   if (fp->hash_names_of_ClipFile)
    {
       free(fp->hash_names_of_ClipFile->buckets_of_ClipHashNames);
      /*free(fp->hash_names->mem); */
       free(fp->hash_names_of_ClipFile);
    }
   if (fp->ClipVarDef_staticDefs_of_ClipFile)
      free(fp->ClipVarDef_staticDefs_of_ClipFile);

   return 1;
}

static void
make_func(struct ClipFile *file, ClipCodeVar * dest, long *hash)
{
   char     *modbeg = M_OFFS(file->body_of_ClipFile, 2, 0);

   dest->ClipType_type_of_ClipCodeVar.ClipVartype_type_of_ClipType = PCODE_type_of_ClipVarType;
   dest->ClipType_type_of_ClipCodeVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   dest->u.ClipBlock_block_of_ClipCodeVar->file_of_ClipBlock = file;
   SETLONG(hash, GETLONG(file->pos_of_ClipFile + 0));
   dest->u.ClipBlock_block_of_ClipCodeVar->function_of_ClipBlock = modbeg + GETLONG(file->pos_of_ClipFile + 1);
}

static char *
get_proc_name(ClipCodeVar * dest)
{
   int       nprivates, nlocals, nreflocals, numlocals;

   char     *procname, *func;

   if (dest->ClipType_type_of_ClipCodeVar.ClipVartype_type_of_ClipType != PCODE_type_of_ClipVarType)
      return "";

   func = dest->u.ClipBlock_block_of_ClipCodeVar->function_of_ClipBlock;
   nlocals = GETSHORT(F_OFFS(func, 3, 0, 1));
   nreflocals = GETSHORT(F_OFFS(func, 3, 1, 1));
   numlocals = nlocals + nreflocals;
   nprivates = GETSHORT(F_OFFS(func, 3, 3, 1));
   procname = F_OFFS(func, 3 + nprivates + numlocals, 5 + numlocals, 1 + 1);

   return procname;
}

int
_clip_first_File(struct ClipFile *file, ClipCodeVar * dest, long *hash)
{
   long      funcOffs = GETLONG(M_OFFS(file->body_of_ClipFile, 7, 4));

   char     *modbeg = M_OFFS(file->body_of_ClipFile, 2, 0);

   int       pubnum = GETLONG(M_OFFS(file->body_of_ClipFile, 4, 2));

  /*int initnum = GETLONG( M_OFFS(file->body_of_ClipFile, 7, 5)); */
  /*int exitnum = GETLONG( M_OFFS(file->body_of_ClipFile, 7, 6)); */
   int       num;

   num = pubnum;

   file->pos_of_ClipFile = (long *) (modbeg + funcOffs);
   file->end_of_ClipFile = file->pos_of_ClipFile + 2 * num;
   if (num == 0)
      return 0;
   make_func(file, dest, hash);
   file->pos_of_ClipFile += 2;

   return 1;
}

int
_clip_next_File(struct ClipFile *file, ClipCodeVar * dest, long *hash)
{
   if (file->pos_of_ClipFile == file->end_of_ClipFile)
      return 0;
   make_func(file, dest, hash);
   file->pos_of_ClipFile += 2;
   return 1;
}

/* ] load pcode */
/* [ ClipBlock */

void
_clip_init_Block(ClipMachine * ClipMachineMemory, ClipBlock * dest, struct ClipFile *file, char *func)
{
   dest->file_of_ClipBlock = file;
   dest->function_of_ClipBlock = func;
   ++file->refCount_of_ClipFile;
}

void
destroy_Block(ClipMachine * ClipMachineMemory, ClipBlock * bp)
{
   delete_ClipFile(ClipMachineMemory, bp->file_of_ClipBlock);
}

void
_clip_dup_Block(ClipMachine * ClipMachineMemory, ClipBlock * src, ClipBlock * dest)
{
   *dest = *src;
   ++src->file_of_ClipBlock->refCount_of_ClipFile;
}

#if 0
static void
get_name(char *func, char **sp)
{
   short     npriv = GETSHORT(F_OFFS(func, 3, 2, 1));

   char     *name = F_OFFS(func, 3, 3, 1) + npriv * sizeof(long) + 1;

   *sp = name;
}
#endif

static void
get_num(char *modbeg, int no, double *sp, int *len, int *dec)
{
   short     numOffs = GETSHORT(F_OFFS(modbeg, 4, 2, 0));

   short     numNum = GETSHORT(F_OFFS(modbeg, 2, 0, 0));

   double   *bp = (double *) (modbeg + numOffs);

   unsigned char *lens = (unsigned char *) (bp + numNum);

   unsigned char *decs = lens + numNum;

   memcpy(sp, bp + no, sizeof(*sp));
   memcpy(len, lens + no, sizeof(*len));
   memcpy(dec, decs + no, sizeof(*dec));
}

static void
get_static(ClipMachine * ClipMachineMemory, ClipFile * file, ClipVar * statics, char *modbeg, int no, ClipVar ** dest)
{
   if (!statics->ClipType_t_of_ClipVar.count_of_ClipType)
    {
       ClipBlock init;

       long      initOffs = GETLONG(F_OFFS(modbeg, 4, 4, 0));

       statics->ClipType_t_of_ClipVar.count_of_ClipType = 1;
       init.file_of_ClipBlock = file;
       init.function_of_ClipBlock = modbeg + initOffs;
       _clip_code_func(ClipMachineMemory, &init, 0, 0, 0);
    }
   *dest = statics + no + 1;
}

static void
get_func(char *modbeg, int no, char **fp)
{
   long      funcOffs = GETLONG(F_OFFS(modbeg, 5, 4, 0));

   long     *bp = (long *) (modbeg + funcOffs);

   bp += 2 * no;
   ++bp;
   *fp = modbeg + GETLONG(bp);
}

/* ] ClipBlock */
/* [ run codeblock */

static int
do_code(ClipMachine * ClipMachineMemory, struct ClipBlock *cp, int argc, int isProc, int rest, ClipVarFrame * uplocals)
{
   int       ret, _argc = ClipMachineMemory->argc;

   ClipFrame *fp = ClipMachineMemory->fp;

   ClipVar  *bp = ClipMachineMemory->bp;

   ClipVarFrame *s_up = ClipMachineMemory->fp->ClipVarFrame_localvars_of_ClipFrame;

   ClipMachineMemory->argc = argc;
   ClipMachineMemory->bp = fp->ClipVar_sp_of_ClipFrame;
   ClipMachineMemory->fp->ClipVarFrame_localvars_of_ClipFrame = uplocals;

   if (_clip_profiler)
    {
       _clip_stop_profiler(ClipMachineMemory);
    }

   _clip_logg(6, "PCODE call from: proc '%s' file '%s' line %d",
	      fp->procname_of_ClipFrame ? fp->procname_of_ClipFrame : "unknown", fp->filename_of_ClipFrame,
	      fp->line_of_ClipFrame);
   for (;;)
    {
       int       dnext = 0;

       if (_clip_debuglevel)
	{
	   dnext = _clip_debugnext;
	   _clip_debug(ClipMachineMemory);
	}

#ifdef USE_TASKS
       Task_yield();
#endif
       ret = run_vm(ClipMachineMemory, cp);

       if (_clip_sig_flag)
	  _clip_signal_real(_clip_sig_flag);

       if (_clip_debuglevel)
	{
	   if (!dnext && _clip_debugnext)
	    {
	       _clip_debugnext = 0;
	    }
	}
       _clip_logg(4, "PCODE return=%d to: proc '%s' file '%s' line %d", ret,
		  fp->procname_of_ClipFrame ? fp->procname_of_ClipFrame : "unknown", fp->filename_of_ClipFrame,
		  fp->line_of_ClipFrame);

       if (!ret)
	  break;

       if (!ClipMachineMemory->trapVar)
	  _clip_generr(ClipMachineMemory, ret);

       if (ret < 0)
	  break;

       if (!_clip_errorblock(ClipMachineMemory, ClipMachineMemory->trapVar, ret))
	{
	   ret = -ret;
	   break;
	}

       ClipMachineMemory->argc = _argc;
       ClipMachineMemory->fp = fp;
       ClipMachineMemory->bp = bp;
       ClipMachineMemory->fp->ClipVarFrame_localvars_of_ClipFrame = s_up;
    }

   ClipMachineMemory->argc = _argc;
   ClipMachineMemory->fp = fp;
   ClipMachineMemory->bp = bp;
   ClipMachineMemory->fp->ClipVarFrame_localvars_of_ClipFrame = s_up;

   if (isProc)
      ++argc;
   for (; argc; --argc)
    {
       --(fp->ClipVar_sp_of_ClipFrame);
       _clip_destroy(ClipMachineMemory, fp->ClipVar_sp_of_ClipFrame);
    }

   if (_clip_profiler)
    {
       _clip_start_profiler(ClipMachineMemory);
       ClipMachineMemory->pbucket->callno_of_ProfileBucket--;
    }

   return ret;
}

CLIP_DLLEXPORT int
_clip_code_func(ClipMachine * ClipMachineMemory, struct ClipBlock *cp, int argc, int rest, ClipVarFrame * uplocals)
{
   return do_code(ClipMachineMemory, cp, argc, 0, rest, uplocals);
}

CLIP_DLLEXPORT int
_clip_code_proc(struct ClipMachine *ClipMachineMemory, struct ClipBlock *cp, int argc, int rest, ClipVarFrame * uplocals)
{
   return do_code(ClipMachineMemory, cp, argc, 1, rest, uplocals);
}

static int
search_long(long *coll, int count, long key)
{
   int       l, h, i;

   long      c;

   l = 0;
   h = count - 1;
   while (l <= h)
    {
       i = (l + h) >> 1;
       if ((c = GETLONG(coll + i)) < key)
	  l = i + 1;
       else
	{
	   if (c == key)
	      return i;
	   h = i - 1;
	}
    }
   return -1;
}

static int
run_vm(ClipMachine * ClipMachineMemory, ClipBlock * bp)
{
   char     *func = bp->function_of_ClipBlock;

   char     *modbeg = func - GETLONG(func);

   char     *pp = modbeg + GETLONG(F_OFFS(func, 1, 0, 0));

   char      code;

   long      len = GETLONG(F_OFFS(func, 2, 0, 0));

   char     *beg = pp, *end = pp + len;

   long      staticsOffs = GETLONG(modbeg);

   ClipFile *file = bp->file_of_ClipBlock;

   ClipVar  *statics = file->ClipVar_statics_of_ClipFile + staticsOffs;

   int       nlocals = GETSHORT(F_OFFS(func, 3, 0, 1));

   int       nreflocals = GETSHORT(F_OFFS(func, 3, 1, 1));

   int       ret = 0;

   int       i;

#if 0
   ClipVar  *locals /* = (ClipVar *) alloca(sizeof(ClipVar) * nlocals) */ ;
#endif
   int       maxdeep = GETSHORT(F_OFFS(func, 3, 2, 1));

#ifdef OS_MINGW
   ClipVar  *stack = (ClipVar *) malloc(sizeof(ClipVar) * maxdeep);
#else
   ClipVar  *stack = (ClipVar *) alloca(sizeof(ClipVar) * maxdeep);
#endif
   char     *filename = F_OFFS(modbeg, 7, 4, 0);

   int       nprivates = GETSHORT(F_OFFS(func, 3, 3, 1));

  /*int nparams = GETSHORT( F_OFFS(func, 3, 4, 1)); */
  /*long *privates = (long *) F_OFFS(func, 3, 5, 1); */
  /*long *localDefHashs = (long *) F_OFFS(func, 3 + nprivates, 5, 1); */
   int       numlocals = nlocals + nreflocals;

  /*ClipVarDef *ldp, *localDefs = (ClipVarDef *) alloca(sizeof(ClipVarDef) * (numlocals+1)); */
  /*short *localDefNums = (short *) F_OFFS(func, 3 + nprivates + numlocals, 5, 1); */

   char     *procname = F_OFFS(func, 3 + nprivates + numlocals, 5 + numlocals, 1 + 1);

   char     *localnames = procname + *(unsigned char *) F_OFFS(func, 3 + nprivates + numlocals,
							       5 + numlocals, 1) + 1;

  /*ClipVar *params = (ClipVar *)alloca( nparams*sizeof(ClipVar)); */
   ClipFrame frame = { stack, stack, filename, 0, 0, 0, 0 /*localDefs */ , file->ClipVarDef_staticDefs_of_ClipFile, 0,
      file->hash_names_of_ClipFile, procname, maxdeep, 0
   };
   unsigned char c, c1;

   short     s, s1;

   long      l, l1;

   ClipVar  *vp;

   ClipBlock bl;

   char     *trap_pp = 0;

   int       debuglevel = _clip_debuglevel;

   ClipVarFrame *localvars = 0, *reflocals = 0;

   int       local_locals = 0;

   if (!nlocals && !nreflocals)
    {
       reflocals = frame.ClipVarFrame_localvars_of_ClipFrame = ClipMachineMemory->fp->ClipVarFrame_localvars_of_ClipFrame;
       if (reflocals && reflocals->refcount_of_ClipVarFrame)
	  reflocals->refcount_of_ClipVarFrame++;
    }
   else if (nreflocals)
    {
       localvars = (ClipVarFrame *) calloc(1, sizeof(ClipVarFrame) + numlocals * sizeof(ClipVar));
       localvars->ClipVar_vars_of_ClipVarFrame = (ClipVar *) (localvars + 1);
       localvars->refcount_of_ClipVarFrame = 1;
       reflocals = frame.ClipVarFrame_localvars_of_ClipFrame = localvars;
       localvars->size_of_ClipVarFrame = numlocals;
       localvars->names_of_ClipVarFrame = localnames;
    }
   else
    {
#if 1
       localvars = (ClipVarFrame *) calloc(1, sizeof(ClipVarFrame) + numlocals * sizeof(ClipVar));
       localvars->ClipVar_vars_of_ClipVarFrame = (ClipVar *) (localvars + 1);
       localvars->size_of_ClipVarFrame = numlocals;
       localvars->refcount_of_ClipVarFrame = 1;
       localvars->names_of_ClipVarFrame = localnames;
       reflocals = frame.ClipVarFrame_localvars_of_ClipFrame = localvars;
#else
       locals = (ClipVar *) alloca(sizeof(ClipVar) * numlocals);
       memset(locals, 0, numlocals * sizeof(ClipVar));
       localvars = (ClipVarFrame *) alloca(sizeof(ClipVarFrame));
       localvars->vars = locals;
       localvars->size_of_ClipVarFrame_of_ClipVarFrame = numlocals;
       localvars->refcount_of_ClipVarFrame = 0;
       localvars->names_of_ClipVarFrame = localnames;
       reflocals = frame.ClipVarFrame_localvars_of_ClipFrame = localvars;
       local_locals = 1;
#endif
    }

   frame.ClipFrame_up_of_ClipFrame = ClipMachineMemory->fp;
   ClipMachineMemory->fp = &frame;

#if 0
   localDefs[0].name = numlocals;
   for (i = 0, ldp = localDefs + 1; i < numlocals; i++, ldp++, localDefHashs++, localDefNums++)
    {
      /*int no = *localDefNums; */
       long      hash = *localDefHashs;

       ldp->name = hash;
      /*
         if (no < 0)
         ldp->vp = reflocals - no;
         else
       */
      /*ldp->vp = locals + no; */
       ldp->vp = 0;
    }
#endif

   if (_clip_profiler)
      _clip_start_profiler(ClipMachineMemory);

   _clip_logg(4, "PCODE call: proc '%s' file '%s' line %d",
	      frame.procname_of_ClipFrame ? frame.procname_of_ClipFrame : "unknown", frame.filename_of_ClipFrame,
	      frame.line_of_ClipFrame);

 cont:
   while (pp >= beg && pp < end)
    {
       if (debuglevel)
	  _clip_debug(ClipMachineMemory);
       switch ((code = *pp++))
	{
	case CLIP_NOP:
	   break;
	case CLIP_POP:
	   _clip_pop(ClipMachineMemory);
	   break;
	case CLIP_LINE:
	   frame.line_of_ClipFrame = GET_SHORT(pp);
	   if (debuglevel)
	      _clip_line(ClipMachineMemory);
	   break;
	case CLIP_PUSH_NUM:
	   {
	      int       len, dec;

	      double    d;

	      s = GET_SHORT(pp);
	      frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	      frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	      frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	      get_num(modbeg, s, &d, &len, &dec);
	      frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.len_of_ClipType = len;
	      frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.dec_of_ClipType = dec;
	      frame.ClipVar_sp_of_ClipFrame->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = d;
	      frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	      ++frame.ClipVar_sp_of_ClipFrame;
	   }
	   break;
	case CLIP_PUSH_STR:
#if 0
	   s = GET_SHORT(pp);
	   frame.sp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	   frame.sp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MSTAT_ClipFlags;
	   frame.sp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	   get_str(modbeg, s, &(frame.sp->s.ClipBuf_str_of_ClipStrVar.buf), &(frame.sp->s.ClipBuf_str_of_ClipStrVar.len));
	  /*
	     if ( !(ClipMachineMemory->flags1 & NOEXPAND_MACRO_FLAG) )
	     {
	   */
	   if (strchr(frame.sp->s.ClipBuf_str_of_ClipStrVar.buf, '&'))
	      _clip_expand_var(ClipMachineMemory, frame.sp);
	  /*} */
	   ++frame.sp;
	   break;
#endif
	case CLIP_PUSH_STRDUP:
	   s = GET_SHORT(pp);
	   frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	   frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	   frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	   {
	      char     *str = "";

	      int       len = 0;

	      get_str(modbeg, s, &str, &len);
	      frame.ClipVar_sp_of_ClipFrame->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf =
	       _clip_memdup(str, len);
	      frame.ClipVar_sp_of_ClipFrame->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = len;
	   }
	  /*
	     if ( !(ClipMachineMemory->flags1 & NOEXPAND_MACRO_FLAG) )
	     {
	   */
	   if (strchr(frame.ClipVar_sp_of_ClipFrame->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, '&'))
	      _clip_expand_var(ClipMachineMemory, frame.ClipVar_sp_of_ClipFrame);
	  /*} */
	   ++frame.ClipVar_sp_of_ClipFrame;
	   break;
	case CLIP_PUSH_NIL:
	   frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = UNDEF_type_of_ClipVarType;
	   frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	   ++frame.ClipVar_sp_of_ClipFrame;
	   break;
	case CLIP_PUSH_TRUE:
	   frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
	   frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	   frame.ClipVar_sp_of_ClipFrame->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = 1;
	   ++frame.ClipVar_sp_of_ClipFrame;
	   break;
	case CLIP_PUSH_FALSE:
	   frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
	   frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	   frame.ClipVar_sp_of_ClipFrame->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = 0;
	   ++frame.ClipVar_sp_of_ClipFrame;
	   break;
	case CLIP_MEMVAR_PUBLIC:
	   l = GET_LONG(pp);
	   _clip_memvar_public(ClipMachineMemory, l);
	   break;
	case CLIP_MEMVAR_SPACE:
	   l = GET_LONG(pp);
	   l1 = GET_LONG(pp);
	   _clip_memvar_space(ClipMachineMemory, _clip_space(ClipMachineMemory, l), l1, 1);
	   break;
	case CLIP_MEMVAR_PRIVATE:
	   l = GET_LONG(pp);
	   _clip_memvar_private(ClipMachineMemory, l);
	   break;
	case CLIP_MEMVAR_PUBLIC_POP:
	   l = _clip_pop_hash(ClipMachineMemory);
	   _clip_memvar_public(ClipMachineMemory, l);
	   break;
	case CLIP_MEMVAR_SPACE_POP:
	   l = GET_LONG(pp);
	   l1 = _clip_pop_hash(ClipMachineMemory);
	   _clip_memvar_space(ClipMachineMemory, _clip_space(ClipMachineMemory, l), l1, 1);
	   break;
	case CLIP_MEMVAR_PRIVATE_POP:
	   l = _clip_pop_hash(ClipMachineMemory);
/*_clip_memvar_private(ClipMachineMemory, l);*/
	   _clip_add_private(ClipMachineMemory, l);
	   break;
	case CLIP_MEMVAR_PARAM:
	   l = GET_LONG(pp);
	   c = GET_BYTE(pp);
	   _clip_memvar_param(ClipMachineMemory, l, c);
	   break;
	case CLIP_PUSH_PARAM:
	   c = GET_BYTE(pp);
	   s = GET_SHORT(pp);
	   _clip_param(ClipMachineMemory, c, s);
	   break;
	case CLIP_PUSH_LOCAL:
	   s = GET_SHORT(pp);
	   if ((ret = _clip_push_local(ClipMachineMemory, s)))
	      goto _trap;
	   break;
	case CLIP_PUSH_REF_LOCAL:
	   s = GET_SHORT(pp);
#if 0
	   frame.sp->ClipRefVar_p_of_ClipVar.vp = _clip_ref_local(ClipMachineMemory, s);
#else
	   {
	      ClipVar  *vp1 = _clip_ref_local(ClipMachineMemory, s);

	      frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = vp1;
	   }
#endif
	   break;
	case CLIP_PUSH_STATIC:
	   s = GET_SHORT(pp);
	   get_static(ClipMachineMemory, file, statics, modbeg, s, &vp);
	   if ((ret = _clip_push_static(ClipMachineMemory, vp)))
	      goto _trap;
	   break;
	case CLIP_PUSH_REF_STATIC:
	   s = GET_SHORT(pp);
	   get_static(ClipMachineMemory, file, statics, modbeg, s, &vp);
	   frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = vp;
	   break;
	case CLIP_PUSH_MEMVAR:
	   l = GET_LONG(pp);
	   if ((ret = _clip_memvar(ClipMachineMemory, l)))
	      goto _trap;
	   break;
	case CLIP_PUSH_FMEMVAR:
	   l = GET_LONG(pp);
	   if ((ret = _clip_fmemvar(ClipMachineMemory, l)))
	      goto _trap;
	   break;
	case CLIP_PUSH_MEMVARF:
	   l = GET_LONG(pp);
	   if ((ret = _clip_memvarf(ClipMachineMemory, l)))
	      goto _trap;
	   break;
	case CLIP_REF_FMEMVAR:
	   l = GET_LONG(pp);
	   if ((ret = _clip_ref_fmemvar(ClipMachineMemory, l)))
	      goto _trap;
	   break;
	case CLIP_PUSH_REF_MEMVAR:
	   l = GET_LONG(pp);
#if 0
	   frame.sp->ClipRefVar_p_of_ClipVar.vp = _clip_ref_memvar(ClipMachineMemory, l);
#else
	   {
	      ClipVar  *vp1 = _clip_ref_memvar(ClipMachineMemory, l);

	      frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = vp1;
	   }
#endif
	   break;
	case CLIP_PUSH_REF_MEMVAR_NOADD:
	   l = GET_LONG(pp);
	   {
	      ClipVar  *vp1 = _clip_ref_memvar_noadd(ClipMachineMemory, l);

	      frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = vp1;
	   }
	   break;
	case CLIP_PUSH_PUBLIC:
	   l = GET_LONG(pp);
	   if ((ret = _clip_public(ClipMachineMemory, l)))
	      goto _trap;
	   break;
	case CLIP_PUSH_REF_PUBLIC:
	   l = GET_LONG(pp);
#if 0
	   frame.sp->ClipRefVar_p_of_ClipVar.vp = _clip_ref_public(ClipMachineMemory, l);
#else
	   {
	      ClipVar  *vp1 = _clip_ref_public(ClipMachineMemory, l);

	      frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = vp1;
	   }
#endif
	   break;
	case CLIP_REFMACRO:
	   if ((ret = _clip_refmacro(ClipMachineMemory)))
	      goto _trap;
	   break;
	case CLIP_MAKE_REF:
	   c = GET_BYTE(pp);
	   vp = frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
	   if ((ret = _clip_ref(ClipMachineMemory, vp, c)))
	      goto _trap;
	   break;
	case CLIP_UNREF_ARR:
	   _clip_unref_arr(ClipMachineMemory);
	   break;
	case CLIP_FIELD:
	   l = GET_LONG(pp);
	   l1 = GET_LONG(pp);
	   if ((ret = _clip_field(ClipMachineMemory, l, l1)))
	      goto _trap;
	   break;
	case CLIP_FIELD_POP:
	   l = GET_LONG(pp);
	   l1 = _clip_pop_hash(ClipMachineMemory);
	   if ((ret = _clip_field(ClipMachineMemory, l, l1)))
	      goto _trap;
	   break;
	case CLIP_FIELD_POP_NAME:
	   _clip_pop_fieldhash(ClipMachineMemory, &l1, &l);
	   if ((ret = _clip_field(ClipMachineMemory, l, l1)))
	      goto _trap;
	   break;
	case CLIP_PUSH_AREA:
	   l = GET_LONG(pp);
	   if ((ret = _clip_push_area(ClipMachineMemory, l)))
	      goto _trap;
	   break;
	case CLIP_PUSH_AREA_POP:
	   l = _clip_pop_hash(ClipMachineMemory);
	   if ((ret = _clip_push_area(ClipMachineMemory, l)))
	      goto _trap;
	   break;
	case CLIP_POP_AREA:
	   if ((ret = _clip_pop_area(ClipMachineMemory)))
	      goto _trap;
	   break;
	case CLIP_FUNC:
	   c = GET_BYTE(pp);
	   l = GET_LONG(pp);
	   if ((ret = _clip_func_hash(ClipMachineMemory, l, c, 0, reflocals)))
	      goto _trap;
	   break;
	case CLIP_FUNCR:
	   c = GET_BYTE(pp);
	   l = GET_LONG(pp);
	   if ((ret = _clip_func_hash(ClipMachineMemory, l, c, 1, reflocals)))
	      goto _trap;
	   break;
	case CLIP_PROC:
	   c = GET_BYTE(pp);
	   l = GET_LONG(pp);
	   if ((ret = _clip_proc_hash(ClipMachineMemory, l, c, 0, reflocals)))
	      goto _trap;
	   break;
	case CLIP_PROCR:
	   c = GET_BYTE(pp);
	   l = GET_LONG(pp);
	   if ((ret = _clip_proc_hash(ClipMachineMemory, l, c, 1, reflocals)))
	      goto _trap;
	   break;
	case CLIP_SFUNC:
	   c = GET_BYTE(pp);
	   s = GET_SHORT(pp);
	   bl.file_of_ClipBlock = file;
	   get_func(modbeg, s, &bl.function_of_ClipBlock);
	   if ((ret = _clip_code_func(ClipMachineMemory, &bl, c, 0, reflocals)))
	      goto _trap;
	   break;
	case CLIP_SFUNCR:
	   c = GET_BYTE(pp);
	   s = GET_SHORT(pp);
	   bl.file_of_ClipBlock = file;
	   get_func(modbeg, s, &bl.function_of_ClipBlock);
	   if ((ret = _clip_code_func(ClipMachineMemory, &bl, c, 1, reflocals)))
	      goto _trap;
	   break;
	case CLIP_SPROC:
	   c = GET_BYTE(pp);
	   s = GET_SHORT(pp);
	   bl.file_of_ClipBlock = file;
	   get_func(modbeg, s, &bl.function_of_ClipBlock);
	   if ((ret = _clip_code_proc(ClipMachineMemory, &bl, c, 0, reflocals)))
	      goto _trap;
	   break;
	case CLIP_SPROCR:
	   c = GET_BYTE(pp);
	   s = GET_SHORT(pp);
	   bl.file_of_ClipBlock = file;
	   get_func(modbeg, s, &bl.function_of_ClipBlock);
	   if ((ret = _clip_code_proc(ClipMachineMemory, &bl, c, 1, reflocals)))
	      goto _trap;
	   break;
	case CLIP_ASSIGN:
	   vp = frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
	   if ((ret = _clip_assign(ClipMachineMemory, vp)))
	      goto _trap;
	   break;
	case CLIP_REF_DESTROY:
	   vp = frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
	  /*if (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType != F_MREF_ClipFlags) */
	   _clip_destroy(ClipMachineMemory, vp);
			/*_clip_destroy(ClipMachineMemory, vp);*/
	   break;
	case CLIP_MACROASSIGN:
	   c = GET_BYTE(pp);
	   c1 = GET_BYTE(pp);
	   if ((ret = _clip_macroassign(ClipMachineMemory, c, c1 & 1, c1 & 2)))
	      goto _trap;
	   break;
	case CLIP_REFASSIGN:
	   c = GET_BYTE(pp);
	   vp = frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
	   if ((ret = _clip_refassign(ClipMachineMemory, vp, c)))
	      goto _trap;
	   break;
	case CLIP_UNREF:
	   vp = frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
	   _clip_destroy(ClipMachineMemory, vp);
	   break;
	case CLIP_IASSIGN:
	   vp = frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
	   if ((ret = _clip_iassign(ClipMachineMemory, vp)))
	      goto _trap;
	   break;
	case CLIP_OPASSIGN:
	   c = GET_BYTE(pp);
	   vp = frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
	   if ((ret = _clip_opassign(ClipMachineMemory, vp, c)))
	      goto _trap;
	   break;
	case CLIP_OPIASSIGN:
	   c = GET_BYTE(pp);
	   vp = frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
	   if ((ret = _clip_opiassign(ClipMachineMemory, vp, c)))
	      goto _trap;
	   break;
	case CLIP_ASSIGN_FIELD:
	   l = GET_LONG(pp);
	   l1 = GET_LONG(pp);
	   if ((ret = _clip_assign_field(ClipMachineMemory, l, l1)))
	      goto _trap;
	   break;
	case CLIP_IASSIGN_FIELD:
	   l = GET_LONG(pp);
	   l1 = GET_LONG(pp);
	   if ((ret = _clip_iassign_field(ClipMachineMemory, l, l1)))
	      goto _trap;
	   break;
	case CLIP_ASSIGN_FIELD_POP:
	   l = GET_LONG(pp);
	   l1 = _clip_pop_hash(ClipMachineMemory);
	   if ((ret = _clip_assign_field(ClipMachineMemory, l, l1)))
	      goto _trap;
	   break;
	case CLIP_IASSIGN_FIELD_POP:
	   l = GET_LONG(pp);
	   l1 = _clip_pop_hash(ClipMachineMemory);
	   if ((ret = _clip_iassign_field(ClipMachineMemory, l, l1)))
	      goto _trap;
	   break;
	case CLIP_ASSIGN_PFIELD:
	   l = _clip_pop_hash(ClipMachineMemory);
	   l1 = GET_LONG(pp);
	   if ((ret = _clip_assign_field(ClipMachineMemory, l, l1)))
	      goto _trap;
	   break;
	case CLIP_IASSIGN_PFIELD:
	   l = _clip_pop_hash(ClipMachineMemory);
	   l1 = GET_LONG(pp);
	   if ((ret = _clip_iassign_field(ClipMachineMemory, l, l1)))
	      goto _trap;
	   break;
	case CLIP_ASSIGN_PFIELD_POP:
	   l = _clip_pop_hash(ClipMachineMemory);
	   l1 = _clip_pop_hash(ClipMachineMemory);
	   if ((ret = _clip_assign_field(ClipMachineMemory, l, l1)))
	      goto _trap;
	   break;
	case CLIP_IASSIGN_PFIELD_POP:
	   l = _clip_pop_hash(ClipMachineMemory);
	   l1 = _clip_pop_hash(ClipMachineMemory);
	   if ((ret = _clip_iassign_field(ClipMachineMemory, l, l1)))
	      goto _trap;
	   break;
	case CLIP_FM_ASSIGN:
	   l = GET_LONG(pp);
	   if ((ret = _clip_fm_assign(ClipMachineMemory, l)))
	      goto _trap;
	   break;
	case CLIP_FM_IASSIGN:
	   l = GET_LONG(pp);
	   if ((ret = _clip_fm_iassign(ClipMachineMemory, l)))
	      goto _trap;
	   break;
	case CLIP_ARRAY:
	   l = GET_LONG(pp);
	   _clip_sarray(ClipMachineMemory, l);
	   break;
	case CLIP_DIMARRAY:
	   l = GET_LONG(pp);
	   _clip_dimarray(ClipMachineMemory, l);
	   break;
	case CLIP_NEWARRAY:
	   l = GET_LONG(pp);
	   _clip_vnewarray(ClipMachineMemory, l, (long *) pp);
	   pp += l * sizeof(long);

	   break;
	case CLIP_GET:
	   l = GET_LONG(pp);
	   if ((ret = _clip_get(ClipMachineMemory, l)))
	      goto _trap;
	   break;
	case CLIP_SET:
	   l = GET_LONG(pp);
	   if ((ret = _clip_set(ClipMachineMemory, l)))
	      goto _trap;
	   break;
	case CLIP_RETURN_POP:
	   _clip_return(ClipMachineMemory);
	case CLIP_RETURN:
	   goto _return;
	case CLIP_STORE:
	   c1 = GET_BYTE(pp);
	   c = GET_BYTE(pp);
	   if ((ret = _clip_store(ClipMachineMemory, c, c1)))
	      goto _trap;
	   break;
	case CLIP_ISTORE:
	   c1 = GET_BYTE(pp);
	   c = GET_BYTE(pp);
	   if ((ret = _clip_istore(ClipMachineMemory, c, c1)))
	      goto _trap;
	   break;
	case CLIP_OPSTORE:
	   c = GET_BYTE(pp);
	   c1 = GET_BYTE(pp);
	   if ((ret = _clip_opstore(ClipMachineMemory, c1, c)))
	      goto _trap;
	   break;
	case CLIP_OPISTORE:
	   c = GET_BYTE(pp);
	   c1 = GET_BYTE(pp);
	   if ((ret = _clip_opistore(ClipMachineMemory, c1, c)))
	      goto _trap;
	   break;
	case CLIP_FETCH:
	   c = GET_BYTE(pp);
	   if ((ret = _clip_fetch(ClipMachineMemory, c)))
	      goto _trap;
	   break;
	case CLIP_FETCHREF:
	   c = GET_BYTE(pp);
#if 0
	   frame.sp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = _clip_fetchref(ClipMachineMemory, c);
#else
	   {
	      ClipVar  *vp1 = _clip_fetchref(ClipMachineMemory, c);

	     /*printf("%p\n", vp1); */
	      frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = vp1;
	   }
#endif
	   if (!frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar)
	      goto _trap;
	   break;
	case CLIP_IFETCH:
	   c = GET_BYTE(pp);
	   if ((ret = _clip_ifetch(ClipMachineMemory, c)))
	      goto _trap;
	   break;
	case CLIP_CALL:
	   c = GET_BYTE(pp);
	   l = GET_LONG(pp);
	   if ((ret = _clip_call(ClipMachineMemory, c, l)))
	      goto _trap;
	   break;
	case CLIP_PUSH_CODE:
	   s = GET_SHORT(pp);
	   c = GET_BYTE(pp);
	   c1 = GET_BYTE(pp);

	   vp = NEW(ClipVar);
	   vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = PCODE_type_of_ClipVarType;
	   vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	   vp->ClipType_t_of_ClipVar.count_of_ClipType = 1;
	   vp->ClipCodeVar_c_of_ClipVar.u.ClipBlock_block_of_ClipCodeVar = NEW(ClipBlock);

	   get_func(modbeg, s, &(vp->ClipCodeVar_c_of_ClipVar.u.ClipBlock_block_of_ClipCodeVar->function_of_ClipBlock));
	   vp->ClipCodeVar_c_of_ClipVar.u.ClipBlock_block_of_ClipCodeVar->file_of_ClipBlock = file;

	   if (c)
	    {
	       int       nlocals = c;

	       ClipVarFrame *localvars = (ClipVarFrame *) calloc(1,
								 sizeof(ClipVarFrame) + nlocals * sizeof(ClipVar));

	       localvars->ClipVar_vars_of_ClipVarFrame = (ClipVar *) (localvars + 1);
	       memcpy(localvars->ClipVar_vars_of_ClipVarFrame, ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - nlocals,
		      nlocals * sizeof(ClipVar));
	       localvars->refcount_of_ClipVarFrame = 1;
	       localvars->size_of_ClipVarFrame = nlocals;
	       vp->ClipCodeVar_c_of_ClipVar.ClipVarFrame_uplocals_of_ClipCodeVar = localvars;
	       ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame -= nlocals;
	    }
	   else if (!c1 && reflocals && reflocals->refcount_of_ClipVarFrame)
	    {
	       reflocals->refcount_of_ClipVarFrame++;
	       vp->ClipCodeVar_c_of_ClipVar.ClipVarFrame_uplocals_of_ClipCodeVar = reflocals;
	    }
	   else
	      vp->ClipCodeVar_c_of_ClipVar.ClipVarFrame_uplocals_of_ClipCodeVar = 0;

	   file->refCount_of_ClipFile++;

	   CLEAR_CLIPVAR(frame.ClipVar_sp_of_ClipFrame);
	   frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = PCODE_type_of_ClipVarType;
	   frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR_ClipFlags;
	   frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = vp;

	   frame.ClipVar_sp_of_ClipFrame++;
	   CLIP_CHECK_STACK;

	   break;
	case CLIP_MACRO:
	   if ((ret = _clip_macro(ClipMachineMemory)))
	      goto _trap;
	   break;
	case CLIP_PCOUNT:
	   _clip_pcount(ClipMachineMemory);
	   break;
	case CLIP_PSHIFT:
	   _clip_pshift(ClipMachineMemory);
	   break;
	case CLIP_PARN:
	   if ((ret = _clip_parn(ClipMachineMemory)))
	      goto _trap;
	   break;
	case CLIP_FUNC_NAME:
	   c = GET_BYTE(pp);
	   if ((ret = _clip_func_name(ClipMachineMemory, c)))
	      goto _trap;
	   break;
	case CLIP_INCR:
	   vp = frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
	   if ((ret = _clip_incr(ClipMachineMemory, vp, 1, 0)))
	      goto _trap;
	   break;
	case CLIP_INCR_POST:
	   vp = frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
	   if ((ret = _clip_incr(ClipMachineMemory, vp, 1, 1)))
	      goto _trap;
	   break;
	case CLIP_DECR:
	   vp = frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
	   if ((ret = _clip_incr(ClipMachineMemory, vp, 0, 0)))
	      goto _trap;
	   break;
	case CLIP_DECR_POST:
	   vp = frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
	   if ((ret = _clip_incr(ClipMachineMemory, vp, 0, 1)))
	      goto _trap;
	   break;
	case CLIP_OP:
	   c = GET_BYTE(pp);
	   if ((ret = _clip_op(ClipMachineMemory, c)))
	      goto _trap;
	   break;
	case CLIP_NOT:
	   if ((ret = _clip_not(ClipMachineMemory)))
	      goto _trap;
	   break;
	case CLIP_COND:
	   s = GET_SHORT(pp);
	   if ((ret = _clip_cond(ClipMachineMemory, &i)))
	      goto _trap;
	   if (!i)
	      pp += s;
	   break;
	case CLIP_TCOND:
	   s = GET_SHORT(pp);
	   if ((ret = _clip_tcond(ClipMachineMemory, &i)))
	      goto _trap;
	   if (!i)
	      pp += s;
	   break;
	case CLIP_ITCOND:
	   s = GET_SHORT(pp);
	   if ((ret = _clip_tcond(ClipMachineMemory, &i)))
	      goto _trap;
	   if (i)
	      pp += s;
	   break;
	case CLIP_GOTO:
	   s = GET_SHORT(pp);
	   pp += s;
	   break;
	case CLIP_FORSTEP:
	   s = GET_SHORT(pp);
	   if ((ret = _clip_forstep(ClipMachineMemory, &i)))
	      goto _trap;
	   if (!i)
	      pp += s;
	   break;
	case CLIP_MAP_FIRST:
	   c = GET_BYTE(pp);
	   s = GET_SHORT(pp);
	   if ((ret = _clip_map_first(ClipMachineMemory, c, &i)))
	      goto _trap;
	   if (!i)
	      pp += s;
	   break;
	case CLIP_MAP_NEXT:
	   c = GET_BYTE(pp);
	   s = GET_SHORT(pp);
	   if ((ret = _clip_map_next(ClipMachineMemory, c, &i)))
	      goto _trap;
	   if (!i)
	      pp += s;
	   break;
	case CLIP_MINUS:
	   if ((ret = _clip_minus(ClipMachineMemory)))
	      goto _trap;
	   break;
	case CLIP_RESETTRAP:
	   trap_pp = 0;
	   break;
	case CLIP_SETTRAP:
	   s = GET_SHORT(pp);
	   trap_pp = pp + s;
	   break;
	case CLIP_RECOVER:
	   while (frame.ClipVar_sp_of_ClipFrame > frame.ClipVar_stack_of_ClipFrame)
	      _clip_destroy(ClipMachineMemory, --frame.ClipVar_sp_of_ClipFrame);
	   ret = 0;
	   break;
	case CLIP_USING:
	   if (ClipMachineMemory->trapVar)
	    {
	       vp = frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
	       *frame.ClipVar_sp_of_ClipFrame = *ClipMachineMemory->trapVar;
	       ++frame.ClipVar_sp_of_ClipFrame;
	       free(ClipMachineMemory->trapVar);
	       ClipMachineMemory->trapVar = 0;
	       if ((ret = _clip_assign(ClipMachineMemory, vp)))
		  goto _trap;
	    }
	   break;
	case CLIP_BREAK:
#if 0
			/*_clip_trap_str(ClipMachineMemory, filename, frame.line, "BREAK");*/
#else
	   vp = NEW(ClipVar);
	   _clip_trap_var(ClipMachineMemory, filename, frame.line_of_ClipFrame, vp);
#endif
	   ret = -1;
	   goto _trap;
	case CLIP_BREAK_EXPR:
	   vp = NEW(ClipVar);
	   --frame.ClipVar_sp_of_ClipFrame;
	   *vp = *frame.ClipVar_sp_of_ClipFrame;
	   _clip_trap_var(ClipMachineMemory, filename, frame.line_of_ClipFrame, vp);
	   ret = -1;
	   goto _trap;
	case CLIP_SWAP:
	   c = GET_BYTE(pp);
	   _clip_swap(ClipMachineMemory, c);
	   break;
	case CLIP_PUSH_HASH:
	   l = GET_LONG(pp);
	   frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	   frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	   frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	   frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.len_of_ClipType = 10;
	   frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.dec_of_ClipType = 0;
	   frame.ClipVar_sp_of_ClipFrame->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = l;
	   frame.ClipVar_sp_of_ClipFrame++;
	   CLIP_CHECK_STACK;
	   break;
	case CLIP_CALC_HASH:
	   _clip_calc_hash(ClipMachineMemory);
	   break;
	case CLIP_CALC_HASH2:
	   _clip_calc_hash2(ClipMachineMemory, 1);
	   break;
	case CLIP_PUSH_LOCALE:
	   _clip_push_locale(ClipMachineMemory);
	   break;
	case CLIP_RDDFIELD:
	   s = GET_SHORT(pp);
	   s1 = GET_SHORT(pp);
	   _clip_rddfield(ClipMachineMemory, s, s1);
	   break;
	case CLIP_CATSTR:
	   s = GET_SHORT(pp);
	   _clip_catstr(ClipMachineMemory, s);
	   break;
	case CLIP_QUOT:
	   _clip_quot(ClipMachineMemory);
	   break;
	case CLIP_SWITCH:
	   s = GET_SHORT(pp);	/* label num */
	   l = _clip_pop_shash(ClipMachineMemory);	/* hash */
	   {
	      short     other = GET_SHORT(pp);

	      long     *lp = (long *) pp;

	      short    *jmps = (short *) (pp + s * sizeof(long));

	      int       n = search_long(lp, s, l);

	      if (n < 0)
		 pp += other;
	      else
		 pp += GETSHORT(jmps + n);
	   }
	   break;
	default:
	   _clip_trap_printf(ClipMachineMemory, filename, frame.line_of_ClipFrame, "invalid bytecode %d", code);
	   _clip_call_errblock(ClipMachineMemory, 1);
	   ret = 1;
	   goto _trap;
	}
    }
   goto _return;
 _trap:
   if (trap_pp /*&& ret > 0 */ )
    {
       pp = trap_pp;
       goto cont;
    }
   _clip_trap(ClipMachineMemory, filename, frame.line_of_ClipFrame);
  /*ret = 1; */
 _return:
   if (_clip_profiler)
      _clip_stop_profiler(ClipMachineMemory);
   if (local_locals)
      _clip_destroy_locals(ClipMachineMemory);
   _clip_resume(ClipMachineMemory, nlocals, nreflocals);
#if 0
   _clip_vresume(ClipMachineMemory, nlocals, locals);
#endif
#if 0
   _clip_vresume(ClipMachineMemory, nparams, params);
#endif
#if 0
   if (nreflocals)
      _clip_vresume(ClipMachineMemory, nreflocals, reflocals);
#endif
/*_clip_vremove_privates(ClipMachineMemory, nprivates, privates);*/

#ifdef OS_MINGW
   free(stack);
#endif
   return ret;
}

/* ] run codeblock */
