static int
pass_RefNode(void *self, Pass pass, int level, void *par)
{
   VAR(RefNode, np, self);
   switch (pass)
    {
    case Print:
       fprintfOffs(stdout, level, "Ref node: %s selfref=%d\n", np->name ? np->name : "", np->selfref);
       break;
    case CalcDeep:
       {
	  VAR(Function, fp, par);
	  addDeep(fp, 2);
	  if (np->name)
	   {
	      Function  buf;

	      int       no;

			buf.name_of_Function = np->name;
			buf.isPublic_of_Function = 1;
	      if (!search_Coll(&curFile->Coll_functions_of_File, &buf, &no))
	       {
		  if (!search_Coll(&curFile->Coll_externFunctions_of_File, np->name, &no))
		   {
		      yyerror("reference to undeclared function '%s' near line %d", np->name, np->node.line);
		      return 0;
		   }
	       }
	      else
	       {
		  np->fp = (Function *) curFile->Coll_functions_of_File.items[no];
		  np->name = np->fp->name_of_Function;
	       }
	   }
       }
       break;
    case CText:
       {
	  VAR(FILE, out, par);
	  if (np->expr)
	   {
	      if (np->expr->isArrEl)
	       {
		  VAR(ArrElNode, ap, np->expr);
		  if (!strcmp(ap->oarr->name, "call"))
		     np->expr->pass(np->expr, CText, level, par);
		  else
		   {
		      np->expr->pass(np->expr, CTextLval, level, par);
		      fprintfOffs(out, level,
				  "if ((_ret=_clip_ref( _mp, _clip_pop_ref(_mp), 0 ))) goto _trap_%d;\n", np->node.seqNo);
		   }
	       }
	      else if (np->expr->isMacro)
	       {
		  np->expr->pass(np->expr, CTextLval, level, par);
		  fprintfOffs(out, level, "if ((_ret=_clip_refmacro(_mp) )) goto _trap_%d;\n", np->node.seqNo);
	       }
	      else if (np->expr->isField)
	       {
		  np->expr->pass(np->expr, CText, level, par);
	       }
	      else if (np->expr->isFMemvar)
	       {
		  np->expr->pass(np->expr, CTextRef, level, par);
	       }
	      else
	       {
		  fprintfOffs(out, level, "if ((_ret=_clip_ref( _mp, ");
		  np->expr->pass(np->expr, CTextLval, level, par);
		  fprintf(out, ", %d ))) goto _trap_%d;\n", (np->selfref && np->expr->isLocal) ? 1 : 0, np->node.seqNo);
	       }
	   }
	  else
	   {
	      fprintfOffs(out, level, "_clip_push_func( _mp, clip_%s, 0, 1 );\n", np->name);
	   }
       }
       return 0;
    case OText:
       {
	  VAR(StrBuf, out, par);
	  if (np->expr)
	   {
	      if (np->expr->isMacro)
	       {
		  np->expr->pass(np->expr, OTextLval, level, par);
		  putByte_StrBuf(out, CLIP_REFMACRO);
	       }
	      else if (np->expr->isField)
	       {
		  np->expr->pass(np->expr, OText, level, par);
	       }
	      else if (np->expr->isFMemvar)
	       {
		  np->expr->pass(np->expr, OTextRef, level, par);
	       }
	      else
	       {
		  int       done = 0;

		  if (np->expr->isArrEl)
		   {
		      VAR(ArrElNode, ap, np->expr);
		      if (!strcmp(ap->oarr->name, "call"))
		       {
			  np->expr->pass(np->expr, OText, level, par);
			  done = 1;
		       }
		   }

		  if (!done)
		   {
		      np->expr->pass(np->expr, OTextLval, level, par);
		      putByte_StrBuf(out, CLIP_MAKE_REF);
		      putByte_StrBuf(out, (np->selfref && np->expr->isLocal) ? 1 : 0);
		   }
	       }
	   }
	  else
	   {
	      putByte_StrBuf(out, CLIP_PUSH_CODE);
			putShort_StrBuf(out, np->fp->mno_of_Function);
	      putByte_StrBuf(out, 0);
	      putByte_StrBuf(out, 1);
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

   if (np->expr)
      np->expr->pass(np->expr, pass, level + 1, par);
   return 0;
}
