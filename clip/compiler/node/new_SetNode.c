Node *
new_SetNode(Node * obj, char *name, Node * expr)
{
   char *s;

   NEWVAR(ObjNode, ret);

   init_Node(ret, pass_SetNode, "set");
   ret->name = name;
   for (s = ret->name; *s; ++s)
      *s = toupper(*s);
   ret->hash = hashstr(name);
   ret->argc = 0;
   ret->obj = obj;
   append_Node(&ret->node, expr);
   curFunction->goto_trap_of_Function++;
   ins_name(name);

   return (Node *) ret;
}
