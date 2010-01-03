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

#include "clipbase/static__rdd_takevalue.h"
#include "clipbase/static_is_dig.h"
#include "clipbase/static_is_id.h"
#include "clipbase/_clip_flushbuffer.h"
#include "clipbase/register_dbdriver.h"
#include "clipbase/db_driver.h"

#include "clipbase/clip_INIT_DBFNTX.h"
#include "clipbase/clip_INIT_DBFCTX.h"
#include "clipbase/clip_INIT_DBFCDX.h"
#include "clipbase/clip_INIT_SIXCDX.h"
#include "clipbase/clip_INIT_VFPCDX.h"
#include "clipbase/clip_INIT_DBFIDX.h"
#include "clipbase/clip_INIT_DBFCDX64.h"
#include "clipbase/clip_DBFNTX.h"
#include "clipbase/clip_DBFCTX.h"
#include "clipbase/clip_DBFCDX.h"
#include "clipbase/clip__DBFCDX.h"
#include "clipbase/clip_SIXCDX.h"
#include "clipbase/clip_VFPCDX.h"
#include "clipbase/clip_DBFIDX.h"
#include "clipbase/clip_DBFCDX64.h"

/* [ fields */
#if 0
static ClipVar *
space_var(ClipMachine * ClipMachineMemory, int line, long area, long hash)
{
   HashTable *hp = _clip_space(ClipMachineMemory, area);

   ClipVar  *vp;

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
#include "clipbase/_clip_field.h"
#include "clipbase/_clip_try_field.h"
#include "clipbase/_clip_take_field.h"
#include "clipbase/_clip_fm_assign.h"
#include "clipbase/_clip_fm_iassign.h"
#include "clipbase/_clip_assign_field.h"
#include "clipbase/_clip_iassign_field.h"
#include "clipbase/_clip_push_area.h"
#include "clipbase/_clip_pop_area.h"
#include "clipbase/_clip_set_area.h"
#include "clipbase/_clip_close_all.h"

/* ] fields */
/* [ work area */
#include "clipbase/_clip_register_driver.h"
#include "clipbase/_clip_get_driver.h"
#include "clipbase/cur_area.h"
#include "clipbase/get_area.h"
#include "clipbase/static_close_area.h"
#include "clipbase/_clip_set_deleted.h"
#include "clipbase/_clip_relation_chields.h"

/* ] work area */
/* [ user interface */
#include "clipbase/clip_DBSETDRIVER.h"
#include "clipbase/clip__DBCREATE.h"
#include "clipbase/clip_DBACCESS.h"
#include "clipbase/clip_DBUSEAREA.h"
#include "clipbase/clip_DBCLOSEAREA.h"
#include "clipbase/clip_DBCLOSEALL.h"
#include "clipbase/clip_DBSELECTAREA.h"
#include "clipbase/clip_DBCREATEINDEX.h"
#include "clipbase/clip_DBCREATEIND.h"
#include "clipbase/clip_M6_CREATEINDEX.h"
#include "clipbase/clip_ORDCREATE.h"
#include "clipbase/clip_M6_ORDCREATE.h"
#include "clipbase/clip_DBSETINDEX.h"
#include "clipbase/clip_DBSETORDER.h"
#include "clipbase/clip_DBCLEARINDEX.h"
#include "clipbase/clip_DBCLEARIND.h"
#include "clipbase/clip_DBREINDEX.h"
#include "clipbase/clip___DBPACK.h"
#include "clipbase/clip___DBZAP.h"
#include "clipbase/clip_DBGOTOP.h"
#include "clipbase/clip_DBAPPEND.h"
#include "clipbase/clip_DBDELETE.h"
#include "clipbase/clip_DBRECALL.h"
#include "clipbase/clip_DBGOBOTTOM.h"
#include "clipbase/clip_DBGOTO.h"
#include "clipbase/clip_DBSKIP.h"
#include "clipbase/clip_DBSEEK.h"
#include "clipbase/clip___DBLOCATE.h"
#include "clipbase/clip___DBSETLOCATE.h"
#include "clipbase/clip___DBSETLOC.h"
#include "clipbase/clip___DBLOCATEOPTIMIZE.h"
#include "clipbase/clip_M6_DBLOCATE.h"
#include "clipbase/clip___DBCONTINUE.h"
#include "clipbase/clip_M6_DBCONTINUE.h"
#include "clipbase/clip_BOF.h"
#include "clipbase/clip_EOF.h"
#include "clipbase/clip_USED.h"
#include "clipbase/clip_DELETED.h"
#include "clipbase/clip_DELE.h"
#include "clipbase/clip_DELETE.h"
#include "clipbase/clip_RECNO.h"
#include "clipbase/clip_RECN.h"
#include "clipbase/clip_RECCOUNT.h"
#include "clipbase/clip_RECC.h"
#include "clipbase/clip_LASTREC.h"
#include "clipbase/clip_DBSTRUCT.h"
#include "clipbase/clip_AFIELDS.h"
#include "clipbase/clip_FCOUNT.h"
#include "clipbase/clip_FCOU.h"
#include "clipbase/clip_FCOUN.h"
#include "clipbase/clip_FIELDNAME.h"
#include "clipbase/clip_FIELD.h"
#include "clipbase/clip_FIELDPOS.h"
#include "clipbase/_clip_parse_name.h"
#include "clipbase/clip_ISFIELD.h"
#include "clipbase/_clip_nameassign.h"
#include "clipbase/clip_SETFIELD.h"
#include "clipbase/clip_FIELDGET.h"
#include "clipbase/clip_FIELDPUT.h"
#include "clipbase/clip_DBREAD.h"
#include "clipbase/clip_DBWRITE.h"
#include "clipbase/clip_ALIAS.h"
#include "clipbase/clip_DBF.h"
#include "clipbase/clip_SELECT.h"
#include "clipbase/clip_SELE.h"
#include "clipbase/clip_INDEXKEY.h"
#include "clipbase/clip_INDEXORD.h"
#include "clipbase/clip_RDDSETDEFAULT.h"
#include "clipbase/clip_INDEXEXT.h"
#include "clipbase/clip_ORDBAGEXT.h"
#include "clipbase/clip_TABLEEXT.h"
#include "clipbase/clip_MEMOEXT.h"
#include "clipbase/clip_RDDNAME.h"
#include "clipbase/clip_RDDLIST.h"
#include "clipbase/clip_LUPDATE.h"
#include "clipbase/clip_RECSIZE.h"
#include "clipbase/clip_HEADER.h"
#include "clipbase/_clip_filter.h"
#include "clipbase/clip_DBCLEARFILTER.h"
#include "clipbase/clip_DBSETFILTER.h"
#include "clipbase/clip_M6_SETFILTER.h"
#include "clipbase/clip_DBFILTER.h"
#include "clipbase/clip_ISFILTERED.h"
#include "clipbase/clip_DBRELATION.h"
#include "clipbase/clip_FOUND.h"
#include "clipbase/clip_FOUN.h"
#include "clipbase/clip_DBCLEARRELATION.h"
#include "clipbase/clip_DBCLEARREL.h"
#include "clipbase/clip_DBSETRELATION.h"
#include "clipbase/clip_RLOCK.h"
#include "clipbase/clip_DBRLOCK.h"
#include "clipbase/clip_DBRUNLOCK.h"
#include "clipbase/clip_DBUNLOCK.h"
#include "clipbase/clip_DBUNLOCKALL.h"
#include "clipbase/clip_FLOCK.h"
#include "clipbase/clip_NETERR.h"
#include "clipbase/clip_DBCOMMIT.h"
#include "clipbase/clip_DBCOMMITALL.h"
#include "clipbase/clip_DBRLOCKLIST.h"
#include "clipbase/clip_DBINFO.h"
#include "clipbase/clip_DBFIELDINFO.h"
#include "clipbase/clip_DBRECORDINFO.h"
#include "clipbase/get_orderno.h"
#include "clipbase/clip_DBORDERINFO.h"
#include "clipbase/clip_ORDSCOPE.h"
#include "clipbase/clip_ORDSETFOCUS.h"
#include "clipbase/clip_ORDBAGNAME.h"
#include "clipbase/clip_ORDCONDSET.h"
#include "clipbase/clip_M6_ORDCONDSET.h"
#include "clipbase/clip_FIELDDECI.h"
#include "clipbase/clip_FIELDNUM.h"
#include "clipbase/clip_FIELDSIZE.h"
#include "clipbase/clip_FIELDTYPE.h"
#include "clipbase/clip_FIELDBLANK.h"
#include "clipbase/clip_ORDKEYADD.h"
#include "clipbase/clip_ORDKEYDEL.h"
#include "clipbase/clip_ORDDESCEND.h"
#include "clipbase/clip_ORDDESTROY.h"
#include "clipbase/clip_ORDFOR.h"
#include "clipbase/clip_ORDISUNIQUE.h"
#include "clipbase/clip_ORDKEY.h"
#include "clipbase/clip_ORDKEYCOUNT.h"
#include "clipbase/clip_ORDKEYGOTO.h"
#include "clipbase/clip_ORDKEYNO.h"
#include "clipbase/clip_ORDKEYVAL.h"
#include "clipbase/clip_ORDLISTADD.h"
#include "clipbase/clip_ORDLISTCLEAR.h"
#include "clipbase/clip_ORDLISTREBUILD.h"
#include "clipbase/clip_ORDNAME.h"
#include "clipbase/clip_ORDNUMBER.h"
#include "clipbase/clip_ORDSETRELATION.h"
#include "clipbase/clip_ORDSKIPUNIQUE.h"
#include "clipbase/clip_DBRSELECT.h"

/* ] user interface */

/* [ Independed Index */

#include "clipbase/clip_II_CREATE.h"
#include "clipbase/clip_II_OPEN.h"
#include "clipbase/clip_II_CREATETAG.h"
#include "clipbase/clip_II_OPENTAG.h"
#include "clipbase/clip_II_CLOSE.h"
#include "clipbase/clip_II_CLOSETAG.h"
#include "clipbase/clip_II_ADDKEY.h"
#include "clipbase/clip_II_DELKEY.h"
#include "clipbase/clip_II_GOTOP.h"
#include "clipbase/clip_II_GOBOTTOM.h"
#include "clipbase/clip_II_BOF.h"
#include "clipbase/clip_II_EOF.h"
#include "clipbase/clip_II_ID.h"
#include "clipbase/clip_II_KEY.h"
#include "clipbase/clip_II_SKIP.h"

/* ] Independed Index */

#include "clipbase/clip_RDDHANDLE.h"
#include "clipbase/clip_GETAREA.h"
#include "clipbase/clip_GETALIAS.h"
#include "clipbase/clip___DBCOPYRAW.h"
#include "clipbase/clip___DBSETFOUND.h"
#include "clipbase/clip___DBSETFOU.h"
#include "clipbase/clip_CLIP_DBFTYPE.h"
#include "clipbase/clip_CLIP_MEMOTYPE.h"
#include "clipbase/clip_CLIP_USEDRDD.h"
#include "clipbase/clip_CLIP_ISMEMO.h"
/* FlagShip functions */
#include "clipbase/clip_ISDBEXCL.h"
#include "clipbase/clip_ISDBFLOCK.h"
#include "clipbase/clip_ISDBRLOCK.h"
#include "clipbase/clip_RLOCKVERIFY.h"
#include "clipbase/clip_CLIP_EVALSEEK.h"
#include "clipbase/clip__SEEK_EVAL.h"
/* end of FlagShip functions */
#include "clipbase/clip_ISDBRLOCKFOREIGN.h"
#include "clipbase/clip_ISDBRDONLY.h"
#include "clipbase/clip_CLIP_REGEXSEEK.h"
