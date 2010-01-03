Node     *
new3_CallNode(const char *name, Node * expr1, Node * expr2, Node * expr3)
{
   char     *s;

   int       argc = 0;

   NEWVAR(CallNode, ret);

   init_Node(ret, pass_CallNode, "call");
   if (expr1)
    {
       append_Node(&ret->node, expr1);
       argc++;
    }
   if (expr2)
    {
       append_Node(&ret->node, expr2);
       argc++;
    }
   if (expr3)
    {
       append_Node(&ret->node, expr3);
       argc++;
    }
   ret->name = strdup(name);
   for (s = ret->name; *s; ++s)
      *s = toupper(*s);
   ret->hash = hashstr(name);
   ins_name((char *) name);
   ret->argc = argc;
  /*ret->isC = 2; */
   ret->isC = 1;
   ret->cfunc = curFunction;
	curFunction->goto_trap_of_Function++;

   gettext_CallNode(ret);
   return (Node *) ret;
}
