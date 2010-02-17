/*
	Copyright (C) 2001  ITK
	Author   : Rushad Nabiullin <rust@itk.ru>
	License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI
*/
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include "ci_rdd.h"
#include "ci_six.ch"
#include "ci_error.ch"
#include "ci_set.ch"
#ifdef HAVE_MMAN_H
#include <sys/mman.h>
#endif

#define READLOCK	if((er = wa->rd->vtbl->_rlock(ClipMachineMemory,wa->rd,__PROC__))) goto err
#define WRITELOCK	if((er = wa->rd->vtbl->_wlock(ClipMachineMemory,wa->rd,__PROC__))) goto err
#define UNLOCK		if((er = wa->rd->vtbl->_ulock(ClipMachineMemory,wa->rd,__PROC__))) goto err

extern int get_orderno(DBWorkArea * wa, ClipVar * order, ClipVar * index);
extern DBWorkArea *cur_area(ClipMachine * ClipMachineMemory);
extern DBWorkArea *get_area(ClipMachine * ClipMachineMemory, long area, int any);

extern int clip_ORDKEYNO(ClipMachine * ClipMachineMemory);

#define er_badfilter        _clip_gettext("Bad filter handle")
#define er_invjoin          _clip_gettext("Invalid join type")
#define er_notpermitted     _clip_gettext("Operation not permitted")
#define er_nomemo           _clip_gettext("Memo file not opened")
#define er_nofield          _clip_gettext("No such field")
#define er_notmemo          _clip_gettext("Not memo field")
#define er_noorder          _clip_gettext("No controlling order")
#define er_notable          _clip_gettext("Workarea not in use")
#define er_readlock         _clip_gettext("Shared lock error")
#define er_writelock        _clip_gettext("Exclusive lock error")
#define er_create           _clip_gettext("Create file error")
#define er_open             _clip_gettext("Open file error")
#define er_write            _clip_gettext("File write error")
#define er_read             _clip_gettext("File read error")
#define er_badfltfile       _clip_gettext("Bad .flt file")

int
clip_M6_NEWFILTER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_NEWFILTER";
   int type = _clip_parinfo(ClipMachineMemory, 1);
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   RDD_FILTER *fp;
   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKOPT2(1, NUMERIC_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   if (!wa)
      {
	 _clip_retni(ClipMachineMemory, -1);
	 return 0;
      }

   if ((er = rdd_flushbuffer(ClipMachineMemory, wa->rd, __PROC__)))
      goto err;
   READLOCK;
   if (type == NUMERIC_type_of_ClipVarType || type == UNDEF_type_of_ClipVarType)
      {
	 unsigned int len = _clip_parni(ClipMachineMemory, 1);
	 if ((er = rdd_createuserfilter(ClipMachineMemory, wa->rd, &fp, len, __PROC__)))
	    goto err_unlock;
      }
   else if (type == CHARACTER_type_of_ClipVarType)
      {
	 char *str = _clip_parc(ClipMachineMemory, 1);
	 if ((er = rdd_createfilter(ClipMachineMemory, wa->rd, &fp, NULL, str, NULL, 0, __PROC__)))
	    goto err_unlock;
      }
   UNLOCK;

   _clip_retni(ClipMachineMemory, fp->handle);
   return 0;

 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_M6_SETAREAFILTER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_SETAREAFILTER";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int h = _clip_parni(ClipMachineMemory, 1);
   char expr[PATH_MAX];
   RDD_FILTER *fp;
   int er;

   ClipMachineMemory->m6_error = 0;
   if (!wa)
      {
	 _clip_retl(ClipMachineMemory, 0);
	 return 0;
      }
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   fp = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RYO);
   if (!fp)
      {
	 er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
	 goto err;
      }
   fp->active = 1;
   if (wa->rd->filter)
      wa->rd->filter->active = 0;
   wa->rd->filter = fp;

   fp->rd = wa->rd;
   if (!fp->custom)
      {
	 rdd_expandmacro(wa->rd->area, 0, fp->sfilter, expr);
	 if ((er = _clip_eval_macro(ClipMachineMemory, expr, strlen(expr), &fp->fps->bfilter)))
	    goto err;
      }

   _clip_retl(ClipMachineMemory, 1);
   return 0;
 err:
   return er;
}

int
clip_M6_GETAREAFILTER(ClipMachine * ClipMachineMemory)
{
   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipMachineMemory->m6_error = 0;
   if (!wa)
      {
	 _clip_retni(ClipMachineMemory, -1);
	 return 0;
      }
   if (wa->rd->filter)
      _clip_retni(ClipMachineMemory, wa->rd->filter->handle);
   else
      _clip_retni(ClipMachineMemory, -1);
   return 0;
}

int
clip_M6_FREEFILTER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_FREEFILTER";
   int h = _clip_parni(ClipMachineMemory, 1);
   RDD_FILTER *fp;
   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   fp = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RYO);
   if (!fp)
      {
	 er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
	 goto err;
      }
   if (fp->active)
      {
	 _clip_retl(ClipMachineMemory, 0);
      }
   else
      {
	 if ((er = rdd_destroyfilter(ClipMachineMemory, fp, __PROC__)))
	    goto err;
	 _clip_retl(ClipMachineMemory, 1);
      }
   return 0;
 err:
   return er;
}

int
clip_M6_FILTADDREC(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_FILTADDREC";
   int h = _clip_parni(ClipMachineMemory, 1);
   RDD_FILTER *fp;
   unsigned int rn = _clip_parni(ClipMachineMemory, 2);
   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKARG1(2, NUMERIC_type_of_ClipVarType);

   fp = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RYO);
   if (!fp)
      {
	 er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
	 goto err;
      }
   if (fp->custom && fp->rmap && rn <= fp->size)
      {
	 _rm_setbit(fp->rmap, fp->size, rn);
	 _clip_retl(ClipMachineMemory, 1);
      }
   else
      {
	 _clip_retl(ClipMachineMemory, 0);
      }
   return 0;
 err:
   return er;
}

int
clip_M6_FILTDROPREC(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_FILTDROPREC";
   int h = _clip_parni(ClipMachineMemory, 1);
   RDD_FILTER *fp;
   unsigned int rn = _clip_parni(ClipMachineMemory, 2);
   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKARG1(2, NUMERIC_type_of_ClipVarType);

   fp = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RYO);
   if (!fp)
      {
	 er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
	 goto err;
      }
   if (fp->custom && fp->rmap && rn <= fp->size)
      {
	 _rm_clrbit(fp->rmap, fp->size, rn);
	 _clip_retl(ClipMachineMemory, 1);
      }
   else
      {
	 _clip_retl(ClipMachineMemory, 0);
      }
   return 0;
 err:
   return er;
}

int
clip_M6_FILTCHGREC(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_FILTCHGREC";
   int h = _clip_parni(ClipMachineMemory, 1);
   RDD_FILTER *fp;
   unsigned int rn = _clip_parni(ClipMachineMemory, 2);
   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKARG1(2, NUMERIC_type_of_ClipVarType);

   fp = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RYO);
   if (!fp)
      {
	 er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
	 goto err;
      }
   if (fp->custom && fp->rmap && rn <= fp->size)
      {
	 if (_rm_getbit(fp->rmap, fp->size, rn))
	    {
	       _rm_clrbit(fp->rmap, fp->size, rn);
	    }
	 else
	    {
	       _rm_setbit(fp->rmap, fp->size, rn);
	    }
	 _clip_retl(ClipMachineMemory, 1);
      }
   else
      {
	 _clip_retl(ClipMachineMemory, 0);
      }
   return 0;
 err:
   return er;
}

int
clip_M6_ISFILTER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_ISFILTER";
   int h = _clip_parni(ClipMachineMemory, 1);
   RDD_FILTER *fp;
   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKOPT1(1, NUMERIC_type_of_ClipVarType);
   if (_clip_parinfo(ClipMachineMemory, 1) == NUMERIC_type_of_ClipVarType)
      {
	 fp = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RYO);
      }
   else
      {
	 DBWorkArea *wa = cur_area(ClipMachineMemory);
	 if (!wa)
	    {
	       er = rdd_err(ClipMachineMemory, EG_NOTABLE, 0, __FILE__, __LINE__, __PROC__, er_notable);
	       goto err;
	    }
	 fp = wa->rd->filter;
      }
   if (!fp)
      {
	 er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
	 goto err;
      }

   _clip_retni(ClipMachineMemory, fp->optimize);
   return 0;
 err:
   return er;
}

int
clip_M6_FILTJOIN(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_FILTJOIN";
   int h1 = _clip_parni(ClipMachineMemory, 1);
   int h2 = _clip_parni(ClipMachineMemory, 2);
   int oper = _clip_parni(ClipMachineMemory, 3);
   RDD_FILTER *f1;
   RDD_FILTER *f2;
   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKARG1(2, NUMERIC_type_of_ClipVarType);
   CHECKARG1(3, NUMERIC_type_of_ClipVarType);

   if (oper < 1 || oper > 3)
      {
	 er = rdd_err(ClipMachineMemory, EG_UNSUPPORTED, 0, __FILE__, __LINE__, __PROC__, er_invjoin);
	 goto err;
      }
   f1 = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h1, _C_ITEM_TYPE_RYO);
   f2 = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h2, _C_ITEM_TYPE_RYO);

   if ((er = rm_checkjoin(ClipMachineMemory, f1, f2, __PROC__)))
      goto err;

   if (oper == 1)
      {
	 if ((er = rm_union(ClipMachineMemory, f1, f2, __PROC__)))
	    goto err;
      }
   else if (oper == 2)
      {
	 if ((er = rm_intersect(ClipMachineMemory, f1, f2, __PROC__)))
	    goto err;
      }
   else
      {
	 if (!f1->custom)
	    {
	       er = rdd_err(ClipMachineMemory, EG_UNSUPPORTED, 0, __FILE__, __LINE__, __PROC__, er_notpermitted);
	       goto err;
	    }
	 if ((er = rm_difference(ClipMachineMemory, f1, f2, __PROC__)))
	    goto err;
      }
   if (!f2->active)
      {
	 if ((er = rdd_destroyfilter(ClipMachineMemory, f2, __PROC__)))
	    goto err;
      }
   _clip_retl(ClipMachineMemory, 1);
   return 0;
 err:
   return er;
}

int
clip_M6_EVALPARTIAL(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_EVALPARTIAL";
   int h = _clip_parni(ClipMachineMemory, 1);
   ClipVar *block = _clip_spar(ClipMachineMemory, 2);
   RDD_FILTER *fp;
   unsigned int ret;
   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKOPT2(2, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);
   if (block && (block->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType))
      block = NULL;

   fp = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RYO);
   if (!fp)
      {
	 er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
	 goto err;
      }
   if ((er = fp->rd->vtbl->_rlock(ClipMachineMemory, fp->rd, __PROC__)))
      goto err;
   if ((er = rm_evalpartial(ClipMachineMemory, fp, block, &ret, NULL, __PROC__)))
      goto err_unlock;
   if ((er = fp->rd->vtbl->_ulock(ClipMachineMemory, fp->rd, __PROC__)))
      goto err;
   _clip_retni(ClipMachineMemory, ret);
   return 0;
 err_unlock:
   fp->rd->vtbl->_ulock(ClipMachineMemory, fp->rd, __PROC__);
 err:
   return er;
}

