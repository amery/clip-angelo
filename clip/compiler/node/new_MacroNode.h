Node     *
new_MacroNode(Node * expr, int isexpr)
{
   NEWVAR(ExprNode, ret);

   init_Node(ret, pass_MacroNode, "macro");
   ret->expr = expr;
   ret->node.isMacro = 1;
   ret->node.isExpr = expr->isExpr;
   ret->node.isLval = 1;
   ret->i = isexpr;
   if (!strcmp(expr->name, "fmemvar"))
      ((VarNode *) expr)->p1 = 1;

   return (Node *) ret;
}
