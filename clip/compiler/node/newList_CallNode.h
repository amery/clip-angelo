Node     *
newList_CallNode(char *name, Node * exprlist)
{
   char     *s;

   NEWVAR(CallNode, ret);

   init_Node(ret, pass_CallNode, "call");
   join_Node((Node *) ret, exprlist);
   ret->name = name;
   for (s = ret->name; *s; ++s)
      *s = toupper(*s);
   ret->hash = hashstr(name);
   ret->argc = getCount_Node((Node *) ret);
   ret->isC = 1;
   ret->cfunc = curFunction;
   ins_name(name);
	curFunction->goto_trap_of_Function++;

   gettext_CallNode(ret);
   return (Node *) ret;
}
