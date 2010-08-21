Node *
new_SwapNode(Node * expr1, Node * expr2)
{
   NEWVAR(OpNode, ret);

   init_Node(ret, pass_SwapNode, "swap");
   ret->node.isExpr = 1;

   ret->expr1 = expr1;
   ret->expr2 = expr2;
   ret->node.isConst = expr1->isConst;

   return (Node *) ret;
}
