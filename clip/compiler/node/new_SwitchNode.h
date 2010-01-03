Node     *
new_SwitchNode(Node * expr, Coll * cases, Node * other)
{
   NEWVAR(SwitchNode, ret);
   Coll     *coll;

   int       i;

   init_Node(ret, pass_SwitchNode, "switch");

   ret->expr = expr;
   ret->other = other;

   coll = new_Coll(0, cmp_label);

   for (i = 0; i < cases->count; i++)
    {
       int       j;

       VAR(SwitchEl, ep, cases->items[i]);

		 for (j = 0; j < ep->labels_of_SwitchEl->count; j++)
	{
		VAR(Node, cp, ep->labels_of_SwitchEl->items[j]);
	   int       ind;

	   NEWVAR(CaseLabel, lp);

	   lp->hash = hash_const(cp);
	   lp->ep = ep;
	   lp->cp = cp;

	   if (search_Coll(coll, lp, &ind))
	    {
	       char      buf[64], buf1[64];

	       VAR(CaseLabel, cpp, coll->items[ind]);
	       yyerror
		("CASE label duplication: '%s' at line %d and '%s' at line %d",
		 str_const(cp, buf, sizeof(buf)), cp->line, str_const(cpp->cp, buf1, sizeof(buf1)), cpp->cp->line);
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
