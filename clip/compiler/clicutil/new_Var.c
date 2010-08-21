Var *
new_Var(char *name)
{
   char *s;

   NEWVAR(Var, ret);
   for (s = name; (*s); s++)
      *s = toupper(*s);
   ret->name = name;
   ret->no = -1;
   ret->line = cl_line;
   ret->pos = clic_pos;
   ret->file = currentFile();
   ret->func = curFunction;
   return ret;
}
