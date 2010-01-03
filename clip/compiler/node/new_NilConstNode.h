Node     *
new_NilConstNode()
{
   NEWVAR(ConstNode, ret);
   init_Node(ret, pass_ConstNode, "const");
   ret->type = CONST_NIL;
   ret->func = curFunction;
   ret->node.isConst = 1;
   ret->node.isExpr = 1;
   ret->node.isNil = 1;
	curFunction->goto_trap_of_Function++;
   curFile->haveNil_of_File++;
   return (Node *) ret;
}
