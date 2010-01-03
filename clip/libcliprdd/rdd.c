/*
	Copyright (C) 2001  ITK
	Author  : Rushad Nabiullin <rust@itk.ru>,Pavel Lasarev (paul@itk.ru)
	License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI
*/
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include "ci_rdd.h"
#include "ci_coll.h"
#include "ci_charset.h"
#include "ci_error.ch"
#include "ci_six.ch"
#include "ci_btree.h"
#ifdef USE_TASKS
#include "ci_task.h"
#endif
#ifdef HAVE_MMAN_H
#include <sys/mman.h>
#endif

#include "ci_ncp.h"

#ifdef USE_NCPFS
#define fcntl ncp_fcntl
#endif

#define CHECKII(i) \
	if(!(i)) \
		return rdd_err(ClipMachineMemory,EG_ARG,0,__FILE__,__LINE__,__PROC__,"Bad independed index handle"); \

#define CHECKIO(o) \
	if(!(o)) \
		return rdd_err(ClipMachineMemory,EG_ARG,0,__FILE__,__LINE__,__PROC__,"Bad independed order handle"); \

#define er_badinstance      _clip_gettext("Bad RDD instance")
#define er_badfilterexpr    _clip_gettext("Bad filter expression")
#define er_badrelationexpr  _clip_gettext("Bad relation expression")
#define er_badforexpr       _clip_gettext("Bad FOR expression")
#define er_ioerror          _clip_gettext("I/O error")
#define er_badstructure     _clip_gettext("Bad table structure")
#define er_badfield         _clip_gettext("Bad field")
#define er_notpermitted     _clip_gettext("Operation not permitted")
#define er_noorder          _clip_gettext("No controlling order")
#define er_nofield          _clip_gettext("No such field")

int
rdd_flushbuffer(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   int       i, er;

   if (rd->changed)
    {
       if ((er = rd->vtbl->_wlock(ClipMachineMemory, rd, __PROC__)))
	  goto err;
       if ((er = rd->vtbl->setrecord(ClipMachineMemory, rd, __PROC__)))
	  goto err_unlock;
       if ((er = rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__)))
	  goto err;
       rd->changed = 0;
    }
   for (i = 0; i < rd->rels_opened; i++)
    {
       if ((er = rdd_flushbuffer(ClipMachineMemory, rd->relations[i]->child, __PROC__)))
	  goto err;
    }
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}

int
clip_INIT_RDD(ClipMachine * ClipMachineMemory)
{
   strncpy(ClipMachineMemory->def_data_driver, "DBF", 3);
   strncpy(ClipMachineMemory->def_idx_driver, "NTX", 3);
   strncpy(ClipMachineMemory->def_memo_driver, "DBT", 3);
   strncpy(ClipMachineMemory->def_db_driver, "DBFNTX", 6);
   return 0;
}

void
rdd_registerdatadriver(ClipMachine * ClipMachineMemory, RDD_DATA_VTBL * vtbl)
{
   if (!*ClipMachineMemory->ndata_drivers)
      *ClipMachineMemory->data_drivers = (RDD_DATA_VTBL **) malloc(sizeof(RDD_DATA_VTBL *));
   else
      *ClipMachineMemory->data_drivers =
       (RDD_DATA_VTBL **) realloc(*ClipMachineMemory->data_drivers,
				  sizeof(RDD_DATA_VTBL *) * (*ClipMachineMemory->ndata_drivers + 1));
   (*ClipMachineMemory->ndata_drivers)++;
   (*ClipMachineMemory->data_drivers)[*ClipMachineMemory->ndata_drivers - 1] = vtbl;
}

void
rdd_registerindexdriver(ClipMachine * ClipMachineMemory, RDD_INDEX_VTBL * vtbl)
{
   if (!*ClipMachineMemory->nidx_drivers)
      *ClipMachineMemory->idx_drivers = (RDD_INDEX_VTBL **) malloc(sizeof(RDD_INDEX_VTBL *));
   else
      *ClipMachineMemory->idx_drivers =
       (RDD_INDEX_VTBL **) realloc(*ClipMachineMemory->idx_drivers,
				   sizeof(RDD_INDEX_VTBL *) * (*ClipMachineMemory->nidx_drivers + 1));
   (*ClipMachineMemory->nidx_drivers)++;
   (*ClipMachineMemory->idx_drivers)[*ClipMachineMemory->nidx_drivers - 1] = vtbl;
}

void
rdd_registermemodriver(ClipMachine * ClipMachineMemory, RDD_MEMO_VTBL * vtbl)
{
   if (!*ClipMachineMemory->nmemo_drivers)
      *ClipMachineMemory->memo_drivers = (RDD_MEMO_VTBL **) malloc(sizeof(RDD_MEMO_VTBL *));
   else
      *ClipMachineMemory->memo_drivers =
       (RDD_MEMO_VTBL **) realloc(*ClipMachineMemory->memo_drivers,
				  sizeof(RDD_MEMO_VTBL *) * (*ClipMachineMemory->nmemo_drivers + 1));
   (*ClipMachineMemory->nmemo_drivers)++;
   (*ClipMachineMemory->memo_drivers)[*ClipMachineMemory->nmemo_drivers - 1] = vtbl;
}

RDD_DATA_VTBL *
rdd_datadriver(ClipMachine * ClipMachineMemory, const char *driver, const char *__PROC__)
{
   int       i;

   char      err[256];

   if (!driver || !driver[0])
      driver = ClipMachineMemory->def_data_driver;
   for (i = 0; i < *ClipMachineMemory->ndata_drivers; i++)
    {
       if ((strlen(driver) == strlen((*ClipMachineMemory->data_drivers)[i]->id)) &&
	   (strncasecmp((*ClipMachineMemory->data_drivers)[i]->id, driver,
			sizeof((*ClipMachineMemory->data_drivers)[i]->id)) == 0))

	  return (*ClipMachineMemory->data_drivers)[i];
    }
   snprintf(err, sizeof(err), _clip_gettext("No data file driver linked: %s"), driver);
   rdd_err(ClipMachineMemory, EG_UNSUPPORTED, 0, __FILE__, __LINE__, __PROC__, err);
   return NULL;
}

RDD_INDEX_VTBL *
rdd_indexdriver(ClipMachine * ClipMachineMemory, const char *driver, const char *__PROC__)
{
   int       i;

   char      err[256];

   if (!driver || !driver[0])
      driver = ClipMachineMemory->def_idx_driver;
   for (i = 0; i < *ClipMachineMemory->nidx_drivers; i++)
    {
       if ((strlen(driver) == strlen((*ClipMachineMemory->idx_drivers)[i]->id)) &&
	   (strncasecmp((*ClipMachineMemory->idx_drivers)[i]->id, driver,
			sizeof((*ClipMachineMemory->idx_drivers)[i]->id)) == 0))

	  return (*ClipMachineMemory->idx_drivers)[i];
    }
   snprintf(err, sizeof(err), _clip_gettext("No index file driver linked: %s"), driver);
   rdd_err(ClipMachineMemory, EG_UNSUPPORTED, 0, __FILE__, __LINE__, __PROC__, err);
   return NULL;
}

RDD_MEMO_VTBL *
rdd_memodriver(ClipMachine * ClipMachineMemory, const char *driver, const char *__PROC__)
{
   int       i;

   char      err[256];

   if (!driver || !driver[0])
      driver = ClipMachineMemory->def_memo_driver;
   for (i = 0; i < *ClipMachineMemory->nmemo_drivers; i++)
    {
       if ((strlen(driver) == strlen((*ClipMachineMemory->memo_drivers)[i]->id)) &&
	   (strncasecmp((*ClipMachineMemory->memo_drivers)[i]->id, driver,
			sizeof((*ClipMachineMemory->memo_drivers)[i]->id)) == 0))

	  return (*ClipMachineMemory->memo_drivers)[i];
    }
   snprintf(err, sizeof(err), _clip_gettext("No memo file driver linked: %s"), driver);
   rdd_err(ClipMachineMemory, EG_UNSUPPORTED, 0, __FILE__, __LINE__, __PROC__, err);
   return NULL;
}

void
loc_read(DbfLocale * loc, unsigned char *buf, int len)
{
   unsigned char *c = buf;

   unsigned char *e = c + len;

   for (; c < e; c++)
      if (*c & 0x80)
	 *c = loc->read[*c & 0x7f];
}

void
loc_write(DbfLocale * loc, unsigned char *buf, int len)
{
   unsigned char *c = buf;

   unsigned char *e = c + len;

   for (; c < e; c++)
      if (*c & 0x80)
	 *c = loc->write[*c & 0x7f];
}

int
loc_cmp(DbfLocale * loc, unsigned char *s1, unsigned char *s2, int len)
{
   int       r = 0;

   unsigned int ch;

   unsigned char *ss1, *ss2;

   unsigned char *e = s1 + len;

   int       ch1, ch2;

   for (ss1 = s1, ss2 = s2; ss1 < e && !r; ss1++, ss2++)
    {
       ch = *ss1;
       if (ch >> 7)
	  ch = loc->read[ch & 0x7F];
       ch1 = _clip_cmptbl[ch];
       ch2 = _clip_cmptbl[*ss2];
       r = ch1 - ch2;
    }
   return r;
}

int
_rdd_fieldno(RDD_DATA * rd, long hash)
{
   RDD_FIELD *fp = (RDD_FIELD *) HashTable_fetch(rd->hashes, hash);

   int       fno = fp - rd->fields;

   if ((fno >= rd->nfields) || (fno < 0))
      return -1;

   return fno;
}

int
_clip_rddfieldno(ClipMachine * ClipMachineMemory, int h, long hash)
{
   RDD_DATA *rd = (RDD_DATA *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RDD);

   if (!rd)
      return -1;
   return _rdd_fieldno(rd, hash);
}

static void
_rdd_freebuf(ClipMachine * ClipMachineMemory, RDD_DATA * rd)
{
   int       i;

   for (i = 0; i < rd->nfields; i++)
    {
       if (rd->data[i])
	{
	   _clip_destroy(ClipMachineMemory, rd->data[i]);
	   free(rd->data[i]);
	   rd->data[i] = NULL;
	}
    }
}

int
rdd_open(ClipMachine * ClipMachineMemory, char *path, int readonly, int shared, int *fd, const char *__PROC__)
{
   *fd = _clip_open(ClipMachineMemory, path, readonly ? O_RDONLY : O_RDWR, 0, !shared);
   if (*fd == -1)
    {
       return rdd_err(ClipMachineMemory, EG_OPEN, errno, __FILE__, __LINE__, __PROC__, path);
    }
   return 0;
}

int
rdd_calc(ClipMachine * ClipMachineMemory, int area, ClipVar * block, ClipVar * var, int noerror)
{
   int       oldarea = 0;

   int       er;

   if (area != -1)
    {
       oldarea = ClipMachineMemory->curArea;
       ClipMachineMemory->curArea = area;
    }
   if (noerror)
    {
       ClipMachineMemory->noerrblock++;
       if ((er = _clip_eval(ClipMachineMemory, block, 0, 0, var)))
	{
	   var->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
	   var->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = 0;
	}
       ClipMachineMemory->noerrblock--;
    }
   else
    {
       if ((er = _clip_eval(ClipMachineMemory, block, 0, 0, var)))
	  return er;
    }
   if (area != -1)
      ClipMachineMemory->curArea = oldarea;
   return 0;
}

int
rdd_takevalue(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int no, ClipVar * vv, const char *__PROC__)
{
   int       r, er;

   if (rd->pending_child_parent)
      if ((er = rdd_child_duty(ClipMachineMemory, rd, __PROC__)))
	 return er;

   if (rd->eof)
    {
       switch (rd->fields[no].type)
	{
	case 'C':
	   vv->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	   vv->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	   vv->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	   vv->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = rd->fields[no].len;
	   vv->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf =
	    (char *) malloc(vv->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf + 1);
	   memset(vv->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, 32,
		  vv->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
	   vv->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[vv->ClipStrVar_s_of_ClipVar.
										ClipBuf_str_of_ClipStrVar.len_of_ClipBuf] = 0;
	   break;
	case 'M':
	   vv->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	   vv->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	   vv->ClipType_t_of_ClipVar.memo_of_ClipType = 1;
	   vv->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = 0;
	   vv->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = (char *) calloc(1, 1);
	   break;
	case 'N':
	   vv->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	   vv->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	   vv->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	   vv->ClipType_t_of_ClipVar.len_of_ClipType = rd->fields[no].len;
	   vv->ClipType_t_of_ClipVar.dec_of_ClipType = rd->fields[no].dec;
	   vv->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = 0;
	   break;
	case 'D':
	   vv->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATE_type_of_ClipVarType;
	   vv->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	   vv->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	   vv->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = 0;
	   break;
	case 'L':
	   vv->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
	   vv->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	   vv->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	   vv->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = 0;
	   break;
	case 'V':
	   if (rd->fields[no].len == 3)
	    {
	       vv->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATE_type_of_ClipVarType;
	       vv->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	       vv->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	       vv->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = 0;
	    }
	   else if (rd->fields[no].len == 4)
	    {
	       vv->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	       vv->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	       vv->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	       vv->ClipType_t_of_ClipVar.len_of_ClipType = 10;
	       vv->ClipType_t_of_ClipVar.dec_of_ClipType = 0;
	       vv->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = 0;
	    }
	   else if (rd->fields[no].len >= 6)
	    {
	       vv->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	       vv->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	       vv->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	       vv->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = rd->fields[no].len - 6;
	       vv->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf =
		(char *) malloc(vv->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf + 1);
	       memset(vv->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, 32,
		      vv->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
	       vv->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[vv->ClipStrVar_s_of_ClipVar.
										    ClipBuf_str_of_ClipStrVar.len_of_ClipBuf] =
		0;
	    }
	   break;
	}
    }
   else
    {
       if ((er = rd->vtbl->getvalue(ClipMachineMemory, rd, no, vv, __PROC__)))
	  return er;
    }
   if ((er = rdd_event(ClipMachineMemory, EVENT_GET, rd->area, no + 1, vv, &r, __PROC__)))
      return er;
   return 0;
}

int
_clip_rddfield(ClipMachine * ClipMachineMemory, int h, int no)
{
   const char *__PROC__ = "_clip_rddfield";

   RDD_DATA *rd = (RDD_DATA *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RDD);

   ClipVar   vv;

   int       er;

   CLEAR_CLIPVAR(&vv);

   if (!rd)
    {
       rdd_err(ClipMachineMemory, EG_NOTABLE, 0, __FILE__, __LINE__, __PROC__, er_badinstance);
       return -1;
    }
   if ((er = rdd_takevalue(ClipMachineMemory, rd, no, &vv, __PROC__)))
      return er;
   _clip_push(ClipMachineMemory, &vv);
   _clip_destroy(ClipMachineMemory, &vv);
   return 0;
}

int
rdd_err(ClipMachine * ClipMachineMemory, int genCode, int osCode,
	const char *subSystem, int line, const char *operation, const char *desc)
{
   ClipVar  *vp;

   char      buf[256];

   if (osCode)
    {
#ifndef OS_MINGW
       strcpy(ClipMachineMemory->syserr, strerror(osCode));
#else
       FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(),
		     MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT),
		     ClipMachineMemory->syserr, sizeof(ClipMachineMemory->syserr), NULL);
       _clip_translate_charset("cp1251", _clip_hostcs, ClipMachineMemory->syserr,
			       ClipMachineMemory->syserr, strlen(ClipMachineMemory->syserr));
#endif
    }
	/*_clip_logg(0,"RDD layer error: %s/%d: %s: %s",subSystem,line,desc,operation);*/
   snprintf(buf, sizeof(buf), "%s: %s", desc, operation);
   vp = _clip_new_error(ClipMachineMemory, _clip_errname(genCode), 2, genCode, osCode, 0, 0, 0, 1, subSystem, line, buf);
   _clip_trap_var(ClipMachineMemory,
		  ClipMachineMemory->fp ? ClipMachineMemory->fp->filename_of_ClipFrame : "",
		  ClipMachineMemory->fp ? ClipMachineMemory->fp->line_of_ClipFrame : 0, vp);
   return genCode;
}

void
rdd_expandmacro(int area, int r, const char *key, char *expr)
{
   if (area != -1)
    {
       sprintf(expr, "{|| %s}", key);
       return;
    }
   sprintf(expr, "{|| __rdd__ %d %s __rdd__}", r, key);
   return;
}

int
rdd_calcfilter(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *fok, const char *__PROC__)
{
   int       i, er;

   *fok = 1;
   if (!rd->filter)
      return 0;
   for (i = 0; i < rd->filter->nfps; i++)
    {
       ClipVar   v, *vp;

       if ((er = rdd_calc(ClipMachineMemory, rd->area, &rd->filter->fps[i].bfilter, &v, rd->x)))
	  return er;
       vp = _clip_vptr(&v);
       if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
	{
	   _clip_destroy(ClipMachineMemory, &v);
	   return rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_badfilterexpr);
	}
       switch (rd->filter->fps[i].op)
	{
	case 0:
	   *fok = vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
	   break;
	case 1:
	   *fok = *fok || vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
	   break;
	case 2:
	   *fok = *fok && vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
	   break;
	case 3:
	   *fok = (*fok && !vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar) || (!(*fok)
										 && vp->ClipLogVar_l_of_ClipVar.
										 value_of_ClipLogVar);
	   break;
	case 4:
	   *fok = !vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
	}
       _clip_destroy(ClipMachineMemory, &v);
    }
   return 0;
}

