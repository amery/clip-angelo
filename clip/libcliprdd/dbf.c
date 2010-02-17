/*
	Copyright (C) 2001  ITK
	Author  : Rushad Nabiullin <rust@itk.ru>
	License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI
*/
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <ctype.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <limits.h>
#include "ci_rdd.h"
#include "ci_btree.h"
#include "ci_error.ch"
#include "ci_dbinfo.ch"
#ifdef HAVE_MMAN_H
#include <sys/mman.h>
#endif

#include "ci_ncp.h"

#ifdef USE_NCPFS
#define fcntl ncp_fcntl
#endif

typedef struct _DBF_HEADER_
{
   unsigned char version;
   unsigned char yy;
   unsigned char mm;
   unsigned char dd;
   unsigned char recs[4];
   unsigned char hdrsize[2];
   unsigned char recsize[2];
   unsigned char reserved1[2];
   unsigned char trans;
   unsigned char encrypted;
   unsigned char thread[4];
   unsigned char reserved2[8];
   unsigned char mdx;
   unsigned char lang;
   unsigned char reserved3[2];
} DBF_HEADER;

typedef struct _DBF_FIELD_
{
   char name[11];
   char type;
   unsigned char offs[4];
   union
   {
      char len[2];
      struct
      {
	 char len;
	 char dec;
      } num;
   } len;
   char flags;
   char flags2;
   char waid;
   char reserved2[2];
   char setfields;
   char reserved3[7];
   char indexed;
} DBF_FIELD;

static RDD_DATA_VTBL *dbf_vtbl();
static RDD_DATA_VTBL *vfp_vtbl();
static RDD_DATA_VTBL *fox_vtbl();

static int dbf_open(ClipMachine * ClipMemoryStore, RDD_DATA * rd, const char *__PROC__);
static int dbf_create(ClipMachine * ClipMemoryStore, RDD_DATA_VTBL * vtbl, char *name, RDD_STRUCT * rddstru, int nfields, const char *__PROC__);
static int dbf_pack(ClipMachine * ClipMemoryStore, RDD_DATA * rd, int tfd, const char *__PROC__);
static int dbf_zap(ClipMachine * ClipMemoryStore, RDD_DATA * rd, const char *__PROC__);
static int dbf_close(ClipMachine * ClipMemoryStore, RDD_DATA * rd, const char *__PROC__);
static int dbf_lastrec(ClipMachine * ClipMemoryStore, RDD_DATA * rd, int *lastrec, const char *__PROC__);
static int dbf_deleted(ClipMachine * ClipMemoryStore, RDD_DATA * rd, int *deleted, const char *__PROC__);
static int dbf_gotop(ClipMachine * ClipMemoryStore, RDD_DATA * rd, const char *__PROC__);
static int dbf_gobottom(ClipMachine * ClipMemoryStore, RDD_DATA * rd, const char *__PROC__);
static int dbf_next(ClipMachine * ClipMemoryStore, RDD_DATA * rd, int filtok, const char *__PROC__);
static int dbf_prev(ClipMachine * ClipMemoryStore, RDD_DATA * rd, const char *__PROC__);
static int dbf_goto(ClipMachine * ClipMemoryStore, RDD_DATA * rd, unsigned int rec, const char *__PROC__);
static int dbf_rawgoto(ClipMachine * ClipMemoryStore, RDD_DATA * rd, unsigned int rec, char valid_stack, const char *__PROC__);

static int dbf_rlock(ClipMachine * ClipMemoryStore, RDD_DATA * rd, unsigned int rec, int *r, const char *__PROC__);
static int dbf_flock(ClipMachine * ClipMemoryStore, RDD_DATA * rd, int *r, const char *__PROC__);
static int dbf_ulock(ClipMachine * ClipMemoryStore, RDD_DATA * rd, unsigned int rec, int flock, const char *__PROC__);
static int dbf_rlocked(ClipMachine * ClipMemoryStore, RDD_DATA * rd, unsigned int rec, int *r, const char *__PROC__);
static int dbf_forlock(ClipMachine * ClipMemoryStore, RDD_DATA * rd, unsigned int rec, int *r, const char *__PROC__);
static int dbf__rlock(ClipMachine * ClipMemoryStore, RDD_DATA * rd, const char *__PROC__);
static int dbf__wlock(ClipMachine * ClipMemoryStore, RDD_DATA * rd, const char *__PROC__);
static int dbf__ulock(ClipMachine * ClipMemoryStore, RDD_DATA * rd, const char *__PROC__);

static int dbf_append(ClipMachine * ClipMemoryStore, RDD_DATA * rd, const char *__PROC__);
static int dbf_getrecord(ClipMachine * ClipMemoryStore, RDD_DATA * rd, const char *__PROC__);
static int dbf_getvalue(ClipMachine * ClipMemoryStore, RDD_DATA * rd, int no, ClipVar * vp, const char *__PROC__);
static int dbf_setrecord(ClipMachine * ClipMemoryStore, RDD_DATA * rd, const char *__PROC__);
static int dbf_setvalue(ClipMachine * ClipMemoryStore, RDD_DATA * rd, int no, ClipVar * vp, int binary, const char *__PROC__);
static int dbf_getmemoid(ClipMachine * ClipMemoryStore, RDD_DATA * rd, int no, unsigned int *memoid, unsigned short int *vlen, const char *__PROC__);
static int dbf_setmemoid(ClipMachine * ClipMemoryStore, RDD_DATA * rd, int no, unsigned int memoid, const char *__PROC__);
static int dbf_delete(ClipMachine * ClipMemoryStore, RDD_DATA * rd, const char *__PROC__);
static int dbf_recall(ClipMachine * ClipMemoryStore, RDD_DATA * rd, const char *__PROC__);

static int dbf_lupdate(ClipMachine * ClipMemoryStore, RDD_DATA * rd, const char *__PROC__);
static int dbf_info(ClipMachine * ClipMemoryStore, RDD_DATA * rd, int cmd, const char *__PROC__);
static int dbf_setstruct(ClipMachine * ClipMemoryStore, RDD_DATA * rd, const char *__PROC__);

static int dbf_calcfiltlist(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_FILTER * fp, const char *__PROC__);

static int vfp_create(ClipMachine * ClipMemoryStore, RDD_DATA_VTBL * vtbl, char *name, RDD_STRUCT * rddstru, int nfields, const char *__PROC__);

static int dbf_verify(ClipMachine * ClipMemoryStore, RDD_DATA * rd, int *r, const char *__PROC__);

#define  er_unlock          _clip_gettext("Unlock error")
#define  er_readlock        _clip_gettext("Shared lock error")
#define  er_writelock       _clip_gettext("Exclusive lock error")
#define  er_badheader       _clip_gettext("Table header corrupted")
#define  er_badstructure    _clip_gettext("Bad table structure")
#define  er_fielduplicate	_clip_gettext("Duplicate field name")
#define  er_notpermitted    _clip_gettext("Operation not permitted")
#define  er_corruption      _clip_gettext("Corruption detected")
#define  er_nomemo          _clip_gettext("Memo file not opened")
#define  er_baddata         _clip_gettext("Bad data")
#define  er_type            _clip_gettext("Type mismatch")
#define  er_ioerror         _clip_gettext("I/O error")

int
clip_INIT_DBF(ClipMachine * ClipMemoryStore)
{
   rdd_registerdatadriver(ClipMemoryStore, dbf_vtbl());
   return 0;
}

int
clip_INIT_VFP(ClipMachine * ClipMemoryStore)
{
   rdd_registerdatadriver(ClipMemoryStore, vfp_vtbl());
   return 0;
}

int
clip_INIT_FOX(ClipMachine * ClipMemoryStore)
{
   rdd_registerdatadriver(ClipMemoryStore, fox_vtbl());
   return 0;
}

static RDD_DATA_VTBL *
dbf_vtbl()
{
   RDD_DATA_VTBL *vtbl = (RDD_DATA_VTBL *) malloc(sizeof(RDD_DATA_VTBL));

   memset(vtbl, 0, sizeof(RDD_DATA_VTBL));
   strcpy(vtbl->id, "DBF");
   strcpy(vtbl->suff, ".dbf");
   strcpy(vtbl->desc, "Generic DBF data files driver v0.0.1 (c) 2001 Copyright ITK Ltd.");
   vtbl->dbfsig = 0x03;
   vtbl->dbfwithmemosig = 0x83;

   vtbl->open = dbf_open;
   vtbl->create = dbf_create;
   vtbl->pack = dbf_pack;
   vtbl->zap = dbf_zap;
   vtbl->close = dbf_close;
   vtbl->lastrec = dbf_lastrec;
   vtbl->deleted = dbf_deleted;
   vtbl->gotop = dbf_gotop;
   vtbl->gobottom = dbf_gobottom;
   vtbl->next = dbf_next;
   vtbl->prev = dbf_prev;
   vtbl->go = dbf_goto;
   vtbl->rawgo = dbf_rawgoto;
   vtbl->append = dbf_append;
   vtbl->getrecord = dbf_getrecord;
   vtbl->getvalue = dbf_getvalue;
   vtbl->setrecord = dbf_setrecord;
   vtbl->setvalue = dbf_setvalue;
   vtbl->getmemoid = dbf_getmemoid;
   vtbl->setmemoid = dbf_setmemoid;
   vtbl->delete = dbf_delete;
   vtbl->recall = dbf_recall;
   vtbl->rlock = dbf_rlock;
   vtbl->flock = dbf_flock;
   vtbl->ulock = dbf_ulock;
   vtbl->rlocked = dbf_rlocked;
   vtbl->forlock = dbf_forlock;
   vtbl->_rlock = dbf__rlock;
   vtbl->_wlock = dbf__wlock;
   vtbl->_ulock = dbf__ulock;
   vtbl->lupdate = dbf_lupdate;
   vtbl->info = dbf_info;
   vtbl->setstruct = dbf_setstruct;
   vtbl->verify = dbf_verify;
   vtbl->calcfiltlist = dbf_calcfiltlist;
   return vtbl;
}

