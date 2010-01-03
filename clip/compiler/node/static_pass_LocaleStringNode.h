static int
pass_LocaleStringNode(void *self, Pass pass, int level, void *par)
{
   VAR(ExprNode, np, self);
   switch (pass)
    {
    case Print:
       fprintfOffs(stdout, level, "LocaleStringNode: module %s:\n", CLIP_MODULE);
       break;
    case PrintSrc:
       {
	  VAR(CodestrNode, cp, par);
	  VAR(StrBuf, out, cp->buf);
	  putString_StrBuf(out, "[");
	  np->expr->pass(np->expr, pass, level, par);
	  putString_StrBuf(out, "]");
       }
       return 0;
    case CalcDeep:
       break;
    case CText:
       {
	  VAR(FILE, out, par);
	  np->expr->pass(np->expr, CText, level, par);
	  fprintfOffs(out, level, "_clip_push_locale( _mp );\n");
       }
       return 0;
    case OText:
       {
	  VAR(StrBuf, out, par);
	  np->expr->pass(np->expr, OText, level, par);
	  putByte_StrBuf(out, CLIP_PUSH_LOCALE);
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
   pass_Node(np->expr, pass, level, par);
   return 0;
}
