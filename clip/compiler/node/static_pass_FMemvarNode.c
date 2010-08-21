static int
pass_FMemvarNode(void *self, Pass pass, int level, void *par)
{
   VAR(VarNode, np, self);
   switch (pass)
   {
   case Print:
      fprintfOffs(stdout, level, "FMemvar node: %s (%d) isArg=%d firstMemvar=%d\n", np->vp->name, np->vp->no, np->isArg, np->p1);
      break;
   case PrintSrc:
      {
	 VAR(CodestrNode, cp, par);
	 VAR(StrBuf, out, cp->buf);
	 putString_StrBuf(out, np->vp->name);
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
	 if (np->p1)
	    fprintfOffs(out, level, "if ( (_ret=_clip_memvarf( _mp, %ld /* %s */ ))) goto _trap_%d;\n", (long) hashstr(np->vp->name), np->vp->name, np->node.seqNo);
	 else
	    fprintfOffs(out, level, "if ( (_ret=_clip_fmemvar( _mp, %ld /* %s */ ))) goto _trap_%d;\n", (long) hashstr(np->vp->name), np->vp->name, np->node.seqNo);
      }
      break;
   case CTextLval:
      {
	 VAR(FILE, out, par);
	 fprintf(out, "_clip_ref_memvar( _mp, %ld /* %s */ )", (long) hashstr(np->vp->name), np->vp->name);
      }
      break;
   case CTextRef:
      {
	 VAR(FILE, out, par);
	 if (np->isArg)
	 {
	    fprintfOffs(out, level, "if ((_ret=_clip_ref( _mp, ");
	    fprintf(out, "_clip_ref_memvar_noadd( _mp, %ld /* %s */ )", (long) hashstr(np->vp->name), np->vp->name);
	    fprintf(out, ", 0 ))) goto _trap_%d;\n", np->node.seqNo);
	 }
	 else
	 {
	    fprintfOffs(out, level, "if ( (_ret=_clip_ref_fmemvar( _mp, %ld /* %s */ ))) goto _trap_%d;\n", (long) hashstr(np->vp->name), np->vp->name, np->node.seqNo);
	 }
      }
      break;
   case OText:
      {
	 VAR(StrBuf, out, par);
	 if (np->p1)
	    putByte_StrBuf(out, CLIP_PUSH_MEMVARF);
	 else
	    putByte_StrBuf(out, CLIP_PUSH_FMEMVAR);
	 putLong_StrBuf(out, hashstr(np->vp->name));
      }
      break;
   case OTextLval:
      {
	 VAR(StrBuf, out, par);
	 putByte_StrBuf(out, CLIP_PUSH_REF_MEMVAR);
	 putLong_StrBuf(out, hashstr(np->vp->name));
      }
      break;
   case OTextRef:
      {
	 VAR(StrBuf, out, par);
	 if (np->isArg)
	 {
	    putByte_StrBuf(out, CLIP_PUSH_REF_MEMVAR_NOADD);
	    putLong_StrBuf(out, hashstr(np->vp->name));
	    putByte_StrBuf(out, CLIP_MAKE_REF);
	    putByte_StrBuf(out, 0);
	 }
	 else
	 {
	    putByte_StrBuf(out, CLIP_REF_FMEMVAR);
	    putLong_StrBuf(out, hashstr(np->vp->name));
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
