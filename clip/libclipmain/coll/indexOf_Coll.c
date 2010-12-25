int
indexOf_Coll(Coll * coll, void *item, int *index)
{
   int i;

	for (i = coll->count_of_Coll - 1; i >= 0; i--)
		if (coll->items_of_Coll[i] == item)
      {
	 if (index)
	    *index = i;
	 return 1;
      }
   return 0;
}
