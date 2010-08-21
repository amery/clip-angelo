static cons_CharsetEntry *
in_map(cons_CharsetEntry * cp, int len, unsigned long no)
{
   int i, ch;

   for (i = len - 1; i >= 0; i--)
   {
		if (no == cp[i].unich_of_cons_CharsetEntry)
      {
	 int found = i;

	 ch = cp[i].ch_of_cons_CharsetEntry;

	 for (i++; i < len; i++)
		 if (cp[i].ch_of_cons_CharsetEntry == ch)
	       return 0;

	 return cp + found;
      }
   }
   return 0;
}
