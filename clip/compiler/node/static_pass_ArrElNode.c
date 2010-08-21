static int
pass_ArrElNode(void *self, Pass pass, int level, void *par)
{
   VAR(ArrElNode, np, self);
   switch (pass)
   {
   case Print:
      fprintfOffs(stdout, level, "ArrElNode: op='%c'\n", np->op);
      break;
   case CalcDeep:
      {
	 VAR(Function, fp, par);
	 addDeep(fp, 2);
      }
      break;
   case PrintSrc:
      {
	 VAR(CodestrNode, cp, par);
	 VAR(StrBuf, out, cp->buf);
	 VAR(Node, npp, np->arr);
	 int r, n = 0, loc = 0;

	 putString_StrBuf(out, "(");
	 if (np->oarr->isLocal)
	 {
	    if (!np->expr)
	       flush_codestr(cp, new_QuotNode(&np->node));
	    else
	       yyerror("assign to element of local array conflicts with macro");
	 }
	 else
	 {
	    for (r = first_List(&npp->list); r; r = next_List(&npp->list))
	    {
			 VAR(Node, p, npp->list.current_of_List);
	       if (!n && p->isLocal)
		  loc = 1;

	       if (n)
		  putString_StrBuf(out, "[");
	       if (p->pass(p, pass, level + 1, par))
		  break;
	       if (n)
		  putString_StrBuf(out, "]");
	       n++;
	    }
	    if (np->expr)
	    {
	       putByte_StrBuf(out, np->op);
	       np->expr->pass(np->expr, pass, level + 1, par);
	    }
	 }
	 putString_StrBuf(out, ")");
      }
      return 0;
   case CText:
      {
	 VAR(FILE, out, par);
	 if (np->expr)
	 {
	    np->expr->pass(np->expr, pass, level, par);
	    np->arr->pass(np->arr, pass, level, par);
	    if (np->node.isTop)
	    {
	       if (np->op == '=')
		  fprintfOffs(out, level, "if ((_ret=_clip_store( _mp, %d, %d ))) goto _trap_%d;\n", np->dim, np->expr->isRef ? (np->expr->isNil ? 2 : 1) : 0, np->node.seqNo);
	       else
		  fprintfOffs(out, level, "if ((_ret=_clip_opstore( _mp, %d, '%c' ))) goto _trap_%d;\n", np->dim, np->op, np->node.seqNo);
	    }
	    else
	    {
	       if (np->op == '=')
		  fprintfOffs(out, level, "if ((_ret=_clip_istore( _mp, %d, %d ))) goto _trap_%d;\n", np->dim, np->expr->isRef ? (np->expr->isNil ? 2 : 1) : 0, np->node.seqNo);
	       else
		  fprintfOffs(out, level, "if ((_ret=_clip_opistore( _mp, %d, '%c' ))) goto _trap_%d;\n", np->dim, np->op, np->node.seqNo);
	    }
	 }
	 else
	 {
	    np->arr->pass(np->arr, pass, level, par);
	    /*if (np->node.isTop)
	       fprintfOffs(out, level, "if ((_ret=_clip_fetch( _mp, %d ))) goto _trap_%d;\n", np->dim, np->node.seqNo);
	       else */
	    fprintfOffs(out, level, "if ((_ret=_clip_ifetch( _mp, %d ))) goto _trap_%d;\n", np->dim, np->node.seqNo);
	 }
      }
      return 0;
   case CTextLval:
      {
	 VAR(FILE, out, par);
	 np->arr->pass(np->arr, CText, level, par);
	 fprintfOffs(out, level, "{ClipVar *vp = _clip_fetchref( _mp, %d ); _mp->fp->ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = vp;}\n", np->dim);
      }
      return 0;
   case OText:
      {
	 VAR(StrBuf, out, par);
	 if (np->expr)
	 {
	    np->expr->pass(np->expr, pass, level, par);
	    np->arr->pass(np->arr, pass, level, par);
	    if (np->node.isTop)
	    {
	       if (np->op == '=')
	       {
		  putByte_StrBuf(out, CLIP_STORE);
		  putByte_StrBuf(out, np->expr->isRef ? (np->expr->isNil ? 2 : 1) : 0);
	       }
	       else
	       {
		  putByte_StrBuf(out, CLIP_OPSTORE);
		  putByte_StrBuf(out, np->op);
	       }
	    }
	    else
	    {
	       if (np->op == '=')
	       {
		  putByte_StrBuf(out, CLIP_ISTORE);
		  putByte_StrBuf(out, np->expr->isRef ? (np->expr->isNil ? 2 : 1) : 0);
	       }
	       else
	       {
		  putByte_StrBuf(out, CLIP_OPISTORE);
		  putByte_StrBuf(out, np->op);
	       }
	    }
	    putByte_StrBuf(out, np->dim);
	 }
	 else
	 {
	    np->arr->pass(np->arr, pass, level, par);
	    /*if (np->node.isTop)
	       putByte_StrBuf(out, CLIP_FETCH);
	       else */
	    putByte_StrBuf(out, CLIP_IFETCH);
	    putByte_StrBuf(out, np->dim);
	 }
      }
      return 0;
   case OTextLval:
      {
	 VAR(StrBuf, out, par);
	 np->arr->pass(np->arr, OText, level, par);
	 putByte_StrBuf(out, CLIP_FETCHREF);
	 putByte_StrBuf(out, np->dim);
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
   np->arr->pass(np->arr, pass, level + 1, par);
   if (np->expr)
      np->expr->pass(np->expr, pass, level + 1, par);
   return 0;
}
