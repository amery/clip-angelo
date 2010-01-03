/*
	Copyright (C) 2001  ITK
	Author  : Rushad Nabiullin <rust@itk.ru>
	License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI
*/
#include <string.h>
#include <limits.h>
#include "ci_rdd.h"
#include "ci_error.ch"

#define READLOCK	if((er = rd->vtbl->_rlock(ClipMachineMemory,rd,__PROC__))) goto err
#define WRITELOCK	if((er = rd->vtbl->_wlock(ClipMachineMemory,rd,__PROC__))) goto err
#define UNLOCK		if((er = rd->vtbl->_ulock(ClipMachineMemory,rd,__PROC__))) goto err

#define er_badinstance  _clip_gettext("Bad RDD instance")
#define er_nofield      _clip_gettext("No such field")
#define er_badargument2 _clip_gettext("Bad argument (2)")
#define er_badfilter    _clip_gettext("Bad filter handle")
#define er_noorder      _clip_gettext("No controlling order")

static RDD_DATA *
_fetch_rdd(ClipMachine * ClipMachineMemory, const char *__PROC__)
{
   RDD_DATA *rd = (RDD_DATA *) _clip_fetch_c_item(ClipMachineMemory,
						  _clip_parni(ClipMachineMemory, 1),
						  _C_ITEM_TYPE_RDD);

   if (_clip_parinfo(ClipMachineMemory, 1) != NUMERIC_type_of_ClipVarType)
    {
       rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badinstance);
       return NULL;
    }
   if (!rd)
    {
       rdd_err(ClipMachineMemory, EG_NOTABLE, 0, __FILE__, __LINE__, __PROC__, er_badinstance);
       return NULL;
    }
   return rd;
}

