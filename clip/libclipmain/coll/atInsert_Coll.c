int
atInsert_Coll(Coll * coll, void *item, int index)
{
	if (coll->count_of_Coll == coll->size_of_Coll)
   {
		int le = coll->count_of_Coll / 4;

      if (le < DELTA_SIZE)
	 le = DELTA_SIZE;
	 setSize(coll, coll->count_of_Coll + le);
   }
   if (index != coll->count_of_Coll)
		memmove(coll->items_of_Coll + index + 1, coll->items_of_Coll + index, (coll->count_of_Coll - index) * sizeof(void *));

	coll->count_of_Coll++;
	coll->items_of_Coll[index] = item;
   return index;
}
