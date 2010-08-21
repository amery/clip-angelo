int
search_Coll(Coll * coll, void *key, int *index)
{
   /*void *p; */
   int l, h, res, i, c;

   if (!coll->compare)
      return 0;
   l = 0;
	h = coll->count_of_Coll - 1;
   res = 0;
   while (l <= h)
   {
      i = (l + h) >> 1;
		c = coll->compare(coll->items_of_Coll[i], key);
      if (c < 0)
	 l = i + 1;
      else
      {
	 h = i - 1;
	 if (c == 0)
	 {
	    res = 1;
		 if (!coll->duplicates_of_Coll)
	       l = i;
	 }
      }
   }
   if (index)
      *index = l;
   return res;
}
