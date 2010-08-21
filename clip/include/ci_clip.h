/*
	Copyright (C) 2001  ITK
	Author   : Paul Lasarev <paul@itk.ru>
	License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI
*/
#ifndef CN_CLIP_H
#define CN_CLIP_H
#include "ci_clipcfg.h"
/*
#if defined(OS_MINGW)
	#include "ci__win32.h"
#else
	#define INT64 long long
#endif
*/
/*#define MEMDEBUG */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/time.h>
#ifdef __FreeBSD__
#include <unistd.h>
#endif
#if defined(__GNUC__)
#include "ci_clipcfg.h"
#else
#include "ci_wintypes.h"
#endif
#ifdef MEMDEBUG
#ifdef MALLOC_FUNC_CHECK
#include "memdebug/malloc_str.h"
#endif
#include "memdebug/malloc.h"
#endif
#ifdef USE_THREADS
#include <pthread.h>
#endif

#define SYSERRLEN	128

#define _C_ITEM_TYPE_BTREE					14
#define _C_ITEM_TYPE_CORBA             12
#define _C_ITEM_TYPE_CORBA_INTERFACE   13
#define _C_ITEM_TYPE_DATETIME				10
#define _C_ITEM_TYPE_FILE					1
#define _C_ITEM_TYPE_GDLIB 				6
#define _C_ITEM_TYPE_GTK_TIMEOUT			15
#define _C_ITEM_TYPE_HIPER					11
#define _C_ITEM_TYPE_I_INDEX				8
#define _C_ITEM_TYPE_I_ORDER				9
#define _C_ITEM_TYPE_RDD					3
#define _C_ITEM_TYPE_REGEX					5
#define _C_ITEM_TYPE_RYO					7
#define _C_ITEM_TYPE_SQL					2
#define _C_ITEM_TYPE_UNKNOWN				0
#define _C_ITEM_TYPE_WIDGET 				4
#define _C_ITEM_TYPE_XML_PARSER			16

#define CLIP_CUR_DRIVE						0x3fffffd0
#define CLIP_MAX_ERRORBLOCK_DEEP 		128
#define CLIP_MAX_HISTORY 					32
#define CLIP_MAX_PRINT_DEEP 				16
#define CLIP_OS_NAME 						"Linux"
#define MIN_NUMERIC_DIFF 					0.00000000000001

/* FILESEEK internal buffer */
#define CLIP_FSDATA_HASH_KEY 0x4ef45399	/* hashstr(CLIP_FSDATA_HASH_KEY) */

/* flags */
#define ALTERNATE_FLAG		0x200
#define BELL_FLAG 			0x2000
#define CANCEL_FLAG			0x40
#define CENTURY_FLAG 		0x1000000
#define CONFIRM_FLAG			0x4000
#define CONSOLE_FLAG			0x100
#define DEBUG_FLAG			0x80
#define DELETED_FLAG			0x20
#define DELIMITERS_FLAG		0x100000
#define DEVICE_FLAG			0x400
#define ESCAPE_FLAG			0x8000
#define EXACT_FLAG			0x1
#define EXCLUSIVE_FLAG		0x4
#define EXIT_FLAG				0x20000
#define EXTRA_FLAG			0x800
#define FIXED_FLAG			0x2
#define INSERT_FLAG 			0x10000
#define INTENSITY_FLAG		0x40000
#define MCENTER_FLAG			0x400000
#define PRINTER_FLAG			0x1000
#define RATIONAL_FLAG		0x2000000
#define SCOREBOARD_FLAG		0x80000
#define SCROLLBREAK_FLAG 	0x800000
#define SOFTSEEK_FLAG		0x8
#define TRANSLATE_FLAG		0x4000000
#define UNIQUE_FLAG			0x10
#define WRAP_FLAG	 			0x200000

/* flags1 */
#define AUTOPEN_FLAG        0x1
#define BUFFERING_FLAG      0x40
#define DISPBOX_FLAG        0x8
#define FLUSHOUT_FLAG	    0x200
#define FOPENMODE_FLAG      0x20
#define IDLEINKEY_FLAG	    0x800
#define MAP_FILE_FLAG       0x80
#define MAPERR_FLAG         0x4
#define MULTILOCKS_FLAG     0x100
#define NOEXPAND_MACRO_FLAG 0x10
#define OPTIMIZE_FLAG       0x2
#define UTF8TERM_FLAG	    0x400

#define DEFAULT_COLOR "W/N,N/W,N,N,N/W"

/* cursor types */
/* lower half */
#define SC_INSERT		2
#define SC_NONE		0
#define SC_NORMAL		1
/* full block */
#define SC_SPECIAL1	3
/* upper half */
#define SC_SPECIAL2	4

/* return codes */
#define CLIP_INVARGC	1
#define CLIP_INVARGV	2
#define CLIP_SUCCESS	0
#define CLIP_TRAP		3

/* event_mask values (from inkey.ch) */
#define _CLIP_INKEY_ALL         255	/* 159 */
#define _CLIP_INKEY_KEYBOARD    128	/*  Keyboard events              */
#define _CLIP_INKEY_LDOWN         2	/*  Mouse left button down       */
#define _CLIP_INKEY_LUP           4	/*  Mouse left button up         */
#define _CLIP_INKEY_MDOWN        32	/*  Mouse middle button down      */
#define _CLIP_INKEY_MOVE          1	/*  Mouse movements              */
#define _CLIP_INKEY_MUP          64	/*  Mouse middle button up        */
#define _CLIP_INKEY_RDOWN         8	/*  Mouse right button down      */
#define _CLIP_INKEY_RUP          16	/*  Mouse right button up        */

#define MACRO len
#define CLIP_EPOCH_DEFAULT 1930
#define CLIP_DATEFORMAT_DEFAULT "mm/dd/yyyy"
/* if dateformat have 4th "Y" - century have "true" */
#define CLIP_CENTURY_DEFAULT 1

