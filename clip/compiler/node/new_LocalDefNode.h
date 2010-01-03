Node     *
new_LocalDefNode(VarColl * cp, int err, int localref)
{
   int       i;

	if (curFunction->lastExec_of_Function && err)
      yyerror("LOCAL after executable statements");

   for (i = 0; i < cp->unsorted.count; i++)
    {
       VAR(Var, vp, cp->unsorted.items[i]);
       Node     *ip = 0;

       vp->isLocalRef = localref;
       if (vp->macro)
	{
	   yyerror("macro variable name not allowed here");
	   continue;
	}
       ins_name(vp->name);
       if (undeclaredName(vp->name, 0))
	{
		add_VarColl(curFunction->VarColl_locals_of_Function, vp);
	}

       if (!err)
	{
	   vp->isRef = 0;
		curFunction->uplocals_of_Function++;
	   continue;
	}

       if (vp->init && vp->init->isConst)
	  vp->init->isInit = 1;
#if 1
       if (vp->init
	  /*&& (vp->isRef || pcode_flag || !vp->init->isConst || vp->init->isArray) */
	)
	{
	   ip = vp->init;
	}
       else if (vp->arr)
	{
	   ip = new_NewArrayNode(vp->arr);
	}

       if (ip)
	{
	   Node     *lp = new_LocalNode(vp);

	   Node     *np = new_OperExprNode(new_AssignNode(lp, ip, '='));

	   np->line = vp->line;
		append_Node(curFunction->Node_body_of_Function, np);
	}
#endif
    }

   return (Node *) NULL;
}