static RDD_DATA_VTBL *
vfp_vtbl()
{
   RDD_DATA_VTBL *vtbl = (RDD_DATA_VTBL *) malloc(sizeof(RDD_DATA_VTBL));

   memset(vtbl, 0, sizeof(RDD_DATA_VTBL));
   strcpy(vtbl->id, "VFP");
   strcpy(vtbl->suff, ".dbf");
   strcpy(vtbl->desc, "Generic Visial FoxPro data files driver v0.0.1 (c) 2002 Copyright ITK Ltd.");
   vtbl->dbfsig = 0x30;
   vtbl->dbfwithmemosig = 0x30;

   vtbl->open = dbf_open;
   vtbl->create = vfp_create;
   vtbl->pack = dbf_pack;
   vtbl->zap = dbf_zap;
   vtbl->close = dbf_close;
   vtbl->lastrec = dbf_lastrec;
   vtbl->deleted = dbf_deleted;
   vtbl->gotop = dbf_gotop;
   vtbl->gobottom = dbf_gobottom;
   vtbl->next = dbf_next;
   vtbl->prev = dbf_prev;
   vtbl->go = dbf_goto;
   vtbl->rawgo = dbf_rawgoto;
   vtbl->append = dbf_append;
   vtbl->getrecord = dbf_getrecord;
   vtbl->getvalue = dbf_getvalue;
   vtbl->setrecord = dbf_setrecord;
   vtbl->setvalue = dbf_setvalue;
   vtbl->getmemoid = dbf_getmemoid;
   vtbl->setmemoid = dbf_setmemoid;
   vtbl->delete = dbf_delete;
   vtbl->recall = dbf_recall;
   vtbl->rlock = dbf_rlock;
   vtbl->flock = dbf_flock;
   vtbl->ulock = dbf_ulock;
   vtbl->rlocked = dbf_rlocked;
   vtbl->forlock = dbf_forlock;
   vtbl->_rlock = dbf__rlock;
   vtbl->_wlock = dbf__wlock;
   vtbl->_ulock = dbf__ulock;
   vtbl->lupdate = dbf_lupdate;
   vtbl->info = dbf_info;
   vtbl->setstruct = dbf_setstruct;
   vtbl->verify = dbf_verify;
   vtbl->calcfiltlist = dbf_calcfiltlist;
   return vtbl;
}

static RDD_DATA_VTBL *
fox_vtbl()
{
   RDD_DATA_VTBL *vtbl = (RDD_DATA_VTBL *) malloc(sizeof(RDD_DATA_VTBL));

   memset(vtbl, 0, sizeof(RDD_DATA_VTBL));
   strcpy(vtbl->id, "FOX");
   strcpy(vtbl->suff, ".dbf");
   strcpy(vtbl->desc, "Generic FoxPro data files driver v0.0.1 (c) 2002 Copyright ITK Ltd.");
   vtbl->dbfsig = 0x03;
   vtbl->dbfwithmemosig = 0xF5;

   vtbl->open = dbf_open;
   vtbl->create = dbf_create;
   vtbl->pack = dbf_pack;
   vtbl->zap = dbf_zap;
   vtbl->close = dbf_close;
   vtbl->lastrec = dbf_lastrec;
   vtbl->deleted = dbf_deleted;
   vtbl->gotop = dbf_gotop;
   vtbl->gobottom = dbf_gobottom;
   vtbl->next = dbf_next;
   vtbl->prev = dbf_prev;
   vtbl->go = dbf_goto;
   vtbl->rawgo = dbf_rawgoto;
   vtbl->append = dbf_append;
   vtbl->getrecord = dbf_getrecord;
   vtbl->getvalue = dbf_getvalue;
   vtbl->setrecord = dbf_setrecord;
   vtbl->setvalue = dbf_setvalue;
   vtbl->getmemoid = dbf_getmemoid;
   vtbl->setmemoid = dbf_setmemoid;
   vtbl->delete = dbf_delete;
   vtbl->recall = dbf_recall;
   vtbl->rlock = dbf_rlock;
   vtbl->flock = dbf_flock;
   vtbl->ulock = dbf_ulock;
   vtbl->rlocked = dbf_rlocked;
   vtbl->forlock = dbf_forlock;
   vtbl->_rlock = dbf__rlock;
   vtbl->_wlock = dbf__wlock;
   vtbl->_ulock = dbf__ulock;
   vtbl->lupdate = dbf_lupdate;
   vtbl->info = dbf_info;
   vtbl->setstruct = dbf_setstruct;
   vtbl->verify = dbf_verify;
   vtbl->calcfiltlist = dbf_calcfiltlist;
   return vtbl;
}

static int
dbf_rlock(ClipMachine * ClipMemoryStore, RDD_DATA * rd, unsigned int rec, int *r, const char *__PROC__)
{
   *r = 1;
   if (rd->shared)
      {
	 int i;

	 for (i = 0; i < rd->nlocks; i++)
	    {
	       if (rd->locks[i] == rec)
		  return 0;
	    }
	 if (!rd->flocked)
	    {
	       if (_clip_setlock(ClipMemoryStore, rd->file.filehash, rd->file.fd,
				 (rd->lockstyle ? 0x10000000 : 1000000000) + rec, (rd->readonly ? CLIP_LOCK_READ : CLIP_LOCK_WRITE) | CLIP_LOCK_HILEVEL))

		  *r = 0;

	       if (*r)
		  {
		     rd->locks = realloc(rd->locks, (rd->nlocks + 1) * sizeof(unsigned int));
		     rd->locks[rd->nlocks] = rec;
		     rd->nlocks++;
		  }
	    }
      }
   return 0;
}

static int
dbf_flock(ClipMachine * ClipMemoryStore, RDD_DATA * rd, int *r, const char *__PROC__)
{
   int er;

   *r = 1;
   if (rd->shared)
      {
	 int i;

	 if (rd->flocked)
	    return 0;
	 for (i = 0; i < rd->nlocks; i++)
	    if ((er = dbf_ulock(ClipMemoryStore, rd, rd->locks[i], 0, __PROC__)))
	       return er;

	 if (_clip_setlock(ClipMemoryStore, rd->file.filehash, rd->file.fd, (rd->lockstyle ? 0x10000000 : 1000000000) + 1, CLIP_LOCK_WRITE | CLIP_LOCK_FLOCK | CLIP_LOCK_HILEVEL))

	    *r = 0;

	 if (*r)
	    rd->flocked = 1;
      }
   return 0;
}

static int
dbf_ulock(ClipMachine * ClipMemoryStore, RDD_DATA * rd, unsigned int rec, int flock, const char *__PROC__)
{
   if (rd->shared)
      {
	 int i;

	 if (rec)
	    {
	       for (i = 0; i < rd->nlocks; i++)
		  {
		     if (rd->locks[i] == rec)
			break;
		  }
	       if (i == rd->nlocks)
		  return 0;
	       if (!rd->flocked)
		  {
		     if (_clip_unlock(ClipMemoryStore, rd->file.filehash, rd->file.fd, (rd->lockstyle ? 0x10000000 : 1000000000) + rec, 0))

			return rdd_err(ClipMemoryStore, EG_LOCK, errno, __FILE__, __LINE__, __PROC__, er_unlock);
		  }
	       rd->nlocks--;
	       for (; i < rd->nlocks; i++)
		  rd->locks[i] = rd->locks[i + 1];
	       rd->locks = realloc(rd->locks, rd->nlocks * sizeof(unsigned int));
	    }
	 else
	    {
	       if (rd->flocked)
		  {
		     if (flock)
			{
			   if (_clip_unlock(ClipMemoryStore, rd->file.filehash, rd->file.fd, (rd->lockstyle ? 0x10000000 : 1000000000) + 1, CLIP_LOCK_FLOCK))

			      return rdd_err(ClipMemoryStore, EG_LOCK, errno, __FILE__, __LINE__, __PROC__, er_unlock);

			   rd->flocked = 0;
			   rd->locks = realloc(rd->locks, 0);
			   rd->nlocks = 0;
			}
		  }
	       else
		  {
		     for (i = 0; i < rd->nlocks; i++)
			{
			   if (_clip_unlock(ClipMemoryStore, rd->file.filehash, rd->file.fd, (rd->lockstyle ? 0x10000000 : 1000000000) + rd->locks[i], 0))

			      return rdd_err(ClipMemoryStore, EG_LOCK, errno, __FILE__, __LINE__, __PROC__, er_unlock);
			}
		     rd->locks = realloc(rd->locks, 0);
		     rd->nlocks = 0;
		  }
	    }
      }
   return 0;
}

static int
dbf_rlocked(ClipMachine * ClipMemoryStore, RDD_DATA * rd, unsigned int rec, int *r, const char *__PROC__)
{
   int i;

   *r = 1;
   if (!rd->shared)
      return 0;
   for (i = 0; i < rd->nlocks; i++)
      {
	 if (rd->locks[i] == rec)
	    break;
      }
   if (i == rd->nlocks)
      *r = 0;
   return 0;
}

static int
dbf_forlock(ClipMachine * ClipMemoryStore, RDD_DATA * rd, unsigned int rec, int *r, const char *__PROC__)
{
   *r = _clip_setlock(ClipMemoryStore, rd->file.filehash, rd->file.fd, (rd->lockstyle ? 0x10000000 : 1000000000) + rec, CLIP_LOCK_WRITE | CLIP_LOCK_HILEVEL);
   if (!*r)
      {
	 if (_clip_unlock(ClipMemoryStore, rd->file.filehash, rd->file.fd, (rd->lockstyle ? 0x10000000 : 1000000000) + rec, 0))

	    return rdd_err(ClipMemoryStore, EG_LOCK, errno, __FILE__, __LINE__, __PROC__, er_unlock);
      }
   return 0;
}

static int
dbf__rlock(ClipMachine * ClipMemoryStore, RDD_DATA * rd, const char *__PROC__)
{
   if (rd->shared)
      {
	 if (!rd->locked)
	    {
	       if (_clip_setlock(ClipMemoryStore, rd->file.filehash, rd->file.fd, rd->lockstyle ? 0x10000000 : 1000000000, CLIP_LOCK_READ | CLIP_LOCK_LOWLEVEL | CLIP_LOCK_WAIT))

		  return rdd_err(ClipMemoryStore, EG_LOCK, errno, __FILE__, __LINE__, __PROC__, er_readlock);
	    }
	 rd->locked++;
      }
   rd->file.rlocked = 1;
   return 0;
}

static int
dbf__wlock(ClipMachine * ClipMemoryStore, RDD_DATA * rd, const char *__PROC__)
{
   if (rd->shared)
      {
	 if (!rd->locked)
	    {
	       if (_clip_setlock(ClipMemoryStore, rd->file.filehash, rd->file.fd, rd->lockstyle ? 0x10000000 : 1000000000, CLIP_LOCK_WRITE | CLIP_LOCK_LOWLEVEL | CLIP_LOCK_WAIT))

		  return rdd_err(ClipMemoryStore, EG_LOCK, errno, __FILE__, __LINE__, __PROC__, er_writelock);
	    }
	 rd->locked++;
      }
   rd->file.wlocked = 1;
   return 0;
}

static int
dbf__ulock(ClipMachine * ClipMemoryStore, RDD_DATA * rd, const char *__PROC__)
{
   if (rd->shared)
      {
	 if (rd->locked == 1)
	    {
	       if (_clip_unlock(ClipMemoryStore, rd->file.filehash, rd->file.fd, rd->lockstyle ? 0x10000000 : 1000000000, 0))

		  return rdd_err(ClipMemoryStore, EG_LOCK, errno, __FILE__, __LINE__, __PROC__, er_unlock);
	    }
	 rd->locked--;
      }
   rd->file.rlocked = 0;
   rd->file.wlocked = 0;
   return 0;
}

