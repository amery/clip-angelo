/*
	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI
*/
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "ci_rdd.h"
#include "ci_error.ch"
#ifdef HAVE_MMAN_H
#include <sys/mman.h>
#endif

extern DBWorkArea *cur_area(ClipMachine * ClipMachineMemory);

typedef struct _HS_HEADER_
{
   unsigned char sig[2];
   unsigned char fuu[4];
   char lcase;
   char filtset;
   unsigned char pagesize[2];
   unsigned char lastrec[4];
   char unused[498];
} HS_HEADER;

typedef struct _HIPER_
{
   char *path;
   char *name;
   RDD_FILE file;
   int pagesize;
   int intsonpage;
   int lcase;
   int filtset;
   unsigned int lastrec;
   int shared;
   char seen[0x0ffff];
   unsigned int *map;
   unsigned int *rmap;
   unsigned int recno;
} HIPER;

static int _hs_add(ClipMachine * ClipMachineMemory, HIPER * hs, const char *str, int len, unsigned int rec, const char *__PROC__);
static int hs_add(ClipMachine * ClipMachineMemory, HIPER * hs, const char *str, int len, unsigned int *rno, const char *__PROC__);
static int hs_close(ClipMachine * ClipMachineMemory, HIPER * hs, const char *__PROC__);
static int hs_create(ClipMachine * ClipMachineMemory, const char *fname, int pagesize, int lcase, int filtset, HIPER ** hsp, const char *__PROC__);
static int hs_filter(ClipMachine * ClipMachineMemory, HIPER * hs, const char *val, int len, unsigned int **map, unsigned int lastrec, unsigned int *count, const char *__PROC__);
static int hs_open(ClipMachine * ClipMachineMemory, const char *fname, int mode, HIPER ** hsp, const char *__PROC__);
static int hs_remove(ClipMachine * ClipMachineMemory, HIPER * hs, unsigned int rec, const char *__PROC__);

static void destroy_hiper(void *);

static const char *er_badhiper = "Bad HiPer handle";
static const char *er_baddata = "Expression returns non-character value";
static const char *er_ioerror = "I/O error";
static const char *er_badsig = "Bad HTX file (wrong signature)";

int
clip_HS_ADD(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "HS_ADD";
   int h = _clip_parni(ClipMachineMemory, 1);
   HIPER *hs;
   unsigned int rno;
   int er;

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKARG3(2, CHARACTER_type_of_ClipVarType, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);

   hs = _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_HIPER);
   if (!hs)
      {
	 er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badhiper);
	 goto err;
      }

   if (_clip_parinfo(ClipMachineMemory, 2) == CHARACTER_type_of_ClipVarType)
      {
	 int len;
	 const char *str = _clip_parcl(ClipMachineMemory, 2, &len);
	 if ((er = hs_add(ClipMachineMemory, hs, str, len, &rno, __PROC__)))
	    goto err;
      }
   else
      {
	 ClipVar *block = _clip_par(ClipMachineMemory, 2);
	 ClipVar v, *vp;

	 if ((er = rdd_calc(ClipMachineMemory, -1, block, &v, 0)))
	    goto err;
	 vp = _clip_vptr(&v);
	 if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType)
	    {
	       er = rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_baddata);
	       goto err;
	    }
	 if ((er = hs_add(ClipMachineMemory, hs, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, &rno,
__PROC__)))
	    goto err;
	 _clip_destroy(ClipMachineMemory, &v);
      }
   if (rno % 32 == 1)
      {
	 int ints = (rno + 31) >> 5;
	 hs->map = realloc(hs->map, ints << 2);
	 hs->map[ints - 1] = 0;
	 if (hs->rmap)
	    {
	       hs->rmap = realloc(hs->rmap, ints << 2);
	       hs->rmap[ints - 1] = 0;
	    }
      }
   _clip_retni(ClipMachineMemory, rno);
   return 0;
 err:
   return er;
}

int
clip_HS_CLOSE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "HS_CLOSE";
   int h = _clip_parni(ClipMachineMemory, 1);
   HIPER *hs;
   int er;

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   hs = _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_HIPER);
   if (!hs)
      {
	 er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badhiper);
	 goto err;
      }

   if ((er = hs_close(ClipMachineMemory, hs, __PROC__)))
      goto err;

   _clip_retni(ClipMachineMemory, 1);
   return 0;
 err:
   return er;
}

int
clip_HS_CREATE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "HS_CREATE";
   const char *fname = _clip_parc(ClipMachineMemory, 1);
   int pagesize = _clip_parni(ClipMachineMemory, 2) * 1024;
   int lcase = _clip_parl(ClipMachineMemory, 4);
   int filtset = _clip_parni(ClipMachineMemory, 5);
   HIPER *hs;
   int er;

   CHECKARG1(1, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(4, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(5, NUMERIC_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      pagesize = getpagesize();
   if (_clip_parinfo(ClipMachineMemory, 4) == UNDEF_type_of_ClipVarType)
      lcase = 0;
   if (_clip_parinfo(ClipMachineMemory, 5) == UNDEF_type_of_ClipVarType)
      filtset = 1;

   if ((er = hs_create(ClipMachineMemory, fname, pagesize, lcase, filtset, &hs, __PROC__)))
      goto err;
   hs->map = calloc(1, 4);
   _clip_retni(ClipMachineMemory, _clip_store_c_item(ClipMachineMemory, (void *) hs, _C_ITEM_TYPE_HIPER, destroy_hiper));
   return 0;
 err:
   return er;
}

int
clip_HS_DELETE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "HS_DELETE";
   int h = _clip_parni(ClipMachineMemory, 1);
   unsigned int rec = _clip_parni(ClipMachineMemory, 2);
   HIPER *hs;
   int er;

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKARG1(2, NUMERIC_type_of_ClipVarType);

   hs = _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_HIPER);
   if (!hs)
      {
	 er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badhiper);
	 goto err;
      }

   _rm_setbit(hs->map, hs->lastrec, rec);

   _clip_retni(ClipMachineMemory, 1);
   return 0;
 err:
   return er;
}

