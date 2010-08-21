static int
pass_LocalNode(void *self, Pass pass, int level, void *par)
{
   VAR(VarNode, np, self);
   switch (pass)
   {
   case Print:
      fprintfOffs(stdout, level,
		  "Local node: %s (%d), isParam=%d(%d), isCodeParam=%d, isRef=%d, level=%d\n",
		  np->vp->name, np->vp->no, np->vp->isParam, np->vp->pno, np->vp->isCodeParam, np->vp->isRef, np->vp->level);
      break;
   case PrintSrc:
      {
	 VAR(CodestrNode, cp, par);
	 if (np->vp->isCodeParam)
	 {
	    VAR(StrBuf, out, cp->buf);
	    putString_StrBuf(out, np->vp->name);
	 }
	 else
	 {
	    flush_codestr(cp, new_QuotNode(&np->node));
	 }
      }
      return 0;
   case CalcDeep:
      {
	 VAR(Function, fp, par);
	 addDeep(fp, 1);
      }
      break;
   case CText:
      {
	 VAR(FILE, out, par);
	 fprintfOffs(out, level, "if ((_ret=_clip_push_local( _mp, %d))) goto _trap_%d;\n", np->vp->no, np->node.seqNo);
      }
      break;
   case OText:
      {
	 VAR(StrBuf, out, par);
	 putByte_StrBuf(out, CLIP_PUSH_LOCAL);
	 putShort_StrBuf(out, np->vp->no);
      }
      break;
   case CTextLval:
      {
	 VAR(FILE, out, par);
	 fprintf(out, "_clip_ref_local( _mp, %d)", np->vp->no);
      }
      break;
   case OTextLval:
      {
	 VAR(StrBuf, out, par);
	 putByte_StrBuf(out, CLIP_PUSH_REF_LOCAL);
	 putShort_StrBuf(out, np->vp->no);
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