static int
dbf_open(ClipMachine * ClipMemoryStore, RDD_DATA * rd, const char *__PROC__)
{
   DBF_HEADER hdr;
   DBF_FIELD field;
   int i;
   int offs = 1;
   char *s;
   int er;

   if ((er = rdd_read(ClipMemoryStore, &rd->file, 0, sizeof(DBF_HEADER), (char *) &hdr, __PROC__)))
      return er;
   rd->hdrsize = _rdd_ushort(hdr.hdrsize);
   rd->recsize = _rdd_ushort(hdr.recsize);
   rd->record = calloc(1, rd->recsize + 1);
   rd->recno = 0;
   rd->bof = rd->v_bof = 0;
   rd->eof = 0;

   if ((hdr.version != 0x03) && (hdr.version != 0x83) && (hdr.version != 0xF5) && (hdr.version != 0x30))
      return rdd_err(ClipMemoryStore, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_badheader);

   rd->sig = hdr.version;

   if (rd->sig == 0x30)		/* VFP */
      rd->nfields = (rd->hdrsize - 263 - sizeof(DBF_HEADER)) / sizeof(DBF_FIELD);
   else
      rd->nfields = (rd->hdrsize - sizeof(DBF_HEADER)) / sizeof(DBF_FIELD);
   rd->fields = (RDD_FIELD *) calloc(rd->nfields, sizeof(RDD_FIELD));
   for (i = 0; i < rd->nfields; i++)
      {
	 if ((er = rdd_read(ClipMemoryStore, &rd->file, sizeof(DBF_HEADER) + sizeof(DBF_FIELD) * i, sizeof(DBF_FIELD), (char *) &field, __PROC__)))
	    return er;
	 if (field.type == '0')
	    {
	       rd->nulloffs = offs;
	       rd->nfields--;
	       rd->fields = (RDD_FIELD *) realloc(rd->fields, sizeof(RDD_FIELD) * rd->nfields);
	       break;
	    }
	 rd->fields[i].type = field.type;
	 strcpy(rd->fields[i].name, field.name);
	 s = rd->fields[i].name + strlen(rd->fields[i].name);
	 while (((--s) >= rd->fields[i].name) && *s == ' ')
	    *s = 0;
	 loc_read(rd->loc, (unsigned char *) (rd->fields[i].name), strlen(rd->fields[i].name));
	 if (strchr("NBFYX", field.type))
	    {
	       if (field.type == 'X')
		  rd->x = 1;
	       rd->fields[i].len = field.len.num.len;
	       rd->fields[i].dec = field.len.num.dec;
	    }
	 else if (field.type == 'C')
	    {
	       rd->fields[i].len = _rdd_ushort((unsigned char *) (field.len.len));
	       rd->fields[i].dec = 0;
	    }
	 else
	    {
	       rd->fields[i].len = field.len.num.len;
	       rd->fields[i].dec = 0;
	    }
	 if (rd->sig == 0x30)
	    {			/* VFP */
	       rd->fields[i].binary = (field.flags & 0x04) != 0;
	       if (rd->fields[i].type == 'P')
		  rd->fields[i].binary = 1;
	       rd->fields[i].nullable = (field.flags & 0x02) != 0;
	    }
	 if (strchr("PG", rd->fields[i].type))
	    rd->fields[i].binary = 1;
	 rd->fields[i].offs = offs;
	 offs += rd->fields[i].len;
	 if (!HashTable_insert(rd->hashes, &rd->fields[i], _clip_casehashword(rd->fields[i].name, strlen(rd->fields[i].name))))
	    {
	       return rdd_err(ClipMemoryStore, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_badheader);
	    }
      }
   return 0;
}

static int
dbf_create(ClipMachine * ClipMemoryStore, RDD_DATA_VTBL * vtbl, char *name, RDD_STRUCT * stru, int nfields, const char *__PROC__)
{
   RDD_FILE file;
   DBF_HEADER *hdr = malloc(sizeof(DBF_HEADER));
   char *chdr, *s;
   DBF_FIELD *fld;
   int i, offs;
   time_t ltm;
   struct tm *date;
   HashTable *hashes;
   int er;
   unsigned int recsize;

   memset(hdr, 0, sizeof(DBF_HEADER));
   hdr->version = vtbl->dbfsig;
   for (i = 0; i < nfields; i++)
      if (strchr("MPG", stru[i].type) || (stru[i].type == 'V' && stru[i].len != 3 && stru[i].len != 4))
	 {
	    hdr->version = vtbl->dbfwithmemosig;
	    break;
	 }
   ltm = time(NULL);
   date = gmtime(&ltm);
   hdr->yy = date->tm_year;
   hdr->mm = date->tm_mon + 1;
   hdr->dd = date->tm_mday;
   _rdd_put_uint(hdr->recs, 0);
   _rdd_put_ushort(hdr->hdrsize, sizeof(DBF_HEADER) + nfields * sizeof(DBF_FIELD) + 2);
   recsize = 1;
   hdr = realloc(hdr, _rdd_ushort(hdr->hdrsize) + 1);
   hashes = new_HashTable();
   offs = 1;
   for (i = 0; i < nfields; i++)
      {
	 recsize += stru[i].len;
	 fld = (DBF_FIELD *) ((char *) hdr + sizeof(DBF_HEADER) + i * sizeof(DBF_FIELD));
	 memset(fld, 0, sizeof(DBF_FIELD));
	 strncpy(fld->name, stru[i].name, 10);
	 for (s = fld->name; *s && *s != ' '; s++);
	 *s = 0;
	 loc_write(dbf_get_locale(ClipMemoryStore), (unsigned char *) (fld->name), strlen(fld->name));
	 fld->type = stru[i].type;
	 if (stru[i].type == 'V' && stru[i].len != 3 && stru[i].len != 4 && stru[i].len < 6)
	    {
	       er = rdd_err(ClipMemoryStore, EG_CREATE, 0, __FILE__, __LINE__, __PROC__, er_badstructure);
	       goto err;
	    }
	 _rdd_put_uint(fld->offs, offs);
	 offs += stru[i].len;
	 if (fld->type == 'N')
	    {
	       if (stru[i].dec >= stru[i].len - (stru[i].dec != 0))
		  {
		     er = rdd_err(ClipMemoryStore, EG_CREATE, 0, __FILE__, __LINE__, __PROC__, er_badstructure);
		     goto err;
		  }
	       fld->len.num.len = stru[i].len;
	       fld->len.num.dec = stru[i].dec;
	    }
	 else if (fld->type == 'X')
	    {
	       if (stru[i].len < 10 || stru[i].len > 127)
		  {
		     er = rdd_err(ClipMemoryStore, EG_CREATE, 0, __FILE__, __LINE__, __PROC__, er_badstructure);
		     goto err;
		  }
	       fld->len.num.len = stru[i].len;
	       fld->len.num.dec = stru[i].dec;
	    }
	 else
	    {
	       _rdd_put_ushort((unsigned char *) (fld->len.len), stru[i].len);
	    }
	 if (!HashTable_insert(hashes, fld, _clip_casehashword(fld->name, strlen(fld->name))))
	    {
	       er = rdd_err(ClipMemoryStore, EG_CREATE, 0, __FILE__, __LINE__, __PROC__, er_fielduplicate);
	       goto err;
	    }
      }
   if (recsize > 0xffff)
      {
	 er = rdd_err(ClipMemoryStore, EG_CREATE, 0, __FILE__, __LINE__, __PROC__, er_badstructure);
	 goto err;
      }
   _rdd_put_ushort(hdr->recsize, recsize);
   delete_HashTable(hashes);

   chdr = (char *) hdr;
   chdr[_rdd_ushort(hdr->hdrsize) - 2] = 0x0D;
   chdr[_rdd_ushort(hdr->hdrsize) - 1] = 0x00;
   chdr[_rdd_ushort(hdr->hdrsize)] = 0x1A;

   memset(&file, 0, sizeof(RDD_FILE));
   file.md = (char *) -1;
   file.fd = _clip_creat(ClipMemoryStore, name, O_RDWR, ClipMemoryStore->fileCreateMode, 1);
   if (file.fd == -1)
      goto err_create;
   if ((er = rdd_write(ClipMemoryStore, &file, 0, _rdd_ushort(hdr->hdrsize) + 1, (char *) hdr, __PROC__)))
      goto err;
   if (_clip_close(ClipMemoryStore, _clip_hashstr(name), file.fd) == -1)
      goto err_create;

   free(hdr);
   return 0;
 err_create:
   er = rdd_err(ClipMemoryStore, EG_CREATE, errno, __FILE__, __LINE__, __PROC__, name);
 err:
   free(hdr);
   return er;
}

