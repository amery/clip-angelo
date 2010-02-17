/*
	Copyright (C) 2001  ITK
	Authors  : Uri Hnykin <uri@itk.ru>, Paul Lasarev <paul@itk.ru>
	License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/
#include "ci_clip.h"
#include "ci_clipcfg.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <sys/time.h>
#ifdef OS_MINGW
#include <ltdl.h>
#else
#include <dlfcn.h>
#include <netinet/in.h>
#include <sys/resource.h>
#endif
#include <errno.h>

#include "ci_clip.h"
#include "ci_error.ch"
#include "ci_rational.h"
#include "ci_coll.h"
#include "ci_hashcode.h"
#include "../libclipscreen/ci_charset.h"
#include "../libclipscreen/ci_screen.h"
#include "../libcliptask/ci_task.h"

extern char *CLIPROOT;

#define NEW(type) ((type*)calloc(sizeof(type),1))
#define NEWVECT(type,len) ((type*)calloc(sizeof(type),(len)))

#ifndef RTLD_NOW
#define RTLD_NOW DL_LAZY
#endif
#ifndef RTLD_GLOBAL
#define RTLD_GLOBAL DL_LAZY
#endif

#if 0
static void
decount(ClipVar * vp)
{
   switch (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType)
    {
    case F_MPTR:
    case F_MREF:
       vp->ClipRefVar_p_of_ClipVar.vp->ClipType_t_of_ClipVar.count_of_ClipType--;
    default:
    }
}
#endif

int
clip_INIT_CLIPINIT(ClipMachine * ClipMachineMemory)
{
  /*printf("CLIPINIT\n"); */

  /* for colorselect function */
   ClipMachineMemory->colorSelect = 0;
   return 0;
}

static char *
_clip_cygwinroot()
{
#ifdef OS_CYGWIN
  /* need change to WIN32 registry data */
   char     *s = "c:\\cygwin";
#else
   char     *s = "";
#endif
   return s;
}

int
clip_CYGWINROOT(ClipMachine * ClipMachineMemory)
{
   _clip_retc(ClipMachineMemory, _clip_cygwinroot());
   return 0;
}

int
clip_CLIPROOT(ClipMachine * ClipMachineMemory)
{
   char     *ret;

   int       len1, len2;

   char     *s1 = _clip_cygwinroot();

   char     *s2 = getenv("CLIPROOT");

   if (s2 == NULL || *s2 == 0)
      s2 = CLIPROOT;

   len1 = strlen(s1);
   len2 = strlen(s2);
   ret = malloc(len1 + len2 + 1);
   memcpy(ret, s1, len1);
   memcpy(ret + len1, s2, len2);
   ret[len1 + len2] = 0;

   _clip_retcn_m(ClipMachineMemory, ret, len1 + len2);
   return 0;
}

int
clip_CLIP_HOSTCS(ClipMachine * ClipMachineMemory)
{
   _clip_retc(ClipMachineMemory, _clip_hostcs);
   return 0;
}

int
clip_ERRORBLOCK(ClipMachine * ClipMachineMemory)
{
   ClipVar  *vp = _clip_spar(ClipMachineMemory, 1);


   if (ClipMachineMemory->errorblock)
      _clip_clone(ClipMachineMemory, RETPTR(ClipMachineMemory), ClipMachineMemory->errorblock);
   if (vp)
    {
       _clip_delete(ClipMachineMemory, ClipMachineMemory->errorblock);
       ClipMachineMemory->errorblock = (ClipVar *) calloc(1, sizeof(ClipVar));
       _clip_clone(ClipMachineMemory, ClipMachineMemory->errorblock, vp);
    }
   return 0;
}

int
clip_BREAK(ClipMachine * ClipMachineMemory)
{
   ClipVar  *vp = _clip_spar(ClipMachineMemory, 1);

   ClipVar  *np = NEW(ClipVar);


   if (!vp)
    {
       _clip_trap_str(ClipMachineMemory, ClipMachineMemory->fp->filename_of_ClipFrame,
		      ClipMachineMemory->fp->line_of_ClipFrame, "BREAK function requare argument");
       return EG_ARG;
    }

   _clip_clone(ClipMachineMemory, np, vp);
   _clip_trap_var(ClipMachineMemory, ClipMachineMemory->fp->filename_of_ClipFrame,
		  ClipMachineMemory->fp->line_of_ClipFrame, np);

   return -1;
}

int
clip_DATATYPE(ClipMachine * ClipMachineMemory)
{
   int       rc = 0;

   int       t = _clip_parinfo(ClipMachineMemory, 1);


   switch (t)
    {
    case CHARACTER_type_of_ClipVarType:
       rc = 1;
       break;
    case NUMERIC_type_of_ClipVarType:
       rc = 2;
       break;
    case LOGICAL_type_of_ClipVarType:
       rc = 4;
       break;
    case DATE_type_of_ClipVarType:
       rc = 8;
       break;
    case MEMO_type_of_ClipVarType:
       rc = 64;
       break;
    case OBJECT_type_of_ClipVarType:
       rc = 8192;
       break;
    case PCODE_type_of_ClipVarType:
    case CCODE_type_of_ClipVarType:
       rc = 4096;
       break;
    case ARRAY_type_of_ClipVarType:
       rc = 512;
       break;
    case MAP_type_of_ClipVarType:
       rc = 1024;
       break;
    }
   _clip_retni(ClipMachineMemory, rc);
   return 0;
}

int
clip_VALTYPE(ClipMachine * ClipMachineMemory)
{

   _clip_retc(ClipMachineMemory, _clip_type_name(_clip_par(ClipMachineMemory, 1)));
   return 0;
}

int
clip_TYPE(ClipMachine * ClipMachineMemory)
{
   int       len;

   char     *str = _clip_parcl(ClipMachineMemory, 1, &len);

   char     *rc = "UI";

   ClipVar   var;

   int       r;


   if (!str)
    {
       _clip_retc(ClipMachineMemory, "");
       return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "TYPE");
    }
   else
    {
       char     *s, *e;

       for (s = str, e = str + len; s < e; ++s)
	  if (*s == '(' || *s == ')')
	   {
	      _clip_retc(ClipMachineMemory, rc);
	      return 0;
	   }

       var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = UNDEF_type_of_ClipVarType;
       var.ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
       ClipMachineMemory->noerrblock++;
       r = _clip_eval_macro(ClipMachineMemory, str, len, &var);
       ClipMachineMemory->noerrblock--;
       if (r < 0)
	  rc = "UE";
       else if (r > 0)
	  rc = "U";
       else
	  rc = _clip_type_name(&var);
       _clip_destroy(ClipMachineMemory, &var);
    }

   _clip_retc(ClipMachineMemory, rc);
   return 0;
}

int
clip_PROCNAME(ClipMachine * ClipMachineMemory)
{
   int       level = _clip_parni(ClipMachineMemory, 1), l;

   ClipFrame *fp;


   for (fp = ClipMachineMemory->fp, l = 0; fp && l < level; fp = fp->ClipFrame_up_of_ClipFrame, l++)
      ;

   if (!fp)
    {
#if 1
       _clip_retc(ClipMachineMemory, "");
       return 0;
#else
       _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "PROCNAME: invalid call level '%d'", level);
       return 1;
#endif
    }

   _clip_retc(ClipMachineMemory, (char *) fp->procname_of_ClipFrame);

   return 0;
}

int
clip_PROCLINE(ClipMachine * ClipMachineMemory)
{
   int       level = _clip_parni(ClipMachineMemory, 1), l;

   ClipFrame *fp;


   for (fp = ClipMachineMemory->fp, l = 0; fp && l < level; fp = fp->ClipFrame_up_of_ClipFrame, l++)
      ;

   if (!fp)
    {
#if 1
       _clip_retni(ClipMachineMemory, 0);
       return 0;
#else
       _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "PROCNAME: invalid call level '%d'", level);
       return 1;
#endif
    }

   _clip_retni(ClipMachineMemory, fp->line_of_ClipFrame);
   return 0;
}

int
clip_FILENAME(ClipMachine * ClipMachineMemory)
{
   int       level = _clip_parni(ClipMachineMemory, 1), l;

   ClipFrame *fp;


   for (fp = ClipMachineMemory->fp, l = 0; fp && l < level; fp = fp->ClipFrame_up_of_ClipFrame, l++)
      ;

   if (!fp)
    {
#if 1
       _clip_retc(ClipMachineMemory, "");
       return 0;
#else
       _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "FILENAME: invalid call level '%d'", level);
       return 1;
#endif
    }

   _clip_retc(ClipMachineMemory, (char *) fp->filename_of_ClipFrame);

   return 0;
}

/*
   block = compile("{||qout('asdf')}")
 */
int
clip_COMPILE(ClipMachine * ClipMachineMemory)
{
   char     *str;

   int       len, r;

   ClipBlock block;

   ClipVar  *rp, *vp;


   str = _clip_parcl(ClipMachineMemory, 1, &len);

   if (!str)
    {
       _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "COMPILE: non-character parameter");
       return 1;
    }

   r = _clip_compile_Block(ClipMachineMemory, str, len, &block);

   if (r)
    {
       _clip_trap(ClipMachineMemory, __FILE__, __LINE__);
       return r;
    }
   rp = RETPTR(ClipMachineMemory);

   vp = NEW(ClipVar);
   vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = PCODE_type_of_ClipVarType;
   vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   vp->ClipType_t_of_ClipVar.count_of_ClipType = 1;
	vp->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipBlock_block_of_ClipCodeVar = NEW(ClipBlock);
	*vp->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipBlock_block_of_ClipCodeVar = block;

   CLEAR_CLIPVAR(rp);
   rp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = PCODE_type_of_ClipVarType;
   rp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR_ClipFlags;
   rp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = vp;

   return 0;
}

int
BLOCK2STR(ClipMachine * ClipMachineMemory)
{
   ClipVar  *vp = _clip_par(ClipMachineMemory, 1);

   ClipBlock *bp;

   ClipVar  *rp;

   int       l;


   if (!vp || vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != PCODE_type_of_ClipVarType)
    {
       _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "block2str: non-pcode parameter");
       return 1;
    }

bp = vp->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipBlock_block_of_ClipCodeVar;
   rp = RETPTR(ClipMachineMemory);
   rp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   rp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   rp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = l = bp->file_of_ClipBlock->bodySize_of_ClipFile;
   rp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = (char *) malloc(l);
   memcpy(rp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, bp->file_of_ClipBlock->body_of_ClipFile, l);

   return 0;
}

