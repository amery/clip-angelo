static int
search_long(long *coll, int count, long key)
{
   int l, h, i;

   long c;

   l = 0;
   h = count - 1;
   while (l <= h)
   {
      i = (l + h) >> 1;
      if ((c = GETLONG(coll + i)) < key)
	 l = i + 1;
      else
      {
	 if (c == key)
	    return i;
	 h = i - 1;
      }
   }
   return -1;
}
