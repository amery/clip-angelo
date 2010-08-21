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
#include <ci_rdd.h>
#include <ci_hash.h>
#include <ci_error.ch>
#include <ci_hashcode.h>
#include <ci_rational.h>
#include <ci_six.ch>
#include <ci_dbinfo.ch>
#include <ci_dbstruct.ch>

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

#include <clipbase/static__rdd_takevalue.c>
#include <clipbase/static_is_dig.c>
#include <clipbase/static_is_id.c>
#include <clipbase/_clip_flushbuffer.c>
#include <clipbase/register_dbdriver.c>
#include <clipbase/db_driver.c>

#include <clipbase/clip_INIT_DBFNTX.c>
#include <clipbase/clip_INIT_DBFCTX.c>
#include <clipbase/clip_INIT_DBFCDX.c>
#include <clipbase/clip_INIT_SIXCDX.c>
#include <clipbase/clip_INIT_VFPCDX.c>
#include <clipbase/clip_INIT_DBFIDX.c>
#include <clipbase/clip_INIT_DBFCDX64.c>
#include <clipbase/clip_DBFNTX.c>
#include <clipbase/clip_DBFCTX.c>
#include <clipbase/clip_DBFCDX.c>
#include <clipbase/clip__DBFCDX.c>
#include <clipbase/clip_SIXCDX.c>
#include <clipbase/clip_VFPCDX.c>
#include <clipbase/clip_DBFIDX.c>
#include <clipbase/clip_DBFCDX64.c>

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
#include <clipbase/_clip_field.c>
#include <clipbase/_clip_try_field.c>
#include <clipbase/_clip_take_field.c>
#include <clipbase/_clip_fm_assign.c>
#include <clipbase/_clip_fm_iassign.c>
#include <clipbase/_clip_assign_field.c>
#include <clipbase/_clip_iassign_field.c>
#include <clipbase/_clip_push_area.c>
#include <clipbase/_clip_pop_area.c>
#include <clipbase/_clip_set_area.c>
#include <clipbase/_clip_close_all.c>

/* ] fields */
/* [ work area */
#include <clipbase/_clip_register_driver.c>
#include <clipbase/_clip_get_driver.c>
#include <clipbase/cur_area.c>
#include <clipbase/get_area.c>
#include <clipbase/static_close_area.c>
#include <clipbase/_clip_set_deleted.c>
#include <clipbase/_clip_relation_chields.c>

