static int
pass_CallNameNode(void *self, Pass pass, int level, void *par)
{
   VAR(CallNameNode, np, self);
   switch (pass)
   {
   case Print:
      fprintfOffs(stdout, level, "CallNameNode:(%d)\n", np->argc);
      break;
   case CalcDeep:
      {
	 VAR(Function, fp, par);
	 int deep;

	 np->name->pass(np->name, pass, level, par);
	 addDeep(fp, 1);
	 deep = fp->deep_of_Function;
	 pass_Node(self, pass, level, par);
	 fp->deep_of_Function = deep;
	 return 0;
      }
      break;
   case CText:
      {
	 VAR(FILE, out, par);
	 np->name->pass(np->name, CTextLval, level, par);
	 fprintfOffs(out, level, "_clip_push_nil( _mp );\n");
	 pass_Node(self, pass, level - 1, par);
	 fprintfOffs(out, level, "if ((_ret=_clip_func_name(_mp, %d))) goto _trap_%d;\n", np->argc, np->node.seqNo);
	 return 0;
      }
      break;
   case OText:
      {
	 VAR(StrBuf, out, par);
	 np->name->pass(np->name, OTextLval, level, par);
	 putByte_StrBuf(out, CLIP_PUSH_NIL);
	 pass_Node(self, pass, level - 1, par);
	 putByte_StrBuf(out, CLIP_FUNC_NAME);
	 putByte_StrBuf(out, np->argc);
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
   np->name->pass(np->name, pass, level, par);

   return pass_Node(self, pass, level, par);
}
