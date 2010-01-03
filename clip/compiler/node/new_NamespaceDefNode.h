Node     *
new_NamespaceDefNode(char *name, VarColl * cp)
{
   int       i;

   NEWVAR(DefNode, ret);
   long      space = hashstr(name);

   ins_name(name);

   init_Node(ret, pass_NamespaceDefNode, "namespacedef");

   ret->node.isExec = 0;
   for (i = 0; i < cp->coll.count; i++)
    {
       VAR(Var, vp, cp->coll.items[i]);
       vp->alias = strdup(name);
       ins_name(vp->name);
       if (undeclaredName(vp->name, 0))
			 add_VarColl(curFunction->VarColl_fields_of_Function, vp);
		 append_Node(curFunction->Node_body_of_Function, new_CreateVarNode(vp, space, name));
       if (vp->init)
	{
	   Node     *np = new_OperExprNode(new_AssignNode(new_FieldNode(vp), vp->init, '='));

	   np->line = vp->init->line;
		append_Node(curFunction->Node_body_of_Function, np);
	}
       else if (vp->arr)
	{
	   Node     *np = new_OperExprNode(new_AssignNode(new_FieldNode(vp),
							  new_NewArrayNode(vp->arr),
							  '='));

	   np->line = vp->line;
		append_Node(curFunction->Node_body_of_Function, np);
	}

    }

   ret->name = name;
   ret->cp = cp;

   return (Node *) ret;
}