static int
vfp_create(ClipMachine * ClipMemoryStore, RDD_DATA_VTBL * vtbl, char *name, RDD_STRUCT * stru, int nfields, const char *__PROC__)
{
   RDD_FILE file;
   DBF_HEADER *hdr = malloc(sizeof(DBF_HEADER));
   char *chdr, *s;
   DBF_FIELD *fld;
   int i, offs;
   time_t ltm;
   struct tm *date;
   HashTable *hashes;
   int nulls = 0, er;

   memset(hdr, 0, sizeof(DBF_HEADER));
   hdr->version = vtbl->dbfsig;
   for (i = 0; i < nfields; i++)
      if (strchr("MPG", stru[i].type) || (stru[i].type == 'V' && stru[i].len != 3 && stru[i].len != 4))
	 {
	    hdr->version = vtbl->dbfwithmemosig;
	    break;
	 }
   ltm = time(NULL);
   date = gmtime(&ltm);
   hdr->yy = date->tm_year;
   hdr->mm = date->tm_mon + 1;
   hdr->dd = date->tm_mday;
   _rdd_put_uint(hdr->recs, 0);
   _rdd_put_ushort(hdr->hdrsize, sizeof(DBF_HEADER) + nfields * sizeof(DBF_FIELD) + 2 + 263);
   _rdd_put_ushort(hdr->recsize, 1);
   hdr = realloc(hdr, _rdd_ushort(hdr->hdrsize) + 1);
   hashes = new_HashTable();
   offs = 1;
   for (i = 0; i < nfields; i++)
      {
	 if (stru[i].nullable)
	    nulls = 1;
	 _rdd_put_ushort(hdr->recsize, _rdd_ushort(hdr->recsize) + stru[i].len);
	 fld = (DBF_FIELD *) ((char *) hdr + sizeof(DBF_HEADER) + i * sizeof(DBF_FIELD));
	 memset(fld, 0, sizeof(DBF_FIELD));
	 strncpy(fld->name, stru[i].name, 10);
	 for (s = fld->name; *s && *s != ' '; s++);
	 *s = 0;
	 loc_write(dbf_get_locale(ClipMemoryStore), (unsigned char *) (fld->name), strlen(fld->name));
	 fld->type = stru[i].type;
	 if (stru[i].type == 'V' && stru[i].len != 3 && stru[i].len != 4 && stru[i].len < 6)
	    {
	       er = rdd_err(ClipMemoryStore, EG_CREATE, 0, __FILE__, __LINE__, __PROC__, er_badstructure);
	       goto err;
	    }
	 if (strchr("PG", stru[i].type))
	    stru[i].binary = 1;
	 _rdd_put_uint(fld->offs, offs);
	 offs += stru[i].len;
	 if (fld->type == 'N')
	    {
	       if (stru[i].dec >= stru[i].len - (stru[i].dec != 0))
		  {
		     er = rdd_err(ClipMemoryStore, EG_CREATE, 0, __FILE__, __LINE__, __PROC__, er_badstructure);
		     goto err;
		  }
	       fld->len.num.len = stru[i].len;
	       fld->len.num.dec = stru[i].dec;
	    }
	 else if (fld->type == 'X')
	    {
	       if (stru[i].len < 10 || stru[i].len > 127)
		  {
		     er = rdd_err(ClipMemoryStore, EG_CREATE, 0, __FILE__, __LINE__, __PROC__, er_badstructure);
		     goto err;
		  }
	       fld->len.num.len = stru[i].len;
	       fld->len.num.dec = stru[i].dec;
	    }
	 else if (fld->type == 'Y')
	    {
	       fld->len.num.len = 8;
	       fld->len.num.dec = stru[i].dec;
	    }
	 else
	    {
	       _rdd_put_ushort((unsigned char *) (fld->len.len), stru[i].len);
	    }
	 fld->flags = (stru[i].binary ? 0x04 : 0) | (stru[i].nullable ? 0x02 : 0);
	 if (!HashTable_insert(hashes, fld, _clip_casehashword(fld->name, strlen(fld->name))))
	    {
	       er = rdd_err(ClipMemoryStore, EG_CREATE, 0, __FILE__, __LINE__, __PROC__, er_fielduplicate);
	       goto err;
	    }
      }
   if (nulls)
      {
	 int len = (nfields + 7) / 8;
	 _rdd_put_ushort(hdr->hdrsize, _rdd_ushort(hdr->hdrsize) + sizeof(DBF_FIELD));
	 hdr = realloc(hdr, _rdd_ushort(hdr->hdrsize) + 1);
	 _rdd_put_ushort(hdr->recsize, _rdd_ushort(hdr->recsize) + len);
	 fld = (DBF_FIELD *) ((char *) hdr + sizeof(DBF_HEADER) + i * sizeof(DBF_FIELD));
	 memset(fld, 0, sizeof(DBF_FIELD));
	 strcpy(fld->name, "_NullFlags");
	 fld->type = '0';
	 _rdd_put_uint(fld->offs, offs);
	 fld->len.num.len = len;
	 fld->len.num.dec = 0;
	 fld->flags = 0x05;
      }
   delete_HashTable(hashes);

   chdr = (char *) hdr;
   chdr[_rdd_ushort(hdr->hdrsize) - 2 - 263] = 0x0D;
   chdr[_rdd_ushort(hdr->hdrsize) - 1 - 263] = 0x00;
   chdr[_rdd_ushort(hdr->hdrsize)] = 0x1A;

   memset(&file, 0, sizeof(RDD_FILE));
   file.md = (char *) -1;
#ifdef _WIN32
   file.fd = open(name, O_CREAT | O_TRUNC | O_RDWR | O_BINARY, ClipMemoryStore->fileCreateMode);
#else
   file.fd = open(name, O_CREAT | O_TRUNC | O_RDWR, ClipMemoryStore->fileCreateMode);
#endif
   if (file.fd == -1)
      goto err_create;
   if ((er = rdd_write(ClipMemoryStore, &file, 0, _rdd_ushort(hdr->hdrsize) + 1, (char *) hdr, __PROC__)))
      goto err;
   if (close(file.fd) == -1)
      goto err_create;

   free(hdr);
   return 0;
 err_create:
   er = rdd_err(ClipMemoryStore, EG_CREATE, errno, __FILE__, __LINE__, __PROC__, name);
 err:
   free(hdr);
   return er;
}

static int
dbf_zap(ClipMachine * ClipMemoryStore, RDD_DATA * rd, const char *__PROC__)
{
   DBF_HEADER hdr;
   char end = 0x1A;
   int er;

   if (rd->readonly)
      return rdd_err(ClipMemoryStore, EG_READONLY, 0, __FILE__, __LINE__, __PROC__, er_notpermitted);
   rd->updated = 1;
   if ((er = rdd_read(ClipMemoryStore, &rd->file, 0, sizeof(DBF_HEADER), &hdr, __PROC__)))
      return er;
   _rdd_put_uint(hdr.recs, 0);
   if ((er = rdd_write(ClipMemoryStore, &rd->file, 0, sizeof(DBF_HEADER), &hdr, __PROC__)))
      return er;
   if ((er = rdd_trunc(ClipMemoryStore, &rd->file, _rdd_ushort(hdr.hdrsize) + 1, __PROC__)))
      return er;
   if ((er = rdd_write(ClipMemoryStore, &rd->file, _rdd_ushort(hdr.hdrsize), 1, &end, __PROC__)))
      return er;
   return 0;
}

static int
dbf_close(ClipMachine * ClipMemoryStore, RDD_DATA * rd, const char *__PROC__)
{
   int i;
   unsigned int lastrec;
   DBF_HEADER hdr;
   char end = 0x1A;
   time_t ltm;
   struct tm *date;
   int er;

   for (i = 0; i < rd->idxs_opened; i++)
      {
	 if ((er = rd->indices[i]->vtbl->close(ClipMemoryStore, rd, rd->indices[i], __PROC__)))
	    return er;
      }
   if (rd->memo)
      {
	 if ((er = rd->memo->vtbl->close(ClipMemoryStore, rd, rd->memo, __PROC__)))
	    return er;
	 rd->memo = NULL;
      }
   if ((er = rdd_read(ClipMemoryStore, &rd->file, 0, sizeof(DBF_HEADER), &hdr, __PROC__)))
      return er;
   if ((er = dbf_lastrec(ClipMemoryStore, rd, (int *) (&lastrec), __PROC__)))
      return er;
   if (!rd->readonly && rd->updated)
      {
	 if (!rd->shared)
	    {
	       if ((er = rdd_trunc(ClipMemoryStore, &rd->file, _rdd_ushort(hdr.hdrsize) + _rdd_ushort(hdr.recsize) * lastrec + 1, __PROC__)))
		  return er;
	    }
	 if ((er = rdd_write(ClipMemoryStore, &rd->file, _rdd_ushort(hdr.hdrsize) + _rdd_ushort(hdr.recsize) * lastrec, 1, &end, __PROC__)))
	    return er;
      }
   if (rd->updated)
      {
	 ltm = time(NULL);
	 date = gmtime(&ltm);
	 hdr.yy = date->tm_year < 100 ? date->tm_year : date->tm_year - 100;
	 hdr.mm = date->tm_mon + 1;
	 hdr.dd = date->tm_mday;
	 if ((er = rdd_write(ClipMemoryStore, &rd->file, 1, 3, &hdr.yy, __PROC__)))
	    return er;
      }
/*
	if(_clip_unlock(ClipMemoryStore,rd->file.filehash,rd->file.fd,0x7fffffff,0))
		return rdd_err(ClipMemoryStore,EG_CLOSE,errno,__FILE__,__LINE__,__PROC__,
			er_unlock);
*/
   return 0;
}

static int
dbf_lastrec(ClipMachine * ClipMemoryStore, RDD_DATA * rd, int *lastrec, const char *__PROC__)
{
   char recs[4];
   int er;
   struct stat st;

   if (rd->shared)
      {
	 fstat(rd->file.fd, &st);
	 *lastrec = (st.st_size - rd->hdrsize) / rd->recsize;
      }
   else
      {
	 if ((er = rdd_read(ClipMemoryStore, &rd->file, 4, 4, recs, __PROC__)))
	    return er;
	 *lastrec = _rdd_uint((unsigned char *) recs);
      }
   return 0;
}

static int
dbf_deleted(ClipMachine * ClipMemoryStore, RDD_DATA * rd, int *deleted, const char *__PROC__)
{
   int er;

   *deleted = 0;
   if (rd->recno == 0)
      return 0;
   if (!rd->valid)
      if ((er = dbf_getrecord(ClipMemoryStore, rd, __PROC__)))
	 return er;
   *deleted = (((char *) rd->record)[0] != 0x20);
   return 0;
}

static int
dbf_gotop(ClipMachine * ClipMemoryStore, RDD_DATA * rd, const char *__PROC__)
{
   unsigned int lastrec;
   int res, deleted, er;

   if ((er = dbf_lastrec(ClipMemoryStore, rd, (int *) (&lastrec), __PROC__)))
      return er;

   rd->bof = rd->v_bof = rd->eof = 0;

   if (rd->filter && rd->filter->list)
      {
	 if (rd->filter->listlen == 0)
	    {
	       rd->bof = rd->v_bof = rd->eof = 1;
	       if ((er = dbf_rawgoto(ClipMemoryStore, rd, lastrec + 1, 0, __PROC__)))
		  return er;
	       rd->filter->cursor = 0;
	    }
	 else
	    {
	       rd->filter->cursor = -1;
	       while (++rd->filter->cursor < rd->filter->listlen)
		  {
		     if ((er = dbf_rawgoto(ClipMemoryStore, rd, rd->filter->list[rd->filter->cursor], 0, __PROC__)))
			return er;
		     if (!(ClipMemoryStore->flags & DELETED_FLAG))
			return 0;
		     if ((er = rdd_deleted(ClipMemoryStore, rd, &deleted, __PROC__)))
			return er;
		     if (!deleted)
			return 0;
		  }
	       rd->eof = rd->bof = rd->v_bof = 1;
	       if ((er = dbf_rawgoto(ClipMemoryStore, rd, lastrec + 1, 0, __PROC__)))
		  return er;
	    }
	 return 0;
      }

   rd->recno = 1;

   if (rd->filter && rd->filter->rmap)
      {
	 while (rd->recno <= lastrec && !_rm_getbit(rd->filter->rmap, rd->filter->size, rd->recno))
	    rd->recno++;
      }
   if (lastrec == 0 || rd->recno > lastrec)
      {
	 rd->eof = 1;
	 rd->bof = rd->v_bof = (lastrec == 0);
	 if ((er = dbf_rawgoto(ClipMemoryStore, rd, lastrec + 1, 0, __PROC__)))
	    return er;
	 return 0;
      }
   if ((er = dbf_rawgoto(ClipMemoryStore, rd, rd->recno, 0, __PROC__)))
      return er;
   if ((er = rdd_checkfilter(ClipMemoryStore, rd, &res, __PROC__)))
      return er;
   if (!res)
      {
	 if ((er = dbf_next(ClipMemoryStore, rd, 0, __PROC__)))
	    return er;
	 if (rd->eof)
	    {
	       rd->bof = rd->v_bof = 1;
	       if ((er = dbf_rawgoto(ClipMemoryStore, rd, lastrec + 1, 0, __PROC__)))
		  return er;
	    }
	 return 0;
      }
   return 0;
}

