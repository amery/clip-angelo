Node     *
new_OperExprNode(Node * node)
{
   NEWVAR(OperExprNode, ret);
   init_Node(ret, pass_OperExprNode, "operexpr");
   ret->expr = node;
   return (Node *) ret;
}
