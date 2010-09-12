/*
	Copyright (C) 2001  ITK
	Author   : Rushad Nabiullin <rust@itk.ru>
	License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI
*/
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <ci_rdd.h>
#include <ci_six.ch>
#include <ci_error.ch>
#include <ci_set.ch>
#ifdef HAVE_MMAN_H
#include <sys/mman.h>
#endif

#define READLOCK	if((er = wa->rd->vtbl->_rlock(ClipMachineMemory,wa->rd,__PROC__))) goto err
#define WRITELOCK	if((er = wa->rd->vtbl->_wlock(ClipMachineMemory,wa->rd,__PROC__))) goto err
#define UNLOCK		if((er = wa->rd->vtbl->_ulock(ClipMachineMemory,wa->rd,__PROC__))) goto err

extern int get_orderno(DBWorkArea * wa, ClipVar * order, ClipVar * index);

extern DBWorkArea *cur_area(ClipMachine * ClipMachineMemory);

extern DBWorkArea *get_area(ClipMachine * ClipMachineMemory, long area, int any);

extern int clip_ORDKEYNO(ClipMachine * ClipMachineMemory);

#define er_badfilter        _clip_gettext("Bad filter handle")
#define er_invjoin          _clip_gettext("Invalid join type")
#define er_notpermitted     _clip_gettext("Operation not permitted")
#define er_nomemo           _clip_gettext("Memo file not opened")
#define er_nofield          _clip_gettext("No such field")
#define er_notmemo          _clip_gettext("Not memo field")
#define er_noorder          _clip_gettext("No controlling order")
#define er_notable          _clip_gettext("Workarea not in use")
#define er_readlock         _clip_gettext("Shared lock error")
#define er_writelock        _clip_gettext("Exclusive lock error")
#define er_create           _clip_gettext("Create file error")
#define er_open             _clip_gettext("Open file error")
#define er_write            _clip_gettext("File write error")
#define er_read             _clip_gettext("File read error")
#define er_badfltfile       _clip_gettext("Bad .flt file")

#include <six/clip_M6_NEWFILTER.c>
#include <six/clip_M6_SETAREAFILTER.c>
#include <six/clip_M6_GETAREAFILTER.c>
#include <six/clip_M6_FREEFILTER.c>
#include <six/clip_M6_FILTADDREC.c>
#include <six/clip_M6_FILTDROPREC.c>
#include <six/clip_M6_FILTCHGREC.c>
#include <six/clip_M6_ISFILTER.c>
#include <six/clip_M6_FILTJOIN.c>
#include <six/clip_M6_EVALPARTIAL.c>
#include <six/clip_M6_FILTINVERSE.c>
#include <six/clip_M6_FILTCOPY.c>
#include <six/clip_M6_FILTCOUNT.c>
#include <six/clip_M6_ISFILTREC.c>
#include <six/clip_M6_FILTINFO.c>
#include <six/clip_M6_RECCOUNT.c>
#include <six/clip_M6_ADDSCOPED.c>
#include <six/clip_M6_CHGOWNER.c>
#include <six/clip_M6_FILTSAVE.c>
#include <six/clip_M6_FILTRESTORE.c>
#include <six/clip_M6_ISOPTIMIZE.c>
#include <six/clip_M6_FILTGOREC.c>
#include <six/clip_M6_FILTSKIP.c>
#include <six/clip_M6_FILTTOP.c>
#include <six/clip_M6_FILTBOTT.c>
#include <six/clip_M6_REFRESHFILTER.c>
#include <six/clip_M6_VERSION.c>
#include <six/clip_M6_SETTEMP.c>
#include <six/clip_SX_BLOB2FILE.c>
#include <six/clip_SX_FILE2BLOB.c>
#include <six/clip_SX_MEMOPACK.c>
#include <six/clip_SX_SETTRIGGER.c>
#include <six/clip_RDD_COUNT.c>
#include <six/clip_RDD_NAME.c>
#include <six/clip_RDD_INFO.c>
#include <six/clip_SXCHAR.c>
#include <six/clip_SXNUM.c>
#include <six/clip_SXLOG.c>
#include <six/clip_SXDATE.c>
#include <six/clip_SX_CHILL.c>
#include <six/clip_SX_FREEZE.c>
#include <six/clip_SX_WARM.c>
#include <six/clip_SX_THERMOMETER.c>
#include <six/clip_SX_CLEARORDER.c>
#include <six/clip_SX_WILDMATCH.c>
#include <six/clip_SX_WILDSEEK.c>
#include <six/clip_SX_FNAMEPARSER.c>
#include <six/clip_SX_I_INDEXNAME.c>
#include <six/clip_SX_I_TAGNAME.c>
#include <six/clip_SX_ISREINDEX.c>
#include <six/clip_SX_INDEXCOUNT.c>
#include <six/clip_SX_INDEXNAME.c>
#include <six/clip_SX_TABLENAME.c>
#include <six/clip_SX_INDEXTYPE.c>
#include <six/clip_SX_ISFLOCKED.c>
#include <six/clip_SX_ISLOCKED.c>
#include <six/clip_SX_ISREADONLY.c>
#include <six/clip_SX_ISSHARED.c>
#include <six/clip_SX_KEYGOTO.c>
#include <six/clip_SX_KEYSINCLUDED.c>
#include <six/clip_SX_KILLTAG.c>
#include <six/clip_SX_LOCKRETRY.c>
#include <six/clip_SX_MEMOEXT.c>
#include <six/clip_SX_RLOCK.c>
#include <six/clip_SX_ROLLBACK.c>
#include <six/clip_SX_SEEKLAST.c>
#include <six/clip_SX_FILEORDER.c>
#include <six/clip_SX_SETFILEORD.c>
#include <six/clip_SX_SETTAG.c>
#include <six/clip_SX_SETTAGNO.c>
#include <six/clip_SX_SLIMFAST.c>
#include <six/clip_SX_STEP.c>
#include <six/clip_SX_TAGCOUNT.c>
#include <six/clip_SX_TAGNO.c>
#include <six/clip_SX_TAGINFO.c>
#include <six/clip_SX_TAGS.c>
#include <six/clip_SX_TAGUNIQUE.c>
#include <six/clip_SX_VSIGLEN.c>
#include <six/clip_SX_DESCEND.c>
#include <six/clip_SX_TABLETYPE.c>
#include <six/clip_SX_VERSION.c>
#include <six/clip_M6_SET.c>
#include <six/clip_M6_ERROR.c>
