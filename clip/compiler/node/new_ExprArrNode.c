Node *
new_ExprArrNode()
{
   NEWVAR(ExprArrNode, ret);

   init_Node(ret, pass_ExprArrNode, "exprarr");
   ret->node.isExpr = 1;
   ret->node.isExprList = 1;
   init_Coll(&(ret->coll), 0, 0);

   return (Node *) ret;
}
