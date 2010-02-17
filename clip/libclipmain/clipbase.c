/*
	Copyright (C) 2001  ITK
	Author   : Paul Lasarev <paul@itk.ru>, rust <rust@itk.ru>
	License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include "ci_rdd.h"
#include "ci_hash.h"
#include "ci_error.ch"
#include "ci_hashcode.h"
#include "ci_rational.h"
#include "ci_six.ch"
#include "ci_dbinfo.ch"
#include "ci_dbstruct.ch"

#define NEW(type) ((type*)calloc(sizeof(type),1))
#define VAR(type,var,ini) type *var=(type*)(ini)

#ifndef HIWORD
#define HIWORD(dw) (unsigned short)((dw)>>16)
#endif
#ifndef LOWORD
#define LOWORD(dw) (unsigned short)((dw))
#endif

#define READLOCK    if((er = wa->rd->vtbl->_rlock(ClipMachineMemory,wa->rd,__PROC__))) goto err
#define WRITELOCK	if((er = wa->rd->vtbl->_wlock(ClipMachineMemory,wa->rd,__PROC__))) goto err
#define UNLOCK		if((er = wa->rd->vtbl->_ulock(ClipMachineMemory,wa->rd,__PROC__))) goto err

static int close_area(ClipMachine * ClipMachineMemory, DBWorkArea * wa, const char *__PROC__);

DBWorkArea *cur_area(ClipMachine * ClipMachineMemory);
DBWorkArea *get_area(ClipMachine * ClipMachineMemory, long area, int any, int *no);

static long A_K_aliasHashes[11];

#define er_open         _clip_gettext("Open error")
#define er_badalias     _clip_gettext("Bad alias")
#define er_nofield      _clip_gettext("No such field")
#define er_notable      _clip_gettext("Workarea not in use")
#define er_noalias      _clip_gettext("No alias")
#define er_noorder      _clip_gettext("No controlling order")
#define er_badforexpr   _clip_gettext("Bad FOR expression")

int
_clip_flushbuffer(ClipMachine * ClipMachineMemory, DBWorkArea * wa, const char *__PROC__)
{
   return rdd_flushbuffer(ClipMachineMemory, wa->rd, __PROC__);
}

static int
_rdd_takevalue(ClipMachine * ClipMachineMemory, DBWorkArea * wa, int fno, long fhash, ClipVar * vv, const char *nm)
{
   ClipVar *vp;
   int r, er;
/* #include "ci_clipmachine.h" */

if (vv->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags)
      {
	 if ((!wa->rd->valid || wa->rd->pending_child_parent) && !(wa->rd->file.rlocked || wa->rd->file.wlocked))
	    {

	       if ((er = wa->rd->vtbl->_rlock(ClipMachineMemory, wa->rd, nm)))
		  return er;
		  r = rdd_takevalue(ClipMachineMemory, wa->rd, fno, vv->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar, nm);
	       if ((er = wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, nm)))
		  return er;
	    }
	 else
	    {
			 r = rdd_takevalue(ClipMachineMemory, wa->rd, fno, vv->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar, nm);
	    }

vv->ClipType_t_of_ClipVar.field_of_ClipType = 1;
	 vv->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar = _clip_get_fielddef(ClipMachineMemory, wa->aliasHash, fhash);
	 return r;
      }

   if ((!wa->rd->valid || wa->rd->pending_child_parent) && !(wa->rd->file.rlocked || wa->rd->file.wlocked))
      {

	 if ((er = wa->rd->vtbl->_rlock(ClipMachineMemory, wa->rd, nm)))
	    return er;
	 r = rdd_takevalue(ClipMachineMemory, wa->rd, fno, vv, nm);
	 if ((er = wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, nm)))
	    return er;
      }
   else
      {
	 r = rdd_takevalue(ClipMachineMemory, wa->rd, fno, vv, nm);
      }
   if (r)
      return r;

   vp = NEW(ClipVar);
   *vp = *vv;
	vp->ClipType_t_of_ClipVar.count_of_ClipType = 1;
   CLEAR_CLIPVAR(vv);
	vv->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MREF_ClipFlags;
	vv->ClipType_t_of_ClipVar.memo_of_ClipType = vp->ClipType_t_of_ClipVar.memo_of_ClipType;
	vv->ClipType_t_of_ClipVar.field_of_ClipType = 1;
	vv->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType;
	vv->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = vp;
	vv->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar = _clip_get_fielddef(ClipMachineMemory, wa->aliasHash, fhash);

   return 0;
}

void
register_dbdriver(ClipMachine * ClipMachineMemory, char *id, char *data, char *idx, char *memo)
{
/* #include "ci_clipmachine.h" */
   if (!*ClipMachineMemory->dbdrivers)
      *ClipMachineMemory->ndbdrivers = 0;
   if (!*ClipMachineMemory->ndbdrivers)
      *ClipMachineMemory->dbdrivers = (DBDriver *) malloc(sizeof(DBDriver));
   else
      *ClipMachineMemory->dbdrivers = (DBDriver *) realloc(*ClipMachineMemory->dbdrivers, sizeof(DBDriver) * (*ClipMachineMemory->ndbdrivers + 1));
   (*ClipMachineMemory->ndbdrivers)++;
	strcpy((*ClipMachineMemory->dbdrivers)[*ClipMachineMemory->ndbdrivers - 1].id_of_DBDriver, id);
	strcpy((*ClipMachineMemory->dbdrivers)[*ClipMachineMemory->ndbdrivers - 1].data_of_DBDriver, data);
	strcpy((*ClipMachineMemory->dbdrivers)[*ClipMachineMemory->ndbdrivers - 1].idx_of_DBDriver, idx);
	strcpy((*ClipMachineMemory->dbdrivers)[*ClipMachineMemory->ndbdrivers - 1].memo_of_DBDriver, memo);
}

int
db_driver(ClipMachine * ClipMachineMemory, const char *driver, char *data, char *idx, char *memo, const char *__PROC__)
{
   int i;
   char err[256];
/* #include "ci_clipmachine.h" */

   if (!driver || !driver[0])
      driver = ClipMachineMemory->def_db_driver;
   for (i = 0; i < *ClipMachineMemory->ndbdrivers; i++)
      {
			if ((strlen(driver) == strlen((*ClipMachineMemory->dbdrivers)[i].id_of_DBDriver)) && (strncasecmp((*ClipMachineMemory->dbdrivers)[i].id_of_DBDriver, driver,
				sizeof((*ClipMachineMemory->dbdrivers)[i].id_of_DBDriver)) == 0))
	    {
	       if (data)
				 strcpy(data, (*ClipMachineMemory->dbdrivers)[i].data_of_DBDriver);
	       if (idx)
				 strcpy(idx, (*ClipMachineMemory->dbdrivers)[i].idx_of_DBDriver);
	       if (memo)
				 strcpy(memo, (*ClipMachineMemory->dbdrivers)[i].memo_of_DBDriver);
	       return 0;
	    }
      }
   snprintf(err, sizeof(err), _clip_gettext("No driver linked: %s"), driver);
   return rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, err);
}

int
clip_INIT_DBFNTX(ClipMachine * ClipMachineMemory)
{
   char a[2];
   int i;
/* #include "ci_clipmachine.h" */

   a[1] = 0;
   for (i = 0; i < 11; i++)
      {
	 a[0] = 'A' + i;
	 A_K_aliasHashes[i] = _clip_hashstr(a);
      }
   register_dbdriver(ClipMachineMemory, "DBFNTX", "DBF", "NTX", "DBT");
   return 0;
}

int
clip_INIT_DBFCTX(ClipMachine * ClipMachineMemory)
{
/* #include "ci_clipmachine.h" */
   register_dbdriver(ClipMachineMemory, "DBFCTX", "DBF", "CTX", "DBT");
   return 0;
}

int
clip_INIT_DBFCDX(ClipMachine * ClipMachineMemory)
{
/* #include "ci_clipmachine.h" */
   register_dbdriver(ClipMachineMemory, "DBFCDX", "FOX", "CDX", "FPT");
   return 0;
}

int
clip_INIT_SIXCDX(ClipMachine * ClipMachineMemory)
{
/* #include "ci_clipmachine.h" */
   register_dbdriver(ClipMachineMemory, "SIXCDX", "FOX", "CDX", "FPT");
   return 0;
}

int
clip_INIT_VFPCDX(ClipMachine * ClipMachineMemory)
{
/* #include "ci_clipmachine.h" */
   register_dbdriver(ClipMachineMemory, "VFPCDX", "VFP", "CDX", "FPT");
   return 0;
}

int
clip_INIT_DBFIDX(ClipMachine * ClipMachineMemory)
{
/* #include "ci_clipmachine.h" */
   register_dbdriver(ClipMachineMemory, "DBFIDX", "FOX", "IDX", "FPT");
   return 0;
}

int
clip_INIT_DBFCDX64(ClipMachine * ClipMachineMemory)
{
/* #include "ci_clipmachine.h" */
   register_dbdriver(ClipMachineMemory, "DBFCDX64", "DBF64", "CDX", "FPT");
   return 0;
}

int
clip_DBFNTX(ClipMachine * ClipMachineMemory)
{
/* #include "ci_clipmachine.h" */
   return 0;
}

int
clip_DBFCTX(ClipMachine * ClipMachineMemory)
{
/* #include "ci_clipmachine.h" */
   return 0;
}

int
clip_DBFCDX(ClipMachine * ClipMachineMemory)
{
/* #include "ci_clipmachine.h" */
   return 0;
}

int
clip__DBFCDX(ClipMachine * ClipMachineMemory)
{
/* #include "ci_clipmachine.h" */
   return 0;
}

int
clip_SIXCDX(ClipMachine * ClipMachineMemory)
{
/* #include "ci_clipmachine.h" */
   return 0;
}

int
clip_VFPCDX(ClipMachine * ClipMachineMemory)
{
/* #include "ci_clipmachine.h" */
   return 0;
}

int
clip_DBFIDX(ClipMachine * ClipMachineMemory)
{
/* #include "ci_clipmachine.h" */
   return 0;
}

int
clip_DBFCDX64(ClipMachine * ClipMachineMemory)
{
/* #include "ci_clipmachine.h" */
   return 0;
}

/* [ fields */
#if 0
static ClipVar *
space_var(ClipMachine * ClipMachineMemory, int line, long area, long hash)
{
   HashTable *hp = _clip_space(ClipMachineMemory, area);
   ClipVar *vp;

   if (!hp)
      {
	 _clip_trap_printf(ClipMachineMemory, __FILE__, line, "no namespace %lx", area);
	 return 0;
      }

   vp = _clip_memvar_space(ClipMachineMemory, hp, hash, 1);
   if (!vp)
      {
	 _clip_trap_printf(ClipMachineMemory, __FILE__, line, "no namespace var %lx", hash);
	 return 0;
      }

   return vp;
}
#endif

CLIP_DLLEXPORT int
_clip_field(ClipMachine * ClipMachineMemory, long hash, long area)
{
   ClipVar *sp = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame;
   DBWorkArea *wa;
   const char nm[] = "_clip_field";
   char buf[11];
   int no;
/* #include "ci_clipmachine.h" */

   switch (area)
      {
      case HASH_m:
      case HASH_memv:
      case HASH_memva:
      case HASH_memvar:
	 return _clip_ref(ClipMachineMemory, _clip_ref_memvar(ClipMachineMemory, hash), 0);
      default:
	 break;
      }

   wa = get_area(ClipMachineMemory, area, 0, 0);
   if (!wa)
      {
	 ClipVar *ap = _clip_ref_memvar_noadd(ClipMachineMemory, area);
	 if (ap)
	    {
	       ClipVar *vp = _clip_mget(ClipMachineMemory, ap, hash);
	       if (vp)
		  {
		     _clip_push(ClipMachineMemory, vp);
		     return 0;
		  }
	    }

	 _clip_hash_name(ClipMachineMemory, hash, buf, sizeof(buf));
	 return rdd_err(ClipMachineMemory, EG_NOVAR, 0, __FILE__, __LINE__, buf, er_nofield);
      }
   if ((no = _rdd_fieldno(wa->rd, hash)) == -1)
      {
	 _clip_hash_name(ClipMachineMemory, hash, buf, sizeof(buf));
	 return rdd_err(ClipMachineMemory, EG_NOVAR, 0, __FILE__, __LINE__, buf, er_nofield);
      }
	   sp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = UNDEF_type_of_ClipVarType;
		sp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   ++(ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame);

   return _rdd_takevalue(ClipMachineMemory, wa, no, hash, sp, nm);
}

CLIP_DLLEXPORT int
_clip_try_field(ClipMachine * ClipMachineMemory, long hash)
{
   ClipVar *sp = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame;
   DBWorkArea *wa;
   const char nm[] = "_clip_try_field";
   int no;
/* #include "ci_clipmachine.h" */

   if (!(wa = cur_area(ClipMachineMemory)))
      return 1;

   if ((no = _rdd_fieldno(wa->rd, hash)) == -1)
      return 1;

	sp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = UNDEF_type_of_ClipVarType;
	sp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   ++(ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame);
   if (_rdd_takevalue(ClipMachineMemory, wa, no, hash, sp, nm))
      return 1;

   return 0;
}

CLIP_DLLEXPORT int
_clip_take_field(ClipMachine * ClipMachineMemory, long hash, long area, ClipVar * dest)
{
   DBWorkArea *wa;
   const char nm[] = "_clip_take_field";
   int fno, er;
/* #include "ci_clipmachine.h" */

   if (!(wa = get_area(ClipMachineMemory, area, 0, 0)))
      return 1;

   if ((fno = _rdd_fieldno(wa->rd, hash)) == -1)
      return 1;

	dest->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = UNDEF_type_of_ClipVarType;
	dest->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;

   if ((er = _rdd_takevalue(ClipMachineMemory, wa, fno, hash, dest, nm)))
      return er;
   return 0;
}

CLIP_DLLEXPORT int
_clip_fm_assign(ClipMachine * ClipMachineMemory, long hash)
{
   ClipVar *sp = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - 1;
   int r = _clip_fm_iassign(ClipMachineMemory, hash);
/* #include "ci_clipmachine.h" */

   _clip_destroy(ClipMachineMemory, sp);
   --ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame;
   return r;
}

CLIP_DLLEXPORT int
_clip_fm_iassign(ClipMachine * ClipMachineMemory, long hash)
{
   const char nm[] = "_clip_fm_iassign";
   int r = 0;
   ClipVar *sp = _clip_vptr(ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - 1);
   DBWorkArea *wa;
   int fno;
/* #include "ci_clipmachine.h" */
   if ((wa = cur_area(ClipMachineMemory)) && (fno = _rdd_fieldno(wa->rd, hash)) != -1)
      {
	 r = rdd_setvalue(ClipMachineMemory, wa->rd, fno, sp, nm);
      }
   else
      {
	 r = _clip_iassign(ClipMachineMemory, _clip_ref_memvar(ClipMachineMemory, hash));
      }
   return r;
}

CLIP_DLLEXPORT int
_clip_assign_field(ClipMachine * ClipMachineMemory, long hash, long area)
{
   ClipVar *sp = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - 1;
   int r = _clip_iassign_field(ClipMachineMemory, hash, area);
/* #include "ci_clipmachine.h" */

   _clip_destroy(ClipMachineMemory, sp);
   --ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame;
   return r;
}

CLIP_DLLEXPORT int
_clip_iassign_field(ClipMachine * ClipMachineMemory, long hash, long area)
{
   ClipVar *sp = _clip_vptr(ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - 1);
   DBWorkArea *wa;
   const char nm[] = "_clip_iassign_field";
   int fno, er;
   char buf[11];
/* #include "ci_clipmachine.h" */

   if (!(wa = get_area(ClipMachineMemory, area, 0, 0)))
      {
	 if (area < 0)
	    {
	       return rdd_err(ClipMachineMemory, EG_BADALIAS, 0, __FILE__, __LINE__, nm, er_badalias);
	    }
	 else
	    {
#if 1
	       ClipVar *ap = _clip_ref_memvar_noadd(ClipMachineMemory, area);
	       if (ap)
		  {
		     ClipVar *vp = _clip_mget(ClipMachineMemory, ap, hash);
		     if (vp)
			{
			   return _clip_iassign(ClipMachineMemory, vp);
			}
		  }
	       return rdd_err(ClipMachineMemory, EG_BADALIAS, 0, __FILE__, __LINE__, nm, er_badalias);
#else
	       ClipVar *vp = space_var(ClipMachineMemory, __LINE__, area, hash);

	       if (!vp)
		  return 1;
	       else
		  return _clip_iassign(ClipMachineMemory, vp);
#endif
	    }
      }

   if ((fno = _rdd_fieldno(wa->rd, hash)) == -1)
      {
	 _clip_hash_name(ClipMachineMemory, hash, buf, sizeof(buf));
	 return rdd_err(ClipMachineMemory, EG_NOVAR, 0, __FILE__, __LINE__, buf, er_nofield);
      }
   if ((!wa->rd->valid || wa->rd->pending_child_parent) && !(wa->rd->file.rlocked || wa->rd->file.wlocked))
      {

	 if ((er = wa->rd->vtbl->_rlock(ClipMachineMemory, wa->rd, nm)))
	    return er;
	 if ((er = rdd_setvalue(ClipMachineMemory, wa->rd, fno, sp, nm)))
	    return er;
	 if ((er = wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, nm)))
	    return er;
      }
   else
      {
	 if ((er = rdd_setvalue(ClipMachineMemory, wa->rd, fno, sp, nm)))
	    return er;
      }
   return 0;
}

CLIP_DLLEXPORT int
_clip_push_area(ClipMachine * ClipMachineMemory, long area)
{
   DBWorkArea *wa;
   const char nm[] = "_clip_push_area";
   int no;
/* #include "ci_clipmachine.h" */

   add_ClipVect((ClipMachineMemory->areaStack), (void *) (ClipMachineMemory->curArea));

   if (area > 0 && area <= 256)
      {
	 ClipMachineMemory->curArea = area - 1;
	 if (area > ClipMachineMemory->areas->count_of_ClipVect)
	    {
	       area -= ClipMachineMemory->areas->count_of_ClipVect;
	       while (area--)
		  add_ClipVect(ClipMachineMemory->areas, NULL);
	    }
      }
   else
      {
	 if (!(wa = get_area(ClipMachineMemory, area, 0, &no)) && area)
	    return rdd_err(ClipMachineMemory, EG_BADALIAS, 0, __FILE__, __LINE__, nm, er_badalias);
	 ClipMachineMemory->curArea = no;
      }

   return 0;
}

CLIP_DLLEXPORT int
_clip_pop_area(ClipMachine * ClipMachineMemory)
{
   int no = ClipMachineMemory->areaStack->count_of_ClipVect;
/* #include "ci_clipmachine.h" */

   if (no < 1)
      return rdd_err(ClipMachineMemory, EG_NOALIAS, 0, __FILE__, __LINE__, "_clip_pop_area", er_noalias);

   ClipMachineMemory->areaStack->count_of_ClipVect--;
   ClipMachineMemory->curArea = (long) ClipMachineMemory->areaStack->items_of_ClipVect[no - 1];
   return 0;
}

