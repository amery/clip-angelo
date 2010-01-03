/*
	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI
*/
#include "ci_clip.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "ci_clipcfg.h"
#include "ci_clic.h"
#include "clic.tab.h"
#include "ci_nodepriv.h"

/* [ vars */

char     *clic_outpath = 0;

static int compare_Function(void *k1, void *k2);

/* ] vars */
/* [ util */

int       v_neednl = 0;

#include "clicutil/v_printf.h"

#include "clicutil/vr_printf.h"
#include "clicutil/printString.h"
#include "clicutil/assign_String.h"
#include "clicutil/assign_constString.h"
#include "clicutil/haveMacro.h"

/* ] ConstStruct */
/* [ Var */
#include "clicutil/new_Var.h"
#include "clicutil/newInit_Var.h"

#include "clicutil/static_macro_name.h"
#include "clicutil/mnew_Var.h"
#include "clicutil/mnewInit_Var.h"
#include "clicutil/mnewArr_Var.h"
#include "clicutil/newArr_Var.h"
#include "clicutil/delete_Var.h"
#include "clicutil/static_compare_Var.h"
#include "clicutil/new_VarColl.h"
#include "clicutil/new_VarColl1.h"
#include "clicutil/delete_VarColl.h"
#include "clicutil/search_VarColl.h"
#include "clicutil/rm_VarColl.h"
#include "clicutil/insert_VarColl.h"
#include "clicutil/add_VarColl.h"

/* ] Var */
/* [  File */

File     *curFile = NULL;

Function *curFunction = NULL;

Function *mainFunction = NULL;

VarColl  *curParams = NULL;

VarColl  *curLocals = NULL;

VarColl  *curStatics = NULL;

VarColl  *fileStatics = NULL;

VarColl  *curPublics = NULL;

Coll     *modules = NULL;

Coll     *fileModules = NULL;

Coll     *fileIncludes = NULL;

#include "clicutil/new_File.h"
#include "clicutil/delete_File.h"
#include "clicutil/add_Function.h"

/* ] File */
/* [ Function */
#include "clicutil/new_Function.h"
#include "clicutil/fin_Function.h"
#include "clicutil/delete_Function.h"
#include "clicutil/static_compare_Function.h"

/* ] Function */
/* [ parser init/resume */

static Coll hashNames;

typedef void (*free_func) (void *);

#include "clicutil/static_cmp_long.h"
#include "clicutil/init_parser.h"
#include "clicutil/resume_parser.h"
#include "clicutil/add_HashName.h"
#include "clicutil/get_HashName.h"

/* ] parser init/resume */
/* [ generation utilites */

#if 0
static int
search_CatchName(Coll * names, char *name, int *index)
{
   int       i;

   for (i = names->count - 1; i >= 0; --i)
      if (!strcmp(name, (char *) names->items[i]))
       {
	  if (index)
	     *index = i;
	  return 1;
       }
   return 0;
}
#endif
#include "clicutil/undeclaredName.h"

#include "clicutil/installName.h"

/* add __SELF__ to parameters */
#include "clicutil/add_self.h"

/* ] generation utilites */
/* [ codegen */

/* ] codegen */
/* [ memdebug */

#ifdef MEMDBG

#undef malloc
#undef free
#undef calloc
#undef realloc

int       c_sum = 0, s_sum = 0, m_sum = 0, m_num = 0, c_num = 0, s_num = 0;

#include "clicutil/m_calloc.h"
#include "clicutil/m_malloc.h"
#include "clicutil/m_free.h"
#include "clicutil/m_realloc.h"
#include "clicutil/m_strdup.h"

#endif
/* ] memdebug */
/* [ StrBuf */
#include "clicutil/new_StrBuf.h"
#include "clicutil/delete_StrBuf.h"
#include "clicutil/static_grow_buf.h"
#include "clicutil/putByte_StrBuf.h"
#include "clicutil/write_StrBuf.h"
#include "clicutil/putInt_StrBuf.h"
#include "clicutil/putShort_StrBuf.h"
#include "clicutil/putLong_StrBuf.h"
#include "clicutil/putDouble_StrBuf.h"
#include "clicutil/putStr_StrBuf.h"
#include "clicutil/putString_StrBuf.h"

/* ] StrBuf */
