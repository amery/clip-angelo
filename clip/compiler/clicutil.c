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

char *clic_outpath = 0;

static int compare_Function(void *k1, void *k2);

/* ] vars */
/* [ util */

int v_neednl = 0;

int
v_printf(int level, const char *fmt, ...)
{
   va_list ap;

   int ret = 0;

   if (level > verbose)
      return 0;

   for (; v_neednl; v_neednl--)
      fputc('\n', stdout);

   va_start(ap, fmt);
   ret = vfprintf(stdout, _clic_gettext(fmt), ap);
   va_end(ap);
   fflush(stdout);

   return ret;
}

int
vr_printf(int level, const char *fmt, ...)
{
   va_list ap;

   int ret = 0;

   if (level > verbose)
      return 0;

   for (; v_neednl; v_neednl--)
      fputc('\n', stdout);

   va_start(ap, fmt);
   ret = vfprintf(stdout, fmt, ap);
   va_end(ap);
   fflush(stdout);

   return ret;
}

void
printString(FILE * stream, const char *str)
{
   const unsigned char *s;

   fputc('\"', stream);
   for (s = (unsigned char *) str; *s; s++)
      switch (*s)
	 {
	 case '"':
	    fputc('\\', stream);
	    fputc('"', stream);
	    break;
	 case '\n':
	    fputc('\\', stream);
	    fputc('n', stream);
	    break;
	 case '\t':
	    fputc('\\', stream);
	    fputc('t', stream);
	    break;
	 case '\v':
	    fputc('\\', stream);
	    fputc('v', stream);
	    break;
	 case '\b':
	    fputc('\\', stream);
	    fputc('b', stream);
	    break;
	 case '\r':
	    fputc('\\', stream);
	    fputc('r', stream);
	    break;
	 case '\f':
	    fputc('\\', stream);
	    fputc('f', stream);
	    break;
	 case '\\':
	    fputc('\\', stream);
	    fputc('\\', stream);
	    break;

	 default:
	    if (*s >= 32)
	       fputc(*s, stream);
	    else
	       fprintf(stream, "\\%03o", *s);
	    break;
	 }
   fputc('\"', stream);
}

void
assign_String(char **oldstr, char *newstr)
{
   if (*oldstr)
      free(*oldstr);
   *oldstr = newstr;
}

void
assign_constString(char **oldstr, const char *newstr)
{
   if (*oldstr)
      free(*oldstr);
   *oldstr = strdup(newstr);
}

int
haveMacro(char *str)
{
   char *s;

   for (s = str; *s; ++s)
      if (*s == '&')
	 return 1;
   return 0;
}

/* ] ConstStruct */
/* [ Var */

Var *
new_Var(char *name)
{
   char *s;

   NEWVAR(Var, ret);
   for (s = name; (*s); s++)
      *s = toupper(*s);
   ret->name = name;
   ret->no = -1;
   ret->line = cl_line;
   ret->pos = clic_pos;
   ret->file = currentFile();
   ret->func = curFunction;
   return ret;
}

Var *
newInit_Var(char *name, Node * expr)
{
   Var *ret = new_Var(name);

   ret->init = expr;
   return ret;
}

static char *
macro_name(void)
{
   static int no = 0;

   char name[32];

   snprintf(name, sizeof(name), "macro_%d", no++);
   return strdup(name);
}

Var *
mnew_Var(Node * macro)
{
   Var *ret = new_Var(macro_name());

   ret->macro = macro;
   return ret;
}

Var *
mnewInit_Var(Node * macro, struct Node * expr)
{
   Var *ret = newInit_Var(macro_name(), expr);

   ret->macro = macro;
   return ret;
}

Var *
mnewArr_Var(Node * macro, Coll * arr)
{
   Var *ret = newArr_Var(macro_name(), arr);

   ret->macro = macro;
   return ret;
}

Var *
newArr_Var(char *name, Coll * arr)
{
   Var *ret = new_Var(name);

   ret->arr = arr;
   return ret;
}

