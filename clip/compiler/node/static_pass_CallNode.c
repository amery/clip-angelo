static int
pass_CallNode(void *self, Pass pass, int level, void *par)
{
   VAR(CallNode, np, self);
   switch (pass)
   {
   case Print:
      fprintfOffs(stdout, level, "CallNode: %s (%d) byref=%d, isC=%d, rest=%d\n", np->name, np->argc, np->byref, np->isC, np->rest);
      break;
   case PrintSrc:
      {
	 VAR(CodestrNode, cp, par);
	 VAR(StrBuf, out, cp->buf);
	 VAR(Node, npp, self);
	 int r, n = 0;

	 putString_StrBuf(out, np->name);
	 putString_StrBuf(out, "(");
	 for (r = first_List(&npp->list); r; r = next_List(&npp->list))
	 {
		 VAR(Node, p, npp->list.current_of_List);
	    if (n)
	       putString_StrBuf(out, ", ");
	    n++;
	    if (p->pass(p, pass, level + 1, par))
	       break;
	 }
	 putString_StrBuf(out, ")");
      }
      return 0;
   case Codegen1:
      {
	 Node *npp = (Node *) np;

	 int r;

	 for (r = first_List(&npp->list); r; r = next_List(&npp->list))
	 {
		 VAR(Node, p, npp->list.current_of_List);
	    if (!strcmp(p->name, "ref"))
	    {
	       Node *np1 = ((RefNode *) p)->expr;

	       if (np1 && !strcmp(np1->name, "fmemvar"))
	       {
		  ((VarNode *) np1)->isArg = 1;
	       }
	    }
	 }
      }
      break;
   case CalcDeep:
      {
	 VAR(Function, fp, par);
	 int deep, ret;

	 np->isC = add_to_undeclared(np->name, &np->func);
	 addDeep(fp, 1);
	 deep = fp->deep_of_Function;
	 ret = pass_Node(self, pass, level, par);
	 fp->deep_of_Function = deep;
	 if (np->func && !np->func->isPublic_of_Function)
	    np->name = np->func->name_of_Function;
	 return ret;
      }
      break;
   case CText:
      {
	 VAR(FILE, out, par);
	 fprintfOffs(out, level, "_clip_push_nil( _mp );\n");
	 pass_Node(self, pass, level - 1, par);
	 switch (np->isC)
	 {
	 case 2:
	    fprintfOffs(out, level, "if ( (_ret=_clip_%s(_mp, clip_%s, %d, %d, %s))) goto _trap_%d;\n", np->node.isTop ? "proc" : "func", np->name, np->argc, np->rest, ref_name(np), np->node.seqNo);
	    break;
	 case 1:
	 case 0:
	    fprintfOffs(out, level,
			"if ( (_ret=_clip_%s_hash(_mp, %ld /* %s */, %d, %d, %s))) goto _trap_%d;\n",
			np->node.isTop ? "proc" : "func", (long) hashstr(np->name), np->name, np->argc, np->rest, ref_name(np), np->node.seqNo);
	    break;
	 }
	 return 0;
      }
      break;
   case OText:
      {
	 VAR(StrBuf, out, par);
	 putByte_StrBuf(out, CLIP_PUSH_NIL);
	 pass_Node(self, pass, level - 1, par);
	 switch (np->isC)
	 {
	 case 2:
	    if (np->node.isTop)
	       putByte_StrBuf(out, np->rest ? CLIP_SPROCR : CLIP_SPROC);
	    else
	       putByte_StrBuf(out, np->rest ? CLIP_SFUNCR : CLIP_SFUNC);
	    putByte_StrBuf(out, np->argc);
	    putShort_StrBuf(out, np->func->mno_of_Function);
	    break;
	 case 1:
	 case 0:
	    if (np->node.isTop)
	       putByte_StrBuf(out, np->rest ? CLIP_PROCR : CLIP_PROC);
	    else
	       putByte_StrBuf(out, np->rest ? CLIP_FUNCR : CLIP_FUNC);
	    putByte_StrBuf(out, np->argc);
	    putLong_StrBuf(out, hashstr(np->name));
	    break;
	 }
	 return 0;
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

   return pass_Node(self, pass, level, par);
}