#define ARGPTR(ClipMachineMemory,num) ((ClipMachineMemory)->bp-((ClipMachineMemory)->argc -((num)-1)))
#define RETPTR(ClipMachineMemory) ((ClipMachineMemory)->bp-(ClipMachineMemory)->argc - 1)

#define CLIP_UNDEF_VAR(varp)  memset((varp),0,sizeof(ClipVar))

#define _parc(num)		_clip_parc(_mp, (num))
#define _parinfo(num)	_clip_parinfo(_mp, (num))
#define _parl(num)		_clip_parl(_mp, (num))
#define _parnd(num)		_clip_parnd(_mp, (num))
#define _parni(num)		_clip_parni(_mp, (num))
#define _parnl(num)		_clip_parnl(_mp, (num))
#define _pcount()			(_mp->argc)

#define _retc(n)	_clip_retc(_mp, (n))
#define _retl(n)	_clip_retl(_mp, (n))
#define _retnd(n)	_clip_retnd(_mp, (n))
#define _retni(n)	_clip_retni(_mp, (n))
#define _retnl(n)	_clip_retnl(_mp, (n))

#define _storc(n,num)	_clip_storc(_mp, (n), (num), 0)
#define _storl(n,num)	_clip_storl(_mp, (n), (num), 0)
#define _stornd(n,num)	_clip_stornd(_mp, (n), (num), 0)
#define _storni(n,num)	_clip_storni(_mp, (n), (num), 0)
#define _stornl(n,num)	_clip_stornl(_mp, (n), (num), 0)

/* runtime support */

extern int _clip_argc;

extern char **_clip_argv;

extern char **_clip_envp;

extern int _clip_profiler;

extern char *_clip_progname;

extern int _clip_raw_argc;

extern char **_clip_raw_argv;

extern int _clip_sig_flag;

extern long _hash_cur_dir[26];

/* 128-bytes compare/transform tables */
extern unsigned char *_clip_cmptbl;

extern unsigned char *_clip_intbl;

extern unsigned char *_clip_isalpha_tbl;

extern unsigned char *_clip_lowtbl;

extern unsigned char *_clip_outtbl;

extern unsigned char *_clip_pgtbl;

extern unsigned char *_clip_uptbl;

struct ClipArrVar;

struct ClipAttr;

struct ClipBlock;

struct ClipBuf;

struct ClipCodeVar;

struct ClipDateTimeVar;

struct ClipDateVar;

struct ClipFieldDef;

struct ClipFile;

enum ClipFlags;

struct ClipFrame;

struct ClipHashBucket;

struct ClipHashNames;

struct ClipLogVar;

struct ClipLongVar;

struct ClipMachine;

struct ClipMapVar;

struct ClipNumVar;

struct ClipObjRtti;

struct ClipObjVar;

struct ClipRationalVar;

struct ClipRect;

struct ClipRefVar;

struct ClipStrVar;

struct ClipType;

union ClipVar;

union ClipVarCode;

struct ClipVarDef;

struct ClipVarEl;

struct ClipVarFrame;

union ClipVarLog;

union ClipVarNum;

union ClipVarStr;

enum ClipVarType;

struct ClipVarVect;

struct ClipVect;

struct ClipWindow;

struct Coll;

struct ContainerItem;

struct DBFuncTable;

struct rational;

struct _RDD_DATA_VTBL_;

struct _RDD_INDEX_VTBL_;

struct _RDD_MEMO_VTBL_;

struct Screen;

struct ScreenBase;

typedef int ClipFunction(struct ClipMachine *ClipMachineMemory);

struct ClipMachine *new_ClipMachine(struct Screen *scr);

/* valid type values */
#include "ci_clip/typedef_enum_ClipVarType.h"
#include "ci_clip/typedef_enum_ClipFlags.h"
#include "ci_clip/typedef_struct_ClipType.h"
#include "ci_clip/typedef_struct_ClipBuf.h"
#include "ci_clip/typedef_struct_ClipVect.h"
#include "ci_clip/typedef_struct_ClipFieldDef.h"
#include "ci_clip/typedef_struct_ClipRefVar.h"
#include "ci_clip/typedef_struct_ClipNumVar.h"
#include "ci_clip/typedef_struct_ClipLongVar.h"
#include "ci_clip/typedef_struct_ClipRationalVar.h"
#include "ci_clip/typedef_struct_ClipStrVar.h"
#include "ci_clip/typedef_struct_ClipLogVar.h"
#include "ci_clip/typedef_struct_ClipDateVar.h"
#include "ci_clip/typedef_struct_ClipDateTimeVar.h"
#include "ci_clip/typedef_struct_ClipArrVar.h"
#include "ci_clip/typedef_struct_ClipMapVar.h"
#include "ci_clip/typedef_struct_ClipObjRtti.h"
#include "ci_clip/typedef_struct_ClipObjVar.h"
#include "ci_clip/typedef_struct_ClipBlock.h"
#include "ci_clip/typedef_struct_ClipCodeVar.h"
#include "ci_clip/typedef_union_ClipVar.h"
#include "ci_clip/typedef_struct_ClipVarEl.h"
#include "ci_clip/typedef_union_ClipVarNum.h"
#include "ci_clip/typedef_union_ClipVarStr.h"
#include "ci_clip/typedef_union_ClipVarLog.h"
#include "ci_clip/typedef_union_ClipVarCode.h"
#include "ci_clip/typedef_struct_ClipVarDef.h"
#include "ci_clip/typedef_struct_ClipHashBucket.h"
#include "ci_clip/typedef_struct_ClipHashNames.h"
#include "ci_clip/typedef_struct_ClipVarFrame.h"
#include "ci_clip/typedef_struct_ClipVarVect.h"
#include "ci_clip/typedef_struct_ClipFrame.h"
#include "ci_clip/typedef_struct_ClipRect.h"
#include "ci_clip/typedef_struct_ClipAttr.h"
#include "ci_clip/typedef_struct_ClipWindow.h"
/* Container */
#include "ci_clip/typedef_struct_ContainerItem.h"
#include "ci_clip/typedef_struct_Container.h"
#include "ci_clip/typedef_struct_C_FILE.h"
/* Container end */
#include "ci_clip/typedef_struct_DBDriver.h"
#include "ci_clip/typedef_struct_SQLDriver.h"
#include "ci_clip/typedef_struct_ProfileBucket.h"
#include "ci_clip/typedef_struct_ProfileCount.h"
#include "ci_clip/typedef_struct_ClipMachine.h"
ClipMachine *cur_ClipMachine();

