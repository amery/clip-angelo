Node *
new_CGetNode(Coll * argv)
{
   Node *np, *ip, *sp, *ap;

	if (argv->count_of_Coll < 1)
      goto ret;

	np = (Node *) argv->items_of_Coll[0];

   if (np->isConst)
   {
      yyerror("cannot GET constant");
      goto ret;
   }

   if (np->isArrEl)
   {
      int r, n;

      Node *npp, *ep;

      ap = npp = ((ArrElNode *) np)->arr;
      ep = new_ExprArrNode();

      for (n = 0, r = first_List(&npp->list); r; r = next_List(&npp->list), n++)
      {
			VAR(Node, p, npp->list.current_of_List);
	 if (!n)
	    ap = p;
	 else
	    append_Coll(&(((ExprArrNode *) ep)->coll), p);
      }

      ip = new_ArrayInitNodeN(ep, ((ExprArrNode *) ep)->coll.count_of_Coll);
   }
   else
   {
      ap = np;
      ip = new_NilConstNode();
   }

   if (ap->isMemvar)
      sp = new_StringConstNode(strdup(((VarNode *) ap)->vp->name));
   else if (ap->isMacro)
   {
      ExprNode *ep = (ExprNode *) ap;

      sp = ep->expr;
   }
   else if (ap->isField)
   {
#if 1
      sp = new_CodestrNode(ap, 0, 0);
#else
      StrBuf *bp;

      CodestrNode cn;

      bp = new_StrBuf();
      memset(&cn, 0, sizeof(cn));
      cn.buf = bp;
      init_Coll(&cn.coll, 0, 0);
      ap->pass(ap, PrintSrc, 0, &cn);
      flush_codestr(&cn, 0);
      putByte_StrBuf(bp, 0);
      sp = new_StringConstNode(strdup(bp->buf));
      destroy_Coll(&cn.coll);
      delete_StrBuf(bp);
#endif
   }
   else
      sp = new_NilConstNode();

#if 1
   {
      Var *vp, *vp2;

      VarColl *vc, *vc2;

      Node *cp, *el, *ldp;

      Function *fp;

      Coll *icp;

      vp = new_Var(strdup("_1"));
      vc = new_VarColl();
      add_VarColl(vc, vp);

      if (!np->isRef)
	 np = new_RefNode(np);
      vp2 = newInit_Var(strdup("_P"), np);

      fp = curFunction;
      cp = new_CodeNode(vc);

      vc2 = new_VarColl();
      add_VarColl(vc2, vp2);
      ldp = new_LocalDefNode(vc2, 0, 0);
      vp2->isLocalRef = 1;
      /*prepend_Node(curFunction->body, ldp); */

      icp = new_Coll(NULL, NULL);
      append_Coll(icp, new_OpNode(new_LocalNode(vp), new_NilConstNode(), 'E'));
      append_Coll(icp, new_LocalNode(vp2));
      el = new_IfNode(icp, new_AssignNode(new_LocalNode(vp2), new_LocalNode(vp), '='));

      fin_Function(curFunction);
      append_Node(curFunction->Node_body_of_Function, new_ReturnNode(el, 0 /*1 */ ));

      curFunction = fp;
      np = cp;
   }
#else
   if (!np->isRef && !np->isConst	/*&& !np->isField
					   && np->isLval && !np->isMacro */ )
   {
      np = new_RefNode(np);
   }

   {
      Var *vp;

      char vname[64];

      snprintf(vname, sizeof(vname), "__local_get_%d", local_get_no++);
      vp = new_Var(strdup(vname));
      ins_name(vp->name);
      add_VarColl(curFunction->locals, vp);
      np = new_AssignNode(new_LocalNode(vp), np, '=');
   }
#endif

   argv->items_of_Coll[0] = np;

   atInsert_Coll(argv, sp, 1);
   atInsert_Coll(argv, ip, 1);

 ret:
   return new_CallNode(strdup("__CGET__"), argv, 0);
}