int
rdd_checkfilter(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *ok, const char *__PROC__)
{
  /*1:true,0:false,-1:error */
   int       hidedeleted = ClipMachineMemory->flags & DELETED_FLAG;

   int       filterok = 1;

   int       deletedok = 1;

   int       er;

   *ok = 1;
   if (rd->indexing)
      return 0;
   if ((!rd->filter) && !hidedeleted)
      return 0;

   if (hidedeleted)
    {
       int       deleted;

       if ((er = rd->vtbl->deleted(ClipMachineMemory, rd, &deleted, __PROC__)))
	  return er;
       deletedok = !deleted;
    }
   if (deletedok && rd->filter)
    {
       int       lastrec;

       if ((er = rdd_lastrec(ClipMachineMemory, rd, &lastrec, __PROC__)))
	  return er;
       if (rd->recno > lastrec)
	{
	   *ok = 0;
	   return 0;
	}
       if (!rd->filter->custom
	   && rd->filter->fps[0].bfilter.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
	{
	   if (rd->filter->rmap)
	    {
	       filterok = _rm_getbit(rd->filter->rmap, rd->filter->size, rd->recno);
	    }
	   if (filterok)
	    {
	       if ((er = rdd_calcfilter(ClipMachineMemory, rd, &filterok, __PROC__)))
		  return er;
	    }
	}
       else
	{
	   filterok = _rm_getbit(rd->filter->rmap, rd->filter->size, rd->recno);
	}
    }
   *ok = filterok && deletedok;
   return 0;
}

int
rdd_checkifnew(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   RDD_ORDER *ro;

   int       i, er = 0;

   _rdd_freebuf(ClipMachineMemory, rd);
   if (rd->newrec)
    {
       if (rd->ords_opened > 0)
	{
	   for (i = 0; i < rd->ords_opened; i++)
	    {
	       ro = rd->orders[i];
	       if (!ro->custom && rd->orders[i]->canadd)
		{
		   if ((er = rdd_event(ClipMachineMemory, EVENT_UPDATE, rd->area, 0, NULL, NULL, __PROC__)))
		      return er;
		   if ((er = ro->vtbl->_wlock(ClipMachineMemory, ro, __PROC__)))
		      goto unlock;
		   if ((er = ro->vtbl->addkey(ClipMachineMemory, rd, ro, NULL, __PROC__)))
		      goto unlock;
		   if ((er = ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__)))
		      goto unlock;
		}
	    }
	}
       rd->newrec = 0;
    }
   return 0;
 unlock:
   for (i = 0; i < rd->ords_opened; i++)
    {
       ro = rd->orders[i];
       if (!ro->custom && rd->orders[i]->canadd)
	{
	   ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__);
	}
    }
   return er;
}

int
rdd_childs(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   RDD_DATA *child;

   int       i, er;

   for (i = 0; i < rd->rels_opened; i++)
    {
       child = rd->relations[i]->child;
       child->pending_child_parent = rd;
       if ((er = rdd_childs(ClipMachineMemory, child, __PROC__)))
	  return er;
    }
   return 0;
}

int
rdd_child_duty(ClipMachine * ClipMachineMemory, RDD_DATA * child, const char *__PROC__)
{
   int       i, er = 0, found;

   ClipVar   v, *vp;

   RDD_DATA *rd = child->pending_child_parent;

   RDD_RELATION *rel = NULL;

   int       locked = child->file.rlocked || child->file.wlocked;

   CLEAR_CLIPVAR(&v);
   child->pending_child_parent = NULL;

   if (!rd)
      goto err;
   for (i = 0; i < child->pars_opened; i++)
    {
       if (child->parents[i]->parent == rd)
	{
	   rel = child->parents[i];
	   break;
	}
    }
   if (!rel)
      goto err;

   if (!locked)
    {
       if ((er = child->vtbl->_rlock(ClipMachineMemory, child, __PROC__)))
	  goto err;
    }
   if (rel->simpexpr)
    {
       if ((er = rdd_takevalue(ClipMachineMemory, rd, rel->simpfno, &v, __PROC__)))
	  goto err_unlock;
    }
   else
    {
       if ((er = rdd_calc(ClipMachineMemory, rd->area, &rel->block, &v, 0)))
	  goto err_unlock;
    }
   vp = _clip_vptr(&v);
   if ((child->ords_opened > 0) && (child->curord != -1))
    {
       if ((er = rdd_checkifnew(ClipMachineMemory, child, __PROC__)))
	  goto err_unlock;
       if (rel->scoped)
	{
	   if ((er = rdd_scopetop(ClipMachineMemory, child, vp, __PROC__)))
	      goto err_unlock;
	   if ((er = rdd_scopebottom(ClipMachineMemory, child, vp, __PROC__)))
	      goto err_unlock;
	}
       if (rd->eof)
	{
	   unsigned int lastrec;

	   child->orders[child->curord]->valid_stack = 0;
	   child->eof = 1;
	   if ((er = rdd_lastrec(ClipMachineMemory, child, (int *) (&lastrec), __PROC__)))
	      return er;
	   child->recno = lastrec + 1;
	}
       else
	{
	   if ((er =
		child->orders[child->curord]->vtbl->seek(ClipMachineMemory, child,
							 child->orders[child->curord], vp, 0, 0, &found, __PROC__)))
	      goto err_unlock;
	}
    }
   else
    {
       if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != NUMERIC_type_of_ClipVarType)
	{
	   er = rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_badrelationexpr);
	   goto err_unlock;
	}
       if ((er = child->vtbl->go(ClipMachineMemory, child, vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, __PROC__)))
	  goto err_unlock;
       if (child->area != -1)
	  ((DBWorkArea *) (ClipMachineMemory->areas->items_of_ClipVect[child->area]))->found = !child->eof;
    }
   if (!locked)
    {
       if ((er = child->vtbl->_ulock(ClipMachineMemory, child, __PROC__)))
	  goto err;
    }

   _clip_destroy(ClipMachineMemory, &v);
   return 0;

 err_unlock:
   if (!locked)
      child->vtbl->_ulock(ClipMachineMemory, child, __PROC__);
 err:
   _clip_destroy(ClipMachineMemory, &v);
   return er;
}

int
_rdd_read(ClipMachine * ClipMachineMemory, RDD_FILE * file, int pos, int len, void *buf, const char *__PROC__)
{
#ifdef HAVE_MMAN_H
   struct stat st;

   int       realen;
#endif
   if (file->dbf && !file->rlocked && !file->wlocked)
    {
       char      buf[256];

       time_t    tt;

       struct tm *tp;

       tt = time(0);
       tp = localtime(&tt);
       snprintf(buf, sizeof(buf),
		"%02d:%02d:%02d: Warning: unsafe DBF read: wlocked=%d; %s(%s)\n",
		tp->tm_hour, tp->tm_min, tp->tm_sec, file->wlocked, __PROC__, file->dbf);
       _clip_out_log(buf, strlen(buf));
       _clip_flush_log();
    }
#ifdef HAVE_MMAN_H
   if (file->md != (caddr_t) - 1)
    {
       if (pos + len > file->mapsize)
	{
	   if (fstat(file->fd, &st) == -1)
	      goto err;
	   if (file->mapsize < st.st_size)
	    {
	       if (munmap(file->md, file->mapsize) == -1)
		  goto err;
	       file->mapsize = st.st_size;
	       file->md = (caddr_t) mmap(0, file->mapsize, PROT_READ | PROT_WRITE, MAP_SHARED, file->fd, 0);
	       if (file->md == (caddr_t) - 1)
		{
		   if (lseek(file->fd, pos, SEEK_SET) == -1)
		      goto err;
		   if (read(file->fd, buf, len) == -1)
		      goto err;
		   return 0;
		}
	    }
	}
       realen = (pos + len > file->mapsize) ? file->mapsize - pos : len;
       realen = realen < 0 ? 0 : realen;
       memcpy(buf, file->md + pos, realen);
       memset(buf + realen, 0, len - realen);
    }
   else
    {
#endif
       if (lseek(file->fd, pos, SEEK_SET) == -1)
	  goto err;
       if (read(file->fd, buf, len) == -1)
	  goto err;
#ifdef HAVE_MMAN_H
    }
#endif
   return 0;
 err:
   return rdd_err(ClipMachineMemory, EG_READ, errno, __FILE__, __LINE__, __PROC__, er_ioerror);
}

int
_rdd_write(ClipMachine * ClipMachineMemory, RDD_FILE * file, int pos, int len, void *buf, const char *__PROC__)
{
   struct stat st;

   if (file->dbf && !file->wlocked)
    {
       char      buf[256];

       time_t    tt;

       struct tm *tp;

       tt = time(0);
       tp = localtime(&tt);
       snprintf(buf, sizeof(buf),
		"%02d:%02d:%02d: Warning: unsafe DBF write: rlocked=%d; %s(%s)\n",
		tp->tm_hour, tp->tm_min, tp->tm_sec, file->rlocked, __PROC__, file->dbf);
       _clip_out_log(buf, strlen(buf));
       _clip_flush_log();
    }
#ifdef HAVE_MMAN_H
   if (file->md != (caddr_t) - 1)
    {
       if (pos + len > file->mapsize)
	{
	   if (munmap(file->md, file->mapsize) == -1)
	      goto err;
#ifdef _WIN32
	   {
	      void     *bf;

	      int       p;

	      if ((p = lseek(file->fd, 0, SEEK_END)) == -1)
		 goto err;
	      bf = calloc(1, pos + len - p);
	      if (write(file->fd, bf, pos + len - p) == -1)
		 goto err;
	      free(bf);
	   }
#else
	   if (lseek(file->fd, pos + len - 1, SEEK_SET) == -1)
	      goto err;
	   if (write(file->fd, "", 1) == -1)
	      goto err;
#endif
	   file->mapsize = pos + len;
	   file->md = (caddr_t) mmap(0, file->mapsize, PROT_READ | PROT_WRITE, MAP_SHARED, file->fd, 0);
	   if (file->md == (caddr_t) - 1)
	    {
	       if (lseek(file->fd, pos, SEEK_SET) == -1)
		  goto err;
	       if (write(file->fd, buf, len) == -1)
		  goto err;
	       return 0;
	    }
	}
       memcpy(file->md + pos, buf, len);
    }
   else
    {
#endif
      /* ext2 fs issue */
       if (len == 0)
	{
	   if (fstat(file->fd, &st) == -1)
	      goto err;
	   if (pos > st.st_size)
	    {
	       if (lseek(file->fd, pos - 1, SEEK_SET) == -1)
		  goto err;
	       if (write(file->fd, "", 1) == -1)
		  goto err;
	    }
	}
       else
	{
	   if (lseek(file->fd, pos, SEEK_SET) == -1)
	      goto err;
	   if (write(file->fd, buf, len) == -1)
	      goto err;
	}
#ifdef HAVE_MMAN_H
    }
#endif
   return 0;
 err:
   return rdd_err(ClipMachineMemory, EG_WRITE, errno, __FILE__, __LINE__, __PROC__, er_ioerror);
}

int
_rdd_trunc(ClipMachine * ClipMachineMemory, RDD_FILE * file, unsigned int len, const char *__PROC__)
{
#ifdef HAVE_MMAN_H
   if (file->md != (caddr_t) - 1)
    {
       if (munmap(file->md, file->mapsize) == -1)
	  goto err;
       if (ftruncate(file->fd, len) == -1)
	  goto err;
       file->mapsize = len;
       file->md = (caddr_t) mmap(0, file->mapsize, PROT_READ | PROT_WRITE, MAP_SHARED, file->fd, 0);
    }
   else
    {
#endif
       if (ftruncate(file->fd, len) == -1)
	  goto err;
#ifdef HAVE_MMAN_H
    }
#endif
   return 0;
 err:
   return rdd_err(ClipMachineMemory, EG_WRITE, errno, __FILE__, __LINE__, __PROC__, er_ioerror);
}

int
_rdd_read64(ClipMachine * ClipMachineMemory, RDD_FILE * file, u8 pos, int len, void *buf, const char *__PROC__)
{
   return 0;
}

int
_rdd_write64(ClipMachine * ClipMachineMemory, RDD_FILE * file, u8 pos, int len, void *buf, const char *__PROC__)
{
   return 0;
}

int
_rdd_trunc64(ClipMachine * ClipMachineMemory, RDD_FILE * file, u8 len, const char *__PROC__)
{
   return 0;
}

int
rdd_create(ClipMachine * ClipMachineMemory, const char *driver,
	   const char *memo_driver, const char *name, ClipVar * stru, const char *__PROC__)
{
   int       count = stru->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;

   RDD_STRUCT *rddstru = NULL;

   int       i, j, er = EG_UNSUPPORTED;

   RDD_DATA_VTBL *vtbl;

   char     *path = NULL;

   if (count < 1)
    {
       er = rdd_err(ClipMachineMemory, EG_CREATE, 0, __FILE__, __LINE__, __PROC__, er_badstructure);
       goto err;
    }
   rddstru = calloc(count, sizeof(RDD_STRUCT));
   for (i = 0; i < count; i++)
    {
       ClipVar  *vp;

       ClipVar  *name;

       ClipVar  *type;

       ClipVar  *len;

       ClipVar  *dec;

       ClipVar  *binary;

       ClipVar  *nullable;

       vp = _clip_vptr(stru->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i);
       if ((vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType)
	   || (vp->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar < 4))
	{
	   er = rdd_err(ClipMachineMemory, EG_CREATE, 0, __FILE__, __LINE__, __PROC__, er_badfield);
	   goto err;
	}

       name = _clip_vptr(vp->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + 0);
       if (name->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType)
	{
	   er = rdd_err(ClipMachineMemory, EG_CREATE, 0, __FILE__, __LINE__, __PROC__, er_badfield);
	   goto err;
	}
       strncpy(rddstru[i].name, name->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf,
	       sizeof(rddstru[i].name));
       rddstru[i].name[sizeof(rddstru[i].name) - 1] = 0;
       for (j = 0; j < name->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf; j++)
	  rddstru[i].name[j] = toupper(rddstru[i].name[j]);

       type = _clip_vptr(vp->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + 1);
       if (type->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType)
	{
	   er = rdd_err(ClipMachineMemory, EG_CREATE, 0, __FILE__, __LINE__, __PROC__, er_badfield);
	   goto err;
	}
       rddstru[i].type = toupper(type->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[0]);

       if (rddstru[i].type == 'D')
	{
	   rddstru[i].len = 8;
	   rddstru[i].dec = 0;
	}
       else if (rddstru[i].type == 'L')
	{
	   rddstru[i].len = 1;
	   rddstru[i].dec = 0;
	}
       else if (rddstru[i].type == 'M')
	{
	   rddstru[i].len = 10;
	   rddstru[i].dec = 0;
	}
       else
	{
	   len = _clip_vptr(vp->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + 2);
	   if (len->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != NUMERIC_type_of_ClipVarType)
	    {
	       er = rdd_err(ClipMachineMemory, EG_CREATE, 0, __FILE__, __LINE__, __PROC__, er_badfield);
	       goto err;
	    }
	   rddstru[i].len = (int) (len->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar + 0.5);

	   dec = _clip_vptr(vp->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + 3);
	   if (dec->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != NUMERIC_type_of_ClipVarType)
	    {
	       er = rdd_err(ClipMachineMemory, EG_CREATE, 0, __FILE__, __LINE__, __PROC__, er_badfield);
	       goto err;
	    }
	   rddstru[i].dec = (int) (dec->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar + 0.5);
	}
       if (vp->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar >= 5)
	{
	   binary = _clip_vptr(vp->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + 4);
	   if (binary->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == LOGICAL_type_of_ClipVarType)
	      rddstru[i].binary = binary->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
	}
       if (vp->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar >= 6)
	{
	   nullable = _clip_vptr(vp->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + 5);
	   if (nullable->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == LOGICAL_type_of_ClipVarType)
	      rddstru[i].nullable = nullable->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
	}
    }
   if (!(vtbl = rdd_datadriver(ClipMachineMemory, driver, __PROC__)))
      goto err;

   if ((er = _rdd_parsepath(ClipMachineMemory, name, vtbl->suff, &path, NULL, EG_CREATE, __PROC__)))
      goto err;

   if ((er = vtbl->create(ClipMachineMemory, vtbl, path, rddstru, count, __PROC__)))
      goto err;
   free(path);
   free(rddstru);
   return 0;
 err:
   if (path)
      free(path);
   if (rddstru)
      free(rddstru);
   return er;
}

int
rdd_creatememo(ClipMachine * ClipMachineMemory, const char *driver, const char *name, const char *__PROC__)
{
   RDD_MEMO_VTBL *vtbl;

   char     *path = NULL;

   int       er = EG_UNSUPPORTED;

   if (!(vtbl = rdd_memodriver(ClipMachineMemory, driver, __PROC__)))
      goto err;

   if ((er = _rdd_parsepath(ClipMachineMemory, name, vtbl->suff, &path, NULL, EG_CREATE, __PROC__)))
      goto err;

   if ((er = vtbl->create(ClipMachineMemory, path, __PROC__)))
      goto err;
   free(path);
   return 0;
 err:
   if (path)
      free(path);
   return er;
}

int
rdd_pack(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   char     *s;

   char      tmp[PATH_MAX];

   int       tmpfd;

   struct stat st;

   int       r, er;

   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;
   if ((er = rdd_event(ClipMachineMemory, EVENT_PACK, rd->area, 0, NULL, &r, __PROC__)))
      return er;
   if (!r)
      return 0;

   if (rd->shared)
      return rdd_err(ClipMachineMemory, EG_SHARED, 0, __FILE__, __LINE__, __PROC__, er_notpermitted);

   if (rd->readonly)
      return rdd_err(ClipMachineMemory, EG_READONLY, 0, __FILE__, __LINE__, __PROC__, er_notpermitted);

   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;

#ifdef HAVE_MMAN_H
   if (rd->file.md != (caddr_t) - 1)
    {
       if (munmap(rd->file.md, rd->file.mapsize) == -1)
	  goto err;
    }
#endif
   if (_clip_close(ClipMachineMemory, rd->file.filehash, rd->file.fd) == -1)
      goto err;

   strcpy(tmp, rd->path);
   s = strrchr(tmp, '/');
   sprintf(s + 1, "data%lx", (long) getpid());
   sprintf(s + 9, ".%lx", (long) random());
   s[13] = 0;

   if (rename(rd->path, tmp) == -1)
      goto err;

#ifdef _WIN32
   rd->file.fd = _clip_creat(ClipMachineMemory, rd->path, O_RDWR | O_BINARY, ClipMachineMemory->fileCreateMode, !rd->shared);
#else
   rd->file.fd = _clip_creat(ClipMachineMemory, rd->path, O_RDWR, ClipMachineMemory->fileCreateMode, !rd->shared);
#endif
   if (rd->file.fd == -1)
      goto err_create;
   rd->vtbl->_wlock(ClipMachineMemory, rd, __PROC__);
   if ((er = rdd_open(ClipMachineMemory, tmp, 0, 0, &tmpfd, __PROC__)))
      return er;

   if ((er = rd->vtbl->pack(ClipMachineMemory, rd, tmpfd, __PROC__)))
      return er;
   if (_clip_close(ClipMachineMemory, _clip_hashstr(tmp), tmpfd) == -1)
      goto err;

   if (fstat(rd->file.fd, &st) == -1)
      goto err;
   rd->file.mapsize = st.st_size;
   rd->file.md = (caddr_t) - 1;
#ifdef HAVE_MMAN_H
   if (ClipMachineMemory->flags1 & MAP_FILE_FLAG)
      rd->file.md = (caddr_t) mmap(0, rd->file.mapsize, PROT_READ | PROT_WRITE, MAP_SHARED, rd->file.fd, 0);
#endif
   if (remove(tmp) == -1)
      goto err;
   rd->eof = 1;
   if ((er = rdd_reindex(ClipMachineMemory, rd, __PROC__)))
      return er;
   return 0;
 err:
   return rdd_err(ClipMachineMemory, EG_WRITE, errno, __FILE__, __LINE__, __PROC__, er_ioerror);
 err_create:
   return rdd_err(ClipMachineMemory, EG_CREATE, errno, __FILE__, __LINE__, __PROC__, rd->path);
}

