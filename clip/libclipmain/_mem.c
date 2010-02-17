/*
	Copyright (C) 2001  ITK
	Author   : Paul Lasarev <paul@itk.ru>
	License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <errno.h>

#include "ci_clip.h"
#include "ci_error.ch"
#include "ci_rdd.h"
#include "ci_coll.h"
#include "ci_rational.h"

#define NEW(type) ((type*)calloc(sizeof(type),1))
#define NEWVECT(type,len) ((type*)calloc(sizeof(type),(len)))

#define DBFDATA_NONFREE 0x1

typedef struct
{
   unsigned char name[11];
   unsigned char type;
   unsigned char reserved1[4];
   unsigned char len;
   unsigned char dec;
   unsigned char reserved2[14];
}
MemHeader;

typedef struct
{
   unsigned char type;		/* 'C' 'N' 'D' 'L' 'M' */
   unsigned char len;
   unsigned char dec;
   unsigned char flags;
   union
   {
      double    n;
      struct
      {
	 char     *str;
	 int       len;
      }
      c;
      int       l;
      long      d;		/* julian */
   }
   u;
}
DbfData;

DbfLocale *dbf_get_locale(ClipMachine * ClipMachineMemory);

int       clip2dbf(ClipVar * vp, DbfData * dp);

int       dbf2clip(DbfData * dp, ClipVar * vp);

void
destroy_DbfData(DbfData * vp)
{
   if (!vp)
      return;
   switch (vp->type)
    {
    case 'C':
    case 'M':
       if (!(vp->flags & DBFDATA_NONFREE))
	  free(vp->u.c.str);
       break;
    }
   vp->type = 'U';
   vp->flags = 0;
}

