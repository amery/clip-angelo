Node     *
new_ForeachNode(Node * var, Node * expr, Node * list, int keys)
{
   NEWVAR(ForNode, ret);

   init_Node(ret, pass_ForeachNode, "foreach");
   ret->node.line = var->line;
   ret->node.file = var->file;
   ret->init = expr;
   ret->var = var;
   ret->list = list;
	ret->labelNo = curFunction->labelNo_of_Function;
   ret->keys = keys;
	curFunction->labelNo_of_Function += 1;
	curFunction->haveIf_of_Function++;

   return (Node *) ret;
}
