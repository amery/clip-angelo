static int
pass_ParameterNode(void *self, Pass pass, int level, void *par)
{
   VAR(VarNode, np, self);
   switch (pass)
    {
    case Print:
       fprintfOffs(stdout, level, "Parameter node: %s (%d)\n", np->vp->name, np->vp->no);
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
	  fprintfOffs(out, level,
		      "if ( (_ret=_clip_memvar( _mp, %ld /* %s */ ) )) goto _trap_%d;\n",
		      (long) hashstr(np->vp->name), np->vp->name, np->node.seqNo);
       }
       break;
    case CTextLval:
       {
	  VAR(FILE, out, par);
	  fprintf(out, "_clip_ref_memvar( _mp, %ld /* %s */ )", (long) hashstr(np->vp->name), np->vp->name);
       }
       break;
    case OText:
       {
	  VAR(StrBuf, out, par);
	  putByte_StrBuf(out, CLIP_PUSH_MEMVAR);
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
