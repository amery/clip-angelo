VarColl *
new_VarColl()
{
   NEWVAR(VarColl, ret);
   init_Coll(&ret->coll, delete_Var, compare_Var);
   init_Coll(&ret->unsorted, 0, 0);
   return ret;
}
