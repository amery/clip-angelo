/*
	Copyright (C) 2001  ITK
	Author   : Rushad Nabiullin <rust@itk.ru>
	License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI
*/
#include "ci_clip.h"
#include "ci_hash.h"
#include "ci_ordinfo.ch"

#ifdef CLIPPER50_COMPATIBLE
#define CHECKWA(wa) if(!wa || !wa->used) return 0;
#else
#define CHECKWA(wa) if(!wa || !wa->used) return rdd_err(ClipMachineMemory,EG_NOTABLE,0,__FILE__,__LINE__,__PROC__,"Workarea not in use")
#endif

#define bad_arg		_clip_gettext("Bad argument (%d)")

#define CHECKARG1(n,t) \
	if(_clip_parinfo(ClipMachineMemory,n)!=t){ \
		char err[100]; \
		sprintf(err,bad_arg,n); \
		er = rdd_err(ClipMachineMemory,EG_ARG,0,__FILE__,__LINE__,__PROC__,err); \
		goto err; \
	}

#define CHECKARG2(n,t1,t2) \
	if(_clip_parinfo(ClipMachineMemory,n)!=t1 && _clip_parinfo(ClipMachineMemory,n)!=t2){ \
		char err[100]; \
		sprintf(err,bad_arg,n); \
		er = rdd_err(ClipMachineMemory,EG_ARG,0,__FILE__,__LINE__,__PROC__,err); \
		goto err; \
	}

#define CHECKARG3(n,t1,t2,t3) \
	if(_clip_parinfo(ClipMachineMemory,n)!=t1 && _clip_parinfo(ClipMachineMemory,n)!=t2 && _clip_parinfo(ClipMachineMemory,n)!=t3){ \
		char err[100]; \
		sprintf(err,bad_arg,n); \
		er = rdd_err(ClipMachineMemory,EG_ARG,0,__FILE__,__LINE__,__PROC__,err); \
		goto err; \
	}

#define CHECKOPT1(n,t) \
	if((_clip_parinfo(ClipMachineMemory,n)!=t)&&(_clip_parinfo(ClipMachineMemory,n)!=UNDEF_type_of_ClipVarType)){ \
		char err[100]; \
		sprintf(err,bad_arg,n); \
		er = rdd_err(ClipMachineMemory,EG_ARG,0,__FILE__,__LINE__,__PROC__,err); \
		goto err; \
	}

#define CHECKOPT2(n,t1,t2) \
	if((_clip_parinfo(ClipMachineMemory,n)!=t1)&&(_clip_parinfo(ClipMachineMemory,n)!=t2)&&(_clip_parinfo(ClipMachineMemory,n)!=UNDEF_type_of_ClipVarType)){ \
		char err[100]; \
		sprintf(err,bad_arg,n); \
		er = rdd_err(ClipMachineMemory,EG_ARG,0,__FILE__,__LINE__,__PROC__,err); \
		goto err; \
	}

#define CHECKOPT3(n,t1,t2,t3) \
	if((_clip_parinfo(ClipMachineMemory,n)!=t1)&&(_clip_parinfo(ClipMachineMemory,n)!=t2)&&(_clip_parinfo(ClipMachineMemory,n)!=t3)&&(_clip_parinfo(ClipMachineMemory,n)!=UNDEF_type_of_ClipVarType)){ \
		char err[100]; \
		sprintf(err,bad_arg,n); \
		er = rdd_err(ClipMachineMemory,EG_ARG,0,__FILE__,__LINE__,__PROC__,err); \
		goto err; \
	}

#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#endif
#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif

