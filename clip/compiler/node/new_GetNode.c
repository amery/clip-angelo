Node *
new_GetNode(Node * obj, char *name)
{
   char *s;

   NEWVAR(ObjNode, ret);

   init_Node(ret, pass_GetNode, "get");
   ret->name = name;
   for (s = ret->name; *s; ++s)
      *s = toupper(*s);
   ret->hash = hashstr(name);
   ret->argc = 0;
   ret->obj = obj;
   curFunction->goto_trap_of_Function++;
   ins_name(name);

   return (Node *) ret;
}
