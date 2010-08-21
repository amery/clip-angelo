/*
   Copyright (C) 2001  ITK
   Author   : Paul Lasarev <paul@itk.ru>
   License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/
#include <ci_clipcfg.h>
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

#include <ci_clip.h>
#include <ci_clipvm.h>
#include <ci_hash.h>
#include <ci_ar.h>
#include <ci_coll.h>

#ifdef USE_TASKS
#include <../libcliptask/ci_task.h>
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

static void get_static(ClipMachine * ClipMachineMemory, ClipFile * file, ClipVar * statics, char *modbeg, int no, ClipVar ** dest);

static void get_func(char *modbeg, int no, char **fp);

static int run_vm(ClipMachine * ClipMachineMemory, ClipBlock * bp);

static void make_func(struct ClipFile *file, ClipCodeVar * dest, long *hash);

static char *get_proc_name(ClipCodeVar * dest);

/* [ load pcode */

#ifdef FORCEALIGN
#include <clipvm/static_FORCEALIGN_GETSHORT.c>
#include <clipvm/static_FORCEALIGN_GETLONG.c>
#include <clipvm/static_FORCEALIGN_SETLONG.c>
#include <clipvm/static_FORCEALIGN_SETINT.c>
#include <clipvm/static_FORCEALIGN_get_short.c>
#include <clipvm/static_FORCEALIGN_get_long.c>
#include <clipvm/static_FORCEALIGN_get_byte.c>

#else

#define GETSHORT(p) (*(short*)(p))
#define GETLONG(p) (*(long*)(p))
#define SETLONG(ptr,l) (*(long*)(ptr)=(l))
#define SETINT(ptr,l) (*(int*)(ptr)=(l))

#include <clipvm/static_get_short.c>
#include <clipvm/static_get_long.c>
#include <clipvm/static_get_byte.c>

#endif

#define GET_SHORT(pp) get_short(&(pp))
#define GET_LONG(pp) get_long(&(pp))
#define GET_BYTE(pp) get_byte(&(pp))

#include <clipvm/static_get_str.c>
#include <clipvm/static_load_pobj.c>
#include <clipvm/_clip_hash_buckets.c>

static Coll *loaded_dlls = 0;

#include <clipvm/static_load_dll.c>
#include <clipvm/_clip_load_inits.c>
#include <clipvm/_clip_load.c>
#include <clipvm/delete_ClipFile.c>
#include <clipvm/destroy_ClipFile.c>
#include <clipvm/static_make_func.c>
#include <clipvm/static_get_proc_name.c>
#include <clipvm/_clip_first_File.c>
#include <clipvm/_clip_next_File.c>
/* ] load pcode */
/* [ ClipBlock */
#include <clipvm/_clip_init_Block.c>
#include <clipvm/destroy_Block.c>
#include <clipvm/_clip_dup_Block.c>
#if 0
static void
get_name(char *func, char **sp)
{
   short npriv = GETSHORT(F_OFFS(func, 3, 2, 1));

   char *name = F_OFFS(func, 3, 3, 1) + npriv * sizeof(long) + 1;

   *sp = name;
}
#endif

#include <clipvm/static_get_num.c>
#include <clipvm/static_get_static.c>
#include <clipvm/static_get_func.c>
/* ] ClipBlock */
/* [ run codeblock */
#include <clipvm/static_do_code.c>
#include <clipvm/_clip_code_func.c>
#include <clipvm/_clip_code_proc.c>
#include <clipvm/static_search_long.c>
#include <clipvm/static_run_vm.c>
/* ] run codeblock */