#include "ci_clip/typedef_struct_ClipFile.h"
#include "ci_clip/typedef_struct_ClipNameEntry.h"
#include "ci_clip/typedef_struct_ClipModule.h"
#include "ci_clip/typedef_struct_ClipInitStruct.h"
#include "ci_clip/typedef_struct_OutBuf.h"

struct _HashTable *_clip_space(ClipMachine * ClipMachineMemory, long space);

void _clear_trap(ClipMachine * ClipMachineMemory);

int _clip_aadd(ClipMachine * ClipMachineMemory, ClipVar * ap, ClipVar * vp);	/* array access */

void _clip_add_private(ClipMachine * ClipMachineMemory, long hash);

int _clip_adel(ClipMachine * ClipMachineMemory, ClipVar * ap, int ndim, long *dims);	/* array access */

int _clip_aget(ClipMachine * ClipMachineMemory, ClipVar * ap, ClipVar * resp, int ndim, long *dims);	/* array access */

int _clip_ains(ClipMachine * ClipMachineMemory, ClipVar * ap, int ndim, long *dims);	/* array access */

ClipVar *_clip_aref(ClipMachine * ClipMachineMemory, ClipVar * ap, int ndim, long *dims);	/* array access */

int _clip_array(ClipMachine * ClipMachineMemory, ClipVar * resp, int ndim, long *dims);	/* array access */

int _clip_aset(ClipMachine * ClipMachineMemory, ClipVar * ap, ClipVar * vp, int ndim, long *dims);	/* array access */

int _clip_asize(ClipMachine * ClipMachineMemory, ClipVar * ap, int ndim, long *dims);	/* array access */

int _clip_assign(ClipMachine * ClipMachineMemory, void *lval);

int _clip_assign_field(ClipMachine * ClipMachineMemory, long hash, long area);

int _clip_calc_hash(ClipMachine * ClipMachineMemory);	/* calc hash on stack top */

int _clip_calc_hash2(ClipMachine * ClipMachineMemory, int rev);	/* calc hash on stack top */

int _clip_call(struct ClipMachine *ClipMachineMemory, int argc, long hash);

long _clip_casehash(ClipMachine * ClipMachineMemory, ClipVar * vp);

long _clip_casehashbytes(long seed, const char *bytes, int len);

long _clip_casehashstr(const char *x);

long _clip_casehashword(const char *x, int l);	/* trim head and tail spaces */

void _clip_cdate(long ju, int *dd, int *mm, int *yy, int *ww);

int _clip_code_func(ClipMachine * ClipMachineMemory, ClipBlock * bp, int argc, int rest, ClipVarFrame * uplocals);

int _clip_code_proc(ClipMachine * ClipMachineMemory, ClipBlock * bp, int argc, int rest, ClipVarFrame * uplocals);

int _clip_clone(ClipMachine * ClipMachineMemory, ClipVar * dest, ClipVar * src);

void _clip_close_all(ClipMachine * ClipMachineMemory);

int _clip_compile_Block(ClipMachine * ClipMachineMemory, char *str, int len, ClipBlock * dest);

int _clip_cond(ClipMachine * ClipMachineMemory, int *ip);

void _clip_delete(ClipMachine * ClipMachineMemory, ClipVar * vp);

void _clip_destroy(ClipMachine * ClipMachineMemory, ClipVar * vp);

void _clip_dimarray(ClipMachine * ClipMachineMemory, int ndim);	/* with stack */

double _clip_double(ClipVar * vp);

int _clip_dup(ClipMachine * ClipMachineMemory, ClipVar * dest, ClipVar * src);

void _clip_dup_Block(ClipMachine * ClipMachineMemory, ClipBlock * src, ClipBlock * dest);

void _clip_exit(void);

int _clip_expand(ClipMachine * ClipMachineMemory, ClipBuf * dest, ClipBuf * src);

int _clip_expand_var(ClipMachine * ClipMachineMemory, ClipVar * vp);

int _clip_fetch(ClipMachine * ClipMachineMemory, int dim);	/* with stack */

ClipVar *_clip_fetchref(ClipMachine * ClipMachineMemory, int dim);	/* with stack */

int _clip_field(ClipMachine * ClipMachineMemory, long hash, long area);

int _clip_find_obj(ClipMachine * ClipMachineMemory, long otype, ClipObjRtti ** rtti);

int _clip_first_File(struct ClipFile *file, ClipCodeVar * dest, long *hash);

int _clip_fm_assign(ClipMachine * ClipMachineMemory, long hash);

int _clip_fm_iassign(ClipMachine * ClipMachineMemory, long hash);

int _clip_fmemvar(ClipMachine * ClipMachineMemory, long hash);

int _clip_func(struct ClipMachine *ClipMachineMemory, ClipFunction f, int argc, int rest, ClipVarFrame * uplocals);

int _clip_func_hash(struct ClipMachine *ClipMachineMemory, long hash, int argc, int rest, ClipVarFrame * uplocals);

int _clip_func_name(struct ClipMachine *ClipMachineMemory, int argc);

int _clip_get(struct ClipMachine *ClipMachineMemory, long hash);

long _clip_hash(ClipMachine * ClipMachineMemory, ClipVar * vp);

long _clip_hashbytes(long seed, const char *bytes, int len);

long _clip_hashbytes16(long seed, const char *bytes, int len);