void
delete_Var(void *item)
{
   VAR(Var, vp, item);
   if (!item)
      return;
   if (vp->arr)
      delete_Coll(vp->arr);
   if (vp->alias)
      free(vp->alias);
   free(vp->name);
   free(item);
}

static int
compare_Var(void *k1, void *k2)
{
   VAR(Var, v1, k1);
   VAR(Var, v2, k2);
   return strcasecmp(v1->name, v2->name);
}

VarColl *
new_VarColl()
{
   NEWVAR(VarColl, ret);
   init_Coll(&ret->coll, delete_Var, compare_Var);
   init_Coll(&ret->unsorted, 0, 0);
   return ret;
}

VarColl *
new_VarColl1(Var * vp)
{
   NEWVAR(VarColl, ret);
   init_Coll(&ret->coll, delete_Var, compare_Var);
   init_Coll(&ret->unsorted, 0, 0);
   add_VarColl(ret, vp);
   return ret;
}

void
delete_VarColl(VarColl * coll)
{
   if (!coll)
      return;
   destroy_Coll(&coll->coll);
   destroy_Coll(&coll->unsorted);
   free(coll);
}

int
search_VarColl(VarColl * coll, char *name, int *index)
{
   Var var;

   var.name = name;
   return search_Coll(&coll->coll, &var, index);
}

void
rm_VarColl(VarColl * coll, Var * var)
{
   int no;

   if (search_Coll(&coll->coll, var, &no))
      {
	/*VAR(Var, vp, coll->coll.items[no]); */
	 atRemove_Coll(&coll->coll, no);
	/*remove_Coll(&coll->unsorted, vp); */
      }
}

void
insert_VarColl(VarColl * coll, Var * var)
{
   int no;

   if (!var)
      return;
   if (search_Coll(&coll->coll, var, &no))
      {
	 return;
      }

   var->no = coll->coll.count;
   insert_Coll(&coll->coll, var);
   insert_Coll(&coll->unsorted, var);
}

void
add_VarColl(VarColl * coll, Var * var)
{
   int no;

   if (!var)
      return;
   if (search_Coll(&coll->coll, var, &no))
      {
	 VAR(Var, vp, coll->coll.items[no]);
	 yywarning("duplicate name '%s', previous name defined near line %d pos %d in file '%s'", vp->name, vp->line, vp->pos, fileName(vp->file));
#if 0
	 remove_Coll(&coll->coll, vp);
	 remove_Coll(&coll->unsorted, vp);
	 delete_Var(vp);
#endif
      }

   var->no = coll->coll.count;
   insert_Coll(&coll->coll, var);
   insert_Coll(&coll->unsorted, var);
}

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

File *
new_File(char *name)
{
   char buf[256], *s, *e, *suf;

   NEWVAR(File, ret);
   e = strrchr(name, '/');
   if (e)
      ret->name_of_File = strdup(e + 1);
   else
      ret->name_of_File = strdup(name);
   ret->origname_of_File = strdup(name);
   if (name[0] == '/')
      strncpy(buf, name, sizeof(buf) - 3);
   else
      snprintf(buf, sizeof(buf) - 3, "%s/%s", outdir, name);
   s = strrchr(buf, '.');
   e = strrchr(buf, '/');
   if (pc_flag)
      {
	 if (asm_flag)
	    suf = ".s";
	 else
	    suf = ".c";
      }
   else if (pcode_flag)
      suf = ".po";
   else
      suf = ".c";

   if (s && (!e || e < s))
      strcpy(s, suf);
   else
      strcpy(buf + strlen(buf), suf);
   ret->cname_of_File = strdup(buf);
   s = strrchr(buf, '/');
   if (s)
      s++;
   else
      s = buf;
   ret->s_cname_of_File = strdup(s);

   if (pcode_flag && main_flag)
      {
	 free(name);
	 ret->name_of_File = strdup("MAIN.prg");
      }

   init_Coll(&ret->Coll_functions_of_File, NULL, compare_Function);
   init_Coll(&ret->Coll_unsortedFunctions_of_File, NULL, NULL);
   init_Coll(&ret->Coll_externFunctions_of_File, free, strcasecmp);
   init_Coll(&ret->Coll_externModules_of_File, free, strcasecmp);
   init_Coll(&ret->Coll_undeclExternFunctions_of_File, free, strcasecmp);

   init_Coll(&ret->Coll_numbers_of_File, NULL, compareConstNode);
   init_Coll(&ret->Coll_unsortedNumbers_of_File, NULL, NULL);
   init_Coll(&ret->Coll_strings_of_File, NULL, compareConstNode);
   init_Coll(&ret->Coll_unsortedStrings_of_File, NULL, NULL);

   init_Coll(&ret->Coll_codeblocks_of_File, NULL, NULL);
   init_Coll(&ret->Coll_names_of_File, free, strcmp);

   ret->Node_ctext_of_File = new_OperListNode();
   ret->Node_ctextStatic_of_File = new_OperListNode();

   return ret;
}

