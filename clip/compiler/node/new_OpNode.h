Node     *
new_OpNode(Node * expr1, Node * expr2, int op)
{
   NEWVAR(OpNode, ret);

   init_Node(ret, pass_OpNode, "op");
   ret->expr1 = expr1;
   ret->expr2 = expr2;
   ret->op = op;
	curFunction->goto_trap_of_Function++;

   return (Node *) ret;
}
