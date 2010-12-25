Node *
new_MemvarDefNode(VarColl * cp)
{
   int i;

   if (curFunction->lastExec_of_Function)
      yyerror("MEMVAR after executable statements");

	for (i = 0; i < cp->coll.count_of_Coll; i++)
   {
		VAR(Var, vp, cp->coll.items_of_Coll[i]);
      if (vp->macro)
      {
	 yyerror("macro variable name not allowed here");
	 continue;
      }
      ins_name(vp->name);
      if (undeclaredName(vp->name, 1))
	 add_VarColl(curFunction->VarColl_memvars_of_Function, vp);
   }

   return (Node *) NULL;
}
