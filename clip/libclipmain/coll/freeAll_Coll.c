void
freeAll_Coll(Coll * coll)
{
   int i;

   if (coll->_free)
		for (i = coll->count_of_Coll - 1; i >= 0; i--)
			coll->_free(coll->items_of_Coll[i]);
		coll->count_of_Coll = 0;
}
