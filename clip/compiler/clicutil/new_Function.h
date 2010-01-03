Function *
new_Function(char *name, VarColl * params, int isPublic, int isMain, Function * upfunc, int isCodeblock)
{
   NEWVAR(Function, ret);
   Function *sfp;

   char     *s;

   int       l;

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
