static int
pass_SwitchNode(void *self, Pass pass, int level, void *par)
{
   VAR(SwitchNode, np, self);
   int       i;

   switch (pass)
    {
    case Print:
       fprintfOffs(stdout, level, "SwitchNode: %d cases:\n", np->cases->count);
       fprintfOffs(stdout, level + 1, "expr:\n");
       np->expr->pass(np->expr, pass, level + 1, par);
       for (i = 0; i < np->cases->count; i++)
	{
	   int       j;

	   VAR(SwitchEl, ep, np->cases->items[i]);

		fprintfOffs(stdout, level + 1, "%d: %d labels\n", i, ep->labels_of_SwitchEl->count);
		for (j = 0; j < ep->labels_of_SwitchEl->count; j++)
	    {
			 VAR(Node, cp, ep->labels_of_SwitchEl->items[j]);
	       fprintfOffs(stdout, level + 2, "hash: %ld\n", (long) hash_const(cp));
	       cp->pass(cp, pass, level + 2, par);
	    }
	   fprintfOffs(stdout, level + 1, "operlist:\n");
		ep->operlist_of_SwitchEl->pass(ep->operlist_of_SwitchEl, pass, level + 2, par);
	}
       if (np->other)
	{
	   fprintfOffs(stdout, level + 1, "other:\n");
	   np->other->pass(np->other, pass, level + 2, par);
	}

       return 0;
    case CText:
       {
	  VAR(FILE, out, par);
	  np->expr->pass(np->expr, pass, level + 1, par);
	  fprintfOffs(out, level, "switch( _clip_pop_shash( _mp ) )\n");
	  fprintfOffs(out, level, "{\n");

	  for (i = 0; i < np->cases->count; i++)
	   {
	      int       j;

	      VAR(SwitchEl, ep, np->cases->items[i]);

			for (j = 0; j < ep->labels_of_SwitchEl->count; j++)
	       {
				 VAR(Node, cp, ep->labels_of_SwitchEl->items[j]);
		  fprintfOffs(out, level, "case %ld:\n", (long) hash_const(cp));
	       }
			 ep->operlist_of_SwitchEl->pass(ep->operlist_of_SwitchEl, pass, level + 1, par);
	      fprintfOffs(out, level + 1, "break;\n");
	   }
	  if (np->other)
	   {
	      fprintfOffs(out, level, "default:\n");
	      np->other->pass(np->other, pass, level + 1, par);
	      fprintfOffs(out, level + 1, "break;\n");
	   }
	  fprintfOffs(out, level, "}\n");
       }
       return 0;
    case OText:
       {
	  VAR(StrBuf, out, par);
	  int       labels, ibeg, iend;

	  np->expr->pass(np->expr, pass, level + 1, par);
	  putByte_StrBuf(out, CLIP_SWITCH);
	  putShort_StrBuf(out, np->labels->count);
	  ibeg = out->ptr_of_StrBuf - out->buf_of_StrBuf;
	 /* other offs */
	  putShort_StrBuf(out, 0);
	 /* label hashs */
	  for (i = 0; i < np->labels->count; i++)
	   {
	      VAR(CaseLabel, lp, np->labels->items[i]);
	      putLong_StrBuf(out, lp->hash);
	   }
	 /* place for offsets */
	 labels = out->ptr_of_StrBuf - out->buf_of_StrBuf;
	  for (i = 0; i < np->labels->count; i++)
	   {
	      putShort_StrBuf(out, 0);
	   }
	 /* bodies */
	  for (i = 0; i < np->cases->count; i++)
	   {
	      VAR(SwitchEl, ep, np->cases->items[i]);

			ep->offs_of_SwitchEl = out->ptr_of_StrBuf - out->buf_of_StrBuf;
			ep->operlist_of_SwitchEl->pass(ep->operlist_of_SwitchEl, pass, level + 1, par);
	      putByte_StrBuf(out, CLIP_GOTO);
			ep->end_of_SwitchEl = out->ptr_of_StrBuf - out->buf_of_StrBuf;
	      putShort_StrBuf(out, 0);

	   }
		SETSHORT(out, ibeg, (out->ptr_of_StrBuf - out->buf_of_StrBuf) - (ibeg + sizeof(short)));
	  if (np->other)
	   {
	      np->other->pass(np->other, pass, level + 1, par);
	   }

iend = out->ptr_of_StrBuf - out->buf_of_StrBuf;

	 /* label jumps */
	  for (i = 0; i < np->labels->count; i++)
	   {
	      VAR(CaseLabel, lp, np->labels->items[i]);
			SETSHORT(out, labels + i * sizeof(short), lp->ep->offs_of_SwitchEl - (ibeg + sizeof(short)));
	   }
	  for (i = 0; i < np->cases->count; i++)
	   {
	      VAR(SwitchEl, ep, np->cases->items[i]);
			SETSHORT(out, ep->end_of_SwitchEl, iend - (ep->end_of_SwitchEl + sizeof(short)));
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

   np->expr->pass(np->expr, pass, level, par);
   for (i = 0; i < np->cases->count; i++)
    {
       int       j;

       VAR(SwitchEl, ep, np->cases->items[i]);

		 for (j = 0; j < ep->labels_of_SwitchEl->count; j++)
	{
		VAR(Node, cp, ep->labels_of_SwitchEl->items[j]);
	   cp->pass(cp, pass, level + 2, par);
	}
	ep->operlist_of_SwitchEl->pass(ep->operlist_of_SwitchEl, pass, level + 2, par);
    }
   if (np->other)
      np->other->pass(np->other, pass, level + 2, par);
   return 0;
}