long _clip_hashbytes32(long seed, const char *bytes, int len);

long _clip_hashstr(const char *x);

long _clip_hashword(const char *x, int l);	/* trim head and tail spaces */

int _clip_iassign(ClipMachine * ClipMachineMemory, void *lval);

int _clip_iassign_field(ClipMachine * ClipMachineMemory, long hash, long area);

int _clip_ifetch(ClipMachine * ClipMachineMemory, int dim);	/* with stack */

int _clip_incr(struct ClipMachine *ClipMachineMemory, void *lval, int isIncr, int isPost);

int _clip_init(ClipMachine * ClipMachineMemory, int argc, char **argv, char **envp);

void _clip_init_Block(ClipMachine * ClipMachineMemory, ClipBlock * dest, struct ClipFile *file, char *func);

int _clip_is_memvar(ClipMachine * ClipMachineMemory, long hash);

int _clip_is_private(ClipMachine * ClipMachineMemory, long hash);

int _clip_is_public(ClipMachine * ClipMachineMemory, long hash);

int _clip_isalpha(unsigned char ch);

int _clip_istore(ClipMachine * ClipMachineMemory, int dim, int byref);	/* with stack */

long _clip_jdate(int dd, int mm, int yy);

int _clip_load(ClipMachine * ClipMachineMemory, const char *name, struct Coll *names, ClipVar * res);

int _clip_log(ClipVar * vp);

void _clip_logg(int level, const char *fmt, ...);

int _clip_loglevel(int newlevel);

long _clip_long(ClipVar * vp);

int _clip_macro(ClipMachine * ClipMachineMemory);

int _clip_macroassign(ClipMachine * ClipMachineMemory, int op, int isTop, int fieldprec);

int _clip_main(ClipMachine * ClipMachineMemory, long hash, int argc, char **argv, char **envp);

int _clip_main_code(ClipMachine * ClipMachineMemory, ClipBlock * block, int argc, char **argv, char **envp);

int _clip_main_func(ClipMachine * ClipMachineMemory, ClipFunction func, int argc, char **argv, char **envp);

int _clip_map(ClipMachine * ClipMachineMemory, ClipVar * ap);	/* map access */

int _clip_madd(ClipMachine * ClipMachineMemory, ClipVar * ap, long no, ClipVar * vp);	/* map access */

int _clip_mclone(struct ClipMachine *ClipMachineMemory, ClipVar * dest, ClipVar * src);

int _clip_mdel(ClipMachine * ClipMachineMemory, ClipVar * ap, long no);	/* map access */

void *_clip_memdup(const void *src, int len);

int _clip_memvar(ClipMachine * ClipMachineMemory, long hash);

void _clip_memvar_param(ClipMachine * ClipMachineMemory, long hash, int no);

void _clip_memvar_private(ClipMachine * ClipMachineMemory, long hash);

void _clip_memvar_public(ClipMachine * ClipMachineMemory, long hash);

ClipVar *_clip_memvar_space(ClipMachine * ClipMachineMemory, struct _HashTable *space, long hash, int newvar);

int _clip_memvarf(ClipMachine * ClipMachineMemory, long hash);

ClipVar *_clip_mget(ClipMachine * ClipMachineMemory, ClipVar * ap, long no);

int _clip_mgetc(ClipMachine * ClipMachineMemory, ClipVar * ap, long no, char **strp, int *lenp);

int _clip_mgetl(ClipMachine * ClipMachineMemory, ClipVar * ap, long no, int *n);

int _clip_mgetn(ClipMachine * ClipMachineMemory, ClipVar * ap, long no, double *n);

int _clip_mind(ClipMachine * ClipMachineMemory, ClipVar * ap, long no, int *ind);	/* map access */

int _clip_minus(ClipMachine * ClipMachineMemory);

int _clip_mputc(ClipMachine * ClipMachineMemory, ClipVar * ap, long no, char *str, int len);

int _clip_mputl(ClipMachine * ClipMachineMemory, ClipVar * ap, long no, int n);

int _clip_mputn(ClipMachine * ClipMachineMemory, ClipVar * ap, long no, double n);

int _clip_mtype(ClipMachine * ClipMachineMemory, ClipVar * ap, long no);

int _clip_nameassign(ClipMachine * ClipMachineMemory, char *name, int len, ClipVar * vp, int fieldprec);

void _clip_newarray(ClipMachine * ClipMachineMemory, int n, ...);	/* with stack */

int _clip_next_File(struct ClipFile *file, ClipCodeVar * dest, long *hash);

int _clip_not(ClipMachine * ClipMachineMemory);

int _clip_op(ClipMachine * ClipMachineMemory, int op);

int _clip_opassign(ClipMachine * ClipMachineMemory, void *lval, int op);

int _clip_opiassign(ClipMachine * ClipMachineMemory, void *lval, int op);

int _clip_opistore(ClipMachine * ClipMachineMemory, int dim, int op);	/* with stack */

int _clip_opstore(ClipMachine * ClipMachineMemory, int dim, int op);	/* with stack */

void _clip_param(ClipMachine * ClipMachineMemory, int pno, int lno);

int _clip_parn(ClipMachine * ClipMachineMemory);

void _clip_pcount(ClipMachine * ClipMachineMemory);

void _clip_pop(ClipMachine * ClipMachineMemory);

int _clip_pop_area(ClipMachine * ClipMachineMemory);

void _clip_pop_fieldhash(ClipMachine * ClipMachineMemory, long *area, long *field);	/* pop stack, split to alias->field */

long _clip_pop_hash(ClipMachine * ClipMachineMemory);	/* pop stack */

ClipVar *_clip_pop_ref(ClipMachine * ClipMachineMemory);

long _clip_pop_shash(ClipMachine * ClipMachineMemory);	/* pop stack case-sensitive */

int _clip_proc(struct ClipMachine *ClipMachineMemory, ClipFunction f, int argc, int rest, ClipVarFrame * uplocals);