int
clip_M6_FILTINVERSE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_FILTINVERSE";
   int h = _clip_parni(ClipMachineMemory, 1);
   RDD_FILTER *fp;
   unsigned int ret;
   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   fp = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RYO);
   if (!fp)
      {
	 er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
	 goto err;
      }
   if ((er = fp->rd->vtbl->_rlock(ClipMachineMemory, fp->rd, __PROC__)))
      goto err;
   if ((er = rm_evalpartial(ClipMachineMemory, fp, NULL, &ret, NULL, __PROC__)))
      goto err_unlock;
   if ((er = fp->rd->vtbl->_ulock(ClipMachineMemory, fp->rd, __PROC__)))
      goto err;
   if ((er = rm_inverse(ClipMachineMemory, fp, __PROC__)))
      goto err;
   _clip_retl(ClipMachineMemory, 1);
   return 0;
 err_unlock:
   fp->rd->vtbl->_ulock(ClipMachineMemory, fp->rd, __PROC__);
 err:
   return er;
}

int
clip_M6_FILTCOPY(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_FILTCOPY";
   int h = _clip_parni(ClipMachineMemory, 1);
   RDD_FILTER *fp;
   RDD_FILTER *ret;
   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   fp = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RYO);
   if (!fp)
      {
	 er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
	 goto err;
      }
   if ((er = rm_copyfilter(ClipMachineMemory, fp, &ret, __PROC__)))
      goto err;
   _clip_retni(ClipMachineMemory, ret->handle);
   return 0;
 err:
   return er;
}

int
clip_M6_FILTCOUNT(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_FILTCOUNT";
   int h = _clip_parni(ClipMachineMemory, 1);
   RDD_FILTER *fp;
   unsigned int tmp, cnt;
   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   fp = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RYO);
   if (!fp)
      {
	 er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
	 goto err;
      }

   if ((er = fp->rd->vtbl->_rlock(ClipMachineMemory, fp->rd, __PROC__)))
      goto err;
   if ((er = rm_evalpartial(ClipMachineMemory, fp, NULL, &tmp, &cnt, __PROC__)))
      goto err_unlock;
   if ((er = fp->rd->vtbl->_ulock(ClipMachineMemory, fp->rd, __PROC__)))
      goto err;
   _clip_retni(ClipMachineMemory, cnt);
   return 0;
 err_unlock:
   fp->rd->vtbl->_ulock(ClipMachineMemory, fp->rd, __PROC__);
 err:
   return er;
}

int
clip_M6_ISFILTREC(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_ISFILTREC";
   int h = _clip_parni(ClipMachineMemory, 1);
   RDD_FILTER *fp;
   unsigned int rec = _clip_parni(ClipMachineMemory, 2);
   int r, er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKARG1(2, NUMERIC_type_of_ClipVarType);
   fp = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RYO);
   if (!fp)
      {
	 er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
	 goto err;
      }

   if ((er = rm_isfiltrec(ClipMachineMemory, fp, rec, &r, __PROC__)))
      goto err;
   _clip_retl(ClipMachineMemory, r);
   return 0;
 err:
   return er;
}

int
clip_M6_FILTINFO(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_FILTINFO";
   int h = _clip_parni(ClipMachineMemory, 1);
   RDD_FILTER *fp;
   long vect[1];
   ClipVar *rp = RETPTR(ClipMachineMemory);
   ClipVar var;
   unsigned int tmp, cnt;
   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   fp = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RYO);
   if (!fp)
      {
	 er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
	 goto err;
      }

   vect[0] = 7;
   _clip_array(ClipMachineMemory, rp, 1, vect);

/*  INFO_EXPR (1) */
   vect[0] = INFO_EXPR - 1;
   memset(&var, 0, sizeof(ClipVar));
   var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = fp->sfilter;
   var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen(fp->sfilter);
   _clip_aset(ClipMachineMemory, rp, &var, 1, vect);

/*  INFO_NONEXPR (2) */
   vect[0] = INFO_NONEXPR - 1;
   memset(&var, 0, sizeof(ClipVar));
   var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   if (fp->optimize == 2)
      var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = "";
   else
      var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = fp->sfilter;
   var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen(var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
   _clip_aset(ClipMachineMemory, rp, &var, 1, vect);

/*  INFO_OPTLVL (3) */
   vect[0] = INFO_OPTLVL - 1;
   memset(&var, 0, sizeof(ClipVar));
   var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
   var.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = fp->optimize;
   _clip_aset(ClipMachineMemory, rp, &var, 1, vect);

/*  INFO_COUNT (4) */
   vect[0] = INFO_COUNT - 1;
   memset(&var, 0, sizeof(ClipVar));
   var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
   if ((er = rm_evalpartial(ClipMachineMemory, fp, NULL, &tmp, &cnt, __PROC__)))
      goto err;
   var.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = cnt;
   _clip_aset(ClipMachineMemory, rp, &var, 1, vect);

/*  INFO_SIZE (5) */
   vect[0] = INFO_SIZE - 1;
   memset(&var, 0, sizeof(ClipVar));
   var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
   var.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = fp->size;
   _clip_aset(ClipMachineMemory, rp, &var, 1, vect);

/*  INFO_OWNER (6) */
   vect[0] = INFO_OWNER - 1;
   memset(&var, 0, sizeof(ClipVar));
   var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
   var.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = (fp->active ? 1 : 2);
   _clip_aset(ClipMachineMemory, rp, &var, 1, vect);

/*  INFO_POS (7) */
   vect[0] = INFO_POS - 1;
   memset(&var, 0, sizeof(ClipVar));
   var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
   var.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = -1;
   _clip_aset(ClipMachineMemory, rp, &var, 1, vect);

   return 0;
 err:
   return er;
}

int
clip_M6_RECCOUNT(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_RECCOUNT";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   RDD_FILTER *fp;
   int i, cnt = 0, er;

   ClipMachineMemory->m6_error = 0;
   if (!wa)
      {
	 er = rdd_err(ClipMachineMemory, EG_NOTABLE, 0, __FILE__, __LINE__, __PROC__, er_notable);
	 goto err;
      }
   fp = wa->rd->filter;
   if (!fp)
      {
	 er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
	 goto err;
      }
   if (fp->rmap)
      {
	 for (i = 1; i <= fp->size; i++)
	    if (_rm_getbit(fp->rmap, fp->size, i))
	       cnt++;
      }

   _clip_retni(ClipMachineMemory, cnt);
   return 0;
 err:
   return er;
}

int
clip_M6_ADDSCOPED(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_ADDSCOPED";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int h = _clip_parni(ClipMachineMemory, 1);
   RDD_FILTER *fp;
   ClipVar *t = _clip_vptr(_clip_par(ClipMachineMemory, 2));
   ClipVar *b = _clip_vptr(_clip_par(ClipMachineMemory, 3));
   int ord = _clip_parni(ClipMachineMemory, 4) - 1;
   int opttype = _clip_parinfo(ClipMachineMemory, 5);
   int er, i, cnt = 0;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(4, NUMERIC_type_of_ClipVarType);
   CHECKOPT3(5, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   if (!wa)
      {
	 er = rdd_err(ClipMachineMemory, EG_NOTABLE, 0, __FILE__, __LINE__, __PROC__, er_notable);
	 goto err;
      }
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
   if (_clip_parinfo(ClipMachineMemory, 4) == UNDEF_type_of_ClipVarType)
      ord = wa->rd->curord;
   if (ord >= wa->rd->ords_opened)
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

   if ((er = rdd_flushbuffer(ClipMachineMemory, wa->rd, __PROC__)))
      goto err;
   READLOCK;
   if ((er = wa->rd->orders[ord]->vtbl->setscope(ClipMachineMemory, wa->rd, wa->rd->orders[ord], t, b, fp->rmap, fp->size, 0, __PROC__)))
      goto err_unlock;
   if (opttype)
      {
	 int found;
	 int oldrecno = wa->rd->recno;
	 int oldbof = wa->rd->bof;
	 int oldeof = wa->rd->eof;
	 int words = (fp->size >> 5) + 1;
	 int *tmap = calloc(4, words);
	 if (opttype == CHARACTER_type_of_ClipVarType)
	    {
	       if ((er = rdd_wildseek(ClipMachineMemory, wa->rd, _clip_parc(ClipMachineMemory, 5), 1, 0, &found, __PROC__)))
		  goto err_unlock;
	       while (!wa->rd->eof)
		  {
		     _rm_setbit(tmap, fp->size, wa->rd->recno);
		     if ((er = rdd_wildseek(ClipMachineMemory, wa->rd, _clip_parc(ClipMachineMemory, 5), 1, 1, &found, __PROC__)))
			goto err_unlock;
		  }
	    }
	 else if ((opttype == CCODE_type_of_ClipVarType) || (opttype == PCODE_type_of_ClipVarType))
	    {
	       if ((er = rdd_gotop(ClipMachineMemory, wa->rd, __PROC__)))
		  goto err_unlock;
	       while (!wa->rd->eof)
		  {
		     if ((er = rdd_seekeval(ClipMachineMemory, wa->rd, _clip_spar(ClipMachineMemory, 5), &found, __PROC__)))
			goto err_unlock;
		     if (found)
			_rm_setbit(tmap, fp->size, wa->rd->recno);
		     else
			break;
		     if ((er = rdd_skip(ClipMachineMemory, wa->rd, 1, __PROC__)))
			goto err_unlock;
		  }
	    }
	 if ((er = wa->rd->vtbl->rawgo(ClipMachineMemory, wa->rd, oldrecno, 0, __PROC__)))
	    goto err_unlock;
	 wa->rd->bof = oldbof;
	 wa->rd->eof = oldeof;
	 for (i = 0; i < words; i++)
	    fp->rmap[i] = fp->rmap[i] & tmap[i];
      }
   UNLOCK;
   for (i = 1; i <= fp->size; i++)
      if (_rm_getbit(fp->rmap, fp->size, i))
	 cnt++;

   _clip_retni(ClipMachineMemory, cnt);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_M6_CHGOWNER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_CHGOWNER";
   int h = _clip_parni(ClipMachineMemory, 1);
   RDD_FILTER *fp;
   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   fp = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RYO);
   if (!fp)
      {
	 er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
	 goto err;
      }
   fp->active = 0;
   _clip_retl(ClipMachineMemory, 1);
   return 0;
 err:
   return er;
}

int
clip_M6_FILTSAVE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_FILTSAVE";
   int h = _clip_parni(ClipMachineMemory, 1);
   char *fname = _clip_parc(ClipMachineMemory, 2);
   char *path = NULL;
   RDD_FILTER *fp;
   struct flock fl;
   char buf[4];
   int fd, er, bytes;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKARG1(2, CHARACTER_type_of_ClipVarType);
   fp = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RYO);
   if (!fp)
      {
	 er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
	 goto err;
      }
   if ((er = _rdd_parsepath(ClipMachineMemory, fname, ".flt", &path, NULL, EG_CREATE, __PROC__)))
      goto err;

#ifdef _WIN32
   fd = open(path, O_RDWR | O_BINARY, ClipMachineMemory->fileCreateMode);
#else
   fd = open(path, O_RDWR, ClipMachineMemory->fileCreateMode);
