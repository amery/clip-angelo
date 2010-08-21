static void
gettext_CallNode(CallNode * np)
{
   if (!strcasecmp(np->name, "_clic_gettext"))
   {
      Node *npp = (Node *) np;

      int r;

      char *msg = 0;

      Node *module = 0;

      int no = 0;

      for (r = first_List(&npp->list); r; r = next_List(&npp->list), no++)
      {
			VAR(Node, p, npp->list.current_of_List);
	 if (!strcmp(p->name, "const"))
	 {
	    ConstNode *cp = (ConstNode *) p;

	    if (cp->type == CONST_STRING)
	    {
	       if (no == 0)
		  msg = cp->val;
	    }
	 }
	 if (no == 1)
	    module = p;
      }
      if (msg && !module)
	 put_locale_string(msg);
      if (!module && no == 1)
      {
	 append_Node(&np->node, new_StringConstNode(strdup(CLIP_MODULE)));
	 np->argc++;
      }
   }
   else if (!strcasecmp(np->name, "_clic_ngettext"))
   {
      Node *npp = (Node *) np;

      int r;

      int no = 0;

      char *singular = 0;

      char *plural = 0;

      Node *module = 0;

      for (r = first_List(&npp->list); r; r = next_List(&npp->list), no++)
      {
			VAR(Node, p, npp->list.current_of_List);
	 if (!strcmp(p->name, "const"))
	 {
	    ConstNode *cp = (ConstNode *) p;

	    if (cp->type == CONST_STRING)
	    {
	       if (no == 0)
		  singular = cp->val;
	       else if (no == 1)
		  plural = cp->val;
	    }
	 }
	 if (no == 3)
	    module = p;
      }
      if (singular && plural /*&& !module */ )
	 put_locale_string_plural(singular, plural);
      if (!module && no == 3)
      {
	 append_Node(&np->node, new_StringConstNode(strdup(CLIP_MODULE)));
	 np->argc++;
      }
   }
}
