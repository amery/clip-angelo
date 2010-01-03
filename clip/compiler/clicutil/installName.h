Node     *
installName(char *name)
{
   int       no, level = 0;

   Var      *vp;

   Function *fp;

	if (curFunction->VarColl_code_params_of_Function && search_VarColl(curFunction->VarColl_code_params_of_Function, name, &no))
    {
		 VAR(Var, vp, curFunction->VarColl_code_params_of_Function->coll.items[no]);
       Node     *rp;

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
		  add_VarColl(curFunction->VarColl_fmemvars_of_Function, vp);
     return new_FMemvarNode(vp);
     }
   */
}
