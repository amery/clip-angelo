/*
	Copyright (C) 2001  ITK
	Author  : Rushad Nabiullin <rust@itk.ru>
	License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ci_rdd.h>
#include <ci_error.ch>
#include <ci_dbinfo.ch>

#define FLEX_PAGESIZE   1024
#define FLEX_MAXLEAF    ((FLEX_PAGESIZE-10)/8)
#define FLEX_MAXPARENT  ((FLEX_PAGESIZE-10)/12)

#define CLIP_MEMO    0
#define SIX_MEMO     1
#define FLEX_MEMO    2

static RDD_MEMO_VTBL *fpt_vtbl();

static const char *er_corruption = "Corruption in .FPT file detected";

#include <fpt/clip_INIT_FPT.c>

#include <fpt/typedef_struct_FPT_HEADER.h>

#include <fpt/static_fpt_create.c>
#include <fpt/static_fpt_zap.c>
#include <fpt/static_fpt_open.c>
#include <fpt/static_fpt_close.c>
#include <fpt/static__read_six_str.c>
#include <fpt/static__read_six_int.c>
#include <fpt/static__read_six_double.c>
#include <fpt/static__read_six_date.c>
#include <fpt/static__read_six_log.c>
#include <fpt/static__read_six_array.c>
#include <fpt/static_fpt_getvalue.c>

#include <fpt/typedef_struct_FLEX_ITEM.h>

typedef struct _FLEX_TREE_ FLEX_TREE;

#include <fpt/struct__FLEX_TREE_.h>

static int _flex_splitbranch(ClipMachine * ClipMachineMemory, RDD_MEMO * rm, FLEX_TREE * branch, int size, const char *__PROC__);

static int _flex_remove(ClipMachine * ClipMachineMemory, RDD_MEMO * rm, FLEX_TREE * tree, int size, const char *__PROC__);

#include <fpt/static__flex_freetree.c>
#include <fpt/static__flex_newpage.c>
#include <fpt/static___flex_search.c>
#include <fpt/static__flex_search.c>
#include <fpt/static__flex_leaf.c>
#include <fpt/static__flex_first.c>
#include <fpt/static_.c>
#include <fpt/static__flex_next.c>
#include <fpt/static__flex_last.c>
#include <fpt/static___flex_prev.c>
#include <fpt/static__flex_prev.c>
#include <fpt/static__flex_exactsearch.c>
#include <fpt/static__flex_root.c>
#include <fpt/_flex_saveleaf.c>
#include <fpt/_flex_savebranch.c>
#include <fpt/static__flex_addstick.c>
#include <fpt/static__flex_splitbranch.c>
#include <fpt/static__flex_splitleaf.c>
#include <fpt/static__flex_setlastitem.c>
#include <fpt/static__flex_add.c>
#include <fpt/static__flex_freeblock.c>
#include <fpt/static_flex_freeblock.c>
#include <fpt/static__flex_removestick.c>
#include <fpt/static__flex_remove.c>
#include <fpt/static_flex_useblock.c>
#include <fpt/static__fpt_setvalue.c>
#include <fpt/static_fpt_setvalue.c>
#include <fpt/static_fpt_info.c>
#include <fpt/static_fpt_pack.c>
#include <fpt/static_fpt_getvchar.c>
#include <fpt/static_fpt_setvchar.c>
#include <fpt/static_fpt_vtbl.c>
