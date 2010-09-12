static int
pass_ParamNode(void *self, Pass pass, int level, void *par)
{
   VAR(VarNode, np, self);
   switch (pass)
   {
   case Print:
      fprintfOffs(stdout, level, "Param node: %s (%d, %d)\n", np->vp->name, np->vp->no, np->vp->pno);
      break;
   case CalcDeep:
      {
	 /*VAR(Function, fp, par); */
	 /*addDeep(fp,1); */
      }
      break;
   case CText:
      {
	 VAR(FILE, out, par);
	 fprintf(out, "\t_clip_param(_mp, %d, %d);\n", np->vp->pno, np->vp->no);
      }
      break;

   case OText:
      {
	 VAR(StrBuf, out, par);
	 putByte_StrBuf(out, CLIP_PUSH_PARAM);
	 putByte_StrBuf(out, np->vp->pno);
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