int
clip_LEN(ClipMachine * ClipMachineMemory)
{
   ClipVar  *vp = _clip_par(ClipMachineMemory, 1);

   int       rn = 0, l, d;


   if (_clip_parinfo(ClipMachineMemory, 0) == 0)
    {
       _clip_retndp(ClipMachineMemory, 0, 10, 0);
       return 0;
    }

   vp = _clip_par(ClipMachineMemory, 1);

   switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
    {
    case CHARACTER_type_of_ClipVarType:
       rn = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
       break;
    case ARRAY_type_of_ClipVarType:
       rn = vp->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
       break;
    case MAP_type_of_ClipVarType:
       rn = vp->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar;
       break;
    case NUMERIC_type_of_ClipVarType:
       rn = vp->ClipType_t_of_ClipVar.len_of_ClipType;
       if (_clip_parinfo(ClipMachineMemory, 2) == NUMERIC_type_of_ClipVarType)
	  vp->ClipType_t_of_ClipVar.len_of_ClipType = _clip_parni(ClipMachineMemory, 2);
       break;
    default:
       ;
    }

   l = 10;
   d = 0;
   if (ClipMachineMemory->flags & FIXED_FLAG)
    {
       d = ClipMachineMemory->decimals;
       l += ClipMachineMemory->decimals + 1;
    }
   _clip_retndp(ClipMachineMemory, rn, l, d);
   return 0;
}

int
clip_DEC(ClipMachine * ClipMachineMemory)
{
   ClipVar  *vp = _clip_par(ClipMachineMemory, 1);

   int       rn = 0;


   switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
    {
    case NUMERIC_type_of_ClipVarType:
       rn = vp->ClipType_t_of_ClipVar.dec_of_ClipType;
       break;
    default:
       ;
    }

   _clip_retni(ClipMachineMemory, rn);
   return 0;
}

int
clip_ARRAY(ClipMachine * ClipMachineMemory)
{
   int       i, ndim, r;

   long     *dims;

   ClipVar  *rp;


   rp = RETPTR(ClipMachineMemory);

   ndim = ClipMachineMemory->argc;
#ifdef OS_MINGW
   dims = (long *) malloc(ndim * sizeof(long));
#else
   dims = (long *) alloca(ndim * sizeof(long));
#endif

   for (i = 1; i <= ndim; ++i)
      dims[i - 1] = _clip_parni(ClipMachineMemory, i);

   r = _clip_array(ClipMachineMemory, rp, ndim, dims);

  /*decount(rp); */

#ifdef OS_MINGW
   free(dims);
#endif
   return r;
}

int
clip_MAP(ClipMachine * ClipMachineMemory)
{
   ClipVar  *rp;


   rp = RETPTR(ClipMachineMemory);
   _clip_map(ClipMachineMemory, rp);
  /*decount(rp); */

   return 0;
}

/*  mapkeys(map) --> array of map keys */
int
clip_MAPKEYS(ClipMachine * ClipMachineMemory)
{
   ClipVar  *vp = _clip_par(ClipMachineMemory, 1);

   ClipVar  *rp, *ap;

   int       size, i;


   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != MAP_type_of_ClipVarType)
      return 0;

   rp = RETPTR(ClipMachineMemory);

   CLEAR_CLIPVAR(rp);
   rp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = ARRAY_type_of_ClipVarType;
   rp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR_ClipFlags;
   rp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = ap = (ClipVar *) calloc(sizeof(ClipVar), 1);
   ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = ARRAY_type_of_ClipVarType;
   ap->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   size = ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar = vp->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar;
   ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar = (ClipVar *) calloc(sizeof(ClipVar), size);
   ap->ClipType_t_of_ClipVar.count_of_ClipType = 1;

   for (i = 0; i < size; ++i)
    {
       ClipVar  *vpp = ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i;

       vpp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
       vpp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar =
	(vp->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar + i)->no_of_ClipVarEl;
       vpp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
    }

   return 0;
}

/*
   enable/disable auto-call of map:modify for map modification

   MAPMODIFY(map [, lNewVal]) -> lPrevVal

   modify will be called as:
   modify(map, nFieldHash, newVal) -> valToAssign
 */
int
clip_MAPMODIFY(ClipMachine * ClipMachineMemory)
{
   ClipVar  *vp = _clip_par(ClipMachineMemory, 1);

   int       ret;


   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != MAP_type_of_ClipVarType)
      return 0;

   ret = vp->ClipType_t_of_ClipVar.memo_of_ClipType;
   if (ClipMachineMemory->argc > 1)
      vp->ClipType_t_of_ClipVar.memo_of_ClipType = _clip_parl(ClipMachineMemory, 2);

   _clip_retl(ClipMachineMemory, ret);

   return 0;
}

int
clip_EVAL(ClipMachine * ClipMachineMemory)
{
   ClipVar  *bp = _clip_par(ClipMachineMemory, 1);


   if (!bp
       || (bp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CCODE_type_of_ClipVarType
	   && bp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != PCODE_type_of_ClipVarType))
      return 0;

   return _clip_eval(ClipMachineMemory, bp, ClipMachineMemory->argc - 1,
		     ARGPTR(ClipMachineMemory, 2), RETPTR(ClipMachineMemory));
}

int
clip_EVALA(ClipMachine * ClipMachineMemory)
{
   int       parcount;

   ClipVar  *params;

   ClipVar  *bp = _clip_par(ClipMachineMemory, 1);

   ClipVar  *ap = _clip_par(ClipMachineMemory, 2);


   if (!bp
       || (bp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CCODE_type_of_ClipVarType
	   && bp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != PCODE_type_of_ClipVarType))
      return 0;

   if (!ap || ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType)
    {
       int       r;

       r = _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "EVALA");
       return _clip_call_errblock(ClipMachineMemory, r);
    }

   parcount = ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
   params = ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar;

  //_clip_clip(ClipMachineMemory, funcname, parcount, params, retVal);
   return _clip_eval(ClipMachineMemory, bp, parcount, params, RETPTR(ClipMachineMemory));
}

int
clip_MAPEVAL(ClipMachine * ClipMachineMemory)
{
   ClipVar  *obj = _clip_par(ClipMachineMemory, 1);

   ClipVar  *bp = _clip_par(ClipMachineMemory, 2);

   ClipVar  *sobj;

   int       ret;


   if (!obj || (obj->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != MAP_type_of_ClipVarType))
      return EG_ARG;

   if (!bp
       || (bp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CCODE_type_of_ClipVarType
	   && bp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != PCODE_type_of_ClipVarType
	   && bp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType))
      return EG_ARG;

   sobj = ClipMachineMemory->obj;
   ClipMachineMemory->obj = obj;
   if (bp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
      ret =
       _clip_eval_macro(ClipMachineMemory, bp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf,
			bp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, RETPTR(ClipMachineMemory));
   else
      ret =
       _clip_eval(ClipMachineMemory, bp, ClipMachineMemory->argc - 2, ARGPTR(ClipMachineMemory, 3), RETPTR(ClipMachineMemory));
   ClipMachineMemory->obj = sobj;

   return ret;
}

/* LOAD(pobj-name [, array-for-names]) -> Logical */
int
clip_LOAD(ClipMachine * ClipMachineMemory)
{
   char     *name = _clip_parc(ClipMachineMemory, 1);

   ClipVar  *ap = _clip_par(ClipMachineMemory, 2);

   int       r;

   Coll      names;


   if (!name)
    {
       _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "no name given");
       return 1;
    }
   init_Coll(&names, free, 0);

   r = _clip_load(ClipMachineMemory, name, &names, 0);
   _clip_retl(ClipMachineMemory, !r);

   if (!r && ap && ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
    {
       int       i;

       for (i = 0; i < names.count; i++)
	{
	   ClipVar   v;

	   char     *s = (char *) names.items[i];

	   _clip_var_str(s, strlen(s), &v);
	   _clip_aadd(ClipMachineMemory, ap, &v);
	   _clip_destroy(ClipMachineMemory, &v);
	}
    }

   destroy_Coll(&names);

   return r;
}

/* LOADBLOCK(pobj-name) -> codeblock */
int
clip_LOADBLOCK(ClipMachine * ClipMachineMemory)
{
   char     *name = _clip_parc(ClipMachineMemory, 1);

   int       r;


   if (!name)
    {
       _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "no name given");
       return 1;
    }

   r = _clip_load(ClipMachineMemory, name, 0, RETPTR(ClipMachineMemory));

   return r;
}

/* load system DLL */
int
clip_LOADLIB(ClipMachine * ClipMachineMemory)
{
  /*void *hp; */
   char     *name = _clip_parc(ClipMachineMemory, 1);

   char      buf[256], *e, *s;

   int       r;


   if (!name)
    {
       _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "no name given");
       return 1;
    }

#if 0
   hp = dlopen(name, RTLD_NOW | RTLD_GLOBAL);
   if (!hp)
    {
       _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "ELF '%s' loading problem: %s", name, dlerror());
       return 1;
    }
#else
   snprintf(buf, sizeof(buf), "%s", name);
   e = strrchr(buf, '.');
   s = strrchr(buf, '/');
   if (e && (!s || e > s))
      *e = 0;
   else
      e = buf + strlen(buf);
   strncpy(e, DLLREALSUFF, sizeof(buf) - strlen(buf));

   r = _clip_load(ClipMachineMemory, buf, 0, 0);

#endif
   _clip_retl(ClipMachineMemory, !r);

   return 0;
}

int
clip_EMPTY(ClipMachine * ClipMachineMemory)
{
   ClipVar  *vp = _clip_par(ClipMachineMemory, 1);

   int       r = 1, l;


   if (vp)
    {
       switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
	{
	case CHARACTER_type_of_ClipVarType:
	   if ((l = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf))
	    {
	       char     *s;

	       int       i;

	       for (i = 0, s = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf; i < l; ++i, ++s)
		{
		   switch (*s)
		    {
		    case ' ':
		    case '\t':
		    case '\r':
		    case '\n':
		       continue;
		    }
		   r = 0;
		   break;
		}
	    }
	   break;
	case NUMERIC_type_of_ClipVarType:
	   {
	      double    d = _clip_double(vp);

	      r = (d == 0 ? 1 : 0);
	   }
	   break;
	case DATE_type_of_ClipVarType:
	   r = (vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar == 0 ? 1 : 0);
	   break;
	case LOGICAL_type_of_ClipVarType:
	   r = (vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar ? 0 : 1);
	   break;
	case ARRAY_type_of_ClipVarType:
	   r = (vp->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar == 0 ? 1 : 0);
	   break;
	case MAP_type_of_ClipVarType:
	   r = (vp->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar == 0 ? 1 : 0);
	   break;
	default:
	   r = 0;
	   break;
	case UNDEF_type_of_ClipVarType:
	   r = 1;
	   break;
	}
    }

   _clip_retl(ClipMachineMemory, r);
   return 0;
}

