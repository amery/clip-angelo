Var      *
newArr_Var(char *name, Coll * arr)
{
   Var      *ret = new_Var(name);

   ret->arr = arr;
   return ret;
}
