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

void
print_File(File * file)
{
   int i, count;

   printf("\nFile '%s' (%d functions):\n", file->name_of_File, file->Coll_functions_of_File.count);

   count = file->Coll_externFunctions_of_File.count;
   printf("external functions: %d:\n", count);
   for (i = 0; i < count; ++i)
      {
	 VAR(char, sp, file->Coll_externFunctions_of_File.items[i]);

	 printf("\t%s\n", sp);
      }

  /*
     count = file->undeclExternFunctions.count;
     printf( "implicit declared external functions: %d:\n", count);
     for (i = 0; i < count; ++i)
     {
     VAR(char, sp, file->undeclExternFunctions.items[i]);
     printf( "\t%s\n", sp);
     }
   */

   print_Constants(&file->Coll_unsortedNumbers_of_File, "numbers");
   print_Constants(&file->Coll_unsortedStrings_of_File, "strings");
   printf("codeblocks: %d\n", file->Coll_codeblocks_of_File.count);
   for (i = 0; i < file->Coll_codeblocks_of_File.count; ++i)
      {
	 VAR(Function, fp, file->Coll_codeblocks_of_File.items[i]);
	 print_Function(fp);
      }

/*print_Function( file->main_of_File); */
   count = file->Coll_unsortedFunctions_of_File.count;
   for (i = 0; i < count; ++i)
      {
	 VAR(Function, fp, file->Coll_unsortedFunctions_of_File.items[i]);
	 print_Function(fp);
      }

}

static void
print_Function(Function * fp)
{
   printf("\n");
   if (!fp->isPublic_of_Function)
      printf("STATIC ");
   printf("%s %s\n", fp->isProc_of_Function ? "PROCEDURE" : "FUNCTION", fp->isPublic_of_Function ? fp->name_of_Function : fp->name_of_Function + 3);
   printf("\tAS %s\n", fp->typename_of_Function ? fp->typename_of_Function : "undef");
   printf("nlocals = %d reflocals = %d uplocals=%d\n", fp->nlocals_of_Function, fp->reflocals_of_Function, fp->uplocals_of_Function);
   print_VarColl(fp->VarColl_params_of_Function, "params");
   print_VarColl(fp->VarColl_locals_of_Function, "locals");
   print_VarColl(fp->VarColl_statics_of_Function, "statics");
   print_VarColl(fp->VarColl_memvars_of_Function, "memvars");
   print_VarColl(fp->VarColl_fields_of_Function, "fields");
   print_VarColl(fp->VarColl_fmemvars_of_Function, "fmemvars");
   print_VarColl(fp->VarColl_parameters_of_Function, "paramteres");
   print_VarColl(fp->VarColl_privates_of_Function, "privates");
   print_VarColl(fp->VarColl_publics_of_Function, "publics");

   if (fp->Node_body_of_Function)
      {
	 printf("body:\n");
	 fp->Node_body_of_Function->pass(fp->Node_body_of_Function, Print, 0, NULL);
      }
}

static void
print_VarColl(VarColl * cp, char *msg)
{
   int i, count;

   count = cp->unsorted.count;
   printf("%s (%d):\n", msg, count);
   for (i = 0; i < count; ++i)
      {
	 VAR(Var, vp, cp->unsorted.items[i]);
	 print_Var(vp, 1);
      }
}

static void
print_Constants(Coll * cp, char *msg)
{
   int i, count;

   count = cp->count;
   printf("%s (%d):\n", msg, count);
   for (i = 0; i < count; ++i)
      {
	 VAR(ConstNode, s, cp->items[i]);
	 printf("\t'%s'\n", s->val);
      }
}

/* ] print syntax tree  */
/* [ codegen  */

static void
codegen_Function(Function * fp)
{
   int i, j;

   fp->seqNo_of_Function = 0;
   curFunction = fp;
   fp->nlocals_of_Function = 0;
   fp->reflocals_of_Function = 0;

   pass_Node(fp->Node_body_of_Function, Codegen2, 0, fp);
   pass_Node(fp->Node_body_of_Function, Codegen1, 0, fp);

#if 0
   for (j = fp->locals->unsorted.count - 1; j >= 0; --j)
      {
	 VAR(Var, vp, fp->locals->unsorted.items[j]);

	 Node *ip = 0;

	 if (vp->init && (vp->isRef || pcode_flag || !vp->init->isConst || vp->init->isArray))
	    {
	       ip = vp->init;
	    }
	 else if (vp->arr)
	    {
	       ip = new_NewArrayNode(vp->arr);
	    }

	 if (ip)
	    {
	       Node *lp = new_LocalNode(vp);

	       Node *np = new_OperExprNode(new_AssignNode(lp, ip, '='));

	       np->line = vp->line;
	       prepend_Node(fp->body, np);
	    }

      }
#endif

   for (j = fp->VarColl_locals_of_Function->unsorted.count - 1; j >= 0; --j)
     /*for (j = 0; j < fp->locals->unsorted.count; ++j) */
      {
	 VAR(Var, vp, fp->VarColl_locals_of_Function->unsorted.items[j]);
	 if (!vp->isRef)
	    {
	       vp->no = fp->nlocals_of_Function++;

	       if (vp->refvar)
		  vp->refvar->no = vp->no;
	    }
      }

   for (j = fp->VarColl_locals_of_Function->unsorted.count - 1; j >= 0; --j)
     /*for (j = 0; j < fp->VarColl_locals_of_Function->unsorted.count; ++j) */
      {
	 VAR(Var, vp, fp->VarColl_locals_of_Function->unsorted.items[j]);
	 if (vp->isRef)
	    {
	       if (vp->isParam)
		  continue;
	      /*if (fp->uplocals)
	         yyerror("reflocals and uplocals"); */
	       vp->no = fp->nlocals_of_Function + fp->reflocals_of_Function;
	       fp->reflocals_of_Function++;
	       if (vp->refvar)
		  vp->refvar->no = vp->no;
	    }

      }

   for (j = fp->VarColl_locals_of_Function->unsorted.count - 1; j >= 0; --j)
     /*for (j = 0; j < fp->VarColl_locals_of_Function->unsorted.count; ++j) */
      {
	 VAR(Var, vp, fp->VarColl_locals_of_Function->unsorted.items[j]);
	 if (vp->isRef)
	    {
	       if (!vp->isParam)
		  continue;
	       vp->no = fp->nlocals_of_Function + fp->reflocals_of_Function;
	       fp->reflocals_of_Function++;
	       if (vp->refvar)
		  vp->refvar->no = vp->no;
	    }

      }

#if 0
   for (j = fp->parameters->unsorted.count - 1; j >= 0; --j)
      {
	 VAR(Var, vp, fp->parameters->unsorted.items[j]);
	 if (vp->isParameter && vp->init)
	    {
	       prepend_Node(fp->body, vp->init);
	       continue;
	    }

      }
#endif

   for (j = fp->VarColl_statics_of_Function->unsorted.count - 1; j >= 0; --j)
      {
	 VAR(Var, vp, fp->VarColl_statics_of_Function->unsorted.items[j]);
	 if (vp->init /*&& (pcode_flag || vp->init->isArray) */ )
	    {
	       Node *np = new_OperExprNode(new_AssignNode(new_StaticNode(vp), vp->init, '='));

	       np->line = vp->init->line;
	       prepend_Node(fp->Node_staticInit_of_Function, np);
	    }
	 else if (vp->arr)
	    {
	       Node *np = new_OperExprNode(new_AssignNode(new_StaticNode(vp),
							  new_NewArrayNode(vp->arr),
							  '='));

	       np->line = vp->line;
	       prepend_Node(fp->Node_staticInit_of_Function, np);
	    }
      }

   for (i = 0; i < fp->VarColl_params_of_Function->unsorted.count; i++)
      {
	 VAR(Var, vp, fp->VarColl_params_of_Function->unsorted.items[i]);
	 prepend_Node(fp->Node_body_of_Function, new_ParamNode(vp));
      }

   if (fp->uplocals_of_Function)
      {
	 fp->nlocals_of_Function = 0;
	 fp->reflocals_of_Function = 0;
      }

   pass_Node(fp->Node_body_of_Function, CalcDeep, 0, fp);

}

static int
add_goto_trap(Node * np, void *par)
{
   Function *fp = (Function *) par;

   if (!np->isConst || np->isArray)
      fp->goto_trap_of_Function++;

   return 0;
}

void
codegen_File(File * file)
{
   int i, j;

   for (i = 0; i < file->Coll_unsortedFunctions_of_File.count; i++)
      {
	 VAR(Function, fp, file->Coll_unsortedFunctions_of_File.items[i]);
	 codegen_Function(fp);
      }

   file->Function_init_of_File = new_Function(strdup("_init"), new_VarColl(), 0, 0, 0, 0);
   file->Function_init_of_File->Node_body_of_Function = new_Node();
  /*join_Node(file->init->body, file->main->staticInit); */
   for (i = 0; i < file->Coll_unsortedFunctions_of_File.count; i++)
      {
	 VAR(Function, fp, file->Coll_unsortedFunctions_of_File.items[i]);
	 traverse_Node(fp->Node_staticInit_of_Function, add_goto_trap, file->Function_init_of_File);
	 join_Node(file->Function_init_of_File->Node_body_of_Function, fp->Node_staticInit_of_Function);
      }

   codegen_Function(file->Function_init_of_File);

   for (i = 0; i < file->Coll_codeblocks_of_File.count; i++)
      {
	 VAR(Function, fp, file->Coll_codeblocks_of_File.items[i]);
	 codegen_Function(fp);
      }

   i = 0;
   file->pubcount_of_File = 0;
   file->initcount_of_File = 0;
   file->exitcount_of_File = 0;
   file->allcount_of_File = file->Coll_functions_of_File.count + file->Coll_codeblocks_of_File.count;
   for (j = 0; j < file->Coll_unsortedFunctions_of_File.count; ++j)
      {
	 VAR(Function, fp, file->Coll_unsortedFunctions_of_File.items[j]);
	 switch (fp->isPublic_of_Function)
	    {
	    case 1:
	       fp->mno_of_Function = i++;
	       file->pubcount_of_File++;
	       break;
	    case 2:
	       fp->mno_of_Function = i++;
	       file->initcount_of_File++;
	       break;
	    case 3:
	       fp->mno_of_Function = i++;
	       ++file->exitcount_of_File;
	       break;
	    }
      }
   for (j = 0; j < file->Coll_unsortedFunctions_of_File.count; ++j)
      {
	 VAR(Function, fp, file->Coll_unsortedFunctions_of_File.items[j]);
	 if (!fp->isPublic_of_Function)
	    fp->mno_of_Function = i++;
      }
   for (j = 0; j < file->Coll_codeblocks_of_File.count; ++j)
      {
	 VAR(Function, fp, file->Coll_codeblocks_of_File.items[j]);
	 fp->mno_of_Function = i++;
      }

}

/* ] codegen  */
/* [ write C file  */