int
clip_AADD(ClipMachine * ClipMachineMemory)
{
   ClipVar  *ap = _clip_par(ClipMachineMemory, 1);

   ClipVar  *vp = _clip_spar(ClipMachineMemory, 2);

   long      no;

   int       c;

   ClipVar  *vp3 = _clip_par(ClipMachineMemory, 3);


   if (!ap || !vp)
      return EG_ARG;

   if (vp3)
      no = _clip_hash(ClipMachineMemory, vp3);
   else
      no = 0;

   if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
    {
       c = ++ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
       ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar =
	(ClipVar *) realloc(ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar, sizeof(ClipVar) * c);
       memset(ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + c - 1, 0, sizeof(ClipVar));
       _clip_clone(ClipMachineMemory, ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + c - 1, vp);
       _clip_clone(ClipMachineMemory, RETPTR(ClipMachineMemory), vp);
    }
   else if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
    {
       if (!no)
	  no = ap->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar;
       _clip_clone(ClipMachineMemory, RETPTR(ClipMachineMemory), ARGPTR(ClipMachineMemory, 1));
       return _clip_madd(ClipMachineMemory, ap, no, vp);
    }
   else
      return EG_ARG;

   return 0;
}

int
clip_ADEL(ClipMachine * ClipMachineMemory)
{
   int       r;

   long      n;

   ClipVar  *ap = _clip_par(ClipMachineMemory, 1);

   ClipVar  *vp = _clip_par(ClipMachineMemory, 2);


   if (!ap || !vp)
      return EG_ARG;

   n = _clip_hash(ClipMachineMemory, vp);

   if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
    {
       long      vect = n - 1;

       r = _clip_adel(ClipMachineMemory, ap, 1, &vect);
       if (r)
	  return r;
    }
   else if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
    {
       r = _clip_mdel(ClipMachineMemory, ap, n);
       if (r)
	  return r;
    }
   else
      return EG_ARG;

   _clip_clone(ClipMachineMemory, RETPTR(ClipMachineMemory), ARGPTR(ClipMachineMemory, 1));

   return 0;
}

int
clip_ASIZE(ClipMachine * ClipMachineMemory)
{
   ClipVar  *ap = _clip_par(ClipMachineMemory, 1);

   int       nl = _clip_parni(ClipMachineMemory, 2);

   int       r;

   ClipVar  *rp;


   if (!ap)
      return 0;

   if (nl < 0)
      nl = 0;

   if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
    {
       long      vect = nl;

       r = _clip_asize(ClipMachineMemory, ap, 1, &vect);
       if (r)
	  return r;
    }

   rp = RETPTR(ClipMachineMemory);
   _clip_clone(ClipMachineMemory, rp, ARGPTR(ClipMachineMemory, 1));

   return 0;
}

int
clip_AINS(ClipMachine * ClipMachineMemory)
{
   ClipVar  *ap = _clip_par(ClipMachineMemory, 1);

   ClipVar  *vp1 = _clip_par(ClipMachineMemory, 2);

   ClipVar  *vp = _clip_spar(ClipMachineMemory, 3);

   int       r;

   long      n;


   if (!ap || !vp1)
      return 0;

   n = _clip_hash(ClipMachineMemory, vp1);

   if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
    {
       long      vect = n - 1;

       r = _clip_ains(ClipMachineMemory, ap, 1, &vect);
       if (r)
	  return r;
       if (vp)
	  _clip_clone(ClipMachineMemory, ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + n - 1, vp);
    }
   else if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
    {
       ClipVar   zero;

       memset(&zero, 0, sizeof(zero));

       if (vp)
	  r = _clip_madd(ClipMachineMemory, ap, n, vp);
       else
	  r = _clip_madd(ClipMachineMemory, ap, n, &zero);
       if (r)
	  return r;
    }
   else
      return EG_ARG;

   _clip_clone(ClipMachineMemory, RETPTR(ClipMachineMemory), ARGPTR(ClipMachineMemory, 1));
   return 0;
}

int
clip_ATAIL(ClipMachine * ClipMachineMemory)
{
   ClipVar  *ap = _clip_par(ClipMachineMemory, 1);

   int       c;


   if (!ap)
      return 0;

   if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType
       && (c = ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar) > 0)
    {
       if (c > 0)
	  _clip_clone(ClipMachineMemory, RETPTR(ClipMachineMemory),
		      ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + c - 1);
    }

   return 0;
}

int
clip_AFILL(ClipMachine * ClipMachineMemory)
{
   ClipVar  *ap = _clip_par(ClipMachineMemory, 1);

   ClipVar  *vp = _clip_spar(ClipMachineMemory, 2);

   int       start = _clip_parni(ClipMachineMemory, 3) - 1;

   int       count = _clip_parni(ClipMachineMemory, 4);

   int       c, i, end, r;

   ClipVar  *retp = RETPTR(ClipMachineMemory), *arrp = ARGPTR(ClipMachineMemory, 1);


   _clip_clone(ClipMachineMemory, retp, arrp);

   if (!ap || !vp || ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType)
      return 0;

   c = ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
   if (start >= c)
      return 0;

   if (start < 0)
      start = 0;
   if (_clip_parinfo(ClipMachineMemory, 0) < 4 || count > (c - start))
      count = c - start;

   for (i = start, end = start + count; i < end; ++i)
    {
       ClipVar  *dp = ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i;

       _clip_destroy(ClipMachineMemory, dp);
       r = _clip_clone(ClipMachineMemory, dp, vp);
       if (r)
	  return r;
    }
   return 0;
}

int
clip_ACOPY(ClipMachine * ClipMachineMemory)
{
   ClipVar  *ap = _clip_par(ClipMachineMemory, 1);

   ClipVar  *dp = _clip_par(ClipMachineMemory, 2);

   ClipVar  *tp = _clip_spar(ClipMachineMemory, 2);

   ClipVar  *rp;

   int       start = _clip_parni(ClipMachineMemory, 3) - 1;

   int       count;

   int       dstart = _clip_parni(ClipMachineMemory, 5) - 1;

   int       c, i, end, r, j;


   if (!ap || !dp)
      return EG_ARG;

   if (ClipMachineMemory->argc < 4)
      count = -1;
   else
      count = _clip_parni(ClipMachineMemory, 4);

   rp = RETPTR(ClipMachineMemory);

   if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType
       && dp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
    {
       c = ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
       if (start >= c)
	  return 0;
       if (start < 0)
	  start = 0;
       if (count < 0 || count > (c - start))
	  count = c - start;
       c = dp->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
       if (dstart >= c)
	  return 0;		/*EG_ARG; */
       if (dstart < 0)
	  dstart = 0;
       if (count > (c - dstart))
	  count = c - dstart;

       for (i = start, j = dstart, end = start + count; i < end; ++i, ++j)
	{
	   ClipVar  *app = ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i;

	   ClipVar  *dpp = dp->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + j;

	   _clip_destroy(ClipMachineMemory, dpp);
	   r = _clip_clone(ClipMachineMemory, dpp, app);
	   if (r)
	      return r;
	}
    }
   else if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType
	    && dp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
    {
       c = ap->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar;
       if (start >= c)
	  return 0;
       if (start < 0)
	  start = 0;
       if (count < 0 || count > (c - start))
	  count = c - start;
       c = dp->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar;
       if (start >= c)
	  return 0;
       if (dstart < 0)
	  dstart = 0;
       if (count > (c - dstart))
	  count = c - dstart;

       for (i = start, j = dstart, end = start + count; i < end; ++i, ++j)
	{
	   ClipVarEl *app = ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar + i;

	   ClipVarEl *dpp = dp->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar + j;

	   _clip_destroy(ClipMachineMemory, &dpp->ClipVar_var_of_ClipVarEl);
	   r = _clip_clone(ClipMachineMemory, &dpp->ClipVar_var_of_ClipVarEl, &app->ClipVar_var_of_ClipVarEl);
	   if (r)
	      return r;
	   dpp->no_of_ClipVarEl = app->no_of_ClipVarEl;
	}
    }

   _clip_clone(ClipMachineMemory, rp, tp);
   return 0;
}

int
clip_ACLONE(ClipMachine * ClipMachineMemory)
{
   ClipVar  *ap = _clip_par(ClipMachineMemory, 1);

   ClipVar  *rp;


   if (!ap || ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType)
      return 0;

   rp = RETPTR(ClipMachineMemory);

   return _clip_dup(ClipMachineMemory, rp, ap);
}

