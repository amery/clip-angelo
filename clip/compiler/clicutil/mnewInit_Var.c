Var *
mnewInit_Var(Node * macro, struct Node *expr)
{
   Var *ret = newInit_Var(macro_name(), expr);

   ret->macro = macro;
   return ret;
}
