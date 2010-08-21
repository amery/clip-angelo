Node *
new_FieldDefNode(VarColl * cp, char *alias)
{
   int i;

   if (curFunction->lastExec_of_Function)
      yyerror("FIELD after executable statements");

	for (i = 0; i < cp->coll.count_of_Coll; i++)
   {
		VAR(Var, vp, cp->coll.items_of_Coll[i]);
      if (vp->macro)
      {
	 yyerror("macro variable name not allowed here");
	 continue;
      }
      ins_name(vp->name);
      if (undeclaredName(vp->name, 0))
	 add_VarColl(curFunction->VarColl_fields_of_Function, vp);
      if (alias)
	 vp->alias = strdup(alias);
   }

   free(alias);

   return (Node *) NULL;
}