#define _rm_setbit(m,b,r) ((r)<=(b))?((m)[((r)-1)>>5] = (m)[((r)-1)>>5] | (1<<((r-1)%32))):0
#define _rm_clrbit(m,b,r) ((r)<=(b))?((m)[((r)-1)>>5] = (m)[((r)-1)>>5] & ~(1<<((r-1)%32))):0
#define _rm_getbit(m,b,r) (((r)<=(b))?(((m)[((r)-1)>>5] & (1<<((r-1)%32)))):0)
/*
#define _RM_BITCOUNT  \
	recs = 0;\
	for(i=0;i<bytes;i++){\
		tmp1 = bm[i];\
		if(!tmp1 )\
			continue;\
		recs += ((tmp1 & 0x00000001) !=0 )+\
			((tmp1 & 0x00000002) !=0 )+\
			((tmp1 & 0x00000004) !=0 )+\
			((tmp1 & 0x00000008) !=0 )+\
			((tmp1 & 0x00000010) !=0 )+\
			((tmp1 & 0x00000020) !=0 )+\
			((tmp1 & 0x00000040) !=0 )+\
			((tmp1 & 0x00000080) !=0 )+\
			((tmp1 & 0x00000100) !=0 )+\
			((tmp1 & 0x00000200) !=0 )+\
			((tmp1 & 0x00000400) !=0 )+\
			((tmp1 & 0x00000800) !=0 )+\
			((tmp1 & 0x00001000) !=0 )+\
			((tmp1 & 0x00002000) !=0 )+\
			((tmp1 & 0x00004000) !=0 )+\
			((tmp1 & 0x00008000) !=0 )+\
			((tmp1 & 0x00010000) !=0 )+\
			((tmp1 & 0x00020000) !=0 )+\
			((tmp1 & 0x00040000) !=0 )+\
			((tmp1 & 0x00080000) !=0 )+\
			((tmp1 & 0x00100000) !=0 )+\
			((tmp1 & 0x00200000) !=0 )+\
			((tmp1 & 0x00400000) !=0 )+\
			((tmp1 & 0x00800000) !=0 )+\
			((tmp1 & 0x01000000) !=0 )+\
			((tmp1 & 0x02000000) !=0 )+\
			((tmp1 & 0x04000000) !=0 )+\
			((tmp1 & 0x08000000) !=0 )+\
			((tmp1 & 0x10000000) !=0 )+\
			((tmp1 & 0x20000000) !=0 )+\
			((tmp1 & 0x40000000) !=0 )+\
			((tmp1 & 0x80000000) !=0 ) \
			;                          \
	}
*/

#define RM_END_EXPR	0
#define RM_WORD		1
#define RM_LP		2
#define RM_RP		3
#define RM_OR		4
#define RM_AND		5
#define RM_LAR		6
#define RM_LARE		7
#define RM_LES		8
#define RM_LESE		9
#define RM_NEQU		10
#define RM_EQU		11
#define RM_EEQU		12
#define RM_NOT		13
#define RM_AL		14

#define RM_ERROR	100

#define MAX_BTREE_DEEP 64

#define DBFLS_CLIPPER53	0
#define DBFLS_SIX3		1

/*#ifdef ARCH_i386*/
typedef unsigned long long u8;

/*#endif*/

#define rdd_read(ClipMachineMemory,file,pos,len,buf,__PROC__) \
	((file)->f64?_rdd_read64(ClipMachineMemory,(file),(pos),(len),(buf),__PROC__): \
	_rdd_read(ClipMachineMemory,(file),(pos),(len),(buf),__PROC__))
#define rdd_write(ClipMachineMemory,file,pos,len,buf,__PROC__) \
	((file)->f64?_rdd_write64(ClipMachineMemory,(file),(pos),(len),(buf),__PROC__): \
	_rdd_write(ClipMachineMemory,(file),(pos),(len),(buf),__PROC__))
#define rdd_trunc(ClipMachineMemory,file,len,__PROC__) \
	((file)->f64?_rdd_trunc64(ClipMachineMemory,(file),(len),__PROC__): \
	_rdd_trunc(ClipMachineMemory,(file),(len),__PROC__))