int
rdd_zap(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   int       i, r, er;

   if ((er = rdd_event(ClipMachineMemory, EVENT_ZAP, rd->area, 0, NULL, &r, __PROC__)))
      return er;
   if (!r)
      return 0;

   if (rd->shared)
      return rdd_err(ClipMachineMemory, EG_SHARED, 0, __FILE__, __LINE__, __PROC__, er_notpermitted);
   if (rd->readonly)
      return rdd_err(ClipMachineMemory, EG_READONLY, 0, __FILE__, __LINE__, __PROC__, er_notpermitted);

   if ((er = rd->vtbl->zap(ClipMachineMemory, rd, __PROC__)))
      return er;
   for (i = 0; i < rd->idxs_opened; i++)
      if ((er = rd->indices[i]->vtbl->zap(ClipMachineMemory, rd, rd->indices[i], __PROC__)))
	 return er;
   if (rd->memo)
      if ((er = rd->memo->vtbl->zap(ClipMachineMemory, rd->memo, __PROC__)))
	 return er;
   rd->bof = rd->v_bof = rd->eof = 1;
   rd->recno = 1;
   rd->newrec = 0;
   rd->valid = 0;
   return 0;
}

int
rdd_usearea(ClipMachine * ClipMachineMemory, const char *driver, const char *name,
	    int shared, int readonly, int tempo, RDD_DATA ** rdp, const char *__PROC__)
{
   RDD_DATA *rd = (RDD_DATA *) calloc(1, sizeof(RDD_DATA));

   struct stat st;

   int       er = EG_UNSUPPORTED;

   rd->file.fd = -1;
   rd->file.md = (char *) -1;
   rd->locate_next = -1;
   rd->ClipMachineMemory = ClipMachineMemory;

   *rdp = NULL;
   if (!(rd->vtbl = rdd_datadriver(ClipMachineMemory, driver, __PROC__)))
    {
       er = 1;
       goto err;
    }
   rd->loc = dbf_get_locale(ClipMachineMemory);
   rd->lockstyle = ClipMachineMemory->lockstyle;

   if ((er = _rdd_parsepath(ClipMachineMemory, name, rd->vtbl->suff, &rd->path, &rd->name, EG_OPEN, __PROC__)))
      goto err;

   if (readonly && !shared)
      shared = 1;
   rd->shared = shared;
   rd->readonly = readonly;
   rd->tempo = tempo;

   rd->file.dbf = rd->path;
   rd->file.filehash = _clip_hashstr(rd->path);

   if ((er = rdd_open(ClipMachineMemory, rd->path, rd->readonly, rd->shared, &rd->file.fd, __PROC__)))
      goto err;

   if (fstat(rd->file.fd, &st) == -1)
      goto err_open;
   rd->file.mapsize = st.st_size;
   rd->hashes = new_HashTable();

   rd->file.md = (caddr_t) - 1;
#ifdef HAVE_MMAN_H
   if ((ClipMachineMemory->flags1 & MAP_FILE_FLAG) && !HashTable_fetch(ClipMachineMemory->tasklocks, rd->file.filehash))
      rd->file.md = (caddr_t) mmap(0, rd->file.mapsize, PROT_READ | (readonly ? 0 : PROT_WRITE), MAP_SHARED, rd->file.fd, 0);
#endif
  /* If failed use non-mapped file (md==-1) */

   if ((er = rd->vtbl->_rlock(ClipMachineMemory, rd, __PROC__)))
      goto err;
   if ((er = rd->vtbl->open(ClipMachineMemory, rd, __PROC__)))
      goto err_unlock;

   rd->curord = -1;
   rd->idxs_opened = 0;
   rd->ords_opened = 0;
   rd->rels_opened = 0;
   rd->indices = NULL;
   rd->orders = NULL;
   rd->relations = NULL;
   rd->locks = NULL;
   rd->nlocks = 0;
   rd->data = (ClipVar **) calloc(rd->nfields, sizeof(ClipVar *));

   if ((er = rd->vtbl->gotop(ClipMachineMemory, rd, __PROC__)))
      goto err_unlock;
   if ((er = rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__)))
      goto err;
   *rdp = rd;

   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
   goto err;
 err_open:
   er = rdd_err(ClipMachineMemory, EG_OPEN, errno, __FILE__, __LINE__, __PROC__, rd->path);
 err:
#ifdef HAVE_MMAN_H
   if (rd && rd->file.md != (char *) -1)
      munmap(rd->file.md, rd->file.mapsize);
#endif
   if (rd && rd->file.fd != -1)
      _clip_close(ClipMachineMemory, rd->file.filehash, rd->file.fd);
   if (rd && rd->name)
      free(rd->name);
   if (rd && rd->path)
      free(rd->path);
   if (rd)
      free(rd);
   return er;
}

int
rdd_setindex(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX ** rip,
	     const char *driver, const char *name, const char *tag, int structural, const char *__PROC__)
{
   RDD_INDEX *ri = (RDD_INDEX *) calloc(1, sizeof(RDD_INDEX));

   struct stat st;

   int       i, er = EG_UNSUPPORTED;

   int       l = strlen(name);

   if (!name || !l)
      return 0;

   ri->file.fd = -1;
   ri->file.md = (char *) -1;
   ri->rd = rd;

   if (!structural)
    {
       if (!(l > 4 && isalnum(name[l - 1]) && isalnum(name[l - 2]) && isalnum(name[l - 3]) && name[l - 4] == '.'))
	{

	   if (!tag && (rd->area != -1) && (strcasecmp(driver, "CDX") == 0))
	    {
	       if (!(ri->vtbl = rdd_indexdriver(ClipMachineMemory, "IDX", __PROC__)))
		  goto err;
	       if ((er = _rdd_parsepath(ClipMachineMemory, name, ri->vtbl->suff, &ri->path, &ri->name, EG_OPEN, __PROC__)))
		  goto err;
	       if (access(ri->path, F_OK))
		{
		   free(ri->path);
		   ri->path = NULL;
		   free(ri->name);
		   ri->name = NULL;
		   if (!(ri->vtbl = rdd_indexdriver(ClipMachineMemory, driver, __PROC__)))
		      goto err;
		   if ((er = _rdd_parsepath(ClipMachineMemory, name, ri->vtbl->suff, &ri->path, &ri->name, EG_OPEN, __PROC__)))
		      goto err;
		}
	    }
	   else if (!tag && (rd->area != -1) && (strcasecmp(driver, "CTX") == 0))
	    {
	       if (!(ri->vtbl = rdd_indexdriver(ClipMachineMemory, "NTX", __PROC__)))
		  goto err;
	       if ((er = _rdd_parsepath(ClipMachineMemory, name, ri->vtbl->suff, &ri->path, &ri->name, EG_OPEN, __PROC__)))
		  goto err;
	       if (access(ri->path, F_OK))
		{
		   free(ri->path);
		   ri->path = NULL;
		   free(ri->name);
		   ri->name = NULL;
		   if (!(ri->vtbl = rdd_indexdriver(ClipMachineMemory, driver, __PROC__)))
		      goto err;
		   if ((er = _rdd_parsepath(ClipMachineMemory, name, ri->vtbl->suff, &ri->path, &ri->name, EG_OPEN, __PROC__)))
		      goto err;
		}
	    }
	   else
	    {
	       if (!(ri->vtbl = rdd_indexdriver(ClipMachineMemory, driver, __PROC__)))
		  goto err;
	       if ((er = _rdd_parsepath(ClipMachineMemory, name, ri->vtbl->suff, &ri->path, &ri->name, EG_OPEN, __PROC__)))
		  goto err;
	    }
	}
       else
	{
	   if (!(ri->vtbl = rdd_indexdriver(ClipMachineMemory, driver, __PROC__)))
	      goto err;
	   if ((er = _rdd_parsepath(ClipMachineMemory, name, ri->vtbl->suff, &ri->path, &ri->name, EG_OPEN, __PROC__)))
	      goto err;
	}
    }
   else
    {
       if (!(ri->vtbl = rdd_indexdriver(ClipMachineMemory, driver, __PROC__)))
	  goto err;
       ri->path = strdup(name);
       ri->name = strdup(rd->name);
    }

   for (i = 0; i < rd->idxs_opened; i++)
      if (strcmp(rd->indices[i]->path, ri->path) == 0)
       {
	  free(ri->name);
	  free(ri->path);
	  free(ri);
	  if (rip)
	     *rip = rd->indices[i];
	  return rdd_setorder(ClipMachineMemory, rd, rd->indices[i]->orders[0]->orderno + 1, __PROC__);
       }

   ri->file.filehash = _clip_hashstr(ri->path);

   if ((er = rdd_open(ClipMachineMemory, ri->path, rd->readonly, rd->shared, &ri->file.fd, __PROC__)))
      goto err;

   if (fstat(ri->file.fd, &st) == -1)
      goto err_open;
   ri->file.mapsize = st.st_size;
   ri->file.md = (caddr_t) - 1;
#ifdef HAVE_MMAN_H
   if ((ClipMachineMemory->flags1 & MAP_FILE_FLAG) && !HashTable_fetch(ClipMachineMemory->tasklocks, rd->file.filehash))
      ri->file.md =
       (caddr_t) mmap(0, ri->file.mapsize, PROT_READ | (rd->readonly ? 0 : PROT_WRITE), MAP_SHARED, ri->file.fd, 0);
  /* If failed use non-mapped file (md==-1) */
#endif
   if ((er = ri->vtbl->open(ClipMachineMemory, rd, ri, tag, __PROC__)))
      goto err;
   if (rd->curord == -1)
    {
       rd->curord = ri->orders[0]->orderno;
    }
   rd->idxs_opened++;
   rd->indices = (RDD_INDEX **) realloc(rd->indices, sizeof(RDD_INDEX *) * rd->idxs_opened);
   rd->indices[rd->idxs_opened - 1] = ri;
   ri->indexno = rd->idxs_opened - 1;

   for (i = 0; i < ri->norders; i++)
    {
       RDD_ORDER *ro = ri->orders[i];

       if (ro->cforexpr && ro->cforexpr[0])
	{
	   int       r = _clip_parni(ClipMachineMemory, 1);

	   char      expr[1024];

	   memset(expr, 0, sizeof(expr));
	   rdd_expandmacro(rd->area, r, ro->cforexpr, expr);

	   if ((er = _clip_eval_macro(ClipMachineMemory, expr, strlen(expr), &ro->bforexpr)))
	      goto err;
	}
    }
   if (rip)
      *rip = ri;
   return 0;
 err_open:
   er = rdd_err(ClipMachineMemory, EG_OPEN, errno, __FILE__, __LINE__, __PROC__, ri->path);
 err:
   if (ri && ri->name)
      free(ri->name);
   if (ri && ri->path)
      free(ri->path);
#ifdef HAVE_MMAN_H
   if (ri && ri->file.md != (char *) -1)
      munmap(ri->file.md, ri->file.mapsize);
#endif
   if (ri && ri->file.fd != -1)
      _clip_close(ClipMachineMemory, ri->file.filehash, ri->file.fd);
   if (ri)
      free(ri);
   return er;
}

int
rdd_setmemo(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *driver, const char *name, const char *__PROC__)
{
   RDD_MEMO *rm = (RDD_MEMO *) calloc(1, sizeof(RDD_MEMO));

   struct stat st;

   int       er = EG_UNSUPPORTED;

   if (rd->memo)
    {
       er = rdd_err(ClipMachineMemory, EG_OPEN, 0, __FILE__, __LINE__, __PROC__, "Memo file is already opened");
       goto err;
    }

   memset(rm, 0, sizeof(RDD_MEMO));
   rm->loc = rd->loc;
   if (!(rm->vtbl = rdd_memodriver(ClipMachineMemory, driver, __PROC__)))
      goto err;

   if ((er = _rdd_parsepath(ClipMachineMemory, name, rm->vtbl->suff, &rm->path, &rm->name, EG_OPEN, __PROC__)))
      goto err;

   rm->file.filehash = _clip_hashstr(rm->path);

   if ((er = rdd_open(ClipMachineMemory, rm->path, rd->readonly, rd->shared, &rm->file.fd, __PROC__)))
      goto err;

   if (fstat(rm->file.fd, &st) == -1)
      goto err_open;
   rm->file.mapsize = st.st_size;

   rm->file.md = (caddr_t) - 1;
#ifdef HAVE_MMAN_H
   if ((ClipMachineMemory->flags1 & MAP_FILE_FLAG) && !HashTable_fetch(ClipMachineMemory->tasklocks, rd->file.filehash))
      rm->file.md =
       (caddr_t) mmap(0, rm->file.mapsize, PROT_READ | (rd->readonly ? 0 : PROT_WRITE), MAP_SHARED, rm->file.fd, 0);
  /* If failed use non-mapped file (md==-1) */
#endif
   rd->memo = rm;

   if ((er = rm->vtbl->open(ClipMachineMemory, rd, rm, __PROC__)))
      goto err;

   return 0;
 err_open:
   er = rdd_err(ClipMachineMemory, EG_OPEN, errno, __FILE__, __LINE__, __PROC__, rm->path);
 err:
   if (rm && rm->name)
      free(rm->name);
   if (rm && rm->path)
      free(rm->path);
#ifdef HAVE_MMAN_H
   if (rm && rm->file.md != (char *) -1)
      munmap(rm->file.md, rm->file.mapsize);
#endif
   if (rm && rm->file.fd != -1)
      _clip_close(ClipMachineMemory, rm->file.filehash, rm->file.fd);
   if (rm)
      free(rm);
   rd->memo = NULL;
   return er;
}

int
rdd_createindex(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *driver,
		const char *name, const char *tag, const char *expr, ClipVar * block, int unique, const char *__PROC__)
{
   RDD_INDEX *ri = NULL;

   RDD_ORDER *ro = NULL;

   int       alreadyopened = 0;

   int       cr = 0, er = EG_UNSUPPORTED;

   int       i, j;

   char      tagname[11];

   ri = (RDD_INDEX *) calloc(1, sizeof(RDD_INDEX));
   if (rd->area != -1)
    {
       if (!tag && strcasecmp(driver, "CDX") == 0)
	{
	   if (!(ri->vtbl = rdd_indexdriver(ClipMachineMemory, "IDX", __PROC__)))
	      goto err;
	}
       else if (!tag && strcasecmp(driver, "CTX") == 0)
	{
	   if (!(ri->vtbl = rdd_indexdriver(ClipMachineMemory, "NTX", __PROC__)))
	      goto err;
	}
       else
	{
	   if (!(ri->vtbl = rdd_indexdriver(ClipMachineMemory, driver, __PROC__)))
	      goto err;
	}
    }
   else
    {
       if (!(ri->vtbl = rdd_indexdriver(ClipMachineMemory, driver, __PROC__)))
	  goto err;
    }
   if ((er = _rdd_parsepath(ClipMachineMemory, name, ri->vtbl->suff, &ri->path, &ri->name, EG_CREATE, __PROC__)))
      goto err;

   ri->file.filehash = _clip_hashstr(ri->path);

   if (ri->vtbl->ismulti && (access(ri->path, F_OK) == 0))
    {
       free(ri->path);
       free(ri->name);
       free(ri);
       if ((er = rdd_setindex(ClipMachineMemory, rd, &ri, driver, name, NULL, 0, __PROC__)))
	  goto err;
       alreadyopened = ri->indexno + 1;
    }

   ri->rd = rd;
   ri->loc = rd->loc;

   if (!alreadyopened)
    {
       struct stat st;

       ri->file.fd = _clip_creat(ClipMachineMemory, ri->path, O_RDWR, ClipMachineMemory->fileCreateMode, rd->shared);
       if (ri->file.fd == -1)
	  goto err_create;
       ri->file.mapsize = 1024;
       if (fstat(ri->file.fd, &st) == -1)
	  goto err_open;
       if (lseek(ri->file.fd, ri->file.mapsize - 1, SEEK_SET) == -1)
	  goto err_create;
       if (write(ri->file.fd, "", 1) == -1)
	  goto err_create;
       ri->file.md = (caddr_t) - 1;
#ifdef HAVE_MMAN_H
       if ((ClipMachineMemory->flags1 & MAP_FILE_FLAG) && !HashTable_fetch(ClipMachineMemory->tasklocks, rd->file.filehash))
	  ri->file.md = (caddr_t) mmap(0, ri->file.mapsize, PROT_READ | PROT_WRITE, MAP_SHARED, ri->file.fd, 0);
      /* If failed use non-mapped file (md==-1) */
#endif
       cr = 1;
    }

   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      goto err;

   if (tag)
    {
       strncpy(tagname, tag, 10);
       tagname[10] = 0;
       _clip_upstr(tagname, 10);
    }
   rd->indexing = ri->name;
   if ((er = ri->vtbl->create(ClipMachineMemory, rd, ri, &ro, tag ? tagname : NULL, expr, block, unique, cr, 0, __PROC__)))
      goto err;
   rd->indexing = NULL;

   if (ri->structural && !rd->readonly)
    {
       if ((er = rd->vtbl->setstruct(ClipMachineMemory, rd, __PROC__)))
	  goto err;
    }

   if (!rd->os.lAdditive)
    {
       j = rd->idxs_opened;
       for (i = 0; i < j; i++)
	{
	   if (rd->indices[i]->structural || rd->indices[i]->indexno == alreadyopened - 1)
	      continue;
	   rd->idxs_opened--;
	   rd->ords_opened -= rd->indices[i]->norders;
	   if (rd->indices[i] != ri)
	    {
#ifdef HAVE_MMAN_H
	       if ((rd->indices[i]->file.md) != (caddr_t) - 1)
		{
		   if (munmap(rd->indices[i]->file.md, rd->indices[i]->file.mapsize) == -1)
		      goto err_close;
		}
#endif
	       if (_clip_close(ClipMachineMemory, rd->indices[i]->file.filehash, rd->indices[i]->file.fd) == -1)
		  goto err_close;
	       if ((er = rd->indices[i]->vtbl->close(ClipMachineMemory, rd, rd->indices[i], __PROC__)))
		  goto err;
	    }
	}
    }
   if (!alreadyopened)
    {
       rd->idxs_opened++;
       rd->indices = realloc(rd->indices, rd->idxs_opened * sizeof(RDD_INDEX *));
       rd->indices[rd->idxs_opened - 1] = ri;
    }
   rd->ords_opened = 0;
   for (i = 0; i < rd->idxs_opened; i++)
    {
       for (j = 0; j < rd->indices[i]->norders; j++)
	{
	   rd->orders = realloc(rd->orders, (rd->ords_opened + 1) * sizeof(RDD_ORDER *));
	   rd->orders[rd->ords_opened] = rd->indices[i]->orders[j];
	   rd->orders[rd->ords_opened]->orderno = rd->ords_opened;
	   if (rd->orders[rd->ords_opened] == ro)
	      rd->curord = rd->ords_opened;
	   rd->ords_opened++;
	}
    }
   if (rd->ords_opened == 1)
      if ((er = ri->vtbl->gotop(ClipMachineMemory, rd, ro, __PROC__)))
	 goto err;
   return 0;
 err_open:
   er = rdd_err(ClipMachineMemory, EG_OPEN, errno, __FILE__, __LINE__, __PROC__, ri->path);
   goto err;
 err_create:
   er = rdd_err(ClipMachineMemory, EG_CREATE, errno, __FILE__, __LINE__, __PROC__, ri->path);
   goto err;
 err_close:
   er = rdd_err(ClipMachineMemory, EG_CLOSE, errno, __FILE__, __LINE__, __PROC__, rd->indices[i]->path);
 err:
   if (!alreadyopened)
    {
       if (ri && ri->name)
	  free(ri->name);
       if (ri && ri->path)
	  free(ri->path);
#ifdef HAVE_MMAN_H
       if (ri && ri->file.md != (char *) -1)
	  munmap(ri->file.md, ri->file.mapsize);
#endif
       if (ri && ri->file.fd != -1)
	  _clip_close(ClipMachineMemory, ri->file.filehash, ri->file.fd);
       if (ri)
	  free(ri);
    }
   return er;
}

