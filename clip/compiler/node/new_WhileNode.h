Node     *
new_WhileNode(Node * expr, Node * list)
{
   NEWVAR(WhileNode, ret);

   init_Node(ret, pass_WhileNode, "while");
   ret->node.line = expr->line;
   ret->node.file = expr->file;
   ret->expr = expr;
   ret->list = list;
	ret->labelNo = curFunction->labelNo_of_Function;
	curFunction->labelNo_of_Function += 1;
	curFunction->haveIf_of_Function++;

   return (Node *) ret;
}