static int
dbf_gobottom(ClipMachine * ClipMemoryStore, RDD_DATA * rd, const char *__PROC__)
{
   unsigned int lastrec;
   int res, deleted, er;

   if ((er = dbf_lastrec(ClipMemoryStore, rd, (int *) (&lastrec), __PROC__)))
      return er;
   rd->bof = rd->v_bof = rd->eof = 0;

   if (rd->filter && rd->filter->list)
      {
	 if (rd->filter->listlen == 0)
	    {
	       rd->bof = rd->v_bof = rd->eof = 1;
	       if ((er = dbf_rawgoto(ClipMemoryStore, rd, lastrec + 1, 0, __PROC__)))
		  return er;
	       rd->filter->cursor = 0;
	    }
	 else
	    {
	       rd->filter->cursor = rd->filter->listlen;
	       while (rd->filter->cursor > 0)
		  {
		     if ((er = dbf_rawgoto(ClipMemoryStore, rd, rd->filter->list[--rd->filter->cursor], 0, __PROC__)))
			return er;
		     if (!(ClipMemoryStore->flags & DELETED_FLAG))
			return 0;
		     if ((er = rdd_deleted(ClipMemoryStore, rd, &deleted, __PROC__)))
			return er;
		     if (!deleted)
			return 0;
		  }
	       rd->eof = rd->bof = rd->v_bof = 1;
	       if ((er = dbf_lastrec(ClipMemoryStore, rd, (int *) (&lastrec), __PROC__)))
		  return er;
	       if ((er = dbf_rawgoto(ClipMemoryStore, rd, lastrec + 1, 0, __PROC__)))
		  return er;
	    }
	 return 0;
      }

   if (lastrec == 0)
      {
	 rd->bof = rd->v_bof = rd->eof = 1;
	 if ((er = dbf_rawgoto(ClipMemoryStore, rd, 1, 0, __PROC__)))
	    return er;
	 return 0;
      }

   rd->bof = rd->v_bof = rd->eof = 0;
   rd->recno = lastrec;
   if (rd->filter && rd->filter->rmap)
      {
	 while (rd->recno >= 1 && !_rm_getbit(rd->filter->rmap, rd->filter->size, rd->recno))
	    rd->recno--;
      }
   if (rd->recno < 1)
      {
	 rd->eof = 1;
	 if ((er = dbf_rawgoto(ClipMemoryStore, rd, lastrec + 1, 0, __PROC__)))
	    return er;
	 return 0;
      }
   if ((er = dbf_rawgoto(ClipMemoryStore, rd, rd->recno, 0, __PROC__)))
      return er;

   if ((er = rdd_checkfilter(ClipMemoryStore, rd, &res, __PROC__)))
      return er;
   if (!res)
      {
	 if ((er = dbf_prev(ClipMemoryStore, rd, __PROC__)))
	    return er;
	 if (rd->bof)
	    {
	       rd->eof = 1;
	       if ((er = dbf_rawgoto(ClipMemoryStore, rd, lastrec + 1, 0, __PROC__)))
		  return er;
	    }
	 return 0;
      }
   return 0;
}

static int
dbf_next(ClipMachine * ClipMemoryStore, RDD_DATA * rd, int filtok, const char *__PROC__)
{
   int lastrec, res, er, deleted;

   if (rd->eof)
      return 0;
   rd->bof = rd->v_bof = rd->eof = 0;

   if ((er = dbf_lastrec(ClipMemoryStore, rd, &lastrec, __PROC__)))
      return er;
   if (rd->filter && rd->filter->list)
      {
	 while (++rd->filter->cursor < rd->filter->listlen)
	    {
	       if ((er = dbf_rawgoto(ClipMemoryStore, rd, rd->filter->list[rd->filter->cursor], 0, __PROC__)))
		  return er;
	       if (!(ClipMemoryStore->flags & DELETED_FLAG))
		  return 0;
	       if ((er = rdd_deleted(ClipMemoryStore, rd, &deleted, __PROC__)))
		  return er;
	       if (!deleted)
		  return 0;
	    }
	 rd->eof = 1;
	 if ((er = dbf_rawgoto(ClipMemoryStore, rd, lastrec + 1, 0, __PROC__)))
	    return er;
	 if (rd->filter->listlen == 0)
	    {
	       rd->bof = rd->v_bof = 1;
	       rd->filter->cursor = 0;
	    }
	 return 0;
      }
   while (1)
      {
	 rd->recno++;
	 if (rd->filter && rd->filter->rmap)
	    while (rd->recno <= lastrec && !_rm_getbit(rd->filter->rmap, rd->filter->size, rd->recno))
	       rd->recno++;
	 if ((er = dbf_rawgoto(ClipMemoryStore, rd, rd->recno, 0, __PROC__)))
	    return er;
	 if (rd->recno > lastrec)
	    {
	       rd->eof = 1;
	       return 0;
	    }
	 if (filtok)
	    break;
	 if ((er = rdd_checkfilter(ClipMemoryStore, rd, &res, __PROC__)))
	    return er;
	 if (res)
	    break;
      }
   return 0;
}

static int
dbf_prev(ClipMachine * ClipMemoryStore, RDD_DATA * rd, const char *__PROC__)
{
   int res, deleted, lastrec, er;

   if (rd->bof)
      return 0;
   rd->bof = rd->v_bof = rd->eof = 0;

   if (rd->filter && rd->filter->list)
      {
	 int oldcursor = rd->filter->cursor;
	 while (rd->filter->cursor > 0)
	    {
	       if ((er = dbf_rawgoto(ClipMemoryStore, rd, rd->filter->list[--rd->filter->cursor], 0, __PROC__)))
		  return er;
	       if (!(ClipMemoryStore->flags & DELETED_FLAG))
		  return 0;
	       if ((er = rdd_deleted(ClipMemoryStore, rd, &deleted, __PROC__)))
		  return er;
	       if (!deleted)
		  return 0;
	    }
	 rd->bof = rd->v_bof = 1;
	 if (rd->filter->listlen == 0)
	    {
	       rd->eof = 1;
	       if ((er = dbf_lastrec(ClipMemoryStore, rd, &lastrec, __PROC__)))
		  return er;
	       if ((er = dbf_rawgoto(ClipMemoryStore, rd, lastrec + 1, 0, __PROC__)))
		  return er;
	    }
	 else
	    {
	       rd->filter->cursor = oldcursor;
	       if ((er = dbf_rawgoto(ClipMemoryStore, rd, rd->filter->list[oldcursor], 0, __PROC__)))
		  return er;
	    }
	 return 0;
      }

   while (1)
      {
	 rd->recno--;
	 if (rd->filter && rd->filter->rmap)
	    while (rd->recno > 0 && !_rm_getbit(rd->filter->rmap, rd->filter->size, rd->recno))
	       rd->recno--;
	 if (rd->recno <= 0)
	    {
	       if ((er = dbf_gotop(ClipMemoryStore, rd, __PROC__)))
		  return er;
	       rd->bof = rd->v_bof = 1;
	       return 0;
	    }
	 if ((er = dbf_rawgoto(ClipMemoryStore, rd, rd->recno, 0, __PROC__)))
	    return er;
	 if ((er = rdd_checkfilter(ClipMemoryStore, rd, &res, __PROC__)))
	    return er;
	 if (res)
	    break;
      }
   return 0;
}

static int
dbf_rawgoto(ClipMachine * ClipMemoryStore, RDD_DATA * rd, unsigned int rec, char valid_stack, const char *__PROC__)
{
   int er;

   if (rd->valid && rd->changed)
      if ((er = dbf_setrecord(ClipMemoryStore, rd, __PROC__)))
	 return er;
   rd->valid = 0;
   rd->recno = rec;
   if (rd->curord >= 0)
      rd->orders[rd->curord]->valid_stack = valid_stack;
   return rdd_childs(ClipMemoryStore, rd, __PROC__);
}

static int
dbf_goto(ClipMachine * ClipMemoryStore, RDD_DATA * rd, unsigned int rec, const char *__PROC__)
{
   int lastrec;
   int er;

/*	printf("goto %d\n? 'goto %d',recno(),bof()\n",rec,rec);*/
   if ((er = dbf_lastrec(ClipMemoryStore, rd, &lastrec, __PROC__)))
      return er;

   rd->recno = rec;
   rd->bof = rd->v_bof = rd->eof = 0;
   if ((rec < 1) || (rec > lastrec))
      {
	 rd->v_bof = rd->eof = 1;
	 rd->recno = lastrec + 1;
      }
   return dbf_rawgoto(ClipMemoryStore, rd, rd->recno, 0, __PROC__);
}

static int
dbf_append(ClipMachine * ClipMemoryStore, RDD_DATA * rd, const char *__PROC__)
{
   char recs[4];
   int lastrec, flen, i, er;

   if (rd->readonly)
      {
	 er = rdd_err(ClipMemoryStore, EG_READONLY, 0, __FILE__, __LINE__, __PROC__, er_notpermitted);
	 goto err;
      }
   if ((er = dbf_lastrec(ClipMemoryStore, rd, &lastrec, __PROC__)))
      goto err;
   rd->recno = lastrec + 1;
   if (rd->curord >= 0)
      rd->orders[rd->curord]->valid_stack = 0;
   rd->updated = 1;

   if (!rd->shared)
      {
	 flen = rd->hdrsize + rd->recsize * (lastrec + 1) + 1;
	 if (rd->file.md != (caddr_t) - 1)
	    {
	       if (flen > rd->file.mapsize)
		  {
		     int delta = lastrec / 5;
		     if ((er = rdd_write(ClipMemoryStore, &rd->file, rd->hdrsize + rd->recsize * (lastrec + 1 + delta) - 1, 1, "", __PROC__)))
			goto err;
		  }
	    }
      }

   if (rd->sig != 0x30)
      {
	 memset(rd->record, 32, rd->recsize);
	 for (i = 0; i < rd->nfields; i++)
	    if (rd->fields[i].type == 'V' || rd->fields[i].type == 'X')
	       memset(rd->record + rd->fields[i].offs, 0, rd->fields[i].len);
      }
   ((char *) rd->record)[0] = 0x20;
   ((char *) rd->record)[rd->recsize] = 0x1a;
   rd->valid = 1;
   rd->changed = 0;
   lastrec++;
   _rdd_put_uint((unsigned char *) recs, lastrec);
   if ((er = rdd_write(ClipMemoryStore, &rd->file, rd->hdrsize + rd->recsize * (lastrec - 1), rd->recsize + 1, rd->record, __PROC__)))
      goto err;
   if ((er = rdd_write(ClipMemoryStore, &rd->file, 4, 4, recs, __PROC__)))
      goto err;
   rd->bof = rd->v_bof = rd->eof = 0;
   return 0;
 err:
   return er;
}