#endif
   if (fd == -1)
      {
#ifdef _WIN32
	 fd = open(path, O_CREAT | O_TRUNC | O_RDWR | O_BINARY, ClipMachineMemory->fileCreateMode);
#else
	 fd = open(path, O_CREAT | O_TRUNC | O_RDWR, ClipMachineMemory->fileCreateMode);
#endif
      }
   if (fd == -1)
      goto err_create;

   fl.l_type = F_WRLCK;
   fl.l_whence = SEEK_SET;
   fl.l_start = 0;
   fl.l_len = 1;

   if (fcntl(fd, F_SETLKW, &fl))
      {
	 er = rdd_err(ClipMachineMemory, EG_LOCK, errno, __FILE__, __LINE__, __PROC__, er_writelock);
	 goto err;
      }
   if (ftruncate(fd, 0))
      goto err_create;
   if (write(fd, "CFLT", 4) == -1)
      goto err_write;
   if (write(fd, &fp->custom, 1) == -1)
      goto err_write;
   _rdd_put_ushort((unsigned char *) buf, strlen(fp->sfilter));
   if (write(fd, buf, sizeof(short)) == -1)
      goto err_write;
   if (write(fd, fp->sfilter, _rdd_ushort((unsigned char *) buf) + 1) == -1)
      goto err_write;
   _rdd_put_uint((unsigned char *) buf, fp->size);
   if (write(fd, buf, sizeof(int)) == -1)
      goto err_write;
   if (fp->rmap)
      {
	 bytes = (((fp->size + 1) >> 5) + 1) << 2;
	 if (write(fd, fp->rmap, bytes) == -1)
	    goto err_write;
      }
   if (close(fd) == -1)
      goto err_write;
   _clip_retl(ClipMachineMemory, 1);
   free(path);
   return 0;
 err_create:
   er = rdd_err(ClipMachineMemory, EG_CREATE, errno, __FILE__, __LINE__, __PROC__, er_create);
   goto err;
 err_write:
   er = rdd_err(ClipMachineMemory, EG_WRITE, errno, __FILE__, __LINE__, __PROC__, er_write);
 err:
   if (path)
      free(path);
   return er;
}

int
clip_M6_FILTRESTORE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_FILTRESTORE";
   char *fname = _clip_parc(ClipMachineMemory, 1);
   char *path = NULL;
   RDD_FILTER *fp = NULL;
   struct flock fl;
   char buf[4];
   int fd, er, bytes;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, CHARACTER_type_of_ClipVarType);
   if ((er = _rdd_parsepath(ClipMachineMemory, fname, ".flt", &path, NULL, EG_OPEN, __PROC__)))
      goto err;

#ifdef _WIN32
   fd = open(path, O_RDONLY | O_BINARY, ClipMachineMemory->fileCreateMode);
#else
   fd = open(path, O_RDONLY, ClipMachineMemory->fileCreateMode);
#endif

   if (fd == -1)
      goto err_open;

   fl.l_type = F_RDLCK;
   fl.l_whence = SEEK_SET;
   fl.l_start = 0;
   fl.l_len = 1;

   if (fcntl(fd, F_SETLKW, &fl))
      {
	 er = rdd_err(ClipMachineMemory, EG_LOCK, errno, __FILE__, __LINE__, __PROC__, er_readlock);
	 goto err;
      }

   fp = calloc(1, sizeof(RDD_FILTER));
   fp->fps = calloc(1, sizeof(RDD_FPS));
   if (read(fd, buf, 4) == -1)
      goto err_read;
   if (memcmp(buf, "CFLT", 4))
      {
	 er = rdd_err(ClipMachineMemory, EG_ARG, errno, __FILE__, __LINE__, __PROC__, er_badfltfile);
	 goto err;
      }
   if (read(fd, &fp->custom, 1) == -1)
      goto err_read;
   if (read(fd, buf, sizeof(short)) == -1)
      goto err_read;
   fp->sfilter = (char *) calloc(1, _rdd_ushort((unsigned char *) buf) + 1);
   if (read(fd, fp->sfilter, _rdd_ushort((unsigned char *) buf) + 1) == -1)
      goto err_read;
   if (read(fd, buf, sizeof(int)) == -1)
      goto err_read;
   fp->size = _rdd_uint((unsigned char *) buf);
   if (fp->size)
      {
	 bytes = (((fp->size + 1) >> 5) + 1) << 2;
	 fp->rmap = calloc(1, bytes);
	 if (read(fd, fp->rmap, bytes) == -1)
	    goto err_read;
      }
   fp->nfps = 1;
   fp->handle = _clip_store_c_item(ClipMachineMemory, fp, _C_ITEM_TYPE_RYO, destroy_ryo);
   if (close(fd) == -1)
      goto err_read;
   free(path);
   _clip_retni(ClipMachineMemory, fp->handle);
   return 0;
 err_open:
   er = rdd_err(ClipMachineMemory, EG_OPEN, errno, __FILE__, __LINE__, __PROC__, er_open);
   goto err;
 err_read:
   er = rdd_err(ClipMachineMemory, EG_READ, errno, __FILE__, __LINE__, __PROC__, er_read);
 err:
   if (path)
      free(path);
   if (fp)
      {
	 if (fp->fps)
	    free(fp->fps);
	 if (fp->sfilter)
	    free(fp->sfilter);
	 if (fp->rmap)
	    free(fp->rmap);
	 free(fp);
      }
   return er;
}

int
clip_M6_ISOPTIMIZE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_ISOPTIMIZE";
   char *str = _clip_parc(ClipMachineMemory, 1);
   RDD_FILTER *fp = calloc(1, sizeof(RDD_FILTER));
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, CHARACTER_type_of_ClipVarType);
   ClipMachineMemory->m6_error = 0;
   if (!wa)
      {
	 ClipMachineMemory->m6_error = 2001;
	 _clip_retni(ClipMachineMemory, 0);
	 return 0;
      }
   if (!wa->rd->ords_opened)
      {
	 ClipMachineMemory->m6_error = 2005;
	 _clip_retni(ClipMachineMemory, 0);
	 return 0;
      }

   if (str)
      {
	 fp->sfilter = strdup(str);
      }
   READLOCK;
   if ((er = rdd_initrushmore(ClipMachineMemory, wa->rd, fp, NULL, 1, __PROC__)))
      goto err_unlock;
   UNLOCK;
   _clip_retni(ClipMachineMemory, fp->optimize);

   if (fp->rmap)
      free(fp->rmap);
   free(fp);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   if (fp->rmap)
      free(fp->rmap);
   free(fp);
   return er;
}

int
clip_M6_FILTGOREC(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_FILTGOREC";
   int h = _clip_parni(ClipMachineMemory, 1);
   int recno = _clip_parni(ClipMachineMemory, 2);
   RDD_FILTER *fp;
   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKARG1(2, NUMERIC_type_of_ClipVarType);
   fp = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RYO);
   if (!fp)
      {
	 er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
	 goto err;
      }
   if (fp->optimize)
      fp->recno = recno;
   _clip_retni(ClipMachineMemory, fp->recno);
   return 0;
 err:
   return er;
}

int
clip_M6_FILTSKIP(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_FILTSKIP";
   int h = _clip_parni(ClipMachineMemory, 1);
   int nrecs = _clip_parni(ClipMachineMemory, 2);
   RDD_FILTER *fp;
   int i, er, recno = 0;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, NUMERIC_type_of_ClipVarType);
   fp = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RYO);
   if (!fp)
      {
	 er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
	 goto err;
      }
   if (fp->optimize)
      {
	 recno = fp->recno;
	 if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
	    nrecs = 1;
	 if (nrecs < 0)
	    {
	       if (recno > fp->size)
		  recno = fp->size + 1;
	       for (i = 0; i > nrecs && recno > 0; i--)
		  {
		     while (--recno > 0 && !_rm_getbit(fp->rmap, fp->size, recno));
		  }
	       fp->recno = recno;
	    }
	 else
	    {
	       if (recno < 0)
		  recno = 0;
	       for (i = 0; i < nrecs && recno <= fp->size; i++)
		  {
		     while (++recno <= fp->size && !_rm_getbit(fp->rmap, fp->size, recno));
		  }
	       fp->recno = recno;
	       if (recno > fp->size)
		  recno = 0;
	    }
      }
   _clip_retni(ClipMachineMemory, recno);
   return 0;
 err:
   return er;
}

int
clip_M6_FILTTOP(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_FILTTOP";
   int h = _clip_parni(ClipMachineMemory, 1);
   RDD_FILTER *fp;
   int er, recno;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   fp = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RYO);
   if (!fp)
      {
	 er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
	 goto err;
      }
   fp->recno = recno = 0;
   if (fp->optimize)
      {
	 while (++recno <= fp->size && !_rm_getbit(fp->rmap, fp->size, recno));
	 if (recno <= fp->size)
	    fp->recno = recno;
      }
   _clip_retni(ClipMachineMemory, fp->recno);
   return 0;
 err:
   return er;
}

int
clip_M6_FILTBOTT(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_FILTBOTT";
   int h = _clip_parni(ClipMachineMemory, 1);
   RDD_FILTER *fp;
   int er, recno;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   fp = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RYO);
   if (!fp)
      {
	 er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
	 goto err;
      }
   fp->recno = 0;
   recno = fp->size + 1;
   if (fp->optimize)
      {
	 while (--recno > 0 && !_rm_getbit(fp->rmap, fp->size, recno));
	 fp->recno = recno;
      }
   _clip_retni(ClipMachineMemory, fp->recno);
   return 0;
 err:
   return er;
}

int
clip_M6_REFRESHFILTER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_REFRESHFILTER";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   RDD_FILTER *fp;
   int er;

   ClipMachineMemory->m6_error = 0;
   if (!wa)
      {
	 er = rdd_err(ClipMachineMemory, EG_NOTABLE, 0, __FILE__, __LINE__, __PROC__, er_notable);
	 goto err;
      }
   fp = wa->rd->filter;
   if (!fp)
      {
	 er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
	 goto err;
      }
   if (ClipMachineMemory->flags1 & OPTIMIZE_FLAG)
      {
	 if (fp->list)
	    {
	       free(fp->list);
	       fp->list = NULL;
	       fp->listlen = 0;
	    }
	 if ((er = rdd_flushbuffer(ClipMachineMemory, wa->rd, __PROC__)))
	    goto err;
	 READLOCK;
	 if ((er = rdd_initrushmore(ClipMachineMemory, wa->rd, fp, NULL, 0, __PROC__)))
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
clip_M6_VERSION(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_VERSION";
   int type = _clip_parni(ClipMachineMemory, 1);
   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKOPT1(1, NUMERIC_type_of_ClipVarType);
   switch (type)
      {
      case 1:
	 _clip_retdc(ClipMachineMemory, 2002, 10, 14);
	 break;
      case 2:
	 _clip_retc(ClipMachineMemory, "00:00a");
	 break;
      case 3:
	 _clip_retc(ClipMachineMemory, "Mach SIx by (c) SuccessWare, Inc. for CLIP, 1.0b, 10/14/2002, 00:00a");
	 break;
      default:
	 _clip_retc(ClipMachineMemory, "1.0b");
      }
   return 0;
 err:
   return er;
}

int
clip_M6_SETTEMP(ClipMachine * ClipMachineMemory)
{
   ClipMachineMemory->m6_error = 0;
   _clip_retc(ClipMachineMemory, ".");
   return 0;
}

int
clip_SX_BLOB2FILE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_FILE2BLOB";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   const char *filename = _clip_parc(ClipMachineMemory, 1);
   const char *fldname = _clip_parc(ClipMachineMemory, 2);
   char file[PATH_MAX];
   int fd, fno, er;
   ClipVar v;

   ClipMachineMemory->m6_error = 0;
   memset(&v, 0, sizeof(ClipVar));
   CHECKWA(wa);

   if (!wa->rd->memo)
      return rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_nomemo);

   for (fno = 0; fno < wa->rd->nfields; fno++)
      {
	 if (strcasecmp(wa->rd->fields[fno].name, fldname) == 0)
	    break;
      }
   if (fno >= wa->rd->nfields)
      return rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_nofield);

   if (wa->rd->fields[fno].type != 'M')
      return rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_notmemo);

   if ((er = rdd_flushbuffer(ClipMachineMemory, wa->rd, __PROC__)))
      goto err;
   READLOCK;
   if ((er = wa->rd->vtbl->getvalue(ClipMachineMemory, wa->rd, fno, &v, __PROC__)))
      goto err_unlock;
   UNLOCK;

   _clip_translate_path(ClipMachineMemory, filename, file, sizeof(file));
   fd = _clip_creat(ClipMachineMemory, file, O_RDWR, ClipMachineMemory->fileCreateMode, 1);
   if (fd == -1)
      goto err_create;
   if (write(fd, v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf) == -1)
      goto err_create;
   if (_clip_close(ClipMachineMemory, _clip_hashstr(file), fd) == -1)
      goto err_create;

   _clip_destroy(ClipMachineMemory, &v);
   _clip_retl(ClipMachineMemory, 1);
   return 0;

 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
   goto err;
 err_create:
   er = rdd_err(ClipMachineMemory, EG_CREATE, errno, __FILE__, __LINE__, __PROC__, file);
 err:
   _clip_destroy(ClipMachineMemory, &v);
   return er;
}