int
clip_RDDUSEAREA(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDUSEAREA";

   const char *driver = _clip_parc(ClipMachineMemory, 1);

   const char *name = _clip_parc(ClipMachineMemory, 2);

   int       shared = _clip_parl(ClipMachineMemory, 3);

   int       readonly = _clip_parl(ClipMachineMemory, 4);

   int       tempo = _clip_parl(ClipMachineMemory, 5);

   RDD_DATA *data;

   int       er;

   CHECKOPT1(1, CHARACTER_type_of_ClipVarType);
   CHECKARG1(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(3, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(4, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(5, LOGICAL_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      shared = !(ClipMachineMemory->flags & EXCLUSIVE_FLAG);

   if ((er = rdd_usearea(ClipMachineMemory, driver, name, shared, readonly, tempo, &data, __PROC__)))
      goto err;
   data->area = -1;
   data->rdhandle = _clip_store_c_item(ClipMachineMemory, (void *) data, _C_ITEM_TYPE_RDD, destroy_rdd_data);
   _clip_retni(ClipMachineMemory, data->rdhandle);
   return 0;
 err:
   return er;
}

int
clip_RDDCREATE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDCREATE";

   const char *driver = _clip_parc(ClipMachineMemory, 1);

   const char *name = _clip_parc(ClipMachineMemory, 2);

   ClipVar  *stru = _clip_par(ClipMachineMemory, 3);

   int       er;

   CHECKOPT1(1, CHARACTER_type_of_ClipVarType);
   CHECKARG1(2, CHARACTER_type_of_ClipVarType);
   CHECKARG1(3, ARRAY_type_of_ClipVarType);

   if ((er = rdd_create(ClipMachineMemory, driver, NULL, name, stru, __PROC__)))
      goto err;
   return 0;
 err:
   return er;
}

int
clip___RDDZAP(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "__RDDZAP";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int       er;

   if (!rd)
      return EG_NOTABLE;

   if ((er = rdd_zap(ClipMachineMemory, rd, __PROC__)))
      goto err;
   return 0;
 err:
   return er;
}

int
clip_RDDCREATEMEMO(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDCREATEMEMO";

   const char *driver = _clip_parc(ClipMachineMemory, 1);

   const char *name = _clip_parc(ClipMachineMemory, 2);

   int       er;

   CHECKOPT1(1, CHARACTER_type_of_ClipVarType);
   CHECKARG1(2, CHARACTER_type_of_ClipVarType);

   if ((er = rdd_creatememo(ClipMachineMemory, driver, name, __PROC__)))
      goto err;
   return 0;
 err:
   return er;
}

int
clip_RDDSETINDEX(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDSETINDEX";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   const char *driver = _clip_parc(ClipMachineMemory, 2);

   const char *name = _clip_parc(ClipMachineMemory, 3);

   const char *tag = _clip_parc(ClipMachineMemory, 4);

   int       er;

   if (!rd)
      return EG_NOTABLE;
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);
   CHECKARG1(3, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(4, CHARACTER_type_of_ClipVarType);

   READLOCK;
   if ((er = rdd_setindex(ClipMachineMemory, rd, NULL, driver, name, tag, 0, __PROC__)))
      goto err_unlock;
   if ((er = rdd_gotop(ClipMachineMemory, rd, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}

int
clip_RDDSETMEMO(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDSETMEMO";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   const char *driver = _clip_parc(ClipMachineMemory, 2);

   const char *name = _clip_parc(ClipMachineMemory, 3);

   int       er;

   if (!rd)
      return EG_NOTABLE;
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);
   CHECKARG1(3, CHARACTER_type_of_ClipVarType);

   if ((er = rdd_setmemo(ClipMachineMemory, rd, driver, name, __PROC__)))
      goto err;
   return 0;
 err:
   return er;
}

int
clip_RDDSETRELATION(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDSETRELATION";

   int       parent = _clip_parni(ClipMachineMemory, 1);

   int       child = _clip_parni(ClipMachineMemory, 2);

   RDD_DATA *rd = (RDD_DATA *) _clip_fetch_c_item(ClipMachineMemory, parent, _C_ITEM_TYPE_RDD);

   RDD_DATA *crd = (RDD_DATA *) _clip_fetch_c_item(ClipMachineMemory, child, _C_ITEM_TYPE_RDD);

   const char *expr = _clip_parc(ClipMachineMemory, 3);

   int       er;

   if (!rd)
      return EG_NOTABLE;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(3, CHARACTER_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      crd = NULL;
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      crd = NULL;

   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_setrelation(ClipMachineMemory, rd, crd, NULL, expr, 1, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}

int
clip_RDDCREATEINDEX(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDCREATEINDEX";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   const char *driver = _clip_parc(ClipMachineMemory, 2);

   const char *name = _clip_parc(ClipMachineMemory, 3);

   const char *tag = _clip_parc(ClipMachineMemory, 4);

   const char *expr = _clip_parc(ClipMachineMemory, 5);

   int       unique = _clip_parl(ClipMachineMemory, 6);

   int       lIgnoreCase = _clip_parl(ClipMachineMemory, 7);

   const char *cfor = _clip_parc(ClipMachineMemory, 8);

   int       er;

   if (!rd)
      return EG_NOTABLE;
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);
   CHECKARG1(3, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(4, CHARACTER_type_of_ClipVarType);
   CHECKARG1(5, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(6, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(7, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(8, CHARACTER_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 6) == UNDEF_type_of_ClipVarType)
      unique = ClipMachineMemory->flags & UNIQUE_FLAG;

   rd->os.lIgnoreCase = lIgnoreCase;

   if (rd->os.cForCondition)
    {
       free(rd->os.cForCondition);
       rd->os.cForCondition = NULL;
    }
   if (cfor && cfor[0])
    {
       int       r = _clip_parni(ClipMachineMemory, 1);

       char      forexpr[1024];

       rd->os.cForCondition = malloc(strlen(cfor) + 1);
       strcpy(rd->os.cForCondition, cfor);

       memset(forexpr, 0, sizeof(forexpr));

       rdd_expandmacro(rd->area, r, cfor, forexpr);

       if ((er = _clip_eval_macro(ClipMachineMemory, forexpr, strlen(forexpr), &rd->os.bForCondition)))
	  goto err;
    }

   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   if (rd->readonly)
    {
       READLOCK;
    }
   else
    {
       WRITELOCK;
    }
   if ((er = rdd_createindex(ClipMachineMemory, rd, driver, name, tag, expr, NULL, unique, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}

int
clip_RDDREINDEX(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDREINDEX";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int       er;

   if (!rd)
      return EG_NOTABLE;

   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      return er;
   return rdd_reindex(ClipMachineMemory, rd, __PROC__);
}

int
clip_RDDCLEARINDEX(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDCLEARINDEX";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   if (!rd)
      return EG_NOTABLE;

   return rdd_clearindex(ClipMachineMemory, rd, __PROC__);
}

int
clip_RDDSETORDER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDSETORDER";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int       er;

   if (!rd)
      return EG_NOTABLE;
   CHECKOPT2(2, NUMERIC_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);

   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   READLOCK;
   if (_clip_parinfo(ClipMachineMemory, 2) == NUMERIC_type_of_ClipVarType)
    {
       int       order = _clip_parni(ClipMachineMemory, 2);

       if ((er = rdd_setorder(ClipMachineMemory, rd, order, __PROC__)))
	  goto err_unlock;
    }
   else
    {
       const char *tag = _clip_parc(ClipMachineMemory, 2);

       int       i;

       if (tag)
	{
	   for (i = 0; i < rd->ords_opened; i++)
	    {
	       if (strcasecmp(rd->orders[i]->name, tag) == 0)
		{
		   if ((er = rdd_setorder(ClipMachineMemory, rd, i + 1, __PROC__)))
		      goto err_unlock;
		   break;
		}
	    }
	}
    }
   UNLOCK;
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}

int
clip_RDDINDEXORD(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDINDEXORD";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   if (!rd)
      return EG_NOTABLE;

   _clip_retni(ClipMachineMemory, rd->curord + 1);
   return 0;
}

int
clip_RDDCLOSEAREA(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDCLOSEAREA";

   int       key = _clip_parni(ClipMachineMemory, 1);

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int       er;

   if (!rd)
      return EG_NOTABLE;

   if (rd->area != -1)
      return 0;
   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   if (rd->readonly)
      rd->vtbl->_rlock(ClipMachineMemory, rd, __PROC__);
   else
      rd->vtbl->_wlock(ClipMachineMemory, rd, __PROC__);
   if ((er = rdd_closearea(ClipMachineMemory, rd, __PROC__)))
      goto err;
   _clip_destroy_c_item(ClipMachineMemory, key, _C_ITEM_TYPE_RDD);
   return 0;
 err:
   return er;
}

int
clip_RDDCLOSEALL(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDCLOSEALL";

   RDD_DATA *rd;

   int       key, i, er;

   for (i = 0; i < ClipMachineMemory->container->len_of_Container; i++)
    {
       if (ClipMachineMemory->container->ContainerItem_items_of_Container[i].type_of_ContainerItem == _C_ITEM_TYPE_RDD)
	{
	   rd = (RDD_DATA *) ClipMachineMemory->container->ContainerItem_items_of_Container[i].item_of_ContainerItem;
	   if (rd->ClipMachineMemory == ClipMachineMemory && rd->area == -1)
	    {
	       key = ClipMachineMemory->container->ContainerItem_items_of_Container[i].key_of_ContainerItem;
	       if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
		  goto err;
	       if (rd->readonly)
		  rd->vtbl->_rlock(ClipMachineMemory, rd, __PROC__);
	       else
		  rd->vtbl->_wlock(ClipMachineMemory, rd, __PROC__);
	       if ((er = rdd_closearea(ClipMachineMemory, rd, __PROC__)))
		  goto err;
	       _clip_destroy_c_item(ClipMachineMemory, key, _C_ITEM_TYPE_RDD);
	       rd = (RDD_DATA *) _clip_fetch_c_item_type(ClipMachineMemory, _C_ITEM_TYPE_RDD, &key);
	    }
	}
    }
   return 0;
 err:
   return er;
}

int
clip_RDDSEEK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDSEEK";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   ClipVar  *v = _clip_par(ClipMachineMemory, 2);

   int       soft = _clip_parl(ClipMachineMemory, 3);

   int       last = _clip_parl(ClipMachineMemory, 4);

   int       found, er;

   _clip_retl(ClipMachineMemory, 0);
   if (!rd)
      return EG_NOTABLE;
   CHECKOPT1(3, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(4, LOGICAL_type_of_ClipVarType);

   if (!v)
      return 0;

   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      soft = (ClipMachineMemory->flags & SOFTSEEK_FLAG);
   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_seek(ClipMachineMemory, rd, v, soft, last, &found, __PROC__)))
      goto err_unlock;
   UNLOCK;
   _clip_retl(ClipMachineMemory, found);
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}

int
clip_RDDGOTOP(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDGOTOP";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int       er;

   if (!rd)
      return EG_NOTABLE;

   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_gotop(ClipMachineMemory, rd, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}

int
clip_RDDGOBOTTOM(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDGOBOTTOM";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int       er;

   if (!rd)
      return EG_NOTABLE;

   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_gobottom(ClipMachineMemory, rd, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}

int
clip_RDDSKIP(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDSKIP";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int       recs = _clip_parni(ClipMachineMemory, 2);

   int       er;

   if (!rd)
      return EG_NOTABLE;
   CHECKOPT1(2, NUMERIC_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      recs = 1;

   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_skip(ClipMachineMemory, rd, recs, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}

int
clip_RDDGOTO(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDGOTO";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int       rec = _clip_parni(ClipMachineMemory, 2);

   int       er;

   if (!rd)
      return EG_NOTABLE;
   CHECKARG1(2, NUMERIC_type_of_ClipVarType);

   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_goto(ClipMachineMemory, rd, rec, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}

int
clip_RDDGOTOKEY(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDGOTOKEY";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   unsigned int rec = _clip_parni(ClipMachineMemory, 2);

   int       ok, er;

   if (!rd)
      return EG_NOTABLE;
   CHECKARG1(2, NUMERIC_type_of_ClipVarType);
   if (rd->curord == -1)
      return 0;

   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_gotokey(ClipMachineMemory, rd, rd->orders[rd->curord], rec, &ok, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}

int
clip_RDDBOF(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDBOF";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int       bof, er;

   if (!rd)
      return EG_NOTABLE;

   if ((er = rdd_bof(ClipMachineMemory, rd, &bof, __PROC__)))
      goto err;
   _clip_retl(ClipMachineMemory, bof);
   return 0;
 err:
   return er;
}

int
clip_RDDEOF(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDEOF";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int       eof, er;

   if (!rd)
      return EG_NOTABLE;

   if ((er = rdd_eof(ClipMachineMemory, rd, &eof, __PROC__)))
      goto err;
   _clip_retl(ClipMachineMemory, eof);
   return 0;
 err:
   return er;
}

int
clip_RDDRECNO(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDRECNO";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int       recno, er;

   if (!rd)
      return EG_NOTABLE;

   if ((er = rdd_recno(ClipMachineMemory, rd, &recno, __PROC__)))
      goto err;
   _clip_retni(ClipMachineMemory, recno);
   return 0;
 err:
   return er;
}

int
clip_RDDLASTREC(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDLASTREC";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int       lastrec, er;

   if (!rd)
      return EG_NOTABLE;

   READLOCK;
   if ((er = rdd_lastrec(ClipMachineMemory, rd, &lastrec, __PROC__)))
      goto err_unlock;
   UNLOCK;
   _clip_retni(ClipMachineMemory, lastrec);
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}

int
clip_RDDKEYNO(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDKEYNO";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int       keyno = 0, er;

   if (!rd)
      return EG_NOTABLE;

   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_keyno(ClipMachineMemory, rd, &keyno, __PROC__)))
      goto err_unlock;
   UNLOCK;
   _clip_retni(ClipMachineMemory, keyno);
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}

int
clip_RDDLASTKEY(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDLASTKEY";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int       lastkey = 0, er;

   if (!rd)
      return EG_NOTABLE;

   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_lastkey(ClipMachineMemory, rd, &lastkey, __PROC__)))
      goto err_unlock;
   UNLOCK;
   _clip_retni(ClipMachineMemory, lastkey);
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}

int
clip_RDDGETVALUE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDGETVALUE";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int       type = _clip_parinfo(ClipMachineMemory, 2);

   int       er;

   if (!rd)
      return EG_NOTABLE;
   READLOCK;
   if (type == CHARACTER_type_of_ClipVarType)
    {
       const char *fname = _clip_parc(ClipMachineMemory, 2);

       if ((er = rdd_getvaluebn(ClipMachineMemory, rd, fname, __PROC__)))
	  goto err_unlock;
    }
   else if (type == NUMERIC_type_of_ClipVarType)
    {
       int       no = _clip_parni(ClipMachineMemory, 2) - 1;

       if ((er = rdd_getvalue(ClipMachineMemory, rd, no, __PROC__)))
	  goto err_unlock;
    }
   else
    {
       er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_nofield);
       goto err_unlock;
    }
   UNLOCK;
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}

int
clip_RDDSETVALUE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDSETVALUE";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int       type = _clip_parinfo(ClipMachineMemory, 2);

   ClipVar  *vp = _clip_par(ClipMachineMemory, 3);

   int       er;

   if (!rd)
      return EG_NOTABLE;
   WRITELOCK;
   if (type == CHARACTER_type_of_ClipVarType)
    {
       const char *fname = _clip_parc(ClipMachineMemory, 2);

       if ((er = rdd_setvaluebn(ClipMachineMemory, rd, fname, vp, __PROC__)))
	  goto err_unlock;
    }
   else if (type == NUMERIC_type_of_ClipVarType)
    {
       int       no = _clip_parni(ClipMachineMemory, 2) - 1;

       if (no < 0 || no >= rd->nfields)
	{
	   er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_nofield);
	   goto err_unlock;
	}
       if ((er = rdd_setvalue(ClipMachineMemory, rd, no, vp, __PROC__)))
	  goto err_unlock;
    }
   else
    {
       er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_nofield);
       goto err_unlock;
    }
   UNLOCK;
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}

int
clip_RDDREAD(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDREAD";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int       er;

   if (!rd)
      return EG_NOTABLE;
   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_dbread(ClipMachineMemory, rd, RETPTR(ClipMachineMemory), __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}

int
clip_RDDWRITE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDWRITE";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   ClipVar  *ap = _clip_par(ClipMachineMemory, 2);

   int       er;

   if (!rd)
      return EG_NOTABLE;
   CHECKARG1(2, MAP_type_of_ClipVarType);

   _clip_retl(ClipMachineMemory, 1);
   WRITELOCK;
   if ((er = rdd_dbwrite(ClipMachineMemory, rd, ap, __PROC__)))
      goto err_unlock;
   UNLOCK;
   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}

int
clip_RDDAPPEND(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDAPPEND";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   ClipVar  *ap = _clip_par(ClipMachineMemory, 2);

   int       neterr, er;

   if (!rd)
      return EG_NOTABLE;
   CHECKOPT1(2, MAP_type_of_ClipVarType);

   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   WRITELOCK;
   if ((er = rdd_append(ClipMachineMemory, rd, &neterr, __PROC__)))
      goto err_unlock;
   if (ap && ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
      if ((er = rdd_dbwrite(ClipMachineMemory, rd, ap, __PROC__)))
	 goto err_unlock;
   UNLOCK;
   if (ap && ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
      if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
	 goto err;
   _clip_retl(ClipMachineMemory, 1);
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}

int
clip_RDDSETFILTER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDSETFILTER";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   const char *str = _clip_parc(ClipMachineMemory, 2);

   ClipVar  *a = _clip_par(ClipMachineMemory, 3);

   int       lNoOptimize = _clip_parl(ClipMachineMemory, 4);

   RDD_FILTER *fp;

   int       er;

   if (!rd)
      return EG_NOTABLE;
   CHECKARG1(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(3, ARRAY_type_of_ClipVarType);
   CHECKOPT1(4, LOGICAL_type_of_ClipVarType);

   if ((er = rdd_clearfilter(ClipMachineMemory, rd, __PROC__)))
      goto err;

   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_createfilter(ClipMachineMemory, rd, &fp, NULL, str, a, lNoOptimize, __PROC__)))
      goto err_unlock;
   rd->filter = fp;
   fp->active = 1;
   if ((er = _rdd_calcfiltlist(ClipMachineMemory, rd, fp, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}

int
clip_RDDFILTER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDFILTER";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   if (!rd)
      return EG_NOTABLE;

   _clip_retc(ClipMachineMemory, "");
   if (rd->filter && rd->filter->sfilter)
      _clip_retc(ClipMachineMemory, rd->filter->sfilter);
   return 0;
}

int
clip_RDDCLEARFILTER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDCLEARFILTER";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   if (!rd)
      return EG_NOTABLE;

   return rdd_clearfilter(ClipMachineMemory, rd, __PROC__);
}

int
clip_RDDDELETE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDDELETE";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int       er;

   if (!rd)
      return EG_NOTABLE;

   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   WRITELOCK;
   if ((er = rdd_delete(ClipMachineMemory, rd, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}

int
clip_RDDRECALL(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDRECALL";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int       er;

   if (!rd)
      return EG_NOTABLE;

   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   WRITELOCK;
   if ((er = rdd_recall(ClipMachineMemory, rd, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}

int
clip_RDDDELETED(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDDELETED";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int       deleted, er;

   if (!rd)
      return EG_NOTABLE;

   READLOCK;
   if ((er = rdd_deleted(ClipMachineMemory, rd, &deleted, __PROC__)))
      goto err_unlock;
   UNLOCK;
   _clip_retl(ClipMachineMemory, deleted);
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}

int
clip_RDDINDEXKEY(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDINDEXKEY";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   if (!rd)
      return EG_NOTABLE;

   if (rd->curord != -1)
      _clip_retc(ClipMachineMemory, rd->orders[rd->curord]->expr);
   return 0;
}

int
clip_RDDFCOUNT(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDFCOUNT";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   if (!rd)
      return EG_NOTABLE;

   _clip_retni(ClipMachineMemory, rd->nfields);
   return 0;
}

int
clip_RDDFIELDPOS(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDFIELDPOS";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   char     *fname = _clip_parc(ClipMachineMemory, 2);

   if (!rd)
      return EG_NOTABLE;

   _clip_retni(ClipMachineMemory, _rdd_fieldno(rd, _clip_casehashword(fname, strlen(fname))) + 1);
   return 0;
}

int
clip_RDDFIELDNAME(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDFIELDNAME";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int       fno = _clip_parni(ClipMachineMemory, 2);

   if (!rd)
      return EG_NOTABLE;

   if (fno > rd->nfields)
      _clip_retc(ClipMachineMemory, "");
   else
      _clip_retc(ClipMachineMemory, rd->fields[fno - 1].name);
   return 0;
}

int
clip_RDDLOCATE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDLOCATE";

   int       r = _clip_parni(ClipMachineMemory, 1);

   char     *fe = _clip_parc(ClipMachineMemory, 2);

   char     *we = _clip_parc(ClipMachineMemory, 3);

   ClipVar  *vnext = _clip_par(ClipMachineMemory, 4);

   ClipVar  *vrec = _clip_par(ClipMachineMemory, 5);

   ClipVar  *vrest = _clip_par(ClipMachineMemory, 6);

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   ClipVar   fexpr;

   ClipVar   wexpr;

   int       found, er;

   char      expexp[1024];

   if (!rd)
      return EG_NOTABLE;
   CHECKARG1(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(3, CHARACTER_type_of_ClipVarType);

   memset(&fexpr, 0, sizeof(ClipVar));
   memset(&wexpr, 0, sizeof(ClipVar));
   memset(expexp, 0, sizeof(expexp));
   rdd_expandmacro(rd->area, r, fe, expexp);
   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   READLOCK;
   if ((er = _clip_eval_macro(ClipMachineMemory, expexp, strlen(expexp), &fexpr)))
      goto err_unlock;
   if (we)
    {
       memset(expexp, 0, sizeof(expexp));
       rdd_expandmacro(rd->area, r, we, expexp);
       if ((er = _clip_eval_macro(ClipMachineMemory, expexp, strlen(expexp), &wexpr)))
	  goto err_unlock;
    }
   if ((er = rdd_locate(ClipMachineMemory, rd, fe, &fexpr, &wexpr, vnext, vrec, vrest, &found, __PROC__)))
      goto err_unlock;
   UNLOCK;

   _clip_destroy(ClipMachineMemory, &fexpr);
   _clip_destroy(ClipMachineMemory, &wexpr);

   _clip_retl(ClipMachineMemory, found);
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   _clip_destroy(ClipMachineMemory, &fexpr);
   _clip_destroy(ClipMachineMemory, &wexpr);
   return er;
}

int
clip_RDDCONTINUE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDCONTINUE";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int       found;

   int       er;

   if (!rd)
      return EG_NOTABLE;

   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_continue(ClipMachineMemory, rd, &found, __PROC__)))
      goto err_unlock;
   UNLOCK;

   _clip_retl(ClipMachineMemory, found);
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}

int
clip_RDDSTRUCT(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDSTRUCT";

   int       nfld, i;

   ClipVar  *rp = RETPTR(ClipMachineMemory);

   long      vect[2];

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   if (!rd)
      return EG_NOTABLE;

   nfld = rd->nfields;
   vect[0] = nfld;
   vect[1] = 4;
   _clip_array(ClipMachineMemory, rp, 2, vect);

   for (i = 0; i < nfld; i++)
    {
       ClipVar   var;

       RDD_FIELD *fp = &rd->fields[i];

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
    }
   return 0;
}

int
clip_RDDORDSCOPE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDORDSCOPE";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int       scope = _clip_parni(ClipMachineMemory, 2);

   ClipVar  *v = _clip_par(ClipMachineMemory, 3);

   int       er;

   if (!rd)
      return EG_NOTABLE;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   READLOCK;
   if (scope == 0)
    {
       if ((er = rdd_scopetop(ClipMachineMemory, rd, v, __PROC__)))
	  goto err_unlock;
    }
   else if (scope == 1)
    {
       if ((er = rdd_scopebottom(ClipMachineMemory, rd, v, __PROC__)))
	  goto err_unlock;
    }
   else
    {
       er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badargument2);
       goto err_unlock;
    }
   UNLOCK;
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}

int
clip_RDDNEWFILTER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDNEWFILTER";

   int       type = _clip_parinfo(ClipMachineMemory, 2);

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   RDD_FILTER *fp;

   int       er;

   if (!rd)
      return EG_NOTABLE;

   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   READLOCK;
   if (type == NUMERIC_type_of_ClipVarType || type == UNDEF_type_of_ClipVarType)
    {
       unsigned int len = _clip_parni(ClipMachineMemory, 2);

       if ((er = rdd_createuserfilter(ClipMachineMemory, rd, &fp, len, __PROC__)))
	  goto err_unlock;
    }
   else if (type == CHARACTER_type_of_ClipVarType)
    {
       char     *str = _clip_parc(ClipMachineMemory, 2);

       if ((er = rdd_createfilter(ClipMachineMemory, rd, &fp, NULL, str, NULL, 0, __PROC__)))
	  goto err_unlock;
    }
   else
    {
       er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badargument2);
       goto err_unlock;
    }
   UNLOCK;
   _clip_retni(ClipMachineMemory, fp->handle);
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}

int
clip_RDDSETAREAFILTER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDSETAREAFILTER";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int       h = _clip_parni(ClipMachineMemory, 2);

   RDD_FILTER *fp;

   int       er;

   if (!rd)
      return EG_NOTABLE;
   CHECKARG1(2, NUMERIC_type_of_ClipVarType);

   fp = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RYO);
   if (!fp)
    {
       er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
       goto err;
    }
   fp->active = 1;
   if (rd->filter)
      rd->filter->active = 0;
   rd->filter = fp;
   _clip_retl(ClipMachineMemory, 1);
   return 0;
 err:
   return er;
}

int
clip_RDDGETAREAFILTER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDGETAREAFILTER";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   if (!rd)
      return EG_NOTABLE;

   if (rd->filter)
      _clip_retni(ClipMachineMemory, rd->filter->handle);
   else
      _clip_retni(ClipMachineMemory, -1);
   return 0;
}

int
clip_RDDRLOCK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDRLOCK";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   unsigned int recno = _clip_parni(ClipMachineMemory, 2);

   int       lrelease = _clip_parl(ClipMachineMemory, 3);

   int       r, er;

   if (!rd)
      return EG_NOTABLE;

   CHECKOPT1(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(3, LOGICAL_type_of_ClipVarType);

   if (recno == 0)
      recno = rd->recno;

   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      lrelease = 1;

   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   if (lrelease)
    {
       if ((er = rdd_ulock(ClipMachineMemory, rd, 0, 0, __PROC__)))
	  goto err;
    }

   if ((er = rdd_rlock(ClipMachineMemory, rd, recno, &r, __PROC__)))
      goto err;

   _clip_retl(ClipMachineMemory, r);
   return 0;
 err:
   return er;
}

int
clip_RDDFLOCK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDFLOCK";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int       er;

   if (!rd)
      return EG_NOTABLE;

   if ((er = rdd_flock(ClipMachineMemory, rd, __PROC__)))
      goto err;
   return 0;
 err:
   return er;
}

int
clip_RDDUNLOCK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDUNLOCK";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   unsigned int recno = _clip_parni(ClipMachineMemory, 2);

   int       er;

   if (!rd)
      return EG_NOTABLE;

   CHECKOPT1(2, NUMERIC_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
    {
       recno = rd->recno;
    }

   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   if ((er = rdd_ulock(ClipMachineMemory, rd, recno, 1, __PROC__)))
      goto err;
   return 0;
 err:
   return er;
}

int
clip_RDDFIELDDECI(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDFIELDDECI";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int       fno = _clip_parni(ClipMachineMemory, 2) - 1;

   int       dec = 0, er;

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   if (rd)
    {
       if (fno >= 0 && fno < rd->nfields)
	  dec = rd->fields[fno].dec;
    }
   _clip_retni(ClipMachineMemory, dec);
   return 0;
 err:
   return er;
}

int
clip_RDDFIELDNUM(ClipMachine * ClipMachineMemory)
{
   return clip_RDDFIELDPOS(ClipMachineMemory);
}

int
clip_RDDFIELDSIZE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDFIELDSIZE";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int       fno = _clip_parni(ClipMachineMemory, 2) - 1;

   int       len = 0, er;

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   if (rd)
    {
       if (fno >= 0 && fno < rd->nfields)
	  len = rd->fields[fno].len;
    }
   _clip_retni(ClipMachineMemory, len);
   return 0;
 err:
   return er;
}