#include "ci_rdd/typedef_struct_DbfLocale.h"

#if defined(OS_MINGW)
typedef char *caddr_t;
#endif

#include "ci_rdd/typedef_struct__RDD_FILE_.h"
#include "ci_rdd/typedef_struct__RDD_STACK_.h"

struct _RDD_INDEX_;

struct _RDD_INDEX_VTBL_;
#include "ci_rdd/typedef_struct__RDD_ORDER_.h"

struct _RDD_DATA_;
#include "ci_rdd/typedef_struct__RDD_INDEX_.h"

struct _RDD_MEMO_VTBL_;
#include "ci_rdd/typedef_struct__RDD_MEMO_.h"
#include "ci_rdd/typedef_struct__RDD_FIELD_.h"
#include "ci_rdd/typedef_struct__RDD_ORDSTATE_.h"

struct _RDD_DATA_VTBL_;

struct _RDD_RELATION_;

struct _RDD_FILTER_;
#include "ci_rdd/typedef_struct__RDD_DATA_.h"
#include "ci_rdd/typedef_struct__RDD_FPS_.h"
#include "ci_rdd/typedef_struct__RDD_FILTER_.h"
#include "ci_rdd/typedef_struct__RDD_RELATION_.h"
#include "ci_rdd/typedef_struct__RDD_STRUCT_.h"

struct _RDD_DATA_VTBL_;

#include "ci_rdd/typedef_struct__RDD_DATA_VTBL_.h"
#include "ci_rdd/typedef_struct__RDD_INDEX_VTBL_.h"
#include "ci_rdd/typedef_struct__RDD_MEMO_VTBL_.h"
#include "ci_rdd/typedef_struct__RDD_PSEUDO_.h"
#include "ci_rdd/typedef_struct__DBWorkArea_.h"


void      rdd_registerdatadriver(ClipMachine * ClipMachineMemory, RDD_DATA_VTBL * vtbl);

void      rdd_registerindexdriver(ClipMachine * ClipMachineMemory, RDD_INDEX_VTBL * vtbl);

void      rdd_registermemodriver(ClipMachine * ClipMachineMemory, RDD_MEMO_VTBL * vtbl);

RDD_DATA_VTBL *rdd_datadriver(ClipMachine * ClipMachineMemory, const char *driver, const char *__PROC__);

RDD_INDEX_VTBL *rdd_indexdriver(ClipMachine * ClipMachineMemory, const char *driver, const char *__PROC__);

RDD_MEMO_VTBL *rdd_memodriver(ClipMachine * ClipMachineMemory, const char *driver, const char *__PROC__);

void      loc_read(DbfLocale * loc, unsigned char *buf, int len);

void      loc_write(DbfLocale * loc, unsigned char *buf, int len);

int       loc_cmp(DbfLocale * loc, unsigned char *s1, unsigned char *s2, int len);

inline unsigned int _rdd_uint(unsigned char s[]);

inline unsigned short int _rdd_ushort(unsigned char s[]);

inline void _rdd_put_uint(unsigned char s[], unsigned int v);

inline void _rdd_put_ushort(unsigned char s[], unsigned short int v);

inline unsigned int _rdd_backuint(unsigned char s[]);

inline unsigned short int _rdd_backushort(unsigned char s[]);

inline void _rdd_put_backuint(unsigned char s[], unsigned int v);

inline void _rdd_put_backushort(unsigned char s[], unsigned short int v);

int       rdd_open(ClipMachine * ClipMachineMemory, char *path, int readonly, int shared, int *fd, const char *__PROC__);

int       rdd_err(ClipMachine * ClipMachineMemory, int genCode, int osCode,
		  const char *subSystem, int line, const char *operation, const char *desc);

void      rdd_expandmacro(int area, int r, const char *key, char *expr);

int       rdd_calcfilter(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *fok, const char *__PROC__);

int       rdd_checkfilter(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *ok, const char *__PROC__);