int
clip_SX_FILE2BLOB(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_FILE2BLOB";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   const char *filename = _clip_parc(ClipMachineMemory, 1);
   const char *fldname = _clip_parc(ClipMachineMemory, 2);
  /*int action = _clip_parni(ClipMachineMemory,3); */
   char file[PATH_MAX];
   void *buf;
   int fd, fno, er;
   struct stat st;
   ClipVar v;

   ClipMachineMemory->m6_error = 0;
   memset(&v, 0, sizeof(ClipVar));
   CHECKWA(wa);

   if (!wa->rd->memo)
      return rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_nomemo);

   for (fno = 0; fno < wa->rd->nfields; fno++)
      {
	 if (strcasecmp(wa->rd->fields[fno].name, fldname) == 0)
	    break;
      }
   if (fno >= wa->rd->nfields)
      return rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_nofield);

   if (wa->rd->fields[fno].type != 'M')
      return rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_notmemo);

   _clip_translate_path(ClipMachineMemory, filename, file, sizeof(file));
   if ((er = rdd_open(ClipMachineMemory, file, 1, 1, &fd, __PROC__)))
      goto err;

   if (fstat(fd, &st) == -1)
      goto err_open;
   buf = malloc(st.st_size);
   if (read(fd, buf, st.st_size) == -1)
      goto err_open;
   if (_clip_close(ClipMachineMemory, _clip_hashstr(file), fd) == -1)
      goto err_open;

   v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   v.ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   v.ClipType_t_of_ClipVar.memo_of_ClipType = 0;
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = buf;
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = st.st_size;
   WRITELOCK;
   if ((er = wa->rd->vtbl->setvalue(ClipMachineMemory, wa->rd, fno, &v, 1, __PROC__)))
      goto err_unlock;
   UNLOCK;

   _clip_destroy(ClipMachineMemory, &v);
   _clip_retl(ClipMachineMemory, 1);
   return 0;

 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err_open:
   er = rdd_err(ClipMachineMemory, EG_OPEN, errno, __FILE__, __LINE__, __PROC__, file);
 err:
   _clip_destroy(ClipMachineMemory, &v);
   return er;
}

int
clip_SX_MEMOPACK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_MEMOPACK";
   int bsize = _clip_parni(ClipMachineMemory, 1);
   ClipVar *block = _clip_spar(ClipMachineMemory, 2);
   int step = _clip_parni(ClipMachineMemory, 3);
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   char tmp[PATH_MAX];
   char *s;
   int tmpfd;
   struct stat st;
   int r, er;

   ClipMachineMemory->m6_error = 0;
   CHECKWA(wa);
   CHECKOPT1(1, NUMERIC_type_of_ClipVarType);
   CHECKOPT2(2, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);
   CHECKOPT1(3, NUMERIC_type_of_ClipVarType);

   if ((er = rdd_event(ClipMachineMemory, EVENT_PREMEMOPACK, wa->rd->area, 0, NULL, &r, __PROC__)))
      return er;
   if (!r)
      return 0;

   if (!wa->rd->memo)
      return 0;
   if (!wa->rd->memo->vtbl->pack)
      return 0;

   if (wa->rd->shared)
      {
	 er = rdd_err(ClipMachineMemory, EG_SHARED, 0, __FILE__, __LINE__, __PROC__, er_notpermitted);
	 goto err;
      }
   if (wa->rd->readonly)
      {
	 er = rdd_err(ClipMachineMemory, EG_READONLY, 0, __FILE__, __LINE__, __PROC__, er_notpermitted);
	 goto err;
      }

   if ((er = rdd_checkifnew(ClipMachineMemory, wa->rd, __PROC__)))
      goto err;

#ifdef HAVE_MMAN_H
   if (wa->rd->memo->file.md != (caddr_t) - 1)
      {
	 if (munmap(wa->rd->memo->file.md, wa->rd->memo->file.mapsize) == -1)
	    goto err_write;
      }
#endif

   if (_clip_close(ClipMachineMemory, wa->rd->memo->file.filehash, wa->rd->memo->file.fd) == -1)
      goto err_write;

   strcpy(tmp, wa->rd->memo->path);
   s = strrchr(tmp, '/');
   sprintf(s + 1, "memo%lx", (long) getpid());
   sprintf(s + 9, ".%lx", (long) random());
   s[13] = 0;

   if (rename(wa->rd->memo->path, tmp) == -1)
      goto err_write;

   wa->rd->memo->file.fd = _clip_creat(ClipMachineMemory, wa->rd->memo->path, O_RDWR, ClipMachineMemory->fileCreateMode, !wa->rd->shared);
   if (wa->rd->memo->file.fd == -1)
      goto err_open1;

   if ((er = rdd_open(ClipMachineMemory, tmp, 1, 1, &tmpfd, __PROC__)))
      goto err;

   if (wa->rd->memo->vtbl->pack)
      {
	 WRITELOCK;
	 if ((er = wa->rd->memo->vtbl->pack(ClipMachineMemory, wa->rd, wa->rd->memo, tmpfd, bsize, block, step, __PROC__)))
	    goto err_unlock;
	 UNLOCK;
      }
   if (_clip_close(ClipMachineMemory, _clip_hashstr(tmp), tmpfd) == -1)
      goto err_write;

   if (fstat(wa->rd->memo->file.fd, &st) == -1)
      goto err_write;
#ifdef HAVE_MMAN_H
   wa->rd->memo->file.mapsize = st.st_size;
   wa->rd->memo->file.md = (caddr_t) mmap(0, wa->rd->memo->file.mapsize, PROT_READ | PROT_WRITE, MAP_SHARED, wa->rd->memo->file.fd, 0);
#endif
   remove(tmp);

   if ((er = rdd_event(ClipMachineMemory, EVENT_POSTMEMOPACK, wa->rd->area, 0, NULL, NULL, __PROC__)))
      goto err;
   return 0;
 err_write:
   er = rdd_err(ClipMachineMemory, EG_WRITE, errno, __FILE__, __LINE__, __PROC__, wa->rd->memo->path);
   goto err;
 err_open1:
   er = rdd_err(ClipMachineMemory, EG_OPEN, errno, __FILE__, __LINE__, __PROC__, wa->rd->memo->path);
   goto err;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_SX_SETTRIGGER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_SETTRIGGER";
   int action = _clip_parni(ClipMachineMemory, 1);
   const char *trigger = _clip_parc(ClipMachineMemory, 2);
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKWA(wa);
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);

   _clip_retc(ClipMachineMemory, wa->trigger);
   switch (action)
      {
      case TRIGGER_ENABLE:
	 if (wa->trighash)
	    wa->trig_active = 1;
	 break;
      case TRIGGER_DISABLE:
	 wa->trig_active = 0;
	 break;
      case TRIGGER_REMOVE:
	 if (wa->trigger)
	    {
	       free(wa->trigger);
	       wa->trigger = NULL;
	    }
	 wa->trighash = 0;
	 break;
      case TRIGGER_INSTALL:
	 if (trigger && trigger[0])
	    {
	       if (wa->trigger)
		  {
		     free(wa->trigger);
		     wa->trigger = NULL;
		  }
	       wa->trighash = 0;
	       wa->trigger = strdup(trigger);
	       _clip_upstr(wa->trigger, strlen(wa->trigger));
	       wa->trighash = _clip_hashstr(wa->trigger);
	       wa->trig_active = 1;
	    }
	 break;
      }
   return 0;
 err:
   return er;
}

int
clip_RDD_COUNT(ClipMachine * ClipMachineMemory)
{
   ClipMachineMemory->m6_error = 0;
   _clip_retni(ClipMachineMemory, *ClipMachineMemory->ndbdrivers);
   return 0;
}

int
clip_RDD_NAME(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDD_NAME";
   int no = _clip_parni(ClipMachineMemory, 1);
   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

	_clip_retc(ClipMachineMemory, (*ClipMachineMemory->dbdrivers)[no - 1].id_of_DBDriver);
   return 0;
 err:
   return er;
}

