Node *
new_ArrayInitNodeN(Node * exprlist, int n)
{
   NEWVAR(ExprNode, ret);

   init_Node(ret, pass_ArrayInitNode, "arrayinit");
   ret->node.isExpr = 1;

   ret->i = n;
   ret->expr = exprlist;
   ret->node.isConst = exprlist->isConst;
   ret->node.isArray = 1;

   return (Node *) ret;
}