int       rdd_childs(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);

int       rdd_child_duty(ClipMachine * ClipMachineMemory, RDD_DATA * child, const char *__PROC__);

int       rdd_checkifnew(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);

int       rdd_event(ClipMachine * ClipMachineMemory, int nEvent, int nArea,
		    int nFieldPos, ClipVar * xTrigVal, int *re, const char *__PROC__);

int       rdd_calc(ClipMachine * ClipMachineMemory, int area, ClipVar * block, ClipVar * var, int noerror);

int       rdd_takevalue(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int no, ClipVar * vv, const char *__PROC__);

int       _rdd_fieldno(RDD_DATA * rd, long hash);

int       _rdd_parsepath(ClipMachine * ClipMachineMemory, const char *toopen,
			 const char *suff, char **path, char **name, int oper, const char *__PROC__);

int       _rdd_read(ClipMachine * ClipMachineMemory, RDD_FILE * file, int pos, int len, void *buf, const char *__PROC__);

int       _rdd_write(ClipMachine * ClipMachineMemory, RDD_FILE * file, int pos, int len, void *buf, const char *__PROC__);

int       _rdd_trunc(ClipMachine * ClipMachineMemory, RDD_FILE * file, unsigned int len, const char *__PROC__);

int       _rdd_read64(ClipMachine * ClipMachineMemory, RDD_FILE * file, u8 pos, int len, void *buf, const char *__PROC__);

int       _rdd_write64(ClipMachine * ClipMachineMemory, RDD_FILE * file, u8 pos, int len, void *buf, const char *__PROC__);

int       _rdd_trunc64(ClipMachine * ClipMachineMemory, RDD_FILE * file, u8 len, const char *__PROC__);

int       rdd_create(ClipMachine * ClipMachineMemory, const char *driver,
		     const char *memo_driver, const char *name, ClipVar * stru, const char *__PROC__);
int       rdd_creatememo(ClipMachine * ClipMachineMemory, const char *driver, const char *name, const char *__PROC__);

int       rdd_pack(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);

int       rdd_zap(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);

int       rdd_usearea(ClipMachine * ClipMachineMemory, const char *driver,
		      const char *name, int shared, int readonly, int tempo, RDD_DATA ** rdp, const char *__PROC__);
int       rdd_setindex(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX ** rip,
		       const char *driver, const char *name, const char *tag, int structural, const char *__PROC__);
int       rdd_setmemo(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *driver,
		      const char *name, const char *__PROC__);
int       rdd_closearea(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);

int       rdd_createindex(ClipMachine * ClipMachineMemory, RDD_DATA * rd,
			  const char *driver, const char *name, const char *tag,
			  const char *expr, ClipVar * block, int unique, const char *__PROC__);
int       rdd_reindex(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);

int       rdd_setorder(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int order, const char *__PROC__);

int       rdd_gotop(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);

int       rdd_gobottom(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);

int       rdd_skip(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int recs, const char *__PROC__);

int       rdd_seek(ClipMachine * ClipMachineMemory, RDD_DATA * rd, ClipVar * v, int soft,
		   int last, int *found, const char *__PROC__);
int       rdd_goto(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int rec, const char *__PROC__);

int       rdd_gotokey(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro,
		      unsigned int keyno, int *ok, const char *__PROC__);

int       rdd_bof(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *bof, const char *__PROC__);

int       rdd_eof(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *eof, const char *__PROC__);

int       rdd_recno(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *recno, const char *__PROC__);

int       rdd_lastrec(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *lastrec, const char *__PROC__);

int       rdd_keyno(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *keyno, const char *__PROC__);

int       rdd_lastkey(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *lastkey, const char *__PROC__);

int       rdd_fieldname(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int fno, const char *__PROC__);

int       rdd_append(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *neterr, const char *__PROC__);

int       rdd_getvalue(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int no, const char *__PROC__);