int _clip_proc_hash(struct ClipMachine *ClipMachineMemory, long hash, int argc, int rest, ClipVarFrame * uplocals);

void _clip_pshift(ClipMachine * ClipMachineMemory);

int _clip_public(ClipMachine * ClipMachineMemory, long hash);

int _clip_push(ClipMachine * ClipMachineMemory, /*ClipVar */ void *vp);

int _clip_push_area(ClipMachine * ClipMachineMemory, long newarea);

void _clip_push_false(ClipMachine * ClipMachineMemory);

void _clip_push_hash(ClipMachine * ClipMachineMemory, long hash);

int _clip_push_local(ClipMachine * ClipMachineMemory, int no);

void _clip_push_locale(ClipMachine * ClipMachineMemory);

void _clip_push_nil(ClipMachine * ClipMachineMemory);

int _clip_push_static(ClipMachine * ClipMachineMemory, /*ClipVar */ void *vp);

void _clip_push_true(ClipMachine * ClipMachineMemory);

int _clip_ref(ClipMachine * ClipMachineMemory, void *vp, int noadd);

int _clip_refassign(ClipMachine * ClipMachineMemory, void *lval, int isTop);

ClipVar *_clip_ref_destroy(ClipMachine * ClipMachineMemory, ClipVar * vp);

int _clip_ref_fmemvar(ClipMachine * ClipMachineMemory, long hash);

ClipVar *_clip_ref_local(ClipMachine * ClipMachineMemory, int no);

ClipVar *_clip_ref_memvar(ClipMachine * ClipMachineMemory, long hash);

ClipVar *_clip_ref_memvar_noadd(ClipMachine * ClipMachineMemory, long hash);

ClipVar *_clip_ref_param(ClipMachine * ClipMachineMemory, int no, ClipVar * params);

ClipVar *_clip_ref_public(ClipMachine * ClipMachineMemory, long hash);

ClipVar *_clip_ref_public_noadd(ClipMachine * ClipMachineMemory, long hash);

int _clip_refmacro(ClipMachine * ClipMachineMemory);	/* direct push refvar */

int _clip_register(ClipMachine * ClipMachineMemory, ClipFunction * func, const char *name);

int _clip_register_block(ClipMachine * ClipMachineMemory, ClipBlock block, long hash);

int _clip_register_driver(ClipMachine * ClipMachineMemory, struct DBFuncTable *vtbl);

int _clip_register_file(ClipMachine * ClipMachineMemory, ClipFile * file);

int _clip_register_hash(ClipMachine * ClipMachineMemory, ClipFunction * func, long hash);

int _clip_register_module(ClipMachine * ClipMachineMemory, ClipModule * modp);

void _clip_remove_privates(ClipMachine * ClipMachineMemory, int num, ... /*long hash */ );

void _clip_resume(ClipMachine * ClipMachineMemory, int nlocals, int nreflocals);

void _clip_return(ClipMachine * ClipMachineMemory);

void _clip_sarray(ClipMachine * ClipMachineMemory, int n);	/* with stack */

int _clip_set(struct ClipMachine *ClipMachineMemory, long hash);

int _clip_set_area(ClipMachine * ClipMachineMemory, long area);

void _clip_signal(int sig);

void _clip_signal_real(int sig);

int _clip_store(ClipMachine * ClipMachineMemory, int dim, int byref);	/* with stack */

int _clip_strFromVar(ClipMachine * ClipMachineMemory, ClipVar * vp, char **newstr, int *lenp);	/* return new str */

int _clip_take_field(ClipMachine * ClipMachineMemory, long hash, long area, ClipVar * dest);

int _clip_tcond(ClipMachine * ClipMachineMemory, int *ip);	/* don't pop stack */

void _clip_trap(ClipMachine * ClipMachineMemory, const char *filename, int line);

int _clip_trap_err(ClipMachine * ClipMachineMemory, int genCode, int canDefault, int canRetry, const char *subSystem, int subCode, const char *operation);

void _clip_trap_invargv(ClipMachine * ClipMachineMemory, const char *filename, int line);

int _clip_trap_operation(ClipMachine * ClipMachineMemory, const char *filename, int line, const char *oper, ClipVar * lval);

void _clip_trap_pop(ClipMachine * ClipMachineMemory);

void _clip_trap_printf(ClipMachine * ClipMachineMemory, const char *filename, int line, const char *fmt, ...);

void _clip_trap_printv(ClipMachine * ClipMachineMemory, const char *filename, int line, const char *fmt, void *vect);

void _clip_trap_str(ClipMachine * ClipMachineMemory, const char *filename, int line, const char *str);

void _clip_trap_var(ClipMachine * ClipMachineMemory, const char *filename, int line, ClipVar * var);

int _clip_try_field(ClipMachine * ClipMachineMemory, long hash);

const char *_clip_typename(ClipVar * vp);

int _clip_unref(ClipMachine * ClipMachineMemory, ClipVar * lval);

void _clip_unref_arr(ClipMachine * ClipMachineMemory);

void _clip_var_log(int val, ClipVar * vp);

void _clip_var_num(double num, ClipVar * vp);

void _clip_var_str(const char *str, int len, ClipVar * vp);

void _clip_vnewarray(ClipMachine * ClipMachineMemory, int n, long *vect);	/* with stack */

void _clip_vremove_privates(ClipMachine * ClipMachineMemory, int num, long *vect);

void _clip_vresume(ClipMachine * ClipMachineMemory, int num, ClipVar * vect);

void add_ClipVect(ClipVect * vect, void *item);

void clear_ClipVect(ClipVect * vect);

struct DBFuncTable *_clip_get_driver(ClipMachine * ClipMachineMemory, const char *driver);

void delete_ClipFile(ClipMachine * ClipMachineMemory, struct ClipFile *fp);

void delete_ClipMachine(struct ClipMachine *ClipMachineMemory);

void destroy_Block(ClipMachine * ClipMachineMemory, ClipBlock * bp);

