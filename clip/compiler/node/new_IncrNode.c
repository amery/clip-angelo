Node *
new_IncrNode(Node * expr, int isIncr, int isPost)
{
   IncrNode *ret;

   if (expr->isArrEl)
   {
      Node *np;

      ArrElNode *ap = (ArrElNode *) expr;

      /* ap->op = 'I'; */
      np = new_OpNode(expr, new_NumberConstNode(strdup("1"), 0), isIncr ? '+' : '-');

      return new_ArrElNode(ap->arr, np, '=');
   }

   if (expr->isField)
   {
      Node *np;

      if (isPost)
      {
	 np = new_OpNode(expr, new_NumberConstNode(strdup("1"), 0), isIncr ? '+' : '-');
	 np = new_AssignNode(expr, np, '=');
	 np = new_SwapNode(expr, np);
      }
      else
      {
	 np = new_OpNode(expr, new_NumberConstNode(strdup("1"), 0), isIncr ? '+' : '-');
	 np = new_AssignNode(expr, np, '=');
      }
      return np;
   }

   ret = NEW(IncrNode);
   init_Node(ret, pass_IncrNode, "incr");
   ret->expr = expr;
   ret->isIncr = isIncr;
   ret->isPost = isPost;
   if (!expr->isLval && !expr->isArrEl /*&& !expr->isField */ )
      yyerror("INCR with non-lvalue");

   return (Node *) ret;
}
