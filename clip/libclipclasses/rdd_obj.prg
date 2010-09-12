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

#include <ci_rdd.ch>

#include <ci_box.ch>
#include <ci_Inkey.ch>
#include <ci_Setcurs.ch>
#include <ci_Error.ch>
#include <ci_config.ch>
#include <ci_dbedit.ch>


#define MY_HEADSEP      "���"
#define MY_COLSEP       " � "
**********************************************************************
#include <rdd_obj/static_ApplyKey.prg>
#include <rdd_obj/static_rddobj_append.prg>
#include <rdd_obj/static_rddobj_bof.prg>
#include <rdd_obj/static_rddobj_browse.prg>
#include <rdd_obj/static_rddobj_browse_FancyColors.prg>
#include <rdd_obj/static_rddobj_browse_skipper.prg>
#include <rdd_obj/static_rddobj_browse_StockBrowseNew.prg>
#include <rdd_obj/static_rddobj_clearfilter.prg>
#include <rdd_obj/static_rddobj_clearindex.prg>
#include <rdd_obj/static_rddobj_close.prg>
#include <rdd_obj/static_rddobj_continue.prg>
#include <rdd_obj/static_rddobj_create.prg>
#include <rdd_obj/static_rddobj_createindex.prg>
#include <rdd_obj/static_rddobj_creatememo.prg>
#include <rdd_obj/static_rddobj_delete.prg>
#include <rdd_obj/static_rddobj_deleted.prg>
#include <rdd_obj/static_rddobj_destroy.prg>
#include <rdd_obj/static_rddobj_eof.prg>
#include <rdd_obj/static_rddobj_fcount.prg>
#include <rdd_obj/static_rddobj_fieldname.prg>
#include <rdd_obj/static_rddobj_fieldpos.prg>
#include <rdd_obj/static_rddobj_found.prg>
#include <rdd_obj/static_rddobj_getareafilter.prg>
#include <rdd_obj/static_rddobj_getvalue.prg>
#include <rdd_obj/static_rddobj_gobottom.prg>
#include <rdd_obj/static_rddobj_goto.prg>
#include <rdd_obj/static_rddobj_gotokey.prg>
#include <rdd_obj/static_rddobj_gotop.prg>
#include <rdd_obj/static_rddobj_indexkey.prg>
#include <rdd_obj/static_rddobj_keyno.prg>
#include <rdd_obj/static_rddobj_lastkey.prg>
#include <rdd_obj/static_rddobj_lastrec.prg>
#include <rdd_obj/static_rddobj_locate.prg>
#include <rdd_obj/static_rddobj_newfilter.prg>
#include <rdd_obj/static_rddobj_open.prg>
#include <rdd_obj/static_rddobj_ordscope.prg>
#include <rdd_obj/static_rddobj_read.prg>
#include <rdd_obj/static_rddobj_recall.prg>
#include <rdd_obj/static_rddobj_recno.prg>
#include <rdd_obj/static_rddobj_reindex.prg>
#include <rdd_obj/static_rddobj_rlock.prg>
#include <rdd_obj/static_rddobj_runlock.prg>
#include <rdd_obj/static_rddobj_seek.prg>
#include <rdd_obj/static_rddobj_setareafilter.prg>
#include <rdd_obj/static_rddobj_setfilter.prg>
#include <rdd_obj/static_rddobj_setindex.prg>
#include <rdd_obj/static_rddobj_setmemo.prg>
#include <rdd_obj/static_rddobj_setorder.prg>
#include <rdd_obj/static_rddobj_setrelation.prg>
#include <rdd_obj/static_rddobj_setscope.prg>
#include <rdd_obj/static_rddobj_setvalue.prg>
#include <rdd_obj/static_rddobj_skip.prg>
#include <rdd_obj/static_rddobj_struct.prg>
#include <rdd_obj/static_rddobj_write.prg>
#include <rdd_obj/static_rddobj_zap.prg>
**********************************************************************
#include <rdd_obj/_recover_RDD.prg>
#include <rdd_obj/RDDNew.prg>
**********************************************************************
