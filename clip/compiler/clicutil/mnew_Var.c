Var *
mnew_Var(Node * macro)
{
   Var *ret = new_Var(macro_name());

   ret->macro = macro;
   return ret;
}