int
rdd_reindex(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   int       i, er;

   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;
   if (rd->shared)
      return rdd_err(ClipMachineMemory, EG_SHARED, 0, __FILE__, __LINE__, __PROC__, er_notpermitted);
   if (rd->readonly)
      return rdd_err(ClipMachineMemory, EG_READONLY, 0, __FILE__, __LINE__, __PROC__, er_notpermitted);

   for (i = 0; i < rd->idxs_opened; i++)
    {
       RDD_INDEX *ri = rd->indices[i];

       rd->indexing = ri->name;
       rd->reindexing = 1;
       if ((er = ri->vtbl->reindex(ClipMachineMemory, rd, ri, __PROC__)))
	  return er;
       rd->indexing = NULL;
       rd->reindexing = 0;
    }
   rd->newrec = 0;
   return rdd_gotop(ClipMachineMemory, rd, __PROC__);
}

int
rdd_orddestroy(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   int       i, er;

   if ((er = ro->vtbl->destroy(ClipMachineMemory, rd, ro, __PROC__)))
      return er;

   for (i = 0; i < rd->ords_opened; i++)
    {
       if (rd->orders[i] == ro)
	{
	   for (i++; i < rd->ords_opened; i++)
	    {
	       rd->orders[i - 1] = rd->orders[i];
	       rd->orders[i - 1]->orderno = i - 1;
	    }
	   rd->ords_opened--;
	   rd->orders = (RDD_ORDER **) realloc(rd->orders, rd->ords_opened * sizeof(RDD_ORDER *));
	   break;
	}
    }

   for (i = 0; i < ro->index->norders; i++)
    {
       if (ro->index->orders[i] == ro)
	{
	   for (i++; i < ro->index->norders; i++)
	    {
	       ro->index->orders[i - 1] = ro->index->orders[i];
	    }
	   ro->index->norders--;
	   ro->index->orders = (RDD_ORDER **) realloc(ro->index->orders, ro->index->norders * sizeof(RDD_ORDER *));
	   break;
	}
    }

   destroy_rdd_order(ClipMachineMemory, ro);
   rd->curord = -1;
   return 0;
}

int
rdd_setorder(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int order, const char *__PROC__)
{
   int       er;

   if (order < 0 || order > rd->ords_opened || order - 1 == rd->curord)
      return 0;
   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;
   rd->curord = order - 1;
   if (rd->curord != -1)
      rd->orders[rd->curord]->valid_stack = 0;
   if ((er = _rdd_calcfiltlist(ClipMachineMemory, rd, rd->filter, __PROC__)))
      return er;
   return 0;
}

int
rdd_closearea(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   int       i, k, er;

#ifdef HAVE_MMAN_H
   int       fd;

   char      upd;
#endif

   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;
#ifdef HAVE_MMAN_H
   if ((rd->file.md) != (caddr_t) - 1)
    {
       if (msync(rd->file.md, rd->file.mapsize, MS_SYNC | MS_INVALIDATE))
	  goto err;
       if (munmap(rd->file.md, rd->file.mapsize) == -1)
	  goto err;
       rd->file.md = (caddr_t) - 1;
    }
   if ((rd->memo) && ((rd->memo->file.md) != (caddr_t) - 1))
    {
       if (msync(rd->memo->file.md, rd->memo->file.mapsize, MS_SYNC | MS_INVALIDATE))
	  goto err;
       if (munmap(rd->memo->file.md, rd->memo->file.mapsize) == -1)
	  goto err;
       if (rd->memo->updated)
	{
	   fd = rd->memo->file.fd;
	   if (lseek(fd, 0, SEEK_SET))
	      goto err;
	   if (read(fd, &upd, 1) != 1)
	      goto err;
	   if (lseek(fd, 0, SEEK_SET))
	      goto err;
	   if (write(fd, &upd, 1) != 1)
	      goto err;
	}
    }
   for (i = 0; i < rd->idxs_opened; i++)
    {
       if ((rd->indices[i]->file.md) != (caddr_t) - 1)
	{
	   if (msync(rd->indices[i]->file.md, rd->indices[i]->file.mapsize, MS_SYNC | MS_INVALIDATE))
	      goto err;
	   if (munmap(rd->indices[i]->file.md, rd->indices[i]->file.mapsize) == -1)
	      goto err;
	   if (rd->indices[i]->updated)
	    {
	       fd = rd->indices[i]->file.fd;
	       if (lseek(fd, 0, SEEK_SET))
		  goto err;
	       if (read(fd, &upd, 1) != 1)
		  goto err;
	       if (lseek(fd, 0, SEEK_SET))
		  goto err;
	       if (write(fd, &upd, 1) != 1)
		  goto err;
	    }
	}
    }
#endif
   if (rd->memo)
    {
       if (_clip_close(ClipMachineMemory, rd->memo->file.filehash, rd->memo->file.fd) == -1)
	  goto err;
       if (rd->tempo)
	{
	   if (remove(rd->memo->path) == -1)
	      goto err;
	}
    }
   for (i = 0; i < rd->idxs_opened; i++)
    {
       if (_clip_close(ClipMachineMemory, rd->indices[i]->file.filehash, rd->indices[i]->file.fd) == -1)
	  goto err;
       if (rd->tempo)
	{
	   if (remove(rd->indices[i]->path) == -1)
	      goto err;
	}
    }
   if (rd->filter)
    {
       if (rd->filter->active)
	{
	   if ((er = rdd_destroyfilter(ClipMachineMemory, rd->filter, __PROC__)))
	      return er;
	}
    }
   for (k = 0; k < rd->rels_opened; k++)
    {
       RDD_DATA *rel = rd->relations[k]->child;

       rel->pending_child_parent = NULL;
       for (i = 0; i < rel->pars_opened; i++)
	{
	   if (rel->parents[i]->parent == rd)
	    {
	       if (rel->parents[i]->expr)
		  free(rel->parents[i]->expr);
	       _clip_destroy(ClipMachineMemory, &rel->parents[i]->block);
	       free(rel->parents[i]);
	       memmove(rel->parents + i, rel->parents + i + 1, (rel->pars_opened - i - 1) * sizeof(RDD_RELATION *));
	       rel->pars_opened--;
	       rel->parents = realloc(rel->parents, rel->pars_opened * sizeof(void *));
	       i--;
	    }
	}
    }
   if (rd->relations)
      free(rd->relations);

   for (k = 0; k < rd->pars_opened; k++)
    {
       RDD_DATA *par = rd->parents[k]->parent;

       for (i = 0; i < par->rels_opened; i++)
	{
	   if (par->relations[i]->child == rd)
	    {
	       if (par->relations[i]->expr)
		  free(par->relations[i]->expr);
	       _clip_destroy(ClipMachineMemory, &par->relations[i]->block);
	       free(par->relations[i]);
	       memmove(par->relations + i, par->relations + i + 1, (par->rels_opened - i - 1) * sizeof(RDD_RELATION *));
	       par->rels_opened--;
	       par->relations = realloc(par->relations, par->rels_opened * sizeof(void *));
	       i--;
	    }
	}
    }
   if (rd->parents)
      free(rd->parents);

   _clip_destroy(ClipMachineMemory, &rd->os.bForCondition);
   _clip_destroy(ClipMachineMemory, &rd->os.bWhileCondition);
   _clip_destroy(ClipMachineMemory, &rd->os.bEval);

   if ((er = rd->vtbl->close(ClipMachineMemory, rd, __PROC__)))
      return er;
   if ((er = rdd_ulock(ClipMachineMemory, rd, 0, 1, __PROC__)))
      return er;
   if ((er = rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__)))
      return er;
   if (_clip_close(ClipMachineMemory, rd->file.filehash, rd->file.fd) == -1)
      goto err;
   if (rd->tempo)
    {
       if (remove(rd->path) == -1)
	  goto err;
    }
   return 0;
 err:
   return rdd_err(ClipMachineMemory, EG_CLOSE, errno, __FILE__, __LINE__, __PROC__, rd->path);
}

int
rdd_gotop(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   RDD_ORDER *ro;

   int       er;

   rd->pending_child_parent = NULL;
   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;
   if (rd->curord == -1)
    {
       if ((er = rd->vtbl->gotop(ClipMachineMemory, rd, __PROC__)))
	  return er;
    }
   else
    {
       ro = rd->orders[rd->curord];
       if ((er = ro->vtbl->_rlock(ClipMachineMemory, ro, __PROC__)))
	  return er;
       if ((er = ro->vtbl->gotop(ClipMachineMemory, rd, ro, __PROC__)))
	  goto unlock;
       if ((er = ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__)))
	  return er;
    }
   return 0;
 unlock:
   ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__);
   return er;
}

int
rdd_gobottom(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   RDD_ORDER *ro;

   int       er;

   rd->pending_child_parent = NULL;
   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;
   if (rd->curord == -1)
    {
       if ((er = rd->vtbl->gobottom(ClipMachineMemory, rd, __PROC__)))
	  return er;
    }
   else
    {
       ro = rd->orders[rd->curord];
       if ((er = ro->vtbl->_rlock(ClipMachineMemory, ro, __PROC__)))
	  return er;
       if ((er = ro->vtbl->gobottom(ClipMachineMemory, rd, ro, __PROC__)))
	  goto unlock;
       if ((er = ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__)))
	  return er;
    }
   return 0;
 unlock:
   ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__);
   return er;
}

int
rdd_skip(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int recs, const char *__PROC__)
{
   RDD_ORDER *ro;

   int       i, er;

   if (rd->pending_child_parent)
      if ((er = rdd_child_duty(ClipMachineMemory, rd, __PROC__)))
	 return er;
   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;
   if ((er = rdd_childs(ClipMachineMemory, rd, __PROC__)))
      return er;
   if (rd->curord == -1)
    {
       if (recs > 0)
	{
	   for (i = 0; i < recs; i++)
	    {
	       if ((er = rd->vtbl->next(ClipMachineMemory, rd, 0, __PROC__)))
		  return er;
	    }
	}
       else
	{
	   for (i = 0; i > recs; i--)
	    {
	       if ((er = rd->vtbl->prev(ClipMachineMemory, rd, __PROC__)))
		  return er;
	    }
	}
    }
   else
    {
       ro = rd->orders[rd->curord];
       if ((er = ro->vtbl->_rlock(ClipMachineMemory, ro, __PROC__)))
	  return er;
       if (recs > 0)
	{
	   for (i = 0; i < recs; i++)
	    {
	       if ((er = ro->vtbl->next(ClipMachineMemory, rd, ro, __PROC__)))
		  goto unlock;
	    }
	}
       else
	{
	   for (i = 0; i > recs; i--)
	    {
	       if ((er = ro->vtbl->prev(ClipMachineMemory, rd, ro, __PROC__)))
		  goto unlock;
	    }
	}
       if ((er = ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__)))
	  return er;
    }
   if (recs == 0)
    {
       rd->valid = 0;
    }
   return 0;
 unlock:
   ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__);
   return er;
}

int
rdd_seek(ClipMachine * ClipMachineMemory, RDD_DATA * rd, ClipVar * v, int soft, int last, int *found, const char *__PROC__)
{
   RDD_ORDER *ro;

   int       er;

   if ((rd->ords_opened < 1) || (rd->curord == -1))
      return rdd_err(ClipMachineMemory, EG_NOORDER, 0, __FILE__, __LINE__, __PROC__, er_noorder);

   rd->pending_child_parent = NULL;
   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;

   ro = rd->orders[rd->curord];
   if ((er = ro->vtbl->_rlock(ClipMachineMemory, ro, __PROC__)))
      return er;
   if ((er = ro->vtbl->seek(ClipMachineMemory, rd, ro, v, soft, last, found, __PROC__)))
      goto unlock;
   if ((er = ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__)))
      return er;
   return 0;
 unlock:
   ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__);
   return er;
}

int
rdd_goto(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int rec, const char *__PROC__)
{
   int       er;

   rd->pending_child_parent = NULL;
   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;

   if ((er = rd->vtbl->go(ClipMachineMemory, rd, rec, __PROC__)))
      return er;
/*	if(rd->curord != -1){
		if((er = rd->orders[rd->curord]->vtbl->buildtree(
			ClipMachineMemory,rd,rd->orders[rd->curord],__PROC__))) return er;
	}*/
   return 0;
}

int
rdd_gotokey(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, unsigned int keyno, int *ok, const char *__PROC__)
{
   int       er;

   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;
   if ((er = ro->vtbl->_rlock(ClipMachineMemory, ro, __PROC__)))
      return er;
   if ((er = ro->vtbl->gotokey(ClipMachineMemory, rd, ro, keyno, ok, __PROC__)))
      goto unlock;
   if ((er = ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__)))
      return er;
   return 0;
 unlock:
   ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__);
   return er;
}

int
rdd_bof(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *bof, const char *__PROC__)
{
   *bof = rd->bof | rd->v_bof;
   return 0;
}

int
rdd_eof(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *eof, const char *__PROC__)
{
   int       er;

   if (rd->pending_child_parent)
      if ((er = rdd_child_duty(ClipMachineMemory, rd, __PROC__)))
	 return er;
   *eof = rd->eof;
   return 0;
}

int
rdd_recno(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *recno, const char *__PROC__)
{
   int       er;

   if (rd->pending_child_parent)
      if ((er = rdd_child_duty(ClipMachineMemory, rd, __PROC__)))
	 return er;
   *recno = rd->recno;
   return 0;
}

int
rdd_lastrec(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *lastrec, const char *__PROC__)
{
   return rd->vtbl->lastrec(ClipMachineMemory, rd, lastrec, __PROC__);
}

int
rdd_keyno(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *keyno, const char *__PROC__)
{
   int       er;

   if ((rd->ords_opened < 1) || (rd->curord == -1))
      return rdd_err(ClipMachineMemory, EG_NOORDER, 0, __FILE__, __LINE__, __PROC__, er_noorder);

   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;
   if ((er = rd->orders[rd->curord]->vtbl->keyno(ClipMachineMemory, rd, rd->orders[rd->curord], keyno, __PROC__)))
      return er;
   return 0;
}

int
rdd_lastkey(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *lastkey, const char *__PROC__)
{
   int       er;

   if ((rd->ords_opened < 1) || (rd->curord == -1))
      return rdd_err(ClipMachineMemory, EG_NOORDER, 0, __FILE__, __LINE__, __PROC__, er_noorder);

   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;
   if ((er = rd->orders[rd->curord]->vtbl->lastkey(ClipMachineMemory, rd, rd->orders[rd->curord], lastkey, __PROC__)))
      return er;
   return 0;
}

int
rdd_fieldname(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int fno, const char *__PROC__)
{
   if (fno < 0 || fno >= rd->nfields)
      _clip_retc(ClipMachineMemory, "");

   _clip_retc(ClipMachineMemory, rd->fields[fno].name);
   return 0;
}

int
rdd_append(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *neterr, const char *__PROC__)
{
   int       r, er;

   int       lastrec;

   rd->pending_child_parent = NULL;
   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;
   if ((er = rdd_event(ClipMachineMemory, EVENT_APPEND, rd->area, 0, NULL, &r, __PROC__)))
      return er;
   if (!r)
      return 0;
   if ((er = rdd_lastrec(ClipMachineMemory, rd, &lastrec, __PROC__)))
      return er;
   if ((er = rd->vtbl->rlock(ClipMachineMemory, rd, lastrec + 1, &r, __PROC__)))
      return er;
   *neterr = 0;
   if (!r)
    {				// neterr()
       *neterr = 1;
       return 0;
    }
   if ((er = rd->vtbl->append(ClipMachineMemory, rd, __PROC__)))
      return er;
   if (rd->filter && rd->filter->rmap)
    {
       unsigned int bytes = ((lastrec + 1) >> 5) + 1;

       int       fok;

       if (((lastrec) >> 5) + 1 < bytes)
	{
	   rd->filter->rmap = realloc(rd->filter->rmap, bytes << 2);
	   rd->filter->size = lastrec;
	}
       if ((er = rdd_calcfilter(ClipMachineMemory, rd, &fok, __PROC__)))
	  return er;
       if (fok)
	{
	   _rm_setbit(rd->filter->rmap, rd->filter->size, rd->recno);
	}
       else
	{
	   _rm_clrbit(rd->filter->rmap, rd->filter->size, rd->recno);
	}
    }
   rd->newrec = 1;
   return 0;
}

