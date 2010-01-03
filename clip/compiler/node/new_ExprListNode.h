Node     *
new_ExprListNode()
{
   NEWVAR(ExprListNode, ret);

   init_Node(ret, pass_ExprListNode, "exprlist");
   ret->node.isExpr = 1;
   ret->node.isExprList = 1;

   return (Node *) ret;
}
