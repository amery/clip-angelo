static int
pass_WhileNode(void *self, Pass pass, int level, void *par)
{
   VAR(WhileNode, np, self);
   switch (pass)
   {
   case Print:
      fprintfOffs(stdout, level, "WhileNode :\n");
      break;
   case PrintSrc:
      {
	 yyerror("%s not allowed in CODESTR", np->node.name);
      }
      return 0;
   case CalcDeep:
      break;
   case CText:
      {
	 VAR(FILE, out, par);
	 int no = np->labelNo;

	 int saveNo = loopNo;

	 print_line(self, level, out);
	 fprintfOffs(out, level, "_loop_%d:\n", no);

	 np->expr->pass(np->expr, pass, level, par);

	 fprintfOffs(out, level, "if ((_ret=_clip_cond( _mp, &_if ))) goto _trap_%d;\n", np->node.seqNo);
	 fprintfOffs(out, level, "if ( !_if ) goto _endloop_%d;\n", no);

	 loopNo = no;
	 np->list->pass(np->list, pass, level, par);

	 fprintfOffs(out, level, "goto _loop_%d;\n", no);
	 fprintfOffs(out, level, "_endloop_%d:\n", no);
	 loopNo = saveNo;
      }
      return 0;
   case OText:
      {
	 VAR(StrBuf, out, par);
	 int jmp, eoffs, boffs = out->ptr_of_StrBuf - out->buf_of_StrBuf;

	 LoopData ld;

	 put_line(self, level, out);
	 np->expr->pass(np->expr, pass, level, par);

	 putByte_StrBuf(out, CLIP_COND);
	 jmp = out->ptr_of_StrBuf - out->buf_of_StrBuf;
	 putShort_StrBuf(out, 0);

	 np->list->pass(np->list, pass, level, par);

	 putByte_StrBuf(out, CLIP_GOTO);
	 eoffs = (out->ptr_of_StrBuf - out->buf_of_StrBuf) + sizeof(short);

	 putShort_StrBuf(out, boffs - eoffs);
	 SETSHORT(out, jmp, eoffs - (jmp + sizeof(short)));

	 ld.StrBuf_out_of_LoopData = out;
	 ld.boffs_of_LoopData = boffs;
	 ld.eoffs_of_LoopData = eoffs;
	 np->list->pass(np->list, OTextLoop, level, &ld);
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
   np->list->pass(np->list, pass, level, par);
   return 0;
}