static void
write_Function(FILE * out, File * file, Function * fp)
{
   int j;

   int namelen;

   int local_locals = 0;

   clic_line = fp->line_of_Function;
   if (file->Function_init_of_File != fp)
      fprintf(out, "\n%sint\nclip_%s(ClipMachine *_mp)\n{\n", fp->isPublic_of_Function ? "" : "static ", fp != file->Function_init_of_File ? fp->name_of_Function : "_init");
   else
      fprintf(out, "\nstatic int\n_init(ClipMachine *_mp, ClipFrame *_frame_p)\n{\n");

   fprintf(out, "\tint _ret = 0;\n");
   if (fp->haveIf_of_Function)
      fprintf(out, "\tint _if;\n");
   fprintf(out, "\tClipVar _stack [ %d ];\n", fp->maxdeep_of_Function + 1);

#if 0
   if (fp->reflocals)
      {
	 fprintf(out, "\tClipVar _reflocals [ %d ];\n", fp->reflocals);
      }
#else
   for (namelen = 1, j = 0; j < fp->VarColl_locals_of_Function->unsorted.count; j++)
      {
	 VAR(Var, vp, fp->VarColl_locals_of_Function->unsorted.items[j]);
	 namelen += strlen(vp->name) + 1;
      }

   if (!fp->nlocals_of_Function && !fp->reflocals_of_Function)
      {
	 fprintf(out, "\tClipVarFrame *_localvars = _mp->fp->localvars;\n");
      }
   else if (fp->reflocals_of_Function)
      {
	 int i, l;

	 fprintf(out, "\tClipVarFrame *_localvars = (ClipVarFrame*) calloc(1, sizeof(ClipVarFrame)+%d*sizeof(ClipVar));\n", fp->VarColl_locals_of_Function->unsorted.count);
	 fprintf(out, "\tstatic const char _localnames[] = {");
	 for (j = 0; j < fp->VarColl_locals_of_Function->unsorted.count; j++)
	    {
	       VAR(Var, vp, fp->VarColl_locals_of_Function->unsorted.items[j]);
	       if (vp->isRef)
		  continue;
	       l = strlen(vp->name);
	       for (i = 0; i <= l; i++)
		  fprintf(out, "%d, ", vp->name[i]);
	       fprintf(out, " /* %s */ ", vp->name);
	    }
	 for (j = 0; j < fp->VarColl_locals_of_Function->unsorted.count; j++)
	    {
	       VAR(Var, vp, fp->VarColl_locals_of_Function->unsorted.items[j]);
	       if (!vp->isRef)
		  continue;
	       l = strlen(vp->name);
	       for (i = 0; i <= l; i++)
		  fprintf(out, "%d, ", vp->name[i]);
	       fprintf(out, " /* %s */ ", vp->name);
	    }
	 fprintf(out, "0 };\n");
      }
   else
      {
	 int numlocals = fp->VarColl_locals_of_Function->unsorted.count;

	 int i, l;

	 local_locals = 1;
	 fprintf(out, "\tClipVar _vlocals [ %d ] = { ", numlocals);
	 for (j = 0; j < numlocals - 1; j++)
	    fprintf(out, "{ {UNDEF_type_of_ClipVarType, 0,0,0,0} }, ");
	 if (numlocals)
	    fprintf(out, "{ {UNDEF_type_of_ClipVarType, 0,0,0,0} } ");
	 fprintf(out, "};\n");
	 fprintf(out, "\tconst char _localnames[] = { ");
	 for (j = 0; j < fp->VarColl_locals_of_Function->unsorted.count; j++)
	    {
	       VAR(Var, vp, fp->VarColl_locals_of_Function->unsorted.items[j]);
	       if (vp->isRef)
		  continue;
	       l = strlen(vp->name);
	       for (i = 0; i <= l; i++)
		  fprintf(out, "%d, ", vp->name[i]);
	       fprintf(out, " /* %s */ ", vp->name);
	    }
	 for (j = 0; j < fp->VarColl_locals_of_Function->unsorted.count; j++)
	    {
	       VAR(Var, vp, fp->VarColl_locals_of_Function->unsorted.items[j]);
	       if (!vp->isRef)
		  continue;
	       l = strlen(vp->name);
	       for (i = 0; i <= l; i++)
		  fprintf(out, "%d, ", vp->name[i]);
	       fprintf(out, " /* %s */ ", vp->name);
	    }
	 fprintf(out, "0 };\n");
	 fprintf(out, "\tClipVarFrame _local_vars = { 0, %d, _vlocals, _localnames }, *_localvars = &_local_vars;\n", fp->VarColl_locals_of_Function->unsorted.count);
      }
#endif

#if 0
   for (j = 0; j < fp->locals->unsorted.count; j++)
      {
	 VAR(Var, vp, fp->locals->unsorted.items[j]);
	 if (vp->isRef)
	    continue;
	 if (!vp->init)
	    fprintf(out, "\tClipVar %s = { {UNDEF_type_of_ClipVarType, 0, 0,0,0} };\n", vp->name);
	 else if (vp->init->isConst && !vp->init->isArray)
	    {
	       if (vp->init->isNum)
		  fprintf(out, "\tClipVarNum %s = ", vp->name);
	       else if (vp->init->isStr)
		  fprintf(out, "\tClipVarStr %s = ", vp->name);
	       else if (vp->init->isLog)
		  fprintf(out, "\tClipVarLog %s = ", vp->name);
	       else if (vp->init->isCode)
		  fprintf(out, "\tClipVarCode %s = ", vp->name);
	       else
		  fprintf(out, "\tClipVar %s = ", vp->name);
	       vp->init->pass(vp->init, CTextInit, 0, out);
	       fprintf(out, ";\n");
	    }
	 else
	    fprintf(out, "\tClipVar %s = { {UNDEF_type_of_ClipVarType, 0, 0,0,0} };\n", vp->name);
      }
#endif
  /*
     if (fp->params->coll.count)
     {
     int i, c=fp->params->coll.count;

     fprintf(out, "\tClipVar _params[%d] = { ", c);
     for(i=0; i<c; ++i)
     fprintf(out, "{ UNDEF_t, 0, 0,0,0 },");
     fprintf(out, " };\n");
     }
   */

#if 0
   {
      fprintf(out, "\tClipVarDef _locals[] =\n\t{\n");
      fprintf(out, "\t\t{ %d, 0 },\n", fp->locals->coll.count);
      for (j = 0; j < fp->locals->coll.count; j++)
	 {
	    VAR(Var, vp, fp->locals->coll.items[j]);
	    if (vp->isRef)
	       fprintf(out, "\t\t{ %ld, _reflocals+%d },\n", (long) hashstr(vp->name), -vp->no);
	    else
	       fprintf(out, "\t\t{ %ld, &%s },\n", (long) hashstr(vp->name), vp->name);
	 }
      fprintf(out, "\t};\n");
   }
#else
#endif

   fprintf(out,
	   "\tClipFrame _frame = { _stack, _stack, _file, %d, 0, %s, 0, _statics, 0, &_hash_names,\"%s\", %d, 0 };\n",
	   fp->line_of_Function, "_localvars", fp->isPublic_of_Function ? fp->name_of_Function : fp->name_of_Function + 3, fp->maxdeep_of_Function + 1);

   if (file->Function_init_of_File != fp && !fp->nlocals_of_Function && !fp->reflocals_of_Function)
      {
	 fprintf(out, "\tif (_localvars && _localvars->refcount)\n\t\t_localvars->refcount++;\n");
      }
   else if (fp->reflocals_of_Function)
      {
	 fprintf(out, "\t_localvars->vars = (ClipVar*) (_localvars + 1);\n");
	 fprintf(out, "\t_localvars->refcount = 1;\n");
	 fprintf(out, "\t_localvars->size = %d;\n", fp->nlocals_of_Function + fp->reflocals_of_Function);
	 fprintf(out, "\t_localvars->names = _localnames;\n");
      }

   if (file->Function_init_of_File != fp)
      {
	 fprintf(out, "\t_init( _mp, &_frame );\n");
	 fprintf(out, "\t_clip_log_call( _mp );\n\n");
      }
   else
      {
	 fprintf(out, "\t_frame_p->up = _mp->fp;\n");
	 fprintf(out, "\tif (_init_flag)\n\t{\n");
	 fprintf(out, "\t\t_mp->fp = _frame_p;\n");
	 fprintf(out, "\t\treturn 0;\n\t}\n");
	 fprintf(out, "\telse if (_localvars && _localvars->refcount)\n\t\t_localvars->refcount++;\n");
	 fprintf(out, "\t_frame.up = _mp->fp;\n");
	 fprintf(out, "\t_mp->fp = &_frame;\n");
	 fprintf(out, "\t_init_flag = 1;\n");

	/*
	   for(i=0;i<file->unsortedFunctions.count;i++)
	   {
	   VAR(Function, fp, file->unsortedFunctions.items[i]);
	   if (fp->isPublic)
	   fprintf(out, "\t_clip_register( %s, \"%s\" );\n", fp->name, fp->name);
	   }
	 */
	 fprintf(out, "\n");
      }

#if 0
   if (fp->reflocals)
      fprintf(out, "\t_clip_mptr(_mp, _reflocals, %d);\n", fp->reflocals);
#endif

   pass_Node(fp->Node_body_of_Function, CText, 0, out);

   if (file->Function_init_of_File == fp)
      file->Node_ctextStatic_of_File->pass(file->Node_ctextStatic_of_File, CText, 0, out);

   if ( /*file->init!=fp && */ fp->goto_trap_of_Function)
      {
	 fprintf(out, "\n\tgoto _return;\n");
	 fprintf(out, "_trap_0:\n");
	/*fprintf(out, "\t_ret = 1;\n"); */
	 fprintf(out, "\t_clip_trap(_mp, _file, _frame.line);\n");
      }

   fprintf(out, "_return:\n");
#if 1
   if (local_locals)
      fprintf(out, "\t_clip_destroy_locals( _mp );\n");
   fprintf(out, "\t_clip_resume( _mp, %d, %d );\n", fp->nlocals_of_Function, fp->reflocals_of_Function);
#else
   fprintf(out, "\t_clip_resume( _mp, %d ", fp->nlocals_of_Function);
   if (fp->nlocals_of_Function)
      {
	 for (j = fp->VarColl_locals_of_Function->unsorted.count - 1; j >= 0; --j)
	    {
	       VAR(Var, vp, fp->VarColl_locals_of_Function->unsorted.items[j]);
	       if (vp->isRef)
		  continue;
	       fprintf(out, ", &%s", vp->name);
	    }
      }
   fprintf(out, " );\n");
#if 0
   if (fp->refVarColl_locals_of_Function)
      {
	 fprintf(out, "\t_clip_vresume(_mp, %d, _reflocals);\n", fp->refVarColl_locals_of_Function);
      }
#endif
#endif
  /*
     if (fp->VarColl_params_of_Function->coll.count)
     {
     int c=fp->VarColl_params_of_Function->coll.count;
     fprintf(out, "\t_clip_vresume(_mp, %d, _params);\n", c);
     }
   */

#if 0
   if ((fp->VarColl_privates_of_Function->unsorted.count + fp->VarColl_parameters_of_Function->unsorted.count) > 0)
      {
	 fprintf(out, "\t_clip_remove_privates( _mp, %d", fp->VarColl_privates_of_Function->unsorted.count + fp->VarColl_parameters_of_Function->unsorted.count);
	 for (j = fp->VarColl_privates_of_Function->unsorted.count - 1; j >= 0; --j)
	    {
	       VAR(Var, vp, fp->VarColl_privates_of_Function->unsorted.items[j]);
	       fprintf(out, ", %ld", (long) hashstr(vp->name));
	    }
	 for (j = fp->VarColl_parameters_of_Function->unsorted.count - 1; j >= 0; --j)
	    {
	       VAR(Var, vp, fp->VarColl_parameters_of_Function->unsorted.items[j]);
	       fprintf(out, ", %ld", (long) hashstr(vp->name));
	    }
	 fprintf(out, ");\n");
      }
#endif

   if (file->Function_init_of_File == fp)
      {
	/*fprintf(out, "\t_frame_p->up = _mp->fp;\n"); */
	 fprintf(out, "\t_mp->fp = _frame_p;\n");
      }
   fprintf(out, "\treturn _ret;\n}\n");
}

static void
write_statics(FILE * out, VarColl * statics)
{
   int j;

   for (j = 0; j < statics->unsorted.count; j++)
      {
	 VAR(Var, vp, statics->unsorted.items[j]);
#if 1
	 fprintf(out, "static ClipVar s_%s_%s = { {UNDEF_type_of_ClipVarType, 0, 0,0,0} }", vp->func->name_of_Function, vp->name);
#else
	 if (!vp->init)
	    fprintf(out, "static ClipVar s_%s_%s = { {UNDEF_type_of_ClipVarType, 0, 0,0,0} }", vp->func->name_of_Function, vp->name);
	 else if (vp->init->isArray)
	    {
	       fprintf(out, "static ClipVar s_%s_%s", vp->func->name, vp->name);
	    }
	 else
	    {
	       if (vp->init->isNum)
		  fprintf(out, "static ClipVarNum s_%s_%s = ", vp->func->name, vp->name);
	       else if (vp->init->isStr)
		  fprintf(out, "static ClipVarStr s_%s_%s = ", vp->func->name, vp->name);
	       else if (vp->init->isLog)
		  fprintf(out, "static ClipVarLog s_%s_%s = ", vp->func->name, vp->name);
	       else if (vp->init->isCode)
		  fprintf(out, "static ClipVarCode s_%s_%s = ", vp->func->name, vp->name);
	       else
		  fprintf(out, "static ClipVar s_%s_%s = ", vp->func->name, vp->name);
	       vp->init->pass(vp->init, CTextInit, 0, out);
	    }
#endif
	 fprintf(out, ";\n");
      }

}

static void
write_staticDefs(FILE * out, VarColl * statics)
{
   int j;

   for (j = 0; j < statics->coll.count; j++)
      {
	 VAR(Var, vp, statics->coll.items[j]);
	 fprintf(out, "\t{ %ld, &s_%s_%s },\n", (long) hashstr(vp->name), vp->func->name_of_Function, vp->name);
      }
}

static int wrote_main = 0;

