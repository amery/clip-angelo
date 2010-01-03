Node     *
new_CallNameNode(Node * name, Coll * argv)
{
   int       i;

   NEWVAR(CallNameNode, ret);
   init_Node(ret, pass_CallNameNode, "callname");
   for (i = 0; i < argv->count; i++)
    {
       VAR(Node, np, argv->items[i]);
       append_Node(&ret->node, np);
    }
   ret->name = name;
   ret->argc = argv->count;
	curFunction->goto_trap_of_Function++;
   delete_Coll(argv);
   return (Node *) ret;
}