int
clip_RDDFIELDTYPE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDFIELDTYPE";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int       fno = _clip_parni(ClipMachineMemory, 2) - 1;

   char      type[2] = " ";

   int       er;

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   if (rd)
    {
       if (fno >= 0 && fno < rd->nfields)
	  type[0] = rd->fields[fno].type;
    }

   _clip_retc(ClipMachineMemory, type);
   return 0;
 err:
   return er;
}

/* SIX */

int
clip_RDD_M6_NEWFILTER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDD_M6_NEWFILTER";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int       type = _clip_parinfo(ClipMachineMemory, 2);

   RDD_FILTER *fp;

   int       er;

   if (!rd)
      return EG_NOTABLE;
   CHECKOPT2(2, NUMERIC_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);

   READLOCK;
   if (type == NUMERIC_type_of_ClipVarType || type == UNDEF_type_of_ClipVarType)
    {
       unsigned int len = _clip_parni(ClipMachineMemory, 2);

       if ((er = rdd_createuserfilter(ClipMachineMemory, rd, &fp, len, __PROC__)))
	  goto err_unlock;
    }
   else if (type == CHARACTER_type_of_ClipVarType)
    {
       char     *str = _clip_parc(ClipMachineMemory, 2);

       if ((er = rdd_createfilter(ClipMachineMemory, rd, &fp, NULL, str, NULL, 0, __PROC__)))
	  goto err_unlock;
    }
   UNLOCK;

   _clip_retni(ClipMachineMemory, fp->handle);
   return 0;

 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}

