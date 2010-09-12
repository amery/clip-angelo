static int
pass_ForNode(void *self, Pass pass, int level, void *par)
{
   VAR(ForNode, np, self);
   switch (pass)
   {
   case Print:
      fprintfOffs(stdout, level, "ForNode :\n");
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
	 loopPart = 0;
	 np->init->pass(np->init, pass, level, par);
	 np->to->pass(np->to, pass, level, par);
	 np->step->pass(np->step, pass, level, par);

	 fprintfOffs(out, level, "if ((_ret=_clip_forstep( _mp, &_if ))) goto _trap_%d;\n", np->node.seqNo);
	 fprintfOffs(out, level, "if ( !_if ) goto _endloop_%d;\n", no);

	 fprintfOffs(out, level, "goto _lbeg_%d;\n", no);

	 fprintfOffs(out, level, "_loop_%d:\n", no);
	 loopPart = 1;
	 np->var->pass(np->var, pass, level, par);
	 np->to->pass(np->to, pass, level, par);
	 np->step->pass(np->step, pass, level, par);
	 loopPart = 0;

	 fprintfOffs(out, level, "if ((_ret=_clip_forstep( _mp, &_if ))) goto _trap_%d;\n", np->node.seqNo);
	 fprintfOffs(out, level, "if ( !_if ) goto _endloop_%d;\n", no);

	 fprintfOffs(out, level, "_lbeg_%d:\n", no);

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
	 int jmp, eoffs, ibeg, lbeg, boffs;

	 LoopData ld;

	 put_line(self, level, out);
	 np->init->pass(np->init, pass, level, par);
	 np->to->pass(np->to, pass, level, par);
	 np->step->pass(np->step, pass, level, par);
	 putByte_StrBuf(out, CLIP_FORSTEP);
	 ibeg = out->ptr_of_StrBuf - out->buf_of_StrBuf;
	 putShort_StrBuf(out, 0);

	 putByte_StrBuf(out, CLIP_GOTO);
	 lbeg = out->ptr_of_StrBuf - out->buf_of_StrBuf;
	 putShort_StrBuf(out, 0);

	 boffs = out->ptr_of_StrBuf - out->buf_of_StrBuf;
	 put_line(self, level, out);
	 np->var->pass(np->var, pass, level, par);
	 np->to->pass(np->to, pass, level, par);
	 np->step->pass(np->step, pass, level, par);
	 putByte_StrBuf(out, CLIP_FORSTEP);
	 jmp = out->ptr_of_StrBuf - out->buf_of_StrBuf;
	 putShort_StrBuf(out, 0);

	 SETSHORT(out, lbeg, (out->ptr_of_StrBuf - out->buf_of_StrBuf) - (lbeg + sizeof(short)));

	 np->list->pass(np->list, pass, level, par);

	 putByte_StrBuf(out, CLIP_GOTO);
	 eoffs = (out->ptr_of_StrBuf - out->buf_of_StrBuf) + sizeof(short);
	 putShort_StrBuf(out, boffs - eoffs);

	 SETSHORT(out, jmp, eoffs - (jmp + sizeof(short)));
	 SETSHORT(out, ibeg, eoffs - (ibeg + sizeof(short)));

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

   np->init->pass(np->init, pass, level, par);
   np->step->pass(np->step, pass, level, par);
   np->var->pass(np->var, pass, level, par);
   np->to->pass(np->to, pass, level, par);
   np->list->pass(np->list, pass, level, par);

   return 0;
}
