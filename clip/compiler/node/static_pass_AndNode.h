static int
pass_AndNode(void *self, Pass pass, int level, void *par)
{
   VAR(OpNode, np, self);
   int       no = np->op;

   switch (pass)
    {
    case Print:
       fprintfOffs(stdout, level, "AndNode label=%d\n", no);
       break;
    case PrintSrc:
       {
	  VAR(CodestrNode, cp, par);
	  VAR(StrBuf, out, cp->buf);
	  putString_StrBuf(out, "(");
	  np->expr1->pass(np->expr1, pass, level + 1, par);
	  putString_StrBuf(out, ".AND.");
	  np->expr2->pass(np->expr2, pass, level + 1, par);
	  putString_StrBuf(out, ")");
       }
       return 0;
    case CalcDeep:
       break;
    case CText:
       {
	  VAR(FILE, out, par);
	  np->expr1->pass(np->expr1, pass, level, par);
	  fprintfOffs(out, level, "if ((_ret=_clip_tcond( _mp, &_if ))) goto _trap_%d;\n", np->node.seqNo);
	  fprintfOffs(out, level, "if ( !_if ) goto _endand_%d;\n", no);
	  fprintfOffs(out, level, "_clip_pop(_mp);\n");
	  np->expr2->pass(np->expr2, pass, level, par);
	  fprintfOffs(out, level, "_endand_%d:\n", no);
       }
       return 0;
    case OText:
       {
	  VAR(StrBuf, out, par);
	  int       jmp;

	  np->expr1->pass(np->expr1, pass, level, par);

	  putByte_StrBuf(out, CLIP_TCOND);
	  jmp = out->ptr_of_StrBuf - out->buf_of_StrBuf;
	  putShort_StrBuf(out, 0);

	  putByte_StrBuf(out, CLIP_POP);
	  np->expr2->pass(np->expr2, pass, level, par);
	  SETSHORT(out, jmp, (out->ptr_of_StrBuf - out->buf_of_StrBuf) - (jmp + sizeof(short)));

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
   np->expr1->pass(np->expr1, pass, level + 1, par);
   np->expr2->pass(np->expr2, pass, level + 1, par);
   return 0;
}
