/*
	Copyright (C) 2001  ITK
	Author  : Rushad Nabiullin <rust@itk.ru>
	License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI
*/
#include <string.h>
#include <limits.h>
#include <ci_rdd.h>
#include <ci_error.ch>

#define READLOCK	if((er = rd->vtbl->_rlock(ClipMachineMemory,rd,__PROC__))) goto err
#define WRITELOCK	if((er = rd->vtbl->_wlock(ClipMachineMemory,rd,__PROC__))) goto err
#define UNLOCK		if((er = rd->vtbl->_ulock(ClipMachineMemory,rd,__PROC__))) goto err

#define er_badinstance  _clip_gettext("Bad RDD instance")
#define er_nofield      _clip_gettext("No such field")
#define er_badargument2 _clip_gettext("Bad argument (2)")
#define er_badfilter    _clip_gettext("Bad filter handle")
#define er_noorder      _clip_gettext("No controlling order")

#include <rddclip/static__fetch_rdd.c>
#include <rddclip/clip_RDDUSEAREA.c>
#include <rddclip/clip_RDDCREATE.c>
#include <rddclip/clip___RDDZAP.c>
#include <rddclip/clip_RDDCREATEMEMO.c>
#include <rddclip/clip_RDDSETINDEX.c>
#include <rddclip/clip_RDDSETMEMO.c>
#include <rddclip/clip_RDDSETRELATION.c>
#include <rddclip/clip_RDDCREATEINDEX.c>
#include <rddclip/clip_RDDREINDEX.c>
#include <rddclip/clip_RDDCLEARINDEX.c>
#include <rddclip/clip_RDDSETORDER.c>
#include <rddclip/clip_RDDINDEXORD.c>
#include <rddclip/clip_RDDCLOSEAREA.c>
#include <rddclip/clip_RDDCLOSEALL.c>
#include <rddclip/clip_RDDSEEK.c>
#include <rddclip/clip_RDDGOTOP.c>
#include <rddclip/clip_RDDGOBOTTOM.c>
#include <rddclip/clip_RDDSKIP.c>
#include <rddclip/clip_RDDGOTO.c>
#include <rddclip/clip_RDDGOTOKEY.c>
#include <rddclip/clip_RDDBOF.c>
#include <rddclip/clip_RDDEOF.c>
#include <rddclip/clip_RDDRECNO.c>
#include <rddclip/clip_RDDLASTREC.c>
#include <rddclip/clip_RDDKEYNO.c>
#include <rddclip/clip_RDDLASTKEY.c>
#include <rddclip/clip_RDDGETVALUE.c>
#include <rddclip/clip_RDDSETVALUE.c>
#include <rddclip/clip_RDDREAD.c>
#include <rddclip/clip_RDDWRITE.c>
#include <rddclip/clip_RDDAPPEND.c>
#include <rddclip/clip_RDDSETFILTER.c>
#include <rddclip/clip_RDDFILTER.c>
#include <rddclip/clip_RDDCLEARFILTER.c>
#include <rddclip/clip_RDDDELETE.c>
#include <rddclip/clip_RDDRECALL.c>
#include <rddclip/clip_RDDDELETED.c>
#include <rddclip/clip_RDDINDEXKEY.c>
#include <rddclip/clip_RDDFCOUNT.c>
#include <rddclip/clip_RDDFIELDPOS.c>
#include <rddclip/clip_RDDFIELDNAME.c>
#include <rddclip/clip_RDDLOCATE.c>
#include <rddclip/clip_RDDCONTINUE.c>
#include <rddclip/clip_RDDSTRUCT.c>
#include <rddclip/clip_RDDORDSCOPE.c>
#include <rddclip/clip_RDDNEWFILTER.c>
#include <rddclip/clip_RDDSETAREAFILTER.c>
#include <rddclip/clip_RDDGETAREAFILTER.c>
#include <rddclip/clip_RDDRLOCK.c>
#include <rddclip/clip_RDDFLOCK.c>
#include <rddclip/clip_RDDUNLOCK.c>
#include <rddclip/clip_RDDFIELDDECI.c>
#include <rddclip/clip_RDDFIELDNUM.c>
#include <rddclip/clip_RDDFIELDSIZE.c>
#include <rddclip/clip_RDDFIELDTYPE.c>

/* SIX */

#include <rddclip/clip_RDD_M6_NEWFILTER.c>
#include <rddclip/clip_RDD_M6_SETAREAFILTER.c>
#include <rddclip/clip_RDD_M6_GETAREAFILTER.c>
#include <rddclip/clip_RDD_M6_ADDSCOPED.c>