void
write_Main(File * file, FILE * out, const char *start)
{
   if (wrote_main)
      return;
   fprintf(out, "\n\nextern char **environ;\n");
   fprintf(out, "void _clip_init_dll(void);\n");
   fprintf(out, "\nint\nmain(int argc, char **argv)\n{\n");
   fprintf(out, "\tClipMachine *mp;\n\n");
   fprintf(out, "\t_clip_init_dll();\n");
   fprintf(out, "\tTask_INIT();\n");
   fprintf(out, "\tmp = new_ClipMachine(0);\n");
   fprintf(out, "\treturn _clip_main_func( mp, clip_%s, argc, argv, environ);\n", start);
   fprintf(out, "}\n");
   wrote_main = 1;
}

void
write_File(File * file)
{
   int i, j, sum;

   FILE *out;

   time_t tbuf;

   out = fopen(file->cname_of_File, "wb");
   if (!out)
      {
	 yyerror("cannot open output file '%s'", file->cname_of_File);
	 return;
      }

   fprintf(out, "/*\n");
   fprintf(out, " *\tfile '%s', \n", file->cname_of_File);
   fprintf(out, " *\tautomatically generated by clip-");
   printVersion(out);
   fprintf(out, "\n");
   fprintf(out, " *\tfrom source '%s', \n", file->name_of_File);
   time(&tbuf);
   fprintf(out, " *\tat %s", ctime(&tbuf));
   fprintf(out, " */\n");

   fprintf(out, "\n#include \"ci_clip.h\"\n\n");

   fprintf(out, "static const char *_file = \"%s\";\n\n", file->name_of_File);

   fprintf(out, "/* extern functions */\n");
   for (i = 0; i < file->Coll_externFunctions_of_File.count; i++)
      {
	 VAR(char, sp, file->Coll_externFunctions_of_File.items[i]);

	 fprintf(out, "ClipFunction clip_%s;\n", sp);
      }

   fprintf(out, "\n/* forced references to external functions */\n");
   for (i = 0; i < file->Coll_externFunctions_of_File.count; i++)
      {
	 VAR(char, sp, file->Coll_externFunctions_of_File.items[i]);

	 fprintf(out, "static ClipFunction *__%s = clip_%s;\n", sp, sp);
      }

   fprintf(out, "\n/* forced references to external modules */\n");
   for (i = 0; i < file->Coll_externModules_of_File.count; i++)
      {
	 VAR(char, sp, file->Coll_externModules_of_File.items[i]);

	 fprintf(out, "extern ClipModule clip__MODULE_%s;\n", sp);
	 fprintf(out, "static ClipModule *%s_module_ref = &%s_module;\n", sp, sp);
      }

#if 0
   fprintf(out, "\n/* implicit declared functions */\n");
   for (i = 0; i < file->undeclExternFunctions.count; i++)
      {
	 VAR(char, sp, file->undeclExternFunctions.items[i]);

	 fprintf(out, "ClipFunction %s;\n", sp);
      }
#endif

   fprintf(out, "\n/* file functions */\n");
  /*fprintf(out, "ClipFunction %s;\n", file->main->name); */
   for (i = 0; i < file->Coll_unsortedFunctions_of_File.count; i++)
      {
	 VAR(Function, fp, file->Coll_unsortedFunctions_of_File.items[i]);
	 fprintf(out, "%sClipFunction clip_%s;\n", fp->isPublic_of_Function ? "" : "static ", fp->name_of_Function);
      }

   fprintf(out, "\n/* entry table */\n");

   fprintf(out, "static ClipNameEntry __cfunctions[] =\n{\n");
   for (i = 0; i < file->Coll_functions_of_File.count; i++)
      {
	 VAR(Function, fp, file->Coll_functions_of_File.items[i]);
	 if (fp->isPublic_of_Function == 1)
	    fprintf(out, "\t{ %ld, clip_%s },\n", (long) hashstr(fp->name_of_Function), fp->name_of_Function);
      }
   fprintf(out, "\t{ 0, 0 }\n};\n\n");

   fprintf(out, "static ClipFunction *__inits[] =\n{\n");
   for (i = 0; i < file->Coll_functions_of_File.count; i++)
      {
	 VAR(Function, fp, file->Coll_functions_of_File.items[i]);
	 if (fp->isPublic_of_Function == 2)
	    fprintf(out, "\tclip_%s,\n", fp->name_of_Function);
      }
   fprintf(out, "\t0\n};\n\n");

   fprintf(out, "static ClipFunction *__exits[] =\n{\n");
   for (i = 0; i < file->Coll_functions_of_File.count; i++)
      {
	 VAR(Function, fp, file->Coll_functions_of_File.items[i]);
	 if (fp->isPublic_of_Function == 3)
	    fprintf(out, "\tclip_%s,\n", fp->name_of_Function);
      }
   fprintf(out, "\t0\n};\n\n");

   fprintf(out, "ClipModule clip__MODULE_%s=\n{\n", file->mname_of_File);

   fprintf(out, "\t\"%s\",\n", file->mname_of_File);
   fprintf(out, "\t__cfunctions,\n");
   fprintf(out, "\t__inits,\n");
   fprintf(out, "\t__exits,\n");
   fprintf(out, "\t0,\n");
   fprintf(out, "\t0,\n");
   fprintf(out, "\t0,\n");
   fprintf(out, "\t0,\n");

   fprintf(out, "};\n");

  /*if (genDebug) */
   {
      char buf[256];

      Coll coll;

      init_Coll(&coll, free, strcmp);

      fprintf(out, "\nstatic ClipHashBucket _hash_buckets[%d]=\n{\n", file->Coll_names_of_File.count);
      for (sum = 0, i = 0; i < file->Coll_names_of_File.count; ++i)
	 {
	    int l;

	    char *s = (char *) file->Coll_names_of_File.items[i];

	    snprintf(buf, sizeof(buf), "\t{ %ld, %d }, /* %s */\n", (long) hashstr(s), sum, s);
	    insert_Coll(&coll, strdup(buf));
	    l = strlen(s) + 1;
	    sum += l;

	 }

      for (i = 0; i < coll.count; ++i)
	 fprintf(out, "%s", (char *) coll.items[i]);

      fprintf(out, "};\n");

      fprintf(out, "\nstatic char _hash_mem[%d] =\n{\n", sum);
      for (i = 0; i < file->Coll_names_of_File.count; ++i)
	 {
	    int l, j;

	    char *s = (char *) file->Coll_names_of_File.items[i];

	    l = strlen(s) + 1;
	    fprintf(out, "\t");
	    for (j = 0; j < l; ++j)
	       fprintf(out, "%d, ", s[j]);
	    fprintf(out, "\n");
	 }
      fprintf(out, "};\n");

      fprintf(out, "\nstatic ClipHashNames _hash_names =\n{\n");
      fprintf(out, "\t%d,\n", file->Coll_names_of_File.count);
      fprintf(out, "\t_hash_buckets,\n");
      fprintf(out, "\t_hash_mem,\n");
      fprintf(out, "};\n");

      destroy_Coll(&coll);
   }

   fprintf(out, "\n");

   for (j = 0; j < file->Coll_unsortedNumbers_of_File.count; j++)
      {
	 VAR(ConstNode, sp, file->Coll_unsortedNumbers_of_File.items[j]);
	 char *s = strchr(sp->val, '.');

	 int len, dec = 0;

	 len = strlen(sp->val);
	 if (s)
	    dec = len - (s - sp->val) - 1;
	 if (!s)
	    s = strchr(sp->val, 'e');
	 if (!s)
	    s = strchr(sp->val, 'E');
	 if (len < 10)
	    {
	       if (dec > 0)
		  len = 11 + dec;
	       else
		  len = 10;
	    }
	/*if (dec < 2)
	   dec = 2; */
	 if (dec > len)
	    len = dec + 1;
	 fprintf(out, "static ClipVarNum _num_%d = { { { NUMERIC_type_of_ClipVarType,%d,%d,0,0,0 } , %s%s } };\n", j, len, dec, sp->val, s ? "" : ".0");
      }

   for (j = 0; j < file->Coll_unsortedStrings_of_File.count; j++)
      {
	 VAR(ConstNode, sp, file->Coll_unsortedStrings_of_File.items[j]);
	 fprintf(out, "static ClipVarStr _str_%d = { { { CHARACTER_type_of_ClipVarType,%d,0,F_MSTAT_ClipFlags,0 }, { ", j, haveMacro(sp->val));
	 printString(out, sp->val);
	 fprintf(out, ", %ld } } };\n", (long) strlen(sp->val));
      }

   if (main_flag)
      write_Main(file, out, file->Function_main_of_File->name_of_Function);

   file->Node_ctext_of_File->pass(file->Node_ctext_of_File, CText, 0, out);

   if (file->Coll_codeblocks_of_File.count)
      {

	 fprintf(out, "\n");
	 for (j = 0; j < file->Coll_codeblocks_of_File.count; ++j)
	    {
	       VAR(Function, fp, file->Coll_codeblocks_of_File.items[j]);
	       fprintf(out, "static ClipFunction clip_%s;\n", fp->name_of_Function);
	    }
      }

   fprintf(out, "\n/* file statics */\n");
  /*write_statics(out, file->main->statics); */
   for (i = 0; i < file->Coll_unsortedFunctions_of_File.count; i++)
      {
	 VAR(Function, fp, file->Coll_unsortedFunctions_of_File.items[i]);
	 write_statics(out, fp->VarColl_statics_of_Function);
      }

   {
      int nstatics = 0;

      fprintf(out, "static ClipVarDef _statics[] =\n{\n");
      for (i = 0; i < file->Coll_unsortedFunctions_of_File.count; i++)
	 {
	    VAR(Function, fp, file->Coll_unsortedFunctions_of_File.items[i]);
	    nstatics += fp->VarColl_statics_of_Function->coll.count;
	 }
      fprintf(out, "\t{ %d, 0 },\n ", nstatics);
      for (i = 0; i < file->Coll_unsortedFunctions_of_File.count; i++)
	 {
	    VAR(Function, fp, file->Coll_unsortedFunctions_of_File.items[i]);
	    write_staticDefs(out, fp->VarColl_statics_of_Function);
	 }
      fprintf(out, "};\n");
   }

   fprintf(out, "\nstatic int _init_flag=0;\n");
   write_Function(out, file, file->Function_init_of_File);

   if (file->Coll_codeblocks_of_File.count)
      {

	 fprintf(out, "\n");

	 for (j = 0; j < file->Coll_codeblocks_of_File.count; j++)
	    {
	       VAR(Function, fp, file->Coll_codeblocks_of_File.items[j]);
	       write_Function(out, file, fp);
	    }

      }

  /*write_Function(out, file, file->main); */
   for (i = 0; i < file->Coll_unsortedFunctions_of_File.count; i++)
      {
	 VAR(Function, fp, file->Coll_unsortedFunctions_of_File.items[i]);
	 write_Function(out, file, fp);
      }

   fclose(out);
}

void
compile_File(const char *cname)
{
   char cmd[1024], oname[256], *e, *s;

   int i;

   strcpy(oname, cname);
   s = strrchr(cname, '.');
   e = strrchr(oname, '.');
   strcpy(e, OBJSUF);

#ifdef USE_AS
   if (asm_flag && !strcmp(s, ".s"))
      {
	 snprintf(cmd, sizeof(cmd), "%s -o %s %s", AS_PRG, oname, cname);
      }
   else
#endif
      {

	 snprintf(cmd, sizeof(cmd), "%s", CC);
	 for (e = cmd + strlen(cmd), i = 0; i < includePaths.count; ++i)
	    {
	       snprintf(e, sizeof(cmd) - (e - cmd), " %s %s", INCLUDE_FLAG, (char *) includePaths.items[i]);
	       e = cmd + strlen(cmd);
	    }
	 snprintf(e, sizeof(cmd) - (e - cmd), " %s %s %s %s %s %s %s %s", optLevel ? COPT : "", genDebug ? CDBG : "", CFLAGS, ADDCFLAGS, COMPILE_FLAG, cname, OUT_FLAG, oname);

      }

   v_printf(2, "%s\n", cmd);
   if (system(cmd))
      yyerror("C level error in command: %s", cmd);
   else if (rmc_flag)
      unlink(cname);
}

void
share_File(const char *cname)
{
   char cmd[1024], oname[256], soname[256], *e;

   strcpy(oname, cname);
   e = strrchr(oname, '.');
   strcpy(e, OBJSUF);

   strcpy(soname, cname);
   e = strrchr(soname, '.');
   strcpy(e, SOBJSUF);

   sprintf(cmd, "%s", CC);
/*
   for (e = cmd + strlen(cmd), i = 0; i < includePaths.count; ++i)
   {
   sprintf(e, " %s %s", INCLUDE_FLAG, (char *) includePaths.items[i]);
   e = cmd + strlen(cmd);
   }
 */
   e = cmd + strlen(cmd);
   sprintf(e, " %s %s %s %s %s %s %s", optLevel ? COPT : "", genDebug ? CDBG : "", CFLAGS, SFLAGS, oname, OUT_FLAG, soname);

   v_printf(2, "%s\n", cmd);
   if (system(cmd))
      yyerror("C level error in command: %s", cmd);
}