int
rdd_getvaluebn(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *fname, const char *__PROC__)
{
   int       fno = _rdd_fieldno(rd, _clip_casehashword(fname, strlen(fname)));

   if (fno < 0)
      return rdd_err(ClipMachineMemory, EG_NOVAR, 0, __FILE__, __LINE__, __PROC__, er_nofield);

   return rdd_getvalue(ClipMachineMemory, rd, fno, __PROC__);
}

int
rdd_getvalue(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int no, const char *__PROC__)
{
   int       eof, r, er;

   if (rd->pending_child_parent)
      if ((er = rdd_child_duty(ClipMachineMemory, rd, __PROC__)))
	 return er;

   if ((er = rdd_eof(ClipMachineMemory, rd, &eof, __PROC__)))
      return er;
   if (eof)
    {
       char     *str;

       switch (rd->fields[no].type)
	{
	case 'C':
	   str = (char *) malloc(rd->fields[no].len + 1);
	   memset(str, 32, rd->fields[no].len);
	   str[rd->fields[no].len] = 0;
	   _clip_retcn_m(ClipMachineMemory, str, rd->fields[no].len);
	   break;
	case 'M':
	   _clip_retcn(ClipMachineMemory, "", 0);
	   break;
	case 'N':
	   _clip_retndp(ClipMachineMemory, 0, rd->fields[no].len, rd->fields[no].dec);
	   break;
	case 'D':
	   _clip_retdj(ClipMachineMemory, 0);
	   break;
	case 'L':
	   _clip_retl(ClipMachineMemory, 0);
	   break;
	}
    }
   else
    {
       if ((er = rd->vtbl->getvalue(ClipMachineMemory, rd, no, RETPTR(ClipMachineMemory), __PROC__)))
	  return er;
    }
   if ((er = rdd_event(ClipMachineMemory, EVENT_GET, rd->area, no + 1, RETPTR(ClipMachineMemory), &r, __PROC__)))
      return er;
   return 0;
}

int
rdd_setvaluebn(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *fname, ClipVar * vp, const char *__PROC__)
{
   int       fno = _rdd_fieldno(rd, _clip_casehashword(fname, strlen(fname)));

   if (fno < 0)
      return rdd_err(ClipMachineMemory, EG_NOVAR, 0, __FILE__, __LINE__, __PROC__, er_nofield);

   return rdd_setvalue(ClipMachineMemory, rd, fno, vp, __PROC__);
}

int
rdd_setvalue(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int no, ClipVar * vp, const char *__PROC__)
{
   ClipVar  *v = _clip_vptr(vp);

   RDD_ORDER *ro;

   int       eof, i, r, er;

   unsigned char *recdup = NULL, *recbuf = NULL;

   if (rd->pending_child_parent)
      if ((er = rdd_child_duty(ClipMachineMemory, rd, __PROC__)))
	 return er;

   if ((er = rdd_event(ClipMachineMemory, EVENT_PUT, rd->area, no + 1, v, &r, __PROC__)))
      return er;
   if (!r)
      return 0;

   if ((er = rdd_eof(ClipMachineMemory, rd, &eof, __PROC__)))
      return er;
   if (eof)
    {
       return 0;
    }

   if ((er = rd->vtbl->rlocked(ClipMachineMemory, rd, rd->recno, &r, __PROC__)))
      return er;
   if (!r && !rd->flocked)
      return rdd_err(ClipMachineMemory, EG_UNLOCKED, 0, __FILE__, __LINE__, __PROC__, er_notpermitted);

   if (!rd->valid)
    {
       if ((er = rd->vtbl->getrecord(ClipMachineMemory, rd, __PROC__)))
	  return er;
    }
   recdup = malloc(rd->recsize);
   memcpy(recdup, rd->record, rd->recsize);
   recbuf = malloc(rd->recsize);

   if ((er = rd->vtbl->setvalue(ClipMachineMemory, rd, no, v, 0, __PROC__)))
      goto unlock;

   memcpy(recbuf, recdup, rd->recsize);
   memcpy(recdup, rd->record, rd->recsize);
   memcpy(rd->record, recbuf, rd->recsize);

   if (!rd->newrec)
    {
       for (i = 0; i < rd->ords_opened; i++)
	{
	   ro = rd->orders[i];
	   if (!ro->custom)
	    {
	       if ((ro->bforexpr.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType)
		   && ro->simpexpr && (ro->simpfno != no))
		  continue;
	       if ((er = rdd_event(ClipMachineMemory, EVENT_UPDATE, rd->area, 0, NULL, NULL, __PROC__)))
		  goto unlock;
	       if ((er = ro->vtbl->_wlock(ClipMachineMemory, ro, __PROC__)))
		  goto unlock;
	       if ((er = ro->vtbl->delkey(ClipMachineMemory, rd, ro, __PROC__)))
		  goto unlock;
	       ro->index->updated = 1;
	    }
	}
       rd->eof = 0;
    }

   memcpy(recbuf, recdup, rd->recsize);
   memcpy(recdup, rd->record, rd->recsize);
   memcpy(rd->record, recbuf, rd->recsize);
/*	if((er = rdd_childs(ClipMachineMemory,rd,__PROC__))) return er; */
   if (!rd->newrec)
    {
       for (i = 0; i < rd->ords_opened; i++)
	{
	   RDD_ORDER *ro = rd->orders[i];

	   ClipVar   vv, *vp;

	   if (ro->bforexpr.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
	    {
	       if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->bforexpr, &vv, 0)))
		  goto unlock;
	       vp = _clip_vptr(&vv);
	       if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
		{
		   er = rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_badforexpr);
		   goto unlock;
		}
	       if (!vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
		{
		   _clip_destroy(ClipMachineMemory, &vv);
		   if ((er = ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__)))
		      goto unlock;
		   continue;
		}
	       _clip_destroy(ClipMachineMemory, &vv);
	    }
	   if (!ro->custom)
	    {
	       if ((ro->bforexpr.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType)
		   && ro->simpexpr && (ro->simpfno != no))
		  continue;
	       if ((er = rdd_event(ClipMachineMemory, EVENT_UPDATE, rd->area, 0, NULL, NULL, __PROC__)))
		  goto unlock;
	       if (ro->simpexpr && ro->simpfno == no)
		{
		   if ((er = ro->vtbl->addkey(ClipMachineMemory, rd, ro, v, __PROC__)))
		      goto unlock;
		}
	       else
		{
		   if ((er = ro->vtbl->addkey(ClipMachineMemory, rd, ro, NULL, __PROC__)))
		      goto unlock;
		}
	       if ((er = ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__)))
		  goto unlock;
	    }
	}
    }
   if (rd->filter && rd->filter->rmap)
    {
       int       fok;

       if ((er = rdd_calcfilter(ClipMachineMemory, rd, &fok, __PROC__)))
	  return er;
       if (fok)
	  _rm_setbit(rd->filter->rmap, rd->filter->size, rd->recno);
       else
	  _rm_clrbit(rd->filter->rmap, rd->filter->size, rd->recno);
    }
   if (recdup)
      free(recdup);
   if (recbuf)
      free(recbuf);
   return 0;
 unlock:
   if (!rd->newrec)
    {
       for (i = 0; i < rd->ords_opened; i++)
	{
	   ro = rd->orders[i];
	   if (!ro->custom)
	    {
	       if (ro->simpexpr && (ro->simpfno != no))
		  continue;
	       ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__);
	    }
	}
    }
   return er;
}

int
_rdd_calcfiltlist(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_FILTER * fp, const char *__PROC__)
{
   int       er;

   if (fp && fp->optimize == 2 && ClipMachineMemory->optimizelevel > 0)
    {
       if (rd->curord == -1)
	{
	   if ((er = rd->vtbl->calcfiltlist(ClipMachineMemory, rd, fp, __PROC__)))
	      return er;
	}
       else
	{
	   RDD_ORDER *ro = rd->orders[rd->curord];

	   if (ro->vtbl->calcfiltlist)
	    {
	       if ((er = ro->vtbl->calcfiltlist(ClipMachineMemory, rd, ro, fp, __PROC__)))
		  return er;
	    }
	}
       if ((er = rdd_gotop(ClipMachineMemory, rd, __PROC__)))
	  return er;
    }
   return 0;
}

int
rdd_initrushmore(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_FILTER * fp, ClipVar * a, int test, const char *__PROC__)
{
   unsigned int lastrec;

   unsigned int bytes;

   int       optimize = 0;

   char     *str = strdup(fp->sfilter);

   int       er = EG_UNSUPPORTED;

   RDD_PSEUDO *pseudo = NULL;

   int       npseudo = 0;

   if (a)
    {
       ClipVar  *e, *alias, *name;

       long      i[1], j[1];

       npseudo = a->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
       pseudo = calloc(npseudo, sizeof(RDD_PSEUDO));
       for (i[0] = 0; i[0] < npseudo; i[0]++)
	{
	   e = _clip_vptr(_clip_aref(ClipMachineMemory, a, 1, i));
	   if (e->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType
	       && e->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar == 2)
	    {
	       j[0] = 0;
	       alias = _clip_vptr(_clip_aref(ClipMachineMemory, e, 1, j));
	       j[0] = 1;
	       name = _clip_vptr(_clip_aref(ClipMachineMemory, e, 1, j));
	       if (alias->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType
		   || name->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType)
		  goto err_pseudo;
	       pseudo[i[0]].alias = alias->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
	       pseudo[i[0]].name = name->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
	    }
	   else
	      goto err_pseudo;
	}
    }

   if ((er = rd->vtbl->lastrec(ClipMachineMemory, rd, (int *) (&lastrec), __PROC__)))
      goto err;

   bytes = (lastrec >> 5) + 1;

   if ((er = rm_init(ClipMachineMemory, rd, str, __PROC__)))
      goto err;
   if (fp->rmap)
      free(fp->rmap);
   fp->size = lastrec;

   rd->rmflen = 0;
   rd->rmfilter = malloc(1);
   if (!(fp->rmap = rm_expr(ClipMachineMemory, rd, fp, bytes, &optimize, npseudo, pseudo, test, __PROC__)))
    {
       er = 1;
       goto err;
    }
   free(fp->sfilter);
   fp->sfilter = rd->rmfilter;
   rd->rmfilter = NULL;

   if (test)
      optimize = 0;
   fp->optimize = optimize;
   if (fp->optimize == 0)
    {
       free(fp->rmap);
       fp->rmap = NULL;
    }
   if (fp->optimize == 2)
    {
       if ((er = _rdd_calcfiltlist(ClipMachineMemory, rd, fp, __PROC__)))
	  goto err;
    }
   free(str);
   if (pseudo)
      free(pseudo);
   if (test && fp->rmap)
    {
       free(fp->rmap);
       fp->rmap = NULL;
    }
   return 0;
 err_pseudo:
   free(pseudo);
   er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, "Bad argument");
 err:
   free(str);
   return er;
}

int
rdd_createuserfilter(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_FILTER ** fpp, unsigned int size, const char *__PROC__)
{
   RDD_FILTER *fp = calloc(sizeof(RDD_FILTER), 1);

   int       bytes;

   int       er;

   fp->rd = rd;
   if (size)
    {
       bytes = ((size + 1) >> 5) + 1;
       fp->size = size;
    }
   else
    {
       int       lastrec;

       if ((er = rdd_lastrec(ClipMachineMemory, rd, &lastrec, __PROC__)))
	  goto err;
       bytes = ((lastrec + 1) >> 5) + 1;
       fp->size = lastrec;
    }
   fp->rmap = calloc(sizeof(unsigned int), bytes);
   fp->handle = _clip_store_c_item(ClipMachineMemory, fp, _C_ITEM_TYPE_RYO, destroy_ryo);
   fp->custom = 1;
   *fpp = fp;
   return 0;
 err:
   free(fp);
   return er;
}

int
rdd_createfilter(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_FILTER ** fpp,
		 ClipVar * _block, const char *str, ClipVar * remap, int lNoOptimize, const char *__PROC__)
{
   char      expr[PATH_MAX];

   RDD_FILTER *fp = calloc(sizeof(RDD_FILTER), 1);

   int       er;

   *fpp = NULL;
   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;
   fp->fps = calloc(1, sizeof(RDD_FPS));
   fp->rd = rd;
   if (str)
    {
       fp->sfilter = strdup(str);
    }
   if ((ClipMachineMemory->flags1 & OPTIMIZE_FLAG) && !lNoOptimize && str)
    {
       er = rdd_initrushmore(ClipMachineMemory, rd, fp, remap, 0, __PROC__);
       if (fp->rmap)
	  _clip_destroy(ClipMachineMemory, _block);
    }
   else if (str && remap && remap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
    {
       er = rdd_initrushmore(ClipMachineMemory, rd, fp, remap, 1, __PROC__);
       if (fp->rmap)
	  _clip_destroy(ClipMachineMemory, _block);
    }
   fp->nfps = 1;
   if ((!_block || _block->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType) && str)
    {
       rdd_expandmacro(rd->area, _clip_parni(ClipMachineMemory, 1), fp->sfilter, expr);
       if ((er = _clip_eval_macro(ClipMachineMemory, expr, strlen(expr), &fp->fps->bfilter)))
	  goto err;
    }
   else
    {
       _clip_clone(ClipMachineMemory, &fp->fps->bfilter, _block);
    }
   fp->handle = _clip_store_c_item(ClipMachineMemory, fp, _C_ITEM_TYPE_RYO, destroy_ryo);
   *fpp = fp;
   return 0;
 err:
   free(fp);
   return er;
}

int
rdd_setrelation(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_DATA * child,
		ClipVar * block, const char *expr, int scoped, const char *__PROC__)
{
   RDD_RELATION *rel = (RDD_RELATION *) calloc(1, sizeof(RDD_RELATION));

   int       r = _clip_parni(ClipMachineMemory, 1);

   int       er;

   rel->scoped = scoped;
   rel->child = child;
   rel->parent = rd;
   if (!expr)
      expr = "";
   rel->expr = (char *) malloc(strlen(expr) + 1);
   strcpy(rel->expr, expr);
   if (block)
    {
       _clip_clone(ClipMachineMemory, &rel->block, block);
    }
   else
    {
       if ((rel->simpfno = _rdd_fieldno(rd, _clip_casehashword(expr, strlen(expr)))) != -1)
	  rel->simpexpr = 1;
       if (!rel->simpexpr)
	{
	   char      expexp[PATH_MAX];

	   memset(expexp, 0, sizeof(expexp));
	   rdd_expandmacro(rd->area, r, expr, expexp);
	   if ((er = _clip_eval_macro(ClipMachineMemory, expexp, strlen(expexp), &rel->block)))
	      goto err;
	}
    }
   rd->rels_opened++;
   rd->relations = (RDD_RELATION **) realloc(rd->relations, rd->rels_opened * sizeof(RDD_RELATION *));
   rd->relations[rd->rels_opened - 1] = rel;

   child->pars_opened++;
   child->parents = (RDD_RELATION **) realloc(child->parents, child->pars_opened * sizeof(RDD_RELATION *));
   child->parents[child->pars_opened - 1] = rel;
   if ((er = rdd_childs(ClipMachineMemory, rd, __PROC__)))
      return er;
   return 0;
 err:
   free(rel->expr);
   free(rel);
   return er;
}

int
rdd_delete(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   int       i, r, er;

   RDD_ORDER *ro;

   if (rd->pending_child_parent)
      if ((er = rdd_child_duty(ClipMachineMemory, rd, __PROC__)))
	 return er;

   if ((er = rdd_event(ClipMachineMemory, EVENT_DELETE, rd->area, 0, NULL, &r, __PROC__)))
      return er;
   if (!r)
      return 0;
   if (rd->eof)
      return 0;

   if ((er = rd->vtbl->rlocked(ClipMachineMemory, rd, rd->recno, &r, __PROC__)))
      return er;
   if (!r && !rd->flocked)
      return rdd_err(ClipMachineMemory, EG_UNLOCKED, 0, __FILE__, __LINE__, __PROC__, er_notpermitted);

   if (!rd->newrec)
    {
       for (i = 0; i < rd->ords_opened; i++)
	{
	   ro = rd->orders[i];
	   if (!ro->custom)
	    {
	       if ((ro->bforexpr.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType) && ro->simpexpr)
		  if ((er = ro->vtbl->_wlock(ClipMachineMemory, ro, __PROC__)))
		     goto unlock;
	       if ((er = ro->vtbl->delkey(ClipMachineMemory, rd, ro, __PROC__)))
		  goto unlock;
	       ro->index->updated = 1;
	    }
	}
       rd->eof = 0;
    }
   if ((er = rd->vtbl->delete(ClipMachineMemory, rd, __PROC__)))
      return er;
   if (!rd->newrec)
    {
       for (i = 0; i < rd->ords_opened; i++)
	{
	   RDD_ORDER *ro = rd->orders[i];

	   ClipVar   vv, *vp;

	   if (ro->bforexpr.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
	    {
	       if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->bforexpr, &vv, 0)))
		  goto unlock;
	       vp = _clip_vptr(&vv);
	       if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
		{
		   er = rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_badforexpr);
		   goto unlock;
		}
	       if (!vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
		{
		   _clip_destroy(ClipMachineMemory, &vv);
		   if ((er = ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__)))
		      goto unlock;
		   continue;
		}
	       _clip_destroy(ClipMachineMemory, &vv);
	    }
	   if (!ro->custom)
	    {
	       if ((er = ro->vtbl->addkey(ClipMachineMemory, rd, ro, NULL, __PROC__)))
		  goto unlock;
	       if ((er = ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__)))
		  goto unlock;
	    }
	}
    }
   if (rd->filter && rd->filter->rmap)
    {
       int       fok;

       if ((er = rdd_calcfilter(ClipMachineMemory, rd, &fok, __PROC__)))
	  return er;
       if (fok)
	  _rm_setbit(rd->filter->rmap, rd->filter->size, rd->recno);
       else
	  _rm_clrbit(rd->filter->rmap, rd->filter->size, rd->recno);
    }
   return 0;
 unlock:
   if (!rd->newrec)
    {
       for (i = 0; i < rd->ords_opened; i++)
	{
	   ro = rd->orders[i];
	   if (!ro->custom)
	    {
	       ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__);
	    }
	}
    }
   return er;
}

