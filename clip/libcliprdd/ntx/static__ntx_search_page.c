static int
_ntx_search_page(RDD_DATA * rd, RDD_ORDER * ro, NTX_PAGE * page, char *key, int len, unsigned int recno, int *no, int exact)
{
   int l;

   int h;

   int i = 0;

   int c = 0;

   int count;

   int f = 0;

   NTX_BUCKET *buck;

   *no = 0;
   count = _rdd_ushort(page->nkeys);
   l = 0;
   h = count - 1;
   while (l <= h)
   {
      i = (l + h) / 2;
      buck = _ntx_buck(page, i);
      c = _ntx_compare(rd, ro->unique, ro->descend, (char *) buck->key, ro->keysize, exact ? _rdd_uint(buck->recno) : 0, key, len, exact ? recno : 0);
      if (c && f)
	 return 1;
      if (c < 0)
	 l = i + 1;
      else if (c > 0)
	 h = i - 1;
      else
      {
	 if (exact)
	 {
	    while (!c && i--)
	    {
	       buck = _ntx_buck(page, i);
	       c = _ntx_compare(rd, ro->unique, ro->descend, (char *) buck->key, ro->keysize, _rdd_uint(buck->recno), key, len, recno);
	    }
	    *no = i + 1;
	 }
	 else
	 {
	    *no = i;
	    f = 1;
	    h = i - 1;
	 }
	 return 1;
      }
   }
   if (c < 0)
      i++;
   *no = i;

   return 0;
}
