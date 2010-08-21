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
#include <ci_rdd.h>
#include <ci_btree.h>
#include <ci_error.ch>
#include <ci_dbinfo.ch>
#ifdef HAVE_MMAN_H
#include <sys/mman.h>
#endif

#include <ci_ncp.h>

#ifdef USE_NCPFS
#define fcntl ncp_fcntl
#endif

#include <dbf/typedef_struct_DBF_HEADER.h>
#include <dbf/typedef_struct_DBF_FIELD.h>

static RDD_DATA_VTBL *dbf_vtbl();

static RDD_DATA_VTBL *vfp_vtbl();

static RDD_DATA_VTBL *fox_vtbl();

static int dbf_open(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);

static int dbf_create(ClipMachine * ClipMachineMemory, RDD_DATA_VTBL * vtbl, char *name, RDD_STRUCT * rddstru, int nfields, const char *__PROC__);

static int dbf_pack(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int tfd, const char *__PROC__);

static int dbf_zap(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);

static int dbf_close(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);

static int dbf_lastrec(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *lastrec, const char *__PROC__);

static int dbf_deleted(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *deleted, const char *__PROC__);

static int dbf_gotop(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);

static int dbf_gobottom(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);

static int dbf_next(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int filtok, const char *__PROC__);

static int dbf_prev(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);

static int dbf_goto(ClipMachine * ClipMachineMemory, RDD_DATA * rd, unsigned int rec, const char *__PROC__);

static int dbf_rawgoto(ClipMachine * ClipMachineMemory, RDD_DATA * rd, unsigned int rec, char valid_stack, const char *__PROC__);

static int dbf_rlock(ClipMachine * ClipMachineMemory, RDD_DATA * rd, unsigned int rec, int *r, const char *__PROC__);

static int dbf_flock(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *r, const char *__PROC__);

static int dbf_ulock(ClipMachine * ClipMachineMemory, RDD_DATA * rd, unsigned int rec, int flock, const char *__PROC__);

static int dbf_rlocked(ClipMachine * ClipMachineMemory, RDD_DATA * rd, unsigned int rec, int *r, const char *__PROC__);

static int dbf_forlock(ClipMachine * ClipMachineMemory, RDD_DATA * rd, unsigned int rec, int *r, const char *__PROC__);

static int dbf__rlock(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);

static int dbf__wlock(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);

static int dbf__ulock(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);

static int dbf_append(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);

static int dbf_getrecord(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);

static int dbf_getvalue(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int no, ClipVar * vp, const char *__PROC__);

static int dbf_setrecord(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);

static int dbf_setvalue(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int no, ClipVar * vp, int binary, const char *__PROC__);

static int dbf_getmemoid(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int no, unsigned int *memoid, unsigned short int *vlen, const char *__PROC__);

static int dbf_setmemoid(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int no, unsigned int memoid, const char *__PROC__);

static int dbf_delete(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);

static int dbf_recall(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);

static int dbf_lupdate(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);

static int dbf_info(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int cmd, const char *__PROC__);

static int dbf_setstruct(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);

static int dbf_calcfiltlist(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_FILTER * fp, const char *__PROC__);

static int vfp_create(ClipMachine * ClipMachineMemory, RDD_DATA_VTBL * vtbl, char *name, RDD_STRUCT * rddstru, int nfields, const char *__PROC__);

static int dbf_verify(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *r, const char *__PROC__);

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

#include <dbf/clip_INIT_DBF.c>
#include <dbf/clip_INIT_VFP.c>
#include <dbf/clip_INIT_FOX.c>
#include <dbf/static_dbf_vtbl.c>
#include <dbf/static_vfp_vtbl.c>
#include <dbf/static_fox_vtbl.c>
#include <dbf/static_dbf_rlock.c>
#include <dbf/static_dbf_flock.c>
#include <dbf/static_dbf_ulock.c>
#include <dbf/static_dbf_rlocked.c>
#include <dbf/static_dbf_forlock.c>
#include <dbf/static_dbf__rlock.c>
#include <dbf/static_dbf__wlock.c>
#include <dbf/static_dbf__ulock.c>
#include <dbf/static_dbf_open.c>
#include <dbf/static_dbf_create.c>
#include <dbf/static_vfp_create.c>
#include <dbf/static_dbf_zap.c>
#include <dbf/static_dbf_close.c>
#include <dbf/static_dbf_lastrec.c>
#include <dbf/static_dbf_deleted.c>
#include <dbf/static_dbf_gotop.c>
#include <dbf/static_dbf_gobottom.c>
#include <dbf/static_dbf_next.c>
#include <dbf/static_dbf_prev.c>
#include <dbf/static_dbf_rawgoto.c>
#include <dbf/static_dbf_goto.c>
#include <dbf/static_dbf_append.c>
#include <dbf/static_dbf_getmemoid.c>
#include <dbf/static_dbf_setmemoid.c>
#include <dbf/static_dbf_getrecord.c>
#include <dbf/static_dbf_setrecord.c>
#include <dbf/static_dbf_verify.c>
#include <dbf/static_dbf_getvalue.c>
#include <dbf/dbf_setvalue.c>
#include <dbf/static_dbf_delete.c>
#include <dbf/static_dbf_recall.c>
#include <dbf/static_dbf_lupdate.c>
#include <dbf/static_dbf_info.c>
#include <dbf/static_dbf_setstruct.c>
#include <dbf/static__dbf_compare.c>
#include <dbf/static_dbf_calcfiltlist.c>
#include <dbf/static_dbf_pack.c>
