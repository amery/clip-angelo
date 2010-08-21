Node *
new_CallNameNode(Node * name, Coll * argv)
{
   int i;

   NEWVAR(CallNameNode, ret);
   init_Node(ret, pass_CallNameNode, "callname");
	for (i = 0; i < argv->count_of_Coll; i++)
   {
		VAR(Node, np, argv->items_of_Coll[i]);
      append_Node(&ret->node, np);
   }
   ret->name = name;
	ret->argc = argv->count_of_Coll;
   curFunction->goto_trap_of_Function++;
   delete_Coll(argv);
   return (Node *) ret;
}
