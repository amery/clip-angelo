void
fin_Function(Function * fpp)
{
   int       i;

	for (i = 0; i < fpp->VarColl_params_of_Function->unsorted.count; i++)
    {
       Function *fp;

		 VAR(Var, vp, fpp->VarColl_params_of_Function->unsorted.items[i]);
       int       level;

       if (vp->macro)
	{
	   yyerror("macro variable name not allowed here");
	   continue;
	}

for (level = 0, fp = fpp; fp->Function_upfunc_of_Function; fp = fp->Function_upfunc_of_Function, level++)
	  ;
if (!fpp->VarColl_locals_of_Function->unsorted.count && fp != fpp /*&& fp->isPublic_of_Function */ )
	{
	   char      buf[128], *nm;

	   Var      *nv;

	   vp->pno = i;
		sprintf(buf, "___param_%d", fp->VarColl_locals_of_Function->unsorted.count + 1);
	   nm = strdup(buf);
	   nv = new_Var(nm);
	   nv->func = fp;
	   nv->pno = vp->pno;
		add_VarColl(fp->VarColl_locals_of_Function, nv);
	   vp->no = nv->no;
	   vp->level = level;
	   vp->isParam = 1;
	   nv->refvar = vp;
	   nv->isParam = 1;
	   nv->isRef = 1;
	   vp->isRef = 0;
		fp->reflocals_of_Function++;
	}
       else
	{
	   vp->isParam = 1;
	   vp->pno = i;
	   vp->func = fpp;
		add_VarColl(fpp->VarColl_locals_of_Function, vp);
	}

    }
}