int
clip_HS_FILTER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "HS_FILTER";
   const char *fname = _clip_parc(ClipMachineMemory, 1);
   int len;
   const char *val = _clip_parcl(ClipMachineMemory, 2, &len);
   int elen;
   const char *expr = _clip_parcl(ClipMachineMemory, 3, &elen);
   int land = _clip_parl(ClipMachineMemory, 4);
   int mode = _clip_parni(ClipMachineMemory, 5);
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   HIPER *hs = NULL;
   RDD_FILTER *fp;
   unsigned int count;
   char *fexpr = NULL, *tmp, *s;
   char **vals = NULL;
   int *lens = NULL;
   int nvals = 0, i, j;
   unsigned int *ti;
   int er;

   _clip_retni(ClipMachineMemory, 0);
   CHECKWA(wa);

   CHECKARG1(1, CHARACTER_type_of_ClipVarType);
   CHECKARG1(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(3, CHARACTER_type_of_ClipVarType);
   CHECKOPT2(4, LOGICAL_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(5, NUMERIC_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 4) == NUMERIC_type_of_ClipVarType || _clip_parinfo(ClipMachineMemory, 4) == UNDEF_type_of_ClipVarType)
      land = 0;
   if ((er = wa->rd->vtbl->_rlock(ClipMachineMemory, wa->rd, __PROC__)))
      goto err;
   if ((er = hs_open(ClipMachineMemory, fname, mode, &hs, __PROC__)))
      goto err_unlock;
   if (hs->filtset == 1)
      {
	 tmp = strdup(val);
	 nvals = 0;
	 s = strtok(tmp, " ");
	 while (s)
	    {
	       lens = realloc(lens, sizeof(int) * (nvals + 1));
	       vals = realloc(vals, sizeof(char *) * (nvals + 1));
	       lens[nvals] = strlen(s);
	       vals[nvals] = strdup(s);
	       nvals++;
	       s = strtok(NULL, " ");
	    }
	 free(tmp);
      }
   else
      {
	 vals = malloc(sizeof(char *));
	 vals[0] = strdup(val);
	 lens = malloc(sizeof(int));
	 lens[0] = len;
	 nvals = 1;
      }
   if (!(expr && _clip_parinfo(ClipMachineMemory, 3) == CHARACTER_type_of_ClipVarType && elen > 0))
      {
	 fexpr = malloc(4);
	 strcpy(fexpr, ".t.");
      }
   else
      {
	 fexpr = calloc(1, 1);
	 for (i = 0; i < nvals; i++)
	    {
	       fexpr = realloc(fexpr, strlen(fexpr) + elen + strlen(vals[i]) + 27);
	       if (hs->lcase)
		  {
		     if (i == 0)
			{
			   sprintf(fexpr + strlen(fexpr), "UPPER('%s') $ UPPER(%s)", vals[i], expr);
			}
		     else
			{
			   if (land)
			      sprintf(fexpr + strlen(fexpr), " .AND. UPPER('%s') $ UPPER(%s)", vals[i], expr);
			   else
			      sprintf(fexpr + strlen(fexpr), " .OR. UPPER('%s') $ UPPER(%s)", vals[i], expr);
			}
		  }
	       else
		  {
		     if (i == 0)
			{
			   sprintf(fexpr + strlen(fexpr), "'%s' $ (%s)", vals[i], expr);
			}
		     else
			{
			   if (land)
			      sprintf(fexpr + strlen(fexpr), " .AND. '%s' $ (%s)", vals[i], expr);
			   else
			      sprintf(fexpr + strlen(fexpr), " .OR. '%s' $ (%s)", vals[i], expr);
			}
		  }
	    }
      }

   if ((er = rdd_createfilter(ClipMachineMemory, wa->rd, &fp, NULL, fexpr, NULL, 0, __PROC__)))
      goto err_unlock;
   free(fexpr);
   fexpr = NULL;
   fp->active = 1;
   wa->rd->filter = fp;

   if ((er = rdd_lastrec(ClipMachineMemory, wa->rd, &wa->rd->filter->size, __PROC__)))
      goto err_unlock;
   if ((er = hs_filter(ClipMachineMemory, hs, vals[0], lens[0], &wa->rd->filter->rmap, wa->rd->filter->size, &count, __PROC__)))
      goto err_unlock;
   for (i = 1; i < nvals; i++)
      {
	 if ((er = hs_filter(ClipMachineMemory, hs, vals[i], lens[i], &ti, wa->rd->filter->size, &count, __PROC__)))
	    goto err_unlock;
	 count = (wa->rd->filter->size + 31) / 32;
	 for (j = 0; j < count; j++)
	    {
	       if (land)
		  wa->rd->filter->rmap[j] &= ti[j];
	       else
		  wa->rd->filter->rmap[j] |= ti[j];
	    }
	 free(ti);
      }
   if ((er = hs_close(ClipMachineMemory, hs, __PROC__)))
      goto err_unlock;

   if (expr && _clip_parinfo(ClipMachineMemory, 3) == CHARACTER_type_of_ClipVarType && len > 2)
      {
	 if ((er = rm_evalpartial(ClipMachineMemory, fp, NULL, (unsigned int *) &len, &count, __PROC__)))
	    goto err_unlock;
      }
   if (wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__))
      goto err;

   for (i = 0; i < nvals; i++)
      {
	 free(vals[i]);
      }
   free(vals);
   free(lens);
   _clip_retni(ClipMachineMemory, count);
   return 0;

 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   if (vals)
      {
	 for (i = 0; i < nvals; i++)
	    {
	       free(vals[i]);
	    }
	 free(vals);
      }
   if (lens)
      free(lens);
   if (fexpr)
      free(fexpr);
   return er;
}

int
clip_HS_IFDEL(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "HS_IFDEL";
   int h = _clip_parni(ClipMachineMemory, 1);
   unsigned int rec = _clip_parni(ClipMachineMemory, 2);
   HIPER *hs;
   int er;

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKARG1(2, NUMERIC_type_of_ClipVarType);

   hs = _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_HIPER);
   if (!hs)
      {
	 er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badhiper);
	 goto err;
      }

   _clip_retni(ClipMachineMemory, _rm_getbit(hs->map, hs->lastrec, rec) != 0);
   return 0;
 err:
   return er;
}

int
clip_HS_INDEX(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "HS_INDEX";
   const char *fname = _clip_parc(ClipMachineMemory, 1);
   const char *expr = _clip_parc(ClipMachineMemory, 2);
/*	int mode = _clip_parni(ClipMachineMemory,4);*/
   int pagesize = _clip_parni(ClipMachineMemory, 5) * 1024;
   int lcase = _clip_parl(ClipMachineMemory, 6);
   int filtset = _clip_parni(ClipMachineMemory, 7);
   DBWorkArea *wa = cur_area(ClipMachineMemory);
   char *tmp = NULL;
   unsigned int oldrecno;
   unsigned int lastrec;
   int oldbof, oldeof;
   ClipVar v, *vp;
   ClipVar block;
   unsigned int rno;
   HIPER *hs;
   int er;

   memset(&v, 0, sizeof(ClipVar));
   memset(&block, 0, sizeof(ClipVar));
   _clip_retni(ClipMachineMemory, -1);
   CHECKWA(wa);
   CHECKARG1(1, CHARACTER_type_of_ClipVarType);
   CHECKARG1(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(4, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(5, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(6, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(7, NUMERIC_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 5) == UNDEF_type_of_ClipVarType)
      pagesize = getpagesize();
   if (_clip_parinfo(ClipMachineMemory, 6) == UNDEF_type_of_ClipVarType)
      lcase = 1;
   if (_clip_parinfo(ClipMachineMemory, 7) == UNDEF_type_of_ClipVarType)
      filtset = 1;

   if ((er = wa->rd->vtbl->_rlock(ClipMachineMemory, wa->rd, __PROC__)))
      goto err;
   if ((er = hs_create(ClipMachineMemory, fname, pagesize, lcase, filtset, &hs, __PROC__)))
      goto err_unlock;
   oldrecno = wa->rd->recno;
   oldbof = wa->rd->bof;
   oldeof = wa->rd->eof;
   oldbof = oldeof = 0;
   if ((er = rdd_lastrec(ClipMachineMemory, wa->rd, (int *) &lastrec, __PROC__)))
      goto err_unlock;
   tmp = malloc(strlen(expr) + 5);
   sprintf(tmp, "{||%s}", expr);
   if ((er = _clip_eval_macro(ClipMachineMemory, tmp, strlen(tmp), &block)))
      goto err_unlock;
   memset(&v, 0, sizeof(ClipVar));
   for (wa->rd->recno = 1; wa->rd->recno <= lastrec; wa->rd->recno++)
      {
	 if ((er = rdd_childs(ClipMachineMemory, wa->rd, __PROC__)))
	    goto err_unlock;
	 if ((er = rdd_calc(ClipMachineMemory, wa->rd->area, &block, &v, 0)))
	    goto err_unlock;
	 vp = _clip_vptr(&v);
	 if (!vp || vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType)
	    {
	       er = rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_baddata);
	       goto err_unlock;
	    }
	 if ((er = hs_add(ClipMachineMemory, hs, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, &rno,
__PROC__)))
	    goto err_unlock;
	 _clip_destroy(ClipMachineMemory, &v);
      }
   free(tmp);
   tmp = NULL;
   wa->rd->bof = oldbof;
   wa->rd->eof = oldeof;
   wa->rd->recno = oldrecno;
   _clip_destroy(ClipMachineMemory, &block);
   if ((er = wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__)))
      goto err;

   _clip_retni(ClipMachineMemory, _clip_store_c_item(ClipMachineMemory, (void *) hs, _C_ITEM_TYPE_HIPER, destroy_hiper));
   hs->map = calloc(1, hs->lastrec ? (hs->lastrec + 31) >> 3 : 4);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   if (tmp)
      free(tmp);
   if (v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
      _clip_destroy(ClipMachineMemory, &v);
   if (block.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
      _clip_destroy(ClipMachineMemory, &block);
   return er;
}

int
clip_HS_KEYCOUNT(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "HS_KEYCOUNT";
   int h = _clip_parni(ClipMachineMemory, 1);
   HIPER *hs;
   unsigned char c4[4];
   int er;

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   hs = _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_HIPER);
   if (!hs)
      {
	 er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badhiper);
	 goto err;
      }

   if ((er = rdd_read(ClipMachineMemory, &hs->file, 10, 4, c4, __PROC__)))
      goto err;
   _clip_retni(ClipMachineMemory, _rdd_uint(c4));
   return 0;
 err:
   return er;
}

int
clip_HS_NEXT(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "HS_REMOVE";
   int h = _clip_parni(ClipMachineMemory, 1);
   HIPER *hs;
   int er;

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   hs = _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_HIPER);
   if (!hs)
      {
	 er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badhiper);
	 goto err;
      }

   if (!hs->rmap)
      {
	 _clip_retni(ClipMachineMemory, 0);
	 return 0;
      }

   while ((++hs->recno <= hs->lastrec) && ((!_rm_getbit(hs->rmap, hs->lastrec, hs->recno)) || (_rm_getbit(hs->map, hs->lastrec, hs->recno))));

   _clip_retni(ClipMachineMemory, hs->recno > hs->lastrec ? 0 : hs->recno);
   return 0;
 err:
   return er;
}

