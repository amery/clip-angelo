Node     *
new_StringConstNode(char *val)
{
   int       no;

//#define NEWVAR(type,var) type *var=(type*)calloc(sizeof(type),1)
//      NEWVAR(ConstNode, ret);

   ConstNode *ret = (ConstNode *) calloc(sizeof(ConstNode), 1);

   init_Node(ret, pass_ConstNode, "const");
   ret->type = CONST_STRING;
   ret->val = val;
   ret->len = strlen(val);
   if (search_Coll(&curFile->Coll_strings_of_File, ret, &no))
    {
       VAR(ConstNode, np, curFile->Coll_strings_of_File.items[no]);
       ret->no = np->no;
    }
   else
    {
       ret->no = curFile->Coll_strings_of_File.count;
       insert_Coll(&curFile->Coll_strings_of_File, ret);
       insert_Coll(&curFile->Coll_unsortedStrings_of_File, ret);
    }
   ret->func = curFunction;
   ret->node.isConst = 1;
   ret->node.isExpr = 1;
   ret->node.isStr = 1;
	curFunction->goto_trap_of_Function++;
   return (Node *) ret;
}
