Node *
new_MethodNode(Node * obj, char *name, Coll * argv, int rest)
{
   int i;

   char *s;

   NEWVAR(ObjNode, ret);

   init_Node(ret, pass_MethodNode, "method");
	for (i = 0; i < argv->count_of_Coll; i++)
   {
		VAR(Node, np, argv->items_of_Coll[i]);
      append_Node(&ret->node, np);
   }
   ret->name = name;
   for (s = ret->name; *s; ++s)
      *s = toupper(*s);
   ret->hash = hashstr(name);
	ret->argc = argv->count_of_Coll + 1;
   ret->obj = obj;
   ret->rest = rest;
   ins_name(name);

   delete_Coll(argv);
   curFunction->goto_trap_of_Function++;

   return (Node *) ret;
}