int
clip_HS_REMOVE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "HS_REMOVE";
   int h = _clip_parni(ClipMachineMemory, 1);
   unsigned int rno = _clip_parni(ClipMachineMemory, 2);
   HIPER *hs;
   int er;

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKARG1(2, NUMERIC_type_of_ClipVarType);

   hs = _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_HIPER);
   if (!hs)
      {
	 er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badhiper);
	 goto err;
      }

   if ((er = hs_remove(ClipMachineMemory, hs, rno, __PROC__)))
      goto err;

   _clip_retni(ClipMachineMemory, 1);
   return 0;
 err:
   return er;
}

int
clip_HS_REPLACE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "HS_REPLACE";
   int h = _clip_parni(ClipMachineMemory, 1);
   ClipVar *val = _clip_spar(ClipMachineMemory, 2);
   unsigned int rno = _clip_parni(ClipMachineMemory, 3);
   int dirty = _clip_parni(ClipMachineMemory, 4);
   HIPER *hs;
   ClipVar v, *vp;
   int er;

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKARG3(2, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   CHECKARG1(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(4, LOGICAL_type_of_ClipVarType);

   hs = _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_HIPER);
   if (!hs)
      {
	 er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badhiper);
	 goto err;
      }

   if (!dirty)
      {
	 if ((er = hs_remove(ClipMachineMemory, hs, rno, __PROC__)))
	    goto err;
      }
   if (_clip_parinfo(ClipMachineMemory, 2) == CHARACTER_type_of_ClipVarType)
      {
	 if ((er = _hs_add(ClipMachineMemory, hs, val->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, val->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, rno,
__PROC__)))
	    goto err;
      }
   else
      {
	 if ((er = rdd_calc(ClipMachineMemory, -1, val, &v, 0)))
	    goto err;
	 vp = _clip_vptr(&v);
	 if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType)
	    {
	       er = rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_baddata);
	       goto err;
	    }
	 if ((er = _hs_add(ClipMachineMemory, hs, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, rno,
__PROC__)))
	    goto err;
	 _clip_destroy(ClipMachineMemory, &v);
      }

   _clip_retni(ClipMachineMemory, 1);
   return 0;
 err:
   return er;
}

int
clip_HS_SET(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "HS_SET";
   int h = _clip_parni(ClipMachineMemory, 1);
   int len;
   const char *val = _clip_parcl(ClipMachineMemory, 2, &len);
   int land = _clip_parl(ClipMachineMemory, 3);
   HIPER *hs;
   char *tmp, *s;
   int nvals = 0, i, j;
   int *lens = NULL;
   char **vals = NULL;
   unsigned int *ti;
   unsigned int count;
   int er;

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKARG1(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(3, LOGICAL_type_of_ClipVarType);

   hs = _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_HIPER);
   if (!hs)
      {
	 er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badhiper);
	 goto err;
      }

   if (hs->filtset == 1)
      {
	 tmp = strdup(val);
	 nvals = 0;
	 s = strtok(tmp, " ");
	 while (s)
	    {
	       lens = realloc(lens, sizeof(int) * (nvals + 1));
	       vals = realloc(vals, sizeof(char *) * (nvals + 1));
	       lens[nvals] = strlen(s);
	       vals[nvals] = strdup(s);
	       nvals++;
	       s = strtok(NULL, " ");
	    }
	 free(tmp);
      }
   else
      {
	 vals = malloc(sizeof(char *));
	 vals[0] = strdup(val);
	 lens = malloc(sizeof(int));
	 lens[0] = len;
	 nvals = 1;
      }

   if (hs->rmap)
      {
	 free(hs->rmap);
	 hs->rmap = NULL;
      }
   if ((er = hs_filter(ClipMachineMemory, hs, vals[0], lens[0], &hs->rmap, hs->lastrec, &count, __PROC__)))
      goto err;
   for (i = 1; i < nvals; i++)
      {
	 if ((er = hs_filter(ClipMachineMemory, hs, vals[i], lens[i], &ti, hs->lastrec, &count, __PROC__)))
	    goto err;
	 count = (hs->lastrec + 31) >> 5;
	 for (j = 0; j < count; j++)
	    {
	       if (land)
		  hs->rmap[j] &= ti[j];
	       else
		  hs->rmap[j] |= ti[j];
	    }
	 free(ti);
      }
   hs->recno = 0;
   return 0;
 err:
   if (vals)
      {
	 for (i = 0; i < nvals; i++)
	    {
	       free(vals[i]);
	    }
	 free(vals);
      }
   if (lens)
      free(lens);
   return er;
}

int
clip_HS_UNDELETE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "HS_UNDELETE";
   int h = _clip_parni(ClipMachineMemory, 1);
   unsigned int rec = _clip_parni(ClipMachineMemory, 2);
   HIPER *hs;
   int er;

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKARG1(2, NUMERIC_type_of_ClipVarType);

   hs = _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_HIPER);
   if (!hs)
      {
	 er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badhiper);
	 goto err;
      }

   _rm_clrbit(hs->map, hs->lastrec, rec);

   _clip_retni(ClipMachineMemory, 1);
   return 0;
 err:
   return er;
}

