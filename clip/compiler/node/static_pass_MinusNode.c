static int
pass_MinusNode(void *self, Pass pass, int level, void *par)
{
   VAR(ExprNode, np, self);
   switch (pass)
   {
   case Print:
      fprintfOffs(stdout, level, "MinusNode :\n");
      break;
   case PrintSrc:
      {
	 VAR(CodestrNode, cp, par);
	 VAR(StrBuf, out, cp->buf);
	 putString_StrBuf(out, "(-");
	 np->expr->pass(np->expr, pass, level, par);
	 putString_StrBuf(out, ")");
      }
      return 0;
   case CalcDeep:
      break;
   case CText:
      {
	 VAR(FILE, out, par);
	 np->expr->pass(np->expr, pass, level, par);
	 fprintfOffs(out, level, "if ((_ret=_clip_minus( _mp ))) goto _trap_%d;\n", np->node.seqNo);
      }
      return 0;
   case OText:
      {
	 VAR(StrBuf, out, par);
	 np->expr->pass(np->expr, pass, level, par);
	 putByte_StrBuf(out, CLIP_MINUS);
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
