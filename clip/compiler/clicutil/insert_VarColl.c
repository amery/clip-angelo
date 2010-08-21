void
insert_VarColl(VarColl * coll, Var * var)
{
   int no;

   if (!var)
      return;
   if (search_Coll(&coll->coll, var, &no))
   {
      return;
   }

   var->no = coll->coll.count_of_Coll;
   insert_Coll(&coll->coll, var);
   insert_Coll(&coll->unsorted, var);
}
