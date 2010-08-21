Node *
new_AssignFmemvarNode(Node * var, Node * expr)
{
   AssignNode *ret;

   ret = NEW(AssignNode);
   init_Node(ret, pass_AssignFmemvarNode, "assignfmemvar");
   ret->node.isExpr = 1;

   ret->var = var;
   ret->expr = expr;
   ret->node.isAssign = 1;
   curFunction->goto_trap_of_Function++;

   return (Node *) ret;
}
