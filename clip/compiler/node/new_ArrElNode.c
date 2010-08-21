Node *
new_ArrElNode(Node * arr, Node * expr, int op)
{
   NEWVAR(ArrElNode, ret);
   Node *oarr;

   init_Node(ret, pass_ArrElNode, "arrel");

   first_List(&arr->list);
	oarr = (Node *) arr->list.current_of_List;
   if (!strcmp(oarr->name, "fmemvar"))
   {
      Node *narr = new_MemvarNode(((VarNode *) oarr)->vp);

      narr->line = oarr->line;
      narr->file = oarr->file;

      remove_List(&arr->list);
      prepend_Node(arr, narr);
   }

   ret->oarr = oarr;
   ret->arr = arr;
   ret->expr = expr;
   ret->dim = getCount_Node(arr) - 1;
   ret->op = op;
   curFunction->goto_trap_of_Function++;
   if (ret->expr)
      ret->node.isAssign = 1;
   ret->node.isArrEl = 1;
   if (!expr)
      ret->node.isLval = 1;

   return (Node *) ret;
}
