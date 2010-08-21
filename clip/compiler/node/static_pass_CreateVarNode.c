static int
pass_CreateVarNode(void *self, Pass pass, int level, void *par)
{
   VAR(CreateVarNode, np, self);
   switch (pass)
   {
   case Print:
      {
	 fprintfOffs(stdout, level, "create var node: space=%s vars\n", np->spacename);
	 print_Var(np->vp, level + 1);
      }
      break;
   case CText:
      {
	 VAR(FILE, out, par);
	 print_line(self, level, out);
	 if (np->vp->macro)
	 {
	    np->vp->macro->pass(np->vp->macro, /*pass, */ CTextLval,
				level + 1, par);
	    if (np->space == 1)
	       fprintfOffs(out, level, "_clip_memvar_public( _mp, _clip_pop_hash( _mp ) );\n");
	    else if (np->space == 0)
	       fprintfOffs(out, level, "_clip_add_private( _mp, _clip_pop_hash( _mp ) );\n");
	    else
	       fprintfOffs(out, level, "_clip_memvar_space( _mp, _clip_space( _mp, %ld /* %s */) ,  _clip_pop_hash( _mp ), 1 );\n", (long) np->space, np->spacename);
	 }
	 else
	 {
	    if (np->space == 1)
	       fprintfOffs(out, level, "_clip_memvar_public( _mp, %ld /* %s */ );\n", (long) hashstr(np->vp->name), np->vp->name);
	    else if (np->space == 0)
	       fprintfOffs(out, level, "_clip_memvar_private( _mp, %ld /* %s */ );\n", (long) hashstr(np->vp->name), np->vp->name);
	    else
	       fprintfOffs(out, level, "_clip_memvar_space( _mp, _clip_space( _mp, %ld /* %s */) , %ld /* %s */, 1 );\n", (long) np->space, np->spacename, (long) hashstr(np->vp->name), np->vp->name);
	 }
      }
      break;
   case OText:
      {
	 VAR(StrBuf, out, par);
	 put_line(self, level, out);
	 if (np->vp->macro)
	 {
	    np->vp->macro->pass(np->vp->macro, /*pass, */ OTextLval,
				level + 1, par);
	    if (np->space == 1)
	       putByte_StrBuf(out, CLIP_MEMVAR_PUBLIC_POP);
	    else if (np->space == 0)
	       putByte_StrBuf(out, CLIP_MEMVAR_PRIVATE_POP);
	    else
	    {
	       putByte_StrBuf(out, CLIP_MEMVAR_SPACE_POP);
	       putLong_StrBuf(out, np->space);
	    }
	 }
	 else
	 {
	    if (np->space == 1)
	       putByte_StrBuf(out, CLIP_MEMVAR_PUBLIC);
	    else if (np->space == 0)
	       putByte_StrBuf(out, CLIP_MEMVAR_PRIVATE);
	    else
	    {
	       putByte_StrBuf(out, CLIP_MEMVAR_SPACE);
	       putLong_StrBuf(out, np->space);
	    }
	    putLong_StrBuf(out, hashstr(np->vp->name));
	 }
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

   return 0;
}
