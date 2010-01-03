Var      *
newInit_Var(char *name, Node * expr)
{
   Var      *ret = new_Var(name);

   ret->init = expr;
   return ret;
}