CLIP_DLLEXPORT int
_clip_set_area(ClipMachine * ClipMachineMemory, long area)
{
   DBWorkArea *wa;
   const char nm[] = "_clip_set_area";
   int no;
/* #include "ci_clipmachine.h" */

   if (!(wa = get_area(ClipMachineMemory, area, 0, &no)) && area)
      return rdd_err(ClipMachineMemory, EG_BADALIAS, 0, __FILE__, __LINE__, nm, er_badalias);

   ClipMachineMemory->curArea = no;
   return 0;
}

CLIP_DLLEXPORT void
_clip_close_all(ClipMachine * ClipMachineMemory)
{
   int i;
/* #include "ci_clipmachine.h" */

   for (i = 0; i < ClipMachineMemory->areas->count_of_ClipVect; i++)
      close_area(ClipMachineMemory, (DBWorkArea *) ClipMachineMemory->areas->items_of_ClipVect[i], "DBCLOSEALL");
}

/* ] fields */
/* [ work area */
CLIP_DLLEXPORT int
_clip_register_driver(ClipMachine * ClipMachineMemory, struct DBFuncTable *vtbl)
{
/* #include "ci_clipmachine.h" */
   return 0;
}

CLIP_DLLEXPORT struct DBFuncTable *
_clip_get_driver(ClipMachine * ClipMachineMemory, const char *driver)
{
/* #include "ci_clipmachine.h" */
   return NULL;
}

DBWorkArea *
cur_area(ClipMachine * ClipMachineMemory)
{
   DBWorkArea *wa = 0;
/* #include "ci_clipmachine.h" */

   if (!ClipMachineMemory->areas->count_of_ClipVect)
      return NULL;
   wa = (DBWorkArea *) ClipMachineMemory->areas->items_of_ClipVect[ClipMachineMemory->curArea];
   if (wa && !wa->used)
      return NULL;
   return wa;
}

DBWorkArea *
get_area(ClipMachine * ClipMachineMemory, long area, int any, int *no)
{
   DBWorkArea *wa = NULL;
   int n;
/* #include "ci_clipmachine.h" */

   if (no)
      *no = 0;

   if (!ClipMachineMemory->areas->count_of_ClipVect)
      return 0;

   if (area == 0)
      {
	 for (n = 0; n < ClipMachineMemory->areas->count_of_ClipVect; n++)
	    {
	       DBWorkArea *wa;

	       wa = (DBWorkArea *) ClipMachineMemory->areas->items_of_ClipVect[n];
	       if (wa == NULL || !wa->used)
		  break;
	    }
	 if (n == ClipMachineMemory->areas->count_of_ClipVect)
	    add_ClipVect(ClipMachineMemory->areas, NULL);
	 if (no)
	    *no = n;
	 return ClipMachineMemory->areas->items_of_ClipVect[n];
      }
   else if (area == -1)
      wa = (DBWorkArea *) ClipMachineMemory->areas->items_of_ClipVect[ClipMachineMemory->curArea];
   else
      {
	 wa = (DBWorkArea *) HashTable_fetch(ClipMachineMemory->aliases, area);
	 if (wa == NULL)
	    {
	       char buf[11];
	       long tmp;
	       _clip_hash_name(ClipMachineMemory, area, buf, sizeof(buf));
	       buf[10] = 0;
	       tmp = _clip_hashstr(buf);
	       wa = (DBWorkArea *) HashTable_fetch(ClipMachineMemory->aliases, tmp);
	       if (wa)
		  area = tmp;
	    }

	 if (!wa)
	    {
	       area -= 1;
	       if (area >= 0 && area < ClipMachineMemory->areas->count_of_ClipVect)
		  wa = (DBWorkArea *) ClipMachineMemory->areas->items_of_ClipVect[area];
	       else
		  {
		     int i, l;

		     area += 1;
		     l = min(11, ClipMachineMemory->areas->count_of_ClipVect);
		     for (i = 0; i < l; i++)
			{
			   if (A_K_aliasHashes[i] == area)
			      {
				 wa = (DBWorkArea *) ClipMachineMemory->areas->items_of_ClipVect[i];
				 break;
			      }
			}
		  }
	    }
      }

   if (!wa || (!wa->used && !any))
      return 0;

   if (no)
      *no = wa->no;
   return wa;
}

static int
close_area(ClipMachine * ClipMachineMemory, DBWorkArea * wa, const char *__PROC__)
{
   int er = 0;
/* #include "ci_clipmachine.h" */

   if (wa)
      {
	 if (wa->rd)
	    {
	       int re;
	       if ((er = rdd_event(ClipMachineMemory, EVENT_PRECLOSE, wa->rd->area, 0, NULL, &re, __PROC__)))
		  return er;
	       if (!re)
		  return 0;

	       if ((er = rdd_clearrelation(ClipMachineMemory, wa->rd, __PROC__)))
		  return er;
	       if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
		  return er;
	       if (wa->rd->readonly)
		  wa->rd->vtbl->_rlock(ClipMachineMemory, wa->rd, __PROC__);
	       else
		  wa->rd->vtbl->_wlock(ClipMachineMemory, wa->rd, __PROC__);
	       if ((er = rdd_closearea(ClipMachineMemory, wa->rd, __PROC__)))
		  return er;
	       if ((er = rdd_event(ClipMachineMemory, EVENT_POSTCLOSE, wa->rd->area, 0, NULL, &re, __PROC__)))
		  return er;
	       wa->rd = NULL;
	    }
	 _clip_destroy_c_item(ClipMachineMemory, wa->rd_item, _C_ITEM_TYPE_RDD);
	 HashTable_remove(ClipMachineMemory->aliases, wa->aliasHash);
	 ClipMachineMemory->areas->items_of_ClipVect[wa->no] = NULL;
	 if (wa->trigger)
	    free(wa->trigger);
	 free(wa->name);
	 free(wa->alias);
	 free(wa);
      }
   return 0;
}

void
_clip_set_deleted(ClipMachine * ClipMachineMemory, int value)
{
/* #include "ci_clipmachine.h" */
}

int
_clip_relation_chields(ClipMachine * ClipMachineMemory, DBWorkArea * wa)
{
/* #include "ci_clipmachine.h" */
   return 0;
}

/* ] work area */
/* [ user interface */

int
clip_DBSETDRIVER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBSETDRIVER";
   const char *driver = _clip_parc(ClipMachineMemory, 1);
   int er;
/* #include "ci_clipmachine.h" */

   CHECKOPT1(1, CHARACTER_type_of_ClipVarType);

   _clip_retc(ClipMachineMemory, ClipMachineMemory->def_db_driver);

   if (driver && driver[0])
      {
	 int i;
	 for (i = 0; i < *ClipMachineMemory->ndbdrivers; i++)
	    {
			 if (strncasecmp(driver, (*ClipMachineMemory->dbdrivers)[i].id_of_DBDriver, sizeof((*ClipMachineMemory->dbdrivers)[i].id_of_DBDriver)) == 0)
		  {
			  strncpy(ClipMachineMemory->def_db_driver, driver, sizeof((*ClipMachineMemory->dbdrivers)[i].id_of_DBDriver));
		     break;
		  }
	    }
      }
   return 0;
 err:
   return er;
}

int
clip__DBCREATE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBCREATE";
   const char *name = _clip_parc(ClipMachineMemory, 1);
   ClipVar *stru = _clip_par(ClipMachineMemory, 2);
   const char *driver = _clip_parc(ClipMachineMemory, 3);
   char data_driver[4];
   char idx_driver[4];
   char memo_driver[4];
   int i, er;
/* #include "ci_clipmachine.h" */

   CHECKARG1(1, CHARACTER_type_of_ClipVarType);
   CHECKARG1(2, ARRAY_type_of_ClipVarType);
   CHECKOPT1(3, CHARACTER_type_of_ClipVarType);

   if ((er = db_driver(ClipMachineMemory, driver, data_driver, idx_driver, memo_driver, __PROC__)))
      return er;

   if ((er = rdd_create(ClipMachineMemory, data_driver, memo_driver, name, stru, __PROC__)))
      return er;
	for (i = 0; i < stru->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar; i++)
      {
	 ClipVar *ap, *lp;
	ap = _clip_vptr(stru->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i);
	if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType || ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar < 4)
	    continue;
	lp = _clip_vptr(ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + 2);
	ap = _clip_vptr(ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + 1);
	if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType)
	    continue;

	/*if (toupper(stru->a.ClipVar_items_of_ClipArrVar[i].a.ClipVar_items_of_ClipArrVar[0].a.ClipVar_items_of_ClipArrVar[1].s.str.buf_of_ClipBuf[0]) == 'M') */
	if (strchr("MPG", toupper(ap->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[0])) || (toupper(ap->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[0]) == 'V' &&
lp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar >= 6))
	    {
	       char *mname = malloc(strlen(name) + 1);
	       char *s;

	       strcpy(mname, name);
	       if ((s = strrchr(mname, '.')) && strrchr(mname, '/') < s)
		  *s = 0;
	       if ((er = rdd_creatememo(ClipMachineMemory, memo_driver, mname, __PROC__)))
		  {
		     free(mname);
		     return er;
		  }
	       free(mname);
	       break;
	    }
      }
   return 0;
 err:
   return er;
}

int
clip_DBACCESS(ClipMachine * ClipMachineMemory)
{
/* #include "ci_clipmachine.h" */
/* empty */
   return 0;
}

int
clip_DBUSEAREA(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBUSEAREA";
   int newArea = _clip_parl(ClipMachineMemory, 1);
   const char *driver = _clip_parc(ClipMachineMemory, 2);
   const char *name = _clip_parc(ClipMachineMemory, 3);
   const char *alias = _clip_parc(ClipMachineMemory, 4);
   char *alname;
   int shared = _clip_parl(ClipMachineMemory, 5);
   int readonly = _clip_parl(ClipMachineMemory, 6);
   int tempo = _clip_parl(ClipMachineMemory, 7);
   char data_driver[4];
   char idx_driver[4];
   char memo_driver[4];
   RDD_DATA *rd;
   DBWorkArea *wa = NULL;
   int no, i, re, er;
   const char deftrig[] = "SX_DEFTRIGGER";
   int *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);