int
clip_HS_VERIFY(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "HS_UNDELETE";
   ClipVar *val;
   int subl, strl;
   const char *str = _clip_parcl(ClipMachineMemory, 2, &subl);
   int lcase = _clip_parl(ClipMachineMemory, 3);
   ClipVar v, *vp;
   char *sub = NULL, *string = NULL, *r;
   int er;

   CHECKARG3(1, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   CHECKARG1(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(4, LOGICAL_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 1) == CHARACTER_type_of_ClipVarType)
      {
	 val = _clip_par(ClipMachineMemory, 1);
	 strl = val->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
	 string = malloc(strl + 1);
	 memcpy(string, val->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, strl);
      }
   else
      {
	 val = _clip_spar(ClipMachineMemory, 1);
	 if ((er = rdd_calc(ClipMachineMemory, -1, val, &v, 0)))
	    goto err;
	 vp = _clip_vptr(&v);
	 if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType)
	    {
	       er = rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_baddata);
	       goto err;
	    }
	 strl = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
	 string = malloc(strl + 1);
	 memcpy(string, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, strl);
	 _clip_destroy(ClipMachineMemory, &v);
      }
   sub = malloc(subl + 1);
   memcpy(sub, str, subl);
   if (lcase)
      {
	 _clip_upstr(string, strl);
	 _clip_upstr(sub, subl);
      }
   r = (char *) _clip_strstr(string, strl, sub, subl);

   free(sub);
   free(string);

   _clip_retl(ClipMachineMemory, r != NULL);
   return 0;
 err:
   if (sub)
      free(sub);
   if (string)
      free(string);
   return er;
}

/* ---------------------------------------------------------------- */

static int
_hs_getfuu(ClipMachine * ClipMachineMemory, HIPER * hs, unsigned int *poffs, const char *__PROC__)
{
   unsigned char fuu[4];
   int er;

   if ((er = rdd_read(ClipMachineMemory, &hs->file, 2, 4, fuu, __PROC__)))
      return er;
   *poffs = _rdd_uint(fuu);
   if (*poffs == 0)
      {
	 int delta;
	 unsigned int fsize;

	 if (hs->file.md == (caddr_t) - 1)
	    {
	       struct stat st;
	       fstat(hs->file.fd, &st);
	       fsize = st.st_size;
	    }
	 else
	    fsize = hs->file.mapsize;

	 delta = (fsize / hs->pagesize) / 5;
	 if (delta < 1)
	    delta = 1;
	 *poffs = fsize;
	 if ((er = rdd_write(ClipMachineMemory, &hs->file, *poffs + delta * hs->pagesize, 0, "", __PROC__)))
	    return er;
	 if (delta > 1)
	    {
	       int i;

	       _rdd_put_uint(fuu, *poffs + hs->pagesize);
	       if ((er = rdd_write(ClipMachineMemory, &hs->file, 2, 4, fuu, __PROC__)))
		  return er;
	       for (i = 1; i < delta; i++)
		  {
		     if (i < delta - 1)
			_rdd_put_uint(fuu, *poffs + (i + 1) * hs->pagesize);
		     else
			_rdd_put_uint(fuu, 0);
		     if ((er = rdd_write(ClipMachineMemory, &hs->file, *poffs + i * hs->pagesize, 4, fuu, __PROC__)))
			return er;
		  }
	    }
      }
   else
      {
	 if ((er = rdd_read(ClipMachineMemory, &hs->file, *poffs, 4, fuu, __PROC__)))
	    return er;
	 if ((er = rdd_write(ClipMachineMemory, &hs->file, 2, 4, fuu, __PROC__)))
	    return er;
      }
   return 0;
}

static int
_hs_loadbits(ClipMachine * ClipMachineMemory, HIPER * hs, unsigned int poffs, unsigned int lastrec, unsigned int **rmap, const char *__PROC__)
{
   unsigned char c4[4];
   int bitstyle;
   int rpos, pos;
   unsigned int *rpage = malloc(hs->pagesize);
   unsigned int *page = malloc(hs->pagesize);
   unsigned int next, offs;
   int ints = (lastrec + 31) / 32;
   int c = 0, er;

   *rmap = calloc(1, ints * 4);
   if ((er = rdd_read(ClipMachineMemory, &hs->file, poffs, 4, c4, __PROC__)))
      goto err;
   bitstyle = _rdd_uint(c4);
   if (bitstyle)
      {
	 int tail;

	 rpos = 3;
	 next = poffs;
	 do
	    {
	       if ((er = rdd_read(ClipMachineMemory, &hs->file, next, hs->pagesize, rpage, __PROC__)))
		  goto err;
	       for (; rpos < hs->intsonpage; rpos++)
		  {
		     offs = _rdd_uint((unsigned char *) (rpage + rpos));
		     if (!offs)
			break;
		     tail = min(hs->pagesize, (ints - c) << 2);
		     if ((er = rdd_read(ClipMachineMemory, &hs->file, offs, tail, *rmap + c, __PROC__)))
			goto err;
		     c += hs->intsonpage;
		  }
	       rpos = 1;
	       next = _rdd_uint((unsigned char *) (rpage + ((next == poffs) ? 2 : 0)));
	    }
	 while (next);
      }
   else
      {
	 rpos = 3;
	 next = poffs;
	 do
	    {
	       if ((er = rdd_read(ClipMachineMemory, &hs->file, next, hs->pagesize, rpage, __PROC__)))
		  goto err;
	       for (; rpos < hs->intsonpage; rpos++)
		  {
		     offs = _rdd_uint((unsigned char *) (rpage + rpos));
		     if (!offs)
			break;
		     if ((er = rdd_read(ClipMachineMemory, &hs->file, offs, hs->pagesize, page, __PROC__)))
			goto err;
		     for (pos = 0; pos < hs->intsonpage; pos++)
			{
			   if (!page[pos])
			      break;
			   _rm_setbit(*rmap, lastrec, page[pos]);
			}
		  }
	       rpos = 1;
	       next = _rdd_uint((unsigned char *) (rpage + ((next == poffs) ? 2 : 0)));
	    }
	 while (next);
      }
   free(rpage);
   free(page);
   return 0;
 err:
   free(*rmap);
   *rmap = NULL;
   free(rpage);
   free(page);
   return er;
}

static int
_hs_tobitstyle(ClipMachine * ClipMachineMemory, HIPER * hs, unsigned int poffs, unsigned int rec, unsigned int npairs, int *tobit, const char *__PROC__)
{
   int bints = (max(hs->lastrec, rec) + 31) / 32;
   unsigned int *rmap = NULL;
   int er;

   *tobit = 0;
   if (bints < npairs + 1)
      {
	 unsigned char c4[4];
	 int c = 0;
	 int next = 8;
	 int pos = 12;
	 int tail;

	 if ((er = _hs_loadbits(ClipMachineMemory, hs, poffs, max(hs->lastrec, rec), &rmap, __PROC__)))
	    goto err;

	 *tobit = 1;
	 _rdd_put_uint(c4, 1);
	 if ((er = rdd_write(ClipMachineMemory, &hs->file, poffs, 4, c4, __PROC__)))
	    goto err;

	 while (c < bints)
	    {
	       if (pos == hs->intsonpage)
		  {
		     if ((er = rdd_read(ClipMachineMemory, &hs->file, poffs + next, 4, c4, __PROC__)))
			goto err;
		     poffs = _rdd_uint(c4);
		     next = 0;
		     pos = 4;
		  }
	       if ((er = rdd_read(ClipMachineMemory, &hs->file, poffs + pos, 4, c4, __PROC__)))
		  goto err;
	       tail = min(hs->pagesize, (bints - c) * 4);
	       if ((er = rdd_write(ClipMachineMemory, &hs->file, _rdd_uint(c4), tail, rmap + c, __PROC__)))
		  goto err;
	       if (tail < hs->pagesize)
		  {
		     void *buf = calloc(1, hs->pagesize - tail);
		     if ((er = rdd_write(ClipMachineMemory, &hs->file, _rdd_uint(c4) + tail, hs->pagesize - tail, buf, __PROC__)))
			{
			   free(buf);
			   goto err;
			}
		     free(buf);
		  }
	       c += hs->intsonpage;
	       pos += 4;
	    }
	 free(rmap);
      }
   return 0;
 err:
   if (rmap)
      free(rmap);
   return er;
}

