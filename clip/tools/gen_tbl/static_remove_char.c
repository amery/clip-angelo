static int
remove_char(Coll * cp, unsigned char ch)
{
   int i;

	for (i = 0; i < cp->count_of_Coll; i++)
   {
		UniRecord *rp = (UniRecord *) cp->items_of_Coll[i];

		if (rp->cp_of_UniRecord->ch_of_cons_CharsetEntry == ch)
      {
	 atRemove_Coll(cp, i);
	 return 1;
      }
   }
   return 0;
}