void
delete_File(File * m)
{
   if (!m)
      return;

   free(m->name_of_File);
   free(m->cname_of_File);

   destroy_Coll(&m->Coll_functions_of_File);
   destroy_Coll(&m->Coll_unsortedFunctions_of_File);
   destroy_Coll(&m->Coll_externFunctions_of_File);
   destroy_Coll(&m->Coll_externModules_of_File);
   destroy_Coll(&m->Coll_undeclExternFunctions_of_File);

   destroy_Coll(&m->Coll_numbers_of_File);
   destroy_Coll(&m->Coll_unsortedNumbers_of_File);
   destroy_Coll(&m->Coll_strings_of_File);
   destroy_Coll(&m->Coll_unsortedStrings_of_File);

   destroy_Coll(&m->Coll_codeblocks_of_File);
   destroy_Coll(&m->Coll_names_of_File);

   free(m);
}

void
add_Function(File * file, Function * func)
{
   int no;

   if (search_Coll(&file->Coll_functions_of_File, func, &no))
      {
	 VAR(Function, fp, file->Coll_functions_of_File.items[no]);
	 if (fp == mainFunction)
	    {
	       char buf[80];

	       yywarning("declaration function with module name '%s'; force -n flag", fp->name_of_Function);
	       remove_Coll(&file->Coll_functions_of_File, fp);
	       snprintf(buf, sizeof(buf), "%s_m", fp->name_of_Function);
	       free(fp->name_of_Function);
	       fp->name_of_Function = strdup(buf);
	       insert_Coll(&file->Coll_functions_of_File, fp);

	       no = insert_Coll(&file->Coll_unsortedFunctions_of_File, func);
	       func->no_of_Function = no;
	       insert_Coll(&file->Coll_functions_of_File, func);

	    }
	 else
	    yyerror("function '%s' already defined near line %d file %s", fp->name_of_Function, fp->line_of_Function, fileName(fp->file_of_Function));
      }
   else if (!func->isPublic_of_Function && search_Coll(&file->Coll_externFunctions_of_File, func->name_of_Function, &no))
      {
	 yyerror("static function '%s' already declared as external", func->name_of_Function);
      }
   else
      {
	 no = insert_Coll(&file->Coll_unsortedFunctions_of_File, func);
	 func->no_of_Function = no;
	 insert_Coll(&file->Coll_functions_of_File, func);
      }
}

/* ] File */
/* [ Function */