int
clip_RDD_M6_SETAREAFILTER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDD_M6_SETAREAFILTER";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int       h = _clip_parni(ClipMachineMemory, 2);

   char      expr[PATH_MAX];

   RDD_FILTER *fp;

   int       er;

   if (!rd)
      return EG_NOTABLE;
   CHECKARG1(2, NUMERIC_type_of_ClipVarType);

   fp = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RYO);
   if (!fp)
    {
       er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
       goto err;
    }
   fp->active = 1;
   if (rd->filter)
      rd->filter->active = 0;
   rd->filter = fp;

   fp->rd = rd;
   if (!fp->custom)
    {
       rdd_expandmacro(rd->area, 0, fp->sfilter, expr);
       if ((er = _clip_eval_macro(ClipMachineMemory, expr, strlen(expr), &fp->fps->bfilter)))
	  goto err;
    }

   _clip_retl(ClipMachineMemory, 1);
   return 0;
 err:
   return er;
}

int
clip_RDD_M6_GETAREAFILTER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDD_M6_GETAREAFILTER";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   if (!rd)
      return EG_NOTABLE;

   if (rd->filter)
      _clip_retni(ClipMachineMemory, rd->filter->handle);
   else
      _clip_retni(ClipMachineMemory, -1);
   return 0;
}

