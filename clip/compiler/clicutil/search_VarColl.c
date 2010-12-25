int
search_VarColl(VarColl * coll, char *name, int *index)
{
   Var var;

   var.name = name;
   return search_Coll(&coll->coll, &var, index);
}
