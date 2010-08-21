/*
	Copyright (C) 2001  ITK
	Author  : Rushad Nabiullin (rust@itk.ru)
	Licence : (GPL) http://www.itk.ru/clipper/licence.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <ci_rdd.h>
#include <ci_error.ch>
#include <ci_btree.h>
#include <ci_clipcfg.h>
#ifdef HAVE_REGEX_H
#include <regex.h>
#endif

#define D20070109		/* spped up after "custom manage" */

extern int type_weight(int type);

static RDD_INDEX_VTBL *cdx_vtbl();

static RDD_INDEX_VTBL *idx_vtbl();

#include <cdx/typedef_struct_CDX_HEADER.h>
#include <cdx/typedef_struct_CDX_PAGE.h>
#include <cdx/typedef_struct_CDX_LEAF.h>
#include <cdx/typedef_struct_CDX_BRANCH.h>

#define _CDX_HEADER(ro,hdr) if((er = rdd_read(ClipMachineMemory,&ro->index->file,ro->header,sizeof(CDX_HEADER),hdr,__PROC__))) return er
#define _CDX_PAGE(ro,pageoffs,page)	if((er = rdd_read(ClipMachineMemory,&ro->index->file,pageoffs,sizeof(CDX_PAGE),page,__PROC__))) return er; \
		leaf = (_rdd_ushort((page)->attrs)&2)?(CDX_LEAF*)(page):NULL; \
		branch = (_rdd_ushort((page)->attrs)&2)?NULL:(CDX_BRANCH*)(page)
#define _CDX_LEAF_ITEM(item,rec,dup,trail) \
		rec = (unsigned int)((item) & _rdd_uint(leaf->recmask)); \
		(item) = (item) >> leaf->recbits; \
		dup = (unsigned char)((item) & leaf->dupmask); \
		(item) = (item) >> leaf->dupbits; \
		trail = (unsigned char)((item) & leaf->trailmask)

static int cdx_open(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *tag, const char *__PROC__);

static int cdx_close(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__);

static int cdx_create(ClipMachine * ClipMachineMemory, RDD_DATA * rd,
		      RDD_INDEX * ri, RDD_ORDER ** rop, const char *tag, const char *expr, ClipVar * block, int unique, int first, unsigned int header, const char *__PROC__);
static int cdx_reindex(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__);

static int cdx_zap(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__);

static int cdx_gotop(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__);

static int cdx_gobottom(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__);

static int cdx_next(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__);

static int cdx_prev(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__);

static int cdx_seek(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, ClipVar * v, int soft, int last, int *found, const char *__PROC__);

static int cdx_gotokey(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, unsigned int keyno, int *ok, const char *__PROC__);

static int cdx_addkey(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, ClipVar * v, const char *__PROC__);

static int cdx_formatkey(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, ClipVar * var, void *res, const char *__PROC__);

static int cdx_keyno(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, int *keyno, const char *__PROC__);

static int cdx_lastkey(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, int *lastkey, const char *__PROC__);

static int cdx_info(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, int cmd, const char *__PROC__);

static int cdx_keyvalue(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, ClipVar * v, const char *__PROC__);

static int cdx_setscope(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, ClipVar * top, ClipVar * bottom, unsigned int *map, int bytes, int exact, const char *__PROC__);

static int cdx_wildskip(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *pattern, int regular, const char *s, ClipVar * block, int *found, const char *__PROC__);

static int _cdx_addkey(ClipMachine * ClipMachineMemory, int area, DbfLocale * loc, ClipVar * v, RDD_ORDER * ro, void *key, int len, unsigned int recno, const char *__PROC__);

static int __cdx_addkey(ClipMachine * ClipMachineMemory, int area, RDD_ORDER * ro, ClipVar * v, DbfLocale * loc, unsigned int recno, const char *__PROC__);

#include <cdx/static_cdx__rlock.c>
#include <cdx/static_cdx__wlock.c>
#include <cdx/static_cdx__ulock.c>

static int _cdx_prev_rm(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, CDX_LEAF * leaf, int *out, int cache, const char *__PROC__);

static int _cdx_next_rm(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, CDX_LEAF * leaf, int *out, int cache, const char *__PROC__);

static int idx_open(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *tag, const char *__PROC__);

static int idx_create(ClipMachine * ClipMachineMemory, RDD_DATA * rd,
		      RDD_INDEX * ri, RDD_ORDER ** rop, const char *tag, const char *expr, ClipVar * block, int unique, int first, unsigned int header, const char *__PROC__);
static int idx_zap(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__);

static int idx_reindex(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__);

const unsigned int _cdx_bits[] = {
   0x00000001, 0x00000003, 0x00000007, 0x0000000f,
   0x0000001f, 0x0000003f, 0x0000007f, 0x000000ff,
   0x000001ff, 0x000003ff, 0x000007ff, 0x00000fff,
   0x00001fff, 0x00003fff, 0x00007fff, 0x0000ffff,
   0x0001ffff, 0x0003ffff, 0x0007ffff, 0x000fffff,
   0x001fffff, 0x003fffff, 0x007fffff, 0x00ffffff,
   0x01ffffff, 0x03ffffff, 0x07ffffff, 0x0fffffff,
   0x1fffffff, 0x3fffffff, 0x7fffffff, 0xffffffff
};