int
clip_RDD_INFO(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDD_INFO";
   int type = _clip_parinfo(ClipMachineMemory, 1);
   int no, i, er;
   char *rdd;
   char err[100];
   ClipVar *ap = RETPTR(ClipMachineMemory);
   long dim[] = { 0 };
   ClipVar v;

   ClipMachineMemory->m6_error = 0;
   memset(&v, 0, sizeof(ClipVar));
   if (type == CHARACTER_type_of_ClipVarType)
      {
	 rdd = _clip_parc(ClipMachineMemory, 1);
	 for (no = 0; no < *ClipMachineMemory->ndbdrivers; no++)
		 if (strncasecmp(rdd, (*ClipMachineMemory->dbdrivers)[no].id_of_DBDriver, 6) == 0)
	       break;
	 if (no >= *ClipMachineMemory->ndbdrivers)
	    return 0;
      }
   else if (type == NUMERIC_type_of_ClipVarType)
      {
	 DBWorkArea *wa;

	 no = _clip_parni(ClipMachineMemory, 1) - 1;
	 if (no < 0 || no >= *ClipMachineMemory->ndbdrivers)
	    {
	       sprintf(err, bad_arg, 1);
	       er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, err);
	       goto err;
	    }
	 wa = (DBWorkArea *) ClipMachineMemory->areas->items_of_ClipVect[no];
	 for (no = 0; no < *ClipMachineMemory->ndbdrivers; no++)
		 if (strncasecmp(wa->driver, (*ClipMachineMemory->dbdrivers)[no].id_of_DBDriver, 6) == 0)
	       break;
      }
   else
      {
	 for (no = 0; no < *ClipMachineMemory->ndbdrivers; no++)
		 if (strncasecmp(ClipMachineMemory->def_db_driver, (*ClipMachineMemory->dbdrivers)[no].id_of_DBDriver, 6) == 0)
	       break;
      }

   _clip_array(ClipMachineMemory, ap, 1, dim);

   v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = strdup((*ClipMachineMemory->dbdrivers)[no].id_of_DBDriver);
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen(v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
   _clip_aadd(ClipMachineMemory, ap, &v);
   _clip_destroy(ClipMachineMemory, &v);

   v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
	v.ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = 1;
   _clip_aadd(ClipMachineMemory, ap, &v);
   _clip_destroy(ClipMachineMemory, &v);

   v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   for (i = 0; i < *ClipMachineMemory->ndata_drivers; i++)
		if (strncasecmp((*ClipMachineMemory->dbdrivers)[no].data_of_DBDriver, (*ClipMachineMemory->data_drivers)[i]->id, 3) == 0)
	 break;
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = strdup((*ClipMachineMemory->data_drivers)[i]->suff);
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen(v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
   _clip_aadd(ClipMachineMemory, ap, &v);
   _clip_destroy(ClipMachineMemory, &v);

   v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   for (i = 0; i < *ClipMachineMemory->nidx_drivers; i++)
		if (strncasecmp((*ClipMachineMemory->dbdrivers)[no].idx_of_DBDriver, (*ClipMachineMemory->idx_drivers)[i]->id, 3) == 0)
	 break;
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = strdup((*ClipMachineMemory->idx_drivers)[i]->sing_suff);
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen(v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
   _clip_aadd(ClipMachineMemory, ap, &v);
   _clip_destroy(ClipMachineMemory, &v);

   v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = strdup((*ClipMachineMemory->idx_drivers)[i]->suff);
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen(v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
   _clip_aadd(ClipMachineMemory, ap, &v);
   _clip_destroy(ClipMachineMemory, &v);

   v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   for (i = 0; i < *ClipMachineMemory->nmemo_drivers; i++)
		if (strncasecmp((*ClipMachineMemory->dbdrivers)[no].memo_of_DBDriver, (*ClipMachineMemory->memo_drivers)[i]->id, 3) == 0)
	 break;
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = strdup((*ClipMachineMemory->memo_drivers)[i]->suff);
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen(v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
   _clip_aadd(ClipMachineMemory, ap, &v);
   _clip_destroy(ClipMachineMemory, &v);

   v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   for (i = 0; i < *ClipMachineMemory->ndata_drivers; i++)
		if (strncasecmp((*ClipMachineMemory->dbdrivers)[no].data_of_DBDriver, (*ClipMachineMemory->data_drivers)[i]->id, 3) == 0)
	 break;
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = strdup((*ClipMachineMemory->data_drivers)[i]->desc);
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen(v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
   _clip_aadd(ClipMachineMemory, ap, &v);
   _clip_destroy(ClipMachineMemory, &v);

   v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   for (i = 0; i < *ClipMachineMemory->nidx_drivers; i++)
		if (strncasecmp((*ClipMachineMemory->dbdrivers)[no].idx_of_DBDriver, (*ClipMachineMemory->idx_drivers)[i]->id, 3) == 0)
	 break;
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = strdup((*ClipMachineMemory->idx_drivers)[i]->desc);
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen(v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
   _clip_aadd(ClipMachineMemory, ap, &v);
   _clip_destroy(ClipMachineMemory, &v);

   v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   for (i = 0; i < *ClipMachineMemory->nmemo_drivers; i++)
		if (strncasecmp((*ClipMachineMemory->dbdrivers)[no].memo_of_DBDriver, (*ClipMachineMemory->memo_drivers)[i]->id, 3) == 0)
	 break;
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = strdup((*ClipMachineMemory->memo_drivers)[i]->desc);
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen(v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
   _clip_aadd(ClipMachineMemory, ap, &v);
   _clip_destroy(ClipMachineMemory, &v);

   return 0;
 err:
   return er;
}

int
clip_SXCHAR(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SXCHAR";
   int len = _clip_parni(ClipMachineMemory, 1);
   ClipVar *v = _clip_par(ClipMachineMemory, 2);
   ClipVar *r = RETPTR(ClipMachineMemory);
   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   if (len < 0)
      len = 0;
   memset(r, 0, sizeof(ClipVar));
   r->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = malloc(len + 1);
   r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = len;
   switch (_clip_parinfo(ClipMachineMemory, 2))
      {
      case CHARACTER_type_of_ClipVarType:
	 {
	    int l = min(len, v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
	    memcpy(r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, l);
	    memset(r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf + l, ' ', len - l);
	    break;
	 }
      case NUMERIC_type_of_ClipVarType:
	 {
	    int d = min(v->ClipType_t_of_ClipVar.dec_of_ClipType, len - (v->ClipType_t_of_ClipVar.len_of_ClipType - v->ClipType_t_of_ClipVar.dec_of_ClipType));
	    char *s = r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
	    _clip_dtostr(s, len, d, v->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, 0);
	    while (*s == ' ')
	       s++;
	    memmove(r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, s, len - (s - r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf));
	    memset(r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf + len - (s - r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf), ' ', (s -
r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf));
	    break;
	 }
      case DATE_type_of_ClipVarType:
	 {
		 char *s = _clip_date_to_str(v->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar, "yyyymmdd");
	    int l = min(len, strlen(s));

	    memcpy(r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, s, l);
	    memset(r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf + l, ' ', len - l);
	    free(s);
	    break;
	 }
      case LOGICAL_type_of_ClipVarType:
	 {
	    if (len > 0)
	       {
				 *r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = v->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar ? 'T' : 'F';
		  memset(r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf + 1, ' ', len - 1);
	       }
	    break;
	 }
      default:
	 memset(r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, ' ', len);
	 break;
      }
   r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[len] = 0;
   return 0;
 err:
   return er;
}

int
clip_SXNUM(ClipMachine * ClipMachineMemory)
{
   ClipVar *v = _clip_par(ClipMachineMemory, 1);

   ClipMachineMemory->m6_error = 0;
   switch (_clip_parinfo(ClipMachineMemory, 1))
      {
      case NUMERIC_type_of_ClipVarType:
	 _clip_retnd(ClipMachineMemory, v->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
	 break;
      case CHARACTER_type_of_ClipVarType:
	 {
	    int d;
	    _clip_retnd(ClipMachineMemory, _clip_strtod(v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, &d));
	    RETPTR(ClipMachineMemory)->ClipType_t_of_ClipVar.dec_of_ClipType = max(0, v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf - d - 1);
	    break;
	 }
      case DATE_type_of_ClipVarType:
	 {
	    int dd, mm, yy, ww, a, b;
	    long r;
		 _clip_cdate(v->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar, &dd, &mm, &yy, &ww);

	    if (mm <= 2)
	       {
		  yy--;
		  mm += 12;
	       }
	    a = yy / 100;
	    b = 2 - a + a / 4;
	    r = ((long) (365.25 * yy - (yy ? 0.0 : 0.75)) + (long) (30.6001 * (mm + 1) + dd + 1720995 + b));
	    _clip_retnd(ClipMachineMemory, r);
	    break;
	 }
      case LOGICAL_type_of_ClipVarType:
			_clip_retnd(ClipMachineMemory, v->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar);
	 break;
      default:
	 _clip_retnd(ClipMachineMemory, 0);
	 break;
      }
   return 0;
}

int
clip_SXLOG(ClipMachine * ClipMachineMemory)
{
   ClipVar *v = _clip_par(ClipMachineMemory, 1);

   ClipMachineMemory->m6_error = 0;
   switch (_clip_parinfo(ClipMachineMemory, 1))
      {
      case LOGICAL_type_of_ClipVarType:
			_clip_retl(ClipMachineMemory, v->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar);
	 break;
      case CHARACTER_type_of_ClipVarType:
	 if (toupper(*v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf) == 'T' || toupper(*v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf) == 'Y')
	    _clip_retl(ClipMachineMemory, 1);
	 else
	    _clip_retl(ClipMachineMemory, 0);
	 break;
      case NUMERIC_type_of_ClipVarType:
	 _clip_retl(ClipMachineMemory, v->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar != 0);
	 break;
      default:
	 _clip_retl(ClipMachineMemory, 0);
	 break;
      }
   return 0;
}

int
clip_SXDATE(ClipMachine * ClipMachineMemory)
{
   ClipVar *v = _clip_par(ClipMachineMemory, 1);

   ClipMachineMemory->m6_error = 0;
   switch (_clip_parinfo(ClipMachineMemory, 1))
      {
      case DATE_type_of_ClipVarType:
			_clip_retdj(ClipMachineMemory, v->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar);
	 break;
      case CHARACTER_type_of_ClipVarType:
	 _clip_retdj(ClipMachineMemory, _clip_str_to_date(v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, ClipMachineMemory->date_format, ClipMachineMemory->epoch));
	 break;
      case NUMERIC_type_of_ClipVarType:
	 {
	    long alf, a, b, c, d, e, r, dd, mm, yy;
	    long ju = v->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
	    alf = (long) ((ju - 1867216.25) / 36524.25);
	    a = ju + 1L + alf - alf / 4;
	    b = a + 1524;
	    c = (long) ((b - 122.1) / 365.25);
	    d = (long) (365.25 * c);
	    e = (long) ((b - d) / 30.6001);
	    dd = b - d - (long) (30.6001 * e);
	    if (e < 14)
	       mm = e - 1;
	    else
	       mm = e - 13;
	    if (mm > 2)
	       yy = c - 4716;
	    else
	       yy = c - 4715;
	    r = _clip_jdate(dd, mm, yy);
	    _clip_retdj(ClipMachineMemory, r);
	    break;
	 }
      default:
	 _clip_retdj(ClipMachineMemory, 0);
	 break;
      }
   return 0;
}

int
clip_SX_CHILL(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_CHILL";
   ClipVar *order = _clip_par(ClipMachineMemory, 1);
   ClipVar *index = _clip_par(ClipMachineMemory, 2);
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int ord, er;

   ClipMachineMemory->m6_error = 0;
   CHECKWA(wa);
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);

   _clip_retl(ClipMachineMemory, 0);
   ord = get_orderno(wa, order, index);
   if (ord == -1)
      ord = wa->rd->curord;
   if (ord == -1)
      return 0;

   wa->rd->orders[ord]->custom = 0;
   wa->rd->orders[ord]->canadd = 0;

   _clip_retl(ClipMachineMemory, 1);
   return 0;
 err:
   return er;
}

int
clip_SX_FREEZE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_FREEZE";
   ClipVar *order = _clip_par(ClipMachineMemory, 1);
   ClipVar *index = _clip_par(ClipMachineMemory, 2);
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int ord, er;

   ClipMachineMemory->m6_error = 0;
   CHECKWA(wa);
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);

   _clip_retl(ClipMachineMemory, 0);
   ord = get_orderno(wa, order, index);
   if (ord == -1)
      ord = wa->rd->curord;
   if (ord == -1)
      return 0;

   wa->rd->orders[ord]->custom = 1;
   wa->rd->orders[ord]->canadd = 0;

   _clip_retl(ClipMachineMemory, 1);
   return 0;
 err:
   return er;
}

int
clip_SX_WARM(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_WARM";
   ClipVar *order = _clip_par(ClipMachineMemory, 1);
   ClipVar *index = _clip_par(ClipMachineMemory, 2);
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int ord, er;

   ClipMachineMemory->m6_error = 0;
   CHECKWA(wa);
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);

   _clip_retl(ClipMachineMemory, 0);
   ord = get_orderno(wa, order, index);
   if (ord == -1)
      ord = wa->rd->curord;
   if (ord == -1)
      return 0;

   wa->rd->orders[ord]->custom = 0;
   wa->rd->orders[ord]->canadd = 1;

   _clip_retl(ClipMachineMemory, 1);
   return 0;
 err:
   return er;
}

int
clip_SX_THERMOMETER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_WARM";
   ClipVar *order = _clip_par(ClipMachineMemory, 1);
   ClipVar *index = _clip_par(ClipMachineMemory, 2);
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int ord, er;

   ClipMachineMemory->m6_error = 0;
   CHECKWA(wa);
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);

   _clip_retni(ClipMachineMemory, 0);
   ord = get_orderno(wa, order, index);
   if (ord == -1)
      ord = wa->rd->curord;
   if (ord == -1)
      return 0;

   if (wa->rd->orders[ord]->custom && !wa->rd->orders[ord]->canadd)
      _clip_retni(ClipMachineMemory, 4);
   else if (!wa->rd->orders[ord]->custom && !wa->rd->orders[ord]->canadd)
      _clip_retni(ClipMachineMemory, 3);
   else if (!wa->rd->orders[ord]->custom && wa->rd->orders[ord]->canadd)
      _clip_retni(ClipMachineMemory, 1);
   return 0;
 err:
   return er;
}

int
clip_SX_CLEARORDER(ClipMachine * ClipMachineMemory)
{
/* dummy */
   ClipMachineMemory->m6_error = 0;
   _clip_retl(ClipMachineMemory, 0);
   return 0;
}

int
clip_SX_WILDMATCH(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_WILDMATCH";
   const char *pattern = _clip_parc(ClipMachineMemory, 1);
   const char *string = _clip_parc(ClipMachineMemory, 2);
   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, CHARACTER_type_of_ClipVarType);
   CHECKARG1(2, CHARACTER_type_of_ClipVarType);

   _clip_retl(ClipMachineMemory, _clip_glob_match(string, pattern, 1) != -1);
   return 0;
 err:
   return er;
}

int
clip_SX_WILDSEEK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_WILDSEEK";
   const char *pattern = _clip_parc(ClipMachineMemory, 1);
   int cont = _clip_parl(ClipMachineMemory, 2);
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int found, er;

   ClipMachineMemory->m6_error = 0;
   CHECKWA(wa);
   if (_clip_parinfo(ClipMachineMemory, 1) != CHARACTER_type_of_ClipVarType)
      {
	 _clip_retl(ClipMachineMemory, 0);
	 return 0;
      }
   CHECKOPT1(2, LOGICAL_type_of_ClipVarType);

   if ((er = rdd_flushbuffer(ClipMachineMemory, wa->rd, __PROC__)))
      return er;
   READLOCK;
   if (rdd_wildseek(ClipMachineMemory, wa->rd, pattern, 0, cont, &found, __PROC__))
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
clip_SX_FNAMEPARSER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_FNAMEPARSER";
   const char *fname = _clip_parc(ClipMachineMemory, 1);
   int lpath = _clip_parl(ClipMachineMemory, 2);
   int lext = _clip_parl(ClipMachineMemory, 3);
   const char *s, *e;
   char *r;
   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(2, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(3, LOGICAL_type_of_ClipVarType);

   s = fname;
   e = fname + strlen(fname);
   if (!lpath && !lext)
      {
	 s = max(s, strrchr(fname, '/'));
	 s = max(s, strrchr(fname, '\\'));
	 if (s > fname)
	    s++;
	 e = strrchr(fname, '.');
	 if (!(e && *(e + 1) != '/' && *(e + 1) != '\\'))
	    e = NULL;
	 if (e < s)
	    e = fname + strlen(fname);
      }
   else if (!lpath && lext)
      {
	 s = max(s, strrchr(fname, '/'));
	 s = max(s, strrchr(fname, '\\'));
	 if (s > fname)
	    s++;
      }
   else if (lpath && !lext)
      {
	 e = strrchr(fname, '.');
	 if (!(e && *(e + 1) != '/' && *(e + 1) != '\\'))
	    e = NULL;
	 if (e < s)
	    e = fname + strlen(fname);
      }
   r = calloc(1, e - s + 1);
   memcpy(r, s, e - s);
   _clip_retcn_m(ClipMachineMemory, r, e - s);
   return 0;
 err:
   return er;
}

int
clip_SX_I_INDEXNAME(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_I_INDEXNAME";
   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipMachineMemory->m6_error = 0;
   CHECKWA(wa);
   _clip_retc(ClipMachineMemory, wa->rd->indexing);
   return 0;
}

int
clip_SX_I_TAGNAME(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_I_TAGNAME";
   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipMachineMemory->m6_error = 0;
   CHECKWA(wa);
   _clip_retc(ClipMachineMemory, wa->rd->tagindexing);
   return 0;
}

int
clip_SX_ISREINDEX(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_ISREINDEX";
   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipMachineMemory->m6_error = 0;
   CHECKWA(wa);
   _clip_retl(ClipMachineMemory, wa->rd->reindexing);
   return 0;
}

int
clip_SX_INDEXCOUNT(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_INDEXCOUNT";
   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipMachineMemory->m6_error = 0;
   CHECKWA(wa);
   _clip_retni(ClipMachineMemory, wa->rd->idxs_opened);
   return 0;
}

int
clip_SX_INDEXNAME(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_INDEXNAME";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   ClipVar *order = _clip_par(ClipMachineMemory, 1);
   int ord, er;

   ClipMachineMemory->m6_error = 0;
   CHECKWA(wa);
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);

   _clip_retc(ClipMachineMemory, "");
   ord = get_orderno(wa, order, NULL);
   if (ord == -1)
      ord = wa->rd->curord;
   if (ord == -1)
      return 0;
   _clip_retc(ClipMachineMemory, wa->rd->orders[ord]->index->name);
   return 0;
 err:
   return er;
}

int
clip_SX_TABLENAME(ClipMachine * ClipMachineMemory)
{
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   char *s = wa->rd->path;

   ClipMachineMemory->m6_error = 0;
   _clip_retc(ClipMachineMemory, "");
   if (!wa)
      return 0;

   s = max(s, strrchr(wa->rd->path, '/'));
   s = max(s, strrchr(wa->rd->path, '\\'));
   if (s > wa->rd->path)
      s++;
   _clip_retc(ClipMachineMemory, s);
   return 0;
}

int
clip_SX_INDEXTYPE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_INDEXTYPE";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   ClipVar *index = _clip_par(ClipMachineMemory, 1);
   int ord, er;
   ClipVar order;

   ClipMachineMemory->m6_error = 0;
   CHECKWA(wa);
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);

   _clip_retni(ClipMachineMemory, 0);
   memset(&order, 0, sizeof(ClipVar));
   order.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
   order.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = 1;
   if (!index)
      ord = -1;
   else
      ord = get_orderno(wa, &order, index);
   if (ord == -1)
      ord = wa->rd->curord;
   if (ord == -1)
      return 0;
   _clip_retni(ClipMachineMemory, wa->rd->orders[ord]->vtbl->ismulti ? 3 : 2);
   return 0;
 err:
   return er;
}

