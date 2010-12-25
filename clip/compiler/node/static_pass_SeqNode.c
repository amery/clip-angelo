static int
pass_SeqNode(void *self, Pass pass, int level, void *par)
{
   VAR(SeqNode, np, self);
   switch (pass)
   {
   case Print:
      fprintfOffs(stdout, level, "SeqNode :\n");
      break;
   case PrintSrc:
      {
	 yyerror("%s not allowed in CODESTR", np->node.name);
      }
      return 0;
   case CalcDeep:
      {
	 VAR(Function, fp, par);
	 if (np->using)
	    addDeep(fp, 1);
      }
      break;
   case CText:
      {
	 VAR(FILE, out, par);
	 print_line(self, level, out);
	 np->list->pass(np->list, pass, level, par);
	 fprintfOffs(out, level, "goto _end_%d;\n", np->seqNo);
	 fprintfOffs(out, level - 1, "_trap_%d:\n", np->seqNo);
	 /* unrecoverable error */
#if 0
	 fprintfOffs(out, level, "if ( _ret<0 )\n");
	 fprintfOffs(out, level + 1, "goto _trap_0;\n");
#endif
	 /* clean up the stack */
	 fprintfOffs(out, level, "while ( _frame.sp > _frame.stack )\n");
	 fprintfOffs(out, level, "\t_clip_destroy(_mp, --_frame.sp);\n");
	 if (np->using)
	 {
	    fprintfOffs(out, level, "if (_mp->trapVar)\n");
	    fprintfOffs(out, level, "{\n");
	    fprintfOffs(out, level, "\t*_frame.sp++ = *_mp->trapVar;\n");
	    fprintfOffs(out, level, "\tfree(_mp->trapVar);\n");
	    fprintfOffs(out, level, "\t_mp->trapVar = 0;\n");
	    fprintfOffs(out, level, "\t_clip_assign(_mp, ");
	    np->using->pass(np->using, CTextLval, level, par);
	    fprintf(out, ");\n");
	    fprintfOffs(out, level, "}\n");
	 }
	 if (np->recover)
	    np->recover->pass(np->recover, pass, level, par);
	 fprintfOffs(out, level, "_ret = 0;\n");
	 fprintfOffs(out, level - 1, "_end_%d:\n", np->seqNo);
      }
      return 0;
   case OText:
      {
	 VAR(StrBuf, out, par);
	 int toffs, moffs, eoffs;

	 TrapData td;

	 put_line(self, level, out);
	 putByte_StrBuf(out, CLIP_SETTRAP);
	 toffs = out->ptr_of_StrBuf - out->buf_of_StrBuf;
	 putShort_StrBuf(out, 0);

	 np->list->pass(np->list, pass, level, par);
	 putByte_StrBuf(out, CLIP_GOTO);
	 eoffs = out->ptr_of_StrBuf - out->buf_of_StrBuf;
	 putShort_StrBuf(out, 0);
	 SETSHORT(out, toffs, (out->ptr_of_StrBuf - out->buf_of_StrBuf) - (toffs + sizeof(short)));

	 moffs = out->ptr_of_StrBuf - out->buf_of_StrBuf;
	 if (np->seqLevel > 0)
	 {
	    /* recover previous handler */
	    putByte_StrBuf(out, CLIP_SETTRAP);
	    np->toffs = out->ptr_of_StrBuf - out->buf_of_StrBuf;
	    putShort_StrBuf(out, 0);
	 }
	 else
	    putByte_StrBuf(out, CLIP_RESETTRAP);

	 putByte_StrBuf(out, CLIP_RECOVER);
	 if (np->using)
	 {
	    np->using->pass(np->using, OTextLval, level, par);
	    putByte_StrBuf(out, CLIP_USING);
	 }
	 if (np->recover)
	    np->recover->pass(np->recover, pass, level, par);
	 SETSHORT(out, eoffs, (out->ptr_of_StrBuf - out->buf_of_StrBuf) - (eoffs + sizeof(short)));

	 td.StrBuf_out_of_TrapData = out;
	 td.offs_of_TrapData = moffs;
	 np->list->pass(np->list, OTextTrap, level, &td);
      }
      return 0;
   case OTextTrap:
      {
	 VAR(TrapData, tp, par);
	 /* SETSHORT (tp->out,  np->toffs,  tp->offs); */
	 SETSHORT(tp->StrBuf_out_of_TrapData, np->toffs, (tp->offs_of_TrapData - (np->toffs + sizeof(short))));
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
   np->list->pass(np->list, pass, level, par);
   if (np->using)
      np->using->pass(np->using, pass, level, par);
   if (np->recover)
      np->recover->pass(np->recover, pass, level, par);
   return 0;
}