Function *
new_Function(char *name, VarColl * params, int isPublic, int isMain, Function * upfunc, int isCodeblock)
{
   NEWVAR(Function, ret);
   Function *sfp;

   char *s;

   int l;

   for (s = name; *s; ++s)
      *s = toupper(*s);
   ret->isPublic_of_Function = isPublic;
   l = strlen(name);

   switch (isPublic)
      {
      case 0:
	 ret->name_of_Function = (char *) malloc(l + 4);
	 memcpy(ret->name_of_Function, "___", 3);
	 memcpy(ret->name_of_Function + 3, name, l + 1);
	 break;
      case 1:
	 if (isMain && pcode_flag && main_flag)
	    ret->name_of_Function = strdup("MAIN");
	 else
	    ret->name_of_Function = name;
	 break;
      case 2:
	 ret->name_of_Function = (char *) malloc(l + 6);
	 memcpy(ret->name_of_Function, "INIT_", 5);
	 memcpy(ret->name_of_Function + 5, name, l + 1);
	 free(name);
	 break;
      case 3:
	 ret->name_of_Function = (char *) malloc(l + 6);
	 memcpy(ret->name_of_Function, "EXIT_", 5);
	 memcpy(ret->name_of_Function + 5, name, l + 1);
	 free(name);
	 break;
      }
   ret->isMain_of_Function = isMain;
   ret->VarColl_params_of_Function = params;
   ret->VarColl_locals_of_Function = new_VarColl();
   ret->VarColl_publics_of_Function = new_VarColl();
   ret->VarColl_privates_of_Function = new_VarColl();
   ret->VarColl_privates_of_Function = new_VarColl();
   ret->VarColl_parameters_of_Function = new_VarColl();
   ret->VarColl_fmemvars_of_Function = new_VarColl();
   ret->VarColl_memvars_of_Function = new_VarColl();
   ret->VarColl_statics_of_Function = new_VarColl();
   ret->VarColl_fields_of_Function = new_VarColl();
   ret->line_of_Function = cl_line;
   ret->file_of_Function = currentFile();
   ret->lastExec_of_Function = 0;
   ret->File_of_Function = curFile;
   ret->Node_staticInit_of_Function = new_Node();
   ret->Node_body_of_Function = new_OperListNode();
   ret->goto_trap_of_Function = 0;
   ret->labelNo_of_Function = 0;
   ret->Function_upfunc_of_Function = upfunc;
   ret->isCodeblock_of_Function = isCodeblock;

   sfp = curFunction;
   curFunction = ret;
   curFunction = sfp;

   return ret;
}

void
fin_Function(Function * fpp)
{
   int i;

   for (i = 0; i < fpp->VarColl_params_of_Function->unsorted.count; i++)
      {
	 Function *fp;

	 VAR(Var, vp, fpp->VarColl_params_of_Function->unsorted.items[i]);
	 int level;

	 if (vp->macro)
	    {
	       yyerror("macro variable name not allowed here");
	       continue;
	    }

	 for (level = 0, fp = fpp; fp->Function_upfunc_of_Function; fp = fp->Function_upfunc_of_Function, level++)
	    ;
	 if (!fpp->VarColl_locals_of_Function->unsorted.count && fp != fpp /*&& fp->isPublic_of_Function */ )
	    {
	       char buf[128], *nm;

	       Var *nv;

	       vp->pno = i;
	       sprintf(buf, "___param_%d", fp->VarColl_locals_of_Function->unsorted.count + 1);
	       nm = strdup(buf);
	       nv = new_Var(nm);
	       nv->func = fp;
	       nv->pno = vp->pno;
	       add_VarColl(fp->VarColl_locals_of_Function, nv);
	       vp->no = nv->no;
	       vp->level = level;
	       vp->isParam = 1;
	       nv->refvar = vp;
	       nv->isParam = 1;
	       nv->isRef = 1;
	       vp->isRef = 0;
	       fp->reflocals_of_Function++;
	    }
	 else
	    {
	       vp->isParam = 1;
	       vp->pno = i;
	       vp->func = fpp;
	       add_VarColl(fpp->VarColl_locals_of_Function, vp);
	    }

      }
}