int
clip_SX_ISFLOCKED(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_ISFLOCKED";
   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipMachineMemory->m6_error = 0;
   CHECKWA(wa);
   _clip_retl(ClipMachineMemory, wa->rd->flocked);
   return 0;
}

int
clip_SX_ISLOCKED(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_ISLOCKED";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int recno = _clip_parni(ClipMachineMemory, 1);
   int r = 0, er;

   ClipMachineMemory->m6_error = 0;
   CHECKWA(wa);
   CHECKOPT1(1, NUMERIC_type_of_ClipVarType);

   if (!recno)
      recno = wa->rd->recno;

   if ((er = wa->rd->vtbl->rlocked(ClipMachineMemory, wa->rd, recno, &r, __PROC__)))
      goto err;
   if (!r)
      {
	 if ((er = wa->rd->vtbl->forlock(ClipMachineMemory, wa->rd, recno, &r, __PROC__)))
	    goto err;
      }
   _clip_retl(ClipMachineMemory, r);
   return 0;
 err:
   return er;
}

int
clip_SX_ISREADONLY(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_ISREADONLY";
   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipMachineMemory->m6_error = 0;
   CHECKWA(wa);

   _clip_retl(ClipMachineMemory, wa->rd->readonly);
   return 0;
}

int
clip_SX_ISSHARED(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_ISSHARED";
   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipMachineMemory->m6_error = 0;
   CHECKWA(wa);

   _clip_retl(ClipMachineMemory, wa->rd->shared);
   return 0;
}

int
clip_SX_KEYGOTO(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_KEYGOTO";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   ClipVar *order = _clip_par(ClipMachineMemory, 1);
   ClipVar *index = _clip_par(ClipMachineMemory, 2);
   unsigned int keyno = _clip_parni(ClipMachineMemory, 3);
   int ord, ok, er;

   ClipMachineMemory->m6_error = 0;
   CHECKWA(wa);
   CHECKARG1(3, NUMERIC_type_of_ClipVarType);

   ord = get_orderno(wa, order, index);
   _clip_retl(ClipMachineMemory, 0);

   if ((er = rdd_flushbuffer(ClipMachineMemory, wa->rd, __PROC__)))
      goto err;
   READLOCK;
   if (ord == -1)
      {
	 if ((er = rdd_goto(ClipMachineMemory, wa->rd, keyno, __PROC__)))
	    goto err_unlock;
	 ok = 1;
      }
   else
      {
	 if ((er = rdd_gotokey(ClipMachineMemory, wa->rd, wa->rd->orders[ord], keyno, &ok, __PROC__)))
	    goto err_unlock;
      }
   UNLOCK;

   _clip_retl(ClipMachineMemory, ok);
   return 0;

 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_SX_KEYSINCLUDED(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_KEYSINCLUDED";
   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipMachineMemory->m6_error = 0;
   CHECKWA(wa);

   _clip_retni(ClipMachineMemory, wa->rd->keysincluded);
   return 0;
}

int
clip_SX_KILLTAG(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_KILLTAG";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int lAll = _clip_parl(ClipMachineMemory, 1);
   ClipVar *order = _clip_par(ClipMachineMemory, 1);
   ClipVar *index = _clip_par(ClipMachineMemory, 2);
   int t1 = _clip_parinfo(ClipMachineMemory, 1);
   RDD_INDEX *ri;
   int ord = 0, i, er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG2(1, CHARACTER_type_of_ClipVarType, LOGICAL_type_of_ClipVarType);
   _clip_retl(ClipMachineMemory, 0);
   CHECKWA(wa);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);

   if (t1 == LOGICAL_type_of_ClipVarType)
      {
	 if (!lAll)
	    return 0;
      }
   else
      {
	 lAll = 0;
      }

   if (lAll && (_clip_parinfo(ClipMachineMemory, 2) == CHARACTER_type_of_ClipVarType))
      {
	 ri = NULL;
	 for (i = 0; i < wa->rd->idxs_opened; i++)
	    {
	       if (strcasecmp(wa->rd->indices[i]->name, index->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf) == 0)
		  {
		     ri = wa->rd->indices[i];
		     break;
		  }
	    }
	 if (!ri)
	    return 0;
      }
   else
      {
	 ord = get_orderno(wa, order, index);
	 if (ord == -1)
	    ord = wa->rd->curord;
	 if (ord == -1)
	    return 0;
	 ri = wa->rd->orders[ord]->index;
      }

   WRITELOCK;
   if (lAll)
      {
	 while (ri->norders > 0)
	    {
	       if ((er = rdd_orddestroy(ClipMachineMemory, wa->rd, ri->orders[0], __PROC__)))
		  goto err_unlock;
	    }
      }
   else
      {
	 if ((er = rdd_orddestroy(ClipMachineMemory, wa->rd, wa->rd->orders[ord], __PROC__)))
	    goto err_unlock;
      }
   UNLOCK;

   if (ri->norders == 0)
      {
#ifdef HAVE_MMAN_H
	 if ((ri->file.md) != (caddr_t) - 1)
	    {
	       if (munmap(ri->file.md, ri->file.mapsize) == -1)
		  goto err_close;
	    }
#endif
	 if (_clip_close(ClipMachineMemory, ri->file.filehash, ri->file.fd) == -1)
	    goto err_close;
	 if (remove(ri->path) == -1)
	    goto err_close;
	 if ((er = ri->vtbl->close(ClipMachineMemory, wa->rd, ri, __PROC__)))
	    return er;
	 for (i = ri->indexno; i < wa->rd->idxs_opened - 1; i++)
	    {
	       wa->rd->indices[i] = wa->rd->indices[i + 1];
	       wa->rd->indices[i]->indexno = i;
	    }
	 wa->rd->idxs_opened--;
	 wa->rd->indices = (RDD_INDEX **) realloc(wa->rd->indices, wa->rd->idxs_opened * sizeof(RDD_INDEX *));
      }
   _clip_retl(ClipMachineMemory, 1);
   return 0;

 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
   goto err;
 err_close:
   er = rdd_err(ClipMachineMemory, EG_CLOSE, errno, __FILE__, __LINE__, __PROC__, ri->path);
 err:
   return er;
}

