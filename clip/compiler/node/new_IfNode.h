Node     *
new_IfNode(Coll * elseifs, Node * elselist)
{
   NEWVAR(IfNode, ret);
   Node     *expr = 0;

   if (elseifs && elseifs->count)
      expr = (Node *) elseifs->items[0];

   init_Node(ret, pass_IfNode, "if");
   if (expr)
    {
       ret->node.line = expr->line;
       ret->node.file = expr->file;
    }

   ret->elseifs = elseifs;
   ret->elselist = elselist;
	ret->labelNo = curFunction->labelNo_of_Function;
	curFunction->labelNo_of_Function += (elseifs->count / 2 + 1);
	curFunction->goto_trap_of_Function++;
	curFunction->haveIf_of_Function++;

   return (Node *) ret;
}