void
delete_Function(void *v)
{
   VAR(Function, f, v);
   if (!f)
      return;
  /*delete_Node((Node *) f->body); */
   delete_VarColl(f->VarColl_params_of_Function);
   delete_VarColl(f->VarColl_locals_of_Function);
   delete_VarColl(f->VarColl_publics_of_Function);
   delete_VarColl(f->VarColl_privates_of_Function);
   delete_VarColl(f->VarColl_parameters_of_Function);
   delete_VarColl(f->VarColl_statics_of_Function);
   delete_VarColl(f->VarColl_memvars_of_Function);
   delete_VarColl(f->VarColl_fmemvars_of_Function);
   delete_VarColl(f->VarColl_fields_of_Function);

   free(f->name_of_Function);
   free(f->seqStack_of_Function);
   free(f);
}

static int
compare_Function(void *k1, void *k2)
{
   VAR(Function, v1, k1);
   VAR(Function, v2, k2);
   long h1 = hashstr(v1->name_of_Function + (v1->isPublic_of_Function == 0 ? 3 : 0));

   long h2 = hashstr(v2->name_of_Function + (v2->isPublic_of_Function == 0 ? 3 : 0));

  /*return strcasecmp(v1->name, v2->name); */
   if (h1 < h2)
      return -1;
   else if (h1 > h2)
      return 1;
/*
   else if (!v1->isPublic && v2->isPublic)
   return -1;
   else if (v1->isPublic && !v2->isPublic)
   return 1;
 */
   else
      return 0;
}

/* ] Function */
/* [ parser init/resume */

static Coll hashNames;

typedef void (*free_func) (void *);

static int
cmp_long(void *p1, void *p2)
{
   long l1 = *(long *) p1;

   long l2 = *(long *) p2;

   if (l1 < l2)
      return -1;
   else if (l1 > l2)
      return 1;
   else
      return 0;
}

void
init_parser()
{
   fileIncludes = new_Coll(free, NULL);

   if (!clic_outpath)
      clic_outpath = strdup(".");
   init_Coll(&hashNames, free, cmp_long);
  /* malloc_logpath="_memlog"; */
  /* debug2              log-stats, log-non-free, log-perror, log-trans,
     log-bad-pnt, \ check-fence, check-heap, check-lists, check-dblock-fence, \
     error-abort */
  /* malloc_debug( 0x401c1f ); */
  /* malloc_debug(0x405c17); */
}

void
resume_parser()
{
   delete_Coll(modules);
   delete_Coll(fileModules);
   delete_Coll(fileIncludes);
   free(clic_outpath);
   destroy_Coll(&hashNames);
}

void
add_HashName(long hash, const char *name)
{
   int no;

   char *entry;

   if (search_Coll(&hashNames, &hash, &no))
      return;
   no = strlen(name);
   entry = (char *) malloc(sizeof(long) + no + 1);

   *(long *) entry = hash;
   memcpy(entry + sizeof(long), name, no + 1);

   insert_Coll(&hashNames, entry);
}

const char *
get_HashName(long hash)
{
   int no;

   char *entry;

   if (!search_Coll(&hashNames, &hash, &no))
      return 0;
   entry = (char *) hashNames.items[no];
   return entry + sizeof(long);
}

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