static int
_hs_compare(ClipMachine * ClipMachineMemory, HIPER * hs, unsigned int poffs, int pos, unsigned int rec, int *c, unsigned int *offs, unsigned int *roffs, int *rpos, int *nextp, const char *__PROC__)
{
   int curr = hs->intsonpage * (hs->intsonpage - 3);
   int prev = 0;
   unsigned char c4[4];
   int lpos, er;

   *nextp = 8;
   while (pos >= curr)
      {
	 if ((er = rdd_read(ClipMachineMemory, &hs->file, poffs + (prev ? 0 : 8), 4, c4, __PROC__)))
	    goto err;
	 poffs = _rdd_uint(c4);
	 prev = curr;
	 curr += hs->intsonpage * (hs->intsonpage - 1);
	 *nextp = 0;
      }
   *roffs = poffs;
   lpos = (pos - prev) / hs->intsonpage;
   *rpos = lpos + (prev ? 1 : 3);
   if ((er = rdd_read(ClipMachineMemory, &hs->file, poffs + (prev ? 0 : 8) + 4 + lpos * 4, 4, c4, __PROC__)))
      goto err;
   poffs = _rdd_uint(c4);
   lpos = pos % hs->intsonpage;
   if ((er = rdd_read(ClipMachineMemory, &hs->file, poffs + lpos * 4, 4, c4, __PROC__)))
      goto err;
   lpos = _rdd_uint(c4);
   if (lpos < rec)
      *c = -1;
   else if (lpos > rec)
      *c = 1;
   else
      *c = 0;
   *offs = poffs;
   return 0;
 err:
   return er;
}

static int
_hs_insert(ClipMachine * ClipMachineMemory, HIPER * hs, unsigned int offs, int pos, unsigned int rec, unsigned int roffs, int rpos, int nextp, const char *__PROC__)
{
   unsigned char c4[4];
   unsigned int *page = malloc(hs->pagesize + 4);
   int i, er;

   if ((er = rdd_read(ClipMachineMemory, &hs->file, offs, hs->pagesize, page, __PROC__)))
      goto err;
   memmove(page + pos + 1, page + pos, hs->pagesize - pos * 4);
   _rdd_put_uint((unsigned char *) (page + pos), rec);
   if ((er = rdd_write(ClipMachineMemory, &hs->file, offs, hs->pagesize, page, __PROC__)))
      goto err;
   do
      {
	 for (i = rpos + 1; i < hs->intsonpage; i++)
	    {
	       if ((er = rdd_read(ClipMachineMemory, &hs->file, roffs + i * 4, 4, c4, __PROC__)))
		  goto err;
	       offs = _rdd_uint(c4);
	       if (!offs)
		  break;

	       if ((er = rdd_read(ClipMachineMemory, &hs->file, offs, hs->pagesize, page, __PROC__)))
		  goto err;
	       rec = _rdd_uint((unsigned char *) (page + hs->intsonpage));
	       memmove(page + 1, page, hs->pagesize);
	       _rdd_put_uint((unsigned char *) page, rec);
	       if ((er = rdd_write(ClipMachineMemory, &hs->file, offs, hs->pagesize, page, __PROC__)))
		  goto err;
	    }
	 if ((er = rdd_read(ClipMachineMemory, &hs->file, roffs + nextp, 4, c4, __PROC__)))
	    goto err;
	 roffs = _rdd_uint(c4);
      }
   while (roffs);

   free(page);
   return 0;
 err:
   free(page);
   return er;
}