void
compile_CFile(char *name)
{
   char cmd[1024], cname[256], oname[256], *e;

   int i;

   if (preproc_flag)
      {
	 preprocCFile(name, NULL);
	 return;
      }

  /*snprintf(cname, sizeof(cname), "/tmp/clip.XXXXXX");
     mktemp(cname);
     strcat(cname, ".c"); */
  //snprintf(cname, sizeof(cname), "/tmp/clipar.%lx%lx.pa", (long) getpid(), (long) random());
   snprintf(cname, sizeof(cname), "/tmp/clipar.%lx%lx.c", (long) getpid(), (long) random());

   preprocCFile(name, cname);
   if (clic_errorcount)
      return;

   strcpy(oname, name);
   e = strrchr(oname, '.');
   strcpy(e, OBJSUF);

   sprintf(cmd, "%s", CC);
   for (e = cmd + strlen(cmd), i = 0; i < includePaths.count; ++i)
      {
	 sprintf(e, " %s %s", INCLUDE_FLAG, (char *) includePaths.items[i]);
	 e = cmd + strlen(cmd);
      }
   sprintf(e, " %s %s %s %s %s %s %s", optLevel ? COPT : "", genDebug ? CDBG : "", CFLAGS, COMPILE_FLAG, cname, OUT_FLAG, oname);

   v_printf(2, "%s\n", cmd);
   if (system(cmd))
      yyerror("C level error in command: %s", cmd);

   unlink(cname);
}

/* ] write C file  */
/* [ write PO file  */

#define MODBEG (bp->buf_of_StrBuf+8+2*sizeof(long))
#define BEGOFFS (bp->ptr_of_StrBuf-MODBEG)
#define CUROFFS (bp->ptr_of_StrBuf-bp->buf_of_StrBuf)

#ifdef FORCEALIGN

static void
SETLONG(StrBuf * bp, long offs, long val)
{
   memcpy(bp->buf + offs, &val, sizeof(long));
}

static void
SETSHORT(StrBuf * bp, long offs, short val)
{
   memcpy(bp->buf + offs, &val, sizeof(short));
}

#else

#define SETLONG(bp, offs, val) (*(long*)((bp)->buf_of_StrBuf+(offs))=(val))
#define SETSHORT(bp, offs, val) (*(short*)((bp)->buf_of_StrBuf+(offs))=(val))

#endif

#define LONGINCR(offs) ((offs)+=sizeof(long));
#define SHORTINCR(offs) ((offs)+=sizeof(short));

static void
put_function(File * file, StrBuf * bp, Function * fp, long *loffs)
{
   int j, count, namelen;

   long lp, blp;

  /*printf("put_function: beg: %s\n", fp->name);fflush(stdout); */
   if (loffs)
      {
	 SETLONG(bp, *loffs, hashstr(fp->name_of_Function));
	 LONGINCR(*loffs);
	 SETLONG(bp, *loffs, BEGOFFS);
	 LONGINCR(*loffs);
      }
  /* offs from beg of module */
   putLong_StrBuf(bp, BEGOFFS);
   lp = CUROFFS;
  /* body offs (from BOM) */
  /* 1l */
   putLong_StrBuf(bp, 0);
  /* body len */
  /* 2l */
   putLong_StrBuf(bp, 0);
  /* 3l */
   putByte_StrBuf(bp, fp->isPublic_of_Function);
  /* 3l,0s,1b */
   putShort_StrBuf(bp, fp->nlocals_of_Function);
  /* 3l,1s,1b */
   putShort_StrBuf(bp, fp->reflocals_of_Function);
  /* 3l,2s,1b */
   putShort_StrBuf(bp, fp->maxdeep_of_Function + 1);
   count = fp->VarColl_privates_of_Function->unsorted.count + fp->VarColl_parameters_of_Function->unsorted.count;
  /* 3l,3s,1b */
   putShort_StrBuf(bp, count);
  /*!!! *//* 3l,4s,1b */
   putShort_StrBuf(bp, fp->VarColl_params_of_Function->unsorted.count);

  /* 3l,5s,1b */
   for (j = 0; j < fp->VarColl_privates_of_Function->unsorted.count; ++j)
      {
	 VAR(Var, vp, fp->VarColl_privates_of_Function->unsorted.items[j]);
	 putLong_StrBuf(bp, hashstr(vp->name));
      }
   for (j = 0; j < fp->VarColl_parameters_of_Function->unsorted.count; ++j)
      {
	 VAR(Var, vp, fp->VarColl_parameters_of_Function->unsorted.items[j]);
	 putLong_StrBuf(bp, hashstr(vp->name));
      }

   for (j = 0; j < fp->VarColl_locals_of_Function->coll.count; ++j)
      {
	 VAR(Var, vp, fp->VarColl_locals_of_Function->coll.items[j]);
	 putLong_StrBuf(bp, hashstr(vp->name));
      }
   for (j = 0; j < fp->VarColl_locals_of_Function->coll.count; ++j)
      {
	 VAR(Var, vp, fp->VarColl_locals_of_Function->coll.items[j]);
	 putShort_StrBuf(bp, vp->no);
      }

   for (namelen = 1, j = 0; j < fp->VarColl_locals_of_Function->unsorted.count; j++)
      {
	 VAR(Var, vp, fp->VarColl_locals_of_Function->unsorted.items[j]);
	 namelen += strlen(vp->name) + 1;
      }

   if (fp->isPublic_of_Function == 0)
      {
	 putByte_StrBuf(bp, strlen(fp->name_of_Function + 3));
	 putStr_StrBuf(bp, fp->name_of_Function + 3);
      }
   else
      {
	 putByte_StrBuf(bp, strlen(fp->name_of_Function));
	 putStr_StrBuf(bp, fp->name_of_Function);
      }

   for (j = 0; j < fp->VarColl_locals_of_Function->unsorted.count; j++)
      {
	 VAR(Var, vp, fp->VarColl_locals_of_Function->unsorted.items[j]);
	 if (vp->isRef)
	    continue;
	 putStr_StrBuf(bp, vp->name);
      }
   for (j = 0; j < fp->VarColl_locals_of_Function->unsorted.count; j++)
      {
	 VAR(Var, vp, fp->VarColl_locals_of_Function->unsorted.items[j]);
	 if (!vp->isRef)
	    continue;
	 putStr_StrBuf(bp, vp->name);
      }

   SETLONG(bp, lp, BEGOFFS);
   lp += sizeof(long);

   blp = CUROFFS;
  /*printf("put_function: body: %s\n", fp->name);fflush(stdout); */
   pass_Node(fp->Node_body_of_Function, OText, 0, bp);
   SETLONG(bp, lp, CUROFFS - blp);
  /*printf("put_function: end: %s\n", fp->name);fflush(stdout); */
}

static int
cmp_hash_bucket(void *p1, void *p2)
{
	unsigned long h1 = (unsigned long) ((ClipHashBucket *) p1)->hash_of_ClipHashBucket;

	unsigned long h2 = (unsigned long) ((ClipHashBucket *) p2)->hash_of_ClipHashBucket;

   if (h1 < h2)
      return -1;
   else if (h1 > h2)
      return 1;
   else
      return 0;
}

