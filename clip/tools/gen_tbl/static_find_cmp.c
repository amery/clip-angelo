static int
find_cmp(Coll * cp, int ch)
{
   int i;

	for (i = 0; i < cp->count_of_Coll; i++)
   {
		UniRecord *rp = (UniRecord *) cp->items_of_Coll[i];

		if (rp->cp_of_UniRecord->ch_of_cons_CharsetEntry == ch)
	 return i;
   }
   return 0;
}