/* #include "ci_clipmachine.h" */

   ClipMachineMemory->neterr = 0;

   CHECKOPT1(1, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);
   CHECKARG1(3, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(4, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(5, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(6, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(7, LOGICAL_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 5) == UNDEF_type_of_ClipVarType)
      shared = !(ClipMachineMemory->flags & EXCLUSIVE_FLAG);

   if (!driver || !driver[0])
      driver = ClipMachineMemory->def_db_driver;
   if ((er = db_driver(ClipMachineMemory, driver, data_driver, idx_driver, memo_driver, __PROC__)))
      goto err;

   if (ClipMachineMemory->areas->count_of_ClipVect == 0)
      newArea = 1;
   if (!newArea)
      {
	 no = ClipMachineMemory->curArea;
	 wa = (DBWorkArea *) ClipMachineMemory->areas->items_of_ClipVect[no];
	 if ((er = close_area(ClipMachineMemory, wa, __PROC__)))
	    return er;
      }
   else
      {
	 for (no = 0; no < ClipMachineMemory->areas->count_of_ClipVect; no++)
	    if (ClipMachineMemory->areas->items_of_ClipVect[no] == NULL)
	       break;
      }

   ClipMachineMemory->curArea = no;

   if (no == ClipMachineMemory->areas->count_of_ClipVect)
      add_ClipVect(ClipMachineMemory->areas, NULL);

   ClipMachineMemory->noerrblock++;
   errno = 0;
   er = rdd_usearea(ClipMachineMemory, data_driver, name, shared, readonly, tempo, &rd, __PROC__);
   ClipMachineMemory->noerrblock--;
   if (er)
      {
	 if (*err == 32)
	    {
	       ClipMachineMemory->neterr = 1;
	       return 0;
	    }
	 else
	    {
//                      rdd_err(ClipMachineMemory,EG_OPEN,errno,__FILE__,__LINE__,__PROC__,er_open);
	       goto err;
	    }
      }

   wa = (DBWorkArea *) calloc(1, sizeof(DBWorkArea));
   strcpy(wa->driver, driver);
   strcpy(wa->idx_driver, idx_driver);
   strcpy(wa->memo_driver, memo_driver);
   wa->rd = rd;

   wa->name = malloc(strlen(name) + 1);
   strcpy(wa->name, name);

   if (alias)
      alname = (char *) alias;
   else
      alname = wa->rd->name;
   {
     /* Uri: 2006-12-29 Strip long alias names to 10 chars */
      char *s;
      int len = strlen(alname);
      len = len > 10 ? 10 : len;
      wa->alias = malloc(len + 1);
      strncpy(wa->alias, alname, len);
      wa->alias[len] = 0;
      s = wa->alias;
      while ((s = strchr(s, '.')))
	 *s = '_';
   }

   wa->rd->area = no;

   _clip_upstr(wa->alias, strlen(wa->alias));

   for (i = 0; i < wa->rd->nfields; i++)
      {
	 if (wa->rd->fields[i].type == 'M' || (wa->rd->fields[i].type == 'V' && wa->rd->fields[i].len >= 6) || (wa->rd->fields[i].type == 'P') || (wa->rd->fields[i].type == 'G'))
	    {
	       char *s;
	       char *tmpname = strdup(name);

	       if ((s = strrchr(tmpname, '.')))
		  {
		     if (!strrchr(s, '/') && !strrchr(s, '\\'))
			*s = 0;
		  }
	       if ((er = rdd_setmemo(ClipMachineMemory, wa->rd, wa->memo_driver, tmpname, __PROC__)))
		  {
		     free(tmpname);
		     goto err;
		  }
	       free(tmpname);
	       break;
	    }
      }

   wa->used = 1;
   wa->found = 0;
   wa->rd_item = _clip_store_c_item(ClipMachineMemory, (void *) wa->rd, _C_ITEM_TYPE_RDD, destroy_rdd_data);

   wa->no = no;
   ClipMachineMemory->areas->items_of_ClipVect[no] = wa;

   wa->aliasHash = _clip_casehashword(wa->alias, strlen(wa->alias));
   if (!HashTable_insert(ClipMachineMemory->aliases, wa, wa->aliasHash))
      {				/* ||
				   (strlen(wa->alias)==1 && wa->alias[0]>='A' && wa->alias[0]<='L')){ */
	 char ermsg[100];

	 _clip_destroy_c_item(ClipMachineMemory, wa->rd_item, _C_ITEM_TYPE_RDD);
	 HashTable_remove(ClipMachineMemory->aliases, wa->aliasHash);
	 ClipMachineMemory->areas->items_of_ClipVect[wa->no] = NULL;
	 snprintf(ermsg, sizeof(ermsg), _clip_gettext("Alias already in use: %s"), wa->alias);
	 er = rdd_err(ClipMachineMemory, EG_OPEN, 0, __FILE__, __LINE__, __PROC__, ermsg);
	 goto err;
      }

   if (ClipMachineMemory->flags1 & AUTOPEN_FLAG)
      {
	 char iname[1024];
	 char *s;
	 int i;
	 RDD_INDEX_VTBL *ivtbl = rdd_indexdriver(ClipMachineMemory, wa->idx_driver, __PROC__);

	 if (ivtbl && ivtbl->ismulti)
	    {
	       strcpy(iname, wa->rd->path);
	       if ((s = strrchr(iname, '.')))
		  if (s[1] != '/' && s[1] != '\\')
		     *s = 0;
	       for (i = 0; i < *ClipMachineMemory->nidx_drivers; i++)
		  if (strcasecmp(wa->idx_driver, (*ClipMachineMemory->idx_drivers)[i]->id) == 0)
		     {
			strcat(iname, (*ClipMachineMemory->idx_drivers)[i]->suff);
			break;
		     }
	       if (access(iname, F_OK) == 0)
		  {
		     if ((er = rdd_setindex(ClipMachineMemory, wa->rd, NULL, wa->idx_driver, iname, NULL, 1, __PROC__)))
			goto err;
		     wa->rd->indices[0]->structural = 1;
		     READLOCK;
		     if ((er = rdd_setorder(ClipMachineMemory, wa->rd, ClipMachineMemory->autorder, __PROC__)))
			goto err;
		     if ((er = rdd_gotop(ClipMachineMemory, wa->rd, __PROC__)))
			goto err_unlock;
		     UNLOCK;
		  }
	    }
      }

   wa->trighash = _clip_hashstr(deftrig);
   wa->trigger = strdup(deftrig);
   wa->trig_active = 1;
   if ((er = rdd_event(ClipMachineMemory, EVENT_PREUSE, wa->rd->area, 0, NULL, &re, __PROC__)))
      goto err;
   if (re)
      {
	 if ((er = rdd_event(ClipMachineMemory, EVENT_POSTUSE, wa->rd->area, 0, NULL, &re, __PROC__)))
	    goto err;
      }
   else
      {
	 if ((er = rdd_closearea(ClipMachineMemory, wa->rd, __PROC__)))
	    goto err;
	 wa->rd = NULL;
	 _clip_destroy_c_item(ClipMachineMemory, wa->rd_item, _C_ITEM_TYPE_RDD);
	 HashTable_remove(ClipMachineMemory->aliases, wa->aliasHash);
	 ClipMachineMemory->areas->items_of_ClipVect[wa->no] = NULL;
	 er = rdd_err(ClipMachineMemory, EG_OPEN, 0, __FILE__, __LINE__, __PROC__, "EVENT_PREUSE discarded");
	 goto err;
      }
   _clip_retni(ClipMachineMemory, wa->rd_item);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   if (wa && wa->trigger)
      free(wa->trigger);
   if (wa && wa->name)
      free(wa->name);
   if (wa && wa->alias)
      free(wa->alias);
   if (wa)
      free(wa);
   return er;
}

int
clip_DBCLOSEAREA(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBCLOSEAREA";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
/* #include "ci_clipmachine.h" */

   if (!wa)
      return 0;

   return close_area(ClipMachineMemory, wa, __PROC__);
}

int
clip_DBCLOSEALL(ClipMachine * ClipMachineMemory)
{
/* #include "ci_clipmachine.h" */
   _clip_close_all(ClipMachineMemory);
   return 0;
}

int
clip_DBSELECTAREA(ClipMachine * ClipMachineMemory)
{
   ClipVar *vp = _clip_par(ClipMachineMemory, 1);
   ClipVar *data;
   int ret = -1, er;
/* #include "ci_clipmachine.h" */

   if (!vp)
      {
	 _clip_retni(ClipMachineMemory, ClipMachineMemory->curArea + 1);
	 return 0;
      }

   data = NEW(ClipVar);
	if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CCODE_type_of_ClipVarType ||
		vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == PCODE_type_of_ClipVarType)
      {
	 _clip_eval(ClipMachineMemory, _clip_vptr(vp), 0, 0, data);
	 vp = data;
      }
	   else if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
      {
			char ch = toupper(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[0]);

	 if (ch >= '0' && ch <= '9')
		 ret = atol(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
	 else if ((vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf == 1) && (ch >= 'A') && (ch <= 'L'))
	    ret = ch - 'A' + 1;
      }

if (ret >= 0 || vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
      {
	 int n;

	 if (ret >= 0)
	    n = ret;
	 else
	    n = _clip_double(vp);

	 if (n)
	    {
	       ret = n - 1;
	       if (n > ClipMachineMemory->areas->count_of_ClipVect)
		  {
		     n -= ClipMachineMemory->areas->count_of_ClipVect;
		     while (n--)
			add_ClipVect(ClipMachineMemory->areas, NULL);
		  }
	    }
	 else
	    {
	       for (; n < ClipMachineMemory->areas->count_of_ClipVect; n++)
		  {
		     DBWorkArea *wa;

		     wa = (DBWorkArea *) ClipMachineMemory->areas->items_of_ClipVect[n];
		     if (wa == NULL || !wa->used)
			break;
		  }
	       ret = n;
	       if (ret == ClipMachineMemory->areas->count_of_ClipVect)
		  add_ClipVect(ClipMachineMemory->areas, NULL);
	    }
	 ClipMachineMemory->curArea = ret;
      }
	   else if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
      {
	 int len;
	 long hash;
	 DBWorkArea *wa;

	 len = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf > 10 ? 10 : vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
	 hash = _clip_casehashword(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, len);
	 wa = HashTable_fetch(ClipMachineMemory->aliases, hash);

	 if (wa)
	    ClipMachineMemory->curArea = wa->no;
	 else
	    {
			 er = rdd_err(ClipMachineMemory, EG_NOALIAS, 0, __FILE__, __LINE__, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, er_noalias);
	       goto err;
	    }
      }

   _clip_retni(ClipMachineMemory, ret + 1);
   _clip_destroy(ClipMachineMemory, data);
   free(data);
   return 0;
 err:
   _clip_destroy(ClipMachineMemory, data);
   free(data);
   return er;
}

int
clip_DBCREATEINDEX(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBCREATEINDEX";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   const char *name = _clip_parc(ClipMachineMemory, 1);
   const char *expr = _clip_parc(ClipMachineMemory, 2);
   ClipVar *block = _clip_spar(ClipMachineMemory, 3);
   int unique = _clip_parl(ClipMachineMemory, 4);
   int er;
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);
   CHECKARG1(1, CHARACTER_type_of_ClipVarType);
   CHECKARG1(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT2(3, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);
   CHECKOPT1(4, LOGICAL_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 4) == UNDEF_type_of_ClipVarType)
      unique = ClipMachineMemory->flags & UNIQUE_FLAG;

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   if (wa->rd->readonly)
      {
	 READLOCK;
      }
   else
      {
	 WRITELOCK;
      }
   if ((er = rdd_createindex(ClipMachineMemory, wa->rd, wa->idx_driver, name, NULL, expr, block, unique, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_DBCREATEIND(ClipMachine * ClipMachineMemory)
{
/* #include "ci_clipmachine.h" */
   return clip_DBCREATEINDEX(ClipMachineMemory);
}

int
clip_M6_CREATEINDEX(ClipMachine * ClipMachineMemory)
{
   ClipMachineMemory->m6_error = 0;
/* #include "ci_clipmachine.h" */
   return clip_DBCREATEINDEX(ClipMachineMemory);
}

int
clip_ORDCREATE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDCREATE";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   const char *name = _clip_parc(ClipMachineMemory, 1);
   const char *tag = _clip_parc(ClipMachineMemory, 2);
   const char *expr = _clip_parc(ClipMachineMemory, 3);
   ClipVar *block = _clip_spar(ClipMachineMemory, 4);
   int unique = _clip_parl(ClipMachineMemory, 5);
   char *nm = NULL;
   char *s = NULL;
   int er;
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);
   CHECKOPT1(1, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);
   CHECKARG1(3, CHARACTER_type_of_ClipVarType);
   CHECKOPT2(4, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);
   CHECKOPT1(5, LOGICAL_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 4) == UNDEF_type_of_ClipVarType)
      unique = ClipMachineMemory->flags & UNIQUE_FLAG;

   if (!name)
      {
	 nm = strdup(wa->rd->path);
	 s = strrchr(nm, '/');
	 strcpy(s + 1, wa->rd->name);
      }

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   if (wa->rd->readonly)
      {
	 READLOCK;
      }
   else
      {
	 WRITELOCK;
      }
   if ((er = rdd_createindex(ClipMachineMemory, wa->rd, wa->idx_driver, nm ? nm : name, tag, expr, block, unique, __PROC__)))
      goto err_unlock;
   UNLOCK;
   if (nm)
      free(nm);
   return 0;
 err_unlock:
   if (nm)
      free(nm);
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_M6_ORDCREATE(ClipMachine * ClipMachineMemory)
{
   ClipMachineMemory->m6_error = 0;
/* #include "ci_clipmachine.h" */
   return clip_ORDCREATE(ClipMachineMemory);
}

int
clip_DBSETINDEX(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBSETINDEX";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   const char *name = _clip_parc(ClipMachineMemory, 1);
   int er;
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);
   CHECKARG1(1, CHARACTER_type_of_ClipVarType);

   READLOCK;
   if ((er = rdd_setindex(ClipMachineMemory, wa->rd, NULL, wa->idx_driver, name, NULL, 0, __PROC__)))
      goto err_unlock;
   if ((er = rdd_gotop(ClipMachineMemory, wa->rd, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_DBSETORDER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBSETORDER";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int order = _clip_parni(ClipMachineMemory, 1);
   int er;
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);
   CHECKOPT1(1, NUMERIC_type_of_ClipVarType);

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_setorder(ClipMachineMemory, wa->rd, order, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_DBCLEARINDEX(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBCLEARINDEX";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);

   return rdd_clearindex(ClipMachineMemory, wa->rd, __PROC__);
}

int
clip_DBCLEARIND(ClipMachine * ClipMachineMemory)
{
/* #include "ci_clipmachine.h" */
   return clip_DBCLEARINDEX(ClipMachineMemory);
}

int
clip_DBREINDEX(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBREINDEX";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int er;
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   WRITELOCK;
   if ((er = rdd_reindex(ClipMachineMemory, wa->rd, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip___DBPACK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "__DBPACK";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int er;
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   WRITELOCK;
   if ((er = rdd_pack(ClipMachineMemory, wa->rd, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip___DBZAP(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "__DBZAP";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int er;
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);
   WRITELOCK;
   if ((er = rdd_zap(ClipMachineMemory, wa->rd, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_DBGOTOP(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBGOTOP";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int er;
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_gotop(ClipMachineMemory, wa->rd, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_DBAPPEND(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBAPPEND";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int rellocks = _clip_parl(ClipMachineMemory, 1);
   ClipVar *ap = _clip_par(ClipMachineMemory, 2);
   int *neterr = &ClipMachineMemory->neterr;
   int er;
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);
   CHECKOPT1(1, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(2, MAP_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 1) == UNDEF_type_of_ClipVarType)
      rellocks = 1;

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   WRITELOCK;
   if (rellocks)
      {
	 if ((er = rdd_ulock(ClipMachineMemory, wa->rd, 0, 0, __PROC__)))
	    goto err_unlock;
      }
   if ((er = rdd_append(ClipMachineMemory, wa->rd, neterr, __PROC__)))
      goto err_unlock;
   if (!(*neterr))
      {
			if (ap && ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
	    if ((er = rdd_dbwrite(ClipMachineMemory, wa->rd, ap, __PROC__)))
	       goto err_unlock;
      }
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_DBDELETE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBDELETE";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int er;

   CHECKWA(wa);
/* #include "ci_clipmachine.h" */

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   WRITELOCK;
   if ((er = rdd_delete(ClipMachineMemory, wa->rd, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_DBRECALL(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBRECALL";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int er;
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   WRITELOCK;
   if ((er = rdd_recall(ClipMachineMemory, wa->rd, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_DBGOBOTTOM(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBGOBOTTOM";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int er;
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_gobottom(ClipMachineMemory, wa->rd, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_DBGOTO(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBGOTO";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int rec = _clip_parni(ClipMachineMemory, 1);
   int er;
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_goto(ClipMachineMemory, wa->rd, rec, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_DBSKIP(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBSKIP";
   int recs = _clip_parni(ClipMachineMemory, 1);
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int er;
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);
   CHECKOPT1(1, NUMERIC_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 1) == UNDEF_type_of_ClipVarType)
      recs = 1;

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_skip(ClipMachineMemory, wa->rd, recs, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_DBSEEK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBSEEK";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   ClipVar *v = _clip_par(ClipMachineMemory, 1);
   int soft = _clip_parl(ClipMachineMemory, 2);
   int last = _clip_parl(ClipMachineMemory, 3);
   int found, er;
/* #include "ci_clipmachine.h" */

   _clip_retl(ClipMachineMemory, 0);
   CHECKWA(wa);
   CHECKOPT1(2, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(3, LOGICAL_type_of_ClipVarType);

   if (!v)
      return 0;

   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      soft = (ClipMachineMemory->flags & SOFTSEEK_FLAG);

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_seek(ClipMachineMemory, wa->rd, v, soft, last, &found, __PROC__)))
      goto err_unlock;
   UNLOCK;
   wa->found = (found ? 1 : 0);
   _clip_retl(ClipMachineMemory, found);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip___DBLOCATE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "__DBLOCATE";
   ClipVar *fexpr = _clip_spar(ClipMachineMemory, 1);
   ClipVar *wexpr = _clip_spar(ClipMachineMemory, 2);
   ClipVar *vnext = _clip_par(ClipMachineMemory, 3);
   ClipVar *vrec = _clip_par(ClipMachineMemory, 4);
   ClipVar *vrest = _clip_par(ClipMachineMemory, 5);
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int found, er;
/* #include "ci_clipmachine.h" */

   if (!wa)
      return 0;

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_locate(ClipMachineMemory, wa->rd, NULL, fexpr, wexpr, vnext, vrec, vrest, &found, __PROC__)))
      goto err_unlock;
   UNLOCK;

   wa->found = found;
   _clip_retl(ClipMachineMemory, found);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip___DBSETLOCATE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "__DBSETLOCATE";
   ClipVar *fexpr = _clip_spar(ClipMachineMemory, 1);
   ClipVar *rp = RETPTR(ClipMachineMemory);
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int er;
/* #include "ci_clipmachine.h" */

   if (!wa)
      return 0;

   CHECKOPT2(1, PCODE_type_of_ClipVarType, CCODE_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 1) == UNDEF_type_of_ClipVarType)
      {
	 if (wa->rd->locate_filter && wa->rd->locate_filter->fps)
	    _clip_dup(ClipMachineMemory, rp, &wa->rd->locate_filter->fps->bfilter);
      }
   else
      {
	 if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
	    goto err;
	 READLOCK;
	 if (wa->rd->locate_filter)
	    if ((er = rdd_destroyfilter(ClipMachineMemory, wa->rd->locate_filter, __PROC__)))
	       goto err_unlock;
	 if ((er = rdd_createfilter(ClipMachineMemory, wa->rd, &wa->rd->locate_filter, fexpr, NULL, NULL, 0, __PROC__)))
	    goto err_unlock;
	 if (wa->rd->filter)
	    if ((er = rm_intersect(ClipMachineMemory, wa->rd->locate_filter, wa->rd->filter, __PROC__)))
	       goto err_unlock;
	 UNLOCK;
      }
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip___DBSETLOC(ClipMachine * ClipMachineMemory)
{
/* #include "ci_clipmachine.h" */
   return clip___DBSETLOCATE(ClipMachineMemory);
}

int
clip___DBLOCATEOPTIMIZE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "__DBLOCATE";
   char *cfor = _clip_parc(ClipMachineMemory, 1);
   ClipVar *bfor = _clip_spar(ClipMachineMemory, 2);
   ClipVar *wexpr = _clip_spar(ClipMachineMemory, 3);
   ClipVar *vnext = _clip_par(ClipMachineMemory, 4);
   ClipVar *vrec = _clip_par(ClipMachineMemory, 5);
   ClipVar *vrest = _clip_par(ClipMachineMemory, 6);
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int found, er;
/* #include "ci_clipmachine.h" */

   if (!wa)
      return 0;

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_locate(ClipMachineMemory, wa->rd, cfor, bfor, wexpr, vnext, vrec, vrest, &found, __PROC__)))
      goto err_unlock;
   UNLOCK;

   wa->found = found;
   _clip_retl(ClipMachineMemory, found);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_M6_DBLOCATE(ClipMachine * ClipMachineMemory)
{
   ClipMachineMemory->m6_error = 0;
/* #include "ci_clipmachine.h" */
   return clip___DBLOCATEOPTIMIZE(ClipMachineMemory);
}

int
clip___DBCONTINUE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "__DBCONTINUE";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int found, er;
/* #include "ci_clipmachine.h" */

   if (!wa)
      return 0;

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_continue(ClipMachineMemory, wa->rd, &found, __PROC__)))
      goto err_unlock;
   UNLOCK;

   wa->found = found;
   _clip_retl(ClipMachineMemory, found);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_M6_DBCONTINUE(ClipMachine * ClipMachineMemory)
{
   ClipMachineMemory->m6_error = 0;
/* #include "ci_clipmachine.h" */
   return clip___DBCONTINUE(ClipMachineMemory);
}

int
clip_BOF(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "BOF";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int bof, er;
/* #include "ci_clipmachine.h" */

   _clip_retl(ClipMachineMemory, 0);
   if (!wa)
      return 0;
   if ((er = rdd_bof(ClipMachineMemory, wa->rd, &bof, __PROC__)))
      return er;
   _clip_retl(ClipMachineMemory, bof);
   return 0;
}

int
clip_EOF(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "EOF";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int eof, er;

   _clip_retl(ClipMachineMemory, 0);
   if (!wa)
      return 0;
/* #include "ci_clipmachine.h" */
   if ((er = rdd_eof(ClipMachineMemory, wa->rd, &eof, __PROC__)))
      return er;
   _clip_retl(ClipMachineMemory, eof);
   return 0;
}

int
clip_USED(ClipMachine * ClipMachineMemory)
{
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int ret = 0;
/* #include "ci_clipmachine.h" */

   if (wa)
      ret = wa->used;

   _clip_retl(ClipMachineMemory, ret);
   return 0;
}

int
clip_DELETED(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DELETED";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int deleted, er;
   int locked = wa->rd->file.rlocked || wa->rd->file.wlocked;
/* #include "ci_clipmachine.h" */

   _clip_retl(ClipMachineMemory, 0);
   if (!wa)
      return 0;

   if (!locked)
      {
	 READLOCK;
      }
   if ((er = rdd_deleted(ClipMachineMemory, wa->rd, &deleted, __PROC__)))
      goto err_unlock;
   if (!locked)
      {
	 UNLOCK;
      }
   _clip_retl(ClipMachineMemory, deleted);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_DELE(ClipMachine * ClipMachineMemory)
{
/* #include "ci_clipmachine.h" */
   return clip_DELETED(ClipMachineMemory);
}

int
clip_DELETE(ClipMachine * ClipMachineMemory)
{
/* #include "ci_clipmachine.h" */
   return clip_DELETED(ClipMachineMemory);
}

int
clip_RECNO(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RECNO";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int recno, er;
/* #include "ci_clipmachine.h" */

   _clip_retndp(ClipMachineMemory, 0, 7, 0);
   if (!wa)
      return 0;

   if ((er = rdd_recno(ClipMachineMemory, wa->rd, &recno, __PROC__)))
      return er;
   _clip_retndp(ClipMachineMemory, recno, 7, 0);
   return 0;
}

int
clip_RECN(ClipMachine * ClipMachineMemory)
{
/* #include "ci_clipmachine.h" */
   return clip_RECNO(ClipMachineMemory);
}

int
clip_RECCOUNT(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RECCOUNT";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int lastrec, er;
/* #include "ci_clipmachine.h" */

   _clip_retni(ClipMachineMemory, 0);
   if (!wa)
      return 0;
   READLOCK;
   if ((er = rdd_lastrec(ClipMachineMemory, wa->rd, &lastrec, __PROC__)))
      goto err_unlock;
   UNLOCK;
   _clip_retni(ClipMachineMemory, lastrec);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_RECC(ClipMachine * ClipMachineMemory)
{
/* #include "ci_clipmachine.h" */
   return clip_RECCOUNT(ClipMachineMemory);
}

int
clip_LASTREC(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "LASTREC";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int lastrec, er;
/* #include "ci_clipmachine.h" */

   _clip_retni(ClipMachineMemory, 0);
   if (!wa)
      return 0;
   READLOCK;
   if ((er = rdd_lastrec(ClipMachineMemory, wa->rd, &lastrec, __PROC__)))
      goto err_unlock;
   UNLOCK;
   _clip_retni(ClipMachineMemory, lastrec);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_DBSTRUCT(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBSTRUCT";
   DBWorkArea *wa;
   int nfld, i;
   ClipVar *rp = RETPTR(ClipMachineMemory);
   long vect[2];
   int area = _clip_parni(ClipMachineMemory, 1);
   int er;
/* #include "ci_clipmachine.h" */

   CHECKOPT1(1, NUMERIC_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 1) == UNDEF_type_of_ClipVarType)
      wa = cur_area(ClipMachineMemory);
   else
      wa = get_area(ClipMachineMemory, area, 0, 0);

   if (!wa)
      {
	 vect[0] = 0;
	 return _clip_array(ClipMachineMemory, rp, 1, vect);
      }

   nfld = wa->rd->nfields;
   vect[0] = nfld;
   vect[1] = 4 + ((wa->rd->sig == 0x03) ? 0 : 2);
   _clip_array(ClipMachineMemory, rp, 2, vect);

   for (i = 0; i < nfld; i++)
      {
	 ClipVar var;
	 RDD_FIELD *fp = &wa->rd->fields[i];

	 vect[0] = i;
	 memset(&var, 0, sizeof(ClipVar));

	var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = fp->name;
	var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen(fp->name);
	 vect[1] = 0;
	 _clip_aset(ClipMachineMemory, rp, &var, 2, vect);

	 var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = malloc(2);
	 var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[0] = fp->type;
	 var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[1] = 0;
	 var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = 1;
	 vect[1] = 1;
	 _clip_aset(ClipMachineMemory, rp, &var, 2, vect);
	 free(var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);

	 var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	 var.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = fp->len;
	 var.ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	 vect[1] = 2;
	 _clip_aset(ClipMachineMemory, rp, &var, 2, vect);

	 var.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = fp->dec;
	 vect[1] = 3;
	 _clip_aset(ClipMachineMemory, rp, &var, 2, vect);

	 if (wa->rd->sig == 0x30)
	    {
			 var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
			 var.ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = fp->binary;
	       vect[1] = 4;
	       _clip_aset(ClipMachineMemory, rp, &var, 2, vect);

			 var.ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = fp->nullable;
	       vect[1] = 5;
	       _clip_aset(ClipMachineMemory, rp, &var, 2, vect);
	    }
      }
   return 0;
 err:
   return er;
}

int
clip_AFIELDS(ClipMachine * ClipMachineMemory)
{
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int nfld, i;
   long vect[1];
   ClipVar *name_ap = _clip_par(ClipMachineMemory, 1);
   ClipVar *type_ap = _clip_par(ClipMachineMemory, 2);
   ClipVar *len_ap = _clip_par(ClipMachineMemory, 3);
   ClipVar *dec_ap = _clip_par(ClipMachineMemory, 4);
   int r;
/* #include "ci_clipmachine.h" */

   if (!wa)
      {
	 _clip_retni(ClipMachineMemory, 0);
	 return 0;
      }

   nfld = wa->rd->nfields;

   vect[0] = nfld;
	if (name_ap && name_ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType)
      name_ap = NULL;
	if (type_ap && type_ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType)
      type_ap = NULL;
	if (len_ap && len_ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType)
      len_ap = NULL;
	if (dec_ap && dec_ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType)
      dec_ap = NULL;

   for (i = 0; i < nfld; i++)
      {
	 ClipVar var;
	 RDD_FIELD *fp = &wa->rd->fields[i];

	 vect[0] = i;
	 memset(&var, 0, sizeof(ClipVar));

	if (name_ap && i < name_ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar)
	    {
			 var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
			 var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = fp->name;
			 var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen(fp->name);
	       _clip_aset(ClipMachineMemory, name_ap, &var, 1, vect);
	    }

if (type_ap && i < type_ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar)
	    {
			 var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
			 var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = malloc(2);
			 var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[0] = fp->type;
			 var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[1] = 0;
			 var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = 1;
	       _clip_aset(ClipMachineMemory, type_ap, &var, 1, vect);
	    }

if (len_ap && i < len_ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar)
	    {
			 var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
			 var.ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
			 var.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = fp->len;
			 var.ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	       _clip_aset(ClipMachineMemory, len_ap, &var, 1, vect);
	    }

if (dec_ap && i < dec_ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar)
	    {
			 var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
			 var.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = fp->dec;
			 var.ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	       _clip_aset(ClipMachineMemory, dec_ap, &var, 1, vect);
	    }
      }
   r = 0x7fffffff;
   if (name_ap)
		if (name_ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar < r)
			r = name_ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
   if (type_ap)
		if (type_ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar < r)
			r = type_ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
   if (len_ap)
		if (len_ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar < r)
			r = len_ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
   if (dec_ap)
		if (dec_ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar < r)
			r = dec_ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
   _clip_retni(ClipMachineMemory, r);
   return 0;
}

int
clip_FCOUNT(ClipMachine * ClipMachineMemory)
{
   DBWorkArea *wa = cur_area(ClipMachineMemory);
/* #include "ci_clipmachine.h" */

   _clip_retni(ClipMachineMemory, 0);
   if (!wa)
      return 0;

   _clip_retni(ClipMachineMemory, wa->rd->nfields);
   return 0;
}

int
clip_FCOU(ClipMachine * ClipMachineMemory)
{
/* #include "ci_clipmachine.h" */
   return clip_FCOUNT(ClipMachineMemory);
}

int
clip_FCOUN(ClipMachine * ClipMachineMemory)
{
/* #include "ci_clipmachine.h" */
   return clip_FCOUNT(ClipMachineMemory);
}

int
clip_FIELDNAME(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "FIELDNAME";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int fld = _clip_parni(ClipMachineMemory, 1) - 1;
/* #include "ci_clipmachine.h" */

   if (!wa)
      {
	 _clip_retc(ClipMachineMemory, "");
	 return 0;
      }

   return rdd_fieldname(ClipMachineMemory, wa->rd, fld, __PROC__);
}

int
clip_FIELD(ClipMachine * ClipMachineMemory)
{
/* #include "ci_clipmachine.h" */
   return clip_FIELDNAME(ClipMachineMemory);
}

int
clip_FIELDPOS(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "FIELDPOS";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   const char *name = _clip_parc(ClipMachineMemory, 1);
   int er;
/* #include "ci_clipmachine.h" */

   _clip_retni(ClipMachineMemory, 0);
   if (!wa)
      return 0;

   CHECKARG1(1, CHARACTER_type_of_ClipVarType);

   _clip_retni(ClipMachineMemory, _rdd_fieldno(wa->rd, _clip_casehashword(name, strlen(name))) + 1);
   return 0;
 err:
   return er;
}

static int
is_dig(const char *s, int l)
{
   int i;

   for (i = 0; i < l; i++)
      if (!isdigit(s[i]))
	 return 0;
   return 1;
}

static int
is_id(int c)
{
   if (isalnum(c))
      return 1;
   if (c == '_')
      return 1;
   return 0;
}

/*
	return 1 if memvar, 2 if field name, 3 if array/map selector
		, 0 if unspecified (field/memvar)
	*fnamep name of field or memvar
	*anamep alias (if exist)
	*dimp - realloc'able array index vector
	*ndimp - it's length
*/
CLIP_DLLEXPORT int
_clip_parse_name(ClipMachine * ClipMachineMemory, char *name, int l, char **anamep, int *alp, char **fnamep, int *flp, long **dimp, int *ndimp)
{
   char *s, *e, *p, *ep;
   int ret = 0;
   char *aname = 0, *fname = 0;
   int al = 0, fl = 0;
   long val;
   int ll;
/* #include "ci_clipmachine.h" */

   for (e = name + l; e > name && isspace(e[-1]); e--, l--)
      ;
   fname = name;
   fl = l;

   for (s = name; s < e; s++)
      {
	 int el, c = 0;

	 for (; s < e && isspace(*s); s++)
	    ;
	 p = s;
	 for (ep = p; ep < e && is_id((c = *ep)); ep++)
	    ;
	 if (ep == e)
	    break;
	 if (c != ' ' && c != '\t' && c != '-' && c != '[' && c != ']' && c != ':')
	    continue;

	 el = ep - p;
	 if ((el == 5 && !strncasecmp(p, "field", 5)) || (el == 6 && !strncasecmp(p, "_field", 6)) || (el == 1 && !strncasecmp(p, "f", 1)))
	    {
	       for (; ep < e && (c = *ep) != '-'; ep++)
		  ;
	       if (c == '-' && ep < e && ep[1] == '>')
		  {
		     for (p = ep + 2; (p < e && *p == ' ') || *p == '\n'; p++)
			;
		     fname = p;
		     fl = e - p;
		     ret = 2;
		  }
	    }
	 else if ((el == 6 && !strncasecmp(p, "memvar", 6)) || (el == 1 && !strncasecmp(p, "m", 1)))
	    {
	       for (; ep < e && (c = *ep) != '-'; ep++)
		  ;
	       if (c == '-' && ep < e && ep[1] == '>')
		  {
		     for (p = ep + 2; (p < e && *p == ' ') || *p == '\n'; p++)
			;
		     fname = p;
		     fl = e - p;
		     ret = 1;
		  }
	    }
	 else if (!strncasecmp(ep, "::", 2))
	    {
	       fname = "__SELF__";
	       fl = 8;
	       ep += 2;
	    }

	 if (!ret)
	    {
	       fname = p;
	       fl = ep - p;
	       ll = strspn(ep, " \t\n\r");
	       ep += ll;
	       ll = strspn(ep, "[]:->");
	       if (ll == 2 && ep[0] == '-' && ep[1] == '>')
		  {
		     al = el;
		     aname = p;
		     p = ep + 2;
		     ll = strspn(p, " \t\n\r");
		     p += ll;
		     fname = p;
		     fl = strcspn(p, " \t\r\n[]:-");
		     ret = 2;
		  }
	    }

	 if (!ret)
	    {
	       while (ep < e)
		  {
		     ll = strspn(ep, " \t\n\r[]:");
		     ep += ll;
		     ll = strcspn(ep, " \t\n\r[]:");
		     if (!ll)
			break;

		     ret = 3;

		     if (dimp && ndimp)
			*dimp = (long *) realloc(*dimp, sizeof(long) * (1 + *ndimp));

		     if (is_dig(ep, ll))
			val = atoi(ep) - 1;
		     else
			{
			   ClipVar *var;
			   var = _clip_ref_memvar_noadd(ClipMachineMemory, _clip_casehashbytes(0, ep, ll));
			   if (var)
			      val = _clip_long(var) - 1;
			   else
			      val = _clip_casehashbytes(0, ep, ll);
			}
		     if (dimp && ndimp)
			{
			   (*dimp)[*ndimp] = val;
			   (*ndimp)++;
			}
		     ep += ll;
		  }
	    }
      }

   if (anamep && alp)
      {
	 *anamep = aname;
	 *alp = al;
      }
   if (fnamep && flp)
      {
	 *fnamep = fname;
	 *flp = fl;
      }

   return ret;
}

int
clip_ISFIELD(ClipMachine * ClipMachineMemory)
{
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int l, ret = 0;
   char *name = _clip_parcl(ClipMachineMemory, 1, &l);
   char *aname, *fname;
   int al, fl;
/* #include "ci_clipmachine.h" */

   if (name)
      {
	 if (wa)
	    {
	       if (_rdd_fieldno(wa->rd, _clip_casehashword(name, l)) != -1)
		  ret = 1;
	    }

	 if (!ret)
	    {
	       if (_clip_parse_name(ClipMachineMemory, name, l, &aname, &al, &fname, &fl, 0, 0) == 2)
		  {
		     if (aname)
			wa = get_area(ClipMachineMemory, _clip_casehashword(aname, al), 0, 0);
		     else
			wa = cur_area(ClipMachineMemory);
		     if (wa && fname)
			{
			   if (_rdd_fieldno(wa->rd, _clip_casehashword(fname, fl)) != -1)
			      ret = 1;
			}
		  }
	    }
      }

   _clip_retl(ClipMachineMemory, ret);
   return 0;
}

CLIP_DLLEXPORT int
_clip_nameassign(ClipMachine * ClipMachineMemory, char *name, int l, ClipVar * vp, int fieldprec)
{
   int r;
   char *aname, *fname;
   int al, fl;
   DBWorkArea *wa;
   char nm[] = "_clip_nameassign";
   long *dim = 0;
   int ndim = 0;
/* #include "ci_clipmachine.h" */

   if (fieldprec)
      {
	 wa = cur_area(ClipMachineMemory);
	 if (wa)
	    {
	       r = rdd_setvaluebn(ClipMachineMemory, wa->rd, name, vp, nm);
	       if (!r)
		  return 0;
	    }
	 else
	    return rdd_err(ClipMachineMemory, EG_NOALIAS, 0, __FILE__, __LINE__, nm, er_noalias);
      }

   r = _clip_parse_name(ClipMachineMemory, name, l, &aname, &al, &fname, &fl, &dim, &ndim);
   if (r == 2)
      {
	 if (aname)
	    wa = get_area(ClipMachineMemory, _clip_casehashword(aname, al), 0, 0);
	 else
	    wa = cur_area(ClipMachineMemory);
	 if (wa && fname)
	    {
	       r = rdd_setvaluebn(ClipMachineMemory, wa->rd, fname, vp, nm);
	       if (r)
		  return r;
	    }
	 else
	    return EG_NOALIAS;
      }
   else if (r == 3)
      {
	 int r = EG_NOVAR;
	 ClipVar *rp;

	 rp = _clip_ref_memvar(ClipMachineMemory, _clip_casehashword(fname, fl));
	 if (rp)
	    r = _clip_aset(ClipMachineMemory, rp, vp, ndim, dim);
	 free(dim);

	 return r;
      }
   else
      {
	 ClipVar *rp;

	 rp = _clip_ref_memvar(ClipMachineMemory, _clip_casehashword(fname, fl));
	 if (rp)
	    {
	       if (_clip_mclone(ClipMachineMemory, rp, vp))
		  return EG_NOVAR;
	    }
	 else
	    return EG_NOVAR;

      }

   return 0;
}

int
clip_SETFIELD(ClipMachine * ClipMachineMemory)
{
   int l;
   char *name = _clip_parcl(ClipMachineMemory, 1, &l);
   ClipVar *vp = _clip_par(ClipMachineMemory, 2);
/* #include "ci_clipmachine.h" */

   if (!vp || !name)
      return 0;

   return _clip_nameassign(ClipMachineMemory, name, l, vp, 1);
}

int
clip_FIELDGET(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "FIELDGET";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int fno = _clip_parni(ClipMachineMemory, 1);
   int er;
/* #include "ci_clipmachine.h" */

   if (!wa)
      return 0;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   if (fno < 1 || fno > wa->rd->nfields)
      return 0;

   READLOCK;
   if ((er = rdd_getvalue(ClipMachineMemory, wa->rd, fno - 1, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_FIELDPUT(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "FIELDPUT";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int fno = _clip_parni(ClipMachineMemory, 1);
   ClipVar *vp = _clip_par(ClipMachineMemory, 2);
   int er;
/* #include "ci_clipmachine.h" */

   if (!wa)
      return 0;

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   if (fno < 1 || fno > wa->rd->nfields)
      return 0;

   WRITELOCK;
   if ((er = rdd_setvalue(ClipMachineMemory, wa->rd, fno - 1, vp, __PROC__)))
      goto err_unlock;
   UNLOCK;
   _clip_clone(ClipMachineMemory, RETPTR(ClipMachineMemory), vp);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_DBREAD(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBREAD";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int er;
/* #include "ci_clipmachine.h" */

   if (!wa)
      return rdd_err(ClipMachineMemory, EG_NOTABLE, 0, __FILE__, __LINE__, __PROC__, er_notable);

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_dbread(ClipMachineMemory, wa->rd, RETPTR(ClipMachineMemory), __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_DBWRITE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBWRITE";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   ClipVar *ap = _clip_par(ClipMachineMemory, 1);
   int deleted = _clip_parl(ClipMachineMemory, 2);
   int er;
/* #include "ci_clipmachine.h" */

   if (!wa)
      return rdd_err(ClipMachineMemory, EG_NOTABLE, 0, __FILE__, __LINE__, __PROC__, er_notable);
   CHECKARG1(1, MAP_type_of_ClipVarType);
   CHECKOPT1(2, LOGICAL_type_of_ClipVarType);

   WRITELOCK;
   if ((er = rdd_dbwrite(ClipMachineMemory, wa->rd, ap, __PROC__)))
      goto err_unlock;
   if (_clip_parinfo(ClipMachineMemory, 0) > 1)
      {
	 if (deleted)
	    {
	       if ((er = rdd_delete(ClipMachineMemory, wa->rd, __PROC__)))
		  goto err_unlock;
	    }
	 else
	    {
	       if ((er = rdd_recall(ClipMachineMemory, wa->rd, __PROC__)))
		  goto err_unlock;
	    }
      }
   UNLOCK;
   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_ALIAS(ClipMachine * ClipMachineMemory)
{
   int nwa = _clip_parni(ClipMachineMemory, 1);
   DBWorkArea *wa;
/* #include "ci_clipmachine.h" */

   if (nwa > 0)
      wa = get_area(ClipMachineMemory, nwa, 0, 0);
   else
      wa = cur_area(ClipMachineMemory);

   if (!wa)
      {
	 _clip_retc(ClipMachineMemory, "");
	 return 0;
      }

   _clip_retc(ClipMachineMemory, wa->alias);
   return 0;
}

int
clip_DBF(ClipMachine * ClipMachineMemory)
{
/* #include "ci_clipmachine.h" */
   return clip_ALIAS(ClipMachineMemory);
}

int
clip_SELECT(ClipMachine * ClipMachineMemory)
{
   const char *alias = _clip_parc(ClipMachineMemory, 1);
   DBWorkArea *wa;
   int i, len, ret = 0;
   char *al, *e;
/* #include "ci_clipmachine.h" */

   if (_clip_parinfo(ClipMachineMemory, 0) > 0 && _clip_parinfo(ClipMachineMemory, 1) == UNDEF_type_of_ClipVarType)
      {
	 _clip_retni(ClipMachineMemory, 0);
	 return 0;
      }
   if (!alias)
      {
	 _clip_retni(ClipMachineMemory, ClipMachineMemory->curArea + 1);
	 return 0;
      }

   al = strdup(alias);
   e = al + strlen(al);
   while (*(--e) == ' ');
   *(e + 1) = 0;
   e = al;
   while (isalpha(*e) || isdigit(*e) || *e == '_')
      e++;
   *e = 0;
   len = strlen(al);
   if (len > 10)
      len = 10;
   al[len] = 0;

   for (i = 0; i < ClipMachineMemory->areas->count_of_ClipVect; i++)
      {
	 wa = (DBWorkArea *) ClipMachineMemory->areas->items_of_ClipVect[i];
	 if (wa && wa->alias && !strcasecmp(al, wa->alias))
	    {
	       ret = i + 1;
	       break;
	    }
      }
   free(al);

   _clip_retni(ClipMachineMemory, ret);
   return 0;
}

int
clip_SELE(ClipMachine * ClipMachineMemory)
{
/* #include "ci_clipmachine.h" */
   return clip_SELECT(ClipMachineMemory);
}

int
clip_INDEXKEY(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "INDEXKEY";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int ord = _clip_parni(ClipMachineMemory, 1);
   int er;
/* #include "ci_clipmachine.h" */

   _clip_retc(ClipMachineMemory, "");
   CHECKWA(wa);
   CHECKOPT1(1, NUMERIC_type_of_ClipVarType);

   if (ord == 0)
      ord = wa->rd->curord + 1;

   if (ord < 1 || ord > wa->rd->ords_opened)
      return 0;

   _clip_retc(ClipMachineMemory, wa->rd->orders[ord - 1]->expr);
   return 0;
 err:
   return er;
}

int
clip_INDEXORD(ClipMachine * ClipMachineMemory)
{
   DBWorkArea *wa = cur_area(ClipMachineMemory);
/* #include "ci_clipmachine.h" */

   _clip_retni(ClipMachineMemory, 0);
   if (!wa)
      return 0;

   _clip_retni(ClipMachineMemory, wa->rd->curord + 1);
   return 0;
}

int
clip_RDDSETDEFAULT(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDSETDEFAULT";
   const char *driver = _clip_parc(ClipMachineMemory, 1);
   int er;
/* #include "ci_clipmachine.h" */

   CHECKOPT1(1, CHARACTER_type_of_ClipVarType);

   _clip_retc(ClipMachineMemory, ClipMachineMemory->def_db_driver);

   if (driver && driver[0])
      {
	 int i;
	 for (i = 0; i < *ClipMachineMemory->ndbdrivers; i++)
	    {
			 if (strncasecmp(driver, (*ClipMachineMemory->dbdrivers)[i].id_of_DBDriver, sizeof((*ClipMachineMemory->dbdrivers)[i].id_of_DBDriver)) == 0)
		  {
			  strncpy(ClipMachineMemory->def_db_driver, driver, sizeof((*ClipMachineMemory->dbdrivers)[i].id_of_DBDriver));
		     ClipMachineMemory->def_db_driver[8] = 0;
			  strcpy(ClipMachineMemory->def_data_driver, (*ClipMachineMemory->dbdrivers)[i].data_of_DBDriver);
			  strcpy(ClipMachineMemory->def_idx_driver, (*ClipMachineMemory->dbdrivers)[i].idx_of_DBDriver);
			  strcpy(ClipMachineMemory->def_memo_driver, (*ClipMachineMemory->dbdrivers)[i].memo_of_DBDriver);
		     break;
		  }
	    }
      }
   return 0;
 err:
   return er;
}

int
clip_INDEXEXT(ClipMachine * ClipMachineMemory)
{
   int i, j;
/* #include "ci_clipmachine.h" */

   for (i = 0; i < *ClipMachineMemory->ndbdrivers; i++)
      {
			if (strncasecmp(ClipMachineMemory->def_db_driver, (*ClipMachineMemory->dbdrivers)[i].id_of_DBDriver, 6) == 0)
	    {
	       for (j = 0; j < *ClipMachineMemory->nidx_drivers; j++)
		  {
			  if (strncasecmp((*ClipMachineMemory->idx_drivers)[j]->id, (*ClipMachineMemory->dbdrivers)[i].idx_of_DBDriver, sizeof((*ClipMachineMemory->idx_drivers)[j]->id)) == 0)
			{

			   _clip_retc(ClipMachineMemory, (*ClipMachineMemory->idx_drivers)[j]->suff);
			   break;
			}
		  }
	       break;
	    }
      }
   return 0;
}

int
clip_ORDBAGEXT(ClipMachine * ClipMachineMemory)
{
/* #include "ci_clipmachine.h" */
   return clip_INDEXEXT(ClipMachineMemory);
}

int
clip_TABLEEXT(ClipMachine * ClipMachineMemory)
{
   int i, j;
   for (i = 0; i < *ClipMachineMemory->ndbdrivers; i++)
      {
			if (strncasecmp(ClipMachineMemory->def_db_driver, (*ClipMachineMemory->dbdrivers)[i].id_of_DBDriver, 6) == 0)
	    {
	       for (j = 0; j < *ClipMachineMemory->ndata_drivers; j++)
		  {
			  if (strncasecmp((*ClipMachineMemory->data_drivers)[j]->id, (*ClipMachineMemory->dbdrivers)[i].data_of_DBDriver, sizeof((*ClipMachineMemory->data_drivers)[j]->id)) == 0)
			{

			   _clip_retc(ClipMachineMemory, (*ClipMachineMemory->data_drivers)[j]->suff);
			   break;
			}
		  }
	       break;
	    }
      }
   return 0;
}

int
clip_MEMOEXT(ClipMachine * ClipMachineMemory)
{
/* #include "ci_clipmachine.h" */
   int i, j;
   for (i = 0; i < *ClipMachineMemory->ndbdrivers; i++)
      {
			if (strncasecmp(ClipMachineMemory->def_db_driver, (*ClipMachineMemory->dbdrivers)[i].id_of_DBDriver, 6) == 0)
	    {
	       for (j = 0; j < *ClipMachineMemory->nmemo_drivers; j++)
		  {
			  if (strncasecmp((*ClipMachineMemory->memo_drivers)[j]->id, (*ClipMachineMemory->dbdrivers)[i].memo_of_DBDriver, sizeof((*ClipMachineMemory->memo_drivers)[j]->id)) == 0)
			{

			   _clip_retc(ClipMachineMemory, (*ClipMachineMemory->memo_drivers)[j]->suff);
			   break;
			}
		  }
	       break;
	    }
      }
   return 0;
}

int
clip_RDDNAME(ClipMachine * ClipMachineMemory)
{
#ifndef CLIPPER50_COMPATIBLE
   const char *__PROC__ = "RDDNAME";
#endif
   DBWorkArea *wa = cur_area(ClipMachineMemory);
/* #include "ci_clipmachine.h" */

   _clip_retc(ClipMachineMemory, "");
   CHECKWA(wa);

   _clip_retc(ClipMachineMemory, wa->driver);
   return 0;
}

int
clip_RDDLIST(ClipMachine * ClipMachineMemory)
{
   ClipVar *rp = RETPTR(ClipMachineMemory);
   long vect[1];
   int i;
/* #include "ci_clipmachine.h" */

   vect[0] = *ClipMachineMemory->ndbdrivers;
   _clip_array(ClipMachineMemory, rp, 1, vect);

   for (i = 0; i < *ClipMachineMemory->ndbdrivers; i++)
      {
	 ClipVar var;
	var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	var.ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	var.ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen((*ClipMachineMemory->dbdrivers)[i].id_of_DBDriver);
	var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = (*ClipMachineMemory->dbdrivers)[i].id_of_DBDriver;
	 vect[0] = i;
	 _clip_aset(ClipMachineMemory, rp, &var, 1, vect);
      }
   return 0;
}

int
clip_LUPDATE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "LUPDATE";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int er;
/* #include "ci_clipmachine.h" */

   if (!wa)
      return 0;

   READLOCK;
   if ((er = wa->rd->vtbl->lupdate(ClipMachineMemory, wa->rd, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_RECSIZE(ClipMachine * ClipMachineMemory)
{
   DBWorkArea *wa = cur_area(ClipMachineMemory);
/* #include "ci_clipmachine.h" */

   _clip_retni(ClipMachineMemory, 0);
   if (!wa)
      return 0;

   _clip_retni(ClipMachineMemory, wa->rd->recsize);
   return 0;
}

int
clip_HEADER(ClipMachine * ClipMachineMemory)
{
   DBWorkArea *wa = cur_area(ClipMachineMemory);
/* #include "ci_clipmachine.h" */

   _clip_retni(ClipMachineMemory, 0);
   if (!wa)
      return 0;

   _clip_retni(ClipMachineMemory, wa->rd->hdrsize);
   return 0;
}

CLIP_DLLEXPORT int
_clip_filter(void *ClipMachineMemory)
{
   return 0;
}

int
clip_DBCLEARFILTER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBCLEARFILTER";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);

   return rdd_clearfilter(ClipMachineMemory, wa->rd, __PROC__);
}

int
clip_DBSETFILTER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBSETFILTER";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   ClipVar *block = _clip_spar(ClipMachineMemory, 1);
   const char *str = _clip_parc(ClipMachineMemory, 2);
   int lNoOptimize = _clip_parl(ClipMachineMemory, 3);
   RDD_FILTER *fp;
   int er;
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);

   CHECKOPT2(1, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(3, LOGICAL_type_of_ClipVarType);

   if ((er = rdd_clearfilter(ClipMachineMemory, wa->rd, __PROC__)))
      goto err;

   if (_clip_parinfo(ClipMachineMemory, 1) != CCODE_type_of_ClipVarType && _clip_parinfo(ClipMachineMemory, 1) != PCODE_type_of_ClipVarType)
      return 0;

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_createfilter(ClipMachineMemory, wa->rd, &fp, block, str, NULL, lNoOptimize, __PROC__)))
      goto err_unlock;
   wa->rd->filter = fp;
   fp->active = 1;
   if ((er = _rdd_calcfiltlist(ClipMachineMemory, wa->rd, fp, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_M6_SETFILTER(ClipMachine * ClipMachineMemory)
{
   ClipMachineMemory->m6_error = 0;
/* #include "ci_clipmachine.h" */
   return clip_DBSETFILTER(ClipMachineMemory);
}

int
clip_DBFILTER(ClipMachine * ClipMachineMemory)
{
   DBWorkArea *wa = cur_area(ClipMachineMemory);
/* #include "ci_clipmachine.h" */

   if (!wa)
      return 0;

   _clip_retc(ClipMachineMemory, "");
   if (wa->rd->filter && wa->rd->filter->sfilter)
      _clip_retc(ClipMachineMemory, wa->rd->filter->sfilter);
   return 0;
}

int
clip_ISFILTERED(ClipMachine * ClipMachineMemory)
{
   DBWorkArea *wa = cur_area(ClipMachineMemory);
/* #include "ci_clipmachine.h" */

   _clip_retl(ClipMachineMemory, 0);
   if (!wa)
      return 0;

   _clip_retl(ClipMachineMemory, wa->rd->filter != NULL);
   return 0;
}

int
clip_DBRELATION(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBRELATION";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int rel = _clip_parni(ClipMachineMemory, 1);
   int er;
/* #include "ci_clipmachine.h" */

   _clip_retc(ClipMachineMemory, "");
   if (!wa)
      return 0;

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   if (!(rel < 1 || rel > wa->rd->rels_opened))
      _clip_retc(ClipMachineMemory, wa->rd->relations[rel - 1]->expr);
   return 0;
 err:
   return er;
}

int
clip_FOUND(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "FOUND";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int er;
/* #include "ci_clipmachine.h" */

   _clip_retl(ClipMachineMemory, 0);
   if (!wa)
      return 0;

   if (wa->rd->pending_child_parent)
      if ((er = rdd_child_duty(ClipMachineMemory, wa->rd, __PROC__)))
	 return er;

   _clip_retl(ClipMachineMemory, wa->found ? 1 : 0);
   return 0;
}

int
clip_FOUN(ClipMachine * ClipMachineMemory)
{
/* #include "ci_clipmachine.h" */
   return clip_FOUND(ClipMachineMemory);
}

int
clip_DBCLEARRELATION(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBCLEARRELATION";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
/* #include "ci_clipmachine.h" */

   if (!wa)
      return 0;
   return rdd_clearrelation(ClipMachineMemory, wa->rd, __PROC__);
}

int
clip_DBCLEARREL(ClipMachine * ClipMachineMemory)
{
/* #include "ci_clipmachine.h" */
   return clip_DBCLEARRELATION(ClipMachineMemory);
}

int
clip_DBSETRELATION(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBSETRELATION";
   int type = _clip_parinfo(ClipMachineMemory, 1);
   ClipVar *block = _clip_spar(ClipMachineMemory, 2);
   const char *expr = _clip_parc(ClipMachineMemory, 3);
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   DBWorkArea *childwa;
   int er;
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);
   CHECKARG2(1, NUMERIC_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   CHECKARG2(2, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);
   CHECKOPT1(3, CHARACTER_type_of_ClipVarType);

   if (type == NUMERIC_type_of_ClipVarType)
      {
	 childwa = get_area(ClipMachineMemory, _clip_parni(ClipMachineMemory, 1), 0, 0);
      }
   else
      {
	 int i, len;
	 DBWorkArea *twa;
	 char *alias = strdup(_clip_parc(ClipMachineMemory, 1));
	 char *e = alias + strlen(alias);
	 while (*(--e) == ' ');
	 *(e + 1) = 0;
	 len = strlen(alias);
	 if (len > 10)
	    len = 10;
	 alias[len] = 0;
	 for (i = 0; i < ClipMachineMemory->areas->count_of_ClipVect; i++)
	    {
	       twa = (DBWorkArea *) ClipMachineMemory->areas->items_of_ClipVect[i];
	       if (twa && twa->alias && !strcasecmp(alias, twa->alias))
		  break;
	    }
	 free(alias);
	 childwa = get_area(ClipMachineMemory, i + 1, 0, 0);
      }

   CHECKWA(childwa);

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_setrelation(ClipMachineMemory, wa->rd, childwa->rd, block, expr, 0, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_RLOCK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RLOCK";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int r, er;
/* #include "ci_clipmachine.h" */

   if (!wa)
      return 0;

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if (!(ClipMachineMemory->flags1 & MULTILOCKS_FLAG))
      {
	 if ((er = rdd_ulock(ClipMachineMemory, wa->rd, 0, 0, __PROC__)))
	    goto err_unlock;
      }
   if (wa->rd->pending_child_parent)
      if ((er = rdd_child_duty(ClipMachineMemory, wa->rd, __PROC__)))
	 goto err_unlock;
   if ((er = rdd_rlock(ClipMachineMemory, wa->rd, wa->rd->recno, &r, __PROC__)))
      goto err_unlock;
   UNLOCK;
   _clip_retl(ClipMachineMemory, r);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_DBRLOCK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBRLOCK";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int rno = _clip_parni(ClipMachineMemory, 1);
   int r, er;
/* #include "ci_clipmachine.h" */

   _clip_retl(ClipMachineMemory, 0);
   if (!wa)
      return 0;

   CHECKOPT1(1, NUMERIC_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 1) == UNDEF_type_of_ClipVarType)
      return clip_RLOCK(ClipMachineMemory);

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_rlock(ClipMachineMemory, wa->rd, rno, &r, __PROC__)))
      goto err_unlock;
   UNLOCK;
   _clip_retl(ClipMachineMemory, r);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_DBRUNLOCK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBRUNLOCK";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   unsigned int rec = _clip_parni(ClipMachineMemory, 1);
   int er;
/* #include "ci_clipmachine.h" */

   if (!wa)
      return 0;
   CHECKOPT1(1, NUMERIC_type_of_ClipVarType);

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_ulock(ClipMachineMemory, wa->rd, rec, 0, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_DBUNLOCK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBUNLOCK";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int er;
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      return er;
   READLOCK;
   if ((er = rdd_ulock(ClipMachineMemory, wa->rd, 0, 1, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_DBUNLOCKALL(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBUNLOCKALL";
   DBWorkArea *wa;
   int i, er;
/* #include "ci_clipmachine.h" */

   for (i = 0; i < ClipMachineMemory->areas->count_of_ClipVect; i++)
      {
	 wa = get_area(ClipMachineMemory, i + 1, 0, 0);
	 if (!wa)
	    continue;
	 if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
	    goto err;
	 READLOCK;
	 if ((er = rdd_ulock(ClipMachineMemory, wa->rd, 0, 1, __PROC__)))
	    goto err_unlock;
	 UNLOCK;
      }
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_FLOCK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "FLOCK";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int er;
/* #include "ci_clipmachine.h" */

   if (!wa)
      return 0;

   READLOCK;
   if ((er = rdd_flock(ClipMachineMemory, wa->rd, __PROC__)))
      goto err_unlock;
   UNLOCK;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_NETERR(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "NETERR";
   int *err, r;
   int neterr = _clip_parl(ClipMachineMemory, 1);
   int er;
/* #include "ci_clipmachine.h" */

   CHECKOPT1(1, LOGICAL_type_of_ClipVarType);
   err = &ClipMachineMemory->neterr;
   r = *err;

   if (_clip_parinfo(ClipMachineMemory, 1) != UNDEF_type_of_ClipVarType)
      *err = neterr;

   _clip_retl(ClipMachineMemory, r);
   return 0;
 err:
   return er;
}

int
clip_DBCOMMIT(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBCOMMIT";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int er;
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);

   WRITELOCK;
   if ((er = rdd_commit(ClipMachineMemory, wa->rd, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_DBCOMMITALL(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBCOMMITALL";
   DBWorkArea *wa;
   int i, er;
/* #include "ci_clipmachine.h" */

   for (i = 0; i < ClipMachineMemory->areas->count_of_ClipVect; i++)
      {
	 wa = (DBWorkArea *) ClipMachineMemory->areas->items_of_ClipVect[i];
	 if (!wa || !wa->used || wa->rd->readonly)
	    continue;
	 WRITELOCK;
	 if ((er = rdd_commit(ClipMachineMemory, wa->rd, __PROC__)))
	    goto err_unlock;
	 UNLOCK;
      }
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_DBRLOCKLIST(ClipMachine * ClipMachineMemory)
{
#ifndef CLIPPER50_COMPATIBLE
   const char *__PROC__ = "DBRLOCKLIST";
#endif
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   ClipVar *rp = RETPTR(ClipMachineMemory);
   long vect[1];
   int i;
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);

   vect[0] = wa->rd->nlocks;
   _clip_array(ClipMachineMemory, rp, 1, vect);

   for (i = 0; i < wa->rd->nlocks; i++)
      {
	 ClipVar var;
	var.ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	var.ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	var.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = wa->rd->locks[i];
	 var.ClipType_t_of_ClipVar.dec_of_ClipType = 0;
	 var.ClipType_t_of_ClipVar.len_of_ClipType = 7;
	 vect[0] = i;
	 _clip_aset(ClipMachineMemory, rp, &var, 1, vect);
      }
   return 0;
}

int
clip_DBINFO(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBINFO";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int cmd = _clip_parni(ClipMachineMemory, 1);
   int er;

   CHECKWA(wa);
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
/* #include "ci_clipmachine.h" */

   switch (cmd)
      {
      case DBI_ALIAS:
	 {
	    _clip_retc(ClipMachineMemory, wa->alias);
	    break;
	 }
      case DBI_GETDELIMITER:
      case DBI_SETDELIMITER:
      case DBI_ISDBF:
      case DBI_VALIDBUFFER:
      case DBI_LOCKOFFSET:
	 {
	    if ((er = wa->rd->vtbl->info(ClipMachineMemory, wa->rd, cmd, __PROC__)))
	       goto err;
	    break;
	 }
      case DBI_BLOB_DIRECT_LEN:
      case DBI_BLOB_DIRECT_TYPE:
      case DBI_BLOB_INTEGRITY:
      case DBI_BLOB_OFFSET:
      case DBI_BLOB_RECOVER:
      case DBI_MEMOBLOCKSIZE:
	 {
	    if (wa->rd->memo)
	       if ((er = wa->rd->memo->vtbl->info(ClipMachineMemory, wa->rd, wa->rd->memo, cmd, __PROC__)))
		  goto err;
	    break;
	 }
      case DBI_BOF:
	 {
	    int bof;
	    if ((er = rdd_bof(ClipMachineMemory, wa->rd, &bof, __PROC__)))
	       goto err;
	    _clip_retl(ClipMachineMemory, bof);
	    break;
	 }
      case DBI_CANPUTREC:
	 {
	    _clip_retl(ClipMachineMemory, !wa->rd->readonly);
	    break;
	 }
      case DBI_CHILDCOUNT:
	 {
	    _clip_retni(ClipMachineMemory, wa->rd->rels_opened);
	    break;
	 }
      case DBI_DB_VERSION:
      case DBI_RDD_VERSION:
	 {
	    char ver[101];
	    int opt = _clip_parni(ClipMachineMemory, 2);
	    if (_clip_parinfo(ClipMachineMemory, 2) == NUMERIC_type_of_ClipVarType && opt == 1)
	       {
		  snprintf(ver, sizeof(ver), "%d.%d (%s)", HIWORD(wa->rd->vtbl->version), LOWORD(wa->rd->vtbl->version), wa->rd->vtbl->desc);
	       }
	    else
	       {
		  snprintf(ver, sizeof(ver), "%d.%d", HIWORD(wa->rd->vtbl->version), LOWORD(wa->rd->vtbl->version));
	       }
	    _clip_retc(ClipMachineMemory, ver);
	    break;
	 }
      case DBI_DBFILTER:
	 {
	    _clip_retc(ClipMachineMemory, "");
	    if (wa->rd->filter && wa->rd->filter->sfilter)
	       _clip_retc(ClipMachineMemory, wa->rd->filter->sfilter);
	    break;
	 }
      case DBI_DBFILTER_BLOCK:
	 {
	    ClipVar *ret = RETPTR(ClipMachineMemory);
	    _clip_dup(ClipMachineMemory, ret, &wa->rd->filter->fps->bfilter);
	    break;
	 }
      case DBI_EOF:
	 {
	    int eof;
	    if ((er = rdd_eof(ClipMachineMemory, wa->rd, &eof, __PROC__)))
	       goto err;
	    _clip_retl(ClipMachineMemory, eof);
	    break;
	 }
      case DBI_FCOUNT:
	 {
	    _clip_retni(ClipMachineMemory, wa->rd->nfields);
	    break;
	 }
      case DBI_FILEHANDLE:
	 {
	    _clip_retni(ClipMachineMemory, -1);
	    break;
	 }
      case DBI_FOUND:
	 {
	    _clip_retl(ClipMachineMemory, wa->found);
	    break;
	 }
      case DBI_FULLPATH:
	 {
	    _clip_retc(ClipMachineMemory, wa->rd->path);
	    break;
	 }
      case DBI_GETHEADERSIZE:
	 {
	    _clip_retni(ClipMachineMemory, wa->rd->hdrsize);
	    break;
	 }
      case DBI_GETLOCKARRAY:
	 {
	    return clip_DBRLOCKLIST(ClipMachineMemory);
	 }
      case DBI_GETRECSIZE:
	 {
	    _clip_retni(ClipMachineMemory, wa->rd->recsize);
	    break;
	 }
      case DBI_ISFLOCK:
	 {
	    _clip_retl(ClipMachineMemory, wa->rd->flocked);
	    break;
	 }
      case DBI_LASTUPDATE:
	 {
	    return clip_LUPDATE(ClipMachineMemory);
	 }
      case DBI_LOCKCOUNT:
	 {
	    _clip_retni(ClipMachineMemory, wa->rd->nlocks);
	    break;
	 }
      case DBI_MEMOEXT:
	 {
	    return clip_MEMOEXT(ClipMachineMemory);
	 }
      case DBI_MEMOHANDLE:
	 {
	    _clip_retni(ClipMachineMemory, -1);
	    break;
	 }
      case DBI_SHARED:
	 {
	    _clip_retl(ClipMachineMemory, wa->rd->shared);
	    break;
	 }
      case DBI_TABLEEXT:
	 {
	    return clip_TABLEEXT(ClipMachineMemory);
	 }
      }
   return 0;
 err:
   return er;
}

int
clip_DBFIELDINFO(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBFIELDINFO";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int cmd = _clip_parni(ClipMachineMemory, 1);
   int fno = _clip_parni(ClipMachineMemory, 2) - 1;
   int er;
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKARG1(2, NUMERIC_type_of_ClipVarType);

   switch (cmd)
      {
      case DBS_BLOB_LEN:
      case DBS_BLOB_OFFSET:
      case DBS_BLOB_POINTER:
      case DBS_BLOB_TYPE:
	 {
/*			if(wa->rd->memo && wa->rd->fields[fno].type=='M')
				if(wa->rd->memo->vtbl->fieldinfo(ClipMachineMemory,wa->rd,wa->rd->memo,cmd,
					__PROC__)) return 1;*/
	    break;
	 }
      case DBS_DEC:
	 {
	    _clip_retni(ClipMachineMemory, wa->rd->fields[fno].dec);
	    break;
	 }
      case DBS_LEN:
	 {
	    _clip_retni(ClipMachineMemory, wa->rd->fields[fno].len);
	    break;
	 }
      case DBS_NAME:
	 {
	    _clip_retc(ClipMachineMemory, wa->rd->fields[fno].name);
	    break;
	 }
      case DBS_TYPE:
	 {
	    char t[2];
	    t[0] = wa->rd->fields[fno].type;
	    t[1] = 0;
	    _clip_retc(ClipMachineMemory, t);
	    break;
	 }
      }
   return 0;
 err:
   return er;
}

int
clip_DBRECORDINFO(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBRECORDINFO";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int cmd = _clip_parni(ClipMachineMemory, 1);
   unsigned int rec = _clip_parni(ClipMachineMemory, 2);
   int oldrecno, er;
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, NUMERIC_type_of_ClipVarType);

   oldrecno = wa->rd->recno;

   if (rec)
      wa->rd->recno = rec;

   switch (cmd)
      {
      case DBRI_DELETED:
	 {
	    int d;
	    if ((er = rdd_deleted(ClipMachineMemory, wa->rd, &d, __PROC__)))
	       goto err;
	    _clip_retl(ClipMachineMemory, d);
	    break;
	 }
      case DBRI_LOCKED:
	 {
	    int r = 0, i;

	    for (i = 0; i < wa->rd->nlocks; i++)
	       {
		  if (wa->rd->locks[i] == wa->rd->recno)
		     {
			r = 1;
			break;;
		     }
	       }
	    _clip_retl(ClipMachineMemory, r);
	    break;
	 }
      case DBRI_RECSIZE:
	 {
	    _clip_retni(ClipMachineMemory, wa->rd->recsize);
	    break;
	 }
      case DBRI_RECNO:
	 {
	    _clip_retni(ClipMachineMemory, wa->rd->recno);
	    break;
	 }
      case DBRI_UPDATED:
	 {
	    _clip_retl(ClipMachineMemory, 0);
	    break;
	 }
      }
   wa->rd->recno = oldrecno;
   return 0;
 err:
   return er;
}

int
get_orderno(DBWorkArea * wa, ClipVar * order, ClipVar * index)
{
   int ord = -1;
   int indno = 0;

	if ((!order || (order->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType)) && (!index || (index->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType ==
UNDEF_type_of_ClipVarType)))
      return wa->rd->curord;
		if (index && (index->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType || index->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType ==
NUMERIC_type_of_ClipVarType))
      {
			if (index->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
	    {
			 indno = index->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar - 1;
	    }
	 else
	    {
			 char *i = strdup(index->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
	       char *e = i + strlen(i);
	       int l;

	       while (*(--e) == ' ');
	       *(e + 1) = 0;
	       l = e - i + 1;
	       for (indno = 0; indno < wa->rd->idxs_opened; indno++)
		  {
		     if (strcasecmp(wa->rd->indices[indno]->name, i) == 0)
			break;
		  }
	    }
	 if (indno < wa->rd->idxs_opened && indno >= 0)
	    {
	       RDD_INDEX *ri = wa->rd->indices[indno];
			 if (order && order->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
		  {
			  if (order->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar >= 1 && order->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar <= ri->norders)
				  ord = ri->orders[(int) order->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar - 1]->orderno;
		  }
        else if (order && order->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
		  {
		     int ordno;
			  char *o = strdup(order->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
		     char *e = o + strlen(o);
		     int l;

		     while (*(--e) == ' ');
		     *(e + 1) = 0;
		     l = e - o + 1;
		     for (ordno = 0; ordno < ri->norders; ordno++)
			{
			   if (strcasecmp(ri->orders[ordno]->name, o) == 0)
			      {
				 ord = ri->orders[ordno]->orderno;
				 break;
			      }
			}
		  }
	       else
		  {
		     ord = ri->orders[0]->orderno;
		  }
	    }
      }
   else if (order)
      {
			if (order->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
	    {
			 if (order->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar >= 1 && order->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar <= wa->rd->ords_opened)
				 ord = order->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar - 1;
	    }
	 else
	    {
	       int ordno;
			 char *o = strdup(order->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
	       char *e = o + strlen(o);
	       int l;

	       while (*(--e) == ' ');
	       *(e + 1) = 0;
	       l = e - o + 1;
	       for (ordno = 0; ordno < wa->rd->ords_opened; ordno++)
		  {
		     if (strcasecmp(wa->rd->orders[ordno]->name, o) == 0)
			{
			   ord = ordno;
			   break;
			}
		  }
	    }
      }
   return ord;
}

int
clip_DBORDERINFO(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBORDERINFO";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int cmd = _clip_parni(ClipMachineMemory, 1);
   ClipVar *index = _clip_par(ClipMachineMemory, 2);
   ClipVar *order = _clip_par(ClipMachineMemory, 3);
   RDD_ORDER *ro;
   int ord, er;
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT2(3, NUMERIC_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);

   ord = get_orderno(wa, order, index);
   if (ord == -1)
      ord = wa->rd->curord;
   if (ord == -1)
      return rdd_err(ClipMachineMemory, EG_NOORDER, 0, __FILE__, __LINE__, __PROC__, er_noorder);

   ro = wa->rd->orders[ord];
   switch (cmd)
      {
      case DBOI_CONDITION:
      case DBOI_CUSTOM:
      case DBOI_HPLOCKING:
      case DBOI_ISCOND:
      case DBOI_ISDESC:
      case DBOI_KEYADD:
      case DBOI_KEYDELETE:
      case DBOI_KEYGOTO:
      case DBOI_LOCKOFFSET:
      case DBOI_ORDERCOUNT:
      case DBOI_SCOPEBOTTOM:
      case DBOI_SCOPEBOTTOMCLEAR:
      case DBOI_SCOPETOP:
      case DBOI_SCOPETOPCLEAR:
      case DBOI_SKIPUNIQUE:
	 {
	    if ((er = wa->rd->orders[ord]->vtbl->info(ClipMachineMemory, wa->rd, ro, cmd, __PROC__)))
	       goto err;
	    break;
	 }
      case DBOI_EXPRESSION:
	 {
	    _clip_retc(ClipMachineMemory, ro->expr);
	    break;
	 }
      case DBOI_FILEHANDLE:
	 {
	    _clip_retni(ClipMachineMemory, -1);
	    break;
	 }
      case DBOI_INDEXEXT:
	 {
	    _clip_retc(ClipMachineMemory, ro->vtbl->suff);
	    break;
	 }
      case DBOI_INDEXNAME:
	 {
	    _clip_retc(ClipMachineMemory, ro->index->name);
	    break;
	 }
      case DBOI_KEYCOUNT:
      case DBOI_KEYSINCLUDED:
	 {
	    int c;
	    if ((er = ro->vtbl->lastkey(ClipMachineMemory, wa->rd, ro, &c, __PROC__)))
	       goto err;
	    _clip_retni(ClipMachineMemory, c);
	    break;
	 }
      case DBOI_KEYDEC:
	 {
	    _clip_retni(ClipMachineMemory, ro->dec);
	    break;
	 }
      case DBOI_KEYSIZE:
	 {
	    _clip_retni(ClipMachineMemory, ro->keysize);
	    break;
	 }
      case DBOI_KEYTYPE:
	 {
	    ClipVar vv;
	    char t[2];
	    if ((er = _clip_eval(ClipMachineMemory, &ro->block, 0, 0, &vv)))
	       goto err;
		 if (vv.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
	       t[0] = 'C';
		 else if (vv.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
	       t[0] = 'N';
		 else if (vv.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == DATE_type_of_ClipVarType)
	       t[0] = 'D';
		 else if (vv.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == LOGICAL_type_of_ClipVarType)
	       t[0] = 'L';
	    else
	       t[0] = 'U';
	    t[1] = 0;
	    _clip_destroy(ClipMachineMemory, &vv);
	    _clip_retc(ClipMachineMemory, t);
	    break;
	 }
      case DBOI_KEYVAL:
	 {
	    if ((er = _clip_eval(ClipMachineMemory, &ro->block, 0, 0, RETPTR(ClipMachineMemory))))
	       goto err;
	    break;
	 }
      case DBOI_NAME:
	 {
	    _clip_retc(ClipMachineMemory, ro->name);
	    break;
	 }
      case DBOI_NUMBER:
	 {
	    _clip_retni(ClipMachineMemory, ord + 1);
	    break;
	 }
      case DBOI_RECNO:
	 {
	    int n;
	    if ((er = ro->vtbl->keyno(ClipMachineMemory, wa->rd, ro, &n, __PROC__)))
	       goto err;
	    _clip_retni(ClipMachineMemory, n);
	    break;
	 }
      case DBOI_SETCODEBLOCK:
	 {
	    _clip_clone(ClipMachineMemory, RETPTR(ClipMachineMemory), &ro->block);
	    break;
	 }
      case DBOI_UNIQUE:
	 {
	    _clip_retl(ClipMachineMemory, ro->unique);
	    break;
	 }
      }
   return 0;
 err:
   return er;
}

int
clip_ORDSCOPE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDSCOPE";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int scope = _clip_parni(ClipMachineMemory, 1);
   ClipVar *v = _clip_par(ClipMachineMemory, 2);
   int er;
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if (scope == 0)
      {
	 if ((er = rdd_scopetop(ClipMachineMemory, wa->rd, v, __PROC__)))
	    goto err_unlock;
      }
   else if (scope == 1)
      {
	 if ((er = rdd_scopebottom(ClipMachineMemory, wa->rd, v, __PROC__)))
	    goto err_unlock;
      }
   else
      {
	 er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, "Bad argument (1)");
	 goto err_unlock;
      }
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_ORDSETFOCUS(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDSETFOCUS";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   ClipVar *order = _clip_par(ClipMachineMemory, 1);
   ClipVar *index = _clip_par(ClipMachineMemory, 2);
   int ord, er;
/* #include "ci_clipmachine.h" */

   _clip_retc(ClipMachineMemory, "");
   CHECKWA(wa);
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);

   if (wa->rd->curord != -1)
      _clip_retc(ClipMachineMemory, wa->rd->orders[wa->rd->curord]->name);

   if (!_clip_parinfo(ClipMachineMemory, 0))
      return 0;

	if ((order->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType) && (order->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar == 0))
      {
	 ord = -1;
      }
   else
      {
	 ord = get_orderno(wa, order, index);
	 if (ord < 0 || ord >= wa->rd->ords_opened || _clip_parinfo(ClipMachineMemory, 0) == 0)
	    return 0;
      }

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_setorder(ClipMachineMemory, wa->rd, ord + 1, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_ORDBAGNAME(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDBAGNAME";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   ClipVar *order = _clip_par(ClipMachineMemory, 1);
   int ord, er;
/* #include "ci_clipmachine.h" */

   _clip_retc(ClipMachineMemory, "");
   CHECKWA(wa);
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);

   ord = get_orderno(wa, order, NULL);
   if (ord == -1)
      {
			if (_clip_parinfo(ClipMachineMemory, 1) != NUMERIC_type_of_ClipVarType || order->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar == 0)
	    ord = wa->rd->curord;
      }
   if (ord == -1)
      return 0;

   _clip_retc(ClipMachineMemory, wa->rd->orders[ord]->index->name);
   return 0;
 err:
   return er;
}

int
clip_ORDCONDSET(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDCONDSET";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   const char *cForCondition = _clip_parc(ClipMachineMemory, 1);
   ClipVar *bForCondition = _clip_spar(ClipMachineMemory, 2);
   int lAll = _clip_parl(ClipMachineMemory, 3);
   ClipVar *bWhileCondition = _clip_spar(ClipMachineMemory, 4);
   ClipVar *bEval = _clip_spar(ClipMachineMemory, 5);
   int nInterval = _clip_parni(ClipMachineMemory, 6);
   int nStart = _clip_parni(ClipMachineMemory, 7);
   int nNext = _clip_parni(ClipMachineMemory, 8);
   int nRecord = _clip_parni(ClipMachineMemory, 9);
   int lRest = _clip_parl(ClipMachineMemory, 10);
   int lDescend = _clip_parl(ClipMachineMemory, 11);
   int lAdditive = _clip_parl(ClipMachineMemory, 13);
   int lCurrent = _clip_parl(ClipMachineMemory, 14);
   int lCustom = _clip_parl(ClipMachineMemory, 15);
   int lNoOptimize = _clip_parl(ClipMachineMemory, 16);
   int lIgnoreCase = _clip_parl(ClipMachineMemory, 17);
   int er;
/* #include "ci_clipmachine.h" */

   if (!wa)
      return 0;

   CHECKOPT1(1, CHARACTER_type_of_ClipVarType);
   CHECKOPT2(2, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);
   CHECKOPT1(3, LOGICAL_type_of_ClipVarType);
   CHECKOPT2(4, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);
   CHECKOPT2(5, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);
   CHECKOPT1(6, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(7, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(8, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(9, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(10, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(11, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(13, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(14, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(15, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(16, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(17, LOGICAL_type_of_ClipVarType);

   if (wa->rd->os.cForCondition)
      {
	 free(wa->rd->os.cForCondition);
	 wa->rd->os.cForCondition = NULL;
      }
   _clip_destroy(ClipMachineMemory, &wa->rd->os.bForCondition);
   _clip_destroy(ClipMachineMemory, &wa->rd->os.bWhileCondition);
   _clip_destroy(ClipMachineMemory, &wa->rd->os.bEval);

   if (cForCondition)
      {
	 wa->rd->os.cForCondition = malloc(strlen(cForCondition) + 1);
	 strcpy(wa->rd->os.cForCondition, cForCondition);
      }

   if (bForCondition)
      _clip_clone(ClipMachineMemory, &wa->rd->os.bForCondition, bForCondition);
   wa->rd->os.lAll = lAll;
   if (bWhileCondition)
      _clip_clone(ClipMachineMemory, &wa->rd->os.bWhileCondition, bWhileCondition);
   if (bEval)
      _clip_clone(ClipMachineMemory, &wa->rd->os.bEval, bEval);
   wa->rd->os.nInterval = nInterval;
   wa->rd->os.nStart = (lAll ? 0 : nStart);
   wa->rd->os.nNext = nNext;
   wa->rd->os.nRecord = nRecord;
   wa->rd->os.lRest = lRest;
   wa->rd->os.lDescend = lDescend;
   wa->rd->os.lAdditive = lAdditive;
   wa->rd->os.lCurrent = lCurrent;
   wa->rd->os.lCustom = lCustom;
   wa->rd->os.lNoOptimize = lNoOptimize;
   wa->rd->os.lIgnoreCase = lIgnoreCase;

   _clip_retl(ClipMachineMemory, 1);
   return 0;
 err:
   return er;
}

int
clip_M6_ORDCONDSET(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_ORDCONDSET";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   const char *cForCondition = _clip_parc(ClipMachineMemory, 1);
   ClipVar *bForCondition = _clip_spar(ClipMachineMemory, 2);
   int lAll = _clip_parl(ClipMachineMemory, 3);
   ClipVar *bWhileCondition = _clip_spar(ClipMachineMemory, 4);
   ClipVar *bEval = _clip_spar(ClipMachineMemory, 5);
   int nInterval = _clip_parni(ClipMachineMemory, 6);
   int nStart = _clip_parni(ClipMachineMemory, 7);
   int nNext = _clip_parni(ClipMachineMemory, 8);
   int nRecord = _clip_parni(ClipMachineMemory, 9);
   int lRest = _clip_parl(ClipMachineMemory, 10);
   int lDescend = _clip_parl(ClipMachineMemory, 11);
   int lCurrent = _clip_parl(ClipMachineMemory, 14);
   int lAdditive = _clip_parl(ClipMachineMemory, 17);
   int lCustom = _clip_parl(ClipMachineMemory, 18);
   int lNoOptimize = _clip_parl(ClipMachineMemory, 19);
   int er;
/* #include "ci_clipmachine.h" */

   ClipMachineMemory->m6_error = 0;
   if (!wa)
      return 0;

   CHECKOPT1(1, CHARACTER_type_of_ClipVarType);
   CHECKOPT2(2, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);
   CHECKOPT1(3, LOGICAL_type_of_ClipVarType);
   CHECKOPT2(4, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);
   CHECKOPT2(5, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);
   CHECKOPT1(6, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(7, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(8, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(9, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(10, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(11, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(14, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(17, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(18, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(19, LOGICAL_type_of_ClipVarType);

   if (wa->rd->os.cForCondition)
      {
	 free(wa->rd->os.cForCondition);
	 wa->rd->os.cForCondition = NULL;
      }
   _clip_destroy(ClipMachineMemory, &wa->rd->os.bForCondition);
   _clip_destroy(ClipMachineMemory, &wa->rd->os.bWhileCondition);
   _clip_destroy(ClipMachineMemory, &wa->rd->os.bEval);

   if (cForCondition)
      {
	 wa->rd->os.cForCondition = malloc(strlen(cForCondition) + 1);
	 strcpy(wa->rd->os.cForCondition, cForCondition);
      }

   if (bForCondition)
      _clip_clone(ClipMachineMemory, &wa->rd->os.bForCondition, bForCondition);
   wa->rd->os.lAll = lAll;
   if (bWhileCondition)
      _clip_clone(ClipMachineMemory, &wa->rd->os.bWhileCondition, bWhileCondition);
   if (bEval)
      _clip_clone(ClipMachineMemory, &wa->rd->os.bEval, bEval);
   wa->rd->os.nInterval = nInterval;
   wa->rd->os.nStart = (lAll ? 0 : nStart);
   wa->rd->os.nNext = nNext;
   wa->rd->os.nRecord = nRecord;
   wa->rd->os.lRest = lRest;
   wa->rd->os.lDescend = lDescend;
   wa->rd->os.lAdditive = lAdditive;
   wa->rd->os.lCurrent = lCurrent;
   wa->rd->os.lCustom = lCustom;
   wa->rd->os.lNoOptimize = lNoOptimize;

   _clip_retl(ClipMachineMemory, 1);
   return 0;
 err:
   return er;
}

int
clip_FIELDDECI(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "FIELDDECI";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int fno = _clip_parni(ClipMachineMemory, 1) - 1;
   int dec = 0, er;
/* #include "ci_clipmachine.h" */

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   if (wa)
      {
	 if (fno >= 0 && fno < wa->rd->nfields)
	    dec = wa->rd->fields[fno].dec;
      }
   _clip_retni(ClipMachineMemory, dec);
   return 0;
 err:
   return er;
}

int
clip_FIELDNUM(ClipMachine * ClipMachineMemory)
{
/* #include "ci_clipmachine.h" */
   return clip_FIELDPOS(ClipMachineMemory);
}

int
clip_FIELDSIZE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "FIELDSIZE";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int fno = _clip_parni(ClipMachineMemory, 1) - 1;
   int len = 0, er;
/* #include "ci_clipmachine.h" */

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   if (wa)
      {
	 if (fno >= 0 && fno < wa->rd->nfields)
	    len = wa->rd->fields[fno].len;
      }
   _clip_retni(ClipMachineMemory, len);
   return 0;
 err:
   return er;
}

int
clip_FIELDTYPE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "FIELDTYPE";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int fno = _clip_parni(ClipMachineMemory, 1) - 1;
   char type[2] = " ";
   int er;
/* #include "ci_clipmachine.h" */

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   if (wa)
      {
	 if (fno >= 0 && fno < wa->rd->nfields)
	    type[0] = wa->rd->fields[fno].type;
      }

   _clip_retc(ClipMachineMemory, type);
   return 0;
 err:
   return er;
}

int
clip_FIELDBLANK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "FIELDBLANK";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int fno = _clip_parni(ClipMachineMemory, 1) - 1;
   ClipVar *r = RETPTR(ClipMachineMemory);
   int er;
/* #include "ci_clipmachine.h" */

   _clip_retc(ClipMachineMemory, "");
   CHECKWA(wa);
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   if (fno < 0 || fno >= wa->rd->nfields)
      return rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_nofield);

   if (strchr("MPG", wa->rd->fields[fno].type))
      return 0;

   if ((!wa->rd->valid || wa->rd->pending_child_parent) && !(wa->rd->file.rlocked || wa->rd->file.wlocked))
      {

	 READLOCK;
	 if ((er = rdd_takevalue(ClipMachineMemory, wa->rd, fno, r, __PROC__)))
	    goto err_unlock;
	 UNLOCK;
      }
   else
      {
	 if ((er = rdd_takevalue(ClipMachineMemory, wa->rd, fno, r, __PROC__)))
	    goto err;
      }
	   if (r->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
      {
			memset(r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, ' ', r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
      }
	   else if (r->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
      {
			r->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = 0;
      }
	   else if (r->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == DATE_type_of_ClipVarType)
      {
			r->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = 0;
      }
	   else if (r->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == LOGICAL_type_of_ClipVarType)
      {
			r->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = 0;
      }
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_ORDKEYADD(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDKEYADD";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   ClipVar *order = _clip_par(ClipMachineMemory, 1);
   ClipVar *index = _clip_par(ClipMachineMemory, 2);
   ClipVar *v = _clip_par(ClipMachineMemory, 3);
   int ord, er;
/* #include "ci_clipmachine.h" */

   _clip_retl(ClipMachineMemory, 0);
   CHECKWA(wa);
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);

   ord = get_orderno(wa, order, index);
   if (ord == -1)
      return rdd_err(ClipMachineMemory, EG_NOORDER, 0, __FILE__, __LINE__, __PROC__, er_noorder);

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_keyadd(ClipMachineMemory, wa->rd, wa->rd->orders[ord], v, __PROC__)))
      goto err_unlock;
   UNLOCK;
   _clip_retl(ClipMachineMemory, 1);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_ORDKEYDEL(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDKEYDEL";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   ClipVar *order = _clip_par(ClipMachineMemory, 1);
   ClipVar *index = _clip_par(ClipMachineMemory, 2);
   int ord, er;
/* #include "ci_clipmachine.h" */

   _clip_retl(ClipMachineMemory, 0);
   CHECKWA(wa);
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);

   ord = get_orderno(wa, order, index);
   if (ord == -1)
      return rdd_err(ClipMachineMemory, EG_NOORDER, 0, __FILE__, __LINE__, __PROC__, er_noorder);

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_keydel(ClipMachineMemory, wa->rd, wa->rd->orders[ord], __PROC__)))
      goto err_unlock;
   UNLOCK;
   _clip_retl(ClipMachineMemory, 1);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_ORDDESCEND(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDDESCEND";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   ClipVar *order = _clip_par(ClipMachineMemory, 1);
   ClipVar *index = _clip_par(ClipMachineMemory, 2);
   int descend = _clip_parl(ClipMachineMemory, 3);
   int res, ord, er;
/* #include "ci_clipmachine.h" */

   _clip_retl(ClipMachineMemory, 0);
   CHECKWA(wa);
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(3, LOGICAL_type_of_ClipVarType);

   ord = get_orderno(wa, order, index);
   if (ord == -1)
      return rdd_err(ClipMachineMemory, EG_NOORDER, 0, __FILE__, __LINE__, __PROC__, er_noorder);

   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      descend = -1;
   if ((er = wa->rd->orders[ord]->vtbl->descend(ClipMachineMemory, wa->rd->orders[ord], descend, &res, __PROC__)))
      goto err;
   _clip_retl(ClipMachineMemory, res);
   return 0;
 err:
   return er;
}

int
clip_ORDDESTROY(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDDESTROY";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   ClipVar *order = _clip_par(ClipMachineMemory, 1);
   ClipVar *index = _clip_par(ClipMachineMemory, 2);
   int ord, er;
/* #include "ci_clipmachine.h" */

   if (!wa)
      return 0;

   CHECKARG1(1, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);

   ord = get_orderno(wa, order, index);
   if (ord == -1)
      return 0;

   if ((er = rdd_orddestroy(ClipMachineMemory, wa->rd, wa->rd->orders[ord], __PROC__)))
      goto err;
   return 0;
 err:
   return er;
}

int
clip_ORDFOR(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDFOR";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   ClipVar *order = _clip_par(ClipMachineMemory, 1);
   ClipVar *index = _clip_par(ClipMachineMemory, 2);
   int ord, er;
/* #include "ci_clipmachine.h" */

   _clip_retc(ClipMachineMemory, "");
   CHECKWA(wa);
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);

   ord = get_orderno(wa, order, index);
   if (ord == -1)
      ord = wa->rd->curord;
   if (ord == -1)
      return 0;

   _clip_retc(ClipMachineMemory, wa->rd->orders[ord]->cforexpr);
   return 0;
 err:
   return er;
}

int
clip_ORDISUNIQUE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDISUNIQUE";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   ClipVar *order = _clip_par(ClipMachineMemory, 1);
   ClipVar *index = _clip_par(ClipMachineMemory, 2);
   int ord, er;
/* #include "ci_clipmachine.h" */

   _clip_retl(ClipMachineMemory, 0);
   CHECKWA(wa);
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);

   ord = get_orderno(wa, order, index);
   if (ord == -1)
      return rdd_err(ClipMachineMemory, EG_NOORDER, 0, __FILE__, __LINE__, __PROC__, er_noorder);

   _clip_retl(ClipMachineMemory, wa->rd->orders[ord]->unique);
   return 0;
 err:
   return er;
}

int
clip_ORDKEY(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDKEY";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int ord = _clip_parni(ClipMachineMemory, 1);
   ClipVar *order = _clip_par(ClipMachineMemory, 1);
   ClipVar *index = _clip_par(ClipMachineMemory, 2);
   int er;
/* #include "ci_clipmachine.h" */

   _clip_retc(ClipMachineMemory, "");
   CHECKWA(wa);
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 1) == NUMERIC_type_of_ClipVarType && !ord)
      ord = wa->rd->curord;
   else
      ord = get_orderno(wa, order, index);

   if (!(ord < 0 || ord >= wa->rd->ords_opened))
      _clip_retc(ClipMachineMemory, wa->rd->orders[ord]->expr);
   return 0;
 err:
   return er;
}

int
clip_ORDKEYCOUNT(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDKEYCOUNT";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   ClipVar *order = _clip_par(ClipMachineMemory, 1);
   ClipVar *index = _clip_par(ClipMachineMemory, 2);
   int ord = _clip_parni(ClipMachineMemory, 1);
   int r, er;
/* #include "ci_clipmachine.h" */

   _clip_retni(ClipMachineMemory, 0);
   CHECKWA(wa);
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 1) == NUMERIC_type_of_ClipVarType && !ord)
      ord = wa->rd->curord;
   else
      ord = get_orderno(wa, order, index);
   if (ord == -1)
      return clip_LASTREC(ClipMachineMemory);

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_checkifnew(ClipMachineMemory, wa->rd, __PROC__)))
      goto err_unlock;
   if ((er = wa->rd->orders[ord]->vtbl->lastkey(ClipMachineMemory, wa->rd, wa->rd->orders[ord], &r, __PROC__)))
      goto err_unlock;
   UNLOCK;

   _clip_retni(ClipMachineMemory, r);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_ORDKEYGOTO(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDKEYGOTO";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   unsigned int nkey = _clip_parni(ClipMachineMemory, 1);
   int ok, er;
/* #include "ci_clipmachine.h" */

   _clip_retl(ClipMachineMemory, 0);
   if (!wa)
      return 0;

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   if (wa->rd->curord == -1)
      return 0;

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_gotokey(ClipMachineMemory, wa->rd, wa->rd->orders[wa->rd->curord], nkey, &ok, __PROC__)))
      goto err_unlock;
   UNLOCK;
   _clip_retl(ClipMachineMemory, ok);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_ORDKEYNO(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDKEYNO";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   ClipVar *order = _clip_par(ClipMachineMemory, 1);
   ClipVar *index = _clip_par(ClipMachineMemory, 2);
   int ord = _clip_parni(ClipMachineMemory, 1);
   int r, er;
/* #include "ci_clipmachine.h" */

   _clip_retni(ClipMachineMemory, 0);
   CHECKWA(wa);
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 1) == NUMERIC_type_of_ClipVarType && !ord)
      ord = wa->rd->curord;
   else
      ord = get_orderno(wa, order, index);
   if (ord == -1)
      {
	 _clip_retni(ClipMachineMemory, wa->rd->recno);
	 return 0;
      }

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_checkifnew(ClipMachineMemory, wa->rd, __PROC__)))
      goto err_unlock;
   if ((er = wa->rd->orders[ord]->vtbl->keyno(ClipMachineMemory, wa->rd, wa->rd->orders[ord], &r, __PROC__)))
      goto err_unlock;
   UNLOCK;

   _clip_retni(ClipMachineMemory, r);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_ORDKEYVAL(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDKEYVAL";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int er;
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);

   READLOCK;
   if ((er = rdd_keyvalue(ClipMachineMemory, wa->rd, RETPTR(ClipMachineMemory), __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_ORDLISTADD(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDLISTADD";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   const char *bag = _clip_parc(ClipMachineMemory, 1);
   const char *ord = _clip_parc(ClipMachineMemory, 2);
   int er;
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);
   CHECKARG1(1, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);

   READLOCK;
   if ((er = rdd_setindex(ClipMachineMemory, wa->rd, NULL, wa->idx_driver, bag, ord, 0, __PROC__)))
      goto err_unlock;
   if ((er = rdd_gotop(ClipMachineMemory, wa->rd, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_ORDLISTCLEAR(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDLISTCLEAR";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);
   return rdd_clearindex(ClipMachineMemory, wa->rd, __PROC__);
}

int
clip_ORDLISTREBUILD(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDLISTREBUILD";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int er;
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);
   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   WRITELOCK;
   if ((er = rdd_reindex(ClipMachineMemory, wa->rd, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_ORDNAME(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDNAME";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int ord = _clip_parni(ClipMachineMemory, 1);
   ClipVar *order = _clip_par(ClipMachineMemory, 1);
   ClipVar *index = _clip_par(ClipMachineMemory, 2);
   int er;
/* #include "ci_clipmachine.h" */

   _clip_retc(ClipMachineMemory, "");
   CHECKWA(wa);
   CHECKOPT1(1, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);

   if (!ord)
      ord = wa->rd->curord;
   else
      ord = get_orderno(wa, order, index);
   if (ord == -1)
      return 0;

   _clip_retc(ClipMachineMemory, wa->rd->orders[ord]->name);
   return 0;
 err:
   return er;
}

int
clip_ORDNUMBER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDNUMBER";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   ClipVar *order = _clip_par(ClipMachineMemory, 1);
   ClipVar *index = _clip_par(ClipMachineMemory, 2);
   int ord, er;
/* #include "ci_clipmachine.h" */

   if (!_clip_parinfo(ClipMachineMemory, 0))
      _clip_retni(ClipMachineMemory, wa->rd->curord + 1);
   else
      _clip_retni(ClipMachineMemory, 0);

   CHECKWA(wa);
   CHECKOPT1(1, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);

   ord = get_orderno(wa, order, index);
   if (ord == -1)
      return 0;

   _clip_retni(ClipMachineMemory, ord + 1);
   return 0;
 err:
   return er;
}

int
clip_ORDSETRELATION(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDSETRELATION";
   int type = _clip_parinfo(ClipMachineMemory, 1);
   ClipVar *block = _clip_spar(ClipMachineMemory, 2);
   const char *expr = _clip_parc(ClipMachineMemory, 3);
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   DBWorkArea *childwa = 0;
   int er;
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);
   CHECKARG2(1, NUMERIC_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   CHECKARG2(2, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);
   CHECKOPT1(3, CHARACTER_type_of_ClipVarType);

   if (type == NUMERIC_type_of_ClipVarType)
      {
	 childwa = get_area(ClipMachineMemory, _clip_parni(ClipMachineMemory, 1), 0, 0);
      }
   else if (type == CHARACTER_type_of_ClipVarType)
      {
	 int i;
	 DBWorkArea *twa;
	 const char *alias = _clip_parc(ClipMachineMemory, 1);
	 for (i = 0; i < ClipMachineMemory->areas->count_of_ClipVect; i++)
	    {
	       twa = (DBWorkArea *) ClipMachineMemory->areas->items_of_ClipVect[i];
	       if (twa && twa->alias && !strcasecmp(alias, twa->alias))
		  break;
	    }
	 childwa = get_area(ClipMachineMemory, i + 1, 0, 0);
      }

   CHECKWA(childwa);

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_setrelation(ClipMachineMemory, wa->rd, childwa->rd, block, expr, 1, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_ORDSKIPUNIQUE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDSKIPUNIQUE";
   int recs = _clip_parni(ClipMachineMemory, 1);
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   ClipVar prev;
   ClipVar curr;
   int c, er;
/* #include "ci_clipmachine.h" */

   _clip_retl(ClipMachineMemory, 0);
   CHECKWA(wa);
   CHECKOPT1(1, NUMERIC_type_of_ClipVarType);

   if (recs >= 0)
      recs = 1;
   else
      recs = -1;

   _clip_retl(ClipMachineMemory, 1);

   READLOCK;
   if ((er = rdd_keyvalue(ClipMachineMemory, wa->rd, &prev, __PROC__)))
      goto err_unlock;
   while (1)
      {
	 if (wa->rd->bof || wa->rd->eof)
	    {
	       UNLOCK;
	       return 0;
	    }
	 if ((er = rdd_skip(ClipMachineMemory, wa->rd, recs, __PROC__)))
	    goto err_unlock;
	 if ((er = rdd_keyvalue(ClipMachineMemory, wa->rd, &curr, __PROC__)))
	    goto err_unlock;
	 _clip_cmp(ClipMachineMemory, &prev, &curr, &c, 1);
	 _clip_destroy(ClipMachineMemory, &prev);
	 _clip_clone(ClipMachineMemory, &prev, &curr);
	 if (!c)
	    break;
      }
   UNLOCK;
   _clip_destroy(ClipMachineMemory, &prev);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_DBRSELECT(ClipMachine * ClipMachineMemory)
{
   int rno = _clip_parni(ClipMachineMemory, 1) - 1;
   DBWorkArea *wa = cur_area(ClipMachineMemory);
/* #include "ci_clipmachine.h" */

   _clip_retni(ClipMachineMemory, 0);
   if (!wa)
      return 0;

   if (rno < 0 || rno >= wa->rd->rels_opened)
      return 0;
   _clip_retni(ClipMachineMemory, wa->rd->relations[rno]->child->area + 1);
   return 0;
}

/* ] user interface */

/* [ Independed Index */
int
clip_II_CREATE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "II_CREATE";
   const char *driver = _clip_parc(ClipMachineMemory, 1);
   const char *name = _clip_parc(ClipMachineMemory, 2);
   int er;
/* #include "ci_clipmachine.h" */

   CHECKOPT1(1, CHARACTER_type_of_ClipVarType);
   CHECKARG1(2, CHARACTER_type_of_ClipVarType);

   if (driver && driver[0])
      {
	 if ((er = rdd_ii_create(ClipMachineMemory, driver, name, __PROC__)))
	    goto err;
      }
   else
      {
	 char idx[4];
	 if ((er = db_driver(ClipMachineMemory, NULL, NULL, idx, NULL, __PROC__)))
	    goto err;
	 if ((er = rdd_ii_create(ClipMachineMemory, idx, name, __PROC__)))
	    goto err;
      }

   return 0;
 err:
   return er;
}

int
clip_II_OPEN(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "II_OPEN";
   const char *driver = _clip_parc(ClipMachineMemory, 1);
   const char *name = _clip_parc(ClipMachineMemory, 2);
   int er;
/* #include "ci_clipmachine.h" */

   CHECKOPT1(1, CHARACTER_type_of_ClipVarType);
   CHECKARG1(2, CHARACTER_type_of_ClipVarType);

   if (driver && driver[0])
      {
	 if ((er = rdd_ii_open(ClipMachineMemory, driver, name, __PROC__)))
	    goto err;
      }
   else
      {
	 char idx[4];
	 if ((er = db_driver(ClipMachineMemory, NULL, NULL, idx, NULL, __PROC__)))
	    goto err;
	 if ((er = rdd_ii_open(ClipMachineMemory, idx, name, __PROC__)))
	    goto err;
      }
   return 0;
 err:
   return er;
}

int
clip_II_CREATETAG(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "II_CREATETAG";
   int h = _clip_parni(ClipMachineMemory, 1);
   const char *tag = _clip_parc(ClipMachineMemory, 2);
   const char *expr = _clip_parc(ClipMachineMemory, 3);
   int er;
/* #include "ci_clipmachine.h" */

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKARG1(2, CHARACTER_type_of_ClipVarType);
   CHECKARG1(3, CHARACTER_type_of_ClipVarType);

   if ((er = rdd_ii_createtag(ClipMachineMemory, h, tag, expr, __PROC__)))
      goto err;
   return 0;
 err:
   return er;
}

int
clip_II_OPENTAG(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "II_OPENTAG";
   int h = _clip_parni(ClipMachineMemory, 1);
   const char *tag = _clip_parc(ClipMachineMemory, 2);
   int er;
/* #include "ci_clipmachine.h" */

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKARG1(2, CHARACTER_type_of_ClipVarType);

   if ((er = rdd_ii_opentag(ClipMachineMemory, h, tag, __PROC__)))
      goto err;
   return 0;
 err:
   return er;
}

int
clip_II_CLOSE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "II_CLOSE";
   int h = _clip_parni(ClipMachineMemory, 1);
   int er;
/* #include "ci_clipmachine.h" */

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   if ((er = rdd_ii_close(ClipMachineMemory, h, __PROC__)))
      goto err;
   return 0;
 err:
   return er;
}

int
clip_II_CLOSETAG(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "II_CLOSETAG";
   int h = _clip_parni(ClipMachineMemory, 1);
   int er;
/* #include "ci_clipmachine.h" */

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   if ((er = rdd_ii_closetag(ClipMachineMemory, h, __PROC__)))
      goto err;
   return 0;
 err:
   return er;
}

int
clip_II_ADDKEY(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "II_ADDKEY";
   int h = _clip_parni(ClipMachineMemory, 1);
   const char *id = _clip_parc(ClipMachineMemory, 2);
   ClipVar *key = _clip_par(ClipMachineMemory, 3);
   int er;
/* #include "ci_clipmachine.h" */

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKARG1(2, CHARACTER_type_of_ClipVarType);

   if ((er = rdd_ii_addkey(ClipMachineMemory, h, id, key, __PROC__)))
      goto err;
   return 0;
 err:
   return er;
}

int
clip_II_DELKEY(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "II_DELKEY";
   int h = _clip_parni(ClipMachineMemory, 1);
   int er;
/* #include "ci_clipmachine.h" */

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   if ((er = rdd_ii_delkey(ClipMachineMemory, h, __PROC__)))
      goto err;
   return 0;
 err:
   return er;
}

int
clip_II_GOTOP(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "II_GOTOP";
   int h = _clip_parni(ClipMachineMemory, 1);
   int er;
/* #include "ci_clipmachine.h" */

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   if ((er = rdd_ii_gotop(ClipMachineMemory, h, __PROC__)))
      goto err;
   return 0;
 err:
   return er;
}

int
clip_II_GOBOTTOM(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "II_GOBOTTOM";
   int h = _clip_parni(ClipMachineMemory, 1);
   int er;
/* #include "ci_clipmachine.h" */

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   if ((er = rdd_ii_gobottom(ClipMachineMemory, h, __PROC__)))
      goto err;
   return 0;
 err:
   return er;
}

int
clip_II_BOF(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "II_BOF";
   int h = _clip_parni(ClipMachineMemory, 1);
   int er;
/* #include "ci_clipmachine.h" */

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   if ((er = rdd_ii_bof(ClipMachineMemory, h, __PROC__)))
      goto err;
   return 0;
 err:
   return er;
}

int
clip_II_EOF(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "II_EOF";
   int h = _clip_parni(ClipMachineMemory, 1);
   int er;
/* #include "ci_clipmachine.h" */

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   if ((er = rdd_ii_eof(ClipMachineMemory, h, __PROC__)))
      goto err;
   return 0;
 err:
   return er;
}

int
clip_II_ID(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "II_ID";
   int h = _clip_parni(ClipMachineMemory, 1);
   int er;
/* #include "ci_clipmachine.h" */

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   if ((er = rdd_ii_id(ClipMachineMemory, h, __PROC__)))
      goto err;
   return 0;
 err:
   return er;
}

int
clip_II_KEY(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "II_KEY";
   int h = _clip_parni(ClipMachineMemory, 1);
   int er;
/* #include "ci_clipmachine.h" */

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   if ((er = rdd_ii_key(ClipMachineMemory, h, __PROC__)))
      goto err;
   return 0;
 err:
   return er;
}

int
clip_II_SKIP(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "II_SKIP";
   int h = _clip_parni(ClipMachineMemory, 1);
   int recs = _clip_parni(ClipMachineMemory, 2);
   int er;
/* #include "ci_clipmachine.h" */

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, NUMERIC_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      recs = 1;

   if ((er = rdd_ii_skip(ClipMachineMemory, h, recs, __PROC__)))
      goto err;
   return 0;
 err:
   return er;
}

/* ] Independed Index */

int
clip_RDDHANDLE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDHANDLE";
   int area = _clip_parni(ClipMachineMemory, 1);
   DBWorkArea *wa = NULL;
   int er;
/* #include "ci_clipmachine.h" */

   CHECKOPT1(1, NUMERIC_type_of_ClipVarType);

   _clip_retni(ClipMachineMemory, -1);
   if (_clip_parinfo(ClipMachineMemory, 1) != UNDEF_type_of_ClipVarType)
      wa = get_area(ClipMachineMemory, area, 0, 0);
   if (!wa)
      wa = cur_area(ClipMachineMemory);
   if (wa)
      _clip_retni(ClipMachineMemory, wa->rd_item);
   return 0;
 err:
   return er;
}

int
clip_GETAREA(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "GETAREA";
   const char *fname = _clip_parc(ClipMachineMemory, 1);
   const char *driver = _clip_parc(ClipMachineMemory, 2);
   RDD_DATA_VTBL *vtbl;
   char *path = NULL;
   DBWorkArea *wa;
   int i, er = EG_UNSUPPORTED;
/* #include "ci_clipmachine.h" */

   CHECKARG1(1, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);

   _clip_retni(ClipMachineMemory, 0);
   if (!(vtbl = rdd_datadriver(ClipMachineMemory, driver, __PROC__)))
      goto err;

   if ((er = _rdd_parsepath(ClipMachineMemory, fname, vtbl->suff, &path, NULL, EG_OPEN, __PROC__)))
      goto err;
   for (i = 0; i < ClipMachineMemory->areas->count_of_ClipVect; i++)
      {
	 wa = ClipMachineMemory->areas->items_of_ClipVect[i];
	 if (!wa || !wa->used)
	    continue;
	 if (strcmp(wa->rd->path, path) == 0)
	    {
	       _clip_retni(ClipMachineMemory, i + 1);
	       break;
	    }
      }
   free(path);
   return 0;
 err:
   if (path)
      free(path);
   return er;
}

int
clip_GETALIAS(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "GETALIAS";
   char *alias = _clip_parc(ClipMachineMemory, 1);
   char *ch;
   DBWorkArea *wa;
   int i, er = EG_UNSUPPORTED;
/* #include "ci_clipmachine.h" */

   CHECKARG1(1, CHARACTER_type_of_ClipVarType);

   _clip_retni(ClipMachineMemory, 0);
   for (ch = alias; *ch; ch++)
      *ch = toupper(*ch);
   for (i = 0; i < ClipMachineMemory->areas->count_of_ClipVect; i++)
      {
	 wa = ClipMachineMemory->areas->items_of_ClipVect[i];
	 if (!wa || !wa->used)
	    continue;
	 if (strcmp(wa->alias, alias) == 0)
	    {
	       _clip_retni(ClipMachineMemory, i + 1);
	       break;
	    }
      }
   return 0;
 err:
   return er;
}

int
clip___DBCOPYRAW(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "__RDDCOPY";
   int destwa = _clip_parni(ClipMachineMemory, 1);
   ClipVar *bfor = _clip_spar(ClipMachineMemory, 2);
   ClipVar *bwhile = _clip_spar(ClipMachineMemory, 3);
   int next = _clip_parni(ClipMachineMemory, 4);
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   DBWorkArea *dwa;
   void *buf = NULL;
   ClipVar v, *vp;
   int cnt = 0, er;
/* #include "ci_clipmachine.h" */

   _clip_retni(ClipMachineMemory, 0);
   CHECKWA(wa);
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   dwa = get_area(ClipMachineMemory, destwa, 0, 0);
   CHECKWA(dwa);
   CHECKOPT2(2, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);
   CHECKOPT2(3, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);
   CHECKOPT1(4, NUMERIC_type_of_ClipVarType);

	if (bfor && bfor->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType)
      bfor = NULL;
	if (bwhile && bwhile->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType)
      bwhile = NULL;

   buf = malloc(wa->rd->recsize);
   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if (dwa->rd->vtbl->_wlock(ClipMachineMemory, dwa->rd, __PROC__))
      goto err_unlock;
   while (!wa->rd->eof)
      {
	 if (bwhile)
	    {
	       if ((er = rdd_calc(ClipMachineMemory, wa->rd->area, bwhile, &v, 0)))
		  goto err_unlock;
	       vp = _clip_vptr(&v);
			 if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
		  {
		     er = rdd_err(ClipMachineMemory, EG_SYNTAX, 0, __FILE__, __LINE__, __PROC__, er_badforexpr);
		     goto err_unlock;
		  }
        if (!vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
		  {
		     _clip_destroy(ClipMachineMemory, &v);
		     break;
		  }
	       _clip_destroy(ClipMachineMemory, &v);
	    }
	 if (bfor)
	    {
	       if ((er = rdd_calc(ClipMachineMemory, wa->rd->area, bfor, &v, 0)))
		  goto err_unlock;
	       vp = _clip_vptr(&v);
			 if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
		  {
		     er = rdd_err(ClipMachineMemory, EG_SYNTAX, 0, __FILE__, __LINE__, __PROC__, er_badforexpr);
		     goto err_unlock;
		  }
        if (!vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
		  {
		     _clip_destroy(ClipMachineMemory, &v);
		     if ((er = rdd_skip(ClipMachineMemory, wa->rd, 1, __PROC__)))
			goto err_unlock;
		     continue;
		  }
	       _clip_destroy(ClipMachineMemory, &v);
	    }
	 if ((er = rdd_rawread(ClipMachineMemory, wa->rd, buf, __PROC__)))
	    goto err_unlock;
	 if ((er = rdd_rawwrite(ClipMachineMemory, dwa->rd, buf, 1, __PROC__)))
	    goto err_unlock;
	 if (++cnt >= next)
	    break;
	 if ((er = rdd_skip(ClipMachineMemory, wa->rd, 1, __PROC__)))
	    goto err_unlock;
      }
   if (wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__))
      goto err_unlock;
   if (dwa->rd->vtbl->_ulock(ClipMachineMemory, dwa->rd, __PROC__))
      goto err;
   free(buf);
   _clip_retni(ClipMachineMemory, cnt);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
   dwa->rd->vtbl->_ulock(ClipMachineMemory, dwa->rd, __PROC__);
 err:
   if (buf)
      free(buf);
   return er;
}

int
clip___DBSETFOUND(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "__DBSETFOUND";
   int f = _clip_parl(ClipMachineMemory, 1);
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int er;
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);
   CHECKOPT1(1, LOGICAL_type_of_ClipVarType);

   _clip_retl(ClipMachineMemory, wa->found ? 1 : 0);
   if (_clip_parinfo(ClipMachineMemory, 1) == LOGICAL_type_of_ClipVarType)
      wa->found = f;
   return 0;
 err:
   return er;
}

int
clip___DBSETFOU(ClipMachine * ClipMachineMemory)
{
/* #include "ci_clipmachine.h" */
   return clip___DBSETFOUND(ClipMachineMemory);
}

int
clip_CLIP_DBFTYPE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "CLIP_DBFTYPE";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   char *type = NULL;
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);
   switch (wa->rd->sig)
      {
      case 0x03:
      case 0x83:
	 type = "DBF";
	 break;
      case 0xF5:
	 type = "FOX";
	 break;
      case 0x30:
	 type = "VFP";
	 break;
      default:
	 break;
      }
   _clip_retc(ClipMachineMemory, type);
   return 0;
}

int
clip_CLIP_MEMOTYPE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "CLIP_MEMOTYPE";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   char *type = NULL;
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);
   switch (wa->rd->sig)
      {
      case 0x83:
	 type = "DBT";
	 break;
      case 0xF5:
      case 0x30:
	 type = "FPT";
	 break;
      default:
	 break;
      }
   _clip_retc(ClipMachineMemory, type);
   return 0;
}

int
clip_CLIP_USEDRDD(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "CLIP_USEDRDD";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);
   _clip_retc(ClipMachineMemory, wa->driver);
   return 0;
}

int
clip_CLIP_ISMEMO(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "CLIP_ISMEMO";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);
   _clip_retl(ClipMachineMemory, wa->rd->memo ? 1 : 0);
   return 0;
}

/* FlagShip functions */
int
clip_ISDBEXCL(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ISDBEXCL";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);
   _clip_retl(ClipMachineMemory, !wa->rd->shared);
   return 0;
}

int
clip_ISDBFLOCK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ISDBFLOCK";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);
   _clip_retl(ClipMachineMemory, wa->rd->flocked || !wa->rd->shared);
   return 0;
}

int
clip_ISDBRLOCK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ISDBRLOCK";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int recno = _clip_parni(ClipMachineMemory, 1);
   int r, er;
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);
   CHECKOPT1(1, NUMERIC_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 1) != NUMERIC_type_of_ClipVarType)
      recno = wa->rd->recno;

   if ((er = wa->rd->vtbl->rlocked(ClipMachineMemory, wa->rd, recno, &r, __PROC__)))
      goto err;
   _clip_retl(ClipMachineMemory, wa->rd->flocked || !wa->rd->shared || r);
   return 0;
 err:
   return er;
}

int
clip_RLOCKVERIFY(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RLOCKVERIFY";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int r, er;
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);

   READLOCK;
   if ((er = wa->rd->vtbl->verify(ClipMachineMemory, wa->rd, &r, __PROC__)))
      goto err_unlock;
   UNLOCK;
   if (r)
      {
	 return clip_RLOCK(ClipMachineMemory);
      }
   _clip_retl(ClipMachineMemory, 0);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_CLIP_EVALSEEK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "CLIP_EVALSEEK";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   ClipVar *block = _clip_spar(ClipMachineMemory, 1);
   int found, er;
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);
   CHECKARG2(1, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if (rdd_seekeval(ClipMachineMemory, wa->rd, block, &found, __PROC__))
      goto err_unlock;
   UNLOCK;

   wa->found = found;
   _clip_retl(ClipMachineMemory, found);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip__SEEK_EVAL(ClipMachine * ClipMachineMemory)
{
/* #include "ci_clipmachine.h" */
   return clip_CLIP_EVALSEEK(ClipMachineMemory);
}

/* end of FlagShip functions */

int
clip_ISDBRLOCKFOREIGN(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ISDBRLOCKFOREIGN";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int recno = _clip_parni(ClipMachineMemory, 1);
   int er, r;
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);
   CHECKOPT1(1, NUMERIC_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 1) != NUMERIC_type_of_ClipVarType)
      recno = wa->rd->recno;

   if ((er = wa->rd->vtbl->forlock(ClipMachineMemory, wa->rd, recno, &r, __PROC__)))
      goto err;

   _clip_retl(ClipMachineMemory, r);
   return 0;
 err:
   return er;
}

int
clip_ISDBRDONLY(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ISDBRDONLY";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);
   _clip_retl(ClipMachineMemory, wa->rd->readonly);
   _clip_retl(ClipMachineMemory, wa->rd->memo ? 1 : 0);
   return 0;
}

int
clip_CLIP_REGEXSEEK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "CLIP_REGEXSEEK";
   const char *pattern = _clip_parc(ClipMachineMemory, 1);
   int cont = _clip_parl(ClipMachineMemory, 2);
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int found, er;
/* #include "ci_clipmachine.h" */

   CHECKWA(wa);
   if (_clip_parinfo(ClipMachineMemory, 1) != CHARACTER_type_of_ClipVarType)
      {
	 _clip_retl(ClipMachineMemory, 0);
	 return 0;
      }
   CHECKOPT1(2, LOGICAL_type_of_ClipVarType);

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if (rdd_wildseek(ClipMachineMemory, wa->rd, pattern, 1, cont, &found, __PROC__))
      goto err_unlock;
   UNLOCK;

   wa->found = found;
   _clip_retl(ClipMachineMemory, found);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
