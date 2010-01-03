Node     *
new_NotNode(Node * expr)
{
   NEWVAR(ExprNode, ret);

   init_Node(ret, pass_NotNode, "not");
   ret->expr = expr;
   ret->node.isConst = expr->isConst;
   expr->isMinus = 1;
   ret->node.isNum = expr->isNum;
   ret->node.isStr = expr->isStr;
   ret->node.isLog = expr->isLog;

   return (Node *) ret;
}
