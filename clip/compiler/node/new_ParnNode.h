Node     *
new_ParnNode(Node * expr)
{
   NEWVAR(ExprNode, ret);

   init_Node(ret, pass_ParnNode, "parn");
   ret->expr = expr;

   return (Node *) ret;
}
