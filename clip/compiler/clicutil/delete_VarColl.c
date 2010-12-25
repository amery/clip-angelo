void
delete_VarColl(VarColl * coll)
{
   if (!coll)
      return;
   destroy_Coll(&coll->coll);
   destroy_Coll(&coll->unsorted);
   free(coll);
}
