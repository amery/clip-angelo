/*
    Copyright (C) 2001  ITK
    Author   : Alexey (alexey@itk.ru)
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
**********************************************************************
#ifdef _CHECK_HANDLE
  #define __CHECKING_HANDLE         if ::handle==NIL; return NIL; endif
#else
  #define __CHECKING_HANDLE
#endif
**********************************************************************

#include "ci_rdd.ch"

#include "ci_box.ch"
#include "ci_Inkey.ch"
#include "ci_Setcurs.ch"
#include "ci_Error.ch"
#include "ci_config.ch"
#include "ci_dbedit.ch"


#define MY_HEADSEP      "���"
#define MY_COLSEP       " � "
**********************************************************************
#include "rdd_obj/static_ApplyKey.ch"
#include "rdd_obj/static_rddobj_append.ch"
#include "rdd_obj/static_rddobj_bof.ch"
#include "rdd_obj/static_rddobj_browse.ch"
#include "rdd_obj/static_rddobj_browse_FancyColors.ch"
#include "rdd_obj/static_rddobj_browse_skipper.ch"
#include "rdd_obj/static_rddobj_browse_StockBrowseNew.ch"
#include "rdd_obj/static_rddobj_clearfilter.ch"
#include "rdd_obj/static_rddobj_clearindex.ch"
#include "rdd_obj/static_rddobj_close.ch"
#include "rdd_obj/static_rddobj_continue.ch"
#include "rdd_obj/static_rddobj_create.ch"
#include "rdd_obj/static_rddobj_createindex.ch"
#include "rdd_obj/static_rddobj_creatememo.ch"
#include "rdd_obj/static_rddobj_delete.ch"
#include "rdd_obj/static_rddobj_deleted.ch"
#include "rdd_obj/static_rddobj_destroy.ch"
#include "rdd_obj/static_rddobj_eof.ch"
#include "rdd_obj/static_rddobj_fcount.ch"
#include "rdd_obj/static_rddobj_fieldname.ch"
#include "rdd_obj/static_rddobj_fieldpos.ch"
#include "rdd_obj/static_rddobj_found.ch"
#include "rdd_obj/static_rddobj_getareafilter.ch"
#include "rdd_obj/static_rddobj_getvalue.ch"
#include "rdd_obj/static_rddobj_gobottom.ch"
#include "rdd_obj/static_rddobj_goto.ch"
#include "rdd_obj/static_rddobj_gotokey.ch"
#include "rdd_obj/static_rddobj_gotop.ch"
#include "rdd_obj/static_rddobj_indexkey.ch"
#include "rdd_obj/static_rddobj_keyno.ch"
#include "rdd_obj/static_rddobj_lastkey.ch"
#include "rdd_obj/static_rddobj_lastrec.ch"
#include "rdd_obj/static_rddobj_locate.ch"
#include "rdd_obj/static_rddobj_newfilter.ch"
#include "rdd_obj/static_rddobj_open.ch"
#include "rdd_obj/static_rddobj_ordscope.ch"
#include "rdd_obj/static_rddobj_read.ch"
#include "rdd_obj/static_rddobj_recall.ch"
#include "rdd_obj/static_rddobj_recno.ch"
#include "rdd_obj/static_rddobj_reindex.ch"
#include "rdd_obj/static_rddobj_rlock.ch"
#include "rdd_obj/static_rddobj_runlock.ch"
#include "rdd_obj/static_rddobj_seek.ch"
#include "rdd_obj/static_rddobj_setareafilter.ch"
#include "rdd_obj/static_rddobj_setfilter.ch"
#include "rdd_obj/static_rddobj_setindex.ch"
#include "rdd_obj/static_rddobj_setmemo.ch"
#include "rdd_obj/static_rddobj_setorder.ch"
#include "rdd_obj/static_rddobj_setrelation.ch"
#include "rdd_obj/static_rddobj_setscope.ch"
#include "rdd_obj/static_rddobj_setvalue.ch"
#include "rdd_obj/static_rddobj_skip.ch"
#include "rdd_obj/static_rddobj_struct.ch"
#include "rdd_obj/static_rddobj_write.ch"
#include "rdd_obj/static_rddobj_zap.ch"
**********************************************************************
#include "rdd_obj/_recover_RDD.ch"
#include "rdd_obj/RDDNew.ch"
**********************************************************************