int
rdd_recall(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   int       i, r, er;

   RDD_ORDER *ro;

   if (rd->pending_child_parent)
      if ((er = rdd_child_duty(ClipMachineMemory, rd, __PROC__)))
	 return er;

   if ((er = rdd_event(ClipMachineMemory, EVENT_RECALL, rd->area, 0, NULL, &r, __PROC__)))
      return er;
   if (!r)
      return 0;
   if (rd->eof)
      return 0;

   if ((er = rd->vtbl->rlocked(ClipMachineMemory, rd, rd->recno, &r, __PROC__)))
      return er;
   if (!r && !rd->flocked)
      return rdd_err(ClipMachineMemory, EG_UNLOCKED, 0, __FILE__, __LINE__, __PROC__, er_notpermitted);

   if (!rd->newrec)
    {
       for (i = 0; i < rd->ords_opened; i++)
	{
	   ro = rd->orders[i];
	   if (!ro->custom)
	    {
	       if ((ro->bforexpr.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType) && ro->simpexpr)
		  if ((er = ro->vtbl->_wlock(ClipMachineMemory, ro, __PROC__)))
		     goto unlock;
	       if ((er = ro->vtbl->delkey(ClipMachineMemory, rd, ro, __PROC__)))
		  goto unlock;
	       ro->index->updated = 1;
	    }
	}
       rd->eof = 0;
    }
   if ((er = rd->vtbl->recall(ClipMachineMemory, rd, __PROC__)))
      return er;
   if (!rd->newrec)
    {
       for (i = 0; i < rd->ords_opened; i++)
	{
	   RDD_ORDER *ro = rd->orders[i];

	   ClipVar   vv, *vp;

	   if (ro->bforexpr.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
	    {
	       if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->bforexpr, &vv, 0)))
		  goto unlock;
	       vp = _clip_vptr(&vv);
	       if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
		{
		   er = rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_badforexpr);
		   goto unlock;
		}
	       if (!vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
		{
		   _clip_destroy(ClipMachineMemory, &vv);
		   if ((er = ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__)))
		      goto unlock;
		   continue;
		}
	       _clip_destroy(ClipMachineMemory, &vv);
	    }
	   if (!ro->custom)
	    {
	       if ((er = ro->vtbl->addkey(ClipMachineMemory, rd, ro, NULL, __PROC__)))
		  goto unlock;
	       if ((er = ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__)))
		  goto unlock;
	    }
	}
    }
   if (rd->filter && rd->filter->rmap)
    {
       int       fok;

       if ((er = rdd_calcfilter(ClipMachineMemory, rd, &fok, __PROC__)))
	  return er;
       if (fok)
	  _rm_setbit(rd->filter->rmap, rd->filter->size, rd->recno);
       else
	  _rm_clrbit(rd->filter->rmap, rd->filter->size, rd->recno);
    }
   return 0;
 unlock:
   if (!rd->newrec)
    {
       for (i = 0; i < rd->ords_opened; i++)
	{
	   ro = rd->orders[i];
	   if (!ro->custom)
	    {
	       ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__);
	    }
	}
    }
   return er;
}

int
rdd_deleted(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *deleted, const char *__PROC__)
{
   int       eof, er;

   if ((er = rdd_eof(ClipMachineMemory, rd, &eof, __PROC__)))
      return er;
   if (eof)
      *deleted = 0;
   else if ((er = rd->vtbl->deleted(ClipMachineMemory, rd, deleted, __PROC__)))
      return er;
   return 0;
}

int
rdd_clearindex(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   int       i, er;

   int       structural = -1;

   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;
   for (i = 0; i < rd->idxs_opened; i++)
    {
       if (rd->indices[i]->structural)
	{
	   structural = i;
	}
       else
	{
#ifdef HAVE_MMAN_H
	   if ((rd->indices[i]->file.md) != (caddr_t) - 1)
	    {
	       if (munmap(rd->indices[i]->file.md, rd->indices[i]->file.mapsize) == -1)
		  return rdd_err(ClipMachineMemory, EG_CLOSE, errno, __FILE__, __LINE__, __PROC__, rd->indices[i]->path);
	    }
#endif
	   if (_clip_close(ClipMachineMemory, rd->indices[i]->file.filehash, rd->indices[i]->file.fd) == -1)
	      return rdd_err(ClipMachineMemory, EG_CLOSE, errno, __FILE__, __LINE__, __PROC__, rd->indices[i]->path);
	   if ((er = rd->indices[i]->vtbl->close(ClipMachineMemory, rd, rd->indices[i], __PROC__)))
	      return er;
	}
    }
   if (structural == -1)
    {
       if (rd->indices)
	  free(rd->indices);
       rd->indices = NULL;
       if (rd->orders)
	  free(rd->orders);
       rd->orders = NULL;
       rd->idxs_opened = 0;
       rd->ords_opened = 0;
       rd->newrec = 0;
    }
   else
    {
       rd->indices[0] = rd->indices[structural];
       rd->indices = (RDD_INDEX **) realloc(rd->indices, sizeof(RDD_INDEX *));
       rd->idxs_opened = 1;
       rd->ords_opened = rd->indices[0]->norders;
       rd->orders = (RDD_ORDER **) realloc(rd->orders, rd->ords_opened * sizeof(RDD_ORDER *));
       for (i = 0; i < rd->ords_opened; i++)
	{
	   rd->orders[i] = rd->indices[0]->orders[i];
	   rd->orders[i]->orderno = i;
	}
    }
   rd->curord = -1;
   return 0;
}

int
rdd_destroyfilter(ClipMachine * ClipMachineMemory, RDD_FILTER * fp, const char *__PROC__)
{
   int       i;

   for (i = 0; i < fp->nfps; i++)
      _clip_destroy(ClipMachineMemory, &fp->fps[i].bfilter);
   _clip_destroy_c_item(ClipMachineMemory, fp->handle, _C_ITEM_TYPE_RYO);
   return 0;
}

int
rdd_clearfilter(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   int       er;

   if (rd->filter)
    {
       if (rd->filter->active)
	{
	   if ((er = rdd_destroyfilter(ClipMachineMemory, rd->filter, __PROC__)))
	      return er;
	}
       rd->filter = NULL;
    }
   return 0;
}

static int
_rdd_clearparent(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_DATA * parent)
{
   int       i;

   for (i = 0; i < rd->pars_opened; i++)
    {
       if (rd->parents[i]->parent == parent)
	{
	   memcpy(rd->parents + i, rd->parents + i + 1, (rd->pars_opened - 1) * sizeof(void *));
	   rd->pars_opened--;
	   rd->parents = realloc(rd->parents, rd->pars_opened * sizeof(void *));
	}
    }
   return 0;
}

int
rdd_clearrelation(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   int       i;

   for (i = 0; i < rd->rels_opened; i++)
    {
       rd->relations[i]->child->pending_child_parent = NULL;
       _rdd_clearparent(ClipMachineMemory, rd->relations[i]->child, rd);
       if (rd->relations[i]->expr)
	  free(rd->relations[i]->expr);
       _clip_destroy(ClipMachineMemory, &rd->relations[i]->block);
       free(rd->relations[i]);
    }
   free(rd->relations);
   rd->relations = NULL;
   rd->rels_opened = 0;
   return 0;
}

int
rdd_rlock(ClipMachine * ClipMachineMemory, RDD_DATA * rd, unsigned int rec, int *r, const char *__PROC__)
{
   rd->valid = 0;
   if (rec <= 0)
      return 0;
   return rd->vtbl->rlock(ClipMachineMemory, rd, rec, r, __PROC__);
}

int
rdd_flock(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   int       r, er;

   if ((er = rd->vtbl->flock(ClipMachineMemory, rd, &r, __PROC__)))
      return er;
   _clip_retl(ClipMachineMemory, r);
   return 0;
}

int
rdd_ulock(ClipMachine * ClipMachineMemory, RDD_DATA * rd, unsigned int rec, int flock, const char *__PROC__)
{
/*
	int i;
#ifdef HAVE_MMAN_H
	if(rd->shared && (int)rd->file.md!=-1){
		if(msync(rd->file.md,rd->file.mapsize,MS_ASYNC)==-1)
			return rdd_err(ClipMachineMemory,EG_LOCK,errno,__FILE__,__LINE__,__PROC__,er_ioerror);
		if(rd->memo){
			if(msync(rd->memo->file.md,rd->memo->file.mapsize,MS_ASYNC)==-1)
				return rdd_err(ClipMachineMemory,EG_LOCK,errno,__FILE__,__LINE__,__PROC__,er_ioerror);
		}
		for(i=0;i<rd->idxs_opened;i++){
			if(msync(rd->indices[i]->file.md,rd->indices[i]->file.mapsize,MS_ASYNC)==-1)
				return rdd_err(ClipMachineMemory,EG_LOCK,errno,__FILE__,__LINE__,__PROC__,er_ioerror);
		}
	}
#endif
*/
   if (rec < 0)
      return 0;
   return rd->vtbl->ulock(ClipMachineMemory, rd, rec, flock, __PROC__);
}

int
rdd_commit(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   int       er;

#ifdef HAVE_MMAN_H
   int       i;
#endif

   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;
   if (rd->changed)
    {
       if ((er = rd->vtbl->setrecord(ClipMachineMemory, rd, __PROC__)))
	  return er;
       rd->changed = 0;
    }
#ifdef HAVE_MMAN_H
   if (rd->shared && (rd->file.md != (caddr_t) - 1))
    {
       if (msync(rd->file.md, rd->file.mapsize, MS_ASYNC) == -1)
	  return rdd_err(ClipMachineMemory, EG_WRITE, errno, __FILE__, __LINE__, __PROC__, er_ioerror);
       if (rd->memo)
	{
	   if (msync(rd->memo->file.md, rd->memo->file.mapsize, MS_ASYNC) == -1)
	      return rdd_err(ClipMachineMemory, EG_WRITE, errno, __FILE__, __LINE__, __PROC__, er_ioerror);
	}
       for (i = 0; i < rd->idxs_opened; i++)
	{
	   if (msync(rd->indices[i]->file.md, rd->indices[i]->file.mapsize, MS_ASYNC))
	      return rdd_err(ClipMachineMemory, EG_WRITE, errno, __FILE__, __LINE__, __PROC__, er_ioerror);
	}
    }
#endif
   return 0;
}

int
rdd_dbread(ClipMachine * ClipMachineMemory, RDD_DATA * rd, ClipVar * ap, const char *__PROC__)
{
   ClipVar   vv;

   int       i, er;

   CLEAR_CLIPVAR(&vv);

   _clip_map(ClipMachineMemory, ap);
   for (i = 0; i < rd->nfields; i++)
    {
       if ((er = rdd_takevalue(ClipMachineMemory, rd, i, &vv, __PROC__)))
	  return er;
       _clip_madd(ClipMachineMemory, ap, _clip_casehashword(rd->fields[i].name, strlen(rd->fields[i].name)), &vv);
       _clip_destroy(ClipMachineMemory, &vv);
    }
   return 0;
}

int
rdd_dbwrite(ClipMachine * ClipMachineMemory, RDD_DATA * rd, ClipVar * ap, const char *__PROC__)
{
   RDD_ORDER *ro;

   ClipVar  *vp;

   ClipVar   old;

   int       eof;

   int       i, j, r, er;

   int      *fs = NULL;

   if (rd->pending_child_parent)
      if ((er = rdd_child_duty(ClipMachineMemory, rd, __PROC__)))
	 return er;

   if ((er = rd->vtbl->rlocked(ClipMachineMemory, rd, rd->recno, &r, __PROC__)))
      return er;
   if (!r && !rd->flocked)
      return rdd_err(ClipMachineMemory, EG_UNLOCKED, 0, __FILE__, __LINE__, __PROC__, er_notpermitted);

   if ((er = rdd_eof(ClipMachineMemory, rd, &eof, __PROC__)))
      return er;
   if (eof)
      return 0;

   fs = calloc(rd->nfields, sizeof(int));
   for (i = 0; i < rd->nfields; i++)
    {
       vp = _clip_vptr(_clip_mget(ClipMachineMemory, ap, _clip_casehashword(rd->fields[i].name, strlen(rd->fields[i].name))));
       if (vp)
	{
	   if ((er = rd->vtbl->getvalue(ClipMachineMemory, rd, i, &old, __PROC__)))
	      goto err;
	   _clip_cmp(ClipMachineMemory, &old, vp, &(fs[i]), 1);
	   _clip_destroy(ClipMachineMemory, &old);
	}
    }
   if (!rd->newrec)
    {
       for (i = 0; i < rd->ords_opened; i++)
	{
	   ro = rd->orders[i];
	   if (!ro->custom)
	    {
	       if (ro->simpexpr)
		{
		   for (j = 0; j < rd->nfields; j++)
		      if (fs[j] && ro->simpfno == j)
			 break;
		   if (j == rd->nfields)
		      continue;
		}
	       if ((er = rdd_event(ClipMachineMemory, EVENT_UPDATE, rd->area, 0, NULL, NULL, __PROC__)))
		  goto unlock;
	       if ((er = ro->vtbl->_wlock(ClipMachineMemory, ro, __PROC__)))
		  goto unlock;
	       if ((er = ro->vtbl->delkey(ClipMachineMemory, rd, ro, __PROC__)))
		  goto unlock;
	    }
	}
    }
   for (i = 0; i < rd->nfields; i++)
    {
       if (fs[i])
	{
	   vp =
	    _clip_vptr(_clip_mget(ClipMachineMemory, ap, _clip_casehashword(rd->fields[i].name, strlen(rd->fields[i].name))));
	   if ((er = rd->vtbl->setvalue(ClipMachineMemory, rd, i, vp, 0, __PROC__)))
	      goto unlock;
	}
    }
   if (!rd->newrec)
    {
       for (i = 0; i < rd->ords_opened; i++)
	{
	   ro = rd->orders[i];
	   if (!ro->custom)
	    {
	       if (ro->simpexpr)
		{
		   for (j = 0; j < rd->nfields; j++)
		      if (fs[j] && ro->simpfno == j)
			 break;
		   if (j == rd->nfields)
		      continue;
		}
	       if ((er = rdd_event(ClipMachineMemory, EVENT_UPDATE, rd->area, 0, NULL, NULL, __PROC__)))
		  goto unlock;
	       if ((er = ro->vtbl->addkey(ClipMachineMemory, rd, ro, NULL, __PROC__)))
		  goto unlock;
	       if ((er = ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__)))
		  goto unlock;
	    }
	}
    }
   free(fs);
   fs = NULL;

   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;
   if (rd->filter && rd->filter->rmap)
    {
       int       fok;

       if ((er = rdd_calcfilter(ClipMachineMemory, rd, &fok, __PROC__)))
	  return er;
       if (fok)
	  _rm_setbit(rd->filter->rmap, rd->filter->size, rd->recno);
       else
	  _rm_clrbit(rd->filter->rmap, rd->filter->size, rd->recno);
    }
   return 0;
 unlock:
   if (!rd->newrec)
    {
       for (i = 0; i < rd->ords_opened; i++)
	{
	   ro = rd->orders[i];
	   ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__);
	}
    }
 err:
   if (fs)
      free(fs);
   return er;
}