/* ] work area */
/* [ user interface */
#include <clipbase/clip_DBSETDRIVER.c>
#include <clipbase/clip__DBCREATE.c>
#include <clipbase/clip_DBACCESS.c>
#include <clipbase/clip_DBUSEAREA.c>
#include <clipbase/clip_DBCLOSEAREA.c>
#include <clipbase/clip_DBCLOSEALL.c>
#include <clipbase/clip_DBSELECTAREA.c>
#include <clipbase/clip_DBCREATEINDEX.c>
#include <clipbase/clip_DBCREATEIND.c>
#include <clipbase/clip_M6_CREATEINDEX.c>
#include <clipbase/clip_ORDCREATE.c>
#include <clipbase/clip_M6_ORDCREATE.c>
#include <clipbase/clip_DBSETINDEX.c>
#include <clipbase/clip_DBSETORDER.c>
#include <clipbase/clip_DBCLEARINDEX.c>
#include <clipbase/clip_DBCLEARIND.c>
#include <clipbase/clip_DBREINDEX.c>
#include <clipbase/clip___DBPACK.c>
#include <clipbase/clip___DBZAP.c>
#include <clipbase/clip_DBGOTOP.c>
#include <clipbase/clip_DBAPPEND.c>
#include <clipbase/clip_DBDELETE.c>
#include <clipbase/clip_DBRECALL.c>
#include <clipbase/clip_DBGOBOTTOM.c>
#include <clipbase/clip_DBGOTO.c>
#include <clipbase/clip_DBSKIP.c>
#include <clipbase/clip_DBSEEK.c>
#include <clipbase/clip___DBLOCATE.c>
#include <clipbase/clip___DBSETLOCATE.c>
#include <clipbase/clip___DBSETLOC.c>
#include <clipbase/clip___DBLOCATEOPTIMIZE.c>
#include <clipbase/clip_M6_DBLOCATE.c>
#include <clipbase/clip___DBCONTINUE.c>
#include <clipbase/clip_M6_DBCONTINUE.c>
#include <clipbase/clip_BOF.c>
#include <clipbase/clip_EOF.c>
#include <clipbase/clip_USED.c>
#include <clipbase/clip_DELETED.c>
#include <clipbase/clip_DELE.c>
#include <clipbase/clip_DELETE.c>
#include <clipbase/clip_RECNO.c>
#include <clipbase/clip_RECN.c>
#include <clipbase/clip_RECCOUNT.c>
#include <clipbase/clip_RECC.c>
#include <clipbase/clip_LASTREC.c>
#include <clipbase/clip_DBSTRUCT.c>
#include <clipbase/clip_AFIELDS.c>
#include <clipbase/clip_FCOUNT.c>
#include <clipbase/clip_FCOU.c>
#include <clipbase/clip_FCOUN.c>
#include <clipbase/clip_FIELDNAME.c>
#include <clipbase/clip_FIELD.c>
#include <clipbase/clip_FIELDPOS.c>
#include <clipbase/_clip_parse_name.c>
#include <clipbase/clip_ISFIELD.c>
#include <clipbase/_clip_nameassign.c>
#include <clipbase/clip_SETFIELD.c>
#include <clipbase/clip_FIELDGET.c>
#include <clipbase/clip_FIELDPUT.c>
#include <clipbase/clip_DBREAD.c>
#include <clipbase/clip_DBWRITE.c>
#include <clipbase/clip_ALIAS.c>
#include <clipbase/clip_DBF.c>
#include <clipbase/clip_SELECT.c>
#include <clipbase/clip_SELE.c>
#include <clipbase/clip_INDEXKEY.c>
#include <clipbase/clip_INDEXORD.c>
#include <clipbase/clip_RDDSETDEFAULT.c>
#include <clipbase/clip_INDEXEXT.c>
#include <clipbase/clip_ORDBAGEXT.c>
#include <clipbase/clip_TABLEEXT.c>
#include <clipbase/clip_MEMOEXT.c>
#include <clipbase/clip_RDDNAME.c>
#include <clipbase/clip_RDDLIST.c>
#include <clipbase/clip_LUPDATE.c>
#include <clipbase/clip_RECSIZE.c>
#include <clipbase/clip_HEADER.c>
#include <clipbase/_clip_filter.c>
#include <clipbase/clip_DBCLEARFILTER.c>
#include <clipbase/clip_DBSETFILTER.c>
#include <clipbase/clip_M6_SETFILTER.c>
#include <clipbase/clip_DBFILTER.c>
#include <clipbase/clip_ISFILTERED.c>
#include <clipbase/clip_DBRELATION.c>
#include <clipbase/clip_FOUND.c>
#include <clipbase/clip_FOUN.c>
#include <clipbase/clip_DBCLEARRELATION.c>
#include <clipbase/clip_DBCLEARREL.c>
#include <clipbase/clip_DBSETRELATION.c>
#include <clipbase/clip_RLOCK.c>
#include <clipbase/clip_DBRLOCK.c>
#include <clipbase/clip_DBRUNLOCK.c>
#include <clipbase/clip_DBUNLOCK.c>
#include <clipbase/clip_DBUNLOCKALL.c>
#include <clipbase/clip_FLOCK.c>
#include <clipbase/clip_NETERR.c>
#include <clipbase/clip_DBCOMMIT.c>
#include <clipbase/clip_DBCOMMITALL.c>
#include <clipbase/clip_DBRLOCKLIST.c>
#include <clipbase/clip_DBINFO.c>
#include <clipbase/clip_DBFIELDINFO.c>
#include <clipbase/clip_DBRECORDINFO.c>
#include <clipbase/get_orderno.c>
#include <clipbase/clip_DBORDERINFO.c>
#include <clipbase/clip_ORDSCOPE.c>
#include <clipbase/clip_ORDSETFOCUS.c>
#include <clipbase/clip_ORDBAGNAME.c>
#include <clipbase/clip_ORDCONDSET.c>
#include <clipbase/clip_M6_ORDCONDSET.c>
#include <clipbase/clip_FIELDDECI.c>
#include <clipbase/clip_FIELDNUM.c>
#include <clipbase/clip_FIELDSIZE.c>
#include <clipbase/clip_FIELDTYPE.c>
#include <clipbase/clip_FIELDBLANK.c>
#include <clipbase/clip_ORDKEYADD.c>
#include <clipbase/clip_ORDKEYDEL.c>
#include <clipbase/clip_ORDDESCEND.c>
#include <clipbase/clip_ORDDESTROY.c>
#include <clipbase/clip_ORDFOR.c>
#include <clipbase/clip_ORDISUNIQUE.c>
#include <clipbase/clip_ORDKEY.c>
#include <clipbase/clip_ORDKEYCOUNT.c>
#include <clipbase/clip_ORDKEYGOTO.c>
#include <clipbase/clip_ORDKEYNO.c>
#include <clipbase/clip_ORDKEYVAL.c>
#include <clipbase/clip_ORDLISTADD.c>
#include <clipbase/clip_ORDLISTCLEAR.c>
#include <clipbase/clip_ORDLISTREBUILD.c>
#include <clipbase/clip_ORDNAME.c>
#include <clipbase/clip_ORDNUMBER.c>
#include <clipbase/clip_ORDSETRELATION.c>
#include <clipbase/clip_ORDSKIPUNIQUE.c>
#include <clipbase/clip_DBRSELECT.c>