int
clip_SX_LOCKRETRY(ClipMachine * ClipMachineMemory)
{
/* dummy */
   ClipMachineMemory->m6_error = 0;
   _clip_retni(ClipMachineMemory, 0);
   return 0;
}

int
clip_SX_MEMOEXT(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_MEMOEXT";
   const char *ext = _clip_parc(ClipMachineMemory, 1);
   int i, j, er;

   ClipMachineMemory->m6_error = 0;
   CHECKOPT1(1, CHARACTER_type_of_ClipVarType);

   for (i = 0; i < *ClipMachineMemory->ndbdrivers; i++)
      {
			if (strncasecmp(ClipMachineMemory->def_db_driver, (*ClipMachineMemory->dbdrivers)[i].id_of_DBDriver, 6) == 0)
	    {
	       for (j = 0; j < *ClipMachineMemory->nmemo_drivers; j++)
		  {
			  if (strncasecmp((*ClipMachineMemory->memo_drivers)[j]->id, (*ClipMachineMemory->dbdrivers)[i].memo_of_DBDriver, sizeof((*ClipMachineMemory->memo_drivers)[j]->id)) == 0)
			{

			   _clip_retc(ClipMachineMemory, (*ClipMachineMemory->memo_drivers)[j]->suff);
			   if (ext && ext[0])
			      {
				 memset((*ClipMachineMemory->memo_drivers)[j]->suff, 0, 5);
				 strncpy((*ClipMachineMemory->memo_drivers)[j]->suff, ext, min(4, strlen(ext)));
				 for (i = 0; i < 4; i++)
				    (*ClipMachineMemory->memo_drivers)[j]->suff[i] = tolower((*ClipMachineMemory->memo_drivers)[j]->suff[i]);
			      }
			   break;
			}
		  }
	       break;
	    }
      }
   return 0;
 err:
   return er;
}

int
clip_SX_RLOCK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_RLOCK";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int t = _clip_parinfo(ClipMachineMemory, 1);
   ClipVar *ap, *vp;
   int ok = 1, aok, i, er;

   ClipMachineMemory->m6_error = 0;
   CHECKOPT2(1, ARRAY_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   _clip_retl(ClipMachineMemory, 0);
   if (!wa)
      return 0;

   READLOCK;
   if (t == ARRAY_type_of_ClipVarType)
      {
	 ap = _clip_vptr(_clip_par(ClipMachineMemory, 1));
	for (i = 0; i < ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar; i++)
	    {
			 vp = _clip_vptr(&ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar[i]);
	       if ((er = rdd_rlock(ClipMachineMemory, wa->rd, (unsigned int) vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, &aok, __PROC__)))
		  goto err_unlock;
	       if (!aok)
		  ok = 0;
	    }
      }
   else if (t == NUMERIC_type_of_ClipVarType)
      {
	 if ((er = rdd_rlock(ClipMachineMemory, wa->rd, _clip_parni(ClipMachineMemory, 1), &ok, __PROC__)))
	    goto err_unlock;
      }
   else if (t == UNDEF_type_of_ClipVarType)
      {
	 if ((er = rdd_rlock(ClipMachineMemory, wa->rd, wa->rd->recno, &ok, __PROC__)))
	    goto err_unlock;
      }
   _clip_retl(ClipMachineMemory, ok);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_SX_ROLLBACK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_ROLLBACK";
   int nwa = _clip_parni(ClipMachineMemory, 1);
   DBWorkArea *wa;
   int i, j, er;

   ClipMachineMemory->m6_error = 0;
   CHECKOPT1(1, NUMERIC_type_of_ClipVarType);
   if (nwa > 0)
      wa = get_area(ClipMachineMemory, nwa, 0);
   else
      wa = cur_area(ClipMachineMemory);

   _clip_retl(ClipMachineMemory, 0);
   if (!wa)
      return 0;

   if (nwa > 0)
      wa = get_area(ClipMachineMemory, nwa, 0);
   else
      wa = cur_area(ClipMachineMemory);

   WRITELOCK;
   for (i = 0; i < wa->rd->nfields; i++)
      {
	 if (wa->rd->data[i])
	    {
	       if ((er = rdd_setvalue(ClipMachineMemory, wa->rd, i, wa->rd->data[i], __PROC__)))
		  goto err_unlock;
	       _clip_destroy(ClipMachineMemory, wa->rd->data[i]);
	       free(wa->rd->data[i]);
	       wa->rd->data[i] = NULL;
	    }
      }
   for (j = 0; j < wa->rd->rels_opened; j++)
      {
	 RDD_DATA *child = wa->rd->relations[j]->child;
	 for (i = 0; i < child->nfields; i++)
	    {
	       if (child->data[i])
		  {
		     if ((er = rdd_setvalue(ClipMachineMemory, child, i, child->data[i], __PROC__)))
			goto err_unlock;
		     _clip_destroy(ClipMachineMemory, child->data[i]);
		     free(child->data[i]);
		     child->data[i] = NULL;
		  }
	    }
      }
   UNLOCK;

   _clip_retl(ClipMachineMemory, 1);
   return 0;

 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}

