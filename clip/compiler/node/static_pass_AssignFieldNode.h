static int
pass_AssignFieldNode(void *self, Pass pass, int level, void *par)
{
   VAR(AssignFieldNode, np, self);
   switch (pass)
    {
    case Print:
       fprintfOffs(stdout, level, "AssignFieldNode: %s\n", np->name ? np->name : "");
       fprintfOffs(stdout, level, "area: %s\n", np->area ? np->area : "");
       if (np->areaExpr)
	  np->areaExpr->pass(np->areaExpr, pass, level + 1, par);
       if (np->nameExpr)
	{
	   fprintfOffs(stdout, level, "field:\n");
	   np->nameExpr->pass(np->nameExpr, pass, level + 1, par);
	}
       else
	  fprintfOffs(stdout, level, "field: %s\n", np->name);
       fprintfOffs(stdout, level, "expr:\n");
       np->expr->pass(np->expr, pass, level + 1, par);
       return 0;
    case PrintSrc:
       {
	  VAR(CodestrNode, cp, par);
	  VAR(StrBuf, out, cp->buf);
	  if (np->area)
	   {
	      putString_StrBuf(out, np->area);
	      putString_StrBuf(out, "->");
	   }
	  else if (np->areaExpr)
	   {
	      putString_StrBuf(out, "(");
	      np->areaExpr->pass(np->areaExpr, pass, level + 1, par);
	      putString_StrBuf(out, ")");
	   }
	  else
	   {
	      putString_StrBuf(out, "FIELD->");
	   }

	  if (np->name)
	   {
	      putString_StrBuf(out, np->name);
	   }
	  else if (np->nameExpr)
	   {
	      if (!np->nameExpr->isMacro)
		 putString_StrBuf(out, "(");
	      np->nameExpr->pass(np->nameExpr, pass, level + 1, par);
	      if (!np->nameExpr->isMacro)
		 putString_StrBuf(out, ")");
	   }
	  putString_StrBuf(out, ":=");
	  np->expr->pass(np->expr, pass, level + 1, par);
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

	  np->expr->pass(np->expr, pass, level, par);
	  if (np->nameExpr)
	   {
	      if (np->nameExpr->isMacro)
	       {
		  np->nameExpr->pass(np->nameExpr, CTextLval, level, par);
		  if (np->areaExpr || np->area)
		     fprintfOffs(out, level, "_clip_calc_hash(_mp);\n");
		  else
		     fprintfOffs(out, level, "_clip_calc_hash2(_mp, 0);\n");
	       }
	      else
		 np->nameExpr->pass(np->nameExpr, pass, level, par);
	   }
	  if (np->areaExpr)
	   {
	      if (np->areaExpr->isMacro)
		 np->areaExpr->pass(np->areaExpr, CTextLval, level, par);
	      else
		 np->areaExpr->pass(np->areaExpr, pass, level, par);
	   }

	  if (np->node.isTop)
	     fprintfOffs(out, level, "if ( (_ret=_clip_assign_field");
	  else
	     fprintfOffs(out, level, "if ( (_ret=_clip_iassign_field");

	  if (np->nameExpr)
	   {
	      fprintf(out, "( _mp, _clip_pop_hash(_mp), ");
	      if (np->nameExpr->isMacro && !np->areaExpr && !np->area)
		 fprintf(out, "_clip_pop_hash(_mp) ))) goto _trap_%d;\n", np->node.seqNo);
	   }
	  else
	     fprintf(out, "( _mp, %ld /* %s */, ", (long) hashstr(np->name), np->name);

	  if (np->areaExpr)
	   {
	      fprintf(out, "_clip_pop_hash(_mp)))) goto _trap_%d;\n", np->node.seqNo);
	   }
	  else
	   {
	      if (np->area)
		 fprintf(out, "%ld /* %s */))) goto _trap_%d;\n", (long) n_hashstr(np->area), np->area, np->node.seqNo);
	      else if (np->nameExpr && np->nameExpr->isMacro)
		 ;
	      else
		 fprintf(out, "-1))) goto _trap_%d;\n", np->node.seqNo);
	   }
       }
       return 0;
    case OText:
       {
	  VAR(StrBuf, out, par);

	  np->expr->pass(np->expr, pass, level, par);
	  if (np->areaExpr)
	   {
	      if (np->areaExpr->isMacro)
		 np->areaExpr->pass(np->areaExpr, OTextLval, level, par);
	      else
		 np->areaExpr->pass(np->areaExpr, pass, level, par);
	   }

	  if (np->nameExpr)
	   {
	      if (np->nameExpr->isMacro)
	       {
		  np->nameExpr->pass(np->nameExpr, OTextLval, level, par);
		  if (np->areaExpr || np->area)
		     putByte_StrBuf(out, CLIP_CALC_HASH);
		  else
		     putByte_StrBuf(out, CLIP_CALC_HASH2);
	       }
	      else
		 np->nameExpr->pass(np->nameExpr, pass, level, par);
	   }
	  if (np->areaExpr)
	   {
	      if (np->nameExpr)
	       {
		  if (np->node.isTop)
		     putByte_StrBuf(out, CLIP_ASSIGN_PFIELD_POP);
		  else
		     putByte_StrBuf(out, CLIP_IASSIGN_PFIELD_POP);
	       }
	      else
	       {
		  if (np->node.isTop)
		     putByte_StrBuf(out, CLIP_ASSIGN_FIELD_POP);
		  else
		     putByte_StrBuf(out, CLIP_IASSIGN_FIELD_POP);
		  putLong_StrBuf(out, hashstr(np->name));
	       }
	   }
	  else
	   {
	      if (np->nameExpr)
	       {
		  if (np->node.isTop)
		   {
		      if (np->nameExpr->isMacro && !np->areaExpr && !np->area)
		       {
			  putByte_StrBuf(out, CLIP_ASSIGN_PFIELD_POP);
		       }
		      else
		       {
			  putByte_StrBuf(out, CLIP_ASSIGN_PFIELD);
			  if (np->area)
			     putLong_StrBuf(out, n_hashstr(np->area));
			  else
			     putLong_StrBuf(out, -1);
		       }
		   }
		  else
		   {
		      if (np->nameExpr->isMacro && !np->areaExpr && !np->area)
		       {
			  putByte_StrBuf(out, CLIP_IASSIGN_PFIELD_POP);
		       }
		      else
		       {
			  putByte_StrBuf(out, CLIP_IASSIGN_PFIELD);
			  if (np->area)
			     putLong_StrBuf(out, n_hashstr(np->area));
			  else
			     putLong_StrBuf(out, -1);
		       }
		   }
	       }
	      else
	       {
		  if (np->node.isTop)
		   {
		      putByte_StrBuf(out, CLIP_ASSIGN_FIELD);
		      putLong_StrBuf(out, hashstr(np->name));
		      if (np->area)
			 putLong_StrBuf(out, n_hashstr(np->area));
		      else
			 putLong_StrBuf(out, -1);
		   }
		  else
		   {
		      putByte_StrBuf(out, CLIP_IASSIGN_FIELD);
		      putLong_StrBuf(out, hashstr(np->name));
		      if (np->area)
			 putLong_StrBuf(out, n_hashstr(np->area));
		      else
			 putLong_StrBuf(out, -1);
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
   if (np->nameExpr)
      np->nameExpr->pass(np->nameExpr, pass, level + 1, par);
   if (np->areaExpr)
      np->areaExpr->pass(np->areaExpr, pass, level + 1, par);
   np->expr->pass(np->expr, pass, level + 1, par);
   return 0;
}
