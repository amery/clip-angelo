Node *
new_LogicConstNode(int val)
{
   NEWVAR(ConstNode, ret);
   init_Node(ret, pass_ConstNode, "const");
   ret->type = CONST_LOGIC;
   ret->no = val;
   ret->func = curFunction;
   ret->node.isConst = 1;
   ret->node.isExpr = 1;
   ret->node.isLog = 1;
   ret->node.isMinus = 0;
   if (val)
      curFile->haveTrue_of_File++;
   else
      curFile->haveFalse_of_File++;
   curFunction->goto_trap_of_Function++;
   return (Node *) ret;
}