#define er_deepexceeded     _clip_gettext("MAX_BTREE_DEEP exceeded")
#define er_badroot          _clip_gettext("CDX internal error: bad root page")
#define er_badleaf          _clip_gettext("CDX internal error: bad leaf page")
#define er_nokeys           _clip_gettext("CDX internal error: no keys on the page")
#define er_badforexpr       _clip_gettext("Bad FOR expression")
#define er_badheader        _clip_gettext("Bad header")
#define er_notag            _clip_gettext("No such tag")
#define er_baddata          _clip_gettext("Bad data")
#define er_badwhileexpr     _clip_gettext("Bad WHILE expression")
#define er_badevalexpr      _clip_gettext("Bad EVAL expression")

#include <cdx/clip_INIT_CDX.c>
#include <cdx/clip_INIT_IDX.c>
#include <cdx/static__cdx_loadpage.c>
#include <cdx/static__cdx_getfreepage.c>
#include <cdx/static__cdx_putfreepage.c>
#include <cdx/static__cdx_init_order.c>
#include <cdx/static__cdx_init_rootleaf.c>
#include <cdx/static__cdx_dup_trail_rec.c>
#include <cdx/static__cdx_recno.c>
#include <cdx/static__cdx_leaf_item.c>
#include <cdx/static__cdx_recno_keyval.c>
#include <cdx/static__cdx_search_leaf_recno.c>
#include <cdx/static__cdx_cmp.c>
#include <cdx/static__cdx_search_leaf.c>
#include <cdx/static__cdx_search_tree.c>
#include <cdx/static__cdx_back_search_tree.c>
#include <cdx/static__cdx_first.c>
#include <cdx/static__cdx_last.c>
#include <cdx/static__cdx_next_rm.c>
#include <cdx/static__cdx_next.c>
#include <cdx/static__cdx_prev_rm.c>
#include <cdx/static__cdx_prev.c>
#include <cdx/static__cdx_checkscope.c>
#include <cdx/static__cdx_keyno.c>
#include <cdx/static__cdx_dupbytes.c>
#include <cdx/static__cdx_leaf2split.c>
#include <cdx/static__cdx_loadkeys.c>
#include <cdx/static__cdx_addnode.c>
#include <cdx/static__cdx_splitleaf.c>
#include <cdx/static__cdx_addroot.c>
#include <cdx/static__cdx_adjust_tree.c>
#include <cdx/static__cdx_addstick.c>
#include <cdx/static___cdx_addkey.c>
#include <cdx/static__cdx_addkey.c>
#include <cdx/static__cdx_delnode.c>
#include <cdx/static__cdx_delstick.c>
#include <cdx/static__cdx_delkey.c>
#include <cdx/static_cdx_open.c>
#include <cdx/static_cdx_close.c>
#include <cdx/static__cdx_init_leaf.c>
#include <cdx/static__cdx_init_branch.c>
#include <cdx/static__cdx_add.c>
#include <cdx/static___cdx_sets.c>
#include <cdx/static___cdx_adds.c>
#include <cdx/static__cdx_adds.c>
#include <cdx/static__cdx_savebtree.c>
#include <cdx/static__cdx_compare.c>

#define D20070119

#include <cdx/static__cdx_create.c>
#include <cdx/static_cdx_create.c>
#include <cdx/static_cdx_reindex.c>
#include <cdx/static_cdx_zap.c>
#include <cdx/static_cdx_gotop.c>
#include <cdx/static_cdx_gobottom.c>
#include <cdx/static_cdx_next.c>
#include <cdx/static_cdx_prev.c>
#include <cdx/static_cdx_seek.c>
#include <cdx/static_cdx_gotokey.c>
#include <cdx/static_cdx_descend.c>
#include <cdx/static_cdx_addkey.c>
#include <cdx/static_cdx_delkey.c>
#include <cdx/static__cdx_longlong.c>
#include <cdx/static__cdx_double.c>
#include <cdx/static__cdx_keyvar.c>
#include <cdx/static_cdx_formatkey.c>
#include <cdx/static_cdx_keyno.c>
#include <cdx/static_cdx_lastkey.c>
#include <cdx/static_cdx_info.c>
#include <cdx/static_cdx_keyvalue.c>
#include <cdx/static_cdx_setscope.c>
#include <cdx/static_cdx_destroy.c>
#include <cdx/static_cdx_ii_create.c>
#include <cdx/static_cdx_ii_createtag.c>
#include <cdx/static_cdx_ii_open.c>
#include <cdx/static_cdx_ii_opentag.c>
#include <cdx/static_cdx_ii_addkey.c>
#include <cdx/static_cdx_ii_delkey.c>
#include <cdx/static_cdx_ii_gotop.c>
#include <cdx/static_cdx_ii_gobottom.c>
#include <cdx/static_cdx_ii_id.c>
#include <cdx/static_cdx_ii_key.c>
#include <cdx/static_cdx_ii_next.c>
#include <cdx/static_cdx_ii_prev.c>
#include <cdx/static_cdx_calcfiltlist.c>
#include <cdx/static_cdx_buildtree.c>
#include <cdx/static_cdx_vtbl.c>
#include <cdx/static_idx_vtbl.c>
#include <cdx/static_idx_create.c>
#include <cdx/static_idx_open.c>
#include <cdx/static_idx_zap.c>
#include <cdx/static_idx_reindex.c>
#include <cdx/static_cdx_wildskip.c>
