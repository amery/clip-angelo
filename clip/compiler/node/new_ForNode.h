Node     *
new_ForNode(Node * var, Node * from, Node * to, Node * step, Node * list)
{
   NEWVAR(ForNode, ret);

   init_Node(ret, pass_ForNode, "for");
   ret->node.line = from->line;
   ret->node.file = from->file;
   ret->to = to;
   ret->step = step;
   ret->init = new_AssignNode(var, from, '=');
   ret->var = new_AssignNode(var, step, '+');
   ret->list = list;
	ret->labelNo = curFunction->labelNo_of_Function;
	curFunction->labelNo_of_Function += 1;
	curFunction->haveIf_of_Function++;

   return (Node *) ret;
}