int
clip_RDD_M6_ADDSCOPED(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDD_M6_ADDSCOPED";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int       h = _clip_parni(ClipMachineMemory, 2);

   RDD_FILTER *fp;

   ClipVar  *t = _clip_vptr(_clip_par(ClipMachineMemory, 3));

   ClipVar  *b = _clip_vptr(_clip_par(ClipMachineMemory, 4));

   int       ord = _clip_parni(ClipMachineMemory, 5) - 1;

   int       er;

   if (!rd)
      return EG_NOTABLE;
   CHECKARG1(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(5, NUMERIC_type_of_ClipVarType);

   fp = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RYO);
   if (!fp)
    {
       er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
       goto err;
    }
   if (!fp->rmap)
    {
       er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
       goto err;
    }
   if (_clip_parinfo(ClipMachineMemory, 5) == UNDEF_type_of_ClipVarType)
      ord = rd->curord;
   if (ord >= rd->ords_opened)
      ord = -1;
   if (ord == -1)
    {
       er = rdd_err(ClipMachineMemory, EG_NOORDER, 0, __FILE__, __LINE__, __PROC__, er_noorder);
       goto err;
    }

   if (t->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType)
      t = NULL;
   if (b->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType)
      b = NULL;

   READLOCK;
   if ((er = rd->orders[ord]->vtbl->setscope(ClipMachineMemory, rd, rd->orders[ord], t, b, fp->rmap, fp->size, 0, __PROC__)))
      goto err_unlock;
   UNLOCK;

   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}
