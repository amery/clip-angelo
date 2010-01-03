static int
pass_CodestrNode(void *self, Pass pass, int level, void *par)
{
   VAR(CodestrNode, np, self);
   int       i;

   switch (pass)
    {
    case Print:
       fprintfOffs(stdout, level, "CodestrNode: %d\n", np->coll.count);
       if (np->params)
	{
	   for (i = 0; i < np->params->unsorted.count; i++)
	    {
	       VAR(Var, vpp, np->params->unsorted.items[i]);
	       fprintfOffs(stdout, level + 1, "param %d: %s\n", i, vpp->name);
	    }
	}
       for (i = 0; i < np->coll.count; i++)
	{
	   VAR(Node, npp, np->coll.items[i]);
	   fprintfOffs(stdout, level, "%d:\n", i);
	   npp->pass(npp, pass, level + 1, par);
	}
       return 0;
    case CalcDeep:
       {
	  VAR(Function, fp, par);
	  addDeep(fp, 1);
       }
       break;
    case Codegen2:
       {
	  VAR(StrBuf, out, np->buf);
	  if (np->block || np->params)
	   {
	      putString_StrBuf(out, "{|");
	      if (np->params)
	       {
		  for (i = 0; i < np->params->unsorted.count; i++)
		   {
		      VAR(Var, vpp, np->params->unsorted.items[i]);
		      if (i)
			 putString_StrBuf(out, ",");
		      putString_StrBuf(out, vpp->name);
		   }
	       }
	      putString_StrBuf(out, "|");
	   }
	  np->expr->pass(np->expr, PrintSrc, level + 1, np);
	  if (np->block || np->params)
	     putString_StrBuf(out, "}");
	  flush_codestr(np, 0);
       }
       return 0;
    case CText:
       {
	  VAR(FILE, out, par);
	  for (i = 0; i < np->coll.count; i++)
	   {
	      VAR(Node, npp, np->coll.items[i]);
	      npp->pass(npp, pass, level + 1, par);
	   }
	  fprintfOffs(out, level, "_clip_catstr(_mp, %d);\n", np->coll.count);
       }
       return 0;
    case OText:
       {
	  VAR(StrBuf, out, par);
	  for (i = 0; i < np->coll.count; i++)
	   {
	      VAR(Node, npp, np->coll.items[i]);
	      npp->pass(npp, pass, level + 1, par);
	   }
	  putByte_StrBuf(out, CLIP_CATSTR);
	  putShort_StrBuf(out, np->coll.count);
       }
       return 0;
    case Traverse:
       {
	  VAR(TraversePar, tp, par);
	  tp->func((Node *) self, tp->par);
       }
       break;
    default:
       break;
    }

   for (i = 0; i < np->coll.count; i++)
    {
       VAR(Node, npp, np->coll.items[i]);
       npp->pass(npp, pass, level + 1, par);
    }

   return 0;
}