void
write_OFile(File * file, long *len)
{
   int i, j, count;

   FILE *out;

   time_t timestamp;

   StrBuf *bp;

   char *s;

   long modbeg;

   long modlen;

   long loffs, funcOffs;

   long strOffs, numOffs, initOffs;

   long soffs;

   char *hash_buckets_buf = 0;

   int hash_buckets_len = 0;

  /*printf("write_OFile: %s\n", file->name);fflush(stdout); */

/* generate hash-buckets string */
   {
      Coll coll;

      int sum = 0;

      int count = file->Coll_names_of_File.count;
      StrBuf str_buf = { 0, 0, 0, 0 };
      StrBuf *hbp = &str_buf;

      int l;

      int nstatics = 0;

      for (i = 0; i < file->Coll_unsortedFunctions_of_File.count; i++)
	 {
	    VAR(Function, fp, file->Coll_unsortedFunctions_of_File.items[i]);
	    nstatics += fp->VarColl_statics_of_Function->coll.count;
	 }

      init_Coll(&coll, free, cmp_hash_bucket);
      hbp = new_StrBuf();

      for (i = 0; i < count; ++i)
	 {
	    char *s = (char *) file->Coll_names_of_File.items[i];

	    ClipHashBucket *buck = NEW(ClipHashBucket);

		 buck->hash_of_ClipHashBucket = hashstr(s);
		 buck->offs_of_ClipHashBucket = sum;
	    insert_Coll(&coll, buck);
	    l = strlen(s) + 1;
	    sum += l;
	 }
      putLong_StrBuf(hbp, count);
      putLong_StrBuf(hbp, nstatics);

      for (i = 0; i < file->Coll_unsortedFunctions_of_File.count; i++)
	 {
	    VAR(Function, fp, file->Coll_unsortedFunctions_of_File.items[i]);
	    for (j = 0; j < fp->VarColl_statics_of_Function->coll.count; j++)
	       {
		  VAR(Var, vp, fp->VarColl_statics_of_Function->coll.items[j]);
		  putLong_StrBuf(hbp, hashstr(vp->name));
	       }
	 }

      for (i = 0; i < count; ++i)
	 {
	    ClipHashBucket *buck = (ClipHashBucket *) coll.items[i];

		 putLong_StrBuf(hbp, buck->hash_of_ClipHashBucket);
		 putLong_StrBuf(hbp, buck->offs_of_ClipHashBucket);
	 }
      for (i = 0; i < count; ++i)
	 {
	    char *s = (char *) file->Coll_names_of_File.items[i];

	    putStr_StrBuf(hbp, s);
	 }

      putByte_StrBuf(hbp, 0);
      hash_buckets_len = hbp->ptr_of_StrBuf - hbp->buf_of_StrBuf - 1;
      hash_buckets_buf = hbp->buf_of_StrBuf;

      destroy_Coll(&coll);
   }

   time(&timestamp);
   bp = new_StrBuf();
   *len = 0;

   if (getCount_Node(file->Node_ctext_of_File) || getCount_Node(file->Node_ctextStatic_of_File))
      {
	 yyerror("cannot write PCODE module with c-texts");
	 goto _ret;
      }

   out = fopen(file->cname_of_File, "wb");
   if (!out)
      {
	 yyerror("cannot open output file '%s'", file->cname_of_File);
	 goto _ret;
      }

   if (pc_flag)
      {
	 fprintf(out, "/*\n");
	 fprintf(out, " *\tfile '%s', \n", file->cname_of_File);
	 fprintf(out, " *\tautomatically generated by clip\n");
	 fprintf(out, " *\tfrom source %s\n", file->origname_of_File);
	 fprintf(out, " *\tat %s", ctime(&timestamp));
	 fprintf(out, " */\n\n");

#ifdef USE_AS
	 if (asm_flag)
	    {
	       fprintf(out, "\t.file\t\"%s\"\n\n", file->cname);
	    }
	 else
#endif
	    {
	       fprintf(out, "#include \"ci_clip.h\"\n\n");
	    }

      }

  /* magic */
   write_StrBuf(bp, "!<pobj>\n", 8);
  /* 8 */
   putLong_StrBuf(bp, timestamp);
   modlen = CUROFFS;
  /* 8, 1l */
   putLong_StrBuf(bp, 0);
   modbeg = CUROFFS;
  /* 8,2l == modbeg */
   putLong_StrBuf(bp, 0);	/* static Offs */
  /* 8, 3l */
   putLong_StrBuf(bp, file->staticNo_of_File + 1);	/* 1 for init flag */
  /* 8,4l */
   putShort_StrBuf(bp, file->Coll_unsortedNumbers_of_File.count);
  /* 8,4l,1s */
   putShort_StrBuf(bp, file->Coll_unsortedStrings_of_File.count + 1);
  /* 8,4l,2s */
   putLong_StrBuf(bp, file->pubcount_of_File);
  /* 8,5l,2s */
   putLong_StrBuf(bp, file->allcount_of_File);
  /* 8,6l,2s */
   numOffs = CUROFFS;
   putShort_StrBuf(bp, 0);
  /* 8,6l,3s */
   strOffs = CUROFFS;
   putShort_StrBuf(bp, 0);
  /* 8,6l,4s */
   initOffs = CUROFFS;
   putLong_StrBuf(bp, 0);
  /* 8, 7l, 4s */
   funcOffs = CUROFFS;
   putLong_StrBuf(bp, 0);
  /* 8, 8l, 4s */
/* add */
   putShort_StrBuf(bp, file->initcount_of_File);
   putShort_StrBuf(bp, file->exitcount_of_File);
/* eadd */
  /* 8, 8l, 6s */
   putStr_StrBuf(bp, file->origname_of_File);
   SETSHORT(bp, numOffs, BEGOFFS);
   for (j = 0; j < file->Coll_unsortedNumbers_of_File.count; j++)
      {
	 VAR(ConstNode, sp, file->Coll_unsortedNumbers_of_File.items[j]);
	 putDouble_StrBuf(bp, strtod(sp->val, NULL));
      }
   for (j = 0; j < file->Coll_unsortedNumbers_of_File.count; j++)
      {
	 VAR(ConstNode, sp, file->Coll_unsortedNumbers_of_File.items[j]);
	 int dec = 0;

	 char *s = strchr(sp->val, '.');

	 int len = strlen(sp->val);

	 if (s)
	    dec = len - (s - sp->val) - 1;
	 if (len < 10)
	    {
	       if (dec > 0)
		  len = 11 + dec;
	       else
		  len = 10;
	    }
	/*if (dec < 2)
	   dec = 2; */
	 if (dec > len)
	    len = dec + 1;
	 putByte_StrBuf(bp, len);
      }
   for (j = 0; j < file->Coll_unsortedNumbers_of_File.count; j++)
      {
	 VAR(ConstNode, sp, file->Coll_unsortedNumbers_of_File.items[j]);
	 int dec = 0;

	 char *s = strchr(sp->val, '.');

	 int len = strlen(sp->val);

	 if (s)
	    dec = len - (s - sp->val) - 1;
	 if (len < 10)
	    len = 10;
	/*if (dec < 2)
	   dec = 2; */
	 if (dec > len)
	    len = dec + 1;
	 putByte_StrBuf(bp, dec);
      }

   count = file->Coll_unsortedStrings_of_File.count;
   SETSHORT(bp, strOffs, BEGOFFS);
   soffs = CUROFFS;
   for (j = 0; j < count + 1; j++)
      {
	 putLong_StrBuf(bp, 0);
	 putLong_StrBuf(bp, 0);
      }
   for (j = 0; j < count; j++)
      {
	 VAR(ConstNode, sp, file->Coll_unsortedStrings_of_File.items[j]);
	 int le = strlen(sp->val);

	/*int le = sp->len; */

	 SETLONG(bp, soffs, le);
	 LONGINCR(soffs);
	 SETLONG(bp, soffs, BEGOFFS);
	 LONGINCR(soffs);
	 write_StrBuf(bp, sp->val, le + 1);
      }

   {
      SETLONG(bp, soffs, hash_buckets_len);
      LONGINCR(soffs);
      SETLONG(bp, soffs, BEGOFFS);
      LONGINCR(soffs);
      write_StrBuf(bp, hash_buckets_buf, hash_buckets_len + 1);
   }

   SETLONG(bp, initOffs, BEGOFFS);
   put_function(file, bp, file->Function_init_of_File, 0);
   loffs = CUROFFS;
   SETLONG(bp, funcOffs, BEGOFFS);
   for (j = 0; j < file->allcount_of_File; ++j)
      {
	 putLong_StrBuf(bp, 0);	/* hash */
	 putLong_StrBuf(bp, 0);	/* offs */
      }
   for (j = 0; j < file->Coll_unsortedFunctions_of_File.count; ++j)
      {
	 VAR(Function, fp, file->Coll_unsortedFunctions_of_File.items[j]);
	 if (fp->isPublic_of_Function == 1)
	    put_function(file, bp, fp, &loffs);
      }
   for (j = 0; j < file->Coll_unsortedFunctions_of_File.count; ++j)
      {
	 VAR(Function, fp, file->Coll_unsortedFunctions_of_File.items[j]);
	 if (fp->isPublic_of_Function == 2)
	    put_function(file, bp, fp, &loffs);
      }
   for (j = 0; j < file->Coll_unsortedFunctions_of_File.count; ++j)
      {
	 VAR(Function, fp, file->Coll_unsortedFunctions_of_File.items[j]);
	 if (fp->isPublic_of_Function == 3)
	    put_function(file, bp, fp, &loffs);
      }
   for (j = 0; j < file->Coll_unsortedFunctions_of_File.count; ++j)
      {
	 VAR(Function, fp, file->Coll_unsortedFunctions_of_File.items[j]);
	 if (!fp->isPublic_of_Function)
	    put_function(file, bp, fp, &loffs);
      }
   for (j = 0; j < file->Coll_codeblocks_of_File.count; ++j)
      {
	 VAR(Function, fp, file->Coll_codeblocks_of_File.items[j]);
	 put_function(file, bp, fp, &loffs);
      }

   SETLONG(bp, modlen, BEGOFFS);

#ifdef USE_AS
   if (asm_flag)
      {
	 char *mp;

	 long j, modlen, size;

	 char *name = strdup(file->name), *upname;

	 char *s;

	 s = strchr(name, '.');
	 if (s)
	    *s = 0;

	 upname = strdup(name);
	 for (i = 0; i < strlen(name); ++i)
	    upname[i] = toupper(name[i]);

	 fprintf(out, ".data\n");
	 fprintf(out, "\t.align 4\n");
	/*fprintf(out, "\t.type %s_statics,@object\n", name); */
	 fprintf(out, "%s_statics:\n", name);

	 for (i = 0; i < file->staticNo + 1; ++i)
	    {
	       fprintf(out, ".byte 0x0\n");
	       fprintf(out, ".byte 0x0\n");
	       fprintf(out, "\t.zero 2\n");
	       fprintf(out, "\t.zero 12\n");
	    }
/*		fprintf(out, "\t.size %s_statics,%d\n", name, (file->staticNo + 1) * sizeof(ClipVar));*/
	 fprintf(out, ".section\t.rodata\n");
	/*fprintf(out, "\t.type %s_body,@object\n", name); */
	 fprintf(out, "%s_body:\n", name);

	 mp = bp->buf;
	 modlen = bp->ptr - bp->buf;
	 size = 0;

	 for (j = 0; j < modlen; j++)
	    fprintf(out, ".byte %d\n", mp[j]);

/*		fprintf(out, "\t.size %s_body,%ld\n", name, modlen);*/

	 fprintf(out, ".globl %sclip__PCODE_%s\n", US, upname);
	 fprintf(out, ".data\n");
	 fprintf(out, "\t.align 32\n");
	/*fprintf(out, "\t.type clip__PCODE_%s,@object\n", upname); */
/*		fprintf(out, "\t.size clip__PCODE_%s,%d\n", upname, sizeof(ClipFile));*/
	 fprintf(out, US "clip__PCODE_%s:\n", upname);

	 fprintf(out, "\t.long 1\n");
	 fprintf(out, "\t.long %s_body\n", name);
	 fprintf(out, "\t.long %s_body\n", name);
	 fprintf(out, "\t.long %ld\n", size);
	 fprintf(out, "\t.long 3\n");
	 fprintf(out, "\t.long .LC0\n");
	 fprintf(out, "\t.long %s_statics\n", name);
	 fprintf(out, "\t.long 3\n");
	 fprintf(out, "\t.long %d\n", file->staticNo);
	 fprintf(out, "\t.long 0\n");
	 fprintf(out, "\t.long 0\n");
	 fprintf(out, "\t.long 0\n");
	 fprintf(out, "\t.long 0\n");
	 fprintf(out, "\t.long 0\n");
	 fprintf(out, "\t.long -1\n");

	 fprintf(out, ".section\t.rodata\n");
	 fprintf(out, ".LC0:\n\t.string \"%s\"\n", name);

	 fprintf(out, ".data\n");

	 fprintf(out, "\t.align 4\n");
	/*fprintf(out, "\t.type %s_cpfiles,@object\n", upname); */
	 fprintf(out, "%s_cpfiles:\n", upname);
	 fprintf(out, "\t.long %sclip__PCODE_%s\n", US, upname);
	 fprintf(out, "\t.long 0\n");
/*		fprintf(out, "\t.size %s_cpfiles,%d\n", upname, sizeof(ClipFile *) * 2);*/
/*		fprintf(out, ".globl %sclip__MODULE_%s\n", US, upname);*/
	 fprintf(out, ".globl %sclip__MODULE_%s\n", US, file->mname);
	 fprintf(out, "\t.align 32\n");
/*		fprintf(out, "\t.type clip__MODULE_%s,@object\n", upname);*/
/*		fprintf(out, "\t.size clip__MODULE_%s,%d\n", upname, sizeof(ClipModule));*/
/*		fprintf(out, US"clip__MODULE_%s:\n ", upname);      */
	 fprintf(out, US "clip__MODULE_%s:\n ", file->mname);
	 fprintf(out, "\t.long .LC0\n");
	 fprintf(out, "\t.long 0\n");
	 fprintf(out, "\t.long 0\n");
	 fprintf(out, "\t.long 0\n");

	 fprintf(out, "\t.long %s_cpfiles\n", upname);
	 fprintf(out, "\t.long 0\n");
	 fprintf(out, "\t.long 0\n");
	 fprintf(out, "\t.long 0\n");

	 free(name);
	 free(upname);
      }
   else
#endif
   if (pc_flag)
      {
	 char *mp;

	 long j, k, modlen, size;

	 char *name = strdup(file->name_of_File);

	 char *s;

	 s = strchr(name, '.');
	 if (s)
	    *s = 0;
	 fprintf(out, "static ClipVar %s_statics[] =\n{\n", name);
	 for (i = 0; i < file->staticNo_of_File + 1; ++i)
	    fprintf(out, "\t{{0, 0}},\n");
	 fprintf(out, "\n};\n");
	 fprintf(out, "/*body1 of module %s*/\n", name);
	 fprintf(out, "\nstatic const char %s_body[]=\n{\n", name);

	 mp = bp->buf_of_StrBuf;
	 modlen = bp->ptr_of_StrBuf - bp->buf_of_StrBuf;
	 size = 0;
	 j = 0;

	 while (j < modlen)
	    {
	       fprintf(out, "\t");
	       for (k = 0; k < 32 && j < modlen; ++j, ++k, ++size)
		  fprintf(out, "%ld,", (long) mp[j]);
	       fprintf(out, "\n");
	    }

	 fprintf(out, "\n};\n");
	 fprintf(out, "\nstruct ClipFile clip__PCODE_");
	 for (i = 0; i < strlen(name); ++i)
	    fputc(toupper(name[i]), out);
	 fprintf(out, " =\n{\n");
	 fprintf(out, "\t1,\n");
	 fprintf(out, "\t(char*)%s_body,\n", name);
	 fprintf(out, "\t(char*)%s_body,\n", name);
	 fprintf(out, "\t%ld,\n", size);
	 fprintf(out, "\t3,\n");
	 fprintf(out, "\t\"%s\",\n", /*arname */ name);
	 fprintf(out, "\t%s_statics,\n", name);
	 fprintf(out, "\t3,\n");
	 fprintf(out, "\t%d,\n", file->staticNo_of_File);
	 fprintf(out, "\t0,\n");
	 fprintf(out, "\t0,\n");
	 fprintf(out, "\t0,\n");
	 fprintf(out, "\t0,\n");
	 fprintf(out, "\t0,\n");
	 fprintf(out, "\t-1,\n");
	 fprintf(out, "};\n\n");

	 for (i = 0; i < strlen(name); ++i)
	    name[i] = toupper(name[i]);

	 fprintf(out, "static ClipFile *%s_cpfiles[]=\n{\n", name);
	 fprintf(out, "\t&clip__PCODE_%s,\n", name);
	 fprintf(out, "\t0\n};\n");

/*		fprintf(out, "\nClipModule clip__MODULE_%s =\n{\n ", name);*/
	 fprintf(out, "\nClipModule clip__MODULE_%s =\n{\n ", file->mname_of_File);
	 fprintf(out, "\t\"%s\",\n", name);
	 fprintf(out, "\t0,\n");
	 fprintf(out, "\t0,\n");
	 fprintf(out, "\t0,\n");
	/*fprintf(out, "\t&clip__PCODE_%s,\n", name); */

	 fprintf(out, "\t%s_cpfiles,\n", name);
	 fprintf(out, "\t0,\n");
	 fprintf(out, "\t0,\n");
	 fprintf(out, "\t0,\n");
	 fprintf(out, "};\n\n");

	 free(name);
      }
   else
      {
	 for (s = bp->buf_of_StrBuf; s < bp->ptr_of_StrBuf; s += 4096)
	    {
	       int size = 4096, rest = bp->ptr_of_StrBuf - s;

	       if (rest < 4096)
		  size = rest;
	       if (fwrite(s, size, 1, out) != 1)
		  {
		     yyerror("error writing file '%s', %s", file->cname_of_File, strerror(errno));
		     break;
		  }
	    }
      }

   *len = ftell(out);
   fclose(out);
 _ret:
   delete_StrBuf(bp);
}

