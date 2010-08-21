Var *
mnewArr_Var(Node * macro, Coll * arr)
{
   Var *ret = newArr_Var(macro_name(), arr);

   ret->macro = macro;
   return ret;
}