int
clip_AEVAL(ClipMachine * ClipMachineMemory)
{
   ClipVar  *ap = _clip_par(ClipMachineMemory, 1);

   ClipVar  *bp = _clip_par(ClipMachineMemory, 2);

   int       start = _clip_parni(ClipMachineMemory, 3) - 1;

   int       count = _clip_parni(ClipMachineMemory, 4);

   ClipVar  *rp;

   int       c, i, r;


   if (!ap)
      return 0;
   if (!bp
       || (bp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CCODE_type_of_ClipVarType
	   && bp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != PCODE_type_of_ClipVarType))
      return 0;

   if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
    {
       c = ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
       if (start >= c)
	  return 0;
       if (start < 0)
	  start = 0;
       if (count < 0 || count > (c - start) || _clip_parinfo(ClipMachineMemory, 4) == UNDEF_type_of_ClipVarType)
	  count = c - start;

       for (i = start; i < start + count; ++i)
	{
	   ClipVar  *nv = 0, *app = ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i;

	   ClipVar   res, stack[2];

	   unsigned  l = 0;

	   memset(&res, 0, sizeof(ClipVar));
	   memset(stack, 0, sizeof(stack));

	   if (!(app->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MREF_ClipFlags))
	    {
	      //_clip_ref( ClipMachineMemory, app, 0 );
	      //--(ClipMachineMemory->fp->sp);
	       l = 1;
	       nv = NEW(ClipVar);

	       *nv = *app;

	       nv->ClipType_t_of_ClipVar.count_of_ClipType = 2;
	       app->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType =
		F_MREF_ClipFlags /*mptr?F_MPTR_ClipFlags:F_MREF_ClipFlags */ ;
	       app->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = nv;
	       app->ClipType_t_of_ClipVar.field_of_ClipType = 0;
	       app->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar = 0;

	    }
	   stack[0] = *app;
	   stack[1].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	   stack[1].ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	   stack[1].ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = i + 1;

	   r = _clip_eval(ClipMachineMemory, bp, 2, stack, &res);
	   _clip_destroy(ClipMachineMemory, &res);
	   if (l)
	      --(nv->ClipType_t_of_ClipVar.count_of_ClipType);

	   if (r)
	      return r;
	}

       rp = RETPTR(ClipMachineMemory);
       ap = ARGPTR(ClipMachineMemory, 1);
       _clip_clone(ClipMachineMemory, rp, ap);
    }
   else if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
    {
       c = ap->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar;
       if (start >= c)
	  return 0;
       if (start < 0)
	  start = 0;
       if (count < 0 || count > (c - start))
	  count = c - start;

       for (i = 0; i < c; ++i)
	{
	   ClipVar  *nv = 0, *app = &ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[i].ClipVar_var_of_ClipVarEl;

	   long      no = ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[i].no_of_ClipVarEl;

	   ClipVar   res, stack[2];

	   unsigned  l = 0;

	   memset(&res, 0, sizeof(ClipVar));
	   memset(stack, 0, sizeof(stack));

	   if (!(app->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MREF_ClipFlags))
	    {
	      //_clip_ref( ClipMachineMemory, app, 0 );
	      //--(ClipMachineMemory->fp->sp);
	       l = 1;
	       nv = NEW(ClipVar);

	       *nv = *app;

	       nv->ClipType_t_of_ClipVar.count_of_ClipType = 2;
	       app->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MREF_ClipFlags /*mptr?F_MPTR:F_MREF */ ;
	       app->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = nv;
	       app->ClipType_t_of_ClipVar.field_of_ClipType = 0;
	       app->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar = 0;

	    }
	   stack[0] = *app;
	   stack[1].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	   stack[1].ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	   stack[1].ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = no;

	   r = _clip_eval(ClipMachineMemory, bp, 2, stack, &res);
	   _clip_destroy(ClipMachineMemory, &res);
	   if (l)
	      --(nv->ClipType_t_of_ClipVar.count_of_ClipType);
	   if (r)
	      return r;
	}

       rp = RETPTR(ClipMachineMemory);
       ap = ARGPTR(ClipMachineMemory, 1);
       _clip_clone(ClipMachineMemory, rp, ap);
    }

   return 0;
}

