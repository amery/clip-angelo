Node *
new_LangNode(char *ctext)
{
   NEWVAR(LangNode, ret);

   init_Node(ret, pass_LangNode, "lang");
   ret->ctext = ctext;
   curFunction->goto_trap_of_Function++;

   return (Node *) ret;
}
