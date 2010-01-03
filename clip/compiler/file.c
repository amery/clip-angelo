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
#include "ci_clic.h"
#include "ci_nodepriv.h"
#include "ci_clipcfg.h"
#include "ci_clip.h"

#if 0
typedef struct
{
   long      hash;
   long      offs;
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
#include "file/print_File.h"
#include "file/static_print_Function.h"
#include "file/static_print_VarColl.h"
#include "file/static_print_Constants.h"

/* ] print syntax tree  */
/* [ codegen  */
#include "file/static_codegen_Function.h"
#include "file/static_add_goto_trap.h"
#include "file/codegen_File.h"

/* ] codegen  */
/* [ write C file  */
#include "file/static_write_Function.h"
#include "file/static_write_statics.h"
#include "file/static_write_staticDefs.h"

static int wrote_main = 0;

#include "file/write_Main.h"
#include "file/write_File.h"
#include "file/compile_File.h"
#include "file/share_File.h"
#include "file/compile_CFile.h"

/* ] write C file  */
/* [ write PO file  */

#define MODBEG (bp->buf_of_StrBuf+8+2*sizeof(long))
#define BEGOFFS (bp->ptr_of_StrBuf-MODBEG)
#define CUROFFS (bp->ptr_of_StrBuf-bp->buf_of_StrBuf)

#ifdef FORCEALIGN
#include "file/static_SETLONG.h"
#include "file/static_SETSHORT.h"

#else

#define SETLONG(bp, offs, val) (*(long*)((bp)->buf_of_StrBuf+(offs))=(val))
#define SETSHORT(bp, offs, val) (*(short*)((bp)->buf_of_StrBuf+(offs))=(val))

#endif

#define LONGINCR(offs) ((offs)+=sizeof(long));
#define SHORTINCR(offs) ((offs)+=sizeof(short));

#include "file/static_put_function.h"
#include "file/static_cmp_hash_bucket.h"
#include "file/write_OFile.h"

/* ] write PO file  */
#include "file/strsuff.h"
#include "file/test_File.h"
#include "file/static_add_name.h"
#include "file/static_read_file.h"
#include "file/static_read_names.h"

#include "file/typedef_struct_Label.h"
#include "file/static_cmp_Label.h"
#include "file/static_new_Label.h"
#include "file/static_print_tree.h"
#include "file/static_print_labels.h"
#include "file/write_Cfunc.h"
