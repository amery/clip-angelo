static int
pass_StaticNode(void *self, Pass pass, int level, void *par)
{
   VAR(VarNode, np, self);
   switch (pass)
   {
   case Print:
      fprintfOffs(stdout, level, "Static node: %s %s (%d)\n", np->vp->func->name_of_Function, np->vp->name, np->vp->no);
      break;
   case PrintSrc:
      {
	 VAR(CodestrNode, cp, par);
	 flush_codestr(cp, new_QuotNode(&np->node));
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
	 fprintfOffs(out, level, "if ((_ret=_clip_push_static( _mp, &s_%s_%s))) goto _trap_%d;\n", np->vp->func->name_of_Function, np->vp->name, np->node.seqNo);
      }
      break;
   case CTextLval:
      {
	 VAR(FILE, out, par);
	 fprintf(out, "&s_%s_%s", np->vp->func->name_of_Function, np->vp->name);
      }
      break;
   case OText:
      {
	 VAR(StrBuf, out, par);
	 putByte_StrBuf(out, CLIP_PUSH_STATIC);
	 putShort_StrBuf(out, np->vp->no);
      }
      break;
   case OTextLval:
      {
	 VAR(StrBuf, out, par);
	 putByte_StrBuf(out, CLIP_PUSH_REF_STATIC);
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
