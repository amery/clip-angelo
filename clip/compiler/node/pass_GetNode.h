int
pass_GetNode(void *self, Pass pass, int level, void *par)
{
   VAR(ObjNode, np, self);
   switch (pass)
    {
    case Print:
       fprintfOffs(stdout, level, "GetNode: %s\n", np->name);
       break;
    case CalcDeep:
       {
	  VAR(Function, fp, par);
	  int       deep, ret;

	  addDeep(fp, 1);
	  deep = fp->deep_of_Function;
	  ret = np->obj->pass(np->obj, pass, level, par);
	  fp->deep_of_Function = deep;
	  return ret;
       }
       break;
    case CText:
       {
	  VAR(FILE, out, par);
	  fprintfOffs(out, level, "_clip_push_nil( _mp );\n");
	  np->obj->pass(np->obj, pass, level, par);
	  fprintfOffs(out, level,
		      "if ( (_ret=_clip_get(_mp, %ld /* %s */))) goto _trap_%d;\n",
		      (long) hashstr(np->name), np->name, np->node.seqNo);
	  return 0;
       }
       break;
    case OText:
       {
	  VAR(StrBuf, out, par);
	  putByte_StrBuf(out, CLIP_PUSH_NIL);
	  np->obj->pass(np->obj, pass, level, par);
	  putByte_StrBuf(out, CLIP_GET);
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

   return np->obj->pass(np->obj, pass, level + 1, par);
}
