Node     *
new_AndNode(Node * expr1, Node * expr2)
{
   NEWVAR(OpNode, ret);

   init_Node(ret, pass_AndNode, "or");
   ret->expr1 = expr1;
   ret->expr2 = expr2;
	curFunction->goto_trap_of_Function++;
	ret->op = curFunction->labelNo_of_Function++;
	curFunction->haveIf_of_Function++;

   return (Node *) ret;
}
