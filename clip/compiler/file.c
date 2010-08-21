/*
   Copyright (C) 2001  ITK
   Author   : Paul Lasarev <paul@itk.ru>
   License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI
 */
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <ci_clic.h>
#include <ci_nodepriv.h>
#include <ci_clipcfg.h>
#include <ci_clip.h>

#if 0
typedef struct
{
   long hash;
   long offs;
}
ClipHashBucket;
#endif

extern int optLevel;

/* name underscore */
#ifdef NM_UNDERSCORE
#define US "_"
#else
#define US ""
#endif

/* dll import prefix */
#ifdef _WIN32
#define IMP "__imp_"
#else
#define IMP ""
#endif

static void print_Function(Function * fp);

static void print_VarColl(VarColl * cp, char *msg);

static void print_Constants(Coll * cp, char *msg);

/* [ print syntax tree  */
#include <file/print_File.c>
#include <file/static_print_Function.c>
#include <file/static_print_VarColl.c>
#include <file/static_print_Constants.c>

/* ] print syntax tree  */
/* [ codegen  */
#include <file/static_codegen_Function.c>
#include <file/static_add_goto_trap.c>
#include <file/codegen_File.c>

/* ] codegen  */
/* [ write C file  */
#include <file/static_write_Function.c>
#include <file/static_write_statics.c>
#include <file/static_write_staticDefs.c>

static int wrote_main = 0;

#include <file/write_Main.c>
#include <file/write_File.c>
#include <file/compile_File.c>
#include <file/share_File.c>
#include <file/compile_CFile.c>

/* ] write C file  */
/* [ write PO file  */

#define MODBEG (bp->buf_of_StrBuf+8+2*sizeof(long))
#define BEGOFFS (bp->ptr_of_StrBuf-MODBEG)
#define CUROFFS (bp->ptr_of_StrBuf-bp->buf_of_StrBuf)

#ifdef FORCEALIGN
#include <file/static_SETLONG.c>
#include <file/static_SETSHORT.c>

#else

#define SETLONG(bp, offs, val) (*(long*)((bp)->buf_of_StrBuf+(offs))=(val))
#define SETSHORT(bp, offs, val) (*(short*)((bp)->buf_of_StrBuf+(offs))=(val))

#endif

#define LONGINCR(offs) ((offs)+=sizeof(long));
#define SHORTINCR(offs) ((offs)+=sizeof(short));

#include <file/static_put_function.c>
#include <file/static_cmp_hash_bucket.c>
#include <file/write_OFile.c>

/* ] write PO file  */
#include <file/strsuff.c>
#include <file/test_File.c>
#include <file/static_add_name.c>
#include <file/static_read_file.c>
#include <file/static_read_names.c>

#include <file/typedef_struct_Label.h>
#include <file/static_cmp_Label.c>
#include <file/static_new_Label.c>
#include <file/static_print_tree.c>
#include <file/static_print_labels.c>
#include <file/write_Cfunc.c>
