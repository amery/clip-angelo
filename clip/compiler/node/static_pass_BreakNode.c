static int
pass_BreakNode(void *self, Pass pass, int level, void *par)
{
   VAR(ExprNode, np, self);
   switch (pass)
   {
   case Print:
      fprintfOffs(stdout, level, "Break node:\n");
      break;
   case PrintSrc:
      {
	 VAR(CodestrNode, cp, par);
	 VAR(StrBuf, out, cp->buf);
	 putString_StrBuf(out, "BREAK(");
	 if (np->expr)
	    np->expr->pass(np->expr, pass, level, par);
	 putString_StrBuf(out, ")");
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
	 print_line(self, level, out);
	 if (np->expr)
	 {
	    np->expr->pass(np->expr, pass, level, par);
	    fprintfOffs(out, level, "_clip_trap_pop(_mp);\n");
	    fprintfOffs(out, level, "_ret = -1;\n");
	    fprintfOffs(out, level, "goto _trap_%d;\n", np->node.seqNo);
	 }
	 else
	 {
	    fprintfOffs(out, level, "_clip_trap_var(_mp, _frame.filename, _frame.line_of_ClipFrame, 0);\n");
	    fprintfOffs(out, level, "_ret = -1;\n");
	    fprintfOffs(out, level, "goto _trap_%d;\n", np->node.seqNo);
	 }
	 if (!np->i)
	    fprintfOffs(out, level, "_clip_push_nil( _mp );\n");
      }
      return 0;
   case OText:
      {
	 VAR(StrBuf, out, par);
	 put_line(self, level, out);
	 if (np->expr)
	 {
	    np->expr->pass(np->expr, pass, level, par);
	    putByte_StrBuf(out, CLIP_BREAK_EXPR);
	 }
	 else
	    putByte_StrBuf(out, CLIP_BREAK);
	 if (!np->i)
	    putByte_StrBuf(out, CLIP_PUSH_NIL);
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

   if (np->expr)
      np->expr->pass(np->expr, pass, level, par);
   return 0;
}
