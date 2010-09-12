static int
pass_IncrNode(void *self, Pass pass, int level, void *par)
{
   VAR(IncrNode, np, self);
   switch (pass)
   {
   case Print:
      fprintfOffs(stdout, level, "IncrNode: isIncr=%d isPost=%d:\n", np->isIncr, np->isPost);
      break;
   case CalcDeep:
      {
	 VAR(Function, fp, par);
	 addDeep(fp, 1);
      }
      break;
   case PrintSrc:
      {
	 VAR(CodestrNode, cp, par);
	 VAR(StrBuf, out, cp->buf);
	 putString_StrBuf(out, "(");
	 if (!np->isPost)
	    putString_StrBuf(out, np->isIncr ? "++" : "--");
	 np->expr->pass(np->expr, pass, level, par);
	 if (np->isPost)
	    putString_StrBuf(out, np->isIncr ? "++" : "--");
	 putString_StrBuf(out, ")");
      }
      return 0;
   case CText:
      {
	 VAR(FILE, out, par);
	 if (np->expr->isLval)
	 {
	    fprintfOffs(out, level, "if ( (_ret=_clip_incr( _mp, ");
	    np->expr->pass(np->expr, CTextLval, level, par);
	    fprintf(out, ", %d, %d ))) goto _trap_%d;\n", np->isIncr, np->isPost, np->node.seqNo);
	 }
	 else if (np->expr->isArrEl)
	 {
	    np->expr->pass(np->expr, CText, level, par);
	 }
      }
      return 0;
   case OText:
      {
	 VAR(StrBuf, out, par);
	 if (np->expr->isLval)
	 {
	    np->expr->pass(np->expr, OTextLval, level, par);
	    putByte_StrBuf(out, np->isIncr ? (np->isPost ? CLIP_INCR_POST : CLIP_INCR) : (np->isPost ? CLIP_DECR_POST : CLIP_DECR));
	 }
	 else if (np->expr->isArrEl)
	 {
	    np->expr->pass(np->expr, OText, level, par);
	 }
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
