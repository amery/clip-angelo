static int
pass_IfNode(void *self, Pass pass, int level, void *par)
{
   VAR(IfNode, np, self);
   int i;

   switch (pass)
   {
   case Print:
      fprintfOffs(stdout, level, "IfNode :\n");
      break;
   case PrintSrc:
      {
	 VAR(CodestrNode, cp, par);
	 VAR(StrBuf, out, cp->buf);
	 putString_StrBuf(out, "iif(");
	 for (i = 0; i < np->elseifs->count_of_Coll; i += 2)
	 {
		 VAR(Node, p, np->elseifs->items_of_Coll[i]);
		 VAR(Node, lp, np->elseifs->items_of_Coll[i + 1]);
	    p->pass(p, pass, level, par);
	    putString_StrBuf(out, ",");
	    lp->pass(lp, pass, level, par);
	 }
	 putString_StrBuf(out, ",");
	 if (np->elselist)
	    np->elselist->pass(np->elselist, pass, level, par);
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
	 int no = np->labelNo;

	 int eno = no + np->elseifs->count_of_Coll / 2 + (np->elselist ? 0 : -1);

	 print_line(self, level, out);
	 for (i = 0; i < np->elseifs->count_of_Coll; i += 2)
	 {
		 VAR(Node, p, np->elseifs->items_of_Coll[i]);
		 VAR(Node, lp, np->elseifs->items_of_Coll[i + 1]);
	    p->pass(p, pass, level, par);
	    fprintfOffs(out, level, "if ((_ret=_clip_cond( _mp, &_if ))) goto _trap_%d;\n", np->node.seqNo);
	    fprintfOffs(out, level, "if ( !_if ) goto _endif_%d_%d;\n", no, loopPart);
	    lp->pass(lp, pass, level, par);
	    fprintfOffs(out, level, "goto _endif_%d_%d;\n", eno, loopPart);
	    fprintfOffs(out, level, "_endif_%d_%d:\n", no, loopPart);
	    ++no;
	 }
	 if (np->elselist)
	 {
	    np->elselist->pass(np->elselist, pass, level, par);
	    fprintfOffs(out, level, "_endif_%d_%d:\n", no, loopPart);
	 }
      }
      return 0;
   case OText:
      {
	 VAR(StrBuf, out, par);
	 int no;

	 int count = np->elseifs->count_of_Coll / 2 + (np->elselist ? 0 : -1);

	 int eoffs;

	 int *ejmps = (int *) malloc(sizeof(int) * (count + 1));

	 put_line(self, level, out);
	 for (i = 0, no = 0; i < np->elseifs->count_of_Coll; i += 2, ++no)
	 {
		 VAR(Node, p, np->elseifs->items_of_Coll[i]);
		 VAR(Node, lp, np->elseifs->items_of_Coll[i + 1]);
	    int jmp;

	    p->pass(p, pass, level, par);
	    putByte_StrBuf(out, CLIP_COND);
	    jmp = out->ptr_of_StrBuf - out->buf_of_StrBuf;
	    putShort_StrBuf(out, 0);
	    lp->pass(lp, pass, level, par);

	    if (no < count)
	    {
	       putByte_StrBuf(out, CLIP_GOTO);
	       ejmps[no] = out->ptr_of_StrBuf - out->buf_of_StrBuf;
	       putShort_StrBuf(out, 0);
	    }

	    SETSHORT(out, jmp, (out->ptr_of_StrBuf - out->buf_of_StrBuf) - (jmp + sizeof(short)));
	 }
	 if (np->elselist)
	 {
	    np->elselist->pass(np->elselist, pass, level, par);
	 }
	 eoffs = out->ptr_of_StrBuf - out->buf_of_StrBuf;
	 for (i = 0; i < count; ++i)
	 {
	    SETSHORT(out, ejmps[i], eoffs - (ejmps[i] + sizeof(short)));
	 }
	 free(ejmps);
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
      for (i = 0; i < np->elseifs->count_of_Coll; i += 2)
   {
		VAR(Node, p, np->elseifs->items_of_Coll[i]);
		VAR(Node, lp, np->elseifs->items_of_Coll[i + 1]);
      p->pass(p, pass, level, par);
      lp->pass(lp, pass, level, par);
   }
   if (np->elselist)
      np->elselist->pass(np->elselist, pass, level, par);
   return 0;
}