int       rdd_getvaluebn(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *fname, const char *__PROC__);

int       rdd_setvalue(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int no, ClipVar * vp, const char *__PROC__);

int       rdd_setvaluebn(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *fname, ClipVar * vp, const char *__PROC__);

int       rdd_delete(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);

int       rdd_recall(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);

int       rdd_deleted(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *deleted, const char *__PROC__);

int       rdd_keyvalue(ClipMachine * ClipMachineMemory, RDD_DATA * rd, ClipVar * v, const char *__PROC__);

int       rdd_keyadd(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, ClipVar * v, const char *__PROC__);

int       rdd_keydel(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__);

int       rdd_orddestroy(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__);

int       rdd_setrelation(ClipMachine * ClipMachineMemory, RDD_DATA * rd,
			  RDD_DATA * child, ClipVar * block, const char *expr, int scoped, const char *__PROC__);

int       rdd_scopetop(ClipMachine * ClipMachineMemory, RDD_DATA * rd, ClipVar * v, const char *__PROC__);

int       rdd_scopebottom(ClipMachine * ClipMachineMemory, RDD_DATA * rd, ClipVar * v, const char *__PROC__);

int       rdd_createuserfilter(ClipMachine * ClipMachineMemory, RDD_DATA * rd,
			       RDD_FILTER ** fpp, unsigned int size, const char *__PROC__);
int       rdd_createfilter(ClipMachine * ClipMachineMemory, RDD_DATA * rd,
			   RDD_FILTER ** fpp, ClipVar * _block, const char *str,
			   ClipVar * pseudo, int lNoOptimize, const char *__PROC__);

int       rdd_clearindex(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);

int       rdd_destroyfilter(ClipMachine * ClipMachineMemory, RDD_FILTER * fp, const char *__PROC__);

int       rdd_clearfilter(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);

int       rdd_clearrelation(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);

int       rdd_rlock(ClipMachine * ClipMachineMemory, RDD_DATA * rd, unsigned int rec, int *r, const char *__PROC__);

int       rdd_flock(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);

int       rdd_ulock(ClipMachine * ClipMachineMemory, RDD_DATA * rd, unsigned int rec, int flock, const char *__PROC__);

int       rdd_commit(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);

int       rdd_dbread(ClipMachine * ClipMachineMemory, RDD_DATA * rd, ClipVar * vp, const char *__PROC__);

int       rdd_dbwrite(ClipMachine * ClipMachineMemory, RDD_DATA * rd, ClipVar * ap, const char *__PROC__);

int       rdd_locate(ClipMachine * ClipMachineMemory, RDD_DATA * rd, char *cfor,
		     ClipVar * fexpr, ClipVar * wexpr, ClipVar * vnext,
		     ClipVar * vrec, ClipVar * vrest, int *found, const char *__PROC__);
int       rdd_continue(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *found, const char *__PROC__);

int       rdd_wildseek(ClipMachine * ClipMachineMemory, RDD_DATA * rd,
		       const char *pattern, int regular, int cont, int *found, const char *__PROC__);
int       rdd_seekeval(ClipMachine * ClipMachineMemory, RDD_DATA * rd, ClipVar * block, int *found, const char *__PROC__);

int       rdd_ii_create(ClipMachine * ClipMachineMemory, const char *driver, const char *name, const char *__PROC__);

int       rdd_ii_createtag(ClipMachine * ClipMachineMemory, int h, const char *tag, const char *expr, const char *__PROC__);

int       rdd_ii_open(ClipMachine * ClipMachineMemory, const char *driver, const char *name, const char *__PROC__);

int       rdd_ii_opentag(ClipMachine * ClipMachineMemory, int h, const char *tag, const char *__PROC__);

int       rdd_ii_close(ClipMachine * ClipMachineMemory, int h, const char *__PROC__);

int       rdd_ii_closetag(ClipMachine * ClipMachineMemory, int h, const char *__PROC__);

