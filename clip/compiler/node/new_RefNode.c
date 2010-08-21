Node *
new_RefNode(Node * expr)
{
   RefNode *ret;

   if (expr->isNil)
   {
      Node *r = new_Node();

      r->isNil = 1;
      r->isRef = 1;
      return r;
   }

   ret = NEW(RefNode);
   init_Node(ret, pass_RefNode, "ref");
   ret->node.isExpr = 1;
   ret->node.isLval = 0;
   ret->node.isConst = 0;
   if (expr->isConst)
      yyerror("reference to constant");

#if 0
   if (!strcmp(expr->name, "macro"))
   {
      yyerror("reference to macro not allowed");
   }
#endif

   if (strcmp(expr->name, "call"))
   {
      if (!expr->isLval && !expr->isField)
	 yyerror("reference to non-lvalue object");
      ret->expr = expr;
      ret->node.isRef = 1;
   }
   else
   {
      CallNode *np = (CallNode *) expr;

      if (np->argc)
	 yyerror("reference to function with args not allowed");
      else
	 ret->name = strdup(np->name);
   }

   return (Node *) ret;
}
