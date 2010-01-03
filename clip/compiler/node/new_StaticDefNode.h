Node     *
new_StaticDefNode(VarColl * cp)
{
   int       i;

	if (curFunction->lastExec_of_Function)
      yyerror("STATIC after executable statements");

   for (i = 0; i < cp->coll.count; i++)
    {
       VAR(Var, vp, cp->coll.items[i]);
       if (vp->macro)
	{
	   yyerror("macro variable name not allowed here");
	   continue;
	}
       ins_name(vp->name);
       if (undeclaredName(vp->name, 0))
			 add_VarColl(curFunction->VarColl_statics_of_Function, vp);
       if (vp->init && !vp->init->isConst && !vp->init->isCode)
	{
	   int       is_const = 0;

	   vp->init->pass(vp->init, TestConst, 0, &is_const);
#if 0
	   if (!is_const)
	      yyerror("only constant expression allowed for static variable initialisation");
#endif
	}
       if (vp->init)
	{
	   vp->init->isInit = 1;
		curFunction->goto_trap_of_Function++;
	}
       vp->no = curFile->staticNo_of_File;
       ++curFile->staticNo_of_File;
    }

   return (Node *) NULL;
}