static int
_hs_setpair(ClipMachine * ClipMachineMemory, HIPER * hs, unsigned int poffs, unsigned int rec, const char *__PROC__)
{
   unsigned char c4[4];
   void *buf = NULL;
   int binary, er;

   if ((er = rdd_read(ClipMachineMemory, &hs->file, poffs, 4, c4, __PROC__)))
      goto err;
   binary = _rdd_uint(c4);
   if (binary)
      {
	 int rs = (hs->intsonpage * (hs->intsonpage - 3)) << 5;
	 int prs = 0;
	 int f = 3;
	 unsigned int next, offs;
	 int pos, i;

	 buf = calloc(1, hs->pagesize);
	 while (rec > rs)
	    {
	       if ((er = rdd_read(ClipMachineMemory, &hs->file, poffs + (f << 2), 4, c4, __PROC__)))
		  goto err;
	       next = _rdd_uint(c4);
	       if (!next)
		  {
		     if ((er = _hs_getfuu(ClipMachineMemory, hs, &next, __PROC__)))
			goto err;
		     if ((er = rdd_write(ClipMachineMemory, &hs->file, next, hs->pagesize, buf, __PROC__)))
			goto err;
		     _rdd_put_uint(c4, next);
		     if ((er = rdd_write(ClipMachineMemory, &hs->file, poffs + (f << 2), 4, c4, __PROC__)))
			goto err;

		     pos = hs->intsonpage - 1;
		     if ((er = rdd_read(ClipMachineMemory, &hs->file, poffs + (pos << 2), 4, c4, __PROC__)))
			goto err;
		     while (pos > f && _rdd_uint(c4) == 0)
			{
			   if ((er = _hs_getfuu(ClipMachineMemory, hs, &offs, __PROC__)))
			      goto err;
			   if ((er = rdd_write(ClipMachineMemory, &hs->file, offs, hs->pagesize, buf, __PROC__)))
			      goto err;
			   _rdd_put_uint(c4, offs);
			   if ((er = rdd_write(ClipMachineMemory, &hs->file, poffs + (pos << 2), 4, c4, __PROC__)))
			      goto err;
			}
		  }
	       poffs = next;
	       f = 0;
	       prs = rs;
	       rs += (hs->intsonpage * (hs->intsonpage - 1)) << 5;
	    }
	 i = pos = (rec - 1 - prs) / (hs->intsonpage << 5) + f;
	 if ((er = rdd_read(ClipMachineMemory, &hs->file, poffs + (i << 2), 4, c4, __PROC__)))
	    goto err;
	 while (i > f && _rdd_uint(c4) == 0)
	    {
	       if ((er = _hs_getfuu(ClipMachineMemory, hs, &offs, __PROC__)))
		  goto err;
	       if ((er = rdd_write(ClipMachineMemory, &hs->file, offs, hs->pagesize, buf, __PROC__)))
		  goto err;
	       _rdd_put_uint(c4, offs);
	       if ((er = rdd_write(ClipMachineMemory, &hs->file, poffs + (i << 2), 4, c4, __PROC__)))
		  goto err;
	    }
	 if (buf)
	    {
	       free(buf);
	       buf = NULL;
	    }
	 if ((er = rdd_read(ClipMachineMemory, &hs->file, poffs + (pos << 2), 4, c4, __PROC__)))
	    goto err;
	 offs = _rdd_uint(c4);
	 pos = (rec - 1 - prs - (pos - f) * (hs->intsonpage << 5)) >> 5;
	 if ((er = rdd_read(ClipMachineMemory, &hs->file, offs + (pos << 2), 4, c4, __PROC__)))
	    goto err;
	 _rdd_put_uint(c4, _rdd_uint(c4) | (1 << ((rec - 1) % 32)));
	 if ((er = rdd_write(ClipMachineMemory, &hs->file, offs + (pos << 2), 4, c4, __PROC__)))
	    goto err;
      }
   else
      {
	 unsigned int npairs, noffs = 0, offs, roffs, npairs1, npairsN;
	 int tobit, l, h, i = 0, c, rpos, nextp;

	 if ((er = rdd_read(ClipMachineMemory, &hs->file, poffs + 4, 4, c4, __PROC__)))
	    goto err;
	 npairs = _rdd_uint(c4);
	 if ((npairs % hs->intsonpage) == 0)
	    {
	       if ((er = _hs_tobitstyle(ClipMachineMemory, hs, poffs, rec, npairs, &tobit, __PROC__)))
		  goto err;
	       if (tobit)
		  return _hs_setpair(ClipMachineMemory, hs, poffs, rec, __PROC__);
	       buf = calloc(1, hs->pagesize);
	       npairs1 = hs->intsonpage * (hs->intsonpage - 3);
	       npairsN = hs->intsonpage * (hs->intsonpage - 1);
	       if ((npairs == npairs1) || ((npairs > npairs1) && ((npairs - npairs1) % npairsN == 0)))
		  {
		     if (_hs_getfuu(ClipMachineMemory, hs, &noffs, __PROC__))
			goto err;
		     if ((er = rdd_write(ClipMachineMemory, &hs->file, noffs, hs->pagesize, buf, __PROC__)))
			goto err;
		     offs = poffs + 8;
		     if ((er = rdd_read(ClipMachineMemory, &hs->file, offs, 4, c4, __PROC__)))
			goto err;
		     while (_rdd_uint(c4))
			{
			   offs = _rdd_uint(c4);
			   if ((er = rdd_read(ClipMachineMemory, &hs->file, offs, 4, c4, __PROC__)))
			      goto err;
			}
		     _rdd_put_uint(c4, noffs);
		     if ((er = rdd_write(ClipMachineMemory, &hs->file, offs, 4, c4, __PROC__)))
			goto err;
		  }
	       if ((er = _hs_getfuu(ClipMachineMemory, hs, &roffs, __PROC__)))
		  goto err;
	       offs = poffs + 8;
	       if ((er = rdd_read(ClipMachineMemory, &hs->file, offs, 4, c4, __PROC__)))
		  goto err;
	       while (_rdd_uint(c4))
		  {
		     offs = _rdd_uint(c4);
		     if ((er = rdd_read(ClipMachineMemory, &hs->file, offs, 4, c4, __PROC__)))
			goto err;
		  }
	       offs += 4;
	       if ((er = rdd_read(ClipMachineMemory, &hs->file, offs, 4, c4, __PROC__)))
		  goto err;
	       while (_rdd_uint(c4))
		  {
		     offs += 4;
		     if ((er = rdd_read(ClipMachineMemory, &hs->file, offs, 4, c4, __PROC__)))
			goto err;
		  }
	       _rdd_put_uint(c4, roffs);
	       if ((er = rdd_write(ClipMachineMemory, &hs->file, offs, 4, c4, __PROC__)))
		  goto err;
	       if ((er = rdd_write(ClipMachineMemory, &hs->file, roffs, hs->pagesize, buf, __PROC__)))
		  goto err;
	       free(buf);
	       buf = NULL;
	    }
	 l = 0;
	 h = npairs - 1;
	 while (l <= h)
	    {
	       i = (l + h) / 2;
	       if ((er = _hs_compare(ClipMachineMemory, hs, poffs, i, rec, &c, &offs, &roffs, &rpos, &nextp, __PROC__)))
		  goto err;
	       if (c < 0)
		  l = i + 1;
	       else if (c > 0)
		  h = i - 1;
	       else
		  return 0;
	    }
	 if ((er = _hs_insert(ClipMachineMemory, hs, offs, i % hs->intsonpage + 1, rec, noffs ? noffs : roffs, noffs ? 0 : rpos, nextp, __PROC__)))
	    goto err;
	 _rdd_put_uint(c4, npairs + 1);
	 if ((er = rdd_write(ClipMachineMemory, &hs->file, poffs + 4, 4, c4, __PROC__)))
	    goto err;
      }
   return 0;
 err:
   if (buf)
      free(buf);
   return er;
}

static int
_hs_addpair(ClipMachine * ClipMachineMemory, HIPER * hs, unsigned int offs, unsigned int rec, const char *__PROC__)
{
   unsigned int roffs, poffs;
   char *page = calloc(1, hs->pagesize);
   unsigned char c4[4];
   int er;

   if ((er = _hs_getfuu(ClipMachineMemory, hs, &roffs, __PROC__)))
      goto err;
   if ((er = _hs_getfuu(ClipMachineMemory, hs, &poffs, __PROC__)))
      goto err;

   if ((er = rdd_write(ClipMachineMemory, &hs->file, poffs, hs->pagesize, page, __PROC__)))
      goto err;

   _rdd_put_uint((unsigned char *) (page + 4), 1);
   _rdd_put_uint((unsigned char *) (page + 12), poffs);
   if ((er = rdd_write(ClipMachineMemory, &hs->file, roffs, hs->pagesize, page, __PROC__)))
      goto err;

   _rdd_put_uint(c4, roffs);
   if ((er = rdd_write(ClipMachineMemory, &hs->file, offs, 4, c4, __PROC__)))
      goto err;

   _rdd_put_uint(c4, rec);
   if ((er = rdd_write(ClipMachineMemory, &hs->file, poffs, 4, c4, __PROC__)))
      goto err;

   free(page);
   return 0;
 err:
   free(page);
   return er;
}

static int
_hs_delete(ClipMachine * ClipMachineMemory, HIPER * hs, unsigned int offs, int pos, unsigned int roffs, int rpos, int nextp, const char *__PROC__)
{
   unsigned int *buf = calloc(1, hs->pagesize);
   unsigned char c4[4];
   unsigned int next;
   unsigned int lp;
   int er;

   if ((er = rdd_read(ClipMachineMemory, &hs->file, offs, hs->pagesize, buf, __PROC__)))
      goto err;
   memmove(buf + pos, buf + pos + 1, hs->pagesize - 4 - (pos << 2));
   if ((er = rdd_write(ClipMachineMemory, &hs->file, offs, hs->pagesize, buf, __PROC__)))
      goto err;
   lp = offs + hs->pagesize - 4;

   if ((er = rdd_read(ClipMachineMemory, &hs->file, roffs + nextp, 4, c4, __PROC__)))
      goto err;
   next = _rdd_uint(c4);
   while (next)
      {
	 for (; rpos < hs->intsonpage; rpos++)
	    {
	       if ((er = rdd_read(ClipMachineMemory, &hs->file, roffs + (rpos << 2), 4, c4, __PROC__)))
		  goto err;
	       offs = _rdd_uint(c4);
	       if (!offs)
		  break;
	       if ((er = rdd_read(ClipMachineMemory, &hs->file, offs, hs->pagesize, buf, __PROC__)))
		  goto err;
	       if ((er = rdd_write(ClipMachineMemory, &hs->file, lp, 4, buf, __PROC__)))
		  goto err;
	       memmove(buf, buf + 1, hs->pagesize - 4);
	       if ((er = rdd_write(ClipMachineMemory, &hs->file, offs, hs->pagesize, buf, __PROC__)))
		  goto err;
	       lp = offs + hs->pagesize - 4;
	    }
	 roffs = next;

	 rpos = 1;
	 if ((er = rdd_read(ClipMachineMemory, &hs->file, roffs, 4, c4, __PROC__)))
	    goto err;
	 next = _rdd_uint(c4);
      }

   free(buf);
   return 0;
 err:
   free(buf);
   return er;
}

