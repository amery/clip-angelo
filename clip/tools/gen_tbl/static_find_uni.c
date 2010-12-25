static UniRecord *
find_uni(Coll * uni, unsigned long unich)
{
   int ind;

   UniRecord u;

	u.no_of_UniRecord = unich;

   if (search_Coll(uni, &u, &ind))
		return (UniRecord *) uni->items_of_Coll[ind];
   else
      return 0;
}
