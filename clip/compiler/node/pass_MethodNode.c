int
pass_MethodNode(void *self, Pass pass, int level, void *par)
{
   VAR(ObjNode, np, self);
   switch (pass)
   {
   case Print:
      fprintfOffs(stdout, level, "MethodNode: %s (%d)\n", np->name, np->argc);
      break;
   case PrintSrc:
      {
	 VAR(CodestrNode, cp, par);
	 VAR(StrBuf, out, cp->buf);
	 VAR(Node, npp, self);
	 int r, n = 0;

	 np->obj->pass(np->obj, pass, level, par);
	 putString_StrBuf(out, ":");
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
   case CalcDeep:
      {
	 VAR(Function, fp, par);
	 int deep, ret;

	 addDeep(fp, 1);
	 deep = fp->deep_of_Function;
	 np->obj->pass(np->obj, pass, level, par);
	 ret = pass_Node(self, pass, level, par);
	 fp->deep_of_Function = deep;
	 return ret;
      }
      break;
   case CText:
      {
	 VAR(FILE, out, par);
	 fprintfOffs(out, level, "_clip_push_nil( _mp );\n");
	 np->obj->pass(np->obj, pass, level, par);
	 pass_Node(self, pass, level - 1, par);
	 fprintfOffs(out, level, "if ( (_ret=_clip_call(_mp, %d, %ld /* %s */))) goto _trap_%d;\n", np->argc, (long) hashstr(np->name), np->name, np->node.seqNo);
	 return 0;
      }
      break;
   case OText:
      {
	 VAR(StrBuf, out, par);
	 putByte_StrBuf(out, CLIP_PUSH_NIL);
	 np->obj->pass(np->obj, pass, level, par);
	 pass_Node(self, pass, level - 1, par);
	 putByte_StrBuf(out, CLIP_CALL);
	 putByte_StrBuf(out, np->argc);
	 putLong_StrBuf(out, hashstr(np->name));
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

   np->obj->pass(np->obj, pass, level, par);
   return pass_Node(self, pass, level, par);
}