int
rdd_scopetop(ClipMachine * ClipMachineMemory, RDD_DATA * rd, ClipVar * v, const char *__PROC__)
{
   RDD_ORDER *ro;

   int       er;

   if (rd->curord == -1)
      return 0;

   ro = rd->orders[rd->curord];
   if (ro->scopetop)
    {
       free(ro->scopetop);
       ro->scopetop = NULL;
       _clip_destroy(ClipMachineMemory, &ro->scopetopvar);
    }
   if (v && v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
    {
       ro->scopetop = malloc(ro->keysize);
       if ((er = ro->vtbl->formatkey(ClipMachineMemory, ro, v, ro->scopetop, __PROC__)))
	  return er;
       _clip_clone(ClipMachineMemory, &ro->scopetopvar, v);
       if (v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
	  ro->stoplen = min(v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, ro->keysize);
       else
	  ro->stoplen = ro->keysize;
    }
   return 0;
}

int
rdd_scopebottom(ClipMachine * ClipMachineMemory, RDD_DATA * rd, ClipVar * v, const char *__PROC__)
{
   RDD_ORDER *ro;

   int       er;

   if (rd->curord == -1)
      return 0;

   ro = rd->orders[rd->curord];
   if (ro->scopebottom)
    {
       free(ro->scopebottom);
       ro->scopebottom = NULL;
       _clip_destroy(ClipMachineMemory, &ro->scopebottomvar);
    }
   if (v && v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
    {
       ro->scopebottom = malloc(ro->keysize);
       if ((er = ro->vtbl->formatkey(ClipMachineMemory, ro, v, ro->scopebottom, __PROC__)))
	  return er;
       _clip_clone(ClipMachineMemory, &ro->scopebottomvar, v);
       if (v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
	  ro->sbotlen = min(v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, ro->keysize);
       else
	  ro->sbotlen = ro->keysize;
    }
   return 0;
}

int
rdd_keyadd(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, ClipVar * v, const char *__PROC__)
{
   if (!ro->custom)
      return rdd_err(ClipMachineMemory, EG_UNSUPPORTED, 0, __FILE__, __LINE__,
		     __PROC__, "Trying to add key to automatic (non-custom) order");
   return ro->vtbl->addkey(ClipMachineMemory, rd, ro, v, __PROC__);
}

int
rdd_keydel(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   if (!ro->custom)
      return rdd_err(ClipMachineMemory, EG_UNSUPPORTED, 0, __FILE__, __LINE__,
		     __PROC__, "Trying to delete key from automatic (non-custom) order");
   return ro->vtbl->delkey(ClipMachineMemory, rd, ro, __PROC__);
}

int
rdd_keyvalue(ClipMachine * ClipMachineMemory, RDD_DATA * rd, ClipVar * v, const char *__PROC__)
{
   if (rd->curord == -1)
      return rdd_err(ClipMachineMemory, EG_NOORDER, 0, __FILE__, __LINE__, __PROC__, er_noorder);
   return rd->orders[rd->curord]->vtbl->keyvalue(ClipMachineMemory, rd, rd->orders[rd->curord], v, __PROC__);
}

int
rdd_locate(ClipMachine * ClipMachineMemory, RDD_DATA * rd, char *cfor,
	   ClipVar * fexpr, ClipVar * wexpr, ClipVar * vnext, ClipVar * vrec, ClipVar * vrest, int *found, const char *__PROC__)
{
   RDD_FILTER *old = NULL;

   int       next = -1, rec = 0, rest = 0, r = 0, er;

   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;
   *found = 0;
   if (!fexpr
       || (fexpr->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CCODE_type_of_ClipVarType
	   && fexpr->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != PCODE_type_of_ClipVarType))
      fexpr = 0;

   if (!wexpr
       || (wexpr->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CCODE_type_of_ClipVarType
	   && wexpr->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != PCODE_type_of_ClipVarType))
      wexpr = 0;

   if (vnext && vnext->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
    {
       next = _clip_double(vnext);
       next = next ? next - 1 : 0;
    }
   if (vrec && vrec->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
    {
       rec = _clip_double(vrec);
    }
   if (vrest && vrest->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == LOGICAL_type_of_ClipVarType)
      rest = vrest->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
   else if (fexpr)
      rest = 0;
   else
      rest = 1;

   if (!fexpr && !wexpr && !rd->locate_filter && !rd->locate_wpar)
      return 0;

   if (!wexpr)
    {
       if (!rest)
	{
	   if (rec)
	      er = rdd_goto(ClipMachineMemory, rd, rec, __PROC__);
	   else
	      er = rdd_gotop(ClipMachineMemory, rd, __PROC__);
	   if (er)
	      return er;
	   rd->locate_next = next;
	}
    }

   if (fexpr)
    {
       ClipVar  *dest = (ClipVar *) calloc(1, sizeof(ClipVar));

       if (rd->locate_wpar)
	{
	   _clip_destroy(ClipMachineMemory, rd->locate_wpar);
	   free(rd->locate_wpar);
	}
       if (rd->locate_filter)
	  if ((er = rdd_destroyfilter(ClipMachineMemory, rd->locate_filter, __PROC__)))
	     return er;
       if ((er = rdd_createfilter(ClipMachineMemory, rd, &rd->locate_filter, fexpr, cfor, NULL, 0, __PROC__)))
	  return er;
       if (rd->filter)
	  if ((er = rm_intersect(ClipMachineMemory, rd->locate_filter, rd->filter, __PROC__)))
	     return er;
       if (wexpr)
	{
	   _clip_clone(ClipMachineMemory, dest, wexpr);
	   rd->locate_wpar = dest;
	}
       else
	{
	   free(dest);
	   rd->locate_wpar = NULL;
	}
       rd->locate_next = next;
    }

   if (!rd->locate_wpar)
    {
       if (rd->filter)
	{
	   old = rd->filter;
	   old->active = 0;
	}
       rd->filter = rd->locate_filter;
       rd->filter->active = 1;
    }

   while (1)
    {
       int       wcond = 1;

       if ((er = rdd_eof(ClipMachineMemory, rd, &r, __PROC__)))
	  return er;

       if (r || !rd->locate_next)
	{
	  /* wa->locate_next = -1; */
	   break;
	}

       if (rd->locate_wpar)
	{
	   ClipVar   data;

	   _clip_eval(ClipMachineMemory, rd->locate_wpar, 0, 0, &data);

	   if (data.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
	      break;
	   else
	      wcond = data.ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;

	   _clip_destroy(ClipMachineMemory, &data);
	}

       if (!wcond)
	  break;

       if (rd->locate_wpar)
	{
	   ClipVar   data;

	   _clip_eval(ClipMachineMemory, &rd->locate_filter->fps->bfilter, 0, 0, &data);

	   if (data.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
	      break;
	   else
	      *found = data.ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;

	   _clip_destroy(ClipMachineMemory, &data);
	}
       else
	{
	   if ((er = rdd_calcfilter(ClipMachineMemory, rd, found, __PROC__)))
	      return er;
	}

       if (*found)
	  break;

       if ((er = rdd_skip(ClipMachineMemory, rd, 1, __PROC__)))
	  return er;

       if (rd->locate_next > 0)
	  rd->locate_next--;
    }

   if (!rd->locate_wpar)
    {
       if (rd->locate_filter)
	  rd->locate_filter->active = 0;
       rd->filter = NULL;
       if (old)
	{
	   rd->filter = old;
	   rd->filter->active = 1;
	}
    }
   return 0;
}

int
rdd_continue(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *found, const char *__PROC__)
{
   RDD_FILTER *old = NULL;

   int       er;

   if (!rd->locate_filter)
      return 0;
   if (rd->filter)
    {
       old = rd->filter;
       old->active = 0;
    }
   rd->filter = rd->locate_filter;
   rd->filter->active = 1;

   if ((er = rdd_skip(ClipMachineMemory, rd, 1, __PROC__)))
      return er;

   if (rd->locate_filter)
      rd->locate_filter->active = 0;
   rd->filter = NULL;
   if (old)
    {
       rd->filter = old;
       rd->filter->active = 1;
    }

   return rdd_locate(ClipMachineMemory, rd, NULL, NULL, NULL, NULL, NULL, NULL, found, __PROC__);
}

void
destroy_rdd_data(void *data)
{
   RDD_DATA *rd = (RDD_DATA *) data;

   if (rd)
    {
       if (rd->name)
	  free(rd->name);
       if (rd->path)
	  free(rd->path);
       if (rd->fields)
	  free(rd->fields);
       if (rd->indices)
	  free(rd->indices);
       if (rd->orders)
	  free(rd->orders);
       if (rd->word)
	  free(rd->word);
       if (rd->yylval)
	  free(rd->yylval);
       if (rd->locks)
	  free(rd->locks);
       if (rd->os.cForCondition)
	  free(rd->os.cForCondition);
       if (rd->data)
	  free(rd->data);
       if (rd->record)
	  free(rd->record);
       delete_HashTable(rd->hashes);
       free(rd);
    }
}

void
destroy_rdd_index(void *index)
{
   RDD_INDEX *ri = (RDD_INDEX *) index;

   if (ri)
    {
       if (ri->name)
	  free(ri->name);
       if (ri->path)
	  free(ri->path);
       if (ri->orders)
	  free(ri->orders);
       free(ri);
    }
}

void
destroy_rdd_memo(RDD_MEMO * rm)
{
   if (rm)
    {
       if (rm->name)
	  free(rm->name);
       if (rm->path)
	  free(rm->path);
       free(rm);
    }
}

void
destroy_rdd_order(ClipMachine * ClipMachineMemory, RDD_ORDER * ro)
{
   if (ro)
    {
       if (ro->name)
	  free(ro->name);
       if (ro->expr)
	  free(ro->expr);
       if (ro->scopetop)
	  free(ro->scopetop);
       if (ro->scopebottom)
	  free(ro->scopebottom);
       if (ro->cforexpr)
	  free(ro->cforexpr);
       if (ro->key)
	  free(ro->key);
       if (ro->curpage)
	  free(ro->curpage);
       _clip_destroy(ClipMachineMemory, &ro->block);
       _clip_destroy(ClipMachineMemory, &ro->scopetopvar);
       _clip_destroy(ClipMachineMemory, &ro->scopebottomvar);
       _clip_destroy(ClipMachineMemory, &ro->bforexpr);
       free(ro);
    }
}

void
destroy_rdd_i_order(void *order)
{
   RDD_ORDER *ro = (RDD_ORDER *) order;

   if (ro)
      free(ro);
}

void
destroy_ryo(void *data)
{
   RDD_FILTER *fp = (RDD_FILTER *) data;

   if (fp)
    {
       if (fp->sfilter)
	  free(fp->sfilter);
       if (fp->rmap)
	  free(fp->rmap);
       if (fp->fps)
	  free(fp->fps);
       if (fp->list)
	  free(fp->list);
       free(fp);
    }
}

inline unsigned int
_rdd_uint(unsigned char *s)
{
   unsigned int r;

   r = *(s + 3);
   r = r << 8;
   r += *(s + 2);
   r = r << 8;
   r += *(s + 1);
   r = r << 8;
   r += *s;
   return r;
}

inline unsigned short int
_rdd_ushort(unsigned char *s)
{
   unsigned short int r;

   r = *(s + 1);
   r = r << 8;
   r += *s;
   return r;
}

inline void
_rdd_put_uint(unsigned char s[], unsigned int v)
{
   s[0] = v & 0x000000ff;
   v = v >> 8;
   s[1] = v & 0x000000ff;
   v = v >> 8;
   s[2] = v & 0x000000ff;
   v = v >> 8;
   s[3] = v & 0x000000ff;
}

inline void
_rdd_put_ushort(unsigned char s[], unsigned short int v)
{
   s[0] = v & 0x00ff;
   s[1] = (v & 0xff00) >> 8;
}

inline unsigned int
_rdd_backuint(unsigned char *s)
{
   unsigned int r;

   r = *s;
   r = r << 8;
   r += *(s + 1);
   r = r << 8;
   r += *(s + 2);
   r = r << 8;
   r += *(s + 3);
   return r;
}

inline unsigned short int
_rdd_backushort(unsigned char *s)
{
   unsigned short int r;

   r = *s;
   r = r << 8;
   r += *(s + 1);
   return r;
}

inline void
_rdd_put_backuint(unsigned char s[], unsigned int v)
{
   s[3] = v & 0x000000ff;
   v = v >> 8;
   s[2] = v & 0x000000ff;
   v = v >> 8;
   s[1] = v & 0x000000ff;
   v = v >> 8;
   s[0] = v & 0x000000ff;
}

inline void
_rdd_put_backushort(unsigned char s[], unsigned short int v)
{
   s[1] = v & 0x00ff;
   s[0] = (v & 0xff00) >> 8;
}

static DbfLocale koi_locale = {
/*unsigned char _koi_cmptbl[] = */
   {
    0xc4, 0xb3, 0xda, 0xbf, 0xc0, 0xd9, 0xc3, 0xb4,
    0xc2, 0xc1, 0xc5, 0xdf, 0xdc, 0xdb, 0xdd, 0xde,
    0xb0, 0xb1, 0xb2, 0xf4, 0xfe, 0xf9, 0xfb, 0xf7,
    0xf3, 0xf2, 0xff, 0xf5, 0xf8, 0xfd, 0xfa, 0xf6,
    0xcd, 0xba, 0xd5, 0xf1, 0xd6, 0xc9, 0xb8, 0xb7,
    0xbb, 0xd4, 0xd3, 0xc8, 0xbe, 0xbd, 0xbc, 0xc6,
    0xc7, 0xcc, 0xb5, 0xf0, 0xb6, 0xb9, 0xd1, 0xd2,
    0xcb, 0xcf, 0xd0, 0xca, 0xd8, 0xd7, 0xce, 0xfc,
    0xee, 0xa0, 0xa1, 0xe6, 0xa4, 0xa5, 0xe4, 0xa3,
    0xe5, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae,
    0xaf, 0xef, 0xe0, 0xe1, 0xe2, 0xe3, 0xa6, 0xa2,
    0xec, 0xeb, 0xa7, 0xe8, 0xed, 0xe9, 0xe7, 0xea,
    0x9e, 0x80, 0x81, 0x96, 0x84, 0x85, 0x94, 0x83,
    0x95, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e,
    0x8f, 0x9f, 0x90, 0x91, 0x92, 0x93, 0x86, 0x82,
    0x9c, 0x9b, 0x87, 0x98, 0x9d, 0x99, 0x97, 0x9a},

/*unsigned char _koi_uptbl[] = */
   {
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
    0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
    0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
    0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
    0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
    0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7,
    0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
    0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
    0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
    0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
    0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff,
    0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
    0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
    0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
    0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff,
    },

/*unsigned char _koi_lowtbl[] = */
   {
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
    0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
    0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
    0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
    0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
    0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7,
    0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
    0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
    0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
    0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,
    0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
    0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
    0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
    0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,
    0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
    },
   {
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
    0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
    0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
    0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
    0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
    0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7,
    0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
    0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
    0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
    0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,
    0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
    0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
    0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
    0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
    0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff,
    },
   {
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
    0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
    0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
    0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
    0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
    0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7,
    0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
    0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
    0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
    0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,
    0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
    0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
    0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
    0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
    0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff,
    }
};

typedef struct
{
   char     *name;
   DbfLocale *locale;
}
LocData;

static int
cmp_locale(void *p1, void *p2)
{
   return strcmp(((LocData *) p1)->name, ((LocData *) p2)->name);
}

DbfLocale *
dbf_get_locale(ClipMachine * ClipMachineMemory)
{
#if 1
   char     *dbfcs;

   DbfLocale *dbf_locale;
   static Coll locales = { 0, 0, 0, 0, 0, 0 };
   LocData   ld, *lp;

   int       ind;

   char     *hostcs;

   cons_CharsetEntry *cs1 = 0, *cs2 = 0;

   int       len1 = 0, len2 = 0;

   unsigned char *buf;

   dbfcs = (char *) _clip_fetch_item(ClipMachineMemory, _clip_hashstr("DBF_CHARSET"));
   if (!dbfcs)
    {
       _clip_logg(2, "no item DBF_CHARSET, revert to KOI charset");
       return &koi_locale;
    }

   if (!locales.compare)
      init_Coll(&locales, 0, cmp_locale);

   ld.name = dbfcs;
   if (search_Coll(&locales, &ld, &ind))
    {
       lp = (LocData *) locales.items[ind];
       return lp->locale;
    }

   hostcs = _clip_getenv("CLIP_HOSTCS");
   if (!hostcs)
    {
       char     *s;

       s = _clip_getenv("LC_ALL");
       if (s && *s)
	{
	   hostcs = strrchr(s, '.');
	   if (hostcs)
	      hostcs++;
	   else if (strcmp(s, "C") && strcmp(s, "POSIX"))
	      hostcs = s;
	}
    }
   if (!hostcs)
      hostcs = _clip_getenv("CLIP_LANG");
   if (hostcs == NULL)
      hostcs = _clip_getenv("LANG");

   if (!hostcs)
    {
       _clip_logg(0, "dbf: cannot determine host charset, revert to koi_locale");
       return &koi_locale;
    }

   _clip_logg(2, "load host charset '%s'", hostcs);
   if (load_charset_name(hostcs, &cs1, &len1))
    {
       _clip_logg(0, "dbf: cannot load charset '%s': %s, revert to koi_locale", hostcs, strerror(errno));
       return &koi_locale;
    }

   _clip_logg(2, "load dbf charset '%s'", dbfcs);
   if (load_charset_name(dbfcs, &cs2, &len2))
    {
       _clip_logg(0, "dbf: cannot load charset '%s': %s, revert to koi_locale", dbfcs, strerror(errno));
       return &koi_locale;
    }

   buf = (unsigned char *) calloc(256, 3);

   make_translation(cs1, len1, cs2, len2, buf);
   make_translation(cs2, len2, cs1, len1, buf + 256);

   free(cs1);
   free(cs2);

   dbf_locale = (DbfLocale *) calloc(1, sizeof(DbfLocale));
   lp = (LocData *) calloc(1, sizeof(LocData));
   lp->locale = dbf_locale;
   lp->name = strdup(dbfcs);

   memcpy(dbf_locale->read, buf + 256 + 128, 128);
   memcpy(dbf_locale->write, buf + 128, 128);

#if 1
   memcpy(dbf_locale->cmp, _clip_cmptbl + 128, 128);
   memcpy(dbf_locale->upper, _clip_uptbl + 128, 128);
   memcpy(dbf_locale->lower, _clip_lowtbl + 128, 128);
#else
   load_charset_tables(dbfcs, buf, buf + 256, buf + 512, 0, 0);
   memcpy(dbf_locale->cmp, buf + 128, 128);
   memcpy(dbf_locale->upper, buf + 256 + 128, 128);
   memcpy(dbf_locale->lower, buf + 512 + 128, 128);
#endif

   free(buf);
   insert_Coll(&locales, lp);
   return dbf_locale;

#else
   char     *s;

   DbfLocale *dbf_locale = &koi_locale;

   s = (char *) _clip_fetch_item(ClipMachineMemory, _clip_hashstr("DBF_CHARSET"));
   if (s && (!strcasecmp(s, "alt") || !strcasecmp(s, "866")))
      dbf_locale = &alt_locale;
   if (s && (!strcasecmp(s, "win") || !strcasecmp(s, "1251")))
      dbf_locale = &win_locale;
   if (s && (!strcasecmp(s, "main") || !strcasecmp(s, "459")))
      dbf_locale = &main_locale;

   return dbf_locale;
#endif
}

int
rdd_ii_create(ClipMachine * ClipMachineMemory, const char *driver, const char *name, const char *__PROC__)
{
   RDD_INDEX *ri = NULL;

   int       er = EG_UNSUPPORTED;

   ri = (RDD_INDEX *) calloc(1, sizeof(RDD_INDEX));

   memset(ri, 0, sizeof(RDD_INDEX));
   if (!(ri->vtbl = rdd_indexdriver(ClipMachineMemory, driver, __PROC__)))
      goto err;

   if ((er = _rdd_parsepath(ClipMachineMemory, name, ri->vtbl->suff, &ri->path, &ri->name, EG_CREATE, __PROC__)))
      goto err;

   ri->file.filehash = _clip_hashstr(ri->path);

   ri->file.fd = _clip_creat(ClipMachineMemory, ri->path, O_RDWR, ClipMachineMemory->fileCreateMode, 1);
   if (ri->file.fd == -1)
      goto err_create;
   ri->file.mapsize = 1024;
   if (lseek(ri->file.fd, ri->file.mapsize - 1, SEEK_SET) == -1)
      goto err_create;
   if (write(ri->file.fd, "", 1) == -1)
      goto err_create;
   ri->file.md = (caddr_t) - 1;
#ifdef HAVE_MMAN_H
   if (ClipMachineMemory->flags1 & MAP_FILE_FLAG)
      ri->file.md = (caddr_t) mmap(0, ri->file.mapsize, PROT_READ | PROT_WRITE, MAP_SHARED, ri->file.fd, 0);
#endif
   if ((er = ri->vtbl->ii_create(ClipMachineMemory, ri, __PROC__)))
      goto err;

   _clip_retni(ClipMachineMemory, _clip_store_c_item(ClipMachineMemory, ri, _C_ITEM_TYPE_I_INDEX, destroy_rdd_index));
   return 0;
 err_create:
   er = rdd_err(ClipMachineMemory, EG_CREATE, errno, __FILE__, __LINE__, __PROC__, ri->path);
 err:
   if (ri && ri->name)
      free(ri->name);
   if (ri && ri->path)
      free(ri->path);
#ifdef HAVE_MMAN_H
   if (ri && ri->file.md != (char *) -1)
      munmap(ri->file.md, ri->file.mapsize);
#endif
   if (ri && ri->file.fd != -1)
      _clip_close(ClipMachineMemory, ri->file.filehash, ri->file.fd);
   if (ri)
      free(ri);
   return er;
}

int
rdd_ii_createtag(ClipMachine * ClipMachineMemory, int h, const char *tag, const char *expr, const char *__PROC__)
{
   RDD_INDEX *ri = (RDD_INDEX *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_I_INDEX);

   RDD_ORDER *ro;

   int       er;

   CHECKII(ri);

   if ((er = ri->vtbl->ii_createtag(ClipMachineMemory, ri, tag, expr, &ro, __PROC__)))
      return er;

   ro->c_item = _clip_store_c_item(ClipMachineMemory, ro, _C_ITEM_TYPE_I_ORDER, destroy_rdd_i_order);
   _clip_retni(ClipMachineMemory, ro->c_item);
   return 0;
}

int
rdd_ii_open(ClipMachine * ClipMachineMemory, const char *driver, const char *name, const char *__PROC__)
{
   RDD_INDEX *ri = NULL;

   struct stat st;

   int       er = EG_UNSUPPORTED;

   ri = (RDD_INDEX *) calloc(1, sizeof(RDD_INDEX));
   memset(ri, 0, sizeof(RDD_INDEX));
   if (!(ri->vtbl = rdd_indexdriver(ClipMachineMemory, driver, __PROC__)))
      goto err;

   if ((er = _rdd_parsepath(ClipMachineMemory, name, ri->vtbl->suff, &ri->path, &ri->name, EG_CREATE, __PROC__)))
      goto err;

   ri->file.filehash = _clip_hashstr(ri->path);

   if ((er = rdd_open(ClipMachineMemory, ri->path, 0, 0, &ri->file.fd, __PROC__)))
      goto err;

   if (fstat(ri->file.fd, &st) == -1)
      goto err_open;
   ri->file.mapsize = st.st_size;
   ri->file.md = (caddr_t) - 1;
#ifdef HAVE_MMAN_H
   if (ClipMachineMemory->flags1 & MAP_FILE_FLAG)
      ri->file.md = (caddr_t) mmap(0, ri->file.mapsize, PROT_READ | PROT_WRITE, MAP_SHARED, ri->file.fd, 0);
#endif
   if ((er = ri->vtbl->ii_open(ClipMachineMemory, ri, __PROC__)))
      goto err;

   _clip_retni(ClipMachineMemory, _clip_store_c_item(ClipMachineMemory, ri, _C_ITEM_TYPE_I_INDEX, destroy_rdd_index));
   return 0;
 err_open:
   er = rdd_err(ClipMachineMemory, EG_OPEN, errno, __FILE__, __LINE__, __PROC__, ri->path);
 err:
   if (ri && ri->name)
      free(ri->name);
   if (ri && ri->path)
      free(ri->path);
#ifdef HAVE_MMAN_H
   if (ri && ri->file.md != (char *) -1)
      munmap(ri->file.md, ri->file.mapsize);
#endif
   if (ri && ri->file.fd != -1)
      _clip_close(ClipMachineMemory, ri->file.filehash, ri->file.fd);
   if (ri)
      free(ri);
   return er;
}

int
rdd_ii_opentag(ClipMachine * ClipMachineMemory, int h, const char *tag, const char *__PROC__)
{
   RDD_INDEX *ri = (RDD_INDEX *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_I_INDEX);

   RDD_ORDER *ro;

   int       er;

   CHECKII(ri);

   if ((er = ri->vtbl->ii_opentag(ClipMachineMemory, ri, tag, &ro, __PROC__)))
      return er;

   ro->c_item = _clip_store_c_item(ClipMachineMemory, ro, _C_ITEM_TYPE_I_ORDER, destroy_rdd_i_order);
   _clip_retni(ClipMachineMemory, ro->c_item);
   return 0;
}

int
rdd_ii_close(ClipMachine * ClipMachineMemory, int h, const char *__PROC__)
{
   RDD_INDEX *ri = (RDD_INDEX *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_I_INDEX);

   int       i;

   int       er;

   CHECKII(ri);
   for (i = 0; i < ri->norders; i++)
    {
       if ((er = rdd_ii_closetag(ClipMachineMemory, ri->orders[i]->c_item, __PROC__)))
	  return er;
    }
   _clip_destroy_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_I_INDEX);
   return 0;
}

int
rdd_ii_closetag(ClipMachine * ClipMachineMemory, int h, const char *__PROC__)
{
   RDD_ORDER *ro = (RDD_ORDER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_I_ORDER);

   int       i;

   CHECKIO(ro);
   for (i = 0; i < ro->index->norders; i++)
    {
       if (ro->index->orders[i] == ro)
	{
	   for (i++; i < ro->index->norders; i++)
	    {
	       ro->index->orders[i - 1] = ro->index->orders[i];
	    }
	}
    }
   ro->index->norders--;
   ro->index->orders = (RDD_ORDER **) realloc(ro->index->orders, ro->index->norders * sizeof(RDD_ORDER *));
   destroy_rdd_order(ClipMachineMemory, ro);
   _clip_destroy_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_I_ORDER);
   return 0;
}

int
rdd_ii_addkey(ClipMachine * ClipMachineMemory, int h, const char *id, ClipVar * key, const char *__PROC__)
{
   RDD_ORDER *ro = (RDD_ORDER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_I_ORDER);

   CHECKIO(ro);
   if ((key->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType && ro->type != 'C') ||
       (key->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType && ro->type != 'N') ||
       (key->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == DATE_type_of_ClipVarType && ro->type != 'D') ||
       (key->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == LOGICAL_type_of_ClipVarType && ro->type != 'L'))
      return rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, "Type mismatch");

   return ro->vtbl->ii_addkey(ClipMachineMemory, ro, id, key, __PROC__);
}

int
rdd_ii_delkey(ClipMachine * ClipMachineMemory, int h, const char *__PROC__)
{
   RDD_ORDER *ro = (RDD_ORDER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_I_ORDER);

   CHECKIO(ro);
   if (ro->eof)
      return 0;
   return ro->vtbl->ii_delkey(ClipMachineMemory, ro, __PROC__);
}

int
rdd_ii_gotop(ClipMachine * ClipMachineMemory, int h, const char *__PROC__)
{
   RDD_ORDER *ro = (RDD_ORDER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_I_ORDER);

   CHECKIO(ro);
   return ro->vtbl->ii_gotop(ClipMachineMemory, ro, __PROC__);
}

int
rdd_ii_gobottom(ClipMachine * ClipMachineMemory, int h, const char *__PROC__)
{
   RDD_ORDER *ro = (RDD_ORDER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_I_ORDER);

   CHECKIO(ro);
   return ro->vtbl->ii_gobottom(ClipMachineMemory, ro, __PROC__);
}

int
rdd_ii_bof(ClipMachine * ClipMachineMemory, int h, const char *__PROC__)
{
   RDD_ORDER *ro = (RDD_ORDER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_I_ORDER);

   CHECKIO(ro);
   _clip_retl(ClipMachineMemory, ro->bof);
   return 0;
}

int
rdd_ii_eof(ClipMachine * ClipMachineMemory, int h, const char *__PROC__)
{
   RDD_ORDER *ro = (RDD_ORDER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_I_ORDER);

   CHECKIO(ro);
   _clip_retl(ClipMachineMemory, ro->eof);
   return 0;
}

int
rdd_ii_id(ClipMachine * ClipMachineMemory, int h, const char *__PROC__)
{
   ClipVar  *id = RETPTR(ClipMachineMemory);

   RDD_ORDER *ro = (RDD_ORDER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_I_ORDER);

   CHECKIO(ro);
   return ro->vtbl->ii_id(ClipMachineMemory, ro, id, __PROC__);
}

int
rdd_ii_key(ClipMachine * ClipMachineMemory, int h, const char *__PROC__)
{
   ClipVar  *key = RETPTR(ClipMachineMemory);

   RDD_ORDER *ro = (RDD_ORDER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_I_ORDER);

   CHECKIO(ro);
   return ro->vtbl->ii_key(ClipMachineMemory, ro, key, __PROC__);
}

int
rdd_ii_skip(ClipMachine * ClipMachineMemory, int h, int recs, const char *__PROC__)
{
   RDD_ORDER *ro = (RDD_ORDER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_I_ORDER);

   int       i;

   int       er;

   CHECKIO(ro);

   if (recs > 0)
    {
       for (i = 0; i < recs; i++)
	{
	   if ((er = ro->vtbl->ii_next(ClipMachineMemory, ro, __PROC__)))
	      return er;
	}
    }
   else
    {
       for (i = 0; i > recs; i--)
	{
	   if ((er = ro->vtbl->ii_prev(ClipMachineMemory, ro, __PROC__)))
	      return er;
	}
    }
   return 0;
}

int
rdd_event(ClipMachine * ClipMachineMemory, int nEvent, int nArea, int nFieldPos,
	  ClipVar * xTrigVal, int *re, const char *__PROC__)
{
   ClipFunction *fp;

   ClipBlock *bp;

   ClipVar   tres;

   ClipVar   args[4];

   DBWorkArea *wa;

   int       er;

   if (re)
      *re = 1;
   if (nArea < 0)
      return 0;

   wa = ClipMachineMemory->areas->items_of_ClipVect[nArea];
   if (!wa->trig_active)
      return 0;

   if (_clip_get_function(ClipMachineMemory, wa->trighash, &fp, &bp))
    {
       memset(args, 0, sizeof(args));
       args[0].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
       args[0].ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
       args[0].ClipType_t_of_ClipVar.memo_of_ClipType = 0;
       args[0].ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = nEvent;
       args[1].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
       args[1].ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
       args[1].ClipType_t_of_ClipVar.memo_of_ClipType = 0;
       args[1].ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = nArea + 1;
       args[2].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
       args[2].ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
       args[2].ClipType_t_of_ClipVar.memo_of_ClipType = 0;
       args[2].ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = nFieldPos;
       if (xTrigVal)
	{
	   args[3].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType =
	    xTrigVal->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType;
	   args[3].ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MREF_ClipFlags;
	   args[3].ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = xTrigVal;
	}
       if ((er = _clip_clip(ClipMachineMemory, wa->trigger, 4, args, &tres)))
	  return er;
       if (re)
	  *re = tres.ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
       _clip_destroy(ClipMachineMemory, &tres);
    }
   else
    {
       wa->trig_active = 0;
    }
   return 0;
}

int
_rdd_parsepath(ClipMachine * ClipMachineMemory, const char *toopen,
	       const char *suff, char **path, char **name, int oper, const char *__PROC__)
{
   char      p[PATH_MAX];

   char      ret[PATH_MAX];

   char     *c, *e;

   strncpy(p, toopen, sizeof(p) - 1);
   c = strrchr(p, '\\');
   e = strrchr(p, '/');
   e = max(c, e);
   if (!((c = strrchr(p, '.')) && (*(c + 1) != '/') && (*(c + 1) != '\\') && (c >= e)))
    {
       strncat(p, suff, sizeof(p) - strlen(p) - 1);
    }
   strncpy(ret, p, PATH_MAX);
   if ((ClipMachineMemory->flags & TRANSLATE_FLAG) && ((ret[0] == '\\')))
    {
       char     *dname = _clip_fetch_item(ClipMachineMemory, CLIP_CUR_DRIVE);

       snprintf(p, PATH_MAX, "%c:%s", *dname, ret);
    }
   if (_clip_path(ClipMachineMemory, p, ret, sizeof(ret), oper == EG_CREATE))
      return rdd_err(ClipMachineMemory, oper, errno, __FILE__, __LINE__, __PROC__, toopen);
   *path = strdup(ret);
   if (name)
    {
       c = strrchr(ret, '/');
       if (!c)
	  c = strrchr(ret, '\\');
       e = strrchr(ret, '.');
       *e = 0;
       *name = strdup(c + 1);
    }
   return 0;
}

int
rdd_seekeval(ClipMachine * ClipMachineMemory, RDD_DATA * rd, ClipVar * block, int *found, const char *__PROC__)
{
   RDD_ORDER *ro;

   int       er;

   if ((rd->ords_opened < 1) || (rd->curord == -1))
      return rdd_err(ClipMachineMemory, EG_NOORDER, 0, __FILE__, __LINE__, __PROC__, er_noorder);

   ro = rd->orders[rd->curord];
   if (!ro->vtbl->wildskip)
      return rdd_err(ClipMachineMemory, EG_UNSUPPORTED, 0, __FILE__, __LINE__,
		     __PROC__, "Operation is not supported by current RDD");

   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;

   if ((er = ro->vtbl->wildskip(ClipMachineMemory, rd, ro, NULL, 0, NULL, block, found, __PROC__)))
      return er;
   if (!(*found))
    {
       unsigned int lastrec;

       if ((er = rdd_lastrec(ClipMachineMemory, rd, (int *) (&lastrec), __PROC__)))
	  return er;
       rd->eof = 1;
       if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	  return er;
    }
   return 0;
}

static int
_rdd_wildskip(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *pattern,
	      int regular, const char *s, int *found, const char *__PROC__)
{
   RDD_ORDER *ro = rd->orders[rd->curord];

   int       er;

   if (ro->vtbl->wildskip)
    {
       if ((er = ro->vtbl->wildskip(ClipMachineMemory, rd, ro, pattern, regular, s, NULL, found, __PROC__)))
	  return er;
    }
   else
    {
       ClipVar   v, *vp;

       char     *e;

       *found = 1;
       memset(&v, 0, sizeof(ClipVar));
       if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &v, 0)))
	  return er;
       vp = _clip_vptr(&v);
       e =
	vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf +
	vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
       while ((--e >= vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf) && *e == ' ');
       *(e + 1) = 0;
       while (!rd->eof
	      && (memcmp(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, pattern, s - pattern) == 0)
	      && (_clip_glob_match(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, pattern, 1) == -1))
	{

	   _clip_destroy(ClipMachineMemory, &v);
	   if ((er = rdd_skip(ClipMachineMemory, rd, 1, __PROC__)))
	      return er;
	   if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &v, 0)))
	      return er;
	   vp = _clip_vptr(&v);
	   e =
	    vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf +
	    vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
	   while ((--e >= vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf) && *e == ' ');
	   *(e + 1) = 0;
	}
       if (rd->eof || (memcmp(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, pattern, s - pattern) != 0))
	  *found = 0;

       _clip_destroy(ClipMachineMemory, &v);
    }
   return 0;
}

