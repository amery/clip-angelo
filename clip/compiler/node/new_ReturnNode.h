Node     *
new_ReturnNode(Node * expr, int isExprList)
{
   NEWVAR(ExprNode, ret);

   init_Node(ret, pass_ReturnNode, "return");
   ret->expr = expr;
   ret->i = isExprList;

   return (Node *) ret;
}
