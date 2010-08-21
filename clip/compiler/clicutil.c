/*
	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI
*/
#include <ci_clip.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ci_clipcfg.h>
#include <ci_clic.h>
#include <clic.tab.h>
#include <ci_nodepriv.h>

/* [ vars */

char *clic_outpath = 0;

static int compare_Function(void *k1, void *k2);

/* ] vars */
/* [ util */

int v_neednl = 0;

#include <clicutil/v_printf.c>

#include <clicutil/vr_printf.c>
#include <clicutil/printString.c>
#include <clicutil/assign_String.c>
#include <clicutil/assign_constString.c>
#include <clicutil/haveMacro.c>

/* ] ConstStruct */
/* [ Var */
#include <clicutil/new_Var.c>
#include <clicutil/newInit_Var.c>

#include <clicutil/static_macro_name.c>
#include <clicutil/mnew_Var.c>
#include <clicutil/mnewInit_Var.c>
#include <clicutil/mnewArr_Var.c>
#include <clicutil/newArr_Var.c>
#include <clicutil/delete_Var.c>
#include <clicutil/static_compare_Var.c>
#include <clicutil/new_VarColl.c>
#include <clicutil/new_VarColl1.c>
#include <clicutil/delete_VarColl.c>
#include <clicutil/search_VarColl.c>
#include <clicutil/rm_VarColl.c>
#include <clicutil/insert_VarColl.c>
#include <clicutil/add_VarColl.c>

/* ] Var */
/* [  File */

File *curFile = NULL;

Function *curFunction = NULL;

Function *mainFunction = NULL;

VarColl *curParams = NULL;

VarColl *curLocals = NULL;

VarColl *curStatics = NULL;

VarColl *fileStatics = NULL;

VarColl *curPublics = NULL;

Coll *modules = NULL;

Coll *fileModules = NULL;

Coll *fileIncludes = NULL;

#include <clicutil/new_File.c>
#include <clicutil/delete_File.c>
#include <clicutil/add_Function.c>

/* ] File */
/* [ Function */
#include <clicutil/new_Function.c>
#include <clicutil/fin_Function.c>
#include <clicutil/delete_Function.c>
#include <clicutil/static_compare_Function.c>

/* ] Function */
/* [ parser init/resume */

static Coll hashNames;

typedef void (*free_func) (void *);

#include <clicutil/static_cmp_long.c>
#include <clicutil/init_parser.c>
#include <clicutil/resume_parser.c>
#include <clicutil/add_HashName.c>
#include <clicutil/get_HashName.c>

/* ] parser init/resume */
/* [ generation utilites */

#if 0
static int
search_CatchName(Coll * names, char *name, int *index)
{
   int i;

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
#include <clicutil/undeclaredName.c>

#include <clicutil/installName.c>

/* add __SELF__ to parameters */
#include <clicutil/add_self.c>

/* ] generation utilites */
/* [ codegen */

/* ] codegen */
/* [ memdebug */

#ifdef MEMDBG

#undef malloc
#undef free
#undef calloc
#undef realloc

int c_sum = 0, s_sum = 0, m_sum = 0, m_num = 0, c_num = 0, s_num = 0;

#include <clicutil/m_calloc.c>
#include <clicutil/m_malloc.c>
#include <clicutil/m_free.c>
#include <clicutil/m_realloc.c>
#include <clicutil/m_strdup.c>

#endif
/* ] memdebug */
/* [ StrBuf */
#include <clicutil/new_StrBuf.c>
#include <clicutil/delete_StrBuf.c>
#include <clicutil/static_grow_buf.c>
#include <clicutil/putByte_StrBuf.c>
#include <clicutil/write_StrBuf.c>
#include <clicutil/putInt_StrBuf.c>
#include <clicutil/putShort_StrBuf.c>
#include <clicutil/putLong_StrBuf.c>
#include <clicutil/putDouble_StrBuf.c>
#include <clicutil/putStr_StrBuf.c>
#include <clicutil/putString_StrBuf.c>

/* ] StrBuf */
