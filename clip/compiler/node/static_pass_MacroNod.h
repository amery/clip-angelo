static int
pass_MacroNode(void *self, Pass pass, int level, void *par)
{
   VAR(ExprNode, np, self);
   switch (pass)
    {
    case Print:
       fprintfOffs(stdout, level, "MacroNode :\n");
       break;
    case SearchMacro:
       {
	  VAR(int, haveMacro, par);

	  int       paramsonly = 1;

	  TraversePar tpar;

	  tpar.func = find_paramsonly;
	  tpar.par = &paramsonly;
	  np->expr->pass(np->expr, Traverse, 0, &tpar);
	  if (!paramsonly && !np->i)
	     (*haveMacro)++;
       }
       break;
    case PrintSrc:
       {
	  VAR(CodestrNode, cp, par);
	  if (np->expr->isArrEl)
	   {
	      VAR(StrBuf, out, cp->buf);
	      putString_StrBuf(out, "&");
	      np->expr->pass(np->expr, pass, level + 1, par);
	   }
	  else if (!strcmp(np->expr->name, "local") && ((VarNode *) np->expr)->vp->isCodeParam)
	   {
	      VAR(StrBuf, out, cp->buf);
	      putString_StrBuf(out, "&");
	      putString_StrBuf(out, (((VarNode *) np->expr)->vp->name));
	   }
	  else
	     flush_codestr(cp, np->expr);
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
	  np->expr->pass(np->expr, CText, level, par);
	  fprintfOffs(out, level, "if ( (_ret=_clip_macro( _mp ))) goto _trap_%d;\n", np->node.seqNo);
       }
       return 0;
    case CTextLval:
       {
	  np->expr->pass(np->expr, CText, level, par);
       }
       return 0;
    case OText:
       {
	  VAR(StrBuf, out, par);
	  np->expr->pass(np->expr, OText, level, par);
	  putByte_StrBuf(out, CLIP_MACRO);
       }
       return 0;
    case OTextLval:
       {
	  np->expr->pass(np->expr, OText, level, par);
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
   np->expr->pass(np->expr, pass, level + 1, par);
   return 0;
}
