static int
pass_ReturnNode(void *self, Pass pass, int level, void *par)
{
   VAR(ExprNode, np, self);
   switch (pass)
    {
    case Print:
       fprintfOffs(stdout, level, "ReturnNode: isExprList=%d\n", np->i);
       break;
    case PrintSrc:
       {
	  VAR(CodestrNode, cp, par);
	  VAR(StrBuf, out, cp->buf);
	  putString_StrBuf(out, "return ");
	  if (np->expr)
	     np->expr->pass(np->expr, pass, level, par);
       }
       return 0;
    case CalcDeep:
       {
	  if (np->expr)
	     np->expr->pass(np->expr, pass, level, par);
	  return 0;
       }
       break;
    case CText:
       {
	  VAR(FILE, out, par);
	  if (!np->i)
	   {
	      if (np->expr)
	       {
		  np->expr->pass(np->expr, pass, level, par);
		  fprintfOffs(out, level, "_clip_return( _mp );\n");
	       }
	   }
	  else
	   {
	      int       count = getCount_Node(np->expr);

	      if (count > 0)
	       {
		  np->expr->pass(np->expr, pass, level, par);
		  fprintfOffs(out, level, "_clip_return( _mp );\n");
	       }
	   }
	  fprintfOffs(out, level, "goto _return;\n");
	  return 0;
       }
       break;
    case OText:
       {
	  VAR(StrBuf, out, par);
	  if (!np->i)
	   {
	      if (np->expr)
	       {
		  np->expr->pass(np->expr, pass, level, par);
		  putByte_StrBuf(out, CLIP_RETURN_POP);
	       }
	      else
		 putByte_StrBuf(out, CLIP_RETURN);
	   }
	  else
	   {
	      int       count = getCount_Node(np->expr);

	      if (count > 0)
	       {
		  np->expr->pass(np->expr, pass, level, par);
		  putByte_StrBuf(out, CLIP_RETURN_POP);
	       }
	      else
		 putByte_StrBuf(out, CLIP_RETURN);
	   }
	  return 0;
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

   if (np->expr)
      np->expr->pass(np->expr, pass, level, par);
   return 0;
}
