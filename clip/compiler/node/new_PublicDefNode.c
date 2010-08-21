Node *
new_PublicDefNode(VarColl * cp)
{
   int i, no;

   NEWVAR(DefNode, ret);

   init_Node(ret, pass_PublicDefNode, "publicdef");

	for (i = 0; i < cp->coll.count_of_Coll; i++)
   {
		VAR(Var, vp, cp->coll.items_of_Coll[i]);
      Node *vap, *np = 0;

      if (!vp->macro)
      {
	 ins_name(vp->name);
	 if (!undeclaredName(vp->name, 1))
	    rm_VarColl(curFunction->VarColl_memvars_of_Function, vp);
	 if (search_VarColl(curFunction->VarColl_publics_of_Function, vp->name, &no))
	 {
	    /*vp = (Var *) curFunction->VarColl_publics_of_Function->coll.items[no]; */
	 }
	 else
	 {
	    add_VarColl(curFunction->VarColl_publics_of_Function, vp);
	 }

	 append_Node(&ret->node, new_CreateVarNode(vp, 1, strdup("public")));

	 vap = new_PublicNode(vp);
      }
      else
      {
	 vap = vp->macro;
	 append_Node(&ret->node, new_CreateVarNode(vp, 1, strdup("public")));
      }

      if (vp->init)
      {
	 np = new_OperExprNode(new_AssignNode(vap, vp->init, '='));
	 np->line = vp->init->line;
      }
      else if (vp->arr)
      {
	 np = new_OperExprNode(new_AssignNode(vap, new_NewArrayNode(vp->arr), '='));
	 np->line = vp->line;
      }
      else if (vp->macro)
      {
	 /*np = new_OperExprNode(vp->macro); */
      }
      if (np)
	 append_Node(&ret->node, np);
   }

   ret->cp = cp;

   return (Node *) ret;
}
