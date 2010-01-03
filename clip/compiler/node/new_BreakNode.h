Node     *
new_BreakNode(Node * expr, int isOper)
{
   NEWVAR(ExprNode, ret);

   init_Node(ret, pass_BreakNode, "break");
   ret->expr = expr;
   ret->i = isOper;
	curFunction->goto_trap_of_Function++;

   return (Node *) ret;
}
