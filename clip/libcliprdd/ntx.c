/*
	Copyright (C) 2001  ITK
	Author  : Rushad Nabiullin <rust@itk.ru>
	License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI
*/
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <ci_rdd.h>
#include <ci_error.ch>
#include <ci_btree.h>
#ifdef HAVE_MMAN_H
#include <sys/mman.h>
#endif

#define MAX_BTREE_DEEP 64
#define MAX_TAG_LEN 10

#include <ntx/typedef_struct_NTX_HEADER.h>
#include <ntx/typedef_struct_CTX_TAG.h>
#include <ntx/typedef_struct_CTX_HEADER.h>
#include <ntx/typedef_struct_NTX_PAGE.h>
#include <ntx/typedef_struct_NTX_FREEPAGE.h>
#include <ntx/typedef_struct_NTX_BUCKET.h>

#define _ntx_header(ro,hdr) if((er = rdd_read(ClipMachineMemory,&ro->index->file,ro->header,sizeof(NTX_HEADER),(char*)hdr,__PROC__))) return er
#define _ntx_page(ro,pageoffs,page)	if((er = rdd_read(ClipMachineMemory,&ro->index->file,pageoffs,sizeof(NTX_PAGE),(char*)page,__PROC__))) return er
#define _ntx_buck(page,no) (NTX_BUCKET*)((char*)page+_rdd_ushort(&((page)->offs[(no)*2])))

static RDD_INDEX_VTBL *ntx_vtbl();

static RDD_INDEX_VTBL *ctx_vtbl();

static int ntx_open(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *tag, const char *__PROC__);

static int ntx_close(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__);

static int ntx_create(ClipMachine * ClipMachineMemory, RDD_DATA * rd,
		      RDD_INDEX * ri, RDD_ORDER ** rop, const char *tag, const char *expr, ClipVar * block, int unique, int first, unsigned int header, const char *__PROC__);
static int ntx_reindex(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__);

static int ntx_zap(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *__PROC__);

static int ntx_gotop(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__);

static int ntx_gobottom(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__);

static int ntx_next(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__);

static int ntx_prev(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__);

static int ntx_seek(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, ClipVar * v, int soft, int last, int *found, const char *__PROC__);

static int ntx_lastkey(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, int *lastkey, const char *__PROC__);

static int ntx_keyno(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, int *keyno, const char *__PROC__);

static int ntx_formatkey(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, ClipVar * var, void *res, const char *__PROC__);

static int _ntx_calc_key(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__);

static int _ntx_put_node(ClipMachine * ClipMachineMemory, RDD_DATA * rd,
			 RDD_ORDER * ro, NTX_PAGE * page, unsigned int pageoffs, char *key, unsigned int recno, unsigned int child, int buckno, int isright, int hostcs, const char *__PROC__);
static int _ntx_add_node(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, char *key, int recno, int child, const char *__PROC__);

static int ntx__rlock(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, const char *__PROC__);

static int ntx__wlock(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, const char *__PROC__);

static int ntx__ulock(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, const char *__PROC__);

#define er_unlock           _clip_gettext("Unlock error")
#define er_readlock         _clip_gettext("Shared lock error")
#define er_writelock        _clip_gettext("Exclusive lock error")
#define er_badforexpr       _clip_gettext("Bad FOR expression")
#define er_corruption       _clip_gettext("Index corrupted")
#define er_deepexceeded     _clip_gettext("MAX_BTREE_DEEP exceeded")
#define er_baddata          _clip_gettext("Bad data")
#define er_longkey          _clip_gettext("Key length is too long")
#define er_badwhileexpr     _clip_gettext("Bad WHILE expression")
#define er_badevalexpr      _clip_gettext("Bad EVAL expression")

#include <ntx/clip_INIT_NTX.c>
#include <ntx/clip_INIT_CTX.c>
#include <ntx/static_ntx__rlock.c>
#include <ntx/static_ntx__wlock.c>
#include <ntx/static_ntx__ulock.c>
#include <ntx/static__ntx_checkscope.c>
#include <ntx/static__ntx_compare.c>
#include <ntx/static__ntx_search_page.c>
#include <ntx/static__ntx_loadpage.c>
#include <ntx/static___ntx_last.c>
#include <ntx/static___ntx_first.c>
#include <ntx/static___ntx_prev.c>
#include <ntx/static___ntx_next.c>
#include <ntx/static___ntx_tuneseek.c>
#include <ntx/static___ntx_tuneprev.c>
#include <ntx/static___ntx_tunenext.c>
#include <ntx/static__ntx_search_tree.c>
#include <ntx/static__ntx_calc_key.c>
#include <ntx/static_ntx_formatkey.c>
#include <ntx/static__ntx_init_page.c>
#include <ntx/static__ntx_free_page.c>
#include <ntx/static__ntx_split_node.c>
#include <ntx/static__ntx_put_node.c>
#include <ntx/static__ntx_add_node.c>
#include <ntx/static_ntx_addkey.c>
#include <ntx/static__ntx_remove_node.c>
#include <ntx/static__ntx_put_free_page.c>
#include <ntx/static__ntx_balance.c>
#include <ntx/static__ntx_find_last.c>
#include <ntx/static__ntx_del_node.c>
#include <ntx/static_ntx_delkey.c>
#include <ntx/static_ntx_open.c>
#include <ntx/static_ntx_close.c>
#include <ntx/static__ntx_comp.c>
#include <ntx/static__ntx_savebtree.c>
#include <ntx/static_ntx_create.c>
#include <ntx/static__ntx_reindex.c>
#include <ntx/static_ntx_reindex.c>
#include <ntx/static_ntx_zap.c>
#include <ntx/static__ntx_first.c>
#include <ntx/static__ntx_last.c>
#include <ntx/static_ntx_gotop.c>
#include <ntx/static_ntx_gobottom.c>
#include <ntx/static_ntx_next.c>
#include <ntx/static_ntx_prev.c>
#include <ntx/static_ntx_seek.c>
#include <ntx/static__ntx_leftkeys.c>
#include <ntx/static_ntx_keyno.c>
#include <ntx/static_ntx_lastkey.c>
#include <ntx/static_ntx_gotokey.c>
#include <ntx/static_ntx_descend.c>
#include <ntx/static_ntx_keyvalue.c>
#include <ntx/static_ntx_info.c>
#include <ntx/static_ntx_destroy.c>
#include <ntx/static_ntx_buildtree.c>
#include <ntx/static_ntx_vtbl.c>
#include <ntx/static__ctx_openorder.c>
#include <ntx/static_ctx_open.c>
#include <ntx/static_ctx_close.c>
#include <ntx/static_ctx_zap.c>
#include <ntx/static_ctx_reindex.c>
#include <ntx/static_ctx_create.c>
#include <ntx/static_ctx_destroy.c>
#include <ntx/static_ctx_vtbl.c>
