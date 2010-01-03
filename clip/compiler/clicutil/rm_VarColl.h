void
rm_VarColl(VarColl * coll, Var * var)
{
   int       no;

   if (search_Coll(&coll->coll, var, &no))
    {
      /*VAR(Var, vp, coll->coll.items[no]); */
       atRemove_Coll(&coll->coll, no);
      /*remove_Coll(&coll->unsorted, vp); */
    }
}
