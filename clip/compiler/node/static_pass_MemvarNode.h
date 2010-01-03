static int
pass_MemvarNode(void *self, Pass pass, int level, void *par)
{
   VAR(VarNode, np, self);
   switch (pass)
    {
    case Print:
       fprintfOffs(stdout, level, "Memvar node: %s (%d)\n", np->vp->name, np->vp->no);
       break;
    case PrintSrc:
       {
	  VAR(CodestrNode, cp, par);
	  VAR(StrBuf, out, cp->buf);
	  putString_StrBuf(out, "m->");
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
	  fprintfOffs(out, level,
		      "if ( (_ret=_clip_memvar( _mp, %ld /* %s */ ))) goto _trap_%d;\n",
		      (long) hashstr(np->vp->name), np->vp->name, np->node.seqNo);
       }
       break;
    case CTextLval:
       {
	  VAR(FILE, out, par);
	  if (np->node.isAssignLval)
	     fprintf(out, "_clip_ref_memvar( _mp, %ld /* %s */ )", (long) hashstr(np->vp->name), np->vp->name);
	  else
	     fprintf(out, "_clip_ref_memvar_noadd( _mp, %ld /* %s */ )", (long) hashstr(np->vp->name), np->vp->name);
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
	  if (np->node.isAssignLval)
	     putByte_StrBuf(out, CLIP_PUSH_REF_MEMVAR);
	  else
	     putByte_StrBuf(out, CLIP_PUSH_REF_MEMVAR_NOADD);
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
