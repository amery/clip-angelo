static void
make_utbl(unsigned short *utbl, cons_CharsetEntry * cs, int len)
{
   int i, j;

   for (i = 0; i < 128; i++)
      utbl[i] = i;

   for (i = 128; i < 256; i++)
   {
      utbl[i] = 0;

      for (j = 0; j < len; j++)
      {
			if (cs[j].ch_of_cons_CharsetEntry == i)
	 {
		 utbl[i] = cs[j].unich_of_cons_CharsetEntry;
	    break;
	 }
      }
   }
}
