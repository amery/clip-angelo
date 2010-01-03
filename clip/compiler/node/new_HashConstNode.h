Node     *
new_HashConstNode(char *name)
{
   char      buf[32];

   snprintf(buf, sizeof(buf), "%ld", hashstr(name));
	curFunction->goto_trap_of_Function++;
   ins_name(name);
   return new_NumberConstNode(strdup(buf), name);
}