int
undeclaredName(const char *sp, int memvar)
{
   int no;

   if (search_VarColl(curFunction->VarColl_locals_of_Function, (char *) sp, &no))
      {
	 VAR(Var, vp, curFunction->VarColl_locals_of_Function->coll.items[no]);
	 yyerror("name '%s' already declared as LOCAL near line %d pos %d file '%s'", vp->name, vp->line, vp->pos, fileName(vp->file));
	 return 0;
      }
   else if (search_VarColl(curFunction->VarColl_params_of_Function, (char *) sp, &no))
      {
	 VAR(Var, vp, curFunction->VarColl_params_of_Function->coll.items[no]);
	 yyerror("name '%s' already declared as PARAMETER near line %d pos %d file '%s'", vp->name, vp->line, vp->pos, fileName(vp->file));
	 return 0;
      }
   else if (!memvar && search_VarColl(curFunction->VarColl_memvars_of_Function, (char *) sp, &no))
      {
	 VAR(Var, vp, curFunction->VarColl_memvars_of_Function->coll.items[no]);
	 yywarning("name '%s' already declared as MEMVAR near line %d pos %d file '%s' (previous name will be hidden)", vp->name, vp->line, vp->pos, fileName(vp->file));
	 return 0;
      }
   else if (search_VarColl(curFunction->VarColl_fields_of_Function, (char *) sp, &no))
      {
	 VAR(Var, vp, curFunction->VarColl_fields_of_Function->coll.items[no]);
	 yyerror("name '%s' already declared as FIELD near line %d pos %d file '%s'", vp->name, vp->line, vp->pos, fileName(vp->file));
	 return 0;
      }
   else if (search_VarColl(curFunction->VarColl_statics_of_Function, (char *) sp, &no))
      {
	 VAR(Var, vp, curFunction->VarColl_statics_of_Function->coll.items[no]);
	 yyerror("name '%s' already declared as STATIC near line %d pos %d file '%s'", vp->name, vp->line, vp->pos, fileName(vp->file));
	 return 0;
      }
   return 1;
}

