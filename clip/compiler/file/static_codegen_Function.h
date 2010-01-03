static void
codegen_Function(Function * fp)
{
   int       i, j;

	fp->seqNo_of_Function = 0;
   curFunction = fp;
	fp->nlocals_of_Function = 0;
	fp->reflocals_of_Function = 0;

	pass_Node(fp->Node_body_of_Function, Codegen2, 0, fp);
	pass_Node(fp->Node_body_of_Function, Codegen1, 0, fp);

#if 0
   for (j = fp->locals->unsorted.count - 1; j >= 0; --j)
    {
       VAR(Var, vp, fp->locals->unsorted.items[j]);

       Node     *ip = 0;

       if (vp->init && (vp->isRef || pcode_flag || !vp->init->isConst || vp->init->isArray))
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
		prepend_Node(fp->Node_body, np);
	}

    }
#endif

for (j = fp->VarColl_locals_of_Function->unsorted.count - 1; j >= 0; --j)
     /*for (j = 0; j < fp->locals->unsorted.count; ++j) */
    {
		 VAR(Var, vp, fp->VarColl_locals_of_Function->unsorted.items[j]);
       if (!vp->isRef)
	{
		vp->no = fp->nlocals_of_Function++;

	   if (vp->refvar)
	      vp->refvar->no = vp->no;
	}
    }

for (j = fp->VarColl_locals_of_Function->unsorted.count - 1; j >= 0; --j)
     /*for (j = 0; j < fp->locals->unsorted.count; ++j) */
    {
		 VAR(Var, vp, fp->VarColl_locals_of_Function->unsorted.items[j]);
       if (vp->isRef)
	{
	   if (vp->isParam)
	      continue;
	  /*if (fp->uplocals)
	     yyerror("reflocals and uplocals"); */
	  vp->no = fp->nlocals_of_Function + fp->reflocals_of_Function;
	  fp->reflocals_of_Function++;
	   if (vp->refvar)
	      vp->refvar->no = vp->no;
	}

    }

for (j = fp->VarColl_locals_of_Function->unsorted.count - 1; j >= 0; --j)
     /*for (j = 0; j < fp->locals->unsorted.count; ++j) */
    {
		 VAR(Var, vp, fp->VarColl_locals_of_Function->unsorted.items[j]);
       if (vp->isRef)
	{
	   if (!vp->isParam)
	      continue;
		vp->no = fp->nlocals_of_Function + fp->reflocals_of_Function;
		fp->reflocals_of_Function++;
	   if (vp->refvar)
	      vp->refvar->no = vp->no;
	}

    }

#if 0
   for (j = fp->parameters->unsorted.count - 1; j >= 0; --j)
    {
       VAR(Var, vp, fp->parameters->unsorted.items[j]);
       if (vp->isParameter && vp->init)
	{
		prepend_Node(fp->Node_body, vp->init);
	   continue;
	}

    }
#endif

for (j = fp->VarColl_statics_of_Function->unsorted.count - 1; j >= 0; --j)
    {
		 VAR(Var, vp, fp->VarColl_statics_of_Function->unsorted.items[j]);
       if (vp->init /*&& (pcode_flag || vp->init->isArray) */ )
	{
	   Node     *np = new_OperExprNode(new_AssignNode(new_StaticNode(vp), vp->init, '='));

	   np->line = vp->init->line;
		prepend_Node(fp->Node_staticInit_of_Function, np);
	}
       else if (vp->arr)
	{
	   Node     *np = new_OperExprNode(new_AssignNode(new_StaticNode(vp),
							  new_NewArrayNode(vp->arr),
							  '='));

	   np->line = vp->line;
		prepend_Node(fp->Node_staticInit_of_Function, np);
	}
    }

for (i = 0; i < fp->VarColl_params_of_Function->unsorted.count; i++)
    {
		 VAR(Var, vp, fp->VarColl_params_of_Function->unsorted.items[i]);
		 prepend_Node(fp->Node_body_of_Function, new_ParamNode(vp));
    }

if (fp->uplocals_of_Function)
    {
		 fp->nlocals_of_Function = 0;
		 fp->reflocals_of_Function = 0;
    }

pass_Node(fp->Node_body_of_Function, CalcDeep, 0, fp);

}
