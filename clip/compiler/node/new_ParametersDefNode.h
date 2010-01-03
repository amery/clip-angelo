Node     *
new_ParametersDefNode(VarColl * cp)
{
   int       i;

   NEWVAR(DefNode, ret);

	if (curFunction->VarColl_params_of_Function->coll.count)
      yyerror("PARAMETERS and '(' <paramlist> ')' are incompatible");

   init_Node(ret, pass_ParametersDefNode, "parametersdef");

   ret->cp = cp;
   ret->node.isExec = 0;

   for (i = 0; i < cp->unsorted.count; i++)
    {
       VAR(Var, vp, cp->unsorted.items[i]);
       if (vp->macro)
	{
	   yyerror("macro variable name not allowed here");
	   continue;
	}
       if (vp->init)
	{
	   yyerror("init expression not allowed here");
	   continue;
	}
       if (vp->arr)
	{
	   yyerror("array declaration not allowed here");
	   continue;
	}
       ins_name(vp->name);
#if 0
       if (undeclaredName(vp->name))
	  if (!undeclaredName(vp->name))
	   {
	      rm_VarColl(curFunction->memvars, vp);
	   }
#endif
add_VarColl(curFunction->VarColl_parameters_of_Function, vp);
#if 0
       vp->isParameter = 1;
       add_VarColl(curFunction->plocals, vp);
       vp->init = (Node *) ret;
#endif
    }

#if 1
   return (Node *) ret;
#else
   return 0;
#endif
}
