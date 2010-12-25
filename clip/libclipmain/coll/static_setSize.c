static void
setSize(Coll * coll, int size)
{
   void **aItems;

   aItems = (void **) malloc(size * sizeof(void *));

	if (coll->count_of_Coll != 0)
		memcpy(aItems, coll->items_of_Coll, coll->count_of_Coll * sizeof(void *));

	free(coll->items_of_Coll);
	coll->items_of_Coll = aItems;
	coll->size_of_Coll = size;
}
