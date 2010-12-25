int
search_map(ClipVarEl * items, int count, long key, int *index)
{
   int l, h, res, i, c;

   long ln;

   l = 0;
   h = count - 1;
   res = 0;
   while (l <= h)
   {
      i = (l + h) >> 1;
      ln = items[i].no_of_ClipVarEl;
      if (ln < key)
	 c = -1;
      else if (ln > key)
	 c = 1;
      else
	 c = 0;

      if (c < 0)
	 l = i + 1;
      else
      {
	 h = i - 1;
	 if (c == 0)
	 {
	    res = 1;
	    l = i;
	 }
      }
   }
   if (index)
      *index = l;
   return res;
}
