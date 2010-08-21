static int
pass_ArrayInitNode(void *self, Pass pass, int level, void *par)
{
   VAR(ExprNode, np, self);
   switch (pass)
   {
   case Print:
      fprintfOffs(stdout, level, "ArrayInitNode (%d):\n", np->i);
      break;
   case PrintSrc:
      {
	 VAR(CodestrNode, cp, par);
	 VAR(StrBuf, out, cp->buf);
	 putString_StrBuf(out, "{");
	 np->expr->pass(np->expr, pass, level + 1, par);
	 putString_StrBuf(out, "}");
      }
      return 0;
   case CalcDeep:
      {
	 VAR(Function, fp, par);
	 addDeep(fp, np->i + 1);
      }
      break;
   case CText:
      {
	 VAR(FILE, out, par);

	 np->expr->pass(np->expr, CText, level, par);
	 fprintfOffs(out, level, "_clip_sarray( _mp, %d );\n", np->i);
      }
      return 0;
   case OText:
      {
	 VAR(StrBuf, out, par);
	 np->expr->pass(np->expr, OText, level, par);
	 putByte_StrBuf(out, CLIP_ARRAY);
	 putLong_StrBuf(out, np->i);
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