/* ---------------------------------------------------------------- */

static void
destroy_hiper(void *d)
{
   HIPER *hs = (HIPER *) d;

   if (hs)
      {
	 if (hs->path)
	    free(hs->path);
	 if (hs->name)
	    free(hs->name);
	 free(hs->map);
	 if (hs->rmap)
	    free(hs->rmap);
	 free(hs);
      }
}

static int
_hs_add(ClipMachine * ClipMachineMemory, HIPER * hs, const char *str, int len, unsigned int rec, const char *__PROC__)
{
   unsigned char c4[4];
   const char *s = str;
   const char *e = str + len - 1;
   unsigned char pair[2];
   unsigned int offs;
   int i, er;

   if (!str)
      return 0;
   memset(hs->seen, 0, sizeof(hs->seen));

   while (s < e)
      {
	 memcpy(pair, s, 2);
	 if (hs->filtset == 1)
	    {			/* text */
	       if (!isgraph(pair[0]) || !isgraph(pair[1]))
		  {
		     s++;
		     continue;
		  }
	       if (hs->lcase)
		  {		/* non case-sensitive text */
		     pair[0] = toupper(pair[0]);
		     pair[1] = toupper(pair[1]);
		  }
	    }			/* otherwise binary */
	 i = _rdd_ushort(pair);
	 offs = sizeof(HS_HEADER) + i * 4;
	 if ((er = rdd_read(ClipMachineMemory, &hs->file, offs, 4, c4, __PROC__)))
	    return er;
	 if (!_rdd_uint(c4))
	    {
	       if ((er = _hs_addpair(ClipMachineMemory, hs, offs, rec, __PROC__)))
		  return er;
	    }
	 else
	    {
	       if (!hs->seen[i])
		  {
		     if ((er = _hs_setpair(ClipMachineMemory, hs, _rdd_uint(c4), rec, __PROC__)))
			return er;
		  }
	    }
	 hs->seen[i] = 1;
	 s++;
      }
   return 0;
}

static int
hs_add(ClipMachine * ClipMachineMemory, HIPER * hs, const char *str, int len, unsigned int *rno, const char *__PROC__)
{
   unsigned int rec;
   unsigned char c4[4];
   int er;

   if ((er = rdd_read(ClipMachineMemory, &hs->file, 10, 4, c4, __PROC__)))
      return er;
   rec = _rdd_uint(c4) + 1;
   if ((er = _hs_add(ClipMachineMemory, hs, str, len, rec, __PROC__)))
      return er;
   _rdd_put_uint(c4, rec);
   if ((er = rdd_write(ClipMachineMemory, &hs->file, 10, 4, c4, __PROC__)))
      return er;
   hs->lastrec = rec;
   *rno = rec;
   return 0;
}

static int
hs_close(ClipMachine * ClipMachineMemory, HIPER * hs, const char *__PROC__)
{
   if (hs->file.md != (caddr_t) - 1)
#ifdef OS_MINGW
      free(hs->file.md);
#else
      if (munmap(hs->file.md, hs->file.mapsize) == -1)
	 return rdd_err(ClipMachineMemory, EG_CLOSE, errno, __FILE__, __LINE__, __PROC__, er_ioerror);
#endif

   if (close(hs->file.fd))
      return rdd_err(ClipMachineMemory, EG_CLOSE, errno, __FILE__, __LINE__, __PROC__, er_ioerror);
   destroy_hiper(hs);

   return 0;
}

static int
hs_create(ClipMachine * ClipMachineMemory, const char *fname, int pagesize, int lcase, int filtset, HIPER ** hsp, const char *__PROC__)
{
   HIPER *hs = calloc(1, sizeof(HIPER));
   HS_HEADER hdr;
   unsigned char pairs[0x10000 * 4];
   int er;

   hs->file.fd = -1;
   if (filtset == 1)
      hs->filtset = 1;		/* text */
   else
      hs->filtset = 2;		/* binary */
   hs->lcase = ((filtset == 1) ? lcase : 0);
   hs->pagesize = pagesize;
   hs->shared = 0;
   hs->intsonpage = hs->pagesize / 4;

   memset(&hdr, 0, sizeof(HS_HEADER));
   memcpy(hdr.sig, "HS", 2);
   hdr.lcase = hs->lcase;
   hdr.filtset = hs->filtset;
   _rdd_put_ushort(hdr.pagesize, hs->pagesize);

   if ((er = _rdd_parsepath(ClipMachineMemory, fname, ".htx", &hs->path, &hs->name, EG_CREATE, __PROC__)))
      goto err;

#ifdef _WIN32
   hs->file.fd = open(hs->path, O_CREAT | O_TRUNC | O_RDWR | O_BINARY, ClipMachineMemory->fileCreateMode);
#else
   hs->file.fd = open(hs->path, O_CREAT | O_TRUNC | O_RDWR, ClipMachineMemory->fileCreateMode);
#endif
   if (hs->file.fd == -1)
      goto err_create;
   hs->file.mapsize = sizeof(HS_HEADER);
   if (lseek(hs->file.fd, hs->file.mapsize - 1, SEEK_SET) == -1)
      goto err_create;
   if (write(hs->file.fd, "", 1) == -1)
      goto err_create;
#ifdef HAVE_MMAN_H
   hs->file.md = (caddr_t) mmap(0, hs->file.mapsize, PROT_READ | PROT_WRITE, MAP_SHARED, hs->file.fd, 0);
  /* If failed use non-mapped file (md==-1) */
#endif
   if ((er = rdd_write(ClipMachineMemory, &hs->file, 0, sizeof(HS_HEADER), &hdr, __PROC__)))
      goto err;

   memset(pairs, 0, sizeof(pairs));
   if ((er = rdd_write(ClipMachineMemory, &hs->file, sizeof(HS_HEADER), sizeof(pairs), pairs, __PROC__)))
      goto err;

   *hsp = hs;
   return 0;
 err_create:
   er = rdd_err(ClipMachineMemory, EG_CREATE, errno, __FILE__, __LINE__, __PROC__, hs->path);
 err:
   if (hs->file.md)
#ifdef OS_MINGW
      free(hs->file.md);
#else
      munmap(hs->file.md, hs->file.mapsize);
#endif
   if (hs->file.fd != -1)
      close(hs->file.fd);
   if (hs->name)
      free(hs->name);
   if (hs->path)
      free(hs->path);
   free(hs);
   return er;
}

