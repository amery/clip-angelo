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
   for (namelen = 1, j = 0; j < fp->VarColl_locals_of_Function->unsorted.count_of_Coll; j++)
   {
		VAR(Var, vp, fp->VarColl_locals_of_Function->unsorted.items_of_Coll[j]);
      namelen += strlen(vp->name) + 1;
   }

   if (!fp->nlocals_of_Function && !fp->reflocals_of_Function)
   {
      fprintf(out, "\tClipVarFrame *_localvars = _mp->fp->ClipVarFrame_localvars_of_ClipFrame;\n");
   }
   else if (fp->reflocals_of_Function)
   {
      int i, l;

		fprintf(out, "\tClipVarFrame *_localvars = (ClipVarFrame*) calloc(1, sizeof(ClipVarFrame)+%d*sizeof(ClipVar));\n", fp->VarColl_locals_of_Function->unsorted.count_of_Coll);
      fprintf(out, "\tstatic const char _localnames[] = {");
		for (j = 0; j < fp->VarColl_locals_of_Function->unsorted.count_of_Coll; j++)
      {
			VAR(Var, vp, fp->VarColl_locals_of_Function->unsorted.items_of_Coll[j]);
	 if (vp->isRef)
	    continue;
	 l = strlen(vp->name);
	 for (i = 0; i <= l; i++)
	    fprintf(out, "%d, ", vp->name[i]);
	 fprintf(out, " /* %s */ ", vp->name);
      }
            for (j = 0; j < fp->VarColl_locals_of_Function->unsorted.count_of_Coll; j++)
      {
			VAR(Var, vp, fp->VarColl_locals_of_Function->unsorted.items_of_Coll[j]);
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
		int numlocals = fp->VarColl_locals_of_Function->unsorted.count_of_Coll;

      int i, l;

      local_locals = 1;
      fprintf(out, "\tClipVar _vlocals [ %d ] = { ", numlocals);
      for (j = 0; j < numlocals - 1; j++)
	 fprintf(out, "{ {UNDEF_type_of_ClipVarType, 0,0,0,0} }, ");
      if (numlocals)
	 fprintf(out, "{ {UNDEF_type_of_ClipVarType, 0,0,0,0} } ");
      fprintf(out, "};\n");
      fprintf(out, "\tconst char _localnames[] = { ");
		for (j = 0; j < fp->VarColl_locals_of_Function->unsorted.count_of_Coll; j++)
      {
			VAR(Var, vp, fp->VarColl_locals_of_Function->unsorted.items_of_Coll[j]);
	 if (vp->isRef)
	    continue;
	 l = strlen(vp->name);
	 for (i = 0; i <= l; i++)
	    fprintf(out, "%d, ", vp->name[i]);
	 fprintf(out, " /* %s */ ", vp->name);
      }
            for (j = 0; j < fp->VarColl_locals_of_Function->unsorted.count_of_Coll; j++)
      {
			VAR(Var, vp, fp->VarColl_locals_of_Function->unsorted.items_of_Coll[j]);
	 if (!vp->isRef)
	    continue;
	 l = strlen(vp->name);
	 for (i = 0; i <= l; i++)
	    fprintf(out, "%d, ", vp->name[i]);
	 fprintf(out, " /* %s */ ", vp->name);
      }
      fprintf(out, "0 };\n");
		fprintf(out, "\tClipVarFrame _local_vars = { 0, %d, _vlocals, _localnames }, *_localvars = &_local_vars;\n", fp->VarColl_locals_of_Function->unsorted.count_of_Coll);
   }
#endif

#if 0
   for (j = 0; j < fp->VarColl_locals_of_Function->unsorted.count; j++)
   {
      VAR(Var, vp, fp->VarColl_locals_of_Function->unsorted.items[j]);
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
      if (fp->VarColl_params_of_Function->coll.count)
      {
      int i, c=fp->VarColl_params_of_Function->coll.count;

      fprintf(out, "\tClipVar _params[%d] = { ", c);
      for(i=0; i<c; ++i)
      fprintf(out, "{ UNDEF_type_of_ClipVarType, 0, 0,0,0 },");
      fprintf(out, " };\n");
      }
    */

#if 0
   {
      fprintf(out, "\tClipVarDef _locals[] =\n\t{\n");
      fprintf(out, "\t\t{ %d, 0 },\n", fp->VarColl_locals_of_Function->coll.count);
      for (j = 0; j < fp->VarColl_locals_of_Function->coll.count; j++)
      {
	 VAR(Var, vp, fp->VarColl_locals_of_Function->coll.items[j]);
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
      fprintf(out, "\tif (_localvars && _localvars->refcount_of_ClipVarFrame)\n\t\t_localvars->refcount_of_ClipVarFrame++;\n");
   }
   else if (fp->reflocals_of_Function)
   {
      fprintf(out, "\t_localvars->ClipVar_vars_of_ClipVarFrame = (ClipVar*) (_localvars + 1);\n");
      fprintf(out, "\t_localvars->refcount_of_ClipVarFrame = 1;\n");
      fprintf(out, "\t_localvars->size_of_ClipVarFrame = %d;\n", fp->nlocals_of_Function + fp->reflocals_of_Function);
      fprintf(out, "\t_localvars->names_of_ClipVarFrame = _localnames;\n");
   }

   if (file->Function_init_of_File != fp)
   {
      fprintf(out, "\t_init( _mp, &_frame );\n");
      fprintf(out, "\t_clip_log_call( _mp );\n\n");
   }
   else
   {
      fprintf(out, "\t_frame_p->ClipFrame_up_of_ClipFrame = _mp->fp;\n");
      fprintf(out, "\tif (_init_flag)\n\t{\n");
      fprintf(out, "\t\t_mp->fp = _frame_p;\n");
      fprintf(out, "\t\treturn 0;\n\t}\n");
      fprintf(out, "\telse if (_localvars && _localvars->refcount_of_ClipVarFrame)\n\t\t_localvars->refcount_of_ClipVarFrame++;\n");
      fprintf(out, "\t_frame.ClipFrame_up_of_ClipFrame = _mp->fp;\n");
      fprintf(out, "\t_mp->fp = &_frame;\n");
      fprintf(out, "\t_init_flag = 1;\n");

      /*
         for(i=0;i<file->Coll_unsortedFunctions.count;i++)
         {
         VAR(Function, fp, file->Coll_unsortedFunctions_of_File.items[i]);
         if (fp->isPublic_of_Function)
         fprintf(out, "\t_clip_register( %s, \"%s\" );\n", fp->name_of_Function, fp->name_of_Function);
         }
       */
      fprintf(out, "\n");
   }

#if 0
   if (fp->reflocals)
      fprintf(out, "\t_clip_mptr(_mp, _reflocals, %d);\n", fp->reflocals_of_Function);
#endif

   pass_Node(fp->Node_body_of_Function, CText, 0, out);

   if (file->Function_init_of_File == fp)
      file->Node_ctextStatic_of_File->pass(file->Node_ctextStatic_of_File, CText, 0, out);

   if ( /*file->Function_init_of_File !=fp && */ fp->goto_trap_of_Function)
   {
      fprintf(out, "\n\tgoto _return;\n");
      fprintf(out, "_trap_0:\n");
      /*fprintf(out, "\t_ret = 1;\n"); */
      fprintf(out, "\t_clip_trap(_mp, _file, _frame.line_of_ClipFrame);\n");
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
   if (fp->reflocals)
   {
      fprintf(out, "\t_clip_vresume(_mp, %d, _reflocals);\n", fp->reflocals_of_Function);
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
