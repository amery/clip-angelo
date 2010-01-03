static int
pass_ParametersDefNode(void *self, Pass pass, int level, void *par)
{
   VAR(DefNode, np, self);
   switch (pass)
    {
    case Print:
       {
	  int       i, count = np->cp->unsorted.count;

	  fprintfOffs(stdout, level, "parameters def node: %d vars\n", count);
	  for (i = 0; i < count; i++)
	   {
	      VAR(Var, vp, np->cp->unsorted.items[i]);
	      print_Var(vp, level + 1);
	   }
       }
       break;
    case CText:
       {
	  VAR(FILE, out, par);
	  int       i, count = np->cp->unsorted.count;

	  print_line(self, level, out);
	  for (i = count - 1; i >= 0; --i)
	   {
	      VAR(Var, vp, np->cp->unsorted.items[i]);
	      fprintfOffs(out, level,
			  "_clip_memvar_param( _mp, %ld /* %s */, %d );\n", (long) hashstr(vp->name), vp->name, vp->no);
	   }
       }
       break;
    case OText:
       {
	  VAR(StrBuf, out, par);
	  int       i, count = np->cp->unsorted.count;

	  put_line(self, level, out);
	  for (i = count - 1; i >= 0; --i)
	   {
	      VAR(Var, vp, np->cp->unsorted.items[i]);
	      putByte_StrBuf(out, CLIP_MEMVAR_PARAM);
	      putLong_StrBuf(out, hashstr(vp->name));
	      putByte_StrBuf(out, vp->no);
	   }
       }
       break;
    case Traverse:
       {
	  VAR(TraversePar, tp, par);
	  tp->func((Node *) self, tp->par);
       }
       break;
    default:
       break;
    }

   return 0;
}