/* ] write PO file  */

char *
strsuff(const char *str, const char *suff)
{
   const char *e;

   int l = strlen(str);

   int ls = strlen(suff);

   for (e = str + l - ls; e >= str; e--)
      if (!memcmp(e, suff, ls))
	 return (char *) e;
   return 0;
}

void
test_File(File * mp)
{
}

static void
add_name(Coll * cp, char *s)
{
   int no;

   if (!names_flag)
      return;
   if (!search_Coll(cp, s, &no))
      atInsert_Coll(cp, strdup(s), no);
}

static void
read_file(char *path, Coll * coll)
{
   FILE *f;

   char buf[128];

   f = fopen(path, "r");
   if (!f)
      return;

   while (fgets(buf, sizeof(buf), f))
      {
	 int l;

	 l = strlen(buf);
	 while (l > 0 && (buf[l - 1] == '\n' || buf[l - 1] == '\r'))
	    l--;
	 buf[l] = 0;
	 if (!l || buf[0] == '#')
	    continue;

	 add_name(coll, buf);
      }

   fclose(f);
}

static void
read_names(char *s, Coll * ex, Coll * nm)
{
   int al;

   char path[256], *e, *b;

   if (!names_flag)
      return;

   b = strrchr(s, '/');
   if (b)
      b++;
   else
      b = s;
   e = strchr(b, '.');

   if (e && (e > b))
      al = e - s;
   else
      al = strlen(s);

   snprintf(path, sizeof(path), "%s", s);
   snprintf(path + al, sizeof(path) - al, ".ex");
   read_file(path, ex);
   snprintf(path + al, sizeof(path) - al, ".nm");
   read_file(path, nm);
}

typedef struct
{
   long hash;
   int label;
   int branch;
   char *str;
}
Label;

static int
cmp_Label(void *p1, void *p2)
{
   Label *l1 = (Label *) p1;

   Label *l2 = (Label *) p2;

   if (l1->hash < l2->hash)
      return -1;
   else if (l1->hash > l2->hash)
      return 1;
   else
      return 0;
}

static Label *
new_Label(char *str)
{
   Label *ret;

   ret = (Label *) calloc(1, sizeof(Label));
   ret->hash = hashstr(str + 5);
   ret->str = str;

   return ret;
}

static void
print_tree(FILE * out, Coll * tree, int beg, int end)
{
   int med;

   Label *lp, *rp, *cp;

   if (beg > end)
      return;

   med = (beg + end) / 2;

   fprintf(out, "#1234/%d/%d/%d\n", beg, med, end);

   if (beg == end)
      {
	 cp = (Label *) tree->items[beg];

	/*fprintf(out, "\t.p2align 4,,7\n"); */
	 fprintf(out, ".L%d:\n", cp->branch);
	 fprintf(out, "\tcmpl $%ld,%%eax\n", cp->hash);
	 fprintf(out, "\tje .L%d\n", cp->label);
	 fprintf(out, "\tjmp .L1\n");
	 return;
      }
   else if ((beg + 1) == med && (end - 1) == med)
      {
	 cp = (Label *) tree->items[med];
	 lp = (Label *) tree->items[beg];
	 rp = (Label *) tree->items[end];

	/*fprintf(out, "\t.p2align 4,,7\n"); */
	 fprintf(out, ".L%d:\n", cp->branch);
	 fprintf(out, "\tcmpl $%ld,%%eax\n", lp->hash);
	 fprintf(out, "\tje .L%d\n", lp->label);
	 fprintf(out, "\tcmpl $%ld,%%eax\n", cp->hash);
	 fprintf(out, "\tje .L%d\n", cp->label);
	 fprintf(out, "\tcmpl $%ld,%%eax\n", rp->hash);
	 fprintf(out, "\tje .L%d\n", rp->label);

	 fprintf(out, "\tjmp .L1\n");
	 return;
      }
   else if (beg == (end - 1))
      {
	 cp = (Label *) tree->items[beg];
	 rp = (Label *) tree->items[end];

	/*fprintf(out, "\t.p2align 4,,7\n"); */
	 fprintf(out, ".L%d:\n", cp->branch);
	 fprintf(out, "\tcmpl $%ld,%%eax\n", cp->hash);
	 fprintf(out, "\tje .L%d\n", cp->label);
	 fprintf(out, "\tcmpl $%ld,%%eax\n", rp->hash);
	 fprintf(out, "\tje .L%d\n", rp->label);

	 fprintf(out, "\tjmp .L1\n");
	 return;
      }
   else
      {
	 cp = (Label *) tree->items[med];
	 lp = (Label *) tree->items[(beg + med - 1) / 2];
	 rp = (Label *) tree->items[(med + 1 + end) / 2];

	/*fprintf(out, "\t.p2align 4,,7\n"); */
	 fprintf(out, ".L%d:\n", cp->branch);
	 fprintf(out, "\tcmpl $%ld,%%eax\n", cp->hash);
	 fprintf(out, "\tje .L%d\n", cp->label);
	 fprintf(out, "\tcmpl $%ld,%%eax\n", cp->hash);
	 fprintf(out, "\tjl .L%d\n", lp->branch);
	 fprintf(out, "\tcmpl $%ld,%%eax\n", cp->hash);
	 fprintf(out, "\tjg .L%d\n", rp->branch);

	 fprintf(out, "\tjmp .L1\n");
      }

   print_tree(out, tree, beg, med - 1);
   print_tree(out, tree, med + 1, end);
}

static void
print_labels(FILE * out, Coll * tree)
{
   int i, count;

   for (i = 0, count = tree->count; i < count; i++)
      {
	 Label *cp = (Label *) tree->items[i];

	/*fprintf(out, "\t.p2align 4,,7\n"); */
	 fprintf(out, ".L%d:\n", cp->label);
	 fprintf(out, "\tmovl $%s%s,%%eax\n", US, cp->str);
	 fprintf(out, "\tjmp .L2\n");
      }
}

