int
undeclaredName(const char *sp, int memvar)
{
   int       no;

	if (search_VarColl(curFunction->VarColl_locals_of_Function, (char *) sp, &no))
    {
		 VAR(Var, vp, curFunction->VarColl_locals_of_Function->coll.items[no]);
       yyerror
	("name '%s' already declared as LOCAL near line %d pos %d file '%s'", vp->name, vp->line, vp->pos, fileName(vp->file));
       return 0;
    }
	 else if (search_VarColl(curFunction->VarColl_params_of_Function, (char *) sp, &no))
    {
		 VAR(Var, vp, curFunction->VarColl_params_of_Function->coll.items[no]);
       yyerror
	("name '%s' already declared as PARAMETER near line %d pos %d file '%s'",
	 vp->name, vp->line, vp->pos, fileName(vp->file));
       return 0;
    }
	 else if (!memvar && search_VarColl(curFunction->VarColl_memvars_of_Function, (char *) sp, &no))
    {
		 VAR(Var, vp, curFunction->VarColl_memvars_of_Function->coll.items[no]);
       yywarning
	("name '%s' already declared as MEMVAR near line %d pos %d file '%s' (previous name will be hidden)",
	 vp->name, vp->line, vp->pos, fileName(vp->file));
       return 0;
    }
	 else if (search_VarColl(curFunction->VarColl_fields_of_Function, (char *) sp, &no))
    {
		 VAR(Var, vp, curFunction->VarColl_fields_of_Function->coll.items[no]);
       yyerror
	("name '%s' already declared as FIELD near line %d pos %d file '%s'", vp->name, vp->line, vp->pos, fileName(vp->file));
       return 0;
    }
	 else if (search_VarColl(curFunction->VarColl_statics_of_Function, (char *) sp, &no))
    {
		 VAR(Var, vp, curFunction->VarColl_statics_of_Function->coll.items[no]);
       yyerror
	("name '%s' already declared as STATIC near line %d pos %d file '%s'", vp->name, vp->line, vp->pos, fileName(vp->file));
       return 0;
    }
   return 1;
}
