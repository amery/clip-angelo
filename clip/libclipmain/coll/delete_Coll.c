void
delete_Coll(void *coll)
{
   if (!coll)
      return;
   destroy_Coll((Coll *) coll);
   free(coll);
}
