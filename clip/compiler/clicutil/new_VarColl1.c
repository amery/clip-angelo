VarColl *
new_VarColl1(Var * vp)
{
   NEWVAR(VarColl, ret);
   init_Coll(&ret->coll, delete_Var, compare_Var);
   init_Coll(&ret->unsorted, 0, 0);
   add_VarColl(ret, vp);
   return ret;
}
