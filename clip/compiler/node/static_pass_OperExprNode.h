static int
pass_OperExprNode(void *self, Pass pass, int level, void *par)
{
   VAR(OperExprNode, np, self);
   switch (pass)
    {
    case Print:
       fprintfOffs(stdout, level, "operexpr node:\n");
       break;
    case CalcDeep:
       {
	  VAR(Function, fp, par);
	  fp->deep_of_Function = 0;
	  np->expr->isTop = 1;
       }
       break;
    case CText:
       {
	  VAR(FILE, out, par);
	  int       ret;

	  print_line(self, level, out);
	  ret = np->expr->pass(np->expr, pass, level, par);
	  if (!np->expr->isAssign)
	     fprintfOffs(out, level, "_clip_pop(_mp);\n");
	  return ret;
       }
    case OText:
       {
	  VAR(StrBuf, out, par);
	  int       ret;

	  put_line(self, level, out);
	  ret = np->expr->pass(np->expr, pass, level, par);
	  if (!np->expr->isAssign)
	     putByte_StrBuf(out, CLIP_POP);
	  return ret;
    case Traverse:
	  {
	     VAR(TraversePar, tp, par);
	     tp->func((Node *) self, tp->par);
	  }
	  break;
    default:
	  break;
       }
    }
   return np->expr->pass(np->expr, pass, level + 1, par);
}
