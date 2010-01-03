Node     *
new_CallNode(char *name, Coll * argv, int rest)
{
   int       i;

   char     *s;

   NEWVAR(CallNode, ret);
   init_Node(ret, pass_CallNode, "call");
   if (search_Coll(&reffuncs, name, 0))
      ret->byref = 1;
   for (i = 0; i < argv->count; i++)
    {
       VAR(Node, np, argv->items[i]);
       if (ret->byref && !np->isRef && !np->isConst && !np->isField && !np->isFMemvar && np->isLval && !np->isMacro)
	{
	   np = new_RefNode(np);
	}
       append_Node(&ret->node, np);
    }
   ret->name = name;
   for (s = ret->name; *s; ++s)
      *s = toupper(*s);
   ret->hash = hashstr(name);
   ret->argc = argv->count;
   ret->isC = 1;
   ret->rest = rest;
   ret->cfunc = curFunction;
	curFunction->goto_trap_of_Function++;

   ret->node.isAssign = 1;
   ins_name(name);

   delete_Coll(argv);
   gettext_CallNode(ret);
   return (Node *) ret;
}