int
rdd_wildseek(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *pat,
	     int regular, int cont, int *found, const char *__PROC__)
{
   char     *e;

   char     *s;

   char     *q;

   char     *b = NULL;

   char     *pattern;

   int       er;

   if ((rd->ords_opened < 1) || (rd->curord == -1))
      return rdd_err(ClipMachineMemory, EG_NOORDER, 0, __FILE__, __LINE__, __PROC__, er_noorder);

   if (rd->orders[rd->curord]->type != 'C')
      return rdd_err(ClipMachineMemory, EG_UNSUPPORTED, 0, __FILE__, __LINE__, __PROC__, "Order is not CHARACTER");

   pattern = strdup(pat);
   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      goto err;

   e = (char *) pattern + strlen(pattern);
   loc_write(rd->loc, (unsigned char *) pattern, e - pattern);
   if (!regular)
    {
       s = strchr(pattern, '*');
       q = strchr(pattern, '?');
       if (!s)
	  s = e;
       if (!q)
	  q = e;
       if (!b)
	  b = e;
       s = min(min(s, q), b);
    }
   else
    {
       s = NULL;
    }

   if (!cont)
    {
       ClipVar   v;

       int       fnd;

       if ((s == pattern) || regular)
	{
	   if ((er = rd->orders[rd->curord]->vtbl->gotop(ClipMachineMemory, rd, rd->orders[rd->curord], __PROC__)))
	      goto err;
	}
       else
	{
	   memset(&v, 0, sizeof(ClipVar));
	   v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = malloc(s - pattern + 1);
	   memcpy(v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, pat, s - pattern);
	   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[s - pattern] = 0;
	   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = s - pattern;
	   if ((er =
		rd->orders[rd->curord]->vtbl->seek(ClipMachineMemory, rd, rd->orders[rd->curord], &v, 0, 0, &fnd, __PROC__)))
	      goto err;
	   free(v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
	}
    }
   else
    {
       if ((er = rdd_skip(ClipMachineMemory, rd, 1, __PROC__)))
	  goto err;
    }
   if (rd->eof)
    {
       *found = 0;
    }
   else
    {
       if ((er = _rdd_wildskip(ClipMachineMemory, rd, pattern, regular, s, found, __PROC__)))
	  goto err;
       if (!(*found))
	{
	   unsigned int lastrec;

	   if ((er = rdd_lastrec(ClipMachineMemory, rd, (int *) (&lastrec), __PROC__)))
	      goto err;
	   rd->eof = 1;
	   if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, lastrec + 1, 0, __PROC__)))
	      goto err;
	}
    }
   free(pattern);
   return 0;
 err:
   free(pattern);
   return er;
}

int
rdd_rawread(ClipMachine * ClipMachineMemory, RDD_DATA * rd, void *buf, const char *__PROC__)
{
   return rdd_read(ClipMachineMemory, &rd->file, rd->hdrsize + rd->recsize * (rd->recno - 1), rd->recsize, buf, __PROC__);
}

int
rdd_rawwrite(ClipMachine * ClipMachineMemory, RDD_DATA * rd, void *buf, int append, const char *__PROC__)
{
   int       er;

   if ((er = rdd_write(ClipMachineMemory, &rd->file, rd->hdrsize + rd->recsize * (rd->recno - 1), rd->recsize, buf, __PROC__)))
      return er;
   if (append)
    {
       unsigned char lr[4];

       long      flen;

       _rdd_put_uint(lr, rd->recno);
       if ((er = rdd_write(ClipMachineMemory, &rd->file, 4, 4, lr, __PROC__)))
	  return er;
       rd->recno++;
       flen = rd->hdrsize + rd->recsize * rd->recno + 1;
       if (rd->file.md != (caddr_t) (-1))
	{
	   if (flen > rd->file.mapsize)
	    {
	       int       delta = rd->recno / 5;

	       if ((er =
		    rdd_write(ClipMachineMemory, &rd->file,
			      rd->hdrsize + rd->recsize * (rd->recno + delta) - 1, 1, "", __PROC__)))
		  return er;
	    }
	}
    }
   rd->updated = 1;
   return 0;
}
