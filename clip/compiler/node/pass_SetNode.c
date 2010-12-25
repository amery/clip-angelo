int
pass_SetNode(void *self, Pass pass, int level, void *par)
{
   VAR(ObjNode, np, self);
   switch (pass)
   {
   case Print:
      fprintfOffs(stdout, level, "SetNode: %s\n", np->name);
      break;
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
	 pass_Node(self, pass, level - 1, par);
	 np->obj->pass(np->obj, pass, level, par);
	 fprintfOffs(out, level, "if ( (_ret=_clip_set(_mp, %ld /* %s */))) goto _trap_%d;\n", (long) hashstr(np->name), np->name, np->node.seqNo);
	 return 0;
      }
      break;
   case OText:
      {
	 VAR(StrBuf, out, par);
	 putByte_StrBuf(out, CLIP_PUSH_NIL);
	 np->obj->pass(np->obj, pass, level, par);
	 pass_Node(self, pass, level - 1, par);
	 putByte_StrBuf(out, CLIP_SET);
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