static int
dbf_getmemoid(ClipMachine * ClipMemoryStore, RDD_DATA * rd, int no, unsigned int *memoid, unsigned short int *vlen, const char *__PROC__)
{
   char memo[11];
   int er;

   *memoid = 0;
   if (strchr("MPG", rd->fields[no].type))
      {
	 if (rd->sig != 0x30)
	    {
	       if ((er = rdd_read(ClipMemoryStore, &rd->file, rd->hdrsize + rd->recsize * (rd->recno - 1) + rd->fields[no].offs, 10, memo, __PROC__)))
		  return er;
	       memo[10] = 0;
	       *memoid = atoi(memo);
	    }
	 else
	    {
	       if ((er = rdd_read(ClipMemoryStore, &rd->file, rd->hdrsize + rd->recsize * (rd->recno - 1) + rd->fields[no].offs, 4, memo, __PROC__)))
		  return er;
	       *memoid = _rdd_uint((unsigned char *) memo);
	    }
      }
   else if (rd->fields[no].type == 'V')
      {
	 if ((er = rdd_read(ClipMemoryStore, &rd->file, rd->hdrsize + rd->recsize * (rd->recno - 1) + rd->fields[no].offs + rd->fields[no].len - 6, 4, memo, __PROC__)))
	    return er;
	 *memoid = _rdd_uint((unsigned char *) memo);
	 if (vlen)
	    {
	       if ((er = rdd_read(ClipMemoryStore, &rd->file, rd->hdrsize + rd->recsize * (rd->recno - 1) + rd->fields[no].offs + rd->fields[no].len - 2, 2, memo, __PROC__)))
		  return er;
	       *vlen = _rdd_ushort((unsigned char *) memo);
	    }
      }
   else
      {
	 return rdd_err(ClipMemoryStore, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_corruption);
      }
   return 0;
}

static int
dbf_setmemoid(ClipMachine * ClipMemoryStore, RDD_DATA * rd, int no, unsigned int memoid, const char *__PROC__)
{
   char memo[11];
   int er;

   if (rd->readonly)
      return rdd_err(ClipMemoryStore, EG_READONLY, 0, __FILE__, __LINE__, __PROC__, er_notpermitted);
   rd->updated = 1;
   if (strchr("MPG", rd->fields[no].type))
      {
	 if (rd->sig != 0x30)
	    {
	       snprintf(memo, sizeof(memo), "%d", memoid);
	       if ((er = rdd_write(ClipMemoryStore, &rd->file, rd->hdrsize + rd->recsize * (rd->recno - 1) + rd->fields[no].offs, 10, memo, __PROC__)))
		  return er;
	    }
	 else
	    {
	       _rdd_put_uint((unsigned char *) memo, memoid);
	       if ((er = rdd_write(ClipMemoryStore, &rd->file, rd->hdrsize + rd->recsize * (rd->recno - 1) + rd->fields[no].offs, 4, memo, __PROC__)))
		  return er;
	    }
      }
   else if (rd->fields[no].type == 'V')
      {
	 _rdd_put_uint((unsigned char *) memo, memoid);
	 if ((er = rdd_write(ClipMemoryStore, &rd->file, rd->hdrsize + rd->recsize * (rd->recno - 1) + rd->fields[no].offs + rd->fields[no].len - 6, 4, memo, __PROC__)))
	    return er;
      }
   else
      {
	 return rdd_err(ClipMemoryStore, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_corruption);
      }
   return 0;
}

static int
dbf_getrecord(ClipMachine * ClipMemoryStore, RDD_DATA * rd, const char *__PROC__)
{
   rd->valid = 1;
   return rdd_read(ClipMemoryStore, &rd->file, rd->hdrsize + rd->recsize * (rd->recno - 1), rd->recsize, rd->record, __PROC__);
}

static int
dbf_setrecord(ClipMachine * ClipMemoryStore, RDD_DATA * rd, const char *__PROC__)
{
   rd->changed = 0;
   return rdd_write(ClipMemoryStore, &rd->file, rd->hdrsize + rd->recsize * (rd->recno - 1), rd->recsize, rd->record, __PROC__);
}

static int
dbf_verify(ClipMachine * ClipMemoryStore, RDD_DATA * rd, int *r, const char *__PROC__)
{
   void *buf = malloc(rd->recsize);
   int er;

   if (!rd->valid)
      {
	 if ((er = dbf_getrecord(ClipMemoryStore, rd, __PROC__)))
	    return er;
	 *r = 1;
	 return 0;
      }
   if ((er = rdd_read(ClipMemoryStore, &rd->file, rd->hdrsize + rd->recsize * (rd->recno - 1), rd->recsize, buf, __PROC__)))
      return er;
   *r = !memcmp(rd->record, buf, rd->recsize);
   return 0;
}

