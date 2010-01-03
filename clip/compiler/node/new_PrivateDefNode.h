Node     *
new_PrivateDefNode(VarColl * cp)
{
   int       i;

   NEWVAR(DefNode, ret);

   init_Node(ret, pass_PrivateDefNode, "privatedef");

   for (i = 0; i < cp->coll.count; i++)
    {
       int       no;

       VAR(Var, vp, cp->coll.items[i]);
       Node     *vap, *np = 0;

       if (!vp->macro)
	{
	   ins_name(vp->name);
	   if (!undeclaredName(vp->name, 1))
			rm_VarColl(curFunction->VarColl_memvars_of_Function, vp);
		if (search_VarColl(curFunction->VarColl_privates_of_Function, vp->name, &no))
			;			/* vp = (Var*) curFunction->VarColl_privates->coll.items[no]; */
	   else
	    {
#if 0
add_VarColl(curFunction->VarColl_privates_of_Function, vp);
prepend_Node(curFunction->body_of_Function, new_CreateVarNode(vp, 0, strdup("private")));
#else
	       append_Node(&ret->node, new_CreateVarNode(vp, 0, strdup("private")));
#endif
	    }

	   vap = new_PrivateNode(vp);
	}
       else
	{
	   vap = vp->macro;
	   append_Node(&ret->node, new_CreateVarNode(vp, 0, strdup("private")));
		/*append_Node(curFunction->body_of_Function, new_CreateVarNode(vp, 0, strdup("private"))); */
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