int
clip_SX_SEEKLAST(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_SEEKLAST";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   ClipVar *v = _clip_par(ClipMachineMemory, 1);
   int soft = _clip_parl(ClipMachineMemory, 2);
   int found, er;

   ClipMachineMemory->m6_error = 0;
   CHECKOPT1(2, LOGICAL_type_of_ClipVarType);

   if (!wa)
      return 0;
   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      soft = (ClipMachineMemory->flags & SOFTSEEK_FLAG);

   if ((er = rdd_flushbuffer(ClipMachineMemory, wa->rd, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_seek(ClipMachineMemory, wa->rd, v, soft, 1, &found, __PROC__)))
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
clip_SX_FILEORDER(ClipMachine * ClipMachineMemory)
{
   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipMachineMemory->m6_error = 0;
   _clip_retni(ClipMachineMemory, 0);
   if (!wa)
      return 0;

   if (wa->rd->curord != -1)
      _clip_retni(ClipMachineMemory, wa->rd->orders[wa->rd->curord]->index->indexno + 1);
   return 0;
}

int
clip_SX_SETFILEORD(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_SETFILEORD";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int ord = _clip_parni(ClipMachineMemory, 1) - 1;
   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   if (!wa)
      return 0;

   if (wa->rd->curord == -1)
      _clip_retni(ClipMachineMemory, 0);
   else
      _clip_retni(ClipMachineMemory, wa->rd->orders[wa->rd->curord]->index->indexno + 1);

   if (ord >= 0 && ord < wa->rd->idxs_opened)
      {
	 if ((er = rdd_setorder(ClipMachineMemory, wa->rd, wa->rd->indices[ord]->orders[0]->orderno + 1, __PROC__)))
	    goto err;
      }
   else if (ord == -1)
      {
	 if ((er = rdd_setorder(ClipMachineMemory, wa->rd, 0, __PROC__)))
	    goto err;
      }
   return 0;
 err:
   return er;
}

int
clip_SX_SETTAG(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_SETTAG";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   ClipVar *order = _clip_par(ClipMachineMemory, 1);
   ClipVar *index = _clip_par(ClipMachineMemory, 2);
   int ord, er;

   ClipMachineMemory->m6_error = 0;
   if (!wa)
      return 0;
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);

   _clip_retl(ClipMachineMemory, 0);

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

   if ((er = rdd_flushbuffer(ClipMachineMemory, wa->rd, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_setorder(ClipMachineMemory, wa->rd, ord + 1, __PROC__)))
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
clip_SX_SETTAGNO(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_SETTAGNO";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   ClipVar *order = _clip_par(ClipMachineMemory, 1);
   ClipVar *index = _clip_par(ClipMachineMemory, 2);
   ClipVar in;
   int ord, er;

   ClipMachineMemory->m6_error = 0;
   if (!wa)
      return 0;
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);

   _clip_retl(ClipMachineMemory, 0);

   if (_clip_parinfo(ClipMachineMemory, 2) == 0)
      {
	 index = &in;
	 if (wa->rd->curord == -1)
	    return 0;
	 memset(index, 0, sizeof(ClipVar));
	 index->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	 index->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = wa->rd->orders[wa->rd->curord]->index->indexno;
      }

   ord = get_orderno(wa, order, index);
   if (ord < -1 || ord >= wa->rd->ords_opened || _clip_parinfo(ClipMachineMemory, 0) == 0)
      return 0;

   if ((er = rdd_flushbuffer(ClipMachineMemory, wa->rd, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_setorder(ClipMachineMemory, wa->rd, ord + 1, __PROC__)))
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
clip_SX_SLIMFAST(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_SLIMFAST";
   const char *str = _clip_parc(ClipMachineMemory, 1);
   const char *s;
   char *ret, *r;
   char quote = 0;
   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, CHARACTER_type_of_ClipVarType);

   if (!str)
      str = "";

   ret = calloc(1, strlen(str) + 1);
   for (s = str, r = ret; *s; s++)
      {
	 if (quote && ((*s == quote) || ((quote == '[') && (*s == ']'))))
	    {
	       quote = 0;
	       *(r++) = *s;
	       continue;
	    }
	 if (*s == '[' || *s == '"' || *s == '\'')
	    quote = *s;
	 if (quote)
	    {
	       *(r++) = *s;
	       continue;
	    }
	 if ((*s == ' ') && ((r > ret) && (*(r - 1) == ' ')))
	    continue;
	 *(r++) = _clip_toupper(*s);
      }
   *r = 0;
   _clip_retc(ClipMachineMemory, ret);
   free(ret);
   return 0;
 err:
   return er;
}

int
clip_SX_STEP(ClipMachine * ClipMachineMemory)
{
   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipMachineMemory->m6_error = 0;
   _clip_retni(ClipMachineMemory, 0);
   if (!wa)
      return 0;

   _clip_retni(ClipMachineMemory, wa->rd->os.nInterval);
   return 0;
}

int
clip_SX_TAGCOUNT(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_TAGCOUNT";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   ClipVar *index = _clip_par(ClipMachineMemory, 1);
   RDD_INDEX *ri = NULL;
   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);

   _clip_retni(ClipMachineMemory, 0);
   if (!wa)
      return 0;

   if (!index || index->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType)
      {
	 if (wa->rd->curord == -1)
	    {
	       if (wa->rd->idxs_opened > 0 && wa->rd->indices[0]->structural)
		  ri = wa->rd->indices[0];
	       else
		  return 0;
	    }
	 else
	    {
	       ri = wa->rd->orders[wa->rd->curord]->index;
	    }
      }
   else if (index->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
      {
	 int no = index->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar - 1;
	 if (no >= 0 && no < wa->rd->idxs_opened)
	    ri = wa->rd->indices[no];
      }
   else
      {
	 int i;

	 for (i = 0; i < wa->rd->idxs_opened; i++)
	    {
	       if (strcasecmp(wa->rd->indices[i]->name, index->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf) == 0)
		  {
		     ri = wa->rd->indices[i];
		     break;
		  }
	    }
      }

   if (!ri)
      return 0;

   _clip_retni(ClipMachineMemory, ri->norders);
   return 0;
 err:
   return er;
}

int
clip_SX_TAGNO(ClipMachine * ClipMachineMemory)
{
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   RDD_ORDER *ro;
   RDD_INDEX *ri;
   int i;

   ClipMachineMemory->m6_error = 0;
   _clip_retni(ClipMachineMemory, 0);

   if (!wa)
      return 0;

   if (wa->rd->curord == -1)
      return 0;

   ro = wa->rd->orders[wa->rd->curord];
   ri = ro->index;
   for (i = 0; i < ri->norders; i++)
      {
	 if (ro == ri->orders[i])
	    {
	       _clip_retni(ClipMachineMemory, i + 1);
	       break;
	    }
      }
   return 0;
}

int
clip_SX_TAGINFO(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_TAGINFO";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   ClipVar *index = _clip_par(ClipMachineMemory, 1);
   RDD_INDEX *ri = NULL;
   long vect[2] = { 0, 0 };
   ClipVar *rp = RETPTR(ClipMachineMemory);
   int i, er;

   ClipMachineMemory->m6_error = 0;
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);

   if (!wa)
      {
	 _clip_array(ClipMachineMemory, rp, 1, vect);
	 return 0;
      }

   if (!index || index->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType)
      {
	 if (wa->rd->curord == -1)
	    {
	       if (wa->rd->idxs_opened > 0 && wa->rd->indices[0]->structural)
		  {
		     ri = wa->rd->indices[0];
		  }
	       else
		  {
		     _clip_array(ClipMachineMemory, rp, 1, vect);
		     return 0;
		  }
	    }
	 else
	    {
	       ri = wa->rd->orders[wa->rd->curord]->index;
	    }
      }
   else if (index->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
      {
	 int no = index->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar - 1;
	 if (no >= 0 && no < wa->rd->idxs_opened)
	    ri = wa->rd->indices[no];
      }
   else
      {
	 int i;

	 for (i = 0; i < wa->rd->idxs_opened; i++)
	    {
	       if (strcasecmp(wa->rd->indices[i]->name, index->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf) == 0)
		  {
		     ri = wa->rd->indices[i];
		     break;
		  }
	    }
      }

   if (!ri)
      {
	 _clip_array(ClipMachineMemory, rp, 1, vect);
	 return 0;
      }

   vect[0] = ri->norders;
   vect[1] = 6;
   _clip_array(ClipMachineMemory, rp, 2, vect);

   for (i = 0; i < ri->norders; i++)
      {
	 ClipVar v;
	 RDD_ORDER *ro = ri->orders[i];

	 vect[0] = i;
	 memset(&v, 0, sizeof(ClipVar *));

	 v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	 v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = ro->name;
	 v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen(ro->name);
	 vect[1] = 0;
	 _clip_aset(ClipMachineMemory, rp, &v, 2, vect);

	 v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	 v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = ro->expr;
	 v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen(ro->expr);
	 vect[1] = 1;
	 _clip_aset(ClipMachineMemory, rp, &v, 2, vect);

	 v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	 v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = ro->cforexpr;
	 v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = ro->cforexpr ? strlen(ro->cforexpr) : 0;
	 vect[1] = 2;
	 _clip_aset(ClipMachineMemory, rp, &v, 2, vect);

	 v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
	 v.ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = ro->unique;
	 vect[1] = 3;
	 _clip_aset(ClipMachineMemory, rp, &v, 2, vect);

	 v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
	 v.ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = ro->descend;
	 vect[1] = 4;
	 _clip_aset(ClipMachineMemory, rp, &v, 2, vect);

	 v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
	 v.ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = ro->custom;
	 vect[1] = 5;
	 _clip_aset(ClipMachineMemory, rp, &v, 2, vect);
      }
   return 0;
 err:
   return er;
}

int
clip_SX_TAGS(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_TAGS";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   ClipVar *index = _clip_par(ClipMachineMemory, 1);
   RDD_INDEX *ri = NULL;
   long vect[1] = { 0 };
   ClipVar *rp = RETPTR(ClipMachineMemory);
   int i, er;

   ClipMachineMemory->m6_error = 0;
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);

   if (!wa)
      {
	 _clip_array(ClipMachineMemory, rp, 1, vect);
	 return 0;
      }

   if (!index || index->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType)
      {
	 if (wa->rd->curord == -1)
	    {
	       if (wa->rd->idxs_opened > 0 && wa->rd->indices[0]->structural)
		  {
		     ri = wa->rd->indices[0];
		  }
	       else
		  {
		     _clip_array(ClipMachineMemory, rp, 1, vect);
		     return 0;
		  }
	    }
	 else
	    {
	       ri = wa->rd->orders[wa->rd->curord]->index;
	    }
      }
   else if (index->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
      {
	 int no = index->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar - 1;
	 if (no >= 0 && no < wa->rd->idxs_opened)
	    ri = wa->rd->indices[no];
      }
   else
      {
	 int i;

	 for (i = 0; i < wa->rd->idxs_opened; i++)
	    {
	       if (strcasecmp(wa->rd->indices[i]->name, index->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf) == 0)
		  {
		     ri = wa->rd->indices[i];
		     break;
		  }
	    }
      }

   if (!ri)
      {
	 _clip_array(ClipMachineMemory, rp, 1, vect);
	 return 0;
      }

   vect[0] = ri->norders;
   _clip_array(ClipMachineMemory, rp, 1, vect);

   for (i = 0; i < ri->norders; i++)
      {
	 ClipVar v;
	 RDD_ORDER *ro = ri->orders[i];

	 CLEAR_CLIPVAR(&v);
	 v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	 v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = ro->name;
	 v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen(ro->name);
	 vect[0] = i;
	 _clip_aset(ClipMachineMemory, rp, &v, 1, vect);
      }
   return 0;
 err:
   return er;
}

int
clip_SX_TAGUNIQUE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_TAGUNIQUE";
   ClipVar *order = _clip_par(ClipMachineMemory, 1);
   ClipVar *index = _clip_par(ClipMachineMemory, 2);
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int ord, er;

   ClipMachineMemory->m6_error = 0;
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT2(2, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);

   _clip_retl(ClipMachineMemory, 0);
   if (!wa)
      return 0;

   ord = get_orderno(wa, order, index);
   if (ord == -1)
      return 0;

   _clip_retl(ClipMachineMemory, wa->rd->orders[ord]->unique);
   return 0;
 err:
   return er;
}

int
clip_SX_VSIGLEN(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_VSIGLEN";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   int t = _clip_parinfo(ClipMachineMemory, 1);
   int fno = -1, er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);

   _clip_retni(ClipMachineMemory, 0);

   if (!wa)
      return 0;

   if (t == CHARACTER_type_of_ClipVarType)
      {
	 char *fname = _clip_parc(ClipMachineMemory, 1);
	 if (fname)
	    fno = _rdd_fieldno(wa->rd, _clip_casehashword(fname, strlen(fname)));
      }
   else
      {
	 fno = _clip_parni(ClipMachineMemory, 1) - 1;
      }

   if (fno < 0 || fno >= wa->rd->nfields)
      return 0;

   if (wa->rd->fields[fno].type == 'V' && wa->rd->fields[fno].len != 3 && wa->rd->fields[fno].len != 4)
      {
	 _clip_retni(ClipMachineMemory, max(wa->rd->fields[fno].len - 6, 0));
      }
   else
      {
	 _clip_retni(ClipMachineMemory, wa->rd->fields[fno].len);
      }
   return 0;
 err:
   return er;
}

int
clip_SX_DESCEND(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_DESCEND";
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   ClipVar *order = _clip_par(ClipMachineMemory, 1);
   ClipVar *index = _clip_par(ClipMachineMemory, 2);
   int res, ord, er;

   ClipMachineMemory->m6_error = 0;
   CHECKWA(wa);
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT2(2, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);

   ord = get_orderno(wa, order, index);
   if (ord == -1)
      return rdd_err(ClipMachineMemory, EG_NOORDER, 0, __FILE__, __LINE__, __PROC__, er_noorder);

   if ((er = wa->rd->orders[ord]->vtbl->descend(ClipMachineMemory, wa->rd->orders[ord], !wa->rd->orders[ord]->descend, &res, __PROC__)))
      return er;
   return 0;
 err:
   return er;

}

int
clip_SX_TABLETYPE(ClipMachine * ClipMachineMemory)
{
   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipMachineMemory->m6_error = 0;
   _clip_retni(ClipMachineMemory, 0);
   if (!wa)
      return 0;
   _clip_retni(ClipMachineMemory, 1);
   return 0;
}

int
clip_SX_VERSION(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_VERSION";
   int type = _clip_parni(ClipMachineMemory, 1);
   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKOPT1(1, NUMERIC_type_of_ClipVarType);
   switch (type)
      {
      case 1:
	 _clip_retdc(ClipMachineMemory, 2002, 10, 14);
	 break;
      case 2:
	 _clip_retc(ClipMachineMemory, "00:00a");
	 break;
      case 3:
	 _clip_retc(ClipMachineMemory, "SIx 3.* by (c) SuccessWare, Inc. for CLIP, 1.0b, 10/14/2002, 00:00a");
	 break;
      default:
	 _clip_retc(ClipMachineMemory, "1.0b");
      }
   return 0;
 err:
   return er;
}

int
clip_M6_SET(ClipMachine * ClipMachineMemory)
{
   int opt = _clip_parni(ClipMachineMemory, 1);
   int f = _clip_parl(ClipMachineMemory, 2);
   int r = 0;

   ClipMachineMemory->m6_error = 0;
   switch (opt)
      {
      case _SET_TYPECHECK:
	 r = 0;
	 break;
      case _SET_OPTIMIZE:
      case 2:
	 r = ClipMachineMemory->flags1 & OPTIMIZE_FLAG;
	 if (_clip_parinfo(ClipMachineMemory, 2) == LOGICAL_type_of_ClipVarType)
	    {
	       if (f)
		  ClipMachineMemory->flags1 |= OPTIMIZE_FLAG;
	       else
		  ClipMachineMemory->flags1 &= ~OPTIMIZE_FLAG;
	    }
	 break;
      case _SET_RECHECK:
	 r = 0;
	 break;
      }
   _clip_retl(ClipMachineMemory, r);
   return 0;
}

int
clip_M6_ERROR(ClipMachine * ClipMachineMemory)
{
   _clip_retni(ClipMachineMemory, ClipMachineMemory->m6_error);
   return 0;
}
