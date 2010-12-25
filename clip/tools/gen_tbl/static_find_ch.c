static UniRecord *
find_ch(Coll * cs, unsigned char ch)
{
   int ind;

   UniRecord u;

   cons_CharsetEntry ce;

	u.cp_of_UniRecord = &ce;
	ce.ch_of_cons_CharsetEntry = ch;

   if (search_Coll(cs, &u, &ind))
		return (UniRecord *) cs->items_of_Coll[ind];
   else
      return 0;
}
