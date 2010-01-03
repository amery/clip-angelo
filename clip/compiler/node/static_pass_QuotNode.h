static int
pass_QuotNode(void *self, Pass pass, int level, void *par)
{
   VAR(ExprNode, np, self);
   switch (pass)
    {
    case Print:
       fprintfOffs(stdout, level, "QuotNode:\n");
       break;
    case CText:
       {
	  VAR(FILE, out, par);
	  np->expr->pass(np->expr, CText, level, par);
	  fprintfOffs(out, level, "_clip_quot( _mp );\n");
       }
       return 0;
    case OText:
       {
	  VAR(StrBuf, out, par);
	  np->expr->pass(np->expr, OText, level, par);
	  putByte_StrBuf(out, CLIP_QUOT);
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
   np->expr->pass(np->expr, pass, level, par);
   return 0;
}
