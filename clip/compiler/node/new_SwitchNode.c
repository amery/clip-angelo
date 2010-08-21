Node *
new_SwitchNode(Node * expr, Coll * cases, Node * other)
{
   NEWVAR(SwitchNode, ret);
   Coll *coll;

   int i;

   init_Node(ret, pass_SwitchNode, "switch");

   ret->expr = expr;
   ret->other = other;

   coll = new_Coll(0, cmp_label);

	for (i = 0; i < cases->count_of_Coll; i++)
   {
      int j;

		VAR(SwitchEl, ep, cases->items_of_Coll[i]);

		for (j = 0; j < ep->labels_of_SwitchEl->count_of_Coll; j++)
      {
			VAR(Node, cp, ep->labels_of_SwitchEl->items_of_Coll[j]);
	 int ind;

	 NEWVAR(CaseLabel, lp);

	 lp->hash_of_CaseLabel = hash_const(cp);
	 lp->SwitchEl_ep_of_CaseLabel = ep;
	 lp->Node_cp_of_CaseLabel = cp;

	 if (search_Coll(coll, lp, &ind))
	 {
	    char buf[64], buf1[64];

		 VAR(CaseLabel, cpp, coll->items_of_Coll[ind]);
		 yyerror("CASE label duplication: '%s' at line %d and '%s' at line %d", str_const(cp, buf, sizeof(buf)), cp->line, str_const(cpp->Node_cp_of_CaseLabel, buf1, sizeof(buf1)),
cpp->Node_cp_of_CaseLabel->line);
	    free(lp);
	 }
	 else
	    insert_Coll(coll, lp);
      }
   }

   ret->cases = cases;
   ret->labels = coll;

   return (Node *) ret;
}
