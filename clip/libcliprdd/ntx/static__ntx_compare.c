static int
_ntx_compare(RDD_DATA * rd, char unique, char descend, char *s1, int l1, unsigned int recno1, char *s2, int l2, unsigned int recno2)
{
   int l = (l1 < l2 ? l1 : l2);

   int r = loc_cmp(rd->loc, (unsigned char *) s1, (unsigned char *) s2, l);

   if (!r)
   {
      if (unique || recno1 == 0 || recno2 == 0)
	 return 0;
      if (recno1 < recno2)
	 r = -1;
      else if (recno1 > recno2)
	 r = 1;
      else
	 r = 0;
   }
   if (descend)
      r = -r;
   return r;
}
