Node *
new_NumberConstNode(char *val, char *rval)
{
   int no;

   NEWVAR(ConstNode, ret);

   init_Node(ret, pass_ConstNode, "const");
   ret->type = CONST_NUMBER;
   ret->val = val;
   ret->rval = rval;
   if (search_Coll(&curFile->Coll_numbers_of_File, ret, &no))
   {
		VAR(ConstNode, np, curFile->Coll_numbers_of_File.items_of_Coll[no]);
      ret->no = np->no;
   }
   else
   {
		ret->no = curFile->Coll_numbers_of_File.count_of_Coll;
      insert_Coll(&curFile->Coll_numbers_of_File, ret);
      insert_Coll(&curFile->Coll_unsortedNumbers_of_File, ret);
   }
   ret->func = curFunction;
   ret->node.isConst = 1;
   ret->node.isExpr = 1;
   ret->node.isNum = 1;
   curFunction->goto_trap_of_Function++;
   return (Node *) ret;
}
