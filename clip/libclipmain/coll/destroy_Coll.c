void
destroy_Coll(Coll * coll)
{
   freeAll_Coll(coll);
	free(coll->items_of_Coll);
}