static int
hs_filter(ClipMachine * ClipMachineMemory, HIPER * hs, const char *val, int len, unsigned int **map, unsigned int lastrec, unsigned int *count, const char *__PROC__)
{
   int ints = (lastrec + 31) / 32;
   unsigned int *r = malloc(ints * 4);
   unsigned int *t = NULL;
   char *s = (char *) val;
   char *e = s + len - 1;
   unsigned char pair[2];
   unsigned int i;
   unsigned int offs;
   unsigned char c4[4];
   unsigned int bit;
   int er;

   memset(r, 0xff, ints * 4);
   while (s < e)
      {
	 memcpy(pair, s, 2);
	 if (hs->filtset == 1)
	    {			/* text */
	       if (!isgraph(pair[0]) || !isgraph(pair[1]))
		  {
		     s++;
		     continue;
		  }
	       if (hs->lcase)
		  {		/* non case-sensitive text */
		     pair[0] = toupper(pair[0]);
		     pair[1] = toupper(pair[1]);
		  }
	    }			/* otherwise binary */
	 offs = sizeof(HS_HEADER) + _rdd_ushort(pair) * 4;
	 if ((er = rdd_read(ClipMachineMemory, &hs->file, offs, 4, c4, __PROC__)))
	    goto err;
	 if ((er = _hs_loadbits(ClipMachineMemory, hs, _rdd_uint(c4), lastrec, &t, __PROC__)))
	    goto err;
	 for (i = 0; i < ints; i++)
	    r[i] &= t[i];
	 free(t);
	 t = NULL;
	 s++;
      }

   *count = 0;
   for (i = 0; i < ints; i++)
      {
	 while (i < ints - 1 && !r[i])
	    i++;
	 bit = 1;
	 *count += ((r[i] & bit) != 0);
	 do
	    {
	       bit = bit << 1;
	       *count += ((r[i] & bit) != 0);
	    }
	 while (!(bit & 0x80000000));
      }
   *map = r;
   return 0;
 err:
   free(r);
   if (t)
      free(t);
   return er;
}

static int
hs_open(ClipMachine * ClipMachineMemory, const char *fname, int mode, HIPER ** hsp, const char *__PROC__)
{
   HIPER *hs = calloc(1, sizeof(HIPER));
   HS_HEADER hdr;
   int shared = 0;
   int rdwr = 0;
   struct stat st;
   int er;

   if (mode < 0 || mode > 3)
      mode = 2;
   if (mode == 0 || mode == 2)
      shared = 1;
   if (mode == 0 || mode == 1)
      rdwr = 1;

   if ((er = _rdd_parsepath(ClipMachineMemory, fname, ".htx", &hs->path, &hs->name, EG_OPEN, __PROC__)))
      goto err;

#ifdef _WIN32
   hs->file.fd = open(hs->path, (rdwr ? O_RDWR : O_RDONLY) | O_BINARY);
#else
   hs->file.fd = open(hs->path, rdwr ? O_RDWR : O_RDONLY);
#endif
   if (hs->file.fd == -1)
      goto err_open;
   if (fstat(hs->file.fd, &st) == -1)
      goto err_open;
#ifdef HAVE_MMAN_H
   hs->file.mapsize = st.st_size;
   hs->file.md = (caddr_t) mmap(0, hs->file.mapsize, PROT_READ | (rdwr ? PROT_WRITE : 0), MAP_SHARED, hs->file.fd, 0);
  /* If failed use non-mapped file (md==-1) */
#endif

   if ((er = rdd_read(ClipMachineMemory, &hs->file, 0, sizeof(HS_HEADER), &hdr, __PROC__)))
      goto err;
   if (memcmp(hdr.sig, "HS", 2) != 0)
      {
	 er = rdd_err(ClipMachineMemory, EG_OPEN, 0, __FILE__, __LINE__, __PROC__, er_badsig);
	 goto err;
      }
   hs->lcase = hdr.lcase;
   hs->filtset = hdr.filtset;
   hs->pagesize = _rdd_ushort(hdr.pagesize);
   hs->shared = shared;
   hs->intsonpage = hs->pagesize / 4;

   *hsp = hs;
   return 0;
 err_open:
   er = rdd_err(ClipMachineMemory, EG_OPEN, errno, __FILE__, __LINE__, __PROC__, hs->path);
 err:
#ifdef HAVE_MMAN_H
   if (hs->file.md)
      munmap(hs->file.md, hs->file.mapsize);
#endif
   if (hs->file.fd != -1)
      close(hs->file.fd);
   if (hs->name)
      free(hs->name);
   if (hs->path)
      free(hs->path);
   free(hs);
   return er;
}

static int
hs_remove(ClipMachine * ClipMachineMemory, HIPER * hs, unsigned int rec, const char *__PROC__)
{
   unsigned char c4[4];
   unsigned int poffs, offs;
   int i, er;

   for (i = sizeof(HS_HEADER); i < 0x40000 + sizeof(HS_HEADER); i += 4)
      {
	 if ((er = rdd_read(ClipMachineMemory, &hs->file, i, 4, c4, __PROC__)))
	    goto err;
	 poffs = _rdd_uint(c4);
	 if (!poffs)
	    continue;
	 if ((er = rdd_read(ClipMachineMemory, &hs->file, poffs, 4, c4, __PROC__)))
	    goto err;
	 if (_rdd_uint(c4))
	    {			/* binary */
	       int rs = (hs->intsonpage * (hs->intsonpage - 3)) << 5;
	       int prs = 0;
	       int f = 3;
	       unsigned int next, offs;
	       int pos;

	       while (rec > rs)
		  {
		     if ((er = rdd_read(ClipMachineMemory, &hs->file, poffs + (f << 2), 4, c4, __PROC__)))
			goto err;
		     next = _rdd_uint(c4);
		     if (!next)
			{
			   return 0;
			}
		     poffs = next;
		     f = 0;
		     prs = rs;
		     rs += (hs->intsonpage * (hs->intsonpage - 1)) << 5;
		  }
	       pos = (rec - 1 - prs) / (hs->intsonpage << 5) + f;
	       if ((er = rdd_read(ClipMachineMemory, &hs->file, poffs + (pos << 2), 4, c4, __PROC__)))
		  goto err;
	       offs = _rdd_uint(c4);
	       pos = (rec - 1 - prs - (pos - f) * (hs->intsonpage << 5)) >> 5;
	       if ((er = rdd_read(ClipMachineMemory, &hs->file, offs + (pos << 2), 4, c4, __PROC__)))
		  goto err;
	       _rdd_put_uint(c4, _rdd_uint(c4) & (~(1 << ((rec - 1) % 32))));
	       if ((er = rdd_write(ClipMachineMemory, &hs->file, offs + (pos << 2), 4, c4, __PROC__)))
		  goto err;
	    }
	 else
	    {
	       unsigned int npairs, roffs;
	       int l, h, i = 0, c, rpos, nextp;

	       if ((er = rdd_read(ClipMachineMemory, &hs->file, poffs + 4, 4, c4, __PROC__)))
		  goto err;
	       npairs = _rdd_uint(c4);
	       l = 0;
	       h = npairs - 1;
	       while (l <= h)
		  {
		     i = (l + h) / 2;
		     if ((er = _hs_compare(ClipMachineMemory, hs, poffs, i, rec, &c, &offs, &roffs, &rpos, &nextp, __PROC__)))
			goto err;
		     if (c < 0)
			l = i + 1;
		     else if (c > 0)
			h = i - 1;
		     else
			break;
		  }
	       if (!c)
		  {
		     if ((er = _hs_delete(ClipMachineMemory, hs, offs, i % hs->intsonpage, roffs, rpos, nextp, __PROC__)))
			goto err;
		     _rdd_put_uint(c4, npairs - 1);
		     if ((er = rdd_write(ClipMachineMemory, &hs->file, poffs + 4, 4, c4, __PROC__)))
			goto err;
		  }
	    }
      }
   return 0;
 err:
   return er;
}