void destroy_Buf(OutBuf * bp);

void destroy_ClipBuf(ClipBuf * buf);

int destroy_ClipFile(ClipMachine * ClipMachineMemory, struct ClipFile *fp);

void destroy_ClipVect(ClipVect * vect);

void init_Buf(OutBuf * bp);

void insert_ClipVect(ClipVect * vect, void *item, int pos);

void putByte_Buf(OutBuf * bp, unsigned char byte);

void putBuf_Buf(OutBuf * bp, char *buf, int len);

void putDouble_Buf(OutBuf * bp, double d);

void putInt32_Buf(OutBuf * bp, long l);

void putLong_Buf(OutBuf * bp, long l);

void putShort_Buf(OutBuf * bp, short s);

void putStr_Buf(OutBuf * bp, char *str);

void remove_ClipVect(ClipVect * vect, int no);

void setLen_ClipBuf(ClipBuf * buf, int newlen);

int _clip_invoke(ClipMachine * ClipMachineMemory, const char *name, void *result, int result_type, const char *fmt, ...);	/* extension system */

/*
   invoke call of Clip function
   fmt set num and type of args - one char for one arg:
   fmt:    type:                C-type:
   U    UNDEF           int
   C    CHARACTER       char*
   N    NUMERIC         double
   D    DATE            long
   L    LOGICAL         int
*/

ClipVar *_clip_par(ClipMachine * ClipMachineMemory, int num);	/* extension system */

int _clip_par_assign(ClipMachine * ClipMachineMemory, int num, ClipVar * vp);	/* extension system */

int _clip_par_assign_str(ClipMachine * ClipMachineMemory, int num, char *str, int str_len);	/* extension system */

int _clip_par_assign_str_m(ClipMachine * ClipMachineMemory, int num, char *str, int str_len);	/* malloc'ed str */

int _clip_par_isref(ClipMachine * ClipMachineMemory, int num);	/* return 1 if parameter sent by ref *//* extension system */

char *_clip_parc(ClipMachine * ClipMachineMemory, int num);	/* extension system */

char *_clip_parcl(ClipMachine * ClipMachineMemory, int num, int *len);	/* extension system */

int _clip_parinfo(ClipMachine * ClipMachineMemory, int num);	/* extension system */

int _clip_parl(ClipMachine * ClipMachineMemory, int num);	/* extension system */

double _clip_parnd(ClipMachine * ClipMachineMemory, int num);	/* extension system */

int _clip_parni(ClipMachine * ClipMachineMemory, int num);	/* extension system */

long _clip_pardc(ClipMachine * ClipMachineMemory, int num, int *yy, int *mm, int *dd, int *ww);	/* calendar *//* extension system */

long _clip_pardj(ClipMachine * ClipMachineMemory, int num);	/* julian *//* extension system */

long _clip_pardtj(ClipMachine * ClipMachineMemory, int num, long *time);	/* extension system */

long _clip_parnl(ClipMachine * ClipMachineMemory, int num);	/* extension system */

int _clip_parp(ClipMachine * ClipMachineMemory, int num, int *len, int *dec);	/* return 0 on success *//* extension system */

void _clip_retc(ClipMachine * ClipMachineMemory, char *str);	/* extension system */

void _clip_retcn(ClipMachine * ClipMachineMemory, char *str, int len);	/* extension system */

void _clip_retcn_m(ClipMachine * ClipMachineMemory, char *str, int l);	/* str is malloc'ed *//* extension system */

void _clip_retdc(ClipMachine * ClipMachineMemory, int yy, int mm, int dd);	/* extension system */

void _clip_retdj(ClipMachine * ClipMachineMemory, long julian);	/* extension system */

void _clip_retdtj(ClipMachine * ClipMachineMemory, long julian, long time);	/* extension system */

void _clip_retnd(ClipMachine * ClipMachineMemory, double n);	/* extension system */

void _clip_retndp(ClipMachine * ClipMachineMemory, double n, int len, int dec);	/* extension system */

void _clip_retni(ClipMachine * ClipMachineMemory, int n);	/* extension system */

void _clip_retnl(ClipMachine * ClipMachineMemory, long n);	/* extension system */

void _clip_retl(ClipMachine * ClipMachineMemory, int l);	/* extension system */

void _clip_retnr(ClipMachine * ClipMachineMemory, struct rational *r, int len, int dec);	/* extension system */

ClipVar *_clip_spar(ClipMachine * ClipMachineMemory, int num);	/* extension system */

int _clip_storc(ClipMachine * ClipMachineMemory, const char *str, int num, int ind);	/* extension system */

int _clip_storclen(ClipMachine * ClipMachineMemory, const char *str, int len, int num, int ind);	/* extension system */

int _clip_stordc(ClipMachine * ClipMachineMemory, int yy, int mm, int dd, int num, int ind);	/* extension system */

int _clip_stordj(ClipMachine * ClipMachineMemory, long julian, int num, int ind);	/* extension system */

int _clip_stornd(ClipMachine * ClipMachineMemory, double n, int num, int ind);	/* extension system */

int _clip_storni(ClipMachine * ClipMachineMemory, int n, int num, int ind);	/* extension system */

int _clip_stornl(ClipMachine * ClipMachineMemory, long n, int num, int ind);	/* extension system */

int _clip_storl(ClipMachine * ClipMachineMemory, int l, int num, int ind);	/* extension system */

ClipVar *_clip_unptr(ClipVar * vp);	/* extension system */

void _clip_upstr(char *s, int len);

int _clip_toupper(int ch);

int _clip_tolower(int ch);

int _clip_close_printer(ClipMachine * ClipMachineMemory);

int _clip_printf(ClipMachine * ClipMachineMemory, const char *fmt, ...);

int _clip_sprintf(ClipMachine * ClipMachineMemory, const char *fmt, ...);

int _clip_vsprintf(ClipMachine * ClipMachineMemory, const char *fmt, void *vect);

