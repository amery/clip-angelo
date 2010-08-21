static int
pass_OpNode(void *self, Pass pass, int level, void *par)
{
   VAR(OpNode, np, self);
   switch (pass)
   {
   case Print:
      fprintfOffs(stdout, level, "OpNode : op='%c'\n", np->op);
      break;
   case PrintSrc:
      {
	 VAR(CodestrNode, cp, par);
	 VAR(StrBuf, out, cp->buf);
	 putString_StrBuf(out, "(");
	 np->expr1->pass(np->expr1, pass, level, par);
	 switch (np->op)
	 {
	 case 'e':
	    putString_StrBuf(out, "=");
	    break;
	 case 'E':
	    putString_StrBuf(out, "==");
	    break;
	 case 'N':
	    putString_StrBuf(out, "!=");
	    break;
	 case 'L':
	    putString_StrBuf(out, "<=");
	    break;
	 case 'G':
	    putString_StrBuf(out, ">=");
	    break;
	 default:
	    putByte_StrBuf(out, np->op);
	    break;
	 }
	 np->expr2->pass(np->expr2, pass, level, par);
	 putString_StrBuf(out, ")");
      }
      return 0;
   case CalcDeep:
      break;
   case CText:
      {
	 VAR(FILE, out, par);
	 np->expr1->pass(np->expr1, pass, level, par);
	 np->expr2->pass(np->expr2, pass, level, par);
	 fprintfOffs(out, level, "if ((_ret=_clip_op( _mp, '%c' ))) goto _trap_%d;\n", np->op, np->node.seqNo);
      }
      return 0;
   case OText:
      {
	 VAR(StrBuf, out, par);
	 np->expr1->pass(np->expr1, pass, level, par);
	 np->expr2->pass(np->expr2, pass, level, par);
	 putByte_StrBuf(out, CLIP_OP);
	 putByte_StrBuf(out, np->op);
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
