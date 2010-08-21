Node *
new_ArrayInitNode(Node * exprlist)
{
   NEWVAR(ExprNode, ret);

   init_Node(ret, pass_ArrayInitNode, "arrayinit");
   ret->node.isExpr = 1;

   ret->i = getCount_Node(exprlist);
   ret->expr = exprlist;
   ret->node.isConst = exprlist->isConst;
   ret->node.isArray = 1;

   return (Node *) ret;
}
