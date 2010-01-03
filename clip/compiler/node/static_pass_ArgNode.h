static int
pass_ArgNode(void *self, Pass pass, int level, void *par)
{
   VAR(ExprNode, np, self);
   switch (pass)
    {
    case Print:
       fprintfOffs(stdout, level, "Arg node: byRef==%d\n", np->i);
       break;
    case CalcDeep:
       break;
    case CText:
       {
	  VAR(FILE, out, par);
	  if (np->i)
	   {
	      fprintfOffs(out, level, "if ((_ret=_clip_ref( _mp, ");
	     /*if (np->expr->isConst) */
	      fprintf(out, "(ClipVar*)");
	      np->expr->pass(np->expr, CTextLval, level, par);
	      fprintf(out, ", 0 ))) goto _trap_%d;\n", np->node.seqNo);
	   }
	  else
	     np->expr->pass(np->expr, pass, level, par);
	 /*
	    if (np->i)
	    fprintfOffs(out, level, "_clip_unref_arr( _mp );\n");
	  */
       }
       return 0;
    case OText:
       {
	  VAR(StrBuf, out, par);
	  if (np->i)
	   {
	      np->expr->pass(np->expr, OTextLval, level, par);
	      putByte_StrBuf(out, CLIP_MAKE_REF);
	      putByte_StrBuf(out, 0);
	   }
	  else
	     np->expr->pass(np->expr, pass, level, par);
	 /*
	    if (np->i)
	    putByte_StrBuf(out, CLIP_UNREF_ARR);
	  */
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