static int
dbf_getvalue(ClipMachine * ClipMemoryStore, RDD_DATA * rd, int no, ClipVar * vp, const char *__PROC__)
{
   char *str;
   char l;
   char date[9];
   int yy = 0, mm = 0, dd = 0;
   char memo[11];
   int er;

   if (!rd->valid)
      if ((er = dbf_getrecord(ClipMemoryStore, rd, __PROC__)))
	 return er;

   if (rd->fields[no].nullable)
      {
	 int nbyte = no >> 3;
	 unsigned char byte;

	 memcpy(&byte, (char *) rd->record + rd->nulloffs + nbyte, 1);
	 if (byte & (1 << (no - (nbyte << 3))))
	    {
	       memset(vp, 0, sizeof(ClipVar));
	       return 0;
	    }
      }
   switch (rd->fields[no].type)
      {
      case 'C':
	 vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	 vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	 vp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	 str = (char *) malloc(rd->fields[no].len + 1);
	 memcpy(str, (char *) rd->record + rd->fields[no].offs, rd->fields[no].len);
	 str[rd->fields[no].len] = 0;
	 vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = str;
	 vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = rd->fields[no].len;
	 if (!rd->fields[no].binary)
	    loc_read(rd->loc, (unsigned char *) (vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf), vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
	 break;
      case 'M':
      case 'P':
      case 'G':
	 vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	 vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	 vp->ClipType_t_of_ClipVar.memo_of_ClipType = 1;
	 if (rd->memo)
	    {
	       if (rd->sig != 0x30)
		  {		/* Not VFP */
		     memcpy(memo, (char *) rd->record + rd->fields[no].offs, 10);
		     memo[10] = 0;
		     if ((er = rd->memo->vtbl->getvalue(ClipMemoryStore, rd->memo, atoi(memo), vp, __PROC__)))
			return er;
		  }
	       else
		  {
		     memcpy(memo, (char *) rd->record + rd->fields[no].offs, 4);
		     if ((er = rd->memo->vtbl->getvalue(ClipMemoryStore, rd->memo, _rdd_uint((unsigned char *) memo), vp, __PROC__)))
			return er;
		  }
	    }
	 else
	    {
	       vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = strdup("!!! Memo file not opened");
	       vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = 24;
	    }
	 break;
      case 'N':
      case 'F':		/* Visual FoxPro type */
	 vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	 vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	 vp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	 str = (char *) malloc(rd->fields[no].len + 1);
	 str[rd->fields[no].len] = 0;
	 memcpy(str, (char *) rd->record + rd->fields[no].offs, rd->fields[no].len);
	 vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = _clip_strtod(str, &yy);
	 free(str);
	 vp->ClipType_t_of_ClipVar.len_of_ClipType = rd->fields[no].len;
	 vp->ClipType_t_of_ClipVar.dec_of_ClipType = rd->fields[no].dec;
	 break;
      case 'D':
	 vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATE_type_of_ClipVarType;
	 vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	 vp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	 memcpy(date, (char *) rd->record + rd->fields[no].offs, rd->fields[no].len);
	 if (!isdigit(date[0]))
	    {
	       dd = 0;
	    }
	 else
	    {
	       dd = (date[7] - '0');
	       dd += (date[6] - '0') * 10;
	       mm = (date[5] - '0');
	       mm += (date[4] - '0') * 10;
	       yy = (date[3] - '0');
	       yy += (date[2] - '0') * 10;
	       yy += (date[1] - '0') * 100;
	       yy += (date[0] - '0') * 1000;
	    }
	 if (dd)
	    vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = _clip_jdate(dd, mm, yy);
	 else
	    vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = 0;
	 break;
      case 'V':
	 {
	    if (rd->fields[no].len == 3)
	       {
		  unsigned char vd[4];
		  unsigned int vi;
		  vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATE_type_of_ClipVarType;
		  vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
		  vp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
		  memcpy(vd + 1, (char *) rd->record + rd->fields[no].offs, rd->fields[no].len);
		  *vd = 0;
		  if (vd[1] == ' ' || vd[1] == 0)
		     vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = 0;
		  else
		     {
			vi = _rdd_backuint(vd);
			vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = _clip_jdate((vi & 0x1f), (vi & 0x1e0) >> 5, vi >> 9);
		     }
	       }
	    else if (rd->fields[no].len == 4)
	       {
		  unsigned char vi[4];
		  vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
		  vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
		  vp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
		  memcpy(vi, (char *) rd->record + rd->fields[no].offs, rd->fields[no].len);
		  vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = (double) _rdd_uint(vi);
		  vp->ClipType_t_of_ClipVar.len_of_ClipType = 11;
		  vp->ClipType_t_of_ClipVar.dec_of_ClipType = 0;
	       }
	    else
	       {
		  unsigned char vlen[4];

		  if (rd->memo)
		     {
			vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
			vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
			vp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
			if (rd->indexing)
			   {
			      char *s;
			      vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = rd->fields[no].len - 6;
			      vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = malloc(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf + 1);
			      vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf] = 0;
			      memcpy(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, (char *) rd->record + rd->fields[no].offs,
vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
			      s = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf + vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
			      while (!(*(--s)))
				 *s = ' ';
			      if (!rd->fields[no].binary)
				 loc_read(rd->loc, (unsigned char *) (vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf), vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
			      return 0;
			   }
			memcpy(vlen, (char *) rd->record + rd->fields[no].offs + rd->fields[no].len - 2, 2);
			vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = _rdd_ushort(vlen);
			vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = malloc(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf + 1);
			vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf] = 0;
			if (vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf <= rd->fields[no].len - 2)
			   {
			      memcpy(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, (char *) rd->record + rd->fields[no].offs,
vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
			   }
			else
			   {
			      memcpy(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, (char *) rd->record + rd->fields[no].offs, rd->fields[no].len - 6);
			      memcpy(vlen, (char *) rd->record + rd->fields[no].offs + rd->fields[no].len - 6, 4);
			      if (rd->memo->vtbl->getvchar)
				 {
				    if ((er =
					 rd->memo->vtbl->getvchar(ClipMemoryStore, rd->memo,
								  vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf - (rd->fields[no].len - 6), _rdd_uint(vlen),
vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf + rd->fields[no].len - 6, __PROC__)))
				       return er;
				 }
			   }
		     }
		  else
		     {
			vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = strdup("!!! Memo file not opened");
			vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = 24;
		     }
		  if (!rd->fields[no].binary)
		     loc_read(rd->loc, (unsigned char *) (vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf), vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
	       }
	    break;
	 }
      case 'L':
	 vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
	 vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	 vp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	 memcpy(&l, (char *) rd->record + rd->fields[no].offs, 1);
	 vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = (l == 'T' ? 1 : 0);
	 break;
      case 'X':
	 {
	    memset(vp, 0, sizeof(ClipVar));
	    switch (((char *) rd->record)[rd->fields[no].offs])
	       {
	       case 'N':
		  {
		     vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
		     memcpy(&vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, (char *) rd->record + rd->fields[no].offs + 2, 8);
		     vp->ClipType_t_of_ClipVar.len_of_ClipType = ((char *) rd->record)[rd->fields[no].offs + 1];
		     vp->ClipType_t_of_ClipVar.dec_of_ClipType = rd->fields[no].dec;
		     break;
		  }
	       case 'C':
		  {
		     vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
		     vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = ((unsigned char *) rd->record)[rd->fields[no].offs + 1];
		     vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = malloc(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf + 1);
		     memcpy(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, (char *) rd->record + rd->fields[no].offs + 2,
vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
		     vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf] = 0;
		     if (!rd->fields[no].binary)
			loc_read(rd->loc, (unsigned char *) (vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf), vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
		     break;
		  }
	       case 'D':
		  {
		     unsigned char vd[4];

		     vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATE_type_of_ClipVarType;
		     memcpy(vd + 1, (char *) rd->record + rd->fields[no].offs + 2, 3);
		     *vd = 0;
		     if (vd[1] == ' ' || vd[1] == 0)
			vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = 0;
		     else
			vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = _rdd_backuint(vd) + 0x15d971;
		     break;
		  }
	       case 'T':
		  {
		     char t[8];

		     vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATETIME_type_of_ClipVarType;
		     memcpy(t, (char *) rd->record + rd->fields[no].offs + 2, 8);
		     vp->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar = _rdd_uint((unsigned char *) t);
		     vp->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar = _rdd_uint((unsigned char *) (t + 4));
		     break;
		  }
	       case 'L':
		  {
		     vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
		     vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = (((char *) rd->record)[rd->fields[no].offs + 2] == 'T') ? 1 : 0;
		     break;
		  }
	       default:
		  break;
	       }
	    break;
	 }
/* Visual FoxPro types */
      case 'I':
	 {
	    unsigned char u4[4];
	    vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	    vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	    vp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	    memcpy(u4, (char *) rd->record + rd->fields[no].offs, 4);
	    vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = (double) ((int) _rdd_uint(u4));
	    vp->ClipType_t_of_ClipVar.len_of_ClipType = 11;
	    vp->ClipType_t_of_ClipVar.dec_of_ClipType = 0;
	    break;
	 }
      case 'B':
      case 'Y':
	 {
	    vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	    vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	    vp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	    memcpy(&vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, (char *) rd->record + rd->fields[no].offs, 8);
	    if (rd->fields[no].type == 'Y')
	       vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = (double) (*(long long *) &vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar) / 10000;
	    vp->ClipType_t_of_ClipVar.len_of_ClipType = 20;
	    vp->ClipType_t_of_ClipVar.dec_of_ClipType = rd->fields[no].dec;
	    break;
	 }
      case 'T':
	 {
	    char t[8];

	    vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATETIME_type_of_ClipVarType;
	    vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	    vp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	    memcpy(t, (char *) rd->record + rd->fields[no].offs, 8);
	    vp->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar = _rdd_uint((unsigned char *) t);
	    vp->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar = _rdd_uint((unsigned char *) (t + 4));
	    break;
	 }
      }
   return 0;
}

int
dbf_setvalue(ClipMachine * ClipMemoryStore, RDD_DATA * rd, int no, ClipVar * vp, int binary, const char *__PROC__)
{
   int er;

   if (rd->readonly)
      return rdd_err(ClipMemoryStore, EG_READONLY, 0, __FILE__, __LINE__, __PROC__, er_notpermitted);

   if (!rd->valid)
      if ((er = dbf_getrecord(ClipMemoryStore, rd, __PROC__)))
	 return er;

   if (!rd->data[no])
      {
	 rd->data[no] = calloc(1, sizeof(ClipVar));
	 if ((er = dbf_getvalue(ClipMemoryStore, rd, no, rd->data[no], __PROC__)))
	    return er;
      }
   rd->updated = rd->changed = 1;
   if (rd->fields[no].nullable)
      {
	 int nbyte = no >> 3;
	 unsigned char byte;

	 memcpy(&byte, rd->record + rd->nulloffs + nbyte, 1);
	 if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType)
	    byte |= (1 << (no - (nbyte << 3)));
	 else
	    byte &= ~(1 << (no - (nbyte << 3)));
	 memcpy(rd->record + rd->nulloffs + nbyte, &byte, 1);
	 if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType)
	    return 0;
      }
   switch (rd->fields[no].type)
      {
      case 'C':
	 {
	    char *str;
	    int l;

	    if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType)
	       goto err;
	    str = malloc(rd->fields[no].len);
	    l = min(rd->fields[no].len, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);

	    memcpy(str, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, l);
	    if (!rd->fields[no].binary)
	       loc_write(rd->loc, (unsigned char *) str, l);
	    memset(str + l, ' ', rd->fields[no].len - l);
	    memcpy(rd->record + rd->fields[no].offs, str, rd->fields[no].len);
	    free(str);
	    break;
	 }
      case 'M':
      case 'P':
      case 'G':
	 {
	    char memoid[11];
	    int memo;

	    if (!rd->memo)
	       return rdd_err(ClipMemoryStore, EG_WRITE, 0, __FILE__, __LINE__, __PROC__, er_nomemo);
	    if (rd->fields[no].type == 'P' || rd->fields[no].type == 'G')
	       binary = 1;
	    if (rd->sig != 0x30)
	       {		/* Not VFP */
		  memcpy(memoid, rd->record + rd->fields[no].offs, 10);
		  memoid[10] = 0;
		  memo = atoi(memoid);
		  if ((er = rd->memo->vtbl->setvalue(ClipMemoryStore, rd->memo, &memo, vp, binary, __PROC__)))
		     return er;
		  snprintf(memoid, sizeof(memoid), "%10d", memo);
		  memcpy(rd->record + rd->fields[no].offs, memoid, 10);
	       }
	    else
	       {
		  memcpy(memoid, rd->record + rd->fields[no].offs, 4);
		  memo = _rdd_uint((unsigned char *) memoid);
		  if ((er = rd->memo->vtbl->setvalue(ClipMemoryStore, rd->memo, &memo, vp, binary, __PROC__)))
		     return er;
		  _rdd_put_uint((unsigned char *) memoid, memo);
		  memcpy(rd->record + rd->fields[no].offs, memoid, 4);
	       }
	    break;
	 }
      case 'N':
      case 'F':		/* Visual FoxPro type */
	 {
	    char *n;
	    int len = rd->fields[no].len;
	    int dec = rd->fields[no].dec;

	    if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != NUMERIC_type_of_ClipVarType)
	       goto err;
	    n = calloc(1, len + 1);
	    if (_clip_dtostr(n, len, dec, vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, 0))
	       {
		  char buf[PATH_MAX];
		  snprintf(buf, sizeof(buf), "%s: %s", rd->path, rd->fields[no].name);
		  free(n);
		  return rdd_err(ClipMemoryStore, EG_DATAWIDTH, 0, __FILE__, __LINE__, buf, er_baddata);
	       }
	    memcpy(rd->record + rd->fields[no].offs, n, len);
	    free(n);
	    break;
	 }
      case 'D':
	 {
	    char d[] = "00000000";

	    if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != DATE_type_of_ClipVarType)
	       goto err;
	    if (vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar)
	       {
		  _clip_datetostr(vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar, d);
	       }
	    memcpy(rd->record + rd->fields[no].offs, d, 8);
	    break;
	 }
      case 'L':
	 if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
	    goto err;
	 memcpy(rd->record + rd->fields[no].offs, vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar ? "T" : "F", 1);
	 break;
      case 'V':
	 {
	    if (rd->fields[no].len == 3)
	       {
		  unsigned char vd[4];
		  unsigned int vi;
		  int dd, mm, yy, ww;

		  if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != DATE_type_of_ClipVarType)
		     goto err;

		  _clip_cdate(vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar, &dd, &mm, &yy, &ww);
		  vi = (yy << 9) + (mm << 5) + dd;

		  _rdd_put_backuint(vd, vi);
		  memcpy(rd->record + rd->fields[no].offs, vd + 1, 3);
	       }
	    else if (rd->fields[no].len == 4)
	       {
		  unsigned char vi[4];
		  if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != NUMERIC_type_of_ClipVarType)
		     goto err;
		  _rdd_put_uint(vi, (int) vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
		  memcpy(rd->record + rd->fields[no].offs, vi, 4);
	       }
	    else
	       {
		  unsigned char vlen[4];
		  char *str;
		  unsigned short int oldlen;
		  unsigned int memoid = 0;

		  if (!rd->memo)
		     return rdd_err(ClipMemoryStore, EG_WRITE, 0, __FILE__, __LINE__, __PROC__, er_nomemo);
		  if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType)
		     goto err;
		  memcpy(vlen, rd->record + rd->fields[no].offs + rd->fields[no].len - 2, 2);
		  oldlen = max((int) _rdd_ushort(vlen) - (rd->fields[no].len - 6), 0);
		  if (oldlen > rd->fields[no].len - 6)
		     {
			memcpy(vlen, rd->record + rd->fields[no].offs + rd->fields[no].len - 6, 4);
			memoid = _rdd_uint(vlen);
		     }
		  _rdd_put_ushort(vlen, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
		  memcpy(rd->record + rd->fields[no].offs + rd->fields[no].len - 2, vlen, 2);
		  str = calloc(1, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
		  memcpy(str, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
		  if (!rd->fields[no].binary)
		     loc_write(rd->loc, (unsigned char *) str, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
		  if (vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf <= rd->fields[no].len - 2)
		     {
			memcpy(rd->record + rd->fields[no].offs, str, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
		     }
		  else
		     {
			memcpy(rd->record + rd->fields[no].offs, str, rd->fields[no].len - 6);
			if (rd->memo->vtbl->setvchar)
			   {
			      if ((er = rd->memo->vtbl->setvchar(ClipMemoryStore, rd->memo,
								 vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf - (rd->fields[no].len - 6), oldlen, &memoid, str + rd->fields[no].len - 6, __PROC__)))
				 {
				    free(str);
				    return er;
				 }
			   }
			_rdd_put_uint(vlen, memoid);
			memcpy(rd->record + rd->fields[no].offs + rd->fields[no].len - 6, vlen, 4);
		     }
		  free(str);
	       }
	    break;
	 }
      case 'X':
	 {
	    memset((char *) rd->record + rd->fields[no].offs, 0, rd->fields[no].len);
	    switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
	       {
	       case NUMERIC_type_of_ClipVarType:
		  {
		     ((char *) rd->record)[rd->fields[no].offs] = 'N';
		     ((unsigned char *) rd->record)[rd->fields[no].offs + 1] = vp->ClipType_t_of_ClipVar.len_of_ClipType;
		     memcpy(rd->record + rd->fields[no].offs + 2, &vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, 8);
		     break;
		  }
	       case CHARACTER_type_of_ClipVarType:
		  {
		     unsigned int len = (unsigned char) min(rd->fields[no].len - 2, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
		     ((char *) rd->record)[rd->fields[no].offs] = 'C';
		     ((unsigned char *) rd->record)[rd->fields[no].offs + 1] = len;
		     memcpy((char *) rd->record + rd->fields[no].offs + 2, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, len);
		     if (!rd->fields[no].binary)
			loc_write(rd->loc, (unsigned char *) rd->record + rd->fields[no].offs + 2, len);
		     break;
		  }
	       case DATE_type_of_ClipVarType:
		  {
		     unsigned char vd[4];

		     ((char *) rd->record)[rd->fields[no].offs] = 'D';
		     ((unsigned char *) rd->record)[rd->fields[no].offs + 1] = 3;
		     _rdd_put_backuint(vd, vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar - 0x15d971);
		     memcpy(rd->record + rd->fields[no].offs + 2, vd + 1, 3);
		     break;
		  }
	       case DATETIME_type_of_ClipVarType:
		  {
		     char t[8];

		     ((char *) rd->record)[rd->fields[no].offs] = 'T';
		     ((unsigned char *) rd->record)[rd->fields[no].offs + 1] = 8;
		     _rdd_put_uint((unsigned char *) t, vp->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar);
		     _rdd_put_uint((unsigned char *) (t + 4), vp->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar);
		     memcpy((char *) rd->record + rd->fields[no].offs + 2, t, 8);
		     break;
		  }
	       case LOGICAL_type_of_ClipVarType:
		  {
		     ((char *) rd->record)[rd->fields[no].offs] = 'L';
		     ((unsigned char *) rd->record)[rd->fields[no].offs + 1] = 1;
		     ((char *) rd->record)[rd->fields[no].offs + 2] = (vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar ? 'T' : 'F');
		     break;
		  }
	       case UNDEF_type_of_ClipVarType:
		  break;
	       default:
		  goto err;
	       }
	    break;
	 }
/* Visual FoxPro types */
      case 'I':
	 {
	    unsigned char u4[4];

	    if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != NUMERIC_type_of_ClipVarType)
	       goto err;
	    _rdd_put_uint(u4, (int) vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
	    memcpy(rd->record + rd->fields[no].offs, u4, 4);
	    break;
	 }
      case 'B':
      case 'Y':
	 {
	    if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != NUMERIC_type_of_ClipVarType)
	       goto err;
	    if (rd->fields[no].type == 'Y')
	       {
		  long long tmp;
		  double d = vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
#ifdef ARCH_i386
		  long long *lld = (long long *) &d;

		  (*lld)++;
#endif
		  tmp = d * 10000;
		  memcpy(rd->record + rd->fields[no].offs, &tmp, 8);
	       }
	    else
	       memcpy(rd->record + rd->fields[no].offs, &vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar, 8);
	    break;
	 }
      case 'T':
	 {
	    char t[8];

	    if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != DATETIME_type_of_ClipVarType)
	       goto err;
	    _rdd_put_uint((unsigned char *) t, vp->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar);
	    _rdd_put_uint((unsigned char *) (t + 4), vp->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar);
	    memcpy((char *) rd->record + rd->fields[no].offs, t, 8);
	    break;
	 }
      }
   return 0;
 err:
   return rdd_err(ClipMemoryStore, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_type);
}

static int
dbf_delete(ClipMachine * ClipMemoryStore, RDD_DATA * rd, const char *__PROC__)
{
   char d = 0x2a;
   int er;

   if (rd->readonly)
      return rdd_err(ClipMemoryStore, EG_READONLY, 0, __FILE__, __LINE__, __PROC__, er_notpermitted);
   if (!rd->valid)
      if ((er = dbf_getrecord(ClipMemoryStore, rd, __PROC__)))
	 return er;
   rd->updated = rd->changed = 1;
   ((char *) rd->record)[0] = d;
   return 0;
}

static int
dbf_recall(ClipMachine * ClipMemoryStore, RDD_DATA * rd, const char *__PROC__)
{
   char d = 0x20;
   int er;

   if (rd->readonly)
      return rdd_err(ClipMemoryStore, EG_READONLY, 0, __FILE__, __LINE__, __PROC__, er_notpermitted);
   if (!rd->valid)
      if ((er = dbf_getrecord(ClipMemoryStore, rd, __PROC__)))
	 return er;
   rd->updated = rd->changed = 1;
   ((char *) rd->record)[0] = d;
   return 0;
}

static int
dbf_lupdate(ClipMachine * ClipMemoryStore, RDD_DATA * rd, const char *__PROC__)
{
   DBF_HEADER hdr;
   int er;

   if ((er = rdd_read(ClipMemoryStore, &rd->file, 0, sizeof(DBF_HEADER), &hdr, __PROC__)))
      return er;
   _clip_retdc(ClipMemoryStore, 1900 + (hdr.yy > 80 ? hdr.yy : hdr.yy + 100), hdr.mm, hdr.dd);
   return 0;
}

static int
dbf_info(ClipMachine * ClipMemoryStore, RDD_DATA * rd, int cmd, const char *__PROC__)
{
   switch (cmd)
      {
      case DBI_ISDBF:
	 {
	    _clip_retl(ClipMemoryStore, 1);
	    break;
	 }
      case DBI_LOCKOFFSET:
	 {
	    _clip_retni(ClipMemoryStore, rd->lockstyle ? 0x10000000 : 1000000000);
	    break;
	 }
      case DBI_VALIDBUFFER:
	 {
	    _clip_retl(ClipMemoryStore, 1);
	    break;
	 }
      }
   return 0;
}

static int
dbf_setstruct(ClipMachine * ClipMemoryStore, RDD_DATA * rd, const char *__PROC__)
{
   return rdd_write(ClipMemoryStore, &rd->file, 28, 1, "\001", __PROC__);
}

static int
_dbf_compare(void *op, void *lp, void *rp, int *uniq)
{
   if ((unsigned long) lp < (unsigned long) rp)
      return -1;
   if ((unsigned long) lp > (unsigned long) rp)
      return 1;
   return 0;
}

static int
dbf_calcfiltlist(ClipMachine * ClipMemoryStore, RDD_DATA * rd, RDD_FILTER * fp, const char *__PROC__)
{
   BTREE *bt;
   int i, j;
   unsigned int recno;

   if (fp->list)
      {
	 bt = bt_create(0, fp->listlen, sizeof(unsigned int), _dbf_compare);
	 for (i = 0; i < fp->listlen; i++)
	    {
	       recno = fp->list[i];
	       bt_add(bt, NULL, (void *) &recno);
	    }
	 free(fp->list);
	 fp->list = malloc(sizeof(unsigned int) * fp->listlen);
	 bt_first(bt);
	 i = 0;
	 fp->list[i] = *(unsigned int *) bt_key(bt);
	 while (!bt_next(bt))
	    {
	       i++;
	       fp->list[i] = *(unsigned int *) bt_key(bt);
	    }
	 bt_destroy(bt);
      }
   else
      {
#if 1
	 unsigned int bytes = ((fp->size + 1) >> 5) + 1;
	 int i, b, bb, t, tt;

	 fp->listlen = 0;
	 for (i = 0; i < bytes; i++)
	    {
	       if (fp->rmap[i])
		  {
		     for (b = (i << 2), bb = 0; bb < 4; b++, bb++)
			{
			   if (((char *) fp->rmap)[b])
			      {
				 for (t = (b << 3) + 1, tt = 0; tt < 8; t++, tt++)
				    {
				       if (_rm_getbit(fp->rmap, fp->size, t))
					  fp->listlen++;
				    }
			      }
			}
		  }
	    }
	 if (fp->listlen > 100)
	    return 0;
#else
	 fp->listlen = 0;
	 for (i = 1; i <= fp->size; i++)
	    if (_rm_getbit(fp->rmap, fp->size, i))
	       fp->listlen++;
#endif
	 fp->list = malloc(sizeof(unsigned int) * (fp->listlen + 1));
#if 1
	 for (i = 0, j = 0; i < bytes; i++)
	    {
	       if (fp->rmap[i])
		  {
		     for (b = (i << 2), bb = 0; bb < 4; b++, bb++)
			{
			   if (((char *) fp->rmap)[b])
			      {
				 for (t = (b << 3) + 1, tt = 0; tt < 8; t++, tt++)
				    {
				       if (_rm_getbit(fp->rmap, fp->size, t))
					  fp->list[j++] = t;
				    }
			      }
			}
		  }
	    }
#else
	 for (i = 1; i <= fp->size; i++)
	    {
	       if (_rm_getbit(fp->rmap, fp->size, i))
		  fp->list[j++] = i;
	    }
#endif
      }
   return 0;
}

static int
dbf_pack(ClipMachine * ClipMemoryStore, RDD_DATA * rd, int tfd, const char *__PROC__)
{
   DBF_HEADER hdr;
   unsigned char *buf = NULL;
   unsigned int oldnrecs;
   unsigned int newnrecs = 0;
   int rs, i;

   if (read(tfd, &hdr, sizeof(DBF_HEADER)) != sizeof(DBF_HEADER))
      goto err;
   oldnrecs = _rdd_uint(hdr.recs);
   _rdd_put_uint(hdr.recs, 0);
   if (write(rd->file.fd, &hdr, sizeof(DBF_HEADER)) != sizeof(DBF_HEADER))
      goto err;
   buf = malloc(_rdd_ushort(hdr.hdrsize) - sizeof(DBF_HEADER));
   if (read(tfd, buf, _rdd_ushort(hdr.hdrsize) - sizeof(DBF_HEADER)) != _rdd_ushort(hdr.hdrsize) - sizeof(DBF_HEADER))
      goto err;
   if (write(rd->file.fd, buf, _rdd_ushort(hdr.hdrsize) - sizeof(DBF_HEADER)) != _rdd_ushort(hdr.hdrsize) - sizeof(DBF_HEADER))
      goto err;
   free(buf);
   rs = _rdd_ushort(hdr.recsize);
   buf = malloc(rs);
   for (i = 0; i < oldnrecs; i++)
      {
	 if (read(tfd, buf, rs) != rs)
	    goto err;
	 if (*buf == 0x20)
	    {
	       if (write(rd->file.fd, buf, rs) != rs)
		  goto err;
	       newnrecs++;
	    }
      }
   free(buf);
   buf = NULL;
   if (lseek(rd->file.fd, 0, SEEK_SET) == -1)
      goto err;
   if (read(rd->file.fd, &hdr, sizeof(DBF_HEADER)) != sizeof(DBF_HEADER))
      goto err;
   _rdd_put_uint(hdr.recs, newnrecs);
   if (lseek(rd->file.fd, 0, SEEK_SET) == -1)
      goto err;
   if (write(rd->file.fd, &hdr, sizeof(DBF_HEADER)) != sizeof(DBF_HEADER))
      goto err;
   return 0;
 err:
   if (buf)
      free(buf);
   return rdd_err(ClipMemoryStore, EG_WRITE, errno, __FILE__, __LINE__, __PROC__, er_ioerror);
}