int
clip2dbf(ClipVar * vp, DbfData * dp)
{
   if (!vp || !dp)
      return 1;
   vp = _clip_vptr(vp);
   switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
    {
    case CHARACTER_type_of_ClipVarType:
       dp->type = 'C';
      /*
         dp->u.c.str = strdup(vp->s.ClipBuf_str_of_ClipStrVar.buf);
       */
       dp->u.c.str = malloc(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf + 1);
       memcpy(dp->u.c.str, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf,
	      vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf + 1);
       dp->u.c.len = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;

       break;
    case NUMERIC_type_of_ClipVarType:
       dp->type = 'N';
       if (vp->ClipType_t_of_ClipVar.memo_of_ClipType)
			 dp->u.n = rational_toDouble(vp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
       else
	  dp->u.n = vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
       break;
    case LOGICAL_type_of_ClipVarType:
       dp->type = 'L';
       dp->u.l = vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
       break;
    case DATE_type_of_ClipVarType:
       dp->type = 'D';
       dp->u.d = vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar;
       break;
    default:
       return 1;
    }
   dp->len = (unsigned char) vp->ClipType_t_of_ClipVar.len_of_ClipType;
   dp->dec = (unsigned char) vp->ClipType_t_of_ClipVar.dec_of_ClipType;
   dp->flags = 0;
   return 0;
}

int
dbf2clip(DbfData * dp, ClipVar * vp)
{
   if (!dp || !vp)
      return 1;
   memset(vp, 0, sizeof(ClipVar));
   switch (dp->type)
    {
    case 'C':
       vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
       vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = dp->u.c.str;
       dp->u.c.str = 0;
       vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = dp->u.c.len;
       break;
    case 'M':
       vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
       vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = dp->u.c.str;
       dp->u.c.str = 0;
       vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = dp->u.c.len;
       vp->ClipType_t_of_ClipVar.memo_of_ClipType = 1;
       break;
    case 'N':
       vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
       vp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
      /*
         vp->t.memo_of_ClipType= 1;
         vp->r.r = rational_double_init(dp->u.n);
         vp->t.len_of_ClipType = dp->u.c.len;
       */
       vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = dp->u.n;
      //vp->t.dec = dp->u.c.dec;   ???????? ��� dec � dbf ?????
       break;
    case 'L':
       vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
       vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = dp->u.l;
       break;
    case 'D':
       vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATE_type_of_ClipVarType;
       vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = dp->u.d;
       break;
    default:
       return 1;
    }
   vp->ClipType_t_of_ClipVar.len_of_ClipType = dp->len;
   vp->ClipType_t_of_ClipVar.dec_of_ClipType = dp->dec;

   return 0;
}

static int
mem_error(char *errbuf, int errbuflen, int line, const char *fmt, ...)
{
   int       l;

   va_list   ap;

   va_start(ap, fmt);
   snprintf(errbuf, errbuflen, "MEM: %s, line %d: ", __FILE__, line);
   l = strlen(errbuf);
   vsnprintf(errbuf + l, errbuflen - l, fmt, ap);
   va_end(ap);
   return 1;
}

int
read_MemFile(const char *path, DbfLocale * tbl, DbfData ** resp, char ***names, int *rescount, char *errbuf, int errbuflen)
{
   int       r = 0, i, line;

   FILE     *file;

   MemHeader mh;

   DbfData  *dp;

   file = fopen(path, "rb");
   if (!file)
      return mem_error(errbuf, errbuflen, __LINE__, "cannot open file '%s': %s", path, strerror(errno));

   for (i = 0;; ++i)
    {
       if (fread(&mh, sizeof(mh), 1, file) != 1)
	{
	   if (feof(file))
	      break;
	   line = __LINE__;
	 readerr:
	   r = mem_error(errbuf, errbuflen, line, "file '%s' read error: %s", strerror(errno));
	   goto ret;
	}

       *resp = (DbfData *) realloc(*resp, ((*rescount) + 1) * sizeof(DbfData));
       *names = (char **) realloc(*names, ((*rescount) + 1) * sizeof(char *));

       dp = (*resp) + (*rescount);
       memset(dp, 0, sizeof(DbfData));
       (*names)[*rescount] = strdup((const char *) mh.name);
       ++(*rescount);

      //printf("\ntype=%d,%x,len=%d,name=%s\n",mh.type,mh.type,mh.len,mh.name);
       switch (mh.type)
	{
	case 'C':		/* string */
	case 0xC3:		/* string */
	   {
	      char     *sp;

	      int       len;

	      len = mh.len | (mh.dec << 8);

	      sp = (char *) malloc(len);
	      if (fread(sp, len, 1, file) != 1)
	       {
		  line = __LINE__;
		  free(sp);
		  goto readerr;
	       }

	      dp->type = 'C';
	      dp->len = mh.len;
	      dp->dec = mh.dec;
	      dp->flags = 0;
	      dp->u.c.str = sp;
	      dp->u.c.len = --len;

	      while (len)
	       {
		  int       s = *(unsigned char *) sp;

		  if (tbl && s > 127)
		     *sp = tbl->read[s - 128];
		  --len;
		  ++sp;
	       }
	   }
	   break;
	case 'L':		/* logic */
	case 0xCC:		/* logic */
	   {
	      char      ch;

	      if (fread(&ch, 1, 1, file) != 1)
	       {
		  line = __LINE__;
		  goto readerr;
	       }
	      dp->type = 'L';
	      dp->len = mh.len;
	      dp->dec = mh.dec;
	      dp->flags = 0;
	      dp->u.l = ch;
	   }
	   break;
	case 'N':		/* numeric */
	case 0xCE:		/* numeric */
	   {
	      double    d = 0;

	      unsigned char buf[8];

	      int       j;

	      if (fread(buf, 8, 1, file) != 1)
	       {
		  line = __LINE__;
		  goto readerr;
	       }
	      for (j = 0; j < 8; ++j)
		 ((unsigned char *) &d)[j] = buf[j];

	      dp->type = 'N';
	      dp->len = mh.len;
	      dp->dec = mh.dec;
	      dp->flags = 0;
	      dp->u.n = d;
	   }
	   break;
	case 'D':		/* date */
	case 0xC4:		/* date */
	   {
	      double    d = 0;

	      unsigned char buf[8];

	      int       j;

	      if (fread(buf, 8, 1, file) != 1)
	       {
		  line = __LINE__;
		  goto readerr;
	       }
	      for (j = 0; j < 8; ++j)
		 ((unsigned char *) &d)[j] = buf[j];

	      dp->type = 'D';
	      dp->len = mh.len;
	      dp->dec = mh.dec;
	      dp->flags = 0;
	      dp->u.d = d;
	   }
	   break;

	default:
	   r = mem_error(errbuf, errbuflen, __LINE__, "invalid mem field type : 0x%0x", mh.type);
	   goto ret;
	}

    }

 ret:
   fclose(file);

   return r;
}

int
write_MemFile(const char *path, DbfLocale * tbl, DbfData * arr, char **names, int acount, char *errbuf, int errbuflen)
{
   int       r = 0, i, line;

   FILE     *file;

   MemHeader mh;

   DbfData  *dp;

   file = fopen(path, "wb");
   if (!file)
      return mem_error(errbuf, errbuflen, __LINE__, "cannot open file '%s': %s", path, strerror(errno));

   for (i = 0; i < acount; ++i)
    {
       dp = arr + i;

       memset(&mh, 0, sizeof(mh));
       strncpy((char *) mh.name, names[i], 10);

       switch (dp->type)
	{
	case 'C':
	   {
	      int       len;

	      len = dp->u.c.len + 1;
	      mh.type = 0xC3;
	      mh.len = len & 0xff;
	      mh.dec = (len >> 8) & 0xff;
	   }
	   break;
	case 'L':
	   mh.type = 0xCC;
	   mh.len = 1;
	   break;
	case 'N':
	   mh.type = 0xCE;
	   mh.len = dp->len;
	   mh.dec = dp->dec;
	   break;
	case 'D':
	   mh.type = 0xC4;
	   mh.len = 1;
	   break;
	default:
	   continue;
	}

       if (fwrite(&mh, sizeof(mh), 1, file) != 1)
	{
	   line = __LINE__;
	 writerr:
	   r = mem_error(errbuf, errbuflen, line, "file '%s' write error: %s", strerror(errno));
	   goto ret;
	}

       switch (mh.type)
	{
	case 0xC3:		/* string */
	   {
	      char     *sp;

	      int       len;

	      int       j;

	      len = dp->u.c.len + 1;
	      sp = dp->u.c.str;

	      for (j = 0; j < len; ++j, ++sp)
	       {
		  int       s = *(unsigned char *) sp;

		  if (tbl && s > 127)
		     s = tbl->write[s - 128];
		  if (fputc(s, file) == EOF)
		   {
		      line = __LINE__;
		      goto writerr;
		   }
	       }
	   }
	   break;
	case 0xCC:		/* logic */
	   {
	      char      ch;

	      ch = dp->u.l;
	      if (fputc(ch, file) == EOF)
	       {
		  line = __LINE__;
		  goto writerr;
	       }
	   }
	   break;
	case 0xCE:		/* numeric */
	   {
	      double    d;

	      unsigned char buf[8];

	      int       j;

	      d = dp->u.n;
	      for (j = 0; j < 8; ++j)
		 buf[j] = ((unsigned char *) &d)[j];
	      if (fwrite(buf, 8, 1, file) != 1)
	       {
		  line = __LINE__;
		  goto writerr;
	       }
	   }
	   break;
	case 0xC4:		/* date */
	   {
	      double    d;

	      unsigned char buf[8];

	      int       j;

	      d = dp->u.d;
	      for (j = 0; j < 8; ++j)
		 buf[j] = ((unsigned char *) &d)[j];
	      if (fwrite(buf, 8, 1, file) != 1)
	       {
		  line = __LINE__;
		  goto writerr;
	       }
	   }
	   break;
	}
    }

 ret:
   fclose(file);

   return r;
}

int
clip___MCLEAR(ClipMachine * ClipMachineMemory)
{
   return 0;
}

/* cName, ... */
int
clip___MXRELEASE(ClipMachine * ClipMachineMemory)
{
   return 0;
}

/* cPattern, bLike */
int
clip___MRELEASE(ClipMachine * ClipMachineMemory)
{
   return 0;
}

/* cFilename, bAdditive */
int
clip___MRESTORE(ClipMachine * ClipMachineMemory)
{
   char     *path = _clip_parc(ClipMachineMemory, 1);

  /*
     int l_add  = _clip_parl(ClipMachineMemory, 2);
   */

  /*int isAdditive = _clip_parl(ClipMachineMemory, 1); */
   char      buf[256], errbuf[256], p[256];

   int       r, i;

   DbfData  *vars = 0;

   char    **names = 0;

   int       nvars = 0;

   char     *e, *s;

   strncpy(p, path, sizeof(p));
   s = strrchr(p, '/');
   e = strrchr(p, '\\');
   s = (s > e) ? s : e;
   e = strrchr(p, '.');
   if (!e || (s && e < s))
      strcat(p, ".mem");
   _clip_path(ClipMachineMemory, p, buf, sizeof(buf), 0);

   r = read_MemFile(buf, dbf_get_locale(ClipMachineMemory), &vars, &names, &nvars, errbuf, sizeof(errbuf));

   if (!r)
    {
       for (i = 0; i < nvars; i++)
	{
	   ClipVar  *vp, tmp;

	   char     *s;

	   long      hash = _clip_casehashstr(names[i]);

	   s = strdup(names[i]);
	   HashTable_store(ClipMachineMemory->hashnames, s, hash);

	   vp = _clip_ref_public_noadd(ClipMachineMemory, hash);
	   if (!vp)
	      vp = _clip_ref_memvar(ClipMachineMemory, hash);

	   if (!vp)
	      continue;
#if 0				/* 2006/11/21 restore in getsys bug fixed */
	   _clip_destroy(ClipMachineMemory, vp);
	   dbf2clip(vars + i, vp);
#else
	   dbf2clip(vars + i, &tmp);
	   if (_clip_push(ClipMachineMemory, &tmp))
	      continue;
	   _clip_assign(ClipMachineMemory, vp);
#endif
	}
    }

   for (i = 0; i < nvars; i++)
    {
       destroy_DbfData(vars + i);
       free(names[i]);
    }

   free(names);
   free(vars);

   if (r)
    {
       _clip_trap_str(ClipMachineMemory, ClipMachineMemory->fp->filename_of_ClipFrame,
		      ClipMachineMemory->fp->line_of_ClipFrame, errbuf);
       return EG_ARG;
    }

   return 0;
}

/* cFilename, cPattern, bLike */
int
clip___MSAVE(ClipMachine * ClipMachineMemory)
{
   char     *path = _clip_parc(ClipMachineMemory, 1);

   char     *pattern = _clip_parc(ClipMachineMemory, 2);

   int       isLike = _clip_parl(ClipMachineMemory, 3);

   char      buf[256], errbuf[64];

   int       r, i;

   DbfData  *vars = 0;

   ClipFrame *fp;

   Coll      snames;

   char     *e, *s;

   if (!path || !pattern)
      return EG_ARG;

   init_Coll(&snames, 0, strcasecmp);

   for (fp = ClipMachineMemory->fp; fp; fp = fp->ClipFrame_up_of_ClipFrame)
    {
       if (fp->names_of_ClipFrame)
	{
	   ClipHashBucket *bp;

	   for (i = 0; i < fp->names_of_ClipFrame->num_of_ClipHashNames; i++)
	    {
	       char     *nm;

	       int       ind;

	       bp = fp->names_of_ClipFrame->buckets_of_ClipHashNames + i;
	       nm = fp->names_of_ClipFrame->mem_of_ClipHashNames + bp->offs_of_ClipHashBucket;
	       if (!search_Coll(&snames, nm, &ind))
		{
		   int       m = (_clip_glob_match(nm, pattern, 1) == -1 ? 0 : 1);

		   if ((m && isLike) || (!m && !isLike))
		      atInsert_Coll(&snames, nm, ind);
		}
	    }
	}
    }

   for (r = HashTable_first(ClipMachineMemory->hashnames); r; r = HashTable_next(ClipMachineMemory->hashnames))
    {
       char     *nm = (char *) HashTable_current(ClipMachineMemory->hashnames);

       int       ind;

       if (!search_Coll(&snames, nm, &ind))
	{
	   int       m = (_clip_glob_match(nm, pattern, 1) == -1 ? 0 : 1);

	   if ((m && isLike) || (!m && !isLike))
	      atInsert_Coll(&snames, nm, ind);
	}
    }

   vars = NEWVECT(DbfData, snames.count);

   for (i = 0; i < snames.count; i++)
    {
       long      hash;

       ClipVar  *vp;

       hash = _clip_casehashstr(snames.items[i]);

       vp = _clip_ref_memvar(ClipMachineMemory, hash);
       clip2dbf(vp, vars + i);
    }

   _clip_path(ClipMachineMemory, path, buf, sizeof(buf), 1);

   e = strrchr(buf, '.');
   s = strrchr(buf, '/');
   if (!e || (s && e < s))
      strncat(buf, ".mem", sizeof(buf));

   r = write_MemFile(buf, dbf_get_locale(ClipMachineMemory),
		     vars, (char **) snames.items, snames.count, errbuf, sizeof(errbuf));

   for (i = 0; i < snames.count; i++)
      destroy_DbfData(vars + i);

   destroy_Coll(&snames);
   free(vars);

   if (r)
    {
       _clip_trap_str(ClipMachineMemory, ClipMachineMemory->fp->filename_of_ClipFrame,
		      ClipMachineMemory->fp->line_of_ClipFrame, errbuf);
       return EG_ARG;
    }

   return 0;
}

int
clip_MEMVARLIST(ClipMachine * ClipMachineMemory)
{
   ClipVar  *rp = RETPTR(ClipMachineMemory);

   char     *pattern = _clip_parc(ClipMachineMemory, 1);

   int       publ = _clip_parl(ClipMachineMemory, 2);

   int       isLike = _clip_parl(ClipMachineMemory, 3);

   int       all = _clip_parinfo(ClipMachineMemory, 2) != LOGICAL_type_of_ClipVarType;

   ClipFrame *fp;

   Coll      snames;

   int       r, i;

   int       isPrivate, isPublic;
   long      dims[] = { 0 };
   ClipVar   vp;

   if (!pattern)
      pattern = "*";

   init_Coll(&snames, 0, strcasecmp);

   for (fp = ClipMachineMemory->fp; fp; fp = fp->ClipFrame_up_of_ClipFrame)
    {
       if (fp->names_of_ClipFrame)
	{
	   ClipHashBucket *bp;

	   for (i = 0; i < fp->names_of_ClipFrame->num_of_ClipHashNames; i++)
	    {
	       char     *nm;

	       int       ind;

	       bp = fp->names_of_ClipFrame->buckets_of_ClipHashNames + i;
	       nm = fp->names_of_ClipFrame->mem_of_ClipHashNames + bp->offs_of_ClipHashBucket;
	       if (!search_Coll(&snames, nm, &ind))
		{
		   int       m = (_clip_glob_match(nm, pattern, 1) == -1 ? 0 : 1);

		   if ((m && !isLike) || (!m && isLike))
		      atInsert_Coll(&snames, nm, ind);
		}
	    }
	}
    }

   for (r = HashTable_first(ClipMachineMemory->hashnames); r; r = HashTable_next(ClipMachineMemory->hashnames))
    {
       char     *nm = (char *) HashTable_current(ClipMachineMemory->hashnames);

       int       ind;

       if (!search_Coll(&snames, nm, &ind))
	{
	   int       m = (_clip_glob_match(nm, pattern, 1) == -1 ? 0 : 1);

	   if ((m && !isLike) || (!m && isLike))
	      atInsert_Coll(&snames, nm, ind);
	}
    }

   _clip_array(ClipMachineMemory, rp, 1, dims);

   for (i = 0; i < snames.count; i++)
    {
       long      hash = _clip_casehashstr(snames.items[i]);

       isPublic = !_clip_is_public(ClipMachineMemory, hash);
       isPrivate = !_clip_is_private(ClipMachineMemory, hash);

       if (all)
	  r = isPublic || isPrivate;
       else if (publ)
	  r = isPublic;
       else
	  r = isPrivate;
       if (r)
	{
	   _clip_var_str(snames.items[i], strlen(snames.items[i]), &vp);
	   _clip_aadd(ClipMachineMemory, rp, &vp);
	   _clip_destroy(ClipMachineMemory, &vp);
	}
    }
   destroy_Coll(&snames);
   return 0;
}
