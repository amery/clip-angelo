static int
pass_ConstNode(void *self, Pass pass, int level, void *par)
{
   VAR(ConstNode, np, self);
   switch (pass)
   {
   case Print:
      fprintfOffs(stdout, level, "const node (%d) isInit=%d:\n", np->no, np->node.isInit);
      switch (np->type)
      {
      case CONST_NUMBER:
	 fprintfOffs(stdout, level, "NUMBER=%s%s, rval='%s'\n", np->node.isMinus ? "-" : "", np->val, np->rval ? np->rval : "");
	 break;
      case CONST_STRING:
	 fprintfOffs(stdout, level, "STRING='%s'\n", np->val);
	 break;
      case CONST_NIL:
	 fprintfOffs(stdout, level, "NIL\n");
	 break;
      case CONST_LOGIC:
	 fprintfOffs(stdout, level, "LOGIC=%s %d\n", np->no ? ".T." : ".F.", np->node.isMinus);
	 break;
      }
      break;
   case CTextInit:
      {
	 VAR(FILE, out, par);
	 switch (np->type)
	 {
	 case CONST_NUMBER:
	    {
	       char *s = strchr(np->val, '.');

	       if (!s)
		  s = strchr(np->val, 'e');
	       if (!s)
		  s = strchr(np->val, 'E');
	       fprintfOffs(out, level, "{ { { NUMERIC_type_of_ClipVarType,0,0,0,0,0}, %s%s%s } }", np->node.isMinus ? "-" : "", np->val, s ? "" : ".0");
	    }
	    break;
	 case CONST_STRING:
	    fprintfOffs(out, level, "{ { { CHARACTER_type_of_ClipVarType,%d,0,F_MSTAT_ClipFlags,0 }, {", haveMacro(np->val));
	    printString(out, np->val);
	    fprintf(out, ", %ld } } }", (long) strlen(np->val));
	    break;
	 case CONST_NIL:
	    fprintfOffs(out, level, "{ { UNDEF_type_of_ClipVarType,0,0,0,0 } }");
	    break;
	 case CONST_LOGIC:
	    {
	       int val = np->no;

	       if (np->node.isMinus)
		  val = !val;
	       fprintfOffs(out, level, "{ { { LOGICAL_type_of_ClipVarType, 0,0,0,0 }, %s } }", val ? "1" : "0");
	    }
	    break;
	 }
      }
      break;
   case CalcDeep:
      {
	 VAR(Function, fp, par);
	 addDeep(fp, 1);
      }
      break;
   case PrintSrc:
      {
	 VAR(CodestrNode, cp, par);
	 VAR(StrBuf, out, cp->buf);

	 switch (np->type)
	 {
	 case CONST_NUMBER:
	    putString_StrBuf(out, np->val);
	    break;
	 case CONST_STRING:
#if 0
	    {
	       int lq = '"', rq = '"';

	       choose_quot(np->val, &lq, &rq, 1);
	       if (lq)
		  putByte_StrBuf(out, lq);
	       putString_StrBuf(out, np->val);
	       if (rq)
		  putByte_StrBuf(out, rq);
	    }
#endif
	    {
	       char *s = quot_str(np->val, 1);

	       putString_StrBuf(out, s);
	       free(s);
	    }
	    break;
	 case CONST_NIL:
	    putString_StrBuf(out, "NIL");
	    break;
	 case CONST_LOGIC:
	    if (np->no)
	       putString_StrBuf(out, ".T.");
	    else
	       putString_StrBuf(out, ".F.");
	    break;
	 }
      }
      return 0;
   case CText:
      {
	 VAR(FILE, out, par);
	 switch (np->type)
	 {
	 case CONST_NUMBER:
	    fprintfOffs(out, level, "_clip_push( _mp, &_num_%d );\n", np->no);
	    break;
	 case CONST_STRING:
	    fprintfOffs(out, level, "if ( (_ret=_clip_push(_mp, &_str_%d) )) goto _trap_%d;\n", np->no, np->node.seqNo);
	    break;
	 case CONST_NIL:
	    fprintfOffs(out, level, "_clip_push_nil( _mp );\n");
	    break;
	 case CONST_LOGIC:
	    if (np->no)
	       fprintfOffs(out, level, "_clip_push_true( _mp );\n");
	    else
	       fprintfOffs(out, level, "_clip_push_false( _mp );\n");
	    break;
	 }
      }
      break;
   case OText:
      {
	 VAR(StrBuf, out, par);
	 switch (np->type)
	 {
	 case CONST_NUMBER:
	    putByte_StrBuf(out, CLIP_PUSH_NUM);
	    putShort_StrBuf(out, np->no);
	    break;
	 case CONST_STRING:
	    putByte_StrBuf(out, CLIP_PUSH_STR);
	    putShort_StrBuf(out, np->no);
	    break;
	 case CONST_NIL:
	    putByte_StrBuf(out, CLIP_PUSH_NIL);
	    break;
	 case CONST_LOGIC:
	    if (np->no)
	       putByte_StrBuf(out, CLIP_PUSH_TRUE);
	    else
	       putByte_StrBuf(out, CLIP_PUSH_FALSE);
	    break;
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
