static int
hs_filter(ClipMachine * ClipMachineMemory, HIPER * hs, const char *val, int len, unsigned int **map, unsigned int lastrec, unsigned int *count, const char *__PROC__)
{
   int ints = (lastrec + 31) / 32;

   unsigned int *r = malloc(ints * 4);

   unsigned int *t = NULL;

   char *s = (char *) val;

   char *e = s + len - 1;

   unsigned char pair[2];

   unsigned int i;

   unsigned int offs;

   unsigned char c4[4];

   unsigned int bit;

   int er;

   memset(r, 0xff, ints * 4);
   while (s < e)
   {
      memcpy(pair, s, 2);
      if (hs->filtset == 1)
      {				/* text */
	 if (!isgraph(pair[0]) || !isgraph(pair[1]))
	 {
	    s++;
	    continue;
	 }
	 if (hs->lcase)
	 {			/* non case-sensitive text */
	    pair[0] = toupper(pair[0]);
	    pair[1] = toupper(pair[1]);
	 }
      }				/* otherwise binary */
      offs = sizeof(HS_HEADER) + _rdd_ushort(pair) * 4;
      if ((er = rdd_read(ClipMachineMemory, &hs->file, offs, 4, c4, __PROC__)))
	 goto err;
      if ((er = _hs_loadbits(ClipMachineMemory, hs, _rdd_uint(c4), lastrec, &t, __PROC__)))
	 goto err;
      for (i = 0; i < ints; i++)
	 r[i] &= t[i];
      free(t);
      t = NULL;
      s++;
   }

   *count = 0;
   for (i = 0; i < ints; i++)
   {
      while (i < ints - 1 && !r[i])
	 i++;
      bit = 1;
      *count += ((r[i] & bit) != 0);
      do
      {
	 bit = bit << 1;
	 *count += ((r[i] & bit) != 0);
      }
      while (!(bit & 0x80000000));
   }
   *map = r;
   return 0;
 err:
   free(r);
   if (t)
      free(t);
   return er;
}