int _clip_init_screen(ClipMachine * ClipMachineMemory);

int _clip_resume_screen(ClipMachine * ClipMachineMemory);

int _clip_str2attr(char *str, int len);

int _clip_attr2str(int attr, char *buf, int buflen, int num_format);

int _clip_cmp(ClipMachine * ClipMachineMemory, ClipVar * lp, ClipVar * rp, int *res, int bytype);

int _clip_eval(ClipMachine * ClipMachineMemory, ClipVar * blockp, int argc, ClipVar * stackp, ClipVar * retp);

int _clip_eval_macro(ClipMachine * ClipMachineMemory, char *str, int len, ClipVar * dest);

int _clip_key(ClipMachine * ClipMachineMemory, int timeout_ms /* <0 ==> unlimited wait */ , int event_mask);

void _clip_hash_name(ClipMachine * ClipMachineMemory, long hash, char *buf, int buflen);

/* item MUST be malloc'ed */
void _clip_store_item(ClipMachine * ClipMachineMemory, long hash, void *item);

void _clip_store_item_destroy(ClipMachine * ClipMachineMemory, long hash, void *item, void (*destroy) (void *));

void _clip_free_item(ClipMachine * ClipMachineMemory, long hash);

void _clip_remove_item(ClipMachine * ClipMachineMemory, long hash);

void *_clip_fetch_item(ClipMachine * ClipMachineMemory, long hash);

/* container */
void *_clip_fetch_c_item(ClipMachine * ClipMachineMemory, int key, int type);

int _clip_destroy_c_item(ClipMachine * ClipMachineMemory, int key, int type);

int _clip_store_c_item(ClipMachine * ClipMachineMemory, void *item, int type, void (*destroy) (void *));

void *_clip_fetch_c_item_type(ClipMachine * ClipMachineMemory, int type, int *key);	/* first item of type */

long _clip_str_to_date(char *str, char *format, int epoch);

char *_clip_date_to_str(long date, char *format);

void _clip_datetostr(long date, char *buf);	/* as DTOS */

int _clip_ctot(ClipMachine * ClipMachineMemory, char *ctime, long *date, long *time, char *format);

char *_clip_ttoc(ClipMachine * ClipMachineMemory, long julian, long time, int *retlen, char *date_format, int hours, int seconds);

/* return size of matched portion; -1 if fail */
int _clip_glob_match(const char *string, const char *pattern, int caseflag);

int _clip_translate_path(ClipMachine * ClipMachineMemory, const char *string, char *buf, int buflen);

void _clip_unix_path(char *p, int tolow);

int _clip_absolute_path(ClipMachine * ClipMachineMemory, const char *path, char *p, int len);

int _clip_path(ClipMachine * ClipMachineMemory, const char *fn, char *p, int len, int create);

int _dos_path(ClipMachine * ClipMachineMemory, const char *fn, char *p, int len);

void _clip_push_func(struct ClipMachine *ClipMachineMemory, ClipFunction f, int nlocals, int nolocals);

void _clip_mptr(ClipMachine * ClipMachineMemory, ClipVar * refs, int num);

void _clip_set_deleted(ClipMachine * ClipMachineMemory, int value);

int _clip_type(ClipVar * vp);

ClipVar *_clip_vptr(ClipVar * vp);

void _clip_log_call(ClipMachine * ClipMachineMemory);

void _clip_log_init(char *filename);

void _clip_out_log(char *buf, int len);

int _clip_errorblock(ClipMachine * ClipMachineMemory, ClipVar * vp, int genCode);

int _clip_errorblock_res(ClipMachine * ClipMachineMemory, ClipVar * vp, int genCode, ClipVar * res);

int _clip_call_errblock(ClipMachine * ClipMachineMemory, int r);

/* create the Error object */
ClipVar *_clip_new_error(ClipMachine * ClipMachineMemory,
			 const char *description, int severity,
			 int genCode, int osCode, int canDefault, int canRetry, int canSubstitute, int tries, const char *subSystem, int subCode, const char *operation);

void _clip_flush_log();

char *_clip_unix_name(ClipMachine * ClipMachineMemory, char *dname);

int clip___QUIT(ClipMachine * ClipMachineMemory);

void _clip_generr(ClipMachine * ClipMachineMemory, int code);

const char *_clip_errname(int code);

void _clip_math_operation(ClipMachine * ClipMachineMemory, int op, ClipVar * Lvar, ClipVar * Rvar);

int _clip_forstep(ClipMachine * ClipMachineMemory, int *_if);

int _clip_map_first(ClipMachine * ClipMachineMemory, int keys, int *_if);

int _clip_map_next(ClipMachine * ClipMachineMemory, int keys, int *_if);

int _clip_get_function(ClipMachine * ClipMachineMemory, long hash, ClipFunction ** fpp, ClipBlock ** bpp);

int _clip_restore_tty(ClipMachine * ClipMachineMemory);

int _clip_init_tty(ClipMachine * ClipMachineMemory);

int _clip_load_inits(ClipMachine * ClipMachineMemory, ClipFile * file);

void _clip_swap(ClipMachine * ClipMachineMemory, int offset);	/* move sp-offset-1 to sp-1 location */

int _clip_putenv(char *name);

int _clip_put_env(char *name, char *val);

char *_clip_getenv(char *name);

extern char *CLIPROOT;

extern char *CLIP_LOCALE_ROOT;

void _clip_add_locale(char *locale);

void _clip_locale_msg(char *module, char *msg, char **dst);

void _clip_locale_msg_plural(char *module, char *msg, char *pl, long n, char **dst);

int _clip_module_locale(char *module, char *filename);

int _clip_rddfieldno(ClipMachine * ClipMachineMemory, int rddno, long namehash);

int _clip_rddfield(ClipMachine * ClipMachineMemory, int rddno, int fieldno);

int _clip_line(ClipMachine * ClipMachineMemory);

