static int
pass_CodeNode(void *self, Pass pass, int level, void *par)
{
   VAR(CodeNode, np, self);
   switch (pass)
   {
   case Print:
		fprintfOffs(stdout, level, "CodeNode: %s: %d locals\n", np->func->name_of_Function, np->func->VarColl_locals_of_Function->unsorted.count_of_Coll);
      break;
   case TestConst:
      {
	 VAR(int, ip, par);

	 np->func->Node_body_of_Function->pass(np->func->Node_body_of_Function, TestConst, 0, ip);
	 /*(*ip)++; */
      }
      break;
   case CalcDeep:
      {
	 VAR(Function, fp, par);
	 addDeep(fp, 3);
      }
      break;

   case Codegen1:
      {
	 /*VAR(Function, fp, par); */
	 int i;

	 for (i = 0; i < np->func->VarColl_locals_of_Function->unsorted.count_of_Coll; i++)
	 {
		 VAR(Var, vp, np->func->VarColl_locals_of_Function->unsorted.items_of_Coll[i]);
	    Node *ip;

	    if (vp->init)
	       ip = vp->init;
	    else if (vp->arr)
	       ip = new_NewArrayNode(vp->arr);
	    else
	       ip = new_NilConstNode();

	    if (ip->isRef)
	    {
	       /*((RefNode*)ip) -> selfref = 1; */
	    }

	    prepend_Node((Node *) self, ip);
	 }

      }
      break;

   case CText:
      {
	 VAR(FILE, out, par);
	 pass_Node(self, pass, level + 1, par);
	 fprintfOffs(out, level, "_clip_push_func( _mp, &clip_%s, %d, 0 );\n", np->func->name_of_Function, np->func->VarColl_locals_of_Function->unsorted.count_of_Coll);
	 return 0;
      }

      break;
   case CTextInit:
      {
	 VAR(FILE, out, par);
	 fprintfOffs(out, level, "{ { {CCODE_t,0,0,0,0,0}, clip_%s} }", np->func->name_of_Function);
	 return 0;
      }
      break;
   case OText:
      {
	 VAR(StrBuf, out, par);
	 pass_Node(self, pass, level + 1, par);
	 putByte_StrBuf(out, CLIP_PUSH_CODE);
	 putShort_StrBuf(out, np->func->mno_of_Function);
	 putByte_StrBuf(out, np->func->VarColl_locals_of_Function->unsorted.count_of_Coll);
	 putByte_StrBuf(out, 0);
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

   pass_Node(self, pass, level + 1, par);
   return 0;
}