/* ] user interface */

/* [ Independed Index */

#include <clipbase/clip_II_CREATE.c>
#include <clipbase/clip_II_OPEN.c>
#include <clipbase/clip_II_CREATETAG.c>
#include <clipbase/clip_II_OPENTAG.c>
#include <clipbase/clip_II_CLOSE.c>
#include <clipbase/clip_II_CLOSETAG.c>
#include <clipbase/clip_II_ADDKEY.c>
#include <clipbase/clip_II_DELKEY.c>
#include <clipbase/clip_II_GOTOP.c>
#include <clipbase/clip_II_GOBOTTOM.c>
#include <clipbase/clip_II_BOF.c>
#include <clipbase/clip_II_EOF.c>
#include <clipbase/clip_II_ID.c>
#include <clipbase/clip_II_KEY.c>
#include <clipbase/clip_II_SKIP.c>

/* ] Independed Index */

#include <clipbase/clip_RDDHANDLE.c>
#include <clipbase/clip_GETAREA.c>
#include <clipbase/clip_GETALIAS.c>
#include <clipbase/clip___DBCOPYRAW.c>
#include <clipbase/clip___DBSETFOUND.c>
#include <clipbase/clip___DBSETFOU.c>
#include <clipbase/clip_CLIP_DBFTYPE.c>
#include <clipbase/clip_CLIP_MEMOTYPE.c>
#include <clipbase/clip_CLIP_USEDRDD.c>
#include <clipbase/clip_CLIP_ISMEMO.c>
/* FlagShip functions */
#include <clipbase/clip_ISDBEXCL.c>
#include <clipbase/clip_ISDBFLOCK.c>
#include <clipbase/clip_ISDBRLOCK.c>
#include <clipbase/clip_RLOCKVERIFY.c>
#include <clipbase/clip_CLIP_EVALSEEK.c>
#include <clipbase/clip__SEEK_EVAL.c>
/* end of FlagShip functions */
#include <clipbase/clip_ISDBRLOCKFOREIGN.c>
#include <clipbase/clip_ISDBRDONLY.c>
#include <clipbase/clip_CLIP_REGEXSEEK.c>