int
clip_ASCAN(ClipMachine * ClipMachineMemory)
{
   ClipVar  *ap = _clip_par(ClipMachineMemory, 1);

   ClipVar  *bp = _clip_par(ClipMachineMemory, 2);

   int       start = _clip_parni(ClipMachineMemory, 3) - 1;

   int       count = _clip_parni(ClipMachineMemory, 4);

   int       back = _clip_parl(ClipMachineMemory, 5);

   int       c, i, r, code = 0, no = 0, end;


   if (!ap || ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType || !bp)
    {
#if 0
       return EG_ARG;
#else
       _clip_retni(ClipMachineMemory, 0);
       return 0;
#endif
    }
   if (bp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CCODE_type_of_ClipVarType
       || bp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == PCODE_type_of_ClipVarType)
      code = 1;

   if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
    {
       c = ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
       if (start >= c)
	  goto _ret;
       if (start < 0)
	  start = 0;
       if (count <= 0 || count > (c - start))
	  count = c - start;
       end = start + count;

       if (code)
	{
	   if (back)
	      i = end - 1;
	   else
	      i = start;
	   for (;;)
	    {
	       ClipVar   res, *app;

	       if (back)
		{
		   if (i <= start)
		      break;
		}
	       else
		{
		   if (i >= end)
		      break;
		}

	       app = ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i;
	       memset(&res, 0, sizeof(res));

	       r = _clip_eval(ClipMachineMemory, bp, 1, app, &res);
	      /*
	         if (app->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType != F_MREF)
	         app->ClipRefVar_p_of_ClipVar.vp->ClipType_t_of_ClipVar.count_of_ClipType --;
	       */
	       if (!r && res.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == LOGICAL_type_of_ClipVarType
		   && res.ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
		  no = i + 1;
	       _clip_destroy(ClipMachineMemory, &res);

	       if (r)
		  return r;
	       if (no)
		  break;

	       if (back)
		  i--;
	       else
		  i++;
	    }
	}
       else
	{
	   if (back)
	      i = end - 1;
	   else
	      i = start;
	   for (;;)
	    {
	       ClipVar  *app;

	       int       res = 0;

	       if (back)
		{
		   if (i <= start)
		      break;
		}
	       else
		{
		   if (i >= end)
		      break;
		}

	       app = ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i;

	       r = _clip_cmp(ClipMachineMemory, app, bp, &res, 1);
	       if (r)
		  return r;
	       if (!res)
		{
		   no = i + 1;
		   break;
		}
	       if (back)
		  i--;
	       else
		  i++;
	    }
	}

    }
   else if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
    {
       c = ap->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar;
       if (start >= c)
	  goto _ret;
       if (start < 0)
	  start = 0;
       if (count <= 0 || count > (c - start))
	  count = c - start;
       end = start + count;

       if (code)
	{
	   if (back)
	      i = end - 1;
	   else
	      i = start;
	   for (;;)
	    {
	       ClipVar   res, *app;

	       if (back)
		{
		   if (i <= start)
		      break;
		}
	       else
		{
		   if (i >= end)
		      break;
		}

	       app = &ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[i].ClipVar_var_of_ClipVarEl;
	       memset(&res, 0, sizeof(res));

	       r = _clip_eval(ClipMachineMemory, bp, 1, app, &res);
	       if (!r && res.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == LOGICAL_type_of_ClipVarType
		   && res.ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
		  no = i + 1;
	       _clip_destroy(ClipMachineMemory, &res);
	       if (r)
		  return r;
	       if (no)
		  break;

	       if (back)
		  i--;
	       else
		  i++;
	    }
	}
       else
	{
	   end = start + count;

	   if (code)
	    {
	       if (back)
		  i = end - 1;
	       else
		  i = start;
	       for (;;)
		{
		   ClipVar  *app;

		   int       res = 0;

		   if (back)
		    {
		       if (i <= start)
			  break;
		    }
		   else
		    {
		       if (i >= end)
			  break;
		    }

		   app = &ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[i].ClipVar_var_of_ClipVarEl;

		   r = _clip_cmp(ClipMachineMemory, app, bp, &res, 1);
		   if (r)
		      return r;
		   if (!res)
		    {
		       no = i + 1;
		       break;
		    }

		   if (back)
		      i--;
		   else
		      i++;
		}
	    }
	}
    }

 _ret:
   _clip_retni(ClipMachineMemory, no);

   return 0;
}

typedef int (*q_cmp) (ClipVar * p1, ClipVar * p2, ClipMachine * ClipMachineMemory, void *par);

static void q_sort(ClipVar * a, int n, q_cmp cmp, ClipMachine * ClipMachineMemory, void *par);

static void swap_sort(ClipVar * a, int n, q_cmp cmp, ClipMachine * ClipMachineMemory, void *par);

static int
compare_var(ClipVar * p1, ClipVar * p2, ClipMachine * ClipMachineMemory, void *par)
{
   int       ret = 0;


   if (!par)
    {
       _clip_cmp(ClipMachineMemory, p1, p2, &ret, 1);
    }
   else
    {
       ClipVar   res, stack[2];

       memset(&res, 0, sizeof(ClipVar));
       stack[0] = *p1;
       stack[1] = *p2;
       _clip_eval(ClipMachineMemory, (ClipVar *) par, 2, stack, &res);
       switch (res.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
	{
	case LOGICAL_type_of_ClipVarType:
	   ret = !res.ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
	   break;
	case NUMERIC_type_of_ClipVarType:
	   ret = _clip_double(&res);
	   break;
	default:
	   ;
	}
       _clip_destroy(ClipMachineMemory, &res);
    }
   return ret;
}

int
clip_ASORT(ClipMachine * ClipMachineMemory)
{

   ClipVar  *rp;

   ClipVar  *ap = _clip_par(ClipMachineMemory, 1);

   int       start = _clip_parni(ClipMachineMemory, 2) - 1;

   int       count = _clip_parni(ClipMachineMemory, 3);

   ClipVar  *bp = _clip_par(ClipMachineMemory, 4);

   int       c;


   if (!ap || ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType)
      return 0;
   if (bp && bp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CCODE_type_of_ClipVarType
       && bp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != PCODE_type_of_ClipVarType)
      bp = 0;

   c = ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
   if (start >= c)
      return 0;
   if (start < 0)
      start = 0;
   if (count <= 0 || count > (c - start))
      count = c - start;

   if (bp)
      swap_sort(ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + start, count, compare_var, ClipMachineMemory, bp);
   else
      q_sort(ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + start, count, compare_var, ClipMachineMemory, bp);

   rp = RETPTR(ClipMachineMemory);
   ap = ARGPTR(ClipMachineMemory, 1);
   _clip_clone(ClipMachineMemory, rp, ap);

   return 0;
}

/* [ */
#define q_swap(a,b) { ClipVar t = *(a); *(a)=*(b); *(b)=t; }
static void
swap_sort(ClipVar * a, int n, q_cmp cmp, ClipMachine * ClipMachineMemory, void *par)
{
   ClipVar  *pl, *pm;


   for (pm = a + 1; pm < a + n; pm++)
      for (pl = pm; pl > a && cmp((pl - 1), pl, ClipMachineMemory, par) > 0; pl--)
	 q_swap(pl, pl - 1);
}

/* ] */
/* [ */

/* standard qsort(3) realisation with additional parameters to cmp function */

#define q_min(a,b)	((a)<(b)?(a):(b))

static void
q_vecswap(ClipVar * pi, ClipVar * pj, int i)
{
   while (i > 0)
    {
       ClipVar   t = *pi;

       *pi++ = *pj;
       *pj++ = t;
       i--;
    }
}

static ClipVar *
med3(ClipVar * a, ClipVar * b, ClipVar * c, q_cmp cmp, ClipMachine * ClipMachineMemory, void *par)
{

   return cmp(a, b, ClipMachineMemory, par) < 0 ?
    (cmp(b, c, ClipMachineMemory, par) <
     0 ? b : (cmp(a, c, ClipMachineMemory, par) < 0 ? c : a)) : (cmp(b, c,
								     ClipMachineMemory,
								     par) >
								 0 ? b : (cmp(a, c, ClipMachineMemory, par) < 0 ? a : c));
}

static void
q_sort(ClipVar * a, int n, q_cmp cmp, ClipMachine * ClipMachineMemory, void *par)
{
   ClipVar  *pa, *pb, *pc, *pd, *pl, *pm, *pn;

   int       d, r, swap_cnt;


 loop:
   swap_cnt = 0;
   if (n < 7)
    {
       for (pm = a + 1; pm < a + n; pm++)
	  for (pl = pm; pl > a && cmp((pl - 1), pl, ClipMachineMemory, par) > 0; pl--)
	     q_swap(pl, pl - 1);
       return;
    }
   pm = a + (n / 2);
   if (n > 7)
    {
       pl = a;
       pn = a + (n - 1);
       if (n > 40)
	{
	   d = (n / 8);
	   pl = med3(pl, pl + d, pl + 2 * d, cmp, ClipMachineMemory, par);
	   pm = med3(pm - d, pm, pm + d, cmp, ClipMachineMemory, par);
	   pn = med3(pn - 2 * d, pn - d, pn, cmp, ClipMachineMemory, par);
	}
       pm = med3(pl, pm, pn, cmp, ClipMachineMemory, par);
    }
   q_swap(a, pm);
   pa = pb = a + 1;

   pc = pd = a + (n - 1);
   for (;;)
    {
       while (pb <= pc && (r = cmp(pb, a, ClipMachineMemory, par)) <= 0)
	{
	   if (r == 0)
	    {
	       swap_cnt = 1;
	       q_swap(pa, pb);
	       pa += 1;
	    }
	   pb += 1;
	}
       while (pb <= pc && (r = cmp(pc, a, ClipMachineMemory, par)) >= 0)
	{
	   if (r == 0)
	    {
	       swap_cnt = 1;
	       q_swap(pc, pd);
	       pd -= 1;
	    }
	   pc -= 1;
	}
       if (pb > pc)
	  break;
       q_swap(pb, pc);
       swap_cnt = 1;
       pb += 1;
       pc -= 1;
    }
   if (swap_cnt == 0)
    {				/* Switch to insertion sort */
       for (pm = a + 1; pm < a + n; pm += 1)
	  for (pl = pm; pl > a && cmp((pl - 1), pl, ClipMachineMemory, par) > 0; pl -= 1)
	     q_swap(pl, pl - 1);
       return;
    }

   pn = a + n;
   r = q_min(pa - a, pb - pa);
   q_vecswap(a, pb - r, r);
   r = q_min(pd - pc, pn - pd - 1);
   q_vecswap(pb, pn - r, r);
   if ((r = pb - pa) > 1)
      q_sort(a, r, cmp, ClipMachineMemory, par);
   if ((r = pd - pc) > 1)
    {
      /* Iterate rather than recurse to save stack space */
       a = pn - r;
       n = r;
       goto loop;
    }
}

int
clip___RUN(ClipMachine * ClipMachineMemory)
{
   char     *com = _clip_parc(ClipMachineMemory, 1);

   int       old_cursor = 0;


   if (com == NULL)
    {
       _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "invalid argument");
       _clip_retc(ClipMachineMemory, "");
       return 1;
    }

   if (ClipMachineMemory->fullscreen)
    {
       old_cursor = ClipMachineMemory->screen->cursor;
       restore_tty(ClipMachineMemory->screen_base);	/* restore start mode */
    }

   system(com);

   if (ClipMachineMemory->fullscreen)
      restart_tty(ClipMachineMemory->screen_base);	/* set work mode */
   if (ClipMachineMemory->fullscreen)
    {
       redraw_Screen(ClipMachineMemory->screen, ClipMachineMemory->flags1 & UTF8TERM_FLAG);
       ClipMachineMemory->screen->cursor = !ClipMachineMemory->screen->cursor;
       sync_Screen(ClipMachineMemory->screen, ClipMachineMemory->flags1 & UTF8TERM_FLAG);
       ClipMachineMemory->screen->cursor = old_cursor;
       sync_Screen(ClipMachineMemory->screen, ClipMachineMemory->flags1 & UTF8TERM_FLAG);
    }

   return 0;
}

int
clip_CLONE(ClipMachine * ClipMachineMemory)
{
   ClipVar  *vp = _clip_par(ClipMachineMemory, 1);

   ClipVar  *rp;


   if (!vp)
      return 0;

   rp = RETPTR(ClipMachineMemory);
   _clip_dup(ClipMachineMemory, rp, vp);
   return 0;
}

int
clip_ISFUNCTION(ClipMachine * ClipMachineMemory)
{
   ClipFunction *fp;

   ClipBlock *bp;

   long      hash;

   ClipVar  *vp = _clip_par(ClipMachineMemory, 1);

   hash = _clip_casehash(ClipMachineMemory, vp);

   if (_clip_get_function(ClipMachineMemory, hash, &fp, &bp))
      _clip_retl(ClipMachineMemory, 1);
   else
      _clip_retl(ClipMachineMemory, 0);

   return 0;
}

int
clip_GLOB(ClipMachine * ClipMachineMemory)
{
   char     *str = _clip_parc(ClipMachineMemory, 1);

   char     *pattern = _clip_parc(ClipMachineMemory, 2);

   int       caseflag = _clip_parl(ClipMachineMemory, 3);


   if (!str || !*pattern)
      _clip_retl(ClipMachineMemory, 0);
   else
      _clip_retl(ClipMachineMemory, _clip_glob_match(str, pattern, caseflag) >= 0);

   return 0;
}

static void
put_str(OutBuf * bp, char *str, long len)
{
   putInt32_Buf(bp, htonl(len));
   putBuf_Buf(bp, str, len);
}

static void
put_var(ClipMachine * ClipMachineMemory, ClipVar * vp, OutBuf * bp, Coll * refs)
{
   vp = _clip_vptr(vp);

   if (search_Coll(refs, vp, 0))
    {
       putByte_Buf(bp, UNDEF_type_of_ClipVarType);
       return;
    }

   putByte_Buf(bp, vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType);
   switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
    {
    case UNDEF_type_of_ClipVarType:
       break;
    case NUMERIC_type_of_ClipVarType:
       {
	  putByte_Buf(bp, vp->ClipType_t_of_ClipVar.dec_of_ClipType);
	  putByte_Buf(bp, vp->ClipType_t_of_ClipVar.len_of_ClipType);
	  putByte_Buf(bp, vp->ClipType_t_of_ClipVar.memo_of_ClipType);
	  if (vp->ClipType_t_of_ClipVar.memo_of_ClipType)
	   {
	      char     *buf;

	      if (vp->ClipType_t_of_ClipVar.dec_of_ClipType)
		 buf =
	    rational_toString(vp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, 10,
				    vp->ClipType_t_of_ClipVar.dec_of_ClipType, 0);
	      else
		 buf =
	    rational_toString(vp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, 10,
				    ClipMachineMemory->decimals, 0);
	      put_str(bp, buf, strlen(buf));
	      free(buf);
	   }
	  else
	   {
	      char      buf[48];

	      if (vp->ClipType_t_of_ClipVar.len_of_ClipType < sizeof(buf))
	       {
		  char     *s;

		  snprintf(buf, sizeof(buf), "%*.*f", vp->ClipType_t_of_ClipVar.len_of_ClipType,
			   vp->ClipType_t_of_ClipVar.dec_of_ClipType, vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
		  for (s = buf + strlen(buf); s > buf; --s)
		     if ((*s) == ',')
			(*s) = '.';
	       }
	      else
		 _clip_dtos(vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, buf, sizeof(buf), 0);
	      put_str(bp, buf, strlen(buf));
	   }
       }
       break;
    case CHARACTER_type_of_ClipVarType:
       put_str(bp, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf,
	       vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
       break;
    case LOGICAL_type_of_ClipVarType:
       putByte_Buf(bp, vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar);
       break;
    case DATE_type_of_ClipVarType:
       putInt32_Buf(bp, htonl(vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar));
       break;
    case OBJECT_type_of_ClipVarType:
       vp->ClipObjVar_o_of_ClipVar.rtti_of_ClipObjVar->print_of_ClipObjRtti(ClipMachineMemory,
									    vp->ClipObjVar_o_of_ClipVar.obj_of_ClipObjVar,
									    vp->ClipObjVar_o_of_ClipVar.rtti_of_ClipObjVar,
									    &ClipMachineMemory->buf,
									    &ClipMachineMemory->buflen);
       put_str(bp, ClipMachineMemory->buf, ClipMachineMemory->buflen);
       break;
    case PCODE_type_of_ClipVarType:
    case CCODE_type_of_ClipVarType:
      /*out_any(ClipMachineMemory, "CODE", 4, attr, dev); */
       break;
    case ARRAY_type_of_ClipVarType:
       {
	  int       i;

	  insert_Coll(refs, vp);
	  putInt32_Buf(bp, htonl(vp->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar));
	  for (i = 0; i < vp->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar; ++i)
	     put_var(ClipMachineMemory, vp->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i, bp, refs);
       }
       break;
    case MAP_type_of_ClipVarType:
       {
	  int       i;

	  insert_Coll(refs, vp);
	  putInt32_Buf(bp, htonl(vp->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar));
	  for (i = 0; i < vp->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar; ++i)
	   {
	      putInt32_Buf(bp, htonl(vp->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[i].no_of_ClipVarEl));
	      put_var(ClipMachineMemory,
		      &(vp->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[i].ClipVar_var_of_ClipVarEl), bp, refs);
	   }
       }
       break;
    default:
       ;
    }
}

static int
refcmp(const void *p1, const void *p2)
{
   if (p1 < p2)
      return -1;
   else if (p1 > p2)
      return 1;
   else
      return 0;
}

void
_clip_var2str(ClipMachine * ClipMachineMemory, ClipVar * vp, char **strp, long *lenp, int method)
{
   OutBuf    buf;

   Coll      refs;

   init_Buf(&buf);
   init_Coll(&refs, 0, refcmp);

   put_var(ClipMachineMemory, vp, &buf, &refs);
   switch (method)
    {
    case 1:			/* uuencode */
       _clip_uuencode(buf.buf_of_OutBuf, buf.ptr_of_OutBuf - buf.buf_of_OutBuf, strp, lenp, 1);
       break;
    case 2:			/* compress */
    default:			/* no either uuencode nor compress */
       *strp = buf.buf_of_OutBuf;
       *lenp = buf.ptr_of_OutBuf - buf.buf_of_OutBuf;
       break;
    }

   if (method == 1 /*|| method == 2 */ )
      destroy_Buf(&buf);

   destroy_Coll(&refs);
}

/*
   var2str(any_var) -> cStringRepresentation
 */
int
clip_VAR2STR(ClipMachine * ClipMachineMemory)
{
   ClipVar  *vp = _clip_par(ClipMachineMemory, 1);

   char     *str = 0;

   long      len = 0;


   if (!vp)
    {
       _clip_retc(ClipMachineMemory, "");
       return 0;
    }

   _clip_var2str(ClipMachineMemory, vp, &str, &len, 1);
   _clip_retcn_m(ClipMachineMemory, str, len);

   return 0;
}

static int
get_byte(char **buf, long *buflen, int *resp)
{
   if (*buflen < 1)
      return 0;
   *resp = *((unsigned char *) *buf);

   (*buf)++;
   (*buflen)--;

   return 1;
}

/*
static int
get_long(char **buf, long *buflen, long *resp)
{
	if (*buflen < 4)
		return 0;
	*resp = ntohl(*((long *) *buf));

	(*buf) += 4;
	(*buflen) -= 4;

	return 4;
}
*/

static int
get_int32(char **buf, long *buflen, long *resp)
{
   int       res;

   if (*buflen < 4)
      return 0;

   res = (ntohl(*((int *) *buf)));
   *resp = (long) res;

   (*buf) += sizeof(res);
   (*buflen) -= sizeof(res);

   return 4;
}

static int
get_str(char **buf, long *buflen, char **strp, long *lenp)
{
   long      l;

   if (get_int32(buf, buflen, &l) != 4)
      return 0;
   if (*buflen < l)
      return 0;

   *lenp = l;
   *strp = (char *) calloc(1 /**strp*/ , l + 1);
   memcpy(*strp, *buf, l);
   (*strp)[l] = 0;

   (*buf) += l;
   (*buflen) -= l;

   return l + 4;
}

static int
null_func(ClipMachine * ClipMachineMemory)
{
   return 0;
}

static int
get_var(ClipMachine * ClipMachineMemory, ClipVar * vp, char **str, long *len)
{
   int       type;


   memset(vp, 0, sizeof(ClipVar));
   if (!get_byte(str, len, &type))
      return -1;

   switch (type)
    {
    case UNDEF_type_of_ClipVarType:
       break;
    case NUMERIC_type_of_ClipVarType:
       {
	  int       dec, l, memo;

	  char     *s = 0;

	  long      sl = 0;

	  if (!get_byte(str, len, &dec))
	     return -1;
	  if (!get_byte(str, len, &l))
	     return -1;
	  if (!get_byte(str, len, &memo))
	     return -1;
	  if (!get_str(str, len, &s, &sl))
	     return -1;

	  vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	  vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	  vp->ClipType_t_of_ClipVar.len_of_ClipType = l;
	  vp->ClipType_t_of_ClipVar.dec_of_ClipType = dec;
	  vp->ClipType_t_of_ClipVar.memo_of_ClipType = memo;
	  if (memo)
		  vp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar = rational_fromString(s);
	  else
	   {
	      int       dec;

	      vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = _clip_strtod(s, &dec);;
	   }
	  free(s);
       }
       break;
    case CHARACTER_type_of_ClipVarType:
       {
	  char     *s = 0;

	  long      sl = 0;

	  if (!get_str(str, len, &s, &sl))
	     return -1;
	  vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	  vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;

	 /*
	    vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf = malloc(sl);
	    memcpy(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf,s,sl);
	    free(s);
	  */
	  vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = s;
	  vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = sl;
       }
       break;
    case LOGICAL_type_of_ClipVarType:
       {
	  int       n;

	  if (!get_byte(str, len, &n))
	     return -1;
	  vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
	  vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	  vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = n;
       }
       break;
    case DATE_type_of_ClipVarType:
       {
	  long      n;

	  if (!get_int32(str, len, &n))
	     return -1;
	  vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATE_type_of_ClipVarType;
	  vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	  vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = n;
       }
       break;
    case OBJECT_type_of_ClipVarType:
       {
	  char     *s = 0;

	  long      sl = 0;

	  if (!get_str(str, len, &s, &sl))
	     return -1;
	 /*vp->o.rtti->print(ClipMachineMemory, vp->o.obj, vp->o.rtti, &ClipMachineMemory->buf, &ClipMachineMemory->buflen); */
	  free(s);
       }
       break;
    case PCODE_type_of_ClipVarType:
    case CCODE_type_of_ClipVarType:
       {
	 /*ClipVar *sp = (ClipVar *) calloc(1, sizeof(ClipVar)); */
	 /*
	    vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR;
	    vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CCODE_t;
	    vp->ClipRefVar_p_of_ClipVar.vp = sp;
	  */

	 /*sp->ClipType_t_of_ClipVar.count_of_ClipType = 1; */
	  vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CCODE_type_of_ClipVarType;
	  vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	  vp->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipFunction_function_of_ClipCodeVar = null_func;
       }
       break;
    case ARRAY_type_of_ClipVarType:
       {
	  long      i, size;

	  ClipVar  *ap;

	  if (!get_int32(str, len, &size))
	     return -1;

	  ap = (ClipVar *) calloc(1, sizeof(ClipVar));
	  vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = ARRAY_type_of_ClipVarType;
	  vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR_ClipFlags;
	  vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = ap;
	  ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = ARRAY_type_of_ClipVarType;
	  ap->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	  ap->ClipType_t_of_ClipVar.count_of_ClipType = 1;
	  ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar = (ClipVar *) malloc(sizeof(ClipVar) * size);
	  ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar = size;

	  for (i = 0; i < size; ++i)
	     if (get_var(ClipMachineMemory, ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i, str, len) < 0)
		return -1;
       }
       break;
    case MAP_type_of_ClipVarType:
       {
	  int       i;

	  long      size;

	  ClipVar  *ap;

	  if (!get_int32(str, len, &size))
	     return -1;

	  ap = (ClipVar *) calloc(1, sizeof(ClipVar));
	  vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = MAP_type_of_ClipVarType;
	  vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR_ClipFlags;
	  vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = ap;
	  ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = MAP_type_of_ClipVarType;
	  ap->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	  ap->ClipType_t_of_ClipVar.count_of_ClipType = 1;
	  ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar = (ClipVarEl *) calloc(sizeof(ClipVarEl), size);
	  ap->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar = size;

	  for (i = 0; i < size; ++i)
	   {
	      if (!get_int32(str, len, &(ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[i].no_of_ClipVarEl)))
		 return -1;
	      if (get_var
		  (ClipMachineMemory, &(ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[i].ClipVar_var_of_ClipVarEl),
		   str, len) < 0)
		 return -1;
	   }
       }
       break;
    default:
       ;
    }
   return 0;
}

#define RECOVER_PREFIX "_recover_"
#define RECOVER_PREFIX_LEN (sizeof(RECOVER_PREFIX)-1)

void
_clip_str2var(ClipMachine * ClipMachineMemory, ClipVar * vp, char *str, long len, int method)
{
   char     *buf = 0, *b;

   long      buflen = 0;


   switch (method)
    {
    case 1:			/* uuencode */
       _clip_uudecode(str, len, &buf, &buflen);
       break;
    case 2:			/* compress */
    default:			/* no either uuencode nor compress */
       buf = str;
       buflen = len;
       break;
    }

   b = buf;
   get_var(ClipMachineMemory, vp, &buf, &buflen);

   if (_clip_type(vp) == MAP_type_of_ClipVarType)
    {
       ClipVar  *np;

       char     *s = 0;

       int       l = 0;

       np = _clip_mget(ClipMachineMemory, vp, HASH_CLASSNAME);
       if (!_clip_strFromVar(ClipMachineMemory, np, &s, &l) && l)
	{
	   buflen = l + RECOVER_PREFIX_LEN + 1;
	   b = (char *) realloc(b, buflen);
	   memcpy(b, RECOVER_PREFIX, RECOVER_PREFIX_LEN);
	   memcpy(b + RECOVER_PREFIX_LEN, s, l);
	   b[RECOVER_PREFIX_LEN + l] = 0;

	   _clip_clip(ClipMachineMemory, b, 1, vp, 0);
	}
       free(s);
    }
   if (method == 1 /*|| method == 2 */ )
      free(b);
}

/*
   str2var(cStrRepresentation) -> var
 */
int
clip_STR2VAR(ClipMachine * ClipMachineMemory)
{
   int       len;

   char     *str = _clip_parcl(ClipMachineMemory, 1, &len);


   if (!str)
      return EG_ARG;

   _clip_str2var(ClipMachineMemory, RETPTR(ClipMachineMemory), str, len, 1);

   return 0;
}

/* Single character decode.  */
#define DEC(ch) (((ch) - ' ') & 077)
/* ENC is the basic 1 character encoding function to make a char printing.  */
#define ENC(ch) (uu_std[(ch) & 077])

const char uu_std[64] = {
   '`', '!', '"', '#', '$', '%', '&', '\'',
   '(', ')', '*', '+', ',', '-', '.', '/',
   '0', '1', '2', '3', '4', '5', '6', '7',
   '8', '9', ':', ';', '<', '=', '>', '?',
   '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G',
   'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
   'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
   'X', 'Y', 'Z', '[', '\\', ']', '^', '_'
};

int
_clip_uuencode(char *sstr, long l, char **strp, long *lenp, int without_newline)
{
   OutBuf    buf;

   int       nl, n, ch, ll;

   unsigned char *str = (unsigned char *) sstr;

   unsigned char *p;

   init_Buf(&buf);
   nl = l / 45;
   for (n = 0, p = str; n < l; n += 45)
    {
       int       nn;

       unsigned char c1, c2;

       ll = (l - n < 45) ? (l - n) : 45;
       putByte_Buf(&buf, ENC(ll));

       for (nn = ll; nn > 2; nn -= 3, p += 3)
	{
	   ch = *p >> 2;
	   ch = ENC(ch);
	   putByte_Buf(&buf, ch);
	   ch = ((*p << 4) & 060) | ((p[1] >> 4) & 017);
	   ch = ENC(ch);
	   putByte_Buf(&buf, ch);
	   ch = ((p[1] << 2) & 074) | ((p[2] >> 6) & 03);
	   ch = ENC(ch);
	   putByte_Buf(&buf, ch);
	   ch = p[2] & 077;
	   ch = ENC(ch);
	   putByte_Buf(&buf, ch);
	}

       if (nn)
	{
	   c1 = *p;
	   c2 = (nn == 1) ? 0 : p[1];

	   ch = c1 >> 2;
	   ch = ENC(ch);
	   putByte_Buf(&buf, ch);

	   ch = ((c1 << 4) & 060) | ((c2 >> 4) & 017);
	   ch = ENC(ch);
	   putByte_Buf(&buf, ch);

	   if (nn == 1)
	      ch = ENC('\0');
	   else
	    {
	       ch = (c2 << 2) & 074;
	       ch = ENC(ch);
	    }
	   putByte_Buf(&buf, ch);
	   ch = ENC('\0');
	   putByte_Buf(&buf, ch);
	   if (!without_newline)
	      putByte_Buf(&buf, '\n');
	}
       else if (!without_newline)
	  putByte_Buf(&buf, '\n');
    }

   ch = ENC('\0');
   putByte_Buf(&buf, ch);
   if (!without_newline)
      putByte_Buf(&buf, '\n');

   *lenp = buf.ptr_of_OutBuf - buf.buf_of_OutBuf;
   *strp = (char *) realloc(*strp, *lenp + 1);
   memcpy(*strp, buf.buf_of_OutBuf, *lenp);
   (*strp)[*lenp] = 0;

   destroy_Buf(&buf);
   return 0;
}

int
_clip_uudecode(char *sstr, long l, char **strp, long *lenp)
{
   OutBuf    buf;

   int       n, ch;

   unsigned char *str = (unsigned char *) sstr;

   unsigned char *p, *e;

   init_Buf(&buf);

   for (p = str, e = str + l; p < e;)
    {
       if (*p == '\n')
	{
	   p++;
	   continue;
	}

       n = DEC(*p);
       if (n < 0)
	  return -1;

       for (++p; n > 0; p += 4, n -= 3)
	{

	   if (p >= e)
	      return -1;

	   if (n >= 3)
	    {
	       if (p + 3 >= e)
		  return -1;

	       ch = DEC(p[0]) << 2 | DEC(p[1]) >> 4;
	       putByte_Buf(&buf, ch);
	       ch = DEC(p[1]) << 4 | DEC(p[2]) >> 2;
	       putByte_Buf(&buf, ch);
	       ch = DEC(p[2]) << 6 | DEC(p[3]);
	       putByte_Buf(&buf, ch);
	    }
	   else
	    {
	       if (n >= 1)
		{
		   if (p + 1 >= e)
		      return -1;
		   ch = DEC(p[0]) << 2 | DEC(p[1]) >> 4;
		   putByte_Buf(&buf, ch);
		}
	       if (n >= 2)
		{
		   if (p + 2 >= e)
		      return -1;
		   ch = DEC(p[1]) << 4 | DEC(p[2]) >> 2;
		   putByte_Buf(&buf, ch);
		}
	    }
	}
    }

   *lenp = buf.ptr_of_OutBuf - buf.buf_of_OutBuf;
   *strp = (char *) realloc(*strp, *lenp + 1);
   memcpy(*strp, buf.buf_of_OutBuf, *lenp);
   (*strp)[*lenp] = 0;

   destroy_Buf(&buf);
   return 0;
}

/*
   UUENCODE(cVar [,without_newline]) -> cVar
 */

int
clip_UUENCODE(ClipMachine * ClipMachineMemory)
{
   char     *str, *out = 0;

   long      ol;

   int       l;

   int       without_newline = _clip_parl(ClipMachineMemory, 2);


   str = _clip_parcl(ClipMachineMemory, 1, &l);
   if (!str)
      return EG_ARG;

   _clip_uuencode(str, l, &out, &ol, without_newline);
   _clip_retcn_m(ClipMachineMemory, out, ol);

   return 0;
}

int
clip_UUDECODE(ClipMachine * ClipMachineMemory)
{
   char     *str, *out = 0;

   long      ol;

   int       l;


   str = _clip_parcl(ClipMachineMemory, 1, &l);
   if (!str)
      return EG_ARG;

   if (_clip_uudecode(str, l, &out, &ol))
      return EG_ARG;
   _clip_retcn_m(ClipMachineMemory, out, ol);

   return 0;
}

/*
	ULIMIT(cResource [, nNewValue]) -> nValue

	cResource is a one of:
	"CPU"      CPU time in seconds
	"FSIZE"    Maximum filesize
	"DATA"     max data size
	"STACK"    max stack size
	"CORE"     max core file size
	"RSS"      max resident set size
	"NPROC"    max number of processes
	"NOFILE"   max number of open files
	"MEMLOCK"  max locked-in-memory address space
	"AS"       address space (virtual memory) limit
*/
int
clip_ULIMIT(ClipMachine * ClipMachineMemory)
{

#ifdef OS_MINGW
   return EG_ARG;
#else
   char     *res = _clip_parc(ClipMachineMemory, 1);

   long      newval;

   int       resource;

   int       r;

   struct rlimit rlimit;


   if (!res)
      return EG_ARG;

#ifdef RLIMIT_CPU
   if (!strcasecmp(res, "CPU"))
      resource = RLIMIT_CPU;
#else
   if (0)
      ;
#endif
#ifdef RLIMIT_FSIZE
   else if (!strcasecmp(res, "FSIZE"))
      resource = RLIMIT_FSIZE;
#endif
#ifdef RLIMIT_DATA
   else if (!strcasecmp(res, "DATA"))
      resource = RLIMIT_DATA;
#endif
#ifdef RLIMIT_STACK
   else if (!strcasecmp(res, "STACK"))
      resource = RLIMIT_STACK;
#endif
#ifdef RLIMIT_CORE
   else if (!strcasecmp(res, "CORE"))
      resource = RLIMIT_CORE;
#endif
#ifdef RLIMIT_RSS
   else if (!strcasecmp(res, "RSS"))
      resource = RLIMIT_RSS;
#endif
#ifdef RLIMIT_NPROC
   else if (!strcasecmp(res, "NPROC"))
      resource = RLIMIT_NPROC;
#endif
#ifdef RLIMIT_NOFILE
   else if (!strcasecmp(res, "NOFILE"))
      resource = RLIMIT_NOFILE;
#endif
#ifdef RLIMIT_MEMLOCK
   else if (!strcasecmp(res, "MEMLOCK"))
      resource = RLIMIT_MEMLOCK;
#endif
#ifdef RLIMIT_AS
   else if (!strcasecmp(res, "AS"))
      resource = RLIMIT_AS;
#else
#ifdef RLIMIT_DATA
   else if (!strcasecmp(res, "AS"))
      resource = RLIMIT_DATA;
#endif
#endif
   else
      return EG_ARG;

   if (ClipMachineMemory->argc > 1)
    {
       newval = _clip_parnl(ClipMachineMemory, 2);
       getrlimit(resource, &rlimit);
       if (newval < 0)
	  rlimit.rlim_cur = RLIM_INFINITY;
       else if (rlimit.rlim_max != RLIM_INFINITY && newval > rlimit.rlim_max)
	  newval = rlimit.rlim_max;
       rlimit.rlim_cur = newval;
       setrlimit(resource, &rlimit);
    }

   r = getrlimit(resource, &rlimit);
   if (r)
      _clip_retnl(ClipMachineMemory, -1);
   else
      _clip_retnl(ClipMachineMemory, rlimit.rlim_cur);

   return 0;
#endif
}

int
clip_MEMVARGET(ClipMachine * ClipMachineMemory)
{
   int       l;

   char     *name = _clip_parcl(ClipMachineMemory, 1, &l);

   ClipVar  *var;

   ClipVar  *ret = RETPTR(ClipMachineMemory);


   var = _clip_ref_memvar_noadd(ClipMachineMemory, _clip_casehashbytes(0, name, l));
   if (var)
      _clip_clone(ClipMachineMemory, ret, var);
   return 0;
}

int
clip_MEMVARSET(ClipMachine * ClipMachineMemory)
{
   int       l;

   char     *name = _clip_parcl(ClipMachineMemory, 1, &l);

   ClipVar  *var;

   ClipVar  *data = _clip_par(ClipMachineMemory, 2);


   var = _clip_ref_memvar_noadd(ClipMachineMemory, _clip_casehashbytes(0, name, l));
   _clip_retl(ClipMachineMemory, 0);
   if (var)
    {
       _clip_clone(ClipMachineMemory, var, data);
       _clip_retl(ClipMachineMemory, 1);
    }
   return 0;
}

int
clip_ISPRIVATE(ClipMachine * ClipMachineMemory)
{
   int       l, ret = 0;

   char     *name = _clip_parcl(ClipMachineMemory, 1, &l);


   if (name)
    {
       if (!_clip_is_private(ClipMachineMemory, _clip_casehashbytes(0, name, l)))
	  ret = 1;
    }

   _clip_retl(ClipMachineMemory, ret);
   return 0;
}

int
clip_ISPUBLIC(ClipMachine * ClipMachineMemory)
{
   int       l, ret = 0;

   char     *name = _clip_parcl(ClipMachineMemory, 1, &l);


   if (name)
    {
       if (!_clip_is_public(ClipMachineMemory, _clip_casehashbytes(0, name, l)))
	  ret = 1;
    }

   _clip_retl(ClipMachineMemory, ret);
   return 0;
}

int
clip_ISMEMVAR(ClipMachine * ClipMachineMemory)
{
   int       l, ret = 0;

   char     *name = _clip_parcl(ClipMachineMemory, 1, &l);


   if (name)
    {
       if (!_clip_is_memvar(ClipMachineMemory, _clip_casehashbytes(0, name, l)))
	  ret = 1;

       if (!ret)
	{
	   ret = _clip_parse_name(ClipMachineMemory, name, l, 0, 0, 0, 0, 0, 0);
	   if (ret == 1 || ret == 3)
	      ret = 1;
	   else
	      ret = 0;
	}
    }
   _clip_retl(ClipMachineMemory, ret);
   return 0;
}

int
clip_HOST_CHARSET(ClipMachine * ClipMachineMemory)
{
   _clip_retc(ClipMachineMemory, _clip_hostcs);
   return 0;
}

/* Translate_ToUtf8([source codepage], string) */
int
_clip_translate_toutf8(char *p1, unsigned char *str, int len, char **result)
{
   int       i, j, clen, total_len, wclen, first, len1 = 0;

   unsigned long *wcs, wc;

   char     *res, *bp;

   const unsigned long *wcp;

   cons_CharsetEntry *cs1 = 0;

   if (!p1 || !str)
      return EG_ARG;

   wclen = strlen((const char *) str);
   wcs = calloc(wclen, sizeof(unsigned long));
   if (load_charset_name(p1, &cs1, &len1))
    {
       _clip_logg(2, "translate_charset: cannot load charset file '%s': %s", p1, strerror(errno));
    }
   if (cs1)
    {
       for (i = 0; i < wclen; i++)
	{
	   for (j = 0; j < len1; j++)
	    {
	       cons_CharsetEntry *cp;

	       cp = cs1 + j;
	       if (cp->ch == str[i])
		  wcs[i] = cp->unich;
	    }
	}
       free(cs1);
    }
   else
    {
       free(wcs);
       return -1;
    }

   wcp = wcs;
   total_len = 0;
   for (i = 0; i < wclen; i++)
    {
       wc = *wcp++;
       if (wc < 0x80)
	  total_len += 1;
       else if (wc < 0x800)
	  total_len += 2;
       else if (wc < 0x10000)
	  total_len += 3;
       else if (wc < 0x200000)
	  total_len += 4;
       else if (wc < 0x4000000)
	  total_len += 5;
       else
	  total_len += 6;
    }

   *result = calloc(total_len + 1, sizeof(char));
   res = *result;

   wcp = wcs;
   bp = res;
   for (i = 0; i < wclen; i++)
    {
       wc = *wcp++;

       if (wc < 0x80)
	{
	   first = 0;
	   clen = 1;
	}
       else if (wc < 0x800)
	{
	   first = 0xc0;
	   clen = 2;
	}
       else if (wc < 0x10000)
	{
	   first = 0xe0;
	   clen = 3;
	}
       else if (wc < 0x200000)
	{
	   first = 0xf0;
	   clen = 4;
	}
       else if (wc < 0x4000000)
	{
	   first = 0xf8;
	   clen = 5;
	}
       else
	{
	   first = 0xfc;
	   clen = 6;
	}

       switch (clen)
	{
	case 6:
	   bp[5] = (wc & 0x3f) | 0x80;
	   wc >>= 6;
	case 5:
	   bp[4] = (wc & 0x3f) | 0x80;
	   wc >>= 6;
	case 4:
	   bp[3] = (wc & 0x3f) | 0x80;
	   wc >>= 6;
	case 3:
	   bp[2] = (wc & 0x3f) | 0x80;
	   wc >>= 6;
	case 2:
	   bp[1] = (wc & 0x3f) | 0x80;
	   wc >>= 6;
	case 1:
	   bp[0] = wc | first;
	}

       bp += clen;
    }
   *bp = 0;

   free(wcs);

   return 0;
}

/* Translate_FromUtf8(string, [source codepage]) */
int
_clip_translate_fromutf8(char *p1, char *str, int len, char **result)
{
   int       i, j, mask, clen, len1 = 0;

   unsigned long *wcs, *wcp;

   unsigned char *cp, *end, c;

   char     *res;

   int       n;

   cons_CharsetEntry *cs1 = 0;

   if (!p1 || !str)
      return EG_ARG;

   cp = (unsigned char *) str;
   end = cp + len;
   n = 0;
   wcs = calloc(len + 1, sizeof(unsigned long));
   wcp = wcs;
   while (cp != end)
    {
       mask = 0;
       c = *cp;
       if (c < 0x80)
	{
	   clen = 1;
	   mask = 0x7f;
	}
       else if ((c & 0xe0) == 0xc0)
	{
	   clen = 2;
	   mask = 0x1f;
	}
       else if ((c & 0xf0) == 0xe0)
	{
	   clen = 3;
	   mask = 0x0f;
	}
       else if ((c & 0xf8) == 0xf0)
	{
	   clen = 4;
	   mask = 0x07;
	}
       else if ((c & 0xfc) == 0xf8)
	{
	   clen = 5;
	   mask = 0x03;
	}
       else if ((c & 0xfc) == 0xfc)
	{
	   clen = 6;
	   mask = 0x01;
	}
       else
	{
	   free(wcs);
	   return -1;
	}

       if (cp + clen > end)
	{
	   free(wcs);
	   return -1;
	}

       *wcp = (cp[0] & mask);
       for (i = 1; i < clen; i++)
	{
	   if ((cp[i] & 0xc0) != 0x80)
	    {
	       free(wcs);
	       return -1;
	    }
	   *wcp <<= 6;
	   *wcp |= (cp[i] & 0x3f);
	}

       cp += clen;
       wcp++;
       n++;
    }
   if (cp != end)
    {
       free(wcs);
       return -1;
    }

  /* n is the number of wide chars constructed */

   *result = calloc(n + 1, sizeof(char));
   res = *result;
   res[n] = 0;

   if (load_charset_name(p1, &cs1, &len1))
    {
       _clip_logg(2, "translate_charset: cannot load charset file '%s': %s", p1, strerror(errno));
    }
   wcp = wcs;
   if (cs1)
    {
       for (i = 0; i < n; i++)
	  for (j = 0; j < len1; j++)
	   {
	      cons_CharsetEntry *cp;

	      cp = cs1 + j;
	      if (wcs[i] == cp->unich)
	       {
		  res[i] = cp->ch;
		  break;
	       }
	   }
    }
   else
      strcpy(res, str);
   free(cs1);
   free(wcs);

   return 0;
}

int
clip_TRANSLATE_CHARSET(ClipMachine * ClipMachineMemory)
{
   int       len = 0, r;

   char     *p1 = _clip_parc(ClipMachineMemory, 1);

   char     *p2 = _clip_parc(ClipMachineMemory, 2);

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 3, &len);

   unsigned char *s;


   if (!p1 || !p2 || !str)
      return EG_ARG;

   if (!strcasecmp(p1, p2))
    {
       _clip_retcn(ClipMachineMemory, (char *) str, len);
       return 0;
    }

   if (!strcasecmp(p1, "utf-8"))
    {
       char     *result;

       if (!_clip_translate_fromutf8(p2, (char *) str, len, &result))
	{
	   _clip_retc(ClipMachineMemory, result);
	   free(result);
	}
       else
	  _clip_retc(ClipMachineMemory, (char *) str);
       return 0;
    }

   if (!strcasecmp(p2, "utf-8"))
    {
       char     *result;

       if (!_clip_translate_toutf8(p1, str, len, &result))
	{
	   _clip_retc(ClipMachineMemory, result);
	   free(result);
	}
       else
	  _clip_retc(ClipMachineMemory, (char *) str);
       return 0;
    }

   s = (unsigned char *) malloc(len + 1);
   s[len] = 0;

   if ((r = _clip_translate_charset(p1, p2, str, s, len)))
      return r;

   _clip_retcn_m(ClipMachineMemory, (char *) s, len);

   return 0;
}

int
clip_REFCOUNT(ClipMachine * ClipMachineMemory)
{
   ClipVar  *vp = _clip_par(ClipMachineMemory, 1);


   if (!vp)
      _clip_retni(ClipMachineMemory, 0);
   else
      _clip_retni(ClipMachineMemory, vp->ClipType_t_of_ClipVar.count_of_ClipType);

   return 0;
}

int
clip_HELP(ClipMachine * ClipMachineMemory)
{
   return 0;
}

/*
	loadModuleMsg(cModule, cFilename_mo) -> bResult
*/
int
clip_LOADMODULEMSG(ClipMachine * ClipMachineMemory)
{
   char     *module = _clip_parc(ClipMachineMemory, 1);

   char     *filename = _clip_parc(ClipMachineMemory, 2);

   int       r;


   if (!module || !filename)
      return EG_ARG;

   r = _clip_module_locale(module, filename);

   _clip_retl(ClipMachineMemory, !r);
   return 0;
}

/*
	GETTEXT(cMsg [,cModule])->cTranslated
*/
int
clip_GETTEXT(ClipMachine * ClipMachineMemory)
{
   char     *msgid = _clip_parc(ClipMachineMemory, 1);

   char     *module = _clip_parc(ClipMachineMemory, 2);


   if (msgid && module)
    {
       char     *rp = 0;

       int       l;

       _clip_locale_msg(module, msgid, &rp);
       l = strlen(rp);
       _clip_retcn_m(ClipMachineMemory, rp, l);
    }
   else
      _clip_retc(ClipMachineMemory, msgid);

   return 0;
}

/*
	NGETTEXT(cSingleMsg, cPluralMsg, nNum [,cModule])->cTranslated
*/
int
clip_NGETTEXT(ClipMachine * ClipMachineMemory)
{
   char     *msgid = _clip_parc(ClipMachineMemory, 1);

   char     *msgid_plural = _clip_parc(ClipMachineMemory, 2);

   long      n = _clip_parnl(ClipMachineMemory, 3);

   char     *module = _clip_parc(ClipMachineMemory, 4);


   if (msgid && msgid_plural && module)
    {
       char     *rp = 0;

       int       l;

       _clip_locale_msg_plural(module, msgid, msgid_plural, n, &rp);
       l = strlen(rp);
       _clip_retcn_m(ClipMachineMemory, rp, l);
    }
   else if (msgid && msgid_plural)
    {
       if (n == 1)
	  _clip_retc(ClipMachineMemory, msgid);
       else
	  _clip_retc(ClipMachineMemory, msgid_plural);
    }
   else if (msgid)
      _clip_retc(ClipMachineMemory, msgid);

   return 0;
}

int
clip_DOSPARAM(ClipMachine * ClipMachineMemory)
{
   char     *s;

   int       i, l;


   for (i = 1, l = 0; i < _clip_raw_argc; i++)
      l += strlen(_clip_raw_argv[i]) + 1;

   s = (char *) malloc(l + 1);

   for (i = 1, l = 0; i < _clip_raw_argc; i++)
    {
       char     *p = _clip_raw_argv[i];

       int       l1 = strlen(p);

       memcpy(s + l, p, l1);
       s[l + l1] = ' ';
       l += l1 + 1;
    }

   if (l)
      l--;
   s[l] = 0;

   _clip_retcn_m(ClipMachineMemory, s, l);

   return 0;
}
