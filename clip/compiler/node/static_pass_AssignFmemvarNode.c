static int
pass_AssignFmemvarNode(void *self, Pass pass, int level, void *par)
{
   VAR(AssignNode, np, self);
   switch (pass)
   {
   case Print:
      fprintfOffs(stdout, level, "AssignFmemvarNode:\n");
      break;
   case PrintSrc:
      {
	 VAR(CodestrNode, cp, par);
	 VAR(StrBuf, out, cp->buf);
	 putString_StrBuf(out, ((VarNode *) np->var)->vp->name);
	 putString_StrBuf(out, ":=");
	 np->expr->pass(np->expr, pass, level, par);
      }
      return 0;
   case CalcDeep:
      break;
   case CText:
      {
	 VAR(FILE, out, par);
	 VAR(VarNode, vp, np->var);

	 np->expr->pass(np->expr, pass, level, par);
	 if (np->node.isTop)
	 {
	    fprintfOffs(out, level, "if ((_ret=_clip_fm_assign( _mp, " "%ld /* %s */  ))) goto _trap_%d;\n", (long) hashstr(vp->vp->name), vp->vp->name, np->node.seqNo);
	 }
	 else
	 {
	    fprintfOffs(out, level, "if ((_ret=_clip_fm_iassign( _mp, " "%ld /* %s */  ))) goto _trap_%d;\n", (long) hashstr(vp->vp->name), vp->vp->name, np->node.seqNo);
	 }
      }
      return 0;
   case OText:
      {
	 VAR(StrBuf, out, par);
	 VAR(VarNode, vp, np->var);

	 np->expr->pass(np->expr, pass, level, par);
	 if (np->node.isTop)
	 {
	    putByte_StrBuf(out, CLIP_FM_ASSIGN);
	    putLong_StrBuf(out, hashstr(vp->vp->name));
	 }
	 else
	 {
	    putByte_StrBuf(out, CLIP_FM_IASSIGN);
	    putLong_StrBuf(out, hashstr(vp->vp->name));
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
   np->var->pass(np->var, pass, level + 1, par);
   np->expr->pass(np->expr, pass, level + 1, par);
   return 0;
}