Node *
installName(char *name)
{
   int no, level = 0;

   Var *vp;

   Function *fp;

   if (curFunction->VarColl_code_params_of_Function && search_VarColl(curFunction->VarColl_code_params_of_Function, name, &no))
      {
	 VAR(Var, vp, curFunction->VarColl_code_params_of_Function->coll.items[no]);
	 Node *rp;

	 rp = new_LocalNode(vp);
	 ((VarNode *) rp)->vp->isCodeParam = 1;
	 ((VarNode *) rp)->vp->isParam = 1;
	 return rp;
      }
   if (search_VarColl(curFunction->VarColl_params_of_Function, name, &no))
      {
	 VAR(Var, vp, curFunction->VarColl_params_of_Function->coll.items[no]);
	 return new_LocalNode(vp);
      }

   fp = curFunction;
   if (!fp->uplocals_of_Function)
      {
	 for (level = 0; fp->Function_upfunc_of_Function; fp = fp->Function_upfunc_of_Function, level++)
	    ;
      }

   if (search_VarColl(fp->VarColl_locals_of_Function, name, &no))
      {
	 VAR(Var, vp, fp->VarColl_locals_of_Function->coll.items[no]);
	 if (level > 0)
	    {
	       vp->isRef = 1;
	       fp->reflocals_of_Function++;
	    }
	 vp->level = level;
	 return new_LocalNode(vp);
      }

   if (search_VarColl(curStatics, name, &no))
      {
	 VAR(Var, vp, curStatics->coll.items[no]);
	 return new_StaticNode(vp);
      }
   if (curStatics != fileStatics && search_VarColl(fileStatics, name, &no))
      {
	 VAR(Var, vp, fileStatics->coll.items[no]);
	 return new_StaticNode(vp);
      }
   if (search_VarColl(curFunction->VarColl_fmemvars_of_Function, name, &no))
      {
	 VAR(Var, vp, curFunction->VarColl_fmemvars_of_Function->coll.items[no]);
	 return new_FMemvarNode(vp);
      }
   if (fp != curFunction && search_VarColl(fp->VarColl_fmemvars_of_Function, name, &no))
      {
	 VAR(Var, vp, fp->VarColl_fmemvars_of_Function->coll.items[no]);
	 return new_FMemvarNode(vp);
      }
   if (search_VarColl(curFunction->VarColl_memvars_of_Function, name, &no))
      {
	 VAR(Var, vp, curFunction->VarColl_memvars_of_Function->coll.items[no]);
	 return new_MemvarNode(vp);
      }
   if (fp != curFunction && search_VarColl(fp->VarColl_memvars_of_Function, name, &no))
      {
	 VAR(Var, vp, fp->VarColl_memvars_of_Function->coll.items[no]);
	 return new_MemvarNode(vp);
      }
   if (curFunction != curFile->Function_main_of_File && search_VarColl(curFile->Function_main_of_File->VarColl_memvars_of_Function, name, &no))
      {
	 VAR(Var, vp, curFile->Function_main_of_File->VarColl_memvars_of_Function->coll.items[no]);
	 return new_MemvarNode(vp);
      }
   if (search_VarColl(curFunction->VarColl_fields_of_Function, name, &no))
      {
	 VAR(Var, vp, curFunction->VarColl_fields_of_Function->coll.items[no]);
	 return new_FieldNode(vp);
      }
   if (fp != curFunction && search_VarColl(fp->VarColl_fields_of_Function, name, &no))
      {
	 VAR(Var, vp, fp->VarColl_fields_of_Function->coll.items[no]);
	 return new_FieldNode(vp);
      }
   if (curFunction != curFile->Function_main_of_File && search_VarColl(curFile->Function_main_of_File->VarColl_fields_of_Function, name, &no))
      {
	 VAR(Var, vp, curFile->Function_main_of_File->VarColl_fields_of_Function->coll.items[no]);
	 return new_FieldNode(vp);
      }
   if (search_VarColl(curFunction->VarColl_parameters_of_Function, name, &no))
      {
	 VAR(Var, vp, curFunction->VarColl_parameters_of_Function->coll.items[no]);
	 return new_ParameterNode(vp);
      }
   if (fp != curFunction && search_VarColl(fp->VarColl_parameters_of_Function, name, &no))
      {
	 VAR(Var, vp, fp->VarColl_parameters_of_Function->coll.items[no]);
	 return new_ParameterNode(vp);
      }

   if (search_VarColl(curFunction->VarColl_privates_of_Function, name, &no))
      {
	 VAR(Var, vp, curFunction->VarColl_privates_of_Function->coll.items[no]);
	/*return new_PrivateNode(vp); */
	 if (auto_memvar)
	    return new_MemvarNode(vp);
	 else
	    return new_FMemvarNode(vp);
      }

   if (fp != curFunction && search_VarColl(fp->VarColl_privates_of_Function, name, &no))
      {
	 VAR(Var, vp, fp->VarColl_privates_of_Function->coll.items[no]);
	/*return new_PrivateNode(vp); */
	 if (auto_memvar)
	    return new_MemvarNode(vp);
	 else
	    return new_FMemvarNode(vp);
      }

   if (search_VarColl(curFunction->VarColl_publics_of_Function, name, &no))
      {
	 VAR(Var, vp, curFunction->VarColl_publics_of_Function->coll.items[no]);
	/*return new_PublicNode(vp); */
	 if (auto_memvar)
	    return new_MemvarNode(vp);
	 else
	    return new_FMemvarNode(vp);
      }
   if (fp != curFunction && search_VarColl(fp->VarColl_publics_of_Function, name, &no))
      {
	 VAR(Var, vp, fp->VarColl_publics_of_Function->coll.items[no]);
	/*return new_PublicNode(vp); */
	 if (auto_memvar)
	    return new_MemvarFNode(vp);
	 else
	    return new_FMemvarNode(vp);
      }
   if (namespace_warning)
      yywarning("namespace of name '%s' undeclared, assumed as FIELD-MEMVAR", name);
   vp = new_Var(strdup(name));

   add_VarColl(curFunction->VarColl_fmemvars_of_Function, vp);
   return new_FMemvarNode(vp);
  /*
     if (auto_memvar)
     return new_MemvarFNode(vp);
     else
     {
     add_VarColl(curFunction->VarColl_fmemvars, vp);
     return new_FMemvarNode(vp);
     }
   */
}

/* add __SELF__ to parameters */
void
add_self(void)
{
   VarColl *params;

   Var *vp;

   int i;

   if (curFunction->haveSelf_of_Function)
      return;

   curFunction->haveSelf_of_Function++;
   params = curFunction->VarColl_params_of_Function;
   vp = new_Var(strdup("__SELF__"));

   insert_Coll(&params->coll, vp);
   prepend_Coll(&params->unsorted, vp);
   add_VarColl(curFunction->VarColl_locals_of_Function, vp);

   for (i = 0; i < params->unsorted.count; ++i)
      {
	 vp = (Var *) params->unsorted.items[i];
	 vp->pno = i;
      }
}

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

