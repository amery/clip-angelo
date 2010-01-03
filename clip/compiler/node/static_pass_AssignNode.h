static int
pass_AssignNode(void *self, Pass pass, int level, void *par)
{
   VAR(AssignNode, np, self);
   switch (pass)
    {
    case Print:
       fprintfOffs(stdout, level, "AssignNode: %c\n", np->op ? np->op : ' ');
       break;
    case PrintSrc:
       {
	  VAR(CodestrNode, cp, par);
	  VAR(StrBuf, out, cp->buf);
	  np->var->pass(np->var, pass, level + 1, par);
	  switch (np->op)
	   {
	   case 0:
	   case '=':
	   default:
	      putString_StrBuf(out, ":=");
	      break;
	   case '+':
	      putString_StrBuf(out, "+=");
	      break;
	   case '-':
	      putString_StrBuf(out, "-=");
	      break;
	   case '*':
	      putString_StrBuf(out, "*=");
	      break;
	   case '/':
	      putString_StrBuf(out, "/=");
	      break;
	   }
	  np->expr->pass(np->expr, pass, level + 1, par);
       }
       return 0;
    case CalcDeep:
       break;
    case CText:
       {
	  VAR(FILE, out, par);

	  if (np->expr->isRef && np->expr->isNil)
	   {
	      fprintfOffs(out, level, "if ((_ret=_clip_unref( _mp, ");
	      np->var->pass(np->var, CTextLval, level, par);
	      fprintf(out, " ))) goto _trap_%d;\n", np->node.seqNo);
	      return 0;
	   }

	  np->expr->pass(np->expr, pass, level, par);

	  if (np->expr->isRef)
	   {
	      fprintfOffs(out, level, "if ((_ret=_clip_refassign( _mp, ");
	      np->var->pass(np->var, CTextLval, level, par);
	      fprintf(out, ", %d ))) goto _trap_%d;\n", np->node.isTop ? 1 : 0, np->node.seqNo);
	   }
	  else if (np->var->isMacro)
	   {
	      np->var->pass(np->var, CTextLval, level, par);
	      fprintfOffs(out, level,
			  "if ((_ret=_clip_macroassign( _mp, '%c', %d, %d))) goto _trap_%d;\n",
			  np->op, np->node.isTop, np->field_flag, np->node.seqNo);

	   }
	  else
	   {
	      int       islocal = 0;

	      if (np->var->isLocal)
	       {
		  VAR(VarNode, lp, np->var);
		  if (!lp->vp->isParam && np->op == '='
		     /*&& !(lp->vp->init && lp->vp->init->isRef) */
		      && !lp->vp->isLocalRef)
		     islocal = 1;
	       }
	      if (np->node.isTop)
	       {
		  if (np->op != '=')
		   {
		      fprintfOffs(out, level, "if ((_ret=_clip_opassign( _mp, ");
		      np->var->pass(np->var, CTextLval, level, par);
		      fprintf(out, ", %d ))) goto _trap_%d;\n", np->op, np->node.seqNo);
		   }
		  else
		   {
		      fprintfOffs(out, level, "if ((_ret=_clip_assign( _mp, ");
#define D20061219
/*
bug in:
ndata := 0
@0,0 get ndata
readmodal(getlist)
ndata := 0   // this line crashed reference in get-object
readmodal(getlist)
*/
#ifndef D20061219
		      if (islocal)
			 fprintf(out, "_clip_ref_destroy(_mp ,");
#endif
		      np->var->pass(np->var, CTextLval, level, par);
#ifndef D20061219
		      if (islocal)
			 fprintf(out, ")");
#endif
		      fprintf(out, " ))) goto _trap_%d;\n", np->node.seqNo);
		   }
	       }
	      else
	       {
		  if (np->op != '=')
		   {
		      fprintfOffs(out, level, "if ((_ret=_clip_opiassign( _mp, ");
		      np->var->pass(np->var, CTextLval, level, par);
		      fprintf(out, ", %d ))) goto _trap_%d;\n", np->op, np->node.seqNo);
		   }
		  else
		   {
		      fprintfOffs(out, level, "if ((_ret=_clip_iassign( _mp, ");
		      if (islocal)
			 fprintf(out, "_clip_ref_destroy(_mp ,");
		      np->var->pass(np->var, CTextLval, level, par);
		      if (islocal)
			 fprintf(out, ")");
		      fprintf(out, " ))) goto _trap_%d;\n", np->node.seqNo);
		   }
	       }
	   }
       }
       return 0;
    case OText:
       {
	  VAR(StrBuf, out, par);
	  if (np->expr->isRef && np->expr->isNil)
	   {
	      np->var->pass(np->var, OTextLval, level, par);
	      putByte_StrBuf(out, CLIP_UNREF);
	      return 0;
	   }
	  np->expr->pass(np->expr, pass, level, par);
	  if (np->expr->isRef)
	   {
	      np->var->pass(np->var, OTextLval, level, par);
	      putByte_StrBuf(out, CLIP_REFASSIGN);
	      putByte_StrBuf(out, np->node.isTop ? 1 : 0);
	   }
	  else if (np->var->isMacro)
	   {
	      unsigned char b;

	      np->var->pass(np->var, OTextLval, level, par);
	      putByte_StrBuf(out, CLIP_MACROASSIGN);
	      putByte_StrBuf(out, np->op);
	      b = (np->node.isTop ? 1 : 0) | (np->field_flag ? 2 : 0);
	      putByte_StrBuf(out, b);
	   }
	  else
	   {
	      np->var->pass(np->var, OTextLval, level, par);
	      if (np->var->isLocal)
	       {
		  VAR(VarNode, lp, np->var);
		  if (!lp->vp->isParam && np->op == '='
		     /*&& !(lp->vp->init && lp->vp->init->isRef) */
		      && !lp->vp->isLocalRef)
		   {
		      putByte_StrBuf(out, CLIP_REF_DESTROY);
		   }
	       }

	      if (np->node.isTop)
	       {
		  if (np->op == '=')
		     putByte_StrBuf(out, CLIP_ASSIGN);
		  else
		   {
		      putByte_StrBuf(out, CLIP_OPASSIGN);
		      putByte_StrBuf(out, np->op);
		   }
	       }
	      else
	       {
		  if (np->op == '=')
		     putByte_StrBuf(out, CLIP_IASSIGN);
		  else
		   {
		      putByte_StrBuf(out, CLIP_OPIASSIGN);
		      putByte_StrBuf(out, np->op);
		   }
	       }
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
