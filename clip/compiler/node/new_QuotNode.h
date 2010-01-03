Node     *
new_QuotNode(Node * expr)
{
   NEWVAR(ExprNode, ret);

   init_Node(ret, pass_QuotNode, "quot");

   ret->expr = expr;
   ret->node.isExpr = 1;

   return (Node *) ret;
}