void
write_Cfunc(const char *name, int argc, char **argv, Coll * ex, Coll * nm)
{
   char buf[1024 * 8];

   char word1[80], word2[80], word3[80];

   int i, use_asm = 0, count;

   FILE *in = 0, *out = 0;

   int shared = shared_flag || eshared_flag;

   time_t tbuf;

   Coll names;

   Coll fnames;

   Coll dnames;

   Coll rnames;

   Coll objs;

   Coll libs;

   Coll nlibs, slibs;

   Coll alibs;

   int labn, labcn;

   init_Coll(&names, free, strcmp);
   init_Coll(&fnames, free, 0 /*strcmp */ );
   init_Coll(&dnames, free, strcmp);
   init_Coll(&rnames, free, strcmp);
   init_Coll(&objs, free, 0);
   init_Coll(&libs, free, 0);
   init_Coll(&nlibs, free, 0);
   init_Coll(&slibs, 0, strcmp);
   init_Coll(&alibs, 0, 0);

   strcpy(buf, NM_PRG);

   for (i = 0; i < argc; i++)
      {
	 char *a = argv[i];

	 if (a[0] == '-' && a[1] == 'l')
	    {
	       char path[256];

	       snprintf(path, sizeof(path), "%s/lib/lib%s%s", CLIPROOT, a + 2, SLIBSUF);
	       if (!access(path, R_OK))
		  append_Coll(&libs, strdup(path));

	       continue;
	    }
	 if (a[0] == '-')
	    {
	       continue;
	    }
	 if (strsuff(a, SLIBSUF) || strsuff(a, LIBSUF))
	    {
	       if (a[0] == '/' || (a[0] == '.' && a[1] == '/') || (a[0] == '.' && a[1] == '.' && a[2] == '/'))
		  {
		     append_Coll(&libs, strdup(a));
		  }
	       else
		  {
		     char path[256];

		     snprintf(path, sizeof(path), "%s/lib/%s", CLIPROOT, a);
		     if (!access(path, R_OK))
			append_Coll(&libs, strdup(path));
		  }
	    }
	 else if (strsuff(a, SOBJSUF) || strsuff(a, OBJSUF))
	    append_Coll(&objs, strdup(a));
      }

   for (i = libs.count - 1; i >= 0; i--)
      {
	 char *s = (char *) libs.items[i];

	 char *e, *r, *b;

	 int l, j, ind, isA = 0;

	 e = strsuff(s, SLIBSUF);

	 if (!e)
	    {
	       e = strsuff(s, LIBSUF);
	       isA = 1;
	    }
	 b = strrchr(s, '/');
	 if (!b)
	    b = s;
	 else
	    b++;
	 if (e && e > b + 1)
	    l = e - b;
	 else
	    l = strlen(b);
	 r = (char *) malloc(l + 1);
	 for (j = 0; j < l; j++)
	    {
	       switch (b[j])
		  {
		  case '-':
		     r[j] = '_';
		     break;
		  default:
		     r[j] = b[j];
		     break;
		  }
	    }
	 r[l] = 0;
	 if (!search_Coll(&slibs, s, &ind))
	    {
	       insert_Coll(&slibs, r);
	       if (isA)
		  {
		     append_Coll(&alibs, s);
		     free(r);
		  }
	       else
		  append_Coll(&nlibs, r);
	    }
	 else
	    free(r);

	 read_names(s, ex, nm);
      }

   if (!shared)
      {
	 for (i = 0; i < libs.count; i++)
	    {
	       char *s = (char *) libs.items[i];

	       strcat(buf, " ");
	       strcat(buf, s);
	    }
      }
   else
      {
	 for (i = 0; i < alibs.count; i++)
	    {
	       char *s = (char *) alibs.items[i];

	       strcat(buf, " ");
	       strcat(buf, s);
	    }
      }

   for (i = 0; i < objs.count; i++)
      {
	 char *s = (char *) objs.items[i];

	 strcat(buf, " ");
	 strcat(buf, s);

	 read_names(s, ex, nm);
      }

   v_printf(2, "%s\n", buf);

   in = popen(buf, "r");

   if (!in)
      {
	 yyerror("cannot open pipe '%s'", buf);
	 goto end;
      }

#ifdef USE_AS
   {
      char *s = strrchr(name, '.');

      if (asm_flag && s && !strcmp(s, ".s"))
	 use_asm = 1;
   }
#endif

   out = fopen(name, "wb");
   if (!out)
      {
	 yyerror("cannot open output file '%s'", name);
	 goto end;
      }

   fprintf(out, "/*\n");
   fprintf(out, " *\tautomatically generated by clip-");
   printVersion(out);
   fprintf(out, "\n");
   time(&tbuf);
   fprintf(out, " *\tat %s", ctime(&tbuf));
   fprintf(out, " *\tfrom sources:\n");
   for (i = 0; i < argc; ++i)
      fprintf(out, " *\t%s\n", argv[i]);
   fprintf(out, " */\n");

   if (!use_asm)
      {
	 fprintf(out, "\n#include \"ci_clip.h\"\n");
      }
   else
      {
	 fprintf(out, "\n\t.file \"%s\"\n", name);
      }

   while (fgets(buf, sizeof(buf), in) != NULL)
      {
	 char *s, *sp;

	 int br;

	 int n = sscanf(buf, "%s %s %s", word1, word2, word3);

	 int l;

	 if (n == 3)
	    {
	       if (!strcmp(word2, "T"))
		  br = 1;
	       else if (!strcmp(word2, "D"))
		  br = 2;
	       else
		  continue;
	       sp = word3;
	    }
	 else if (n == 2)
	    {
	       if (strcmp(word1, "U"))
		  continue;
	       sp = word2;
	       br = 3;
	    }
	 else
	    continue;
#ifdef NM_UNDERSCORE
	 sp++;
#endif
	 l = strlen(sp);
	 if (l < 6 || memcmp(sp, "clip_", 5))
	    goto next;

	 for (s = sp + 5; *s; ++s)
	    if (!isupper(*s) && !isdigit(*s) && *s != '_')
	       goto next;
	 if (br == 2)
	    {
	       if (!memcmp(sp + 5, "_PCODE_", 7))
		  insert_Coll(&fnames, strdup(sp));
	       else if (!memcmp(sp + 5, "_RDD_", 4))
		  insert_Coll(&dnames, strdup(sp));
	       else if (!memcmp(sp + 5, "_RTTI_", 6))
		  insert_Coll(&rnames, strdup(sp));
	    }
	 else
	    insert_Coll(&names, strdup(sp));
       next:
	 ;
      }
   if (in)
      {
	 pclose(in);
	 in = 0;
      }

   for (i = 0; i < names.count; ++i)
      {
	 VAR(char, s, names.items[i]);

	 if (!use_asm)
	    fprintf(out, "ClipFunction %s;\n", s);
	 add_name(nm, s);
      }

   if (shared)
      {
	 for (i = nlibs.count - 1; i >= 0; i--)
	    {
	       char *s = (char *) nlibs.items[i];

	       if (!use_asm)
		  fprintf(out, "CLIP_DLLIMPORT ClipFunction *_clip_builtin_%s ( long hash );\n", s);
	    }
      }

   labn = 3;
   labcn = 0;
   if (!use_asm)
      {
	 fprintf(out, "\nstatic ClipFunction *\n_builtins(long hash)\n{\n");

	 if (shared)
	    {
	       fprintf(out, "\tClipFunction *rp = 0;\n");
	       for (i = nlibs.count - 1; i >= 0; i--)
		  {
		     char *s = (char *) nlibs.items[i];

		     fprintf(out, "\trp = _clip_builtin_%s ( hash );\n", s);
		     fprintf(out, "\tif ( rp )\n\t\treturn rp;\n");
		  }
	    }

	 fprintf(out, "\n\tswitch( hash )\n\t{\n");
	 for (i = 0; i < names.count; ++i)
	    {
	       VAR(char, s, names.items[i]);

	       if (!memcmp(s + 5, "INIT_", 5) || !memcmp(s + 5, "EXIT_", 5))
		  continue;
	       fprintf(out, "\tcase %ld:\n", (long) hashstr(s + 5));
	       fprintf(out, "\t\treturn %s;\n", s);
	    }
	 fprintf(out, "\tdefault:\n\t\treturn 0;\n");
	 fprintf(out, "\t}\n");
	 fprintf(out, "};\n\n");
      }
   else
      {

	 fprintf(out, ".text\n\t.align 4\n");
/*		fprintf(out, "\t.type\t_builtins,@function\n");*/
	 fprintf(out, US "_builtins:\n");

	 fprintf(out, "\tpushl %%ebp\n");
	 fprintf(out, "\tmovl %%esp,%%ebp\n");
#ifdef _WIN32
	 fprintf(out, "\tsubl $36,%%esp\n");
	 fprintf(out, "\tpushl %%ebx\n");
#else
	 fprintf(out, "\tsubl $24,%%esp\n");
#endif
	 fprintf(out, "\tmovl $0,-4(%%ebp)\n");

	 if (shared)
	    {
	       for (i = nlibs.count - 1; i >= 0; i--)
		  {
		     char *s = (char *) nlibs.items[i];

		     fprintf(out, "\taddl $-12,%%esp\n");
		     fprintf(out, "\tmovl 8(%%ebp),%%eax\n");
		     fprintf(out, "\tpushl %%eax\n");
#ifdef _WIN32
		     fprintf(out, "\tmovl %s%s_clip_builtin_%s, %%ebx\n", IMP, US, s);
		     fprintf(out, "\tcall *%%ebx\n");
#else
		     fprintf(out, "\tcall %s%s_clip_builtin_%s\n", IMP, US, s);
#endif
		     fprintf(out, "\taddl $16,%%esp\n");
		     fprintf(out, "\tmovl %%eax,%%eax\n");
		     fprintf(out, "\tmovl %%eax,-4(%%ebp)\n");
		     fprintf(out, "\tcmpl $0,-4(%%ebp)\n");
		     fprintf(out, "\tje .L%d\n", labn);
		     fprintf(out, "\tmovl -4(%%ebp),%%edx\n");
		     fprintf(out, "\tmovl %%edx,%%eax\n");
		     fprintf(out, "\tjmp .L2\n");
		     fprintf(out, "\t.p2align 4,,7\n");
		     fprintf(out, ".L%d:\n", labn);
		     labn++;
		  }
	    }
	 if (names.count)
	    {
	      /* tree create */
	       Coll tree;

	       int i;

	       init_Coll(&tree, free, cmp_Label);

	       fprintf(out, "\tmovl 8(%%ebp),%%eax\n");

	       for (i = 0; i < names.count; i++)
		  insert_Coll(&tree, new_Label((char *) (names.items[i])));

	       for (i = 0; i < tree.count; i++)
		  ((Label *) (tree.items[i]))->branch = labn++;
	       for (i = 0; i < tree.count; i++)
		  ((Label *) (tree.items[i]))->label = labn++;

	       print_tree(out, &tree, 0, names.count - 1);
	       print_labels(out, &tree);

	       destroy_Coll(&tree);
	    }
	 fprintf(out, "\t.p2align 4,,7\n");
	 fprintf(out, ".L1:\n");
	 fprintf(out, "\txorl %%eax,%%eax\n");
	 fprintf(out, "\tjmp .L2\n");
	 fprintf(out, "\t.p2align 4,,7\n");
	 fprintf(out, ".L2:\n");
#ifdef _WIN32
	 fprintf(out, "\tmovl -40(%%ebp),%%ebx\n");
	 fprintf(out, "\tmovl %%ebp,%%esp\n");
	 fprintf(out, "\tpopl %%ebp\n");
#else
	 fprintf(out, "\tleave\n");
#endif
	 fprintf(out, "\tret\n");

	 fprintf(out, ".L%d:\n", labn);
/*		fprintf(out, "\t.size\t_builtins,.L%d-_builtins\n", labn);*/
	 labn++;

      }

   if (!use_asm)
      {
	 fprintf(out, "\nstatic ClipFunction *_inits[]=\n{\n");
      }
   else
      {
	 fprintf(out, ".data\n");
	 fprintf(out, "\t.align 4\n");
/*		fprintf(out, "\t.type\t_inits,@object\n");*/
	 fprintf(out, US "_inits:\n");
      }

  /* CLIPINIT, if defined, _must_ be first */
   count = 0;
   for (i = 0; i < names.count; ++i)
      {
	 VAR(char, s, names.items[i]);

	 if (strcmp(s + 5, "INIT_CLIPINIT"))
	    continue;
	 if (!use_asm)
	    fprintf(out, "\t%s,\n", s);
	 else
	    fprintf(out, "\t.long %s%s\n", US, s);
	 count++;
      }
   for (i = 0; i < names.count; ++i)
      {
	 VAR(char, s, names.items[i]);

	 if (memcmp(s + 5, "INIT_", 5) || !strcmp(s + 5, "INIT_CLIPINIT"))
	    continue;
	 if (!use_asm)
	    fprintf(out, "\t%s,\n", s);
	 else
	    fprintf(out, "\t.long %s%s\n", US, s);
	 count++;
      }
   if (!use_asm)
      {
	 fprintf(out, "\t0,\n");
	 fprintf(out, "};\n\n");
      }
   else
      {
	 fprintf(out, "\t.long 0\n");
/*		fprintf(out, "\t.size _inits,%d\n", (count + 1) * sizeof(ClipFunction *));*/
      }

   if (!use_asm && shared)
      {
	 for (i = 0; i < nlibs.count; ++i)
	    {
	       VAR(char, s, nlibs.items[i]);

	       fprintf(out, "CLIP_DLLIMPORT extern ClipFunction **_libinits_%s;\n", s);
	    }
      }

   if (!use_asm)
      fprintf(out, "\nstatic ClipFunction ***_libinits[]=\n{\n");
   else
      {
	 fprintf(out, "\t.align 4\n");
/*		fprintf(out, "\t.type\t_libinits,@object\n");*/
	 fprintf(out, US "_libinits:\n");
      }

   if (shared)
      {
	 for (i = 0; i < nlibs.count; ++i)
	    {
	       if (!use_asm)
		  fprintf(out, "\t0,\n");
	       else
		  fprintf(out, "\t.long 0\n");
	    }
      }
   if (!use_asm)
      {
	 fprintf(out, "\t0,\n");
	 fprintf(out, "};\n\n");
      }
   else
      {
	 fprintf(out, "\t.long 0\n");
/*		fprintf(out, "\t.size _libinits,%d\n", ((shared ? nlibs.count : 0) + 1) * sizeof(ClipFunction ***));*/
      }

   if (!use_asm)
      {
	 fprintf(out, "\nstatic ClipFunction *_exits[]=\n{\n");
      }
   else
      {
	 fprintf(out, "\t.align 4\n");
/*		fprintf(out, "\t.type\t_exits,@object\n");*/
	 fprintf(out, US "_exits:\n");
      }

   count = 0;
   for (i = 0; i < names.count; ++i)
      {
	 VAR(char, s, names.items[i]);

	 if (memcmp(s + 5, "EXIT_", 5))
	    continue;
	 if (!use_asm)
	    fprintf(out, "\t%s,\n", s);
	 else
	    fprintf(out, "\t.long %s\n", s);
	 count++;
      }
   if (!use_asm)
      {
	 fprintf(out, "\t0\n");
	 fprintf(out, "};\n\n");
      }
   else
      {
	 fprintf(out, "\t.long 0\n");
/*		fprintf(out, "\t.size _exits,%d\n", (count + 1) * sizeof(ClipFunction *));*/
      }

   if (!use_asm && shared)
      {
	 for (i = 0; i < nlibs.count; ++i)
	    {
	       VAR(char, s, nlibs.items[i]);

	       fprintf(out, "CLIP_DLLIMPORT extern ClipFunction **_libexits_%s;\n", s);
	    }
      }
   if (!use_asm)
      fprintf(out, "\nstatic ClipFunction ***_libexits[]=\n{\n");
   else
      {
	 fprintf(out, "\t.align 4\n");
/*		fprintf(out, "\t.type\t_libexits,@object\n");*/
	 fprintf(out, US "_libexits:\n");
      }

   if (shared)
      {
	 for (i = 0; i < nlibs.count; ++i)
	    {
	       if (!use_asm)
		  fprintf(out, "\t0,\n");
	       else
		  fprintf(out, "\t.long 0\n");
	    }
      }
   if (!use_asm)
      {
	 fprintf(out, "\t0,\n");
	 fprintf(out, "};\n\n");
      }
   else
      {
	 fprintf(out, "\t.long 0\n");
/*		fprintf(out, "\t.size _libexits,%d\n", (nlibs.count + 1) * sizeof(ClipFunction ***));*/
      }

   if (!use_asm)
      fprintf(out, "\nstatic const char *_pfunctions[]=\n{\n");
   else
      {
	 fprintf(out, "\t.align 4\n");
/*		fprintf(out, "\t.type\t_pfunctions,@object\n");*/
/*		fprintf(out, "\t.size\t_pfunctions,%d\n", (poName.count + paName.count + 1) * sizeof(char *));*/
	 fprintf(out, US "_pfunctions:\n");
      }
   count = 0;
   for (i = 0; i < poName.count; ++i)
      {
	 VAR(char, name, poName.items[i]);

	 if (!use_asm)
	    fprintf(out, "\t\"%s\",\n", name);
	 else
	    fprintf(out, "\t.long .LC%d\n", labcn + count);
	 count++;
      }
   for (i = 0; i < paName.count; ++i)
      {
	 VAR(char, name, paName.items[i]);

	 if (!use_asm)
	    fprintf(out, "\t\"%s\",\n", name);
	 else
	    fprintf(out, "\t.long .LC%d\n", labcn + count);
	 count++;
      }
   if (!use_asm)
      {
	 fprintf(out, "\t0\n");
	 fprintf(out, "};\n\n");
      }
   else
      {
	 fprintf(out, "\t.long 0\n");
	 if (count)
	    {
	       fprintf(out, ".section\t.rodata\n");
	       count = 0;
	       for (i = 0; i < poName.count; ++i)
		  {
		     VAR(char, name, poName.items[i]);

		     fprintf(out, ".LC%d:\n\t.string \"%s\"\n", labcn + count, name);
		     count++;
		  }
	       for (i = 0; i < paName.count; ++i)
		  {
		     VAR(char, name, paName.items[i]);

		     fprintf(out, ".LC%d:\n\t.string \"%s\"\n", labcn + count, name);
		     count++;
		  }
	       fprintf(out, ".data\n");
	       labcn += count;
	    }
      }

   if (!use_asm)
      {
	 for (i = 0; i < fnames.count; ++i)
	    {
	       VAR(char, s, fnames.items[i]);

	       fprintf(out, "extern ClipFile %s;\n", s);
	    }
      }

   if (!use_asm)
      {
	 fprintf(out, "\nstatic struct ClipFile *_cpfiles[]=\n");
	 fprintf(out, "{\n");
      }
   else
      {
	 fprintf(out, "\t.align 4\n");
/*		fprintf(out, "\t.type\t_cpfiles,@object\n");*/
	 fprintf(out, US "_cpfiles:\n");
      }

   for (i = 0; i < fnames.count; ++i)
      {
	 VAR(char, s, fnames.items[i]);

	 if (!use_asm)
	    fprintf(out, "\t&%s,\n", s);
	 else
	    fprintf(out, "\t.long %s%s\n", US, s);
      }
   if (!use_asm)
      {
	 fprintf(out, "\t0\n");
	 fprintf(out, "};\n\n");
      }
   else
      {
	 fprintf(out, "\t.long 0\n");
/*		fprintf(out, "\t.size _cpfiles,%d\n", (fnames.count + 1) * sizeof(ClipFile *));*/
      }

   if (!use_asm && shared)
      {
	 for (i = 0; i < nlibs.count; ++i)
	    {
	       VAR(char, s, nlibs.items[i]);

	       fprintf(out, "CLIP_DLLIMPORT extern ClipFile **_libcpfiles_%s;\n", s);
	    }
      }
   if (!use_asm)
      fprintf(out, "\nstatic ClipFile ***_libcpfiles[]=\n{\n");
   else
      {
	 fprintf(out, "\t.align 4\n");
/*		fprintf(out, "\t.type\t_libcpfiles,@object\n");*/
	 fprintf(out, US "_libcpfiles:\n");
      }
   if (shared)
      {
	 for (i = 0; i < nlibs.count; ++i)
	    {
	       if (!use_asm)
		  fprintf(out, "\t0,\n");
	       else
		  fprintf(out, "\t.long 0\n");
	    }
      }
   if (!use_asm)
      {
	 fprintf(out, "\t0,\n");
	 fprintf(out, "};\n\n");
      }
   else
      {
	 fprintf(out, "\t.long 0\n");
/*		fprintf(out, "\t.size _cpfiles,%d\n", ((shared ? nlibs.count : 0) + 1) * sizeof(ClipFile *));*/
      }

   if (!use_asm)
      {
	 for (i = 0; i < dnames.count; ++i)
	    {
	       VAR(char, s, dnames.items[i]);

	       fprintf(out, "extern struct DBFuncTable %s;\n", s);
	    }
      }

   if (!use_asm)
      {
	 fprintf(out, "\nstatic struct DBFuncTable *_dbdrivers[]=\n");
	 fprintf(out, "{\n");
      }
   else
      {
	 fprintf(out, "\t.align 4\n");
/*		fprintf(out, "\t.type\t_dbdrivers,@object\n");*/
	 fprintf(out, US "_dbdrivers:\n");
      }

   for (i = 0; i < dnames.count; ++i)
      {
	 VAR(char, s, dnames.items[i]);

	 if (!use_asm)
	    fprintf(out, "\t&%s,\n", s);
	 else
	    fprintf(out, "\t.long %s\n", s);
      }

   if (!use_asm)
      {
	 fprintf(out, "\t0\n");
	 fprintf(out, "};\n\n");
      }
   else
      {
	 fprintf(out, "\t.long 0\n");
/*		fprintf(out, "\t.size _dbdrivers,%d\n", (dnames.count + 1) * sizeof(DBFuncTable *));*/
      }

   if (!use_asm)
      {
	 for (i = 0; i < rnames.count; ++i)
	    {
	       VAR(char, s, rnames.items[i]);

	       fprintf(out, "extern struct ClipObjRtti %s;\n", s);
	    }

	 fprintf(out, "\nstatic struct ClipObjRtti *_objrtti[]=\n");
	 fprintf(out, "{\n");
      }
   else
      {
	 fprintf(out, "\t.align 4\n");
/*		fprintf(out, "\t.type\t_objrtti,@object\n");*/
	 fprintf(out, US "_objrtti:\n");
      }

   for (i = 0; i < rnames.count; ++i)
      {
	 VAR(char, s, rnames.items[i]);

	 if (!use_asm)
	    fprintf(out, "\t&%s,\n", s);
	 else
	    fprintf(out, "\t.long %s\n", s);
      }
   if (!use_asm)
      {
	 fprintf(out, "\t0\n");
	 fprintf(out, "};\n\n");
      }
   else
      {
	 fprintf(out, "\t.long 0\n");
/*		fprintf(out, "\t.size _objrtti,%d\n", (rnames.count + 1) * sizeof(ClipObjRtti *));*/
      }

   if (!use_asm)
      {
	 fprintf(out, "static ClipInitStruct _init_struct =\n{\n");
	 fprintf(out, "\t_builtins,\n");
	 fprintf(out, "\t_inits,\n\t_libinits,\n\t_exits,\n\t_libexits,\n\t_pfunctions,\n");
	 fprintf(out, "\t_cpfiles,\n\t_libcpfiles,\n\t_objrtti,\n\t(void**)_dbdrivers,\n");
	 fprintf(out, "\t\"%s\"\n", targetCharset);
	 fprintf(out, "};\n\n");
	 fprintf(out, "%svoid _clip_init_struct(ClipInitStruct *sp);\n", shared ? "CLIP_DLLIMPORT " : "");
	 fprintf(out, "void\n_clip_init_dll(void)\n{\n");
	 if (shared)
	    {
	       for (i = 0; i < nlibs.count; ++i)
		  {
		     VAR(char, s, nlibs.items[i]);

		     fprintf(out, "\t_libinits[%d] = &_libinits_%s,\n", i, s);
		     fprintf(out, "\t_libexits[%d] = &_libexits_%s,\n", i, s);
		     fprintf(out, "\t_libcpfiles[%d] = &_libcpfiles_%s,\n", i, s);
		  }
	    }
	 fprintf(out, "\t_clip_init_struct(&_init_struct);\n");
	 fprintf(out, "}\n\n");
      }
   else
      {
/*		fprintf(out, "\t.type\t_init_struct,@object\n");*/
/*		fprintf(out, "\t.size\t_init_struct,%d\n", sizeof(ClipInitStruct));*/
	 fprintf(out, US "_init_struct:\n");

	 fprintf(out, "\t.long %s_builtins\n", US);
	 fprintf(out, "\t.long %s_inits\n", US);
	 fprintf(out, "\t.long %s_libinits\n", US);
	 fprintf(out, "\t.long %s_exits\n", US);
	 fprintf(out, "\t.long %s_libexits\n", US);
	 fprintf(out, "\t.long %s_pfunctions\n", US);
	 fprintf(out, "\t.long %s_cpfiles\n", US);
	 fprintf(out, "\t.long %s_libcpfiles\n", US);
	 fprintf(out, "\t.long %s_objrtti\n", US);
	 fprintf(out, "\t.long %s_dbdrivers\n", US);
	 fprintf(out, "\t.long .LC%d\n", labcn);

	 fprintf(out, ".section\t.rodata\n");
	 fprintf(out, ".LC%d:\n\t.string \"%s\"\n", labcn, targetCharset);
	 labcn++;

	 fprintf(out, ".text\n");
	 fprintf(out, "\t.align 4\n");
	 fprintf(out, ".globl %s_clip_init_dll\n", US);
/*		fprintf(out, "\t.type _clip_init_dll,@function\n");*/
	 fprintf(out, US "_clip_init_dll:\n");

	 fprintf(out, "\tpushl %%ebp\n");
	 fprintf(out, "\tmovl %%esp,%%ebp\n");
#ifdef _WIN32
	 fprintf(out, "\tsubl $20,%%esp\n");
	 fprintf(out, "\tpushl %%ebx\n");
#else
	 fprintf(out, "\tsubl $8,%%esp\n");
#endif
	 if (shared)
	    {
	       for (i = 0; i < nlibs.count; ++i)
		  {
		     VAR(char, s, nlibs.items[i]);

#ifdef _WIN32
		     fprintf(out, "\tmovl %s%s_libinits_%s,%%eax\n", IMP, US, s);
		     fprintf(out, "\tmovl %%eax,%s_libinits+%d\n", US, i * sizeof(ClipFunction ***));

		     fprintf(out, "\tmovl %s%s_libexits_%s,%%eax\n", IMP, US, s);
		     fprintf(out, "\tmovl %%eax,%s_libexits+%d\n", US, i * sizeof(ClipFunction ***));

		     fprintf(out, "\tmovl %s%s_libcpfiles_%s,%%eax\n", IMP, US, s);
		     fprintf(out, "\tmovl %%eax,%s_libcpfiles+%d\n", US, i * sizeof(ClipFile ***));
#else
		     fprintf(out, "\tmovl $%s%s_libinits_%s,%s_libinits+%ld\n", IMP, US, s, US, (long) (i * sizeof(ClipFunction ***)));
		     fprintf(out, "\tmovl $%s%s_libexits_%s,%s_libexits+%ld\n", IMP, US, s, US, (long) (i * sizeof(ClipFunction ***)));
		     fprintf(out, "\tmovl $%s%s_libcpfiles_%s,%s_libcpfiles+%ld\n", IMP, US, s, US, (long) (i * sizeof(ClipFile ***)));
#endif
		  }
	    }
	 fprintf(out, "\taddl $-12,%%esp\n");
	 fprintf(out, "\tpushl $%s_init_struct\n", US);
	 fprintf(out, "\tcall %s_clip_init_struct\n", US);
	 fprintf(out, "\taddl $16,%%esp\n");
#ifdef _WIN32
	 fprintf(out, "\tmovl -24(%%ebp),%%ebx\n");
	 fprintf(out, "\tmovl %%ebp,%%esp\n");
	 fprintf(out, "\tpopl %%ebp\n");
#else
	 fprintf(out, "\tleave\n");
#endif
	 fprintf(out, "\tret\n");
	 fprintf(out, ".L%d:\n", labn);
/*		fprintf(out, "\t.size	 _clip_init_dll,.L%d-_clip_init_dll\n", labn);*/
	 labn++;
      }

   if ((pcode_flag || pc_flag || !main_flag) && !wrote_main)
      {

	 if (!use_asm)
	    {
	       fprintf(out, "\n\
\n\
extern char **environ;\n\
\n\
int\n\
main(int argc, char **argv)\n\
{\n\
	ClipMachine *mp;\n\
	_clip_init_dll();\n\
	Task_INIT();\n\
\n\
	mp = new_ClipMachine(0);\n\
	return _clip_main(mp, %ld, argc, argv, environ);\n\
}\n\
", (long) hashstr("MAIN"));

	    }
	 else
	    {
	       fprintf(out, "\t.align 4\n");
	       fprintf(out, ".globl %smain\n", US);
/*			fprintf(out, "\t.type	 main,@function\n");*/
	       fprintf(out, US "main:\n");
	       fprintf(out, "\tpushl %%ebp\n");
	       fprintf(out, "\tmovl %%esp,%%ebp\n");
	       fprintf(out, "\tsubl $24,%%esp\n");
#ifdef _WIN32
	       fprintf(out, "\tcall ___main\n");
#endif
	       fprintf(out, "\tcall %s_clip_init_dll\n", US);
	       fprintf(out, "\taddl $-12,%%esp\n");
	       fprintf(out, "\tpushl $0\n");
	       fprintf(out, "\tcall %snew_ClipMachine\n", US);
	       fprintf(out, "\taddl $16,%%esp\n");
	       fprintf(out, "\tmovl %%eax,-4(%%ebp)\n");
	       fprintf(out, "\taddl $-12,%%esp\n");
	       fprintf(out, "\tmovl %senviron,%%eax\n", US);
	       fprintf(out, "\tpushl %%eax\n");
	       fprintf(out, "\tmovl 12(%%ebp),%%eax\n");
	       fprintf(out, "\tpushl %%eax\n");
	       fprintf(out, "\tmovl 8(%%ebp),%%eax\n");
	       fprintf(out, "\tpushl %%eax\n");
	       fprintf(out, "\tpushl $%ld\n", hashstr("MAIN"));
	       fprintf(out, "\tmovl -4(%%ebp),%%eax\n");
	       fprintf(out, "\tpushl %%eax\n");
	       fprintf(out, "\tcall %s_clip_main\n", US);
	       fprintf(out, "\taddl $32,%%esp\n");
	       fprintf(out, "\tmovl %%eax,%%edx\n");
	       fprintf(out, "\tmovl %%edx,%%eax\n");
	       fprintf(out, "\tjmp .L%d\n", labn);
/*			fprintf(out, "\t.p2align 4,,7\n");*/
	       fprintf(out, "\t.align 4\n");
	       fprintf(out, ".L%d:\n", labn);
	       labn++;
#ifdef _WIN32
	       fprintf(out, "\tmovl %%ebp,%%esp\n");
	       fprintf(out, "\tpopl %%ebp\n");
#else
	       fprintf(out, "\tleave\n");
#endif
	       fprintf(out, "\tret\n");
	       fprintf(out, ".L%d:\n", labn);
/*			fprintf(out, "\t.size	 main,.L%d-main\n", labn);*/

	    }
      }

 end:
   if (in)
      pclose(in);
   if (out)
      fclose(out);
   destroy_Coll(&names);
   destroy_Coll(&fnames);
   destroy_Coll(&dnames);
   destroy_Coll(&objs);
   destroy_Coll(&libs);
   destroy_Coll(&nlibs);
   destroy_Coll(&slibs);
   destroy_Coll(&alibs);
}
