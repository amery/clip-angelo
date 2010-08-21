/*
	Copyright (C) 2001  ITK
	Author  : Rushad Nabiullin <rust@itk.ru>
	License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/
#include <ci_clipcfg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#if defined(__GNUC__)
#include <unistd.h>
#endif
#include <ci_rdd.h>
#include <ci_btree.h>
#include <ci_error.ch>
#ifdef HAVE_MMAN_H
#include <sys/mman.h>
#endif

typedef unsigned char uchar;

typedef unsigned int u4;

#define er_unknowndata 	_clip_gettext("Unknown data type or bad size of item")
#define er_badtree 	_clip_gettext("Bad BTREE handle")
#define er_nofield 	_clip_gettext("No such field")
#define er_internal 	_clip_gettext("Internal error in BTREE module")

extern DBWorkArea *cur_area(ClipMachine * ClipMachineMemory);

#define CHECKBTREE(bt) \
	if(!(bt)){ \
		er = rdd_err(ClipMachineMemory,EG_ARG,0,__FILE__,__LINE__,__PROC__,er_badtree); \
		goto err; \
	}

#define NODE(offs) ((BT_NODE*)((uchar*)bt->data+(offs)))

#include <btree/static__bt_left_size.c>
#include <btree/static__bt_right_size.c>
#include <btree/static__bt_firstnode.c>
#include <btree/static__bt_lastnode.c>
#include <btree/static__bt_left_balance.c>
#include <btree/static__bt_right_balance.c>
#include <btree/static__bt_balance.c>
#include <btree/static__bt_adjust.c>
#include <btree/static__bt_free.c>
#include <btree/static__bt_getfuu.c>
#include <btree/bt_create.c>
#include <btree/bt_destroy.c>
#include <btree/bt_seek.c>
#include <btree/bt_del.c>
#include <btree/bt_add.c>
#include <btree/bt_first.c>
#include <btree/bt_last.c>
#include <btree/bt_next.c>
#include <btree/bt_prev.c>
#include <btree/bt_key.c>
#include <btree/static__comp_uint.c>
#include <btree/static_destroy_btree.c>
#include <btree/clip_BT_CREATE.c>
#include <btree/clip_BT_FIELDFILL.c>
#include <btree/clip_BT_INTERSECT.c>
#include <btree/clip_BT_COUNT.c>
#include <btree/clip_BT_FIRST.c>
#include <btree/clip_BT_LAST.c>
#include <btree/clip_BT_NEXT.c>
#include <btree/clip_BT_PREV.c>
#include <btree/clip_BT_DATA.c>
#include <btree/clip_BT_ADD.c>
#include <btree/clip_BT_DEL.c>
#include <btree/clip_BT_DESTROY.c>
#include <btree/static__bt_newpage.c>
#include <btree/bt1_create.c>
#include <btree/static__bt1_search.c>
#include <btree/bt1_seek.c>
#include <btree/static__bt1_add.c>
#include <btree/bt1_add.c>
#include <btree/static__bt1_first.c>
#include <btree/bt1_first.c>
#include <btree/static__bt1_last.c>
#include <btree/bt1_last.c>
#include <btree/bt1_next.c>
#include <btree/static__bt1_destroy.c>
#include <btree/bt1_destroy.c>
#include <btree/bt1_key.c>
