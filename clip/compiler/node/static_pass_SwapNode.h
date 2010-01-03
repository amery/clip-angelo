static int
pass_SwapNode(void *self, Pass pass, int level, void *par)
{
   VAR(OpNode, np, self);
   switch (pass)
    {
    case Print:
       fprintfOffs(stdout, level, "SwapNode:\n");
       break;
    case CalcDeep:
       break;
    case CText:
       {
	  VAR(FILE, out, par);
	  np->expr1->pass(np->expr1, CText, level, par);
	  np->expr2->pass(np->expr2, CText, level, par);
	 /*fprintfOffs(out, level, "_clip_swap( _mp, 1 );\n"); */
	  fprintfOffs(out, level, "_clip_pop( _mp );\n");
       }
       return 0;
    case OText:
       {
	  VAR(StrBuf, out, par);
	  np->expr1->pass(np->expr1, OText, level, par);
	  np->expr2->pass(np->expr2, OText, level, par);
	 /*putByte_StrBuf(out, CLIP_SWAP);
	    putByte_StrBuf(out, 1); */
	  putByte_StrBuf(out, CLIP_POP);
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
   np->expr1->pass(np->expr1, pass, level + 1, par);
   np->expr2->pass(np->expr2, pass, level + 1, par);
   return 0;
}
