Node *
new_ArgNode(Node * expr, int byRef)
{
   NEWVAR(ExprNode, ret);

   init_Node(ret, pass_ArgNode, "arg");
   ret->expr = expr;
   ret->node.isExpr = 1;
   ret->node.isLval = expr->isLval;
   ret->node.isConst = expr->isConst;
   ret->i = byRef;
   if (byRef && expr->isConst)
      yyerror("reference to constant");
   if (byRef && !expr->isLval)
      yyerror("reference to temporary object");

   return (Node *) ret;
}