int       rdd_ii_addkey(ClipMachine * ClipMachineMemory, int h, const char *id, ClipVar * key, const char *__PROC__);

int       rdd_ii_delkey(ClipMachine * ClipMachineMemory, int h, const char *__PROC__);

int       rdd_ii_gotop(ClipMachine * ClipMachineMemory, int h, const char *__PROC__);

int       rdd_ii_gobottom(ClipMachine * ClipMachineMemory, int h, const char *__PROC__);

int       rdd_ii_bof(ClipMachine * ClipMachineMemory, int h, const char *__PROC__);

int       rdd_ii_eof(ClipMachine * ClipMachineMemory, int h, const char *__PROC__);

int       rdd_ii_id(ClipMachine * ClipMachineMemory, int h, const char *__PROC__);

int       rdd_ii_key(ClipMachine * ClipMachineMemory, int h, const char *__PROC__);

int       rdd_ii_skip(ClipMachine * ClipMachineMemory, int h, int recs, const char *__PROC__);

int       rdd_rawread(ClipMachine * ClipMachineMemory, RDD_DATA * rd, void *buf, const char *__PROC__);

int       rdd_rawwrite(ClipMachine * ClipMachineMemory, RDD_DATA * rd, void *buf, int append, const char *__PROC__);

void      destroy_rdd_data(void *);

void      destroy_rdd_index(void *);

void      destroy_rdd_order(ClipMachine * ClipMachineMemory, RDD_ORDER * ro);

void      destroy_rdd_i_order(void *order);

void      destroy_rdd_memo(RDD_MEMO * rm);

void      destroy_ryo(void *);

int       rdd_initrushmore(ClipMachine * ClipMachineMemory, RDD_DATA * rd,
			   RDD_FILTER * fp, ClipVar * remap, int test, const char *__PROC__);
int       rm_yylex(RDD_DATA * rd, int nomove);

int       rm_bitcount(unsigned int *bm, unsigned int size);

int       rm_init(ClipMachine * ClipMachineMemory, RDD_DATA * rd, char *str, const char *__PROC__);

unsigned int *rm_expr(ClipMachine * ClipMachineMemory, RDD_DATA * rd,
		      RDD_FILTER * fp, int bytes, int *optimize, int npseudo,
		      RDD_PSEUDO * pseudo, int test, const char *__PROC__);
int       rm_checkjoin(ClipMachine * ClipMachineMemory, RDD_FILTER * f1, RDD_FILTER * f2, const char *__PROC__);

int       rm_union(ClipMachine * ClipMachineMemory, RDD_FILTER * f1, RDD_FILTER * f2, const char *__PROC__);

int       rm_intersect(ClipMachine * ClipMachineMemory, RDD_FILTER * f1, RDD_FILTER * f2, const char *__PROC__);

int       rm_difference(ClipMachine * ClipMachineMemory, RDD_FILTER * f1, RDD_FILTER * f2, const char *__PROC__);

int       rm_evalpartial(ClipMachine * ClipMachineMemory, RDD_FILTER * fp,
			 ClipVar * _block, unsigned int *ret, unsigned int *cnt, const char *__PROC__);
int       rm_inverse(ClipMachine * ClipMachineMemory, RDD_FILTER * fp, const char *__PROC__);

int       rm_copyfilter(ClipMachine * ClipMachineMemory, RDD_FILTER * fp, RDD_FILTER ** ret, const char *__PROC__);

int       rm_isfiltrec(ClipMachine * ClipMachineMemory, RDD_FILTER * fp, unsigned int rec, int *r, const char *__PROC__);

DbfLocale *dbf_get_locale(ClipMachine * ClipMachineMemory);

int       _rdd_calcfiltlist(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_FILTER * fp, const char *__PROC__);

int       rdd_tasklock(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int cmd, struct flock *fl);

int       rdd_flushbuffer(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__);
