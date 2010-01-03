static int
pass_NewArrNode(void *self, Pass pass, int level, void *par)
{
   VAR(ExprNode, np, self);
   switch (pass)
    {
    case Print:
       fprintfOffs(stdout, level, "NewArrNode (%d):\n", np->i);
       break;
    case CalcDeep:
       break;
    case CText:
       {
	  VAR(FILE, out, par);
	  np->expr->pass(np->expr, CText, level, par);
	  fprintfOffs(out, level, "_clip_dimarray( _mp, %d );\n", np->i);
       }
       return 0;
    case OText:
       {
	  VAR(StrBuf, out, par);
	  np->expr->pass(np->expr, OText, level, par);
	  putByte_StrBuf(out, CLIP_DIMARRAY);
	  putLong_StrBuf(out, np->i);
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
   pass_Node(np->expr, pass, level, par);
   return 0;
}