int _clip_debug(ClipMachine * ClipMachineMemory);

void _clip_sigdebug(int sig);

void _clip_sigint(int sig);

void _clip_sigint_real(int sig);

extern FILE *_clip_dbg_in;

extern FILE *_clip_dbg_out;

void _clip_print_dbg(ClipMachine * ClipMachineMemory, ClipVar * vp);

#include "ci_clip/typedef_struct_VarEntry.h"
/*
typedef struct VarEntry
{
  long hash;
  ClipVar var;
  struct VarEntry *next;
}
VarEntry;
*/
void _clip_hash_buckets(ClipMachine * ClipMachineMemory, ClipFile * file);

#include "ci_clip/typedef_struct_ClipBreakPoint.h"
/*
typedef struct ClipBreakPoint
{
  char *filename;
  int line;
  char *procname;
}
ClipBreakPoint;
*/
extern ClipBreakPoint *_clip_breakpoint;

extern int _clip_debuglevel;

extern int _clip_debugnext;

void _clip_in_breakpoint(void);

char *_clip_type_name(ClipVar * vp);

int _clip_uudecode(char *sstr, long l, char **strp, long *lenp);

int _clip_uuencode(char *sstr, long l, char **strp, long *lenp, int without_newline);

void _clip_var2str(ClipMachine * ClipMachineMemory, ClipVar * vp, char **strp, long *lenp, int method);

void _clip_str2var(ClipMachine * ClipMachineMemory, ClipVar * vp, char *str, long len, int method);

void _clip_dtos(double d, char *buf, int buflen, int *dp);

char *_clip_strFunc(ClipMachine * ClipMachineMemory, ClipVar * v, int len, int dec, int pad);

int _clip_dtostr(char *buf, int len, int dec, double d, int zero);

double _clip_strtod_base(char *str, int base);

double _clip_strtod(char *str, int *decpos);

int _clip_fileStrModeToNumMode(char *mode);

int _clip_clip(ClipMachine * ClipMachineMemory, const char *funcname, int argc, ClipVar * stackp, ClipVar * retp);

void _clip_destroy_locals(ClipMachine * ClipMachineMemory);

int _clip_parse_name(ClipMachine * ClipMachineMemory, char *name, int l, char **anamep, int *alp, char **fnamep, int *flp, long **dimp, int *ndimp);

#define CLEAR_CLIPVAR(vp) memset((vp),0,sizeof(ClipVar))

ClipFieldDef *_clip_get_fielddef(ClipMachine * ClipMachineMemory, long areahash, long namehash);

int _clip_catstr(ClipMachine * ClipMachineMemory, int num);

int _clip_quot(ClipMachine * ClipMachineMemory);

int _clip_strcmp(const char *str1, const char *str2);

int _clip_strncmp(const char *str1, const char *str2, int len);

int _clip_strcasecmp(const char *str1, const char *str2);

int _clip_strncasecmp(const char *str1, const char *str2, int len);

int _clip_strnncmp(const char *str1, const char *str2, int len1, int len2);

int _clip_strnncasecmp(const char *str1, const char *str2, int len1, int len2);

const char *_clip_strstr(const char *src, int slen, const char *dst, int dlen);

extern char *_clip_hostcs;

int _clip_translate_charset(char *p1, char *p2, unsigned char *str, unsigned char *ostr, int len);

#ifdef USE_TASKS
#define select(p1,p2,p3,p4,p5) clip_task_select_if(p1,p2,p3,p4,p5)
int clip_task_select_if(int, void *, void *, void *, void *);
#endif

void _clip_init_localvars(ClipMachine * ClipMachineMemory, ClipVarFrame ** plocalvars, int num);

int _clip_creat(ClipMachine * ClipMachineMemory, char *file, int flags, mode_t mode, int exclusive);

int _clip_open(ClipMachine * ClipMachineMemory, char *file, int flags, mode_t mode, int exclusive);

int _clip_close(ClipMachine * ClipMachineMemory, long hash, int fd);

#ifdef OS_CYGWIN
struct timeval;
#endif
int _clip_select(int n, fd_set * rfds, fd_set * wfds, fd_set * efds, struct timeval *tv);

int _clip_read(C_FILE * cf, void *buf, size_t count);

int _clip_write(C_FILE * cf, void *buf, size_t count);

void destroy_c_file(void *item);

char *_clip_host_charset();

#ifndef O_BINARY
#define O_BINARY 0
#endif

void _clip_check_stack(ClipMachine * ClipMachineMemory);

#ifdef WITH_CHECK_STACK

#define CLIP_CHECK_STACK _clip_check_stack(ClipMachineMemory)

#else

#define CLIP_CHECK_STACK

#endif

void _clip_fullscreen(ClipMachine * ClipMachineMemory);

char *_clip_gettext(const char *msg);

int _clip_start_profiler(ClipMachine * ClipMachineMemory);

int _clip_stop_profiler(ClipMachine * ClipMachineMemory);

/* for manual call */
int _clip_start_profiler_name(ClipMachine * ClipMachineMemory, const char *procname, const char *filename);

int _clip_stop_profiler_name(ClipMachine * ClipMachineMemory, const char *procname, const char *filename);

ClipVar *_clip_add_tempvar(ClipMachine * ClipMachineMemory, ClipVar * vp);

#define CLIP_LOCK_FLOCK     0x01
#define CLIP_LOCK_ONEBYTE   0x00
#define CLIP_LOCK_WRITE     0x02
#define CLIP_LOCK_READ      0x00
#define CLIP_LOCK_HILEVEL   0x04
#define CLIP_LOCK_LOWLEVEL  0x00
#define CLIP_LOCK_WAIT      0x08
#define CLIP_LOCK_NOWAIT    0x00

int _clip_setlock(ClipMachine * ClipMachineMemory, long hash, int fd, off_t pos, int flags);

int _clip_unlock(ClipMachine * ClipMachineMemory, long hash, int fd, off_t pos, int flags);

#endif
