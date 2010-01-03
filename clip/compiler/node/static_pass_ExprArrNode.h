static int
pass_ExprArrNode(void *self, Pass pass, int level, void *par)
{
   VAR(ExprArrNode, npp, self);

   switch (pass)
    {
    case Print:
       fprintfOffs(stdout, level, "ExprArrNode: %d, islast=%d\n", npp->coll.count, npp->last);
       break;
    case PrintSrc:
       {
	  VAR(CodestrNode, cp, par);
	  VAR(StrBuf, out, cp->buf);
	  int       n;

	  for (n = 0; n < npp->coll.count; n++)
	   {
	      VAR(Node, p, npp->coll.items[n]);
	      if (n)
		 putString_StrBuf(out, ", ");
	      if (p->pass(p, pass, level + 1, par))
		 break;
	   }
       }
       return 0;
    case Traverse:
       {
	  VAR(TraversePar, tp, par);
	  tp->func((Node *) self, tp->par);
       }
       break;
    case CText:
       {
	  int       n, e;

	  VAR(FILE, out, par);
	  for (n = 0, e = npp->coll.count; n < e; n++)
	   {
	      VAR(Node, p, npp->coll.items[n]);
	      p->pass(p, pass, level + 1, par);
	      if (n == (e - 1) && npp->last)
		 fprintfOffs(out, level + 1, "_clip_pop(_mp);\n");
	   }

       }
       return 0;
    case OText:
       {
	  int       n, e;

	  VAR(StrBuf, out, par);
	  for (n = 0, e = npp->coll.count; n < e; n++)
	   {
	      VAR(Node, p, npp->coll.items[n]);
	      p->pass(p, pass, level + 1, par);
	      if (n == (e - 1) && npp->last)
		 putByte_StrBuf(out, CLIP_POP);
	   }

       }
       return 0;
    default:
       break;
    }

   {
      int       n, e;

      for (n = 0, e = npp->coll.count; n < e; n++)
       {
	  VAR(Node, p, npp->coll.items[n]);
	  p->pass(p, pass, level + 1, par);
       }
   }
   return 0;
}