void *
m_calloc(size_t nmemb, size_t size)
{
   int l = nmemb * size;

   void *ret = m_malloc(l);

   memset(ret, 0, l);
   c_sum += l;
   c_num++;
   return ret;
}

void *
m_malloc(size_t size)
{
   m_sum += size;
   m_num++;
   return malloc(size);
}

void
m_free(void *ptr)
{
   free(ptr);
}

void *
m_realloc(void *ptr, size_t size)
{
   return realloc(ptr, size);
}

char *
m_strdup(const char *s)
{
   int l = strlen(s) + 1;

   void *ret = m_malloc(l);

   memcpy(ret, s, l);
   s_sum += l;
   s_num++;
   return ret;
}

#endif
/* ] memdebug */
/* [ StrBuf */

StrBuf *
new_StrBuf()
{
   NEWVAR(StrBuf, bp);

   bp->buf_of_StrBuf = bp->ptr_of_StrBuf = (char *) malloc(64);
   bp->size_of_StrBuf = 64;
   bp->end_of_StrBuf = bp->buf_of_StrBuf + bp->size_of_StrBuf;

   return bp;
}

void
delete_StrBuf(StrBuf * bp)
{
   if (!bp)
      return;
   free(bp->buf_of_StrBuf);
   free(bp);
}

static void
grow_buf(StrBuf * bp, int min)
{
   int dsize = bp->size_of_StrBuf / 2;

   char *buf = bp->buf_of_StrBuf;

   if (min < 16)
      min = 16;
   if (dsize < min)
      dsize = min;
   bp->size_of_StrBuf = bp->size_of_StrBuf + dsize;

   bp->buf_of_StrBuf = realloc(bp->buf_of_StrBuf, bp->size_of_StrBuf);

   bp->end_of_StrBuf = bp->buf_of_StrBuf + bp->size_of_StrBuf;
   if (buf)
      bp->ptr_of_StrBuf = bp->buf_of_StrBuf + (bp->ptr_of_StrBuf - buf);
   else
      bp->ptr_of_StrBuf = bp->buf_of_StrBuf;
}

void
putByte_StrBuf(StrBuf * bp, unsigned char b)
{
   if (!bp->ptr_of_StrBuf || bp->ptr_of_StrBuf >= bp->end_of_StrBuf)
      grow_buf(bp, 1);
   *bp->ptr_of_StrBuf = b;
   ++bp->ptr_of_StrBuf;
}

void
write_StrBuf(StrBuf * bp, void *p, int len)
{
   if (!bp->buf_of_StrBuf || (bp->end_of_StrBuf - bp->ptr_of_StrBuf) <= len)
      grow_buf(bp, len + 1);
   memcpy(bp->ptr_of_StrBuf, p, len);
   bp->ptr_of_StrBuf += len;
}

void
putInt_StrBuf(StrBuf * bp, int i)
{
   long l = i;

   write_StrBuf(bp, &l, sizeof(l));
}

void
putShort_StrBuf(StrBuf * bp, short s)
{
   write_StrBuf(bp, &s, sizeof(s));
}

void
putLong_StrBuf(StrBuf * bp, long i)
{
   long l = i;

   write_StrBuf(bp, &l, sizeof(l));
}

void
putDouble_StrBuf(StrBuf * bp, double d)
{
  /*unsigned char buf[sizeof(double)]; */

   write_StrBuf(bp, &d, sizeof(d));
}

void
putStr_StrBuf(StrBuf * bp, const char *str)
{
   write_StrBuf(bp, (void *) str, strlen(str) + 1);
}

void
putString_StrBuf(StrBuf * bp, const char *str)
{
   write_StrBuf(bp, (void *) str, strlen(str));
}

/* ] StrBuf */
